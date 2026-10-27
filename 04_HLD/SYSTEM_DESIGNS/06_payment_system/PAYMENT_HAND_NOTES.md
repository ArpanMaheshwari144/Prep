# Payment: hand notes (mock se pehle ye padho)

### Shuru

Kaam: ek wallet se doosre wallet me paisa bhejna (Arpan se kirana dukaan), bank se wallet me paisa daalna (top-up), aur wallet se bank me nikaalna (payout). Fail ho to paisa wapas aaye, aur har paisa ka hisaab mile. Fraud, kai currency aur chargeback scope ke bahar hain, ye pehle hi poochh ke hata do. Sabse upar ye ki do baar na kate, dono taraf ek saath badle, kuch gire to bhi paisa na khoye aur payment atka na rahe. ~10 crore user hain. Aam din kuch sau payment har second aate hain, Diwali / sale pe kuch hazaar. Banking jaisa hi: bheed badi dikkat nahi hai, sahi hona badi dikkat hai.

&nbsp;

**1. WALLET SE WALLET = BANKING KA CORE**

Arpan ne dukaan ko 500 bheje, aur dono ka wallet hamare hi DB me hai. To ye bilkul banking wala transfer hai: ek transaction, idempotency key (UNIQUE), `balance >= 500` wala check aur kaatna ek query me, dukaan +500, aur ledger me 2 line. SQL (Postgres), kyunki teeno cheez ek saath badalni hain (ACID). Interview me yahan ek line bolo, "banking jaisa", aur aage badho. Asli naya hissa yahan se hai: paisa bahar se aana aur bahar jaana.

&nbsp;

**2. PSP**

Arpan ke wallet me paisa nahi tha, use HDFC se 2000 daalne the, aur HDFC hamara DB nahi hai. Isliye beech me **PSP** (Razorpay / Juspay). Hum PSP ko order t-50 dete hain, Arpan UPI se pay karta hai, PSP batata hai "t-50 SUCCESS", aur hum ek transaction me wallet +2000 aur ledger me line likhte hain. Har bank se khud judna nahi chuna, kyunki usme har bank, NPCI aur card network se alag-alag judna, manzoori lena aur PCI-DSS ke niyam maanna padta, jo saalon ka kaam hai. PSP ek API deta hai aur thodi fees leta hai.

&nbsp;

**3. PENDING + TIMEOUT + BREAKER + DOOSRA PSP**

PSP 30 second tak chup raha. "FAILED" dikhate to Arpan dobara daalta aur 4000 kat jaate. "SUCCESS" dikhate to wallet me nakli paisa aa jaata. Isliye jo pata nahi, use **PENDING** bolo: "processing, 2-3 minute me pakka batayenge". Timeout chhota rakho (5 sec), warna Diwali pe saare thread PSP ke intezaar me phans jaate aur wallet se wallet wale payment bhi ruk jaate. PSP poora band ho to **circuit breaker**: 1 minute me 50% call fail hui to PSP ko call hi mat karo, 30 sec baad 1-2 call try karo. Naye top-up **PSP-2** se bhejo. Par jo PSP-1 pe PENDING atka hai, use PSP-2 pe dobara mat bhejo, kyunki ho sakta hai PSP-1 ne paisa kaat liya ho.

&nbsp;

**4. INITIATED PEHLE + WEBHOOK + POOCHNE WALA JOB**

Humne PSP ko call kiya aur jawab se pehle hamara hi server gir gaya. Restart pe yaad hi nahi ki call gayi thi ya nahi. Isliye call se **pehle** DB me `t-50 = INITIATED` likh ke COMMIT karo. Ab kam se kam ye pata hai ki kis payment ka pata karna hai, aur t-50 hi PSP ke liye idempotency key ka kaam karta hai. Pakka jawab teen tarah se aata hai:
- **Webhook**: PSP khud message bhejta hai, sign ke saath taaki nakli message phenk sakein. Dohra message aaye to "abhi pakka nahi hua tabhi SUCCESS karo", isliye paisa ek hi baar judta hai.
- **Poochne wala job**: har 1 minute chalta hai, 2 minute purane INITIATED / PENDING uthata hai aur PSP se poochta hai. Isse 2-3 minute ka wada nibhta hai.
- **Raat ka milaan**: PSP ki file se apna DB milao.

&nbsp;

**5. SAGA (DUKAAN KA BANK)**

Dukaan ko paisa wallet me nahi, seedha apne bank me chahiye. Arpan ka wallet hamare DB me hai aur dukaan ka bank bahar, to ek transaction nahi chal sakta. Isliye **saga**:
1. Hamare DB me ek transaction: Arpan -500, **beech ka khaata (hold)** +500, ledger, aur payout t-60 = INITIATED.
2. PSP ko payout call.
3. SUCCESS aaya to hold se -500. FAILED aaya (galat IFSC) to ulta: hold -500, Arpan +500, nayi ledger lines (mitaana nahi).

Beech ka khaata isliye chahiye ki paisa har waqt kahin dikhe aur poore hisaab ka jod 0 rahe. 2PC nahi liya, kyunki bank / PSP uske "taiyaar hoon" wale round me hissa hi nahi le sakte, aur ek ruka to sab ruk jaate.

&nbsp;

**6. LB + READ REPLICA**

Diwali ki shaam 3000 payment har second aaye, ek machine nahi jheli. Isliye kai Payment Service machine aur aage **LB**. Ye chalta hai kyunki service kuch yaad nahi rakhti, saara haal DB me hai. Upar se dukaan wale "aaj ki bikri" report baar-baar khol rahe the, aur wo bhaari query primary pe chal rahi thi. Isliye report **read replica** se. Report 2 second late chalega, par paisa kaatne ka faisla hamesha primary se hota hai.

&nbsp;

**7. SHARD BY ACCOUNT (PHIR SE SAGA)**

Saal bhar me ~1000 crore line ho gayi, aur Diwali pe 3000 likhai / sec. Ek primary ye nahi jhel paaya (banking me sirf 120 / sec thi, isliye wahan shard nahi kiya tha). Isliye **account id se shard**: Arpan ka wallet, uski ledger lines aur payment, sab ek shard pe. Date se nahi baanta, warna aaj ki saari likhai ek hi shard pe girti. Pench: Arpan shard 1 pe hai, dukaan shard 3 pe, to ek transaction nahi chalega. Wahi saga: shard 1 pe hold, shard 3 pe +500, fail hua to ulta.

&nbsp;

### Agar Pooche

- API: `POST /pay { from, to, amount }` + Idempotency-Key header -> `{ status, txn_id }`.

- Key kab banti hai: payment screen khulte hi. Do alag payment = do screen = do key. Cheque number jaisa.

- Haal: INITIATED -> PENDING -> SUCCESS / FAILED. Jitna kata, utna hi juda.

- Owner check: userId JWT se lo, body se nahi, warna 403.

- SPOF: DB primary (sync copy) · PSP (doosra PSP).

- Kaise pata chale ki theek chal raha hai: milaan ka farak, p99, kitne PENDING atke hain, har payment ka trace id.

### Wrap

do baar na kate = idempotency key (PSP ko bhi wahi id) · ek saath = DB me transaction, bahar / doosre shard se saga · paisa na khoye = INITIATED pehle + webhook + job + milaan · trace = ledger


Poora design (har kadam ka diagram aur "kyun") = [06_payment_system.md](06_payment_system.md)
