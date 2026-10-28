# Payment: hand notes (mock se pehle ye padho)

### Shuru

Kaam: wallet se wallet paisa (Arpan se dukaan), bank se wallet me daalna (top-up), aur wallet se bank me nikaalna (payout). Fail ho to paisa wapas aaye, aur har paisa ka hisaab mile. Fraud, kai currency aur chargeback scope ke bahar hain. ~10 crore user hain. Aam din kuch sau payment / sec, Diwali pe kuch hazaar. Sahi hona bheed se badi dikkat hai.

&nbsp;

**BANKING SE AAYA (yahan sirf ek line bolo)**

Wallet se wallet (`POST /pay { from, to, amount }` + Idempotency-Key header -> `{ status, txn_id }`) = dono wallet hamare hi DB me hain. To banking jaisa: SQL, ek transaction, idempotency key, `balance >= 500` check aur kaatna ek query me, aur ledger. Payment ka naya hissa ye hai: **paisa bahar (bank) se aana aur bahar jaana.**

**Key kab banti hai:** payment screen khulte hi banti hai. Cheque number jaisi.

**Owner check:** userId JWT se lo, body se nahi, warna 403.

&nbsp;

**1. PSP**

Arpan ko HDFC se wallet me 2000 daalne hain, aur HDFC hamara DB nahi hai. Beech me PSP (Razorpay / Juspay): hum order t-50 dete hain, Arpan UPI se pay karta hai, PSP batata hai SUCCESS, aur hum wallet +2000 karte hain. Har bank se khud nahi jude, kyunki usme NPCI ki manzoori aur PCI-DSS lagta, jo saalon ka kaam hai. PSP ek API deta hai aur thodi fees leta hai.

&nbsp;

**2. PENDING + TIMEOUT**

PSP 30 sec chup raha. "FAILED" dikhate to Arpan dobara daalta aur 4000 kat jaate. "SUCCESS" dikhate to nakli paisa aa jaata. Isliye jo pata nahi, wo **PENDING** hai: "2-3 min me pakka batayenge". Timeout 5 sec ka, warna Diwali pe saare thread PSP ke intezaar me phans jaate.

&nbsp;

**3. CIRCUIT BREAKER + DOOSRA PSP**

PSP-1 poora band hai.

**Breaker:** iska matlab: 1 min me 50% call fail hui to PSP-1 ko call hi band, 30 sec baad 1-2 call try karo. (Ye rate limiter wala fail-open nahi hai, yahan matlab PSP ko call rokna hai.)

**Doosra PSP:** naye top-up **PSP-2** se jaate hain.

**PENDING wala nahi:** par jo payment PSP-1 pe PENDING atka hai, wo **PSP-2 pe dobara nahi** jaata, kyunki ho sakta hai PSP-1 ne paisa kaat liya ho. Wo PSP-1 ke jawab se hi pakka hoga.

&nbsp;

**4. INITIATED PEHLE**

PSP ko call ke beech hamara server gira, to restart pe yaad nahi ki call gayi ya nahi. Isliye call se **pehle** DB me `t-50 = INITIATED` likho aur COMMIT karo.

**INITIATED likhne se pehle gira:** DB me kuch nahi, paisa kahin nahi gaya, app dobara bheje (wahi key).

**INITIATED likhne ke baad, PSP call se pehle / beech me gira:** row INITIATED pe padi hai, poochne wala job use uthata hai aur PSP se poochh ke aage badhata hai.

t-50 hi PSP ke liye idempotency key hai.

&nbsp;

**5. WEBHOOK + POOCHNE WALA JOB + MILAAN**

Pakka jawab teen raaste se aata hai:
- **Webhook**: PSP khud bhejta hai, sign ke saath, taaki nakli message phenk sakein. Dohra message aaye to "abhi pakka nahi hua tabhi SUCCESS karo", isliye paisa ek baar.
- **Job**: har 1 min chalta hai, 2 min purane INITIATED / PENDING uthata hai aur PSP se poochta hai.
- **Raat ka milaan**: PSP ki file se DB milao.

**AWS pe:** webhook aate hi line me daal do, AWS ka **SQS** (ya Kafka), aur baad me sambhalo. Line AWS chalata hai.

&nbsp;

**6. SAGA + BEECH KA KHAATA**

Dukaan ko paisa apne bank me chahiye, aur bank bahar hai, to ek transaction nahi chal sakta. **Saga**:
1. Hamare DB me: Arpan -500, beech ka khaata (hold) +500, payout t-60 = INITIATED.
2. PSP ko payout call.
3. SUCCESS aaya to hold -500. FAILED aaya to ulta: hold -500, Arpan +500, nayi ledger line.

Step 1 ke baad, PSP call se pehle server gira: t-60 INITIATED pada hai, to job use uthata hai (webhook nahi aayega, call gayi hi nahi). PSP bolta "aisa koi order nahi", to wahi key se dobara bhejo ya FAILED karke ulta. 500 hold me atakte nahi.

Hold isliye ki paisa har waqt kahin dikhe. **2PC nahi**, kyunki bank / PSP uske "taiyaar hoon" round me aa hi nahi sakte, aur ek ruka to sab ruk jaate.

&nbsp;

**7. LB + READ REPLICA**

Diwali pe 3000 payment / sec aaye, to kai service machine aur aage LB. Dukaan ki "aaj ki bikri" report bhaari hai, isliye wo **read replica** se chalti hai. Paisa kaatne ka faisla hamesha primary se.

**SPOF kahan:** DB primary (sync copy) · PSP (doosra PSP).

**AWS pe:** Postgres AWS ka **RDS** lete hain, **Multi-AZ** ke saath: doosri building me sync copy AWS rakhta hai, primary gira to wo khud copy ko primary banata hai. Sync rakhna (paisa hai) hamara faisla tha.

&nbsp;

**8. SHARD BY ACCOUNT (hamare DB ke tukde, bank nahi)**

Wallet sab **hamare** DB me hain.

**Dikkat:** Diwali pe 3000 likhai / sec aur saal bhar me ~1000 crore line, ek primary nahi jhel paaya. Banking me 120 / sec thi, isliye wahan shard nahi kiya tha.

**Account id se:** isliye hamare DB ko **account id** se tukdon me baanta: Arpan ka wallet, ledger aur payment, sab ek shard pe.

**Date se kyun nahi:** date se nahi baanta, warna aaj ki saari likhai ek shard pe girti.

**Pench:** Arpan shard 1 pe hai, dukaan shard 3 pe, to ek transaction nahi chalega, phir se saga.

&nbsp;

### Wrap

bahar ka paisa = PSP · pata nahi = PENDING · PSP band = breaker + PSP-2 (PENDING wala nahi) · server gira = INITIATED pehle · pakka jawab = webhook + job + milaan · bank ko paisa = saga + hold · bheed = LB + replica · likhai = shard by account + saga

**Kaise pata chale ki theek chal raha hai:** milaan ka farak, p99, kitne PENDING atke hain, trace id.


Poora design (har kadam ka diagram aur "kyun") = [06_payment_system.md](06_payment_system.md)
