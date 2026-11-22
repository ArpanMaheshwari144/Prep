# Stock trading: hand notes (Arpan aur Ravi)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan TCS ke 10 share 3500 pe kharidna chahta hai, Ravi apne 10 TCS 3500 pe bechna chahta hai. Dono ek-doosre ko jaante nahi.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Arpan / Ravi  -->  LB  -->  order server  -->  paisa / share hold (funds block)
                                                    |
                                    matching engine (har share ki ek book, ek hi thread)
                                    book RAM me (tez) + har order parchi pe likha (order log)
                                                    |
                                    sauda hua  -->  trade DB (sach)  -->  Kafka
                                                                           |
                                    portfolio · paisa · SMS · bhaav screen (WebSocket + Redis pub/sub)

 1   Arpan Ravi ko jaanta nahi, kaun milaye     ->  beech me ek aadmi, bhaav milao (matching engine)
 2   net gaya, app ne dobara bheja, 20 share    ->  chaabi ka taala (idempotency key)
 3   35,000 se do order, chahiye 70,000         ->  EMI jaisa paisa hold (funds block)
 3b  10 chahiye, 4 mile, 6 baaki                ->  4 abhi + 6 ka hold jaari, ya "sab ya kuch nahi" (partial fill)
 3c  3500 ka order, market band, bika nahi      ->  order khula, hold jaari, cancel pe wapas
 3d  Ravi ke 10 share, 2 sell order, 20 bike    ->  share bhi hold; naya nahi, wahi order badlo (modify)
 3e  order khula, 20,000 withdraw               ->  beech se nahi; cancel ya chhota karke
 4   4 thread, 10 share 20 me bike              ->  ek share ek thread (1 thread per symbol)
 5   book RAM me, machine mar gayi              ->  parchi dekh ke dobara jamao (order log, replay); 3-4 copy
 5b  replay me ho chuke trade bhi wapas         ->  trade DB dekho, ho chuke hatao (DB = sach)
 6   sauda pakka, engine khud sabko call kare   ->  Kafka, consumer group
 7   portfolio DB band, paisa kata share nahi   ->  ulta nahi; parchi rakho, DB aaye to de do (Kafka retry)
 8   bhaav har second, 10 lakh screen           ->  chat wala hi (WebSocket + Redis pub/sub)
 9   9:15 lakhon order, line atki               ->  book RAM, batch me DB, copy
 9b  parchi pe match, DB me abhi nahi           ->  "processing" turant, DB ke baad pakka
```

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      buy / sell order dalo (apna bhaav likh ke) · bhaav mile to sauda · paisa aur share badlo · live bhaav dikhe · order cancel ho sake
        · order ka bhaav badal sake (naya order nahi, wahi order)
        · paise nikaal sake, par sirf jo khule order me ruke nahi (khula order cancel / chhota karke baaki)
        3:30 baje market band hua aur poore din order nahi bika, to kya hoga? Yahan do raaste hain:
          (1) order wahin khatam, aur 3:30 pe paisa / share user ko wapas
          (2) order agle din bhi khula rahe, aur paisa / share hum rok ke rakhein jab tak user khud cancel na kare
        Hamara FR = raasta (2). Isliye hold din badalne pe bhi rehta hai, aur cancel karne pe hi wapas jaata hai.
        Jitna mila utna abhi de do, baaki ke liye order khula rahe; user bole "sab ek saath chahiye, warna kuch nahi" to wahi karo.
        FR ne ye maanga, isliye design ye hai. FR raasta (1) maange, to 3:30 pe hold wapas, aur jawab bhi badlega.
        (pehle sawaal: Zerodha / Groww jaisa poora app 45 minute me nahi banta. Poochho: price, risk,
         settlement me se kya chahiye? margin, IPO, mutual fund scope me hain ya nahi? Hamara design = sirf order + sauda)
NFR     paisa aur share hamesha sahi · jo pehle aaya wo pehle mile · har cheez ka record
Numbers 50 lakh order roz, market ~6 ghante -> ~250 / sec aam · 9:15 pe jhatka ~10,000 / sec, saare share me bata
        live bhaav: ~10 lakh app khule, har second naya bhaav -> poochhne ki jagah push
```

**FR / NFR -> kadam (Arpan: "achha baith ke banaya, sab badhiya, aise hi rehne do"):** order dalo + bhaav mile to sauda = 1 ·
paisa aur share badlo = 3, 6, 7 · live bhaav = 8 · cancel = 3c · bhaav badle (wahi order) = 3d · paise nikaalna = 3e ·
3:30 raasta 2 = 3c · jitna mila utna = 3b · NFR paisa / share sahi = 2, 3, 5b, 7 · pehle aaya pehle = 4 ·
har cheez ka record = 5, 6.

&nbsp;

**1. Arpan aur Ravi ek-doosre ko jaante hi nahi. Unke jaise lakhon log, koi 3490 pe kharidna chahta, koi 3510 pe bechna, har second hazaron naye order. Arpan ka order Ravi ke order se kaun milayega?**
Arpan: Ek cheez bitha de beech me. Jo kharidne wala hai use behtar bhaav chahiye, jo bechne wala hai use bhi behtar bhaav chahiye. Main beech me baitha: achha Arpan ye kharidna chah raha, Ravi ye bechna chah raha. Jaante nahi, mujhe kya lena-dena. Arpan ko is bhaav me chahiye, Ravi ko is bhaav me, dono ka bhaav theek hai to dono ko aapas me sell aur buy kar do. Khatam kahani. Na Arpan ne Ravi ko jaana na Ravi ne Arpan ko, unka kaam ho gaya, beech me ek aadmi baith gaya tha.
Technical me: "beech me ek aadmi" = matching engine, "Arpan is bhaav me, Ravi is bhaav me, dono theek to sauda" = order book me bhaav milna (match).

**Maan lo pooche book dikhti kaisi:** Arpan: Bechne wale ke liye sabse sasta upar, kharidne wale ke liye sabse mehenga upar. Mere paas TCS ke 10 share hain, maine 3000 me khareede; ab bechunga to 3000 ya usse upar hi lunga, neeche nahi. Aise order book hogi, jisse dono ko fayda: jo khareed raha usko bhi, jo bech raha usko bhi.
Technical me: sell side = sabse kam bhaav upar (best ask), buy side = sabse zyada bhaav upar (best bid), dono milein to match.

**2. Arpan ne "Buy 10 TCS" dabaya. Order engine tak pahuncha, par jawab aate waqt net gaya. App ne socha order gaya hi nahi, dobara bhej diya. 20 share kharid liye, chahiye the 10.**
Arpan: Ek key laga de iske saath, jisse wo dobara ho na. Order server pe aaya, saath me ek chaabi le aaya. Main server hoon, maine us chaabi ka taala khareeda, sab taale me band kiya aur chaabi apne paas rakh li. 5 minute baad wahi chaabi leke wo dobara aaya "10 TCS". Maine dekha, ye to wahi hai jo mere paas hai, iska to maine taala khareed liya, kuch ghapla hai. Bhai tu nikal is building se, ye chaabi is building me nahi chalegi.
Technical me: "ek key laga de" = idempotency key (client_order_id), "chaabi ka taala khareed liya" = pehli baar wali key DB me save (UNIQUE), "wahi chaabi dobara aayi to nikal" = duplicate order reject.

**3. Arpan ke account me 35,000. TCS ka 35,000 ka order book me pada, abhi match nahi hua. Isi beech usi 35,000 se INFY ka 35,000 ka order bhi daal diya. Dono match ho gaye, chahiye 70,000.**
Arpan: Arpan ko ye paise kharch karne hi na de. Bank me EMI bharte hain na, bahut log bolte "bank ne mera paisa hold kar rakha, na kharch kar sakta na kahin kar sakta". Salary account pe EMI daal rakhi to wo paisa hold ho jaata hai. Same to same yahin kar lo, paisa hold kar lo, wo kahin bhi kharch nahi kar paayega.
Technical me: "paisa hold kar lo, jaise EMI me bank" = funds block (available se kam, blocked me), "kharch hi na kar paaye" = doosra order reject.

**Maan lo Arpan ko 10 TCS chahiye, 35,000 hold; Ravi sirf 4 bech raha, 4 mile, 6 baaki.**
Arpan: Main beech me baitha hoon. Arpan bola 10 TCS chahiye, Ravi sirf 4 bech raha. Yahan design choice hai, FR dekho: kya Arpan ko 10 abhi turant chahiye? Haan to 4 bhi mat do, sauda hi nahi, aur uska hold wapas kar do. Intezaar kar sakta hai to Ravi ke 4 abhi de do, baaki 6 ka hold rehne do; jab koi aur 6 bechne aayega, main use de dunga, Arpan ko kuch karna nahi.
Technical me: "4 abhi, 6 baad me" = partial fill (order PARTIALLY_FILLED, baaki 6 order book me), "10 turant warna nahi" = all-or-nothing (Fill-or-Kill, hold wapas), FR tay kare. Partial me 4 ka paisa kata (14,000), baaki 6 ka hold (21,000) jaari.

**Maan lo Arpan ne 3500 pe 10 TCS ka order daala, TCS 3600 pe hi raha, 3:30 baje market band, order bika nahi, Arpan cancel bhool gaya.**
Arpan: Main beech me baitha hoon. Bechne wale sab TCS 3600 maang rahe, Arpan bola mujhe 3500 ka chahiye, jab koi 3500 pe bechne aaye mujhe de dena. Theek hai, par pehle paise de ja, warna har koi aake bolega "mere liye rakh lena". Din bhar koi 3500 pe nahi aaya, koi baat nahi, paisa hold pe hi rahega, kyunki app usi liye bana hai. Wo order daal ke so gaya to bhi theek. Agle din TCS 4000 pe chala gaya, to saaf baat: lena hai to le, nahi lena to order cancel kar, paisa wapas tere paas. 3500 pe koi bechne aaya to tab de dunga.
Technical me: "jab tak cancel na kare order khula, paisa hold" = GTC (Good Till Cancelled) order, cancel pe hold wapas available me.

**Maan lo Ravi ke paas 10 TCS, usne 3600 pe 10 bechne ka order daala, bika nahi, phir 3650 pe 10 ka doosra order daal diya; dono bik gaye to 20 bech diye.**
Arpan: Ravi ke paas 10 hi share hain to wo 20 kaise bechega, ganit (maths) hi nahi baithta. Ravi mere paas aaya, bola mere 10 TCS bech de, 3600 chahiye; maine uska order likha 3600 ke 10. Ravi dobara aaya, bola bike nahi, 3650 kar de. To main naya order nahi banaunga, usi order ka bhaav 3600 se 3650 kar dunga. App me bhi yahi: bika nahi to Edit karo, bhaav badlo, peeche DB me wahi order update. Bhaav 3600 karo ya 7000, share 10 hi rahenge.
Technical me: "10 share usi order se bandhe" = share hold (holdings block, free se blocked me; doosra sell order reject), "usi order ka bhaav badlo" = modify order (naya order nahi, wahi order update).

**Maan lo order khula pada hai (35,000 hold) aur Arpan ne "withdraw to bank" se 20,000 nikalne chahe.**
Arpan: Ek aadmi 35,000 de gaya, maine entry bana di aur manager ko vaada kar diya ki 35,000 aa rahe. Thodi der baad bola "20,000 urgent chahiye". Beech me se nahi dunga, kyunki saamne vaada ho chuka. Do raaste: order cancel kar, poore 35,000 wapas tere account me, phir jitne nikalne nikaal; ya UI me modify ho to order chhota kar (15,000 ka), 20,000 free. Aadha-aadha nahi. Paisa uska hai, hold hamesha ke liye nahi, cancel ka raasta hamesha khula (har trading app deta).
Technical me: withdraw sirf available se (balance - blocked = 0, isliye reject + "order cancel / modify karo"), cancel = hold wapas available me, modify order = hold chhota.

**4. 9:15 baje bheed, engine tez karne ko 4 thread. Do thread ne ek saath TCS ki book dekhi, dono ko Ravi ke wahi 10 share dikhe. Ek ne Arpan ko bech diye, doosre ne Mohan ko. 10 share, bik gaye 20.**
Arpan: Sabko ek line me le. Mere paas 10 share hain, maine apni ek copy bana di aur thodi door khada kar diya. Ek ladka mere paas aaya, ek copy ke paas, dono ne 10 khareed liye, 20 bik gaye, mere paas the 10, galat ho gaya. To maine copy ko apne paas kheench liya, mujhme mil ja, yahin khada ho ja. Arpan aaya, Mohan aaya: copy ko bola thodi der ruk, pehle mujhe bechne de. Bech diya, mere paas zero, copy ko kaha apne zero kar le. Ab Mohan aaya, copy ne mana kar diya. Matlab sabko ek hi jagah rakho, alag-alag mat baanto.
Technical me: ladka = ek order (thread) · "copy door khadi, dono ne bech diya" = do thread ek book pe (race), "sabko ek line me, ek jagah, alag mat baanto" = ek share ki book ek hi thread (single-threaded per symbol).

**Aur neeche (hot IPO pe sab ek saath burst):** Arpan: Hot IPO aaya, sab ek saath burst. Baantna nahi hai, baant diya to race se kisi ko bhi kitne bhi share mil jaate. Line me lag jao bhai, counter jaisa: "kitne chahiye? 10; lo 10 share, niklo line se", agla. Ye bahut time nahi lagta, engine lakhon order per second kar sakta, jitna bolne me lag raha utna nahi. Sab ek hi thread pe, dikkat khatam, burst bhi sambhal jaata.
Technical me: "ek line, counter" = symbol ki ek queue, ek thread (sequencer), "baanta to race" = shard ek symbol ke andar nahi.

**5. Tez chalne ko TCS ki book RAM me. 10:30 baje machine mar gayi. Book ke saare order, Arpan ka 3500 wala adhoora order bhi, RAM ke saath ud gaye.**
Arpan: RAM me kyun rakh raha, sabko kisi permanent dabbe me rakh lo. Wo dheema pade to us dabbe ki 3-4 copy bana ke rakh lo, kal gira to doosri ya teesri copy hai. Kaata-jodi RAM me rakhna mujhe galat lag raha, kaata-jodi main DB me, primary DB me. Zyada bheed hai, zyada writes hain, to DB ko alag-alag shard kar do. Ye paise ki cheez hai, RAM me karna risky hai, RAM udi to kya karoge. Humne upar numbers discuss nahi kiye; numbers dekho to itni bheed nahi aati ki ek shard ya ek thread jhel na paaye.
Aur adhoora order: bag pack kar raha, saara saamaan aaya nahi, to poora bag khaali karo aur dobara jamao. Subah ek parchi banayi, Bangalore ja raha, 10 kapde rakhne hain, 5 ye, 3 ye. Jo rakha us pe tick, to-do ki tarah. Bag se nikaalna ho to sab hata do, par wo parchi mere paas utni ki utni hai. Wo parchi dekh ke dobara jama lo.
Technical me: "permanent dabba" = DB, "3-4 copy" = replica / standby, "numbers discuss nahi kiye, itni bheed nahi" = pehle numbers, phir RAM ya DB ka faisla, "subah se ek parchi, utni ki utni rehti" = order log (append-only), "parchi dekh ke bag dobara jamao" = log replay se book wapas.

Isi pe ek aur raasta (RAM me daalne se pehle kahin aur likh lo): RAM fast hai isliye RAM me rakha, par RAM me daalne se pehle usse kahin aur bhi likh le. RAM udi, restart hui, to jo likha hai wo wapas RAM me aa jaayega, kyunki hamare paas sab likha hua hai.
Technical me: "RAM me daalne se pehle kahin aur likh le" = order log (pehle log me likho, phir RAM; append-only, sirf jodna), "restart pe wapas RAM me" = log replay se in-memory book rebuild.

**Isi pe ek aur raasta (match ki book: har event likho, audit bhi):** Arpan: Hum match dekh rahe hain aur ek banda har event ek book me likh raha. Beech me light chali gayi, screen band, sab gum. Par jo banda book me likh raha, usse dobara le sakte: match kahan tak pahuncha, kaunsi ball, kitne run. Iske do fayde: audit trail (kaun, kya, kab) aur ek thread pe sab.
Technical me: "har event book me likhna" = event log (append-only, Kafka ya disk), "light gayi, book se wapas" = replay, "kaun kya kab" = audit trail.

**Maan lo replay me wo order bhi wapas aa gaye jo crash se pehle match hoke trade ban chuke the (Arpan ke 10 share Ravi ko bik chuke, dono ke portfolio bhi update). Phir se match hue to 10 share dobara bikenge.**
Arpan: Aisa nahi hone ka. Jiska order pakka ho gaya, wo DB me likh do, mote register me. Parchi se replay karte waqt mote register me dekh lo: kis-kis ka order tha, kya ho gaya; Arpan aur Ravi dono ka ho gaya, dono ke dashboard me dikh gaya, to inhe book me mat lagao, hatao. Parchi bhi kaam ki hai, par tab jab saath me mota register ho, kyunki aakhri sach (source of truth) mota register hai: kuch bhi bika, trade hua, portfolio me idhar se udhar gaya, wo register me pada hai. Jo order trade ho gaya, use dobara RAM me laane ka matlab hi nahi.
Technical me: "pehle kahin aur likh, phir RAM" = order log pehle (write-ahead), "replay pe mota register dekho, ho chuke hatao" = trade DB = source of truth, replay me sirf khule (unfilled) order book me wapas, dobara match nahi.

**Maan lo matching engine hi mar gaya:** Arpan: Engine ke bhi kuch replica rakhenge, aisa nahi ki ek mara to poora stock band. Ek mara to traffic replica pe, aur jo replica bilkul barabar hai wo promote ho ke naya engine ban jaayega. Us milli-second me kuch gadbad hui to logs hain, unse replay kar sakte. System band nahi hoga.
Technical me: hot standby engine, failover pe promote, beech ka gap order log se replay.

**6. Arpan aur Ravi ka sauda ho gaya. Khabar kai jagah: dono ke portfolio, paise ka hisaab, SMS, bhaav ki screen. Engine khud ek-ek ko call kare to SMS dheema hua to engine bhi ruka, 9:15 ki line atki.**
Arpan: Main apne paas 10 ladke ikatthe karunga: tu aa, tu aa, le ye information, sab pakad ke aa ja. Main khud kuch nahi karunga, maine baith ke instruction de di. 10 ladkon ki fauj: tu ye kar, tu ye kar. Jab sab ho jaaye to mujhe bata dena.
Technical me: "10 ladkon ki fauj" = Kafka (trades topic), "har ladka apna kaam" = har service ka alag consumer group, "main khud kuch nahi karunga" = engine sirf ek baar daal ke aage.

**7. Sauda pakka. Arpan ke portfolio me "10 TCS" likhna tha, us waqt portfolio wala DB band. Paisa kat chuka, share dikh nahi rahe.**
Arpan: Pehle kahin likh lo, hamare paas logs hain. Yahan rollback nahi kar sakte, matching engine ka poora system hi ye hai ki tumhara match ho gaya. Ab kahein "match nahi hua tha" to wo case kar dega, baat consumer court chali jaayegi. Use bata do thodi der baad, abhi service thodi dheeli hai, ghabrao nahi, mail pe ya app me dikha denge jab ho jaayega. Parchi apne paas rakh lo. 10-15 minute baad DB wapas aaya: "le ye meri parchi, ye kaam karna hai, jaldi kar, mere grahak intezaar kar rahe."
Technical me: "rollback nahi, consumer court" = sauda ulta nahi, "thodi der baad dikha denge" = settlement PENDING, "parchi apne paas, DB aaye to de do" = message Kafka me pada, DB aate hi retry.

**Maan lo portfolio DB 2 ghante band:** Arpan: Pehle dekho kya band hai: poora band ya read replica zinda. Replica zinda hai to padhne ka traffic read replica pe bhej do, data dikhta rahe, poora system band karne ki naubat na aaye. Poora band to system retry karta rahe aur user ko bata do "thodi der baad try karo"; peeche logs hain, kuch gadbad hui to logs se wapas.
Technical me: "padhna replica pe" = read replica se portfolio dikhao (likhna ruka), "retry + thodi der baad" = retry + user ko PENDING.

**8. TCS ka bhaav har second badal raha (3500, 3501, 3499), 10 lakh logon ki screen pe live chahiye. Har app har second poochhe to 10 lakh request har second.**
Arpan: Iska design hum upar (chat me) karke aa chuke hain, dobara bolne ka fayda nahi, ho gaya.
Technical me: "upar kar chuke" = chat wala hi: khula taar (WebSocket) + Redis pub/sub.
Arpan (kahani): Ye Twitter, Instagram jaisa hai: tum Salman Khan ke tweet dekhne nahi jaate, follow kar rakha hai to wo apne aap tumhari feed pe aa jaata, Instagram pehle se push kar deta. Same yahan: server pe bhaav badla to WebSocket se apne aap feed pe push.
Technical me: "follow kiya to khud aata" = push (pub/sub), poll nahi.

**9. (Maan lo NSE jaisi bheed, kadam 5 wale ~10,000 / sec nahi) Subah 9:15, market khula, TCS pe ek second me lakhon order. Har order pe book me sabse achha bhaav dekhna (padhna) aur naya order jodna ya match pe book badalna (likhna). Book DB me, har baar kuch millisecond. Line badh rahi, 9:15:01 ka order 9:16 pe match.**
Arpan: Itna bhaar DB pe mat rakho, mota register bechara thak jaayega. Baar-baar mote register pe likhne aur padhne thodi jaaunga. Apne paas ek chhoti parchi rakh lo, dhadadhad us pe likhte raho: ye aaya, iska match hoga, ye hat gaya. Sab ho jaaye to baad me ek saath mote register pe likh do. Koi bole parchi kho gayi to? Koi baat nahi, uski 3-4-5 copy bana lo, ek gayi to doosri se mil jaayegi.
Technical me: "chhoti parchi, dhadadhad likho" = order book RAM me (in-memory), "baad me ek saath mote register pe" = DB me batch write, "parchi ki 3-4-5 copy" = standby / replica (aur order log). Kam bheed ho to block 5 jaisa DB pe chal jaata; NSE jaisi bheed pe RAM.

**Maan lo bheed 10x:** Arpan: Replica laga sakte, aage LB, zyada server, Kafka apna scale ho jaata. Aur matching engine ko scale karne ke liye use aur bada kar sakte, matlab vertical scaling, kyunki ek share ki line baant nahi sakte.
Technical me: app = horizontal (LB + server), engine = vertical scaling (ek symbol ek thread, baant nahi sakte).

**Maan lo Ravi ka sauda parchi pe match ho gaya, DB me abhi gaya nahi. Ravi ko "sauda ho gaya" kab dikhaye?**
Arpan: Sach to mota register (DB) hai, parchi sirf tezi ke liye. Ravi ke liye alag se mote register pe jaaun to fayda kya. Parchi pe tabhi likho jab hamare paas sach me maal hai, koi bechne wala hai. Ravi ko bol do "rakh liya, tu ja, app me update ho jaayega", app pe "processing" dikha do. Sab ho jaaye, mote register me likh do, phir app me pakka dikha do.
Technical me: "sach mota register" = DB = source of truth, "processing dikha do" = ACCEPTED / PENDING turant, "mote register ke baad pakka" = DB me commit ke baad FILLED / confirmed.

&nbsp;

Poora design = [05_stock_broker_trading.md](05_stock_broker_trading.md)
