# BookMyShow: hand notes (do counter, ek seat)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan BookMyShow pe Avengers ke pehle show ki seat D5 book kar raha hai. Arpan ki misaal: cinema ke do counter ek hi seat bech rahe, aur ek-doosre ko pata nahi.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
User --> line (virtual queue, token: capacity jitne andar) --> LB --> kai counter (app server)
                                                                   |
   seat map dekhna:     padhne wali copy se (read replica), dekhne wale bahut, likhne wale kam
   seat chuni:          dekhna + rokna ek query (WHERE FREE), parchi = hold, 5 min umar
   Pay dabaya:          parchi pe PAYMENT_PENDING, umar ruki
   PSP (Razorpay):      ~1 min ghadi (30-55 sec + buffer), chhotu status poochhe + webhook
   SUCCESS:             DB me SUCCESS + "ticket banao" ek saath (outbox) --> ticket service
   der se kata / fail:  REFUND_PENDING --> raat ki job --> refund API --> pakke event pe REFUNDED

 1   do log ek saath D5                         ->  dekhna + rokna ek query (hold)
 1b  family D5 D6 D7, beech me D7 gayi           ->  teeno ek transaction, ek na mile to kuch nahi
 2   parchi laga ke chai peene gaya              ->  parchi ki umar 5 min (hold_until)
 3   parchi phaadne wala chhutti pe              ->  backup job / umar query me hi
 4   10 baje 10 lakh log, seat 50 hazaar         ->  read replica + line me lagao (virtual queue, token)
 5   token 10 min, parchi bachi                  ->  do ghadi: token = gate, parchi = seat (grace)
 6a  Pay 4:30, umar 5:00, PSP jawab 5:20         ->  PAYMENT_PENDING pe umar nahi kat-ti
 6b  PSP chup                                    ->  ~1 min ghadi, phir FAILED + popup (Haan = PSP-2, Naa = seat khali)
 6c  back daba diya, baad me paisa kata          ->  seat HELD, poll + webhook, kata nikla to refund
 6d  SUCCESS likha, ticket se pehle server gira  ->  SUCCESS + parchi ek saath (outbox)
 6e  bank ne 12 min lagaye, seat Priya ko        ->  payment SUCCESS + booking FAILED, refund job
 6f  purana PENDING ne SUCCESS kaata             ->  sach DB me, likhne se pehle DB dekho, sync job
 6g  "refund shuru" ko "ho gaya" likha           ->  pakke event pe hi REFUNDED, stuck refund job
```

**FR / NFR kahan se:** hawa se nahi aate. Upar ki dikkat chain dekh ke nikale; har FR / NFR line ke aage likha hai wo kis kadam ki dikkat se aayi.
Arpan (17-Nov round): Log aa rahe hain, paise dekar ja rahe hain, matlab seat book honi chahiye. Ek seat do aadmi ko nahi milni chahiye. Payment consistent hona chahiye. Logon ko zyada intezaar na karna pade. FR me wo jo BookMyShow karta hai: seat book karna, pay karna, ek seat do ko nahi. NFR wo jo hum engineer ki tarah dekhte: Redis, replica, kai server, load balancer, kuch atke nahi; koi line me aaya aur bahut der khada raha, uski parchi khatam ho gayi aur bina pay kiye chala gaya, 8 minute khade rehne ka fayda hi nahi hua, aisa na ho. Refund me BookMyShow ka seedha lena-dena nahi, wo PSP ka kaam hai. API abhi nahi, kuch din baad.
Arpan (shard pe): BookMyShow me shard nahi, partition kar do. Shard ki ek keemat hai, shard karna matlab apni aisi-taisi karwana. AWS jaise bahut bade platform, ya bank jo Europe, America, Canada, Australia, India sab jagah faila hai, wahan shard keh sakte. BookMyShow India me bahut bada bhi ho to partition kar lo, thoda indexing laga do. Shard bahut alag duniya hai; hum aise hi bol dete "shard kar do", par companiyan 10 baar sochti hain.
Technical me: "pehle partition" = ek hi DB ke andar table partitioning (jaise show date se), "indexing" = sahi index, "shard ki keemat" = cross-shard transaction, rebalancing, ops ka bojh; shard aakhri raasta.
Arpan (FR dobara, chain dekh ke): FR wo jo BookMyShow asal me karta hai. Chain me har baar seat dekhna, rokna, pay karna, book karna hi ho raha. To FR ke liye dikkat dekhne ki zaroorat nahi: seat dekho, seat hai to book karo, book kiya to paise kaato, paise kate to seat do, bas. Baaki NFR: do log ek saath book na karein, zyada bheed na ho, zyada line me intezaar na karna pade, refund turant mile, seat turant dikhe.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      (jo BookMyShow asal me karta hai, iske liye dikkat chain dekhne ki zaroorat nahi)
        1  seat dekho (seat map)
        2  seat khaali hai to book karo (seat roko)
        3  book kiya to paisa kaato
        4  paisa kata to seat do (ticket)
        (pehle se tay: seat chuni to 5 min tere naam, pay nahi to chhoot jaati -> kadam 2 ·
         pay ~1 min me na ho to FAILED, popup Haan = doosre PSP ka QR, Naa = seat khali -> kadam 6b ·
         1 min ke andar kata to seat pakki, baad me kata to refund -> kadam 6b, 6e)

NFR     (ye dikkat chain se)
        1  do log ek seat ek saath book na kar sakein                              -> kadam 1, 1b
        2  zyada bheed jhele                                                         -> kadam 4
        3  line me zyada intezaar na karna pade                                     -> kadam 4, 5
        4  paisa sahi rahe: kata to seat, seat nahi to refund jaldi                  -> kadam 6a-6g
        5  seat map turant dikhe                                                     -> kadam 4

Numbers sirf jahan faisla badle: advance booking 10 baje ~10 lakh log ek saath, itna hi bahut (Arpan 17-Nov)
        -> line + padhne wali copy (kadam 4)
Shard   nahi: shard ki keemat badi hai, companiyan 10 baar sochti hain. Bada ho to pehle partition + index (Arpan 17-Nov)
API     abhi nahi (Arpan 17-Nov: kuch din baad)
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
Doosri kahani (17-Nov BMS round grill): Isme Arpan ki galti nahi. Koi har baar 9:59 pe seat chune to use 1 minute hi milega, ye galat baat. To andar aake seat chuni to thoda grace period de do, 4-5 minute, taaki pay kar le. 9ve minute pe chuni to 9 + 5 = 14 minute tak; usme pay ho gaya to theek, nahi to bahar.
Technical me: "seat chuni to grace" = hold ki apni umar (hold_until), token khatam hone se nahi kat-ti.

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
Technical me: "SUCCESS aur parchi ek saath" = transactional outbox (payment SUCCESS + "ticket banao" ek hi DB transaction me), "koi bhi aaye parchi dekh ke book kare" = relay / worker / consumer outbox table padh ke ticket service ko bheje; server gire to restart pe parchi wahin padi.
Doosri kahani (17-Nov BMS round grill): Payment aur ticket do alag service hain, to dono pe ek transaction nahi lag sakta. Beech me ek dabba (DB) rakh lo jo ek service likhe, doosri padhe. DB me likh do "Arpan ka payment poora, ticket abhi PENDING, bani nahi". Server gira to koi baat nahi, jaankari kahin store hai. Ticket service aayi, dekha "Arpan ka ticket pending hai", bana diya, phir dabbe se nikaal ke phenk do. Ticket service khud bhi beech me giri to dobara aake wahi dabba padhegi: "jiske pending hain sab aa jaao". Gire usse pehle store kar lo, kahin bhi.
Technical me: "do service, ek transaction nahi" = distributed, "beech ka dabba, ticket PENDING" = outbox / pending table, "ticket service pending padh ke banaye" = consumer poll + retry, "kaam ke baad dabbe se hatao" = outbox row processed.

**6e. Arpan ne parchi ki umar ke andar Pay dabaya, bank ne paisa kaatne me 12 minute lagaye. 6a se Pay dabate hi parchi ki umar ruk gayi thi, par 6c wali 30-55 second ki ghadi nikal gayi, payment FAILED dikha, seat bachi umar tak Arpan ki HELD rahi; Arpan ne popup pe Naa dabaya, seat turant khuli, D5 Priya ko mili. 12ve minute pe event: "Arpan ka paisa kat gaya". Arpan ki booking kya dikhegi, paise ka kya?**
Arpan: Razorpay kya karta, kitni der leta, wo hamara design nahi. Hamare haath me itna: Pay dabate hi ek chhotu rakh lo jo Razorpay se har kuch second poochta rahe (3, 5, 10 second, ek-dum nahi), jawab aaye to bata de; main apne aur kaam me lag jaata hoon. Window khatam to seat khul gayi, Priya ne doosre PSP se book kar li, ye theek hai. 12-13 minute pe chhotu aaya "paisa kat gaya", to DB me likh do: paisa kata, seat book nahi hui, refund dena hai. Arpan ko UI pe dikha do "booking fail hui, paisa 2-3 din me wapas aa jaayega". Raat ko sync job chalegi, jiska refund baaki hai sabka karegi: hamare haath me ho to hum, warna Razorpay ko bolo "iska lautao". Razorpay bank se poochega, wo uska design hai; bank BookMyShow ke server se baat nahi karta, Razorpay se karta hai. Yahi jawab har jagah jahan PSP beech me hai.
Technical me: "window khatam" = 30-55 sec client timeout pe payment FAILED, phir popup pe Naa to seat turant khuli (EXPIRED), "chhotu poochta rahe" = status poll job, "paisa kata, seat nahi" = payment SUCCESS + booking FAILED (do alag haalat), "refund dena hai" = REFUND_PENDING, "raat ki sync job" = refund / reconciliation job, "Razorpay ko bolo lautao" = refund API.


**6f. Razorpay ka event aaya SUCCESS, parchi pe SUCCESS likha. 2 second baad chhotu ka purana jawab (usne SUCCESS se pehle poocha tha, jawab "PENDING" raste me atka tha) pahuncha, aur chhotu ne parchi pe wapas PENDING likh diya. Ticket atak gaya.**
Arpan: Sach ek hi jagah ho, ek mote register me, DB me, jahan koi jawab badal na de. Razorpay ka event aaya to wo sach hai, DB me likh do "Arpan ka paisa kat gaya". Chhotu kuch bhi likhne se pehle DB me dekhe: wahan SUCCESS hai to apna purana PENDING phenk de. Aur agar parchi pe galat reh gaya, to har 5-10 minute ek sync job chale: DB me kya likha hai, wahi utha ke parchi pe likho; parchi utha ke DB me kabhi nahi. API dheemi ho sakti hai, purana jawab der se aa sakta hai, isliye hamesha DB dekh ke likho.
Isi dikkat ki doosri kahani: PENDING aur SUCCESS kram se, line by line jaane chahiye, warna dikkat. Maan le kisi ne 4 seat book ki, aur jo main banda hai jiske paas ticket hai wo aaya nahi, kya main baaki 3 ko jaane dunga? Nahi. Waise hi pehle PENDING rakhunga, phir SUCCESS. Tu bol sakta hai server dheema ho jaayega; to ID me rakh lo, numbering kar do: har khabar pe number laga do, PENDING 1, SUCCESS 2. PENDING pehle pahunch gaya to badiya; SUCCESS pehle aa gaya to server dekhega "ye to 2 hai, isse pehle wala abhi aaya nahi", process nahi karega jab tak 1 na aaye, phir dono saath. Saare anklon (server) ko bol do: sab number se aayega, 2 pehle aaya to 1 ka intezaar. Chat me hum phone pe sort kar dete, par yahan seat aur paisa hai, grahak ko dikkat ho jaayegi.
Maan lo PENDING (1) raste me kho gaya, kabhi aaya hi nahi: ankal ji bolenge "SUCCESS ki parchi aa gayi, PENDING ki nahi". 1-2 second dekhenge, nahi aayi to ek chhotu ko Razorpay ke paas bhejo: "paisa kata ya nahi?" Kata hai to ankal ji iska sab kar do, raaste me koi server dheema hoga isliye wo aaya nahi. Ya PENDING aur SUCCESS ko ek hi operation me rakh do: ya dono hon ya koi nahi. Dono jawab valid.
Technical me: "khabar pe number, 2 ruke jab tak 1" = sequence number + ordering (out-of-order hold), "1 kho gaya to chhotu Razorpay se pooche" = timeout ke baad status poll (fetch payment API), "ek operation" = dono status ek saath (atomic).
Technical me: "ek mote register me sach" = source of truth (DB), "likhne se pehle DB dekho, SUCCESS ke upar PENDING nahi" = conditional update (sirf tab likho jab abhi PENDING ho) = state machine, haalat sirf aage jaati, "DB se parchi theek, ulta nahi" = reconciliation / sync job.

**6g. Raat ki job ne Razorpay ko bola "Arpan ka 500 lautao", Razorpay bola "refund shuru". Job ne DB me likh diya "refund ho gaya". 5 din baad Arpan: "paisa nahi aaya". Refund Razorpay ki taraf beech me fail ho gaya tha.**
Arpan: "Refund shuru" ka matlab "ho gaya" nahi. Jab tak Razorpay se pakka jawab na aaye ki refund ho gaya, DB me REFUND_PENDING hi likho. Galat jawab zindagi me kabhi mat likho, paise ke maamle me inconsistency nahi; kal Arpan consumer court gaya aur hamare DB me "refund ho gaya" likha hai jabki bank me paisa nahi, to ungli hum pe. Ek job lagao: 5 din se purane jinka refund abhi tak pending hai, unhe nikaalo aur Razorpay se poochho "inka kya hua", unke support se bhi. Dabaav nahi daal sakte, par poochhna hamara kaam hai. Pakka jawab aaye tabhi DB me REFUNDED.
Technical me: "pakka jawab tak REFUND_PENDING" = refund state (REFUND_PENDING -> REFUNDED / REFUND_FAILED, sirf refund.processed / refund.failed event pe), "5 din se purane pending, Razorpay se poochho" = stuck refund job (reconciliation), "galat jawab mat likho" = source of truth.
Doosri kahani (17-Nov BMS round grill): Razorpay do (ya jitne uske niyam) event bhejega. Pehla "refund shuru" aaya to DB me REFUND INITIATED likho. Usi din ya 1-2 din baad "refund poora" aaye tabhi REFUNDED. Jaise main kisi ladke ko paise dekar bhejoon, wo aake bole "pahuncha diye", to kya main uske bolne se maan lunga? Saamne wale ko phone karke poochhunga "sach me mile?".
Technical me: "shuru" = refund.created (REFUND_PENDING / INITIATED), "poora" = refund.processed (REFUNDED), "ladke ki baat nahi, saamne wale se poochho" = sirf pakke event pe final state.


&nbsp;

Poora design = [09_bookmyshow.md](09_bookmyshow.md)
