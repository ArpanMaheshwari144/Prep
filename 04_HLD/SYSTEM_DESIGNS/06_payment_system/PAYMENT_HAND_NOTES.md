# Payment: hand notes (dukaan pe pay)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan dukaan pe khade ho ke app se pay karta hai. Uska paisa HDFC me, dukaan wale ka ICICI me. Hamara app na HDFC ka, na ICICI ka.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Arpan  -->  app server  -->  parchi INITIATED (pehle apne register me)  -->  PSP-1 (band / chup to PSP-2)  -->  HDFC
                                    |                                           |
                                    |  pakka jawab: PSP webhook ("mil gaya" ack) + status job jo poochhta rahe
                                    v
                          wallet DB (tukdon me, shard) · ledger me hold · saga se ulta karo, 2PC nahi

 1   HDFC hamari sunta hi nahi                  ->  beech ka bada, uski API (PSP)
 2   PSP 30 sec chup, FAILED ya SUCCESS?        ->  apne paas parchi, INITIATED (payment status)
 3   PSP batana bhool gaya                      ->  beech me banda poochhta rahe (status job)
 3b  PSP ka message raste me kho gaya           ->  event + "mil gaya" (webhook + ack)
 4   PSP-1 poora band                           ->  PSP-2 pe bhejo (fallback PSP)
 5   INITIATED likha, call bhejte server gira   ->  pehle register, phir bhejo; dobara bheja to payment id (idempotency)
 5b  net atka, Pay do baar dabaya               ->  phone ki key, wahi parchi lautao (client idempotency key)
 6   wallet se kata, ICICI ne FAILED bola       ->  sab ulta, paise wapas (saga)
 7   2 minute 500 na yahan na wahan             ->  ledger me likho, paise yahan hain (hold)
 8   "sab taiyaar ho?" poochh ke kaato          ->  slow dost se mera paper chhoota (2PC nahi)
 9   Diwali 3,000 / sec, ek register            ->  register ke tukde (shard)
10   wallet tukda 1 me, dukaan tukda 3 me       ->  ladka diary le ke jaaye (saga do shard ke beech)
11   PSP-1 chup, mara ya dheema pata nahi       ->  "process ho raha"; down pakka pata ho tabhi PSP-2
11b  PSP-2 pe bheja to do baar katega?          ->  30-55 sec phir FAILED + PSP-2; PSP-1 pe bhi kata to wo lautaye
```

**FR / NFR kahan se:** upar ki dikkat chain dekh ke nikale; har line ke aage likha hai wo kis kadam se aayi.
Arpan: PSP ka FR / NFR se kuch lena-dena nahi, PSP alag cheez hai (HDFC hamara PSP nahi). FR me ek cheez poochh sakte: sab ek bank me rakhein ya do bank me. Ek hi bank me sab ho to wahan transaction, do bank me ho to saga. Hamara design do bank wala, isliye saga likha.
Arpan (do baar katna): Do baar katta hai bhai, GPay, PhonePe, Paytm sab pe hota. PSP / Razorpay ke chakkar me: wahan se jawab na aaye, doosre PSP pe bhej diya, paise kat gaye; ye hum rok nahi sakte. Kabhi user ka net, kabhi Razorpay ka, kabhi bank ka. Isliye "do baar nahi katega" bolna zyada ho jaayega, interview me cross-Q ban jaayega; bolo hi mat.
Technical me: ek jagah = ek DB transaction (ACID), alag jagah = saga (har kadam ka ulta kadam), 2PC nahi (kadam 8). Double debit = idempotency se kam, par zero ka vaada nahi; reconciliation + auto refund; RBI TAT circular (20-Sep-2019): UPI merchant T+5, P2P T+1, der pe Rs 100 / din.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      1  dukaan ko pay (bank se)                                         -> kadam 1, 2, 3, 4
        2  dukaan ka paisa wallet se uske bank me                          -> kadam 6, 7
        3  fail ho to paisa wapas                                          -> kadam 6, 11b
        4  wallet top-up                                                   -> kadam 11
        5  wallet se wallet (Arpan se dukaan)                              -> kadam 10
        6  FR me poochho: dono khaate ek jagah (ek bank / ek register) ya alag?
           Ek jagah = seedha ek transaction. Alag jagah (do bank, do tukde) = saga.
           Hamara design = alag, isliye saga. FR badle to jawab badle.     -> kadam 6, 8, 10
        (pehle sawaal: bank se paisa PSP (Razorpay jaisa) laata; uska andar ka kaam design ka hissa nahi, Razorpay
         apne aap me alag system design. Interviewer se poochho: PSP ka timeout kitna rakhein, ya PSP scope se bahar? Wo bole
         "30-60 sec bahut" to wahi timeout, uske baad FAILED aur alag column "kitni der baad kata" (BMS 6c jaisa))

NFR     1  do baar na kaatne ki poori koshish (payment id, idempotency); phir bhi PSP / bank / net ki
           gadbad se kabhi do baar kat sakta. Kata to apne aap wapas: raat ka milaan
           (reconciliation) + refund                                       -> kadam 5, 5b, 11b
        2  paisa kabhi gayab na dikhe                                      -> kadam 7
        3  kuch gire to bhi payment atka na rahe                           -> kadam 3, 3b, 4, 5, 11
        4  har paisa trace ho                                              -> kadam 2, 7
        5  CAP me consistency, availability nahi: paise ka niyam hi consistency hai.
           Availability chuni to wahi hoga jo banking kadam 21 me hua: do jagah ki branch (Mumbai aur doosre desh
           wali) dono ne apne-apne register se 1000 de diye, raat ko balance -1000. Isliye paisa ek hi jagah se
           kate (jahan khaata hai); dheema chalega, galat hisaab nahi. PSP gira to doosri service, par hisaab
           ek hi jagah.                                                    -> kadam 7, 9, 10
Numbers ~10 crore user, aam din kuch sau payment / sec, Diwali pe ~3,000 / sec
        -> aam din bheed badi dikkat nahi, sahi hona badi dikkat · Diwali ka 3,000 / sec ek register nahi jhelega (kadam 9)
```

&nbsp;

**1. Arpan ne "pay 500" dabaya, app ko HDFC khaate se 500 nikalne hain. Par app HDFC ka hissa nahi, HDFC hamari sunta hi nahi. Har bank se khud judne jaayein to alag manzoori, saalon lag jaayenge.**
Arpan: Ek aisa system bana do, aisi API jo ye kaam khud kare. Har cheez ke liye alag-alag nahi, sirf ek centralized tareeka ho, koi centralized authority jo ye kaam khud kare. Bas hum uski cheez use karein.
Technical me: "centralized authority jo ye kaam khud kare" = PSP (Razorpay, Juspay), "bas hum uski cheez use karein" = PSP ki API.

Reel se (Stripe; Arpan: "scene-mock ka matlab nahi, tokenization credit card ka apna hota, Stripe ID rakhta"): card ka number apne DB me nahi, PSP card rakhe aur hume ek ID de, DB me sirf wo ID; UPI me sirf UPI ID, PIN user ke UPI app me.
Technical me: tokenization (payment method ID), card data PSP ke paas (PCI DSS), India me RBI ka niyam (1 Oct 2022): dukaan aur PSP dono card number nahi rakhte, sirf bank / card network; baaki ke paas token.
**DB kaunsa: SQL ya NoSQL?** Arpan: Mera jhukaav consistency pe bahut zyada hai, system consistent hona chahiye. SQL DB ACID properties follow karta (atomicity, consistency); NoSQL (Cassandra jaisa) key-value hai, ACID follow nahi karta. Hum transaction, ACID, consistency wali cheezon pe zyada baat kar rahe, scale pe itni nahi. Scale pehle replicas aur index laga ke, sharding (partition) baad me, wo bahut badi baat hai. To MySQL ya Postgres.
Technical me: paisa = ACID chahiye = SQL (Postgres / MySQL); scale ka kram: read replica + index pehle, shard aakhir me.

**2. App ne PSP ko bola "500 kaato", aur PSP 30 second se chup. FAILED dikhaya to Arpan dobara pay karega aur 1,000 kat sakte. SUCCESS dikhaya to ho sakta paisa kata hi na ho, aur dukaan wala bina paise saamaan de de.**
Arpan: Hum apne paas ek parchi rakh lein, jisse dekh sakein PSP pe kya ho raha. Pay dabaya, to maine apne paas parchi rakh di: INITIATED ho gaya. Parchi pe likha rahega kya-kya hua: PSP ne ye kara, ab ye kara. Fail ho gaya to FAIL mark kar do, pass ho gaya to PASS mark kar do.
Technical me: "apne paas ek parchi" = payment table me status, "INITIATED ho gaya" = INITIATED status, "fail hua to FAIL, pass hua to PASS mark" = pakka jawab aane pe hi SUCCESS / FAILED.

**3. Parchi pe jawab ka intezaar hai. PSP ka pakka jawab pata kaise chale? Aur PSP batana bhool jaaye, ya uska message raste me kho jaaye, to payment hamesha atka rahega?**
Arpan: Beech me ek aisa banda bitha do jo humein batata rahe: PSP hua ya nahi hua, bata do. Beech me ek aisa system bithao jo permanent dono se baat karta rahe: haan bhai, hua, nahi hua.
Technical me: "beech me banda jo batata rahe hua ya nahi" = poochne wala job (PENDING utha ke PSP se status poochhe), "permanent dono se baat karta rahe" = PSP aur hamare DB ka milaan.

**Aur neeche (pakka jawab ke teen raaste):**
Arpan: Pakka jawab ke teen raaste: PSP ka event, beech ka banda atki parchi pe PSP se poochhe, aur raat ko PSP ki file aur apne register ki file milao. Atka ya ulta mila to dobara bhejo ya paisa wapas.
Technical me: webhook + status poll + raat ka milaan (settlement file vs payment table, nightly reconciliation).

**Jaankari (sirf jaankari hai, hamare design me ye asal me nahi; reel se, UPI ke andar kya hota, NPCI ke andar tak koi nahi poochta):** QR me sirf UPI ID hoti hai, bank ki detail nahi. PIN phone pe hi taale me band ho jaata hai, Google Pay bhi use nahi padh sakta, sirf tera bank. Google Pay request aage badhata hai: apne partner bank ko, wahan se NPCI (jo saare bank aapas me jodta). NPCI tere bank se 500 kaatne ko kehta, pump wale ke bank se 500 jodne ko; dono taraf se haan aaya tab green tick. Par green tick ke waqt sirf dono bank ke register badle hain, asli paisa bank se bank abhi gaya hi nahi. Din me kuch fixed waqt pe NPCI sab ka jod-ghata karta hai (kis bank ko kitna dena, kitna lena) aur sirf bacha hua paisa ek saath jaata hai, jaise YouTube me 10-10 rupaye jama karke ek saath bank. Isiliye hamare paas bhi PSP ka paisa aur settlement file baad me aati hai, aur raat ka milaan zaroori.
Technical me: NPCI = UPI chalane wala, saare bank usi se jude (har bank har bank se nahi) · green tick = dono bank ke ledger me entry · bacha hua ek saath = deferred net settlement (RBI me rakhe bank khaaton ke beech).

**Maan lo PSP ka message raste me kho gaya, ya beech wala banda bhi der se pahuncha.** (BookMyShow 6c wala hi jawab)
Arpan: PSP ko onboard karte hi bol do: "jaise hi paisa kaato, mujhe ek chhota sa event bhej do", jaise S3 file aane pe event bhejta hai. Event aaya matlab paisa kata, hum haath utha ke bol dete "mil gaya". Event raste me kho gaya to wo PSP ka HLD hai, wo dobara bhejega; tab tak beech wala banda poochhta rahe. Na hum andhe, na wo.
Technical me: "paisa katte hi chhota event" = webhook (PSP callback, sign jaanch ke), "mil gaya" = ack (2xx), "dobara bhejega" = webhook redelivery, beech wala banda = status job backup ke liye.

**4. Aaj PSP-1 poora band, uski har call fail. Dukaan pe log pay kar rahe aur sabka payment ruk raha.**
Arpan: PSP-1 band ho gaya to unka payment alag PSP-2 pe bhej do. Hamare paas fallback tareeka hoga na koi, unhe intezaar kyun karwana. Aur maan lo PSP-1 band hai lekin kuch log apna status jaanna chah rahe, to unhe thodi der rok do: "bhai abhi thoda down hai, aate-jaate hum tumhe bata denge."
Technical me: "PSP-2 pe bhej do, fallback tareeka" = fallback PSP, "status wale ko thodi der rok do" = PSP-1 pe atke payment wahin PENDING, PSP-2 pe dobara nahi; "thodi der" = 30-55 sec, phir neeche wala "Maan lo PSP-1 chup" jawab (chain me 11b) (FAILED + PSP-2, kata nikla to REFUND_PENDING).

**5. Server parchi pe INITIATED likh chuka tha, aur PSP ko call bhejte waqt hi gir gaya. Restart pe yaad nahi ki call PSP tak pahunchi ya nahi.**
Arpan: Beech me ek register rakho, usme likh lo "initiate hua". Server gira to aake dekhega: haan yaar, ye cheez likhi hui hai, chalo PSP se dobara poochte hain, "bata PSP, iska kya hua". To pehle register me likho, phir bhejo. Aur agar dobara bhejna pade to idempotency key laga denge, us key se pata pad jaayega ki do baar hua. Key process ho chuki to do baar nahi hoga.
Technical me: "pehle register me likho, phir bhejo" = PSP call se pehle INITIATED commit, "aake dekhega, PSP se dobara poochte" = restart pe job INITIATED utha ke status poochhe, "idempotency key" = payment id hi PSP ki idempotency key.

**Maan lo net atka, Arpan ne "Pay" do baar daba diya.**
Arpan: Ye idempotency key hi hai. Phone Pay dabate hi key banaye, dono request me wahi key; wo key pehle se hai to server khud reject kar dega. Wahi Chat aur Banking wali key.
Technical me: key pehle se = server reject (duplicate) · "phone ki key" = client idempotency key (app -> server), payment id = server -> PSP ki key. Do teer, dono pe key.

**Maan lo ek hi key ki do request bilkul ek saath pahunchi:**
Arpan: Key client banayega, server nahi. Do request ek saath aayi to DB atomic constraint lenge: check then act, ek waqt me ek hi key process hogi. Wo key DB me pehle se hai to doosri process hi nahi hogi.
Technical me: "atomic check then act" = key pe unique constraint (INSERT ... ek hi jeetega), doosri ko "process ho rahi / ho chuki" lautao.

**6. Arpan ke 500 hamare wallet me dukaan ke khaate me aa gaye. Ab dukaan wala ye 500 apne ICICI bank me nikalna chahta hai. ICICI hamara DB nahi, to ek transaction me nahi. Wallet se kaat liye, aur ICICI me jodte waqt PSP bola "FAILED".**
Arpan: Ulta kar do, sab ulta kar do, sab wapas paise bhej do. Maan le main HDFC bank hoon, bagal wale bank ko bhejne gaya, usne mana kar diya: "nahi bhai, bank band ho gaya, kal aana, abhi server bekaar hai." Fail ho gaya, baatcheet khatam. To main apne register me sab kaat dunga: "maine yahan se paise liye the, ise wapas kar raha hoon", aur sab wapas paise bhej dunga.

Arpan (aur neeche: wallet se 2000 Ravi ke ICICI me, ICICI ne FAILED bola): Agar Arpan ka account bhi usi ICICI me hai to badhiya, ek hi transaction me sab, fail hua to apne aap rollback, koi dikkat nahi. Worst case: Arpan ka account alag bank me hai, dono ke alag DB hain, to ek transaction nahi ban sakta. Tab saga: payment nahi hua to peeche jaate-jaate sab rollback kar do.
Technical me: "ek hi bank = ek transaction me rollback" = ek DB ke andar @Transactional / ACID; "alag DB, ek transaction nahi" = isliye saga.
Technical me: register = hamara DB · "sab ulta kar do" = saga (fail pe ulta kadam), "register me sab kaat diya" = ledger me ulti line, "wapas paise bhej dunga" = 500 wapas dukaan ke wallet me.

**7. Wallet se 500 kaate, PSP ka jawab aane me 2 minute lage. Us 2 minute me 500 na dukaan ke wallet me, na ICICI me. Hisaab wala kahega 500 gayab.**
Arpan: Hum likh lenge na bhai: "wallet se 500 kate", sabse pehle kahin likh lo. Apne ek log ya ledger me kahin bhi likh lo, to wo gayab nahi honge. Koi kuch kahe to bata denge: "haan bhai, tere paise abhi yahan hain."
Technical me: "ledger me likh lo" = ledger entry, "tere paise abhi yahan hain" = hold (beech ka khaata): SUCCESS pe hold se nikle, FAILED pe wapas wallet.

**8. Kisi ne kaha: saga ka jhanjhat kyun? Sabse pehle wallet, PSP, ICICI sabse poochh lo "taiyaar ho?", sab haan bolein tab ek saath kaat do aur jod do.**
Arpan: Aisa kar sakte hain, lekin maan le ICICI ki service dheemi pad gayi. Mere paas paanch bande hain, main poochh raha "sab taiyaar ho?". Teesra banda thoda slow hai, chaar taiyaar hain, par ek ki wajah se sab khade. Ye wahi baat hai: main ek dost ko cheating karwa raha, par wo likhne me bahut slow hai, to uski wajah se mera bhi paper chhoot jaayega.
Technical me: "sab taiyaar ho?" = 2PC (prepare round), "ek slow ki wajah se sab khade, mera bhi paper chhoot jaayega" = 2PC me ek dheema to sab ruke, isliye 2PC nahi, saga + hold.

**9. Diwali pe har second 3,000 payment, saare wallet ek hi register (DB) me. Register itni likhai nahi jhel pa raha.**
Arpan: Us register ko kaat do, chhote-chhote tukde kar do. "Le bhai, tu yahan likh, tu ye likh, tu ye likh." Kaam baant diya, dikkat khatam. Ab aaram se sab apna-apna likh rahe.
Technical me: "register ke chhote tukde" = shard, "tu ye likh, kaam baant diya" = har shard ka apna likhne wala, likhai bant gayi.

**Maan lo Diwali sale pe sirf DB nahi, sab pe pel:**
Arpan: Load balancer plus read replica, aur hum server badha denge, koi dikkat nahi. Alag-alag server hain to saga hi hamare paas.
Technical me: app server scale + LB, padhai read replica pe, likhai shard pe.

**10. Arpan ka wallet tukda 1 me, dukaan ka tukda 3 me. Ek se kaatna, doosre me jodna. Do alag register, ek transaction me nahi hoga.**
Arpan: Apne office se ek ladka pakdunga: "ye 500 mera hai, us tukde me jaana hai, use de ke aa ja, aur jo hua mujhe aake bata." Saath me ek diary rakh, usme likh "ye 500 Arpan ne diye, is tukde me dene hain". Beech me koi dikkat aaye to diary me likhta jaaye kya-kya hua. Hisaab poora hua to theek, nahi to wapas aake mere paise de dega.
Technical me: "ladka 500 le ke us tukde me" = saga do shard ke beech, "diary" = saga ka har kadam likha (beech ka khaata, hold), "wapas aake paise de dega" = fail pe ulta kadam.

**11. Ravi ka wallet top-up 500, PSP-1 30 second se chup. Pata nahi PSP-1 mara hai ya bas dheema, aur Ravi ke bank se paisa kata ya nahi. Doosra PSP khada hai.**
Arpan: Chhotu ko 500 de ke bheja, wo laut ke nahi aaya. Ho sakta hai bhaag gaya, ho sakta hai line me khada ho aur dukaan pe paise de bhi diye hon. Agar pehle se pata hai ki PSP-1 down hai, to payment wahan jaane hi mat do, seedha PSP-2. Par agar pata nahi aur paise shayad kat chuke hain, to PSP-2 ko bheja to wo bhi kaat lega. Isliye Ravi ko bolo "tumhara payment process ho raha hai", aur PSP-1 ko saaf bol do: chhotu, tere paas itna waqt hai, ya haan bol ya naa. Haan aaya to kaam ho gaya, naa aaya to Ravi ke paise wapas. Koi perfect raasta nahi hai, par Ravi ko bekaar me ghanton atka ke nahi rakhna.
Technical me: chhotu = PSP ko gayi payment request · "pehle se pata down hai" = circuit breaker OPEN, tab hi fallback PSP-2, "pata nahi kata ya nahi" = unknown state, PENDING (PSP-2 pe nahi, warna do baar katega) -> par user ko ghanton nahi rokte: 30-55 sec ke baad neeche wala "Maan lo PSP-1 chup" jawab (chain me 11b) (FAILED dikhao + PSP-2, kata nikla to REFUND_PENDING), "itna waqt, haan ya naa" = timeout + status job PSP-1 se poochhe, "naa to paise wapas" = refund (PSP bola "naa" matlab kata hi nahi, to bas FAILED, wapas kuch nahi; kata tha par haan der se aaya, to refund).

**Maan lo PSP-1 chup hai, Ravi ko ghanton nahi rok sakte, aur PSP-2 pe bhejne se darr hai ki do baar na kat jaaye.** (BookMyShow 6c wala hi jawab)
Arpan: PSP-1 kitni der lega, wo hamare haath me nahi, uska design hai. Zyada der Ravi ko nahi rok sakte. To 30 se 55 second rukte hain, phir Ravi ko saaf bolo: "payment abhi ho nahi pa raha, paisa kata hoga to apne aap wapas aa jaayega, dobara karo". Dobara wala PSP-2 pe. Pehla DB me FAILED, peeche PSP-1 se poochte raho aur uska event bhi aayega. Baad me pata chala PSP-1 pe bhi kata tha, to PSP-1 ko bolo "is user ka paisa lautao"; bank se poochhne ka haq PSP-1 ka hai, hamara nahi. Do baar kata bhi to ek wapas, Ravi ka nuksaan nahi.
Haan, onboard karte waqt PSP-1 se SLA zaroor lo: unka system kitna chalega (uptime, jaise 99.9%) aur down hua to kitni der me khabar + theek karenge. Bank kitni der lega, wo nahi. Diwali jaisi badi sale ho to 3-4 company (Razorpay jaisi doosri service) line me rakho.
Technical me: "30-55 sec" = hamara client timeout, "pehla DB me FAILED" = us koshish ka attempt FAILED (payment ka sach event / poll se), "dobara PSP-2 pe" = PSP failover, "PSP-1 se poochte + event" = status poll + webhook, "kata nikla to lautao" = DB me REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe, "onboard pe SLA" = uptime SLA + incident response (latency ka nahi), "3-4 company" = backup PSP.

&nbsp;

Poora design = [06_payment_system.md](06_payment_system.md)
