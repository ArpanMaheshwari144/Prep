# BookMyShow: hand notes (do counter, ek seat)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan BookMyShow pe Avengers ke pehle show ki seat D5 book kar raha hai. Arpan ki misaal: cinema ke do counter ek hi seat bech rahe, aur ek-doosre ko pata nahi.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      show aur seat map dekho · seat chuno -> pay karo -> ticket pakka
        · seat chuni to 5 minute tere naam rukti, pay nahi kiya to chhoot jaati
        · pay ~1 minute (30-55 sec + thoda buffer) me na ho to payment fail dikhe, par seat abhi teri, popup "dobara pay?": Haan = doosre PSP ka QR, Naa = seat turant khali
        · 1 minute ke andar paisa kata to seat pakki; uske baad kata to refund
NFR     ek seat kabhi do logon ko na bike (yahi dil hai) · seat map dekhna tez · badi movie ke din ki bheed jhele
Numbers advance booking 10 baje: 10 lakh log ek saath, seat sirf 50 hazaar · 100 log map dekhte to ~1-2 book karte
        -> dekhna bahut, likhna kam · data chhota (aaj + agle hafte ke show), purana archive
```

&nbsp;

**1. Arpan ne D5 chuni aur "book" dabaya. Usi second ek aur ladke ne bhi D5 pe book dabaya. Dono ke app ne seat khaali dekhi, aur dono ko "seat mil gayi" aa gaya.**
Arpan: Seat dekhna aur seat book karna, ise ek saath rakh, alag-alag mat rakh. Maan le do counter hain, main is counter pe, doosra us counter pe, aur andar ek hi seat bachi. Dono counter ko nahi pata ki doosra bhi wahi seat bech raha. Maine kaha "ek seat chahiye, lo mere paise". Mere counter wale ne jaake seat pe ek parchi rakh di: "ye Arpan ki seat hai, Arpan iska payment karega, ye Arpan ko milegi, kisi aur ko nahi." Doosra counter wala dekhega: "achha, yahan Arpan ki parchi lagi hai, ise main bech nahi sakta. Tu ja bhai, seat book ho gayi."
Technical me: "ek saath rakh, alag mat" = dekhna aur rokna ek hi query (UPDATE ... WHERE seat FREE), "seat pe parchi" = hold, "doosra parchi dekh ke mana" = doosre ko 0 row, "seat chali gayi".

**Maan lo Arpan ko family ke saath D5, D6, D7 teeno chahiye; counter wale ne D5, D6 pe parchi lagayi, D7 tak pahuncha to usi second doosre counter wale ne D7 pe Priya ki parchi laga di.**
Arpan: Ye almost same hai. D5 D6 D7 ka kaam ek saath hi hoga, alag-alag nahi: dekhna aur rokna teeno ka ek hi baar, beech me koi nahi aa sakta. Agar teeno available hain tabhi book karo. Maan lo mujhe D1 se D10 poora block chahiye, to main beech me khada hoke bolunga "sab sun lo, D1 se D10 meri", ek parchi pe likh dunga aur sab counter wale usi parchi se padhenge. Jaise ek chhotu board leke khada ho gaya: "D1 se D10 book hai, ise koi book nahi karega".
Technical me: "teeno ek saath, beech me koi nahi" = ek transaction (all-or-nothing, UPDATE ... WHERE seat IN (...) AND FREE; ek bhi na mili to rollback, kisi pe parchi nahi), "ek parchi poore block ki" = ek booking id ke neeche saari seat.

**2. Arpan ne D5 pe parchi laga di, phir pay kiye bina phone rakh ke chai peene chala gaya, kabhi wapas nahi aaya. D5 pe hamesha ke liye Arpan ki parchi, koi aur le hi nahi sakta.**
Arpan: Parchi ko thodi der ke liye wahin rehne de. Counter wala dekh raha: "ye payment kar raha, chal main 5 minute tak dekhta hoon. Aa gaya to theek, nahi to hata dunga yahan se." Usne 5 minute intezaar kiya, Arpan nahi aaya, to parchi phaad ke phenk di. Seat agle ke liye.
Technical me: "5 minute tak dekhta hoon" = hold expiry (hold_until), "parchi phaad ke phenk di, seat agle ke liye" = hold hata ke seat wapas FREE.

**3. Parchi phaadne wala counter wala har minute ghoom ke purani parchiyan dekhta hai. Aaj wo chhutti pe. Arpan ki parchi ko 20 minute ho gaye, abhi bhi D5 pe lagi, aur naya aadmi D5 lena chahta hai.**
Arpan: Sabse pehli baat, parchi uthane wala chhutti pe hai to uska ek chhota bhai rakh lo, wo kaam kar lega. Ya us din ke liye uska replacement laana padega. Ya phir hum us parchi pe hi umar laga dein ki wo apne aap wahan se gayab ho jaaye: parchi 5 minute tak yahan padi hai, 5 minute ke baad tu hat ja, parchi khatam.
Technical me: "chhota bhai / replacement" = backup safai job, "parchi pe hi umar" = shart query me hi (seat FREE ho YA hold_until nikal chuka), phaadne wale ka intezaar nahi.

**4. Avengers ki advance booking subah 10 baje khuli. 10 lakh log ek saath app pe toot pade, seat sirf 50 hazaar. Servers dub gaye, jo asli me pay kar rahe the unke page bhi atke.**
Arpan: Counter badha de, server badha de. Jo sirf dekhne aa raha uske liye ek alag diary, aur jo kuch hilane aa raha uske liye alag diary, primary-replica wali cheez. Aur thode-thode log andar jaao, zyada nahi. Sabse pehli baat, cinema ki capacity kitni hai, utne log andar. Maan lo mere hall me 1000 ki jagah aur saamne 5000 khade, to 1000 andar jaao, baaki 4000 ghar jaao. Aur wo line me lago: "le bhai, ye teri parchi, tu jaayega, phir agla number tera."
Technical me: "counter / server badha de" = machine badhana, "dekhne wale ki alag diary" = read replica, "capacity jitne andar, baaki line me" = virtual queue (waiting room), "ye teri parchi, agla number tera" = token.

**5. Line me Arpan ka number aaya, andar jaane ka token mila jo 10 minute chalega. Usne 8ve minute pe D5 chuni, to D5 pe 5 minute ki parchi lagi. 10ve minute pe token khatam, par parchi ke 3 minute baaki. Pay kar paayega? Kaunsi ghadi chalegi?**
Arpan: Humne use token diya, wo andar aa gaya. Iska matlab wo authenticate ho chuka, ab koi dikkat nahi. Andar aa gaya, to haan bhai, seat ruk gayi iske liye. Payment nahi karega to use hata do. Use utne time me payment karna hai jo uske liye allot hua. Kar diya to theek, nahi to agla.
Technical me: "token diya, andar aa gaya, authenticate ho chuka" = token sirf gate ka pass, "seat ruk gayi, allot hue time me payment" = pay ka faisla hold ki ghadi (hold_until) karti. Do alag ghadiyan: token = gate, hold = seat.

**6. Arpan ne 4:30 pe Pay dabaya, PSP dheema. 5:00 pe parchi ki umar khatam, 5:10 pe Priya D5 maang rahi, PSP ka jawab 5:20 pe (Pay ke 50 second baad, 6c ki 30-55 sec ghadi ke andar) "paisa kat gaya".**

**6a. Seat kiski?**
Arpan: Time ke andar Pay dabaya to seat Arpan ki, Priya ko nahi; humne vaada kar diya, warna consumer court ka case. PSP dheema hai to galti Arpan ki nahi, hamari taraf ki hai. To parchi pe status likh lo: pay chal raha hai, PSP ka jawab baaki. Jab tak ye likha hai, umar badha do, seat kisi aur ko dikhe hi nahi. Ya query me hi daal do.
Technical me: "status likh lo" = HELD -> PAYMENT_PENDING (Pay dabate hi), "umar badha do" = is status pe hold expire nahi hota (query aur safai job isko nahi chhoote).

**6b. Maan lo chhotu (PSP) jawab hi nahi de raha, 1-2 baar jagaya phir bhi chup. User ko kab tak rokun?**
Arpan: PSP do hi haal me hota: haan bol diya to BOOKED, theek. Dheema hai to 1-2 baar ping karo "bhai kya chal raha". Phir bhi chup to payment FAILED dikha do, kyunki peeche line lagi hai, aur bhi log pay kar rahe. Jaise mere paas do ladke: ek ko baar-baar bol raha "ja pay kar", wo mana kar raha, to "chhod, doosra ladka tu ja". Ab jo bhi naya pay karne aaye, wo doosre PSP pe jaayega, ye wala dheema hai.
Arpan: Seat mat chhodo. User to pay kar raha, dikkat Razorpay / bank ki taraf se, seat chhodi to galti hamari. Gola sirf 30-55 second (peeche picture ki line lagi, log 1 minute bhi nahi rukte, back daba dete), phir payment FAILED dikhao: "paisa kata hoga to wapas aa jaayega, dobara pay karo". Seat parchi ki bachi umar tak Arpan ki HELD; dobara pay doosre PSP pe (Amazon Pay). Dobara pay karna hai ya nahi, wo popup se poochho, neeche "ghadi pakki".

Aur agar dheeme wale ne peeche se paisa kaat liya tha? User seedha PSP se baat nahi karta, beech me hum baithe hain, jaise bank me seedha manager ke paas nahi ja sakte. Har payment ki id hamare paas hai. User ko bol do "paisa kata ho to wapas aa jaayega, dobara pay kar le", aur baad me kata nikla to rollback, paisa wapas. IRCTC, GPay, Paytm sab roz yahi karte; isiliye wo likhte hain "agar paise kate to", kyunki unhe bhi pakka nahi pata.
Arpan (ghadi pakki): Ghadi lagbhag 1 minute: 30-55 second + 5 second ka buffer, kyunki Razorpay ki taraf hum baithe nahi reh sakte, peeche aur log hain, aur user khud pareshan hota "seat hui ya nahi, paisa kata ya nahi". Gole ke beech UI pe likho "tumhara payment process ho raha hai, bank se jawab aate hi batayenge"; peeche chhotu Razorpay se poochhta rahe. 1 minute ke andar paisa kat gaya to seat BOOKED, khatam kahani, refund nahi. 1 minute ho gaya aur jawab nahi to dikhao "booking fail hui, paisa kata hoga to aaram se wapas aa jaayega, seat abhi bhi tumhari hai". Saath me UI pe popup: "pehla payment fail hua, kuch dikkat ho to refund kar denge ya hume reach out karo; isi seat ko book karna hai to dobara pay karo, Haan / Naa". Haan dabaya to QR dobara khule, is baar doosre PSP ka (Amazon Pay), pay hua to seat BOOKED. Naa dabaya to seat turant khali, apni jagah wapas, khatam kahani. Seat ki umar (TTL) ab nahi badhate, badhane ka matlab nahi; seedha user se poochh lo, 99% wo Haan dabayega kyunki galti hamari taraf ki. Har cheez backend se nahi, UI / frontend bhi kaam karta. (Bank ka server hi down ho to wo hamare HLD ke bahar.) DB me likho: is payment ki booking fail, paisa kata ho to refund. Razorpay ko bhej do "iska paisa kata ho to lautao, nahi to kuch nahi"; Razorpay bank se poochh ke lautata aur hume bata deta "process kar diya". 1 minute ke baad aaya "paisa kat gaya" = refund.
Technical me: ghadi = client timeout ~60 sec (30-55 + buffer), "process ho raha" = PAYMENT_PENDING UI message + status poll, 1 minute ke andar SUCCESS = BOOKED (refund nahi), 1 minute ke baad = payment attempt FAILED + booking HELD + UI popup "dobara pay?" (Haan = PSP-2 ka QR, Naa = turant EXPIRED, hold_until nahi badhaya), der se aaya SUCCESS = REFUND_PENDING + refund API (Razorpay bank se poochh ke lautaye), dobara pay = PSP-2.

Hum perfect system ke peeche nahi bhaag rahe. PSP hamare bas me nahi, wo dheema hai to na uski galti na hamari. Perfect consistency maango to ticket bechoge kaise; dikkat aayegi, bas user se saaf baat karo aur paisa wapas karo.
Technical me: "1-2 baar ping" = status check retry (timeout), "payment FAILED dikha do, seat mat chhodo" = payment attempt FAILED, booking wapas HELD (hold_until tak, dobara pay), umar khatam to EXPIRED (paisa kata nikla to refund), "doosra ladka / naye log doosre PSP pe" = circuit breaker -> PSP-2, "id hamare paas" = payment id (idempotency key), "baad me kata nikla to wapas" = reconciliation job + refund.

**6c. Gola ghoomta raha, Arpan ne thak ke back daba diya, booking FAILED, seat chhod di. Thodi der baad Razorpay ne paisa kaat liya: paisa gaya, seat bhi nahi. Ab?** (Myntra wala hi jawab; BookMyShow ka bhi apna PSP nahi, Razorpay hi hai)
Arpan: Razorpay kitni der lega, wo hamare haath me nahi, uska design hai. User ko zyada der nahi rok sakte, itne me to Doomsday ka poora hall book ho jaayega. To 30 se 55 second gola ghumao, uske baad saaf bol do: "payment abhi ho nahi pa raha, paisa kata hoga to apne aap wapas aa jaayega, doosre tareeke se pay kar lo". Doosra pay doosre PSP pe (Amazon Pay), Razorpay pe nahi. Aur seat mat chhodo, wo parchi ki bachi umar tak Arpan ki HELD rahe (6b).

Haan, onboard karte waqt Razorpay se SLA zaroor lo: unka system kitna chalega (uptime, jaise 99.9%) aur down hua to kitni der me khabar + theek karenge. Ye unke haath me hai, isliye likh ke dete hain. Bank kitni der lega, wo nahi.

Pehla payment DB me FAILED likh do (booking nahi, wo HELD rehti). Peeche hum khud Razorpay se poochte rahenge, aur uska event bhi aayega: paisa katte hi chhota sa event, jaise S3 file aane pe bhejta, aur hum haath utha ke "mil gaya" bol dete. Baad me pata chala paisa kata tha, to Razorpay ko bolo "is user ka paisa lautao". Wo bank se poochh ke kata tha to wapas karega, nahi kata to kuch nahi. Bank se poochhne ka haq Razorpay ka hai, BookMyShow ka nahi; bank hamare server ko jawab hi nahi dega. Paisa Razorpay ne kaata, to lautayega bhi wahi; hum apni taraf DB me likh ke rakhte hain. Badi film pe 3-4 company (Stripe, Amazon Pay) line me rakho.
Technical me: "30-55 sec gola" = hamara client timeout, "pehla DB me FAILED" = payment attempt FAILED, booking HELD rehti (payment ka sach event / poll se, BMS 6f), "onboard pe SLA" = PSP ka uptime SLA + incident response (latency ka nahi), "doosra pay doosre PSP pe" = PSP failover (backup PSP), "hum khud poochte" = status poll (fetch payment API), "chhota event" = webhook, "mil gaya" = ack (2xx), "baad me kata nikla to lautao" = REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe.

**6d. Razorpay ka event aaya "paisa kat gaya", parchi pe SUCCESS likh diya. Ticket banane wale ko bolne hi wale the ki server gir gaya. Restart ke baad parchi SUCCESS hai, to koi chhotu use nahi uthata (wo sirf PENDING dekhta). Paisa kata, ticket kabhi nahi bana.**
Arpan: SUCCESS likhna aur ticket book karna, ye do kaam alag-alag nahi, ek hi jagah hon. Ya to dono ek saath karo, ya SUCCESS ke saath hi ek parchi pe likh do "Arpan ne paisa de diya, D5 book karna hai", kyunki SUCCESS likhne ke baad bhi server gir sakta hai. Jaise mujhe D5 book karna tha aur beech me koi kaam yaad aa gaya, main bhool gaya; doosre ko D5 ka kuch pata hi nahi, wo apna agla kaam dekh lega. Isliye parchi pe likh do: koi bhi aaye, kahin se bhi aaye, parchi dekh le aur D5 book kar de.
Technical me: "SUCCESS aur parchi ek saath" = transactional outbox (payment SUCCESS + "ticket banao" ek hi DB transaction me), "koi bhi aaye parchi dekh ke book kare" = relay / worker outbox table padh ke ticket service ko bheje; server gire to restart pe parchi wahin padi.

**6e. Arpan ne parchi ki umar ke andar Pay dabaya, bank ne paisa kaatne me 12 minute lagaye. 6a se Pay dabate hi parchi ki umar ruk gayi thi, par 6c wali 30-55 second ki ghadi nikal gayi, payment FAILED dikha, seat bachi umar tak Arpan ki HELD rahi; Arpan ne popup pe Naa dabaya, seat turant khuli, D5 Priya ko mili. 12ve minute pe event: "Arpan ka paisa kat gaya". Arpan ki booking kya dikhegi, paise ka kya?**
Arpan: Razorpay kya karta, kitni der leta, wo hamara design nahi. Hamare haath me itna: Pay dabate hi ek chhotu rakh lo jo Razorpay se har kuch second poochta rahe (3, 5, 10 second, ek-dum nahi), jawab aaye to bata de; main apne aur kaam me lag jaata hoon. Window khatam to seat khul gayi, Priya ne doosre PSP se book kar li, ye theek hai. 12-13 minute pe chhotu aaya "paisa kat gaya", to DB me likh do: paisa kata, seat book nahi hui, refund dena hai. Arpan ko UI pe dikha do "booking fail hui, paisa 2-3 din me wapas aa jaayega". Raat ko sync job chalegi, jiska refund baaki hai sabka karegi: hamare haath me ho to hum, warna Razorpay ko bolo "iska lautao". Razorpay bank se poochega, wo uska design hai; bank BookMyShow ke server se baat nahi karta, Razorpay se karta hai. Yahi jawab har jagah jahan PSP beech me hai.
Technical me: "window khatam" = 30-55 sec client timeout pe payment FAILED, phir popup pe Naa to seat turant khuli (EXPIRED), "chhotu poochta rahe" = status poll job, "paisa kata, seat nahi" = payment SUCCESS + booking FAILED (do alag haalat), "refund dena hai" = REFUND_PENDING, "raat ki sync job" = refund / reconciliation job, "Razorpay ko bolo lautao" = refund API.


**6f. Razorpay ka event aaya SUCCESS, parchi pe SUCCESS likha. 2 second baad chhotu ka purana jawab (usne SUCCESS se pehle poocha tha, jawab "PENDING" raste me atka tha) pahuncha, aur chhotu ne parchi pe wapas PENDING likh diya. Ticket atak gaya.**
Arpan: Sach ek hi jagah ho, ek mote register me, DB me, jahan koi jawab badal na de. Razorpay ka event aaya to wo sach hai, DB me likh do "Arpan ka paisa kat gaya". Chhotu kuch bhi likhne se pehle DB me dekhe: wahan SUCCESS hai to apna purana PENDING phenk de. Aur agar parchi pe galat reh gaya, to har 5-10 minute ek sync job chale: DB me kya likha hai, wahi utha ke parchi pe likho; parchi utha ke DB me kabhi nahi. API dheemi ho sakti hai, purana jawab der se aa sakta hai, isliye hamesha DB dekh ke likho.
Technical me: "ek mote register me sach" = source of truth (DB), "likhne se pehle DB dekho, SUCCESS ke upar PENDING nahi" = conditional update (sirf tab likho jab abhi PENDING ho) = state machine, haalat sirf aage jaati, "DB se parchi theek, ulta nahi" = reconciliation / sync job.

**6g. Raat ki job ne Razorpay ko bola "Arpan ka 500 lautao", Razorpay bola "refund shuru". Job ne DB me likh diya "refund ho gaya". 5 din baad Arpan: "paisa nahi aaya". Refund Razorpay ki taraf beech me fail ho gaya tha.**
Arpan: "Refund shuru" ka matlab "ho gaya" nahi. Jab tak Razorpay se pakka jawab na aaye ki refund ho gaya, DB me REFUND_PENDING hi likho. Galat jawab zindagi me kabhi mat likho, paise ke maamle me inconsistency nahi; kal Arpan consumer court gaya aur hamare DB me "refund ho gaya" likha hai jabki bank me paisa nahi, to ungli hum pe. Ek job lagao: 5 din se purane jinka refund abhi tak pending hai, unhe nikaalo aur Razorpay se poochho "inka kya hua", unke support se bhi. Dabaav nahi daal sakte, par poochhna hamara kaam hai. Pakka jawab aaye tabhi DB me REFUNDED.
Technical me: "pakka jawab tak REFUND_PENDING" = refund state (REFUND_PENDING -> REFUNDED / REFUND_FAILED, sirf refund.processed / refund.failed event pe), "5 din se purane pending, Razorpay se poochho" = stuck refund job (reconciliation), "galat jawab mat likho" = source of truth.


&nbsp;

Poora design = [09_bookmyshow.md](09_bookmyshow.md)
