# E-commerce: hand notes (mausi ka mart, ab online bhi)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Mausi ka mart (Reliance Smart jaisa), ab online bhi. Dukaan bhi chal rahi, counter pe billing bhi, aur app se ghar delivery bhi. Stock ek hi: wahi shelf, wahi godown.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Ravi  -->  LB  -->  normal dukaan (aata, daal)          sale ki alag dukaan (ghee 500)
                         |                                     |
              DB (order, ek hi kaam me uthana + parchi)   Redis ginti (10 counter, DECR), sale gate: stock jitne hi Pay tak
                         |                                     |  5 min ghadi, phir wapas line me
                    PSP: PAYMENT_PENDING + status job + webhook, fail pe cart wahi, kata to refund
                    paas ka store (location), DB sach, banda Redis aur DB milaye

 -   sale ka maal seemit                         ->  FR me hi cut off: 500 ko hi, baaki out of stock
 1   aakhri ghee, aunty + Ravi ek saath          ->  online ko godown, counter ko shelf (alag stock)
 1b  Lucknow ka store khaali, Kanpur me 20       ->  location se paas ka store (dark store)
 2   ladka uthake likhna bhool gaya              ->  uthana + parchi ek kaam (atomic transaction)
 3   sale ki bheed, aata-daal bhi atka           ->  sale ki alag dukaan (alag service)
 4   ek parchi pe 50 hazaar                      ->  10 counter, Redis DECR (hot row todo)
 4b  Redis me ginti kati, DB me order nahi       ->  DB sach, banda milaye (reconciliation)
 5   counter 3 khatam, counter 7 pe 30           ->  khaali counter band, sold out tab jab sab 0
 6   60 ne pay nahi kiya                         ->  cart sirf list, ginti pay pe
 6b  order me aata khatam                        ->  jo hai wo bhejo, aate pe out of stock
 7   3000 ek second me Pay                       ->  server pe line, 500 ke baad rollback (ya ghadi wala hold)
 7b  rollback + ghadi dono me pech               ->  Apple store jaisa sale gate
 7c  4 tab, Buy now 4 baar                       ->  user id pe ek (idempotency)
 7d  andar gaya, chai peene chala gaya           ->  5 min ghadi, phir wapas (expiry)
 7e  ghadi khatam ke 1 sec baad Pay              ->  1-2 sec grace, andar dabaya to PSP hamari zimmedari
 8   net gaya / PSP chup                         ->  PAYMENT_PENDING + status job, refund
 8b-e  PSP 50 sec chup, kata kaise pata, success do baar, fail pe cart  ->  ghadi, webhook, payment id, cart wahi
 9   sale pe cart ki almari B, C giri           ->  ring pe aage wali chalti almari pe likho (sloppy quorum)
 9b  B, C wapas, iPhone D, E pe                  ->  D ka chhotu lauta de + sync job (hinted handoff, anti-entropy)
```

**FR / NFR kahan se:** upar ki dikkat chain dekh ke nikale; har line ke aage likha hai wo kis kadam se aayi.
Arpan: E-commerce ka matlab: stock dekho aur screen pe dikhao, phir cart me daalo, pay karo, order pakka. Stock pehle aana chahiye. "Ek order me kai cheez, ek khatam to baaki bhejo" FR me nahi, usse design nahi badalta (Hello Interview: FR / NFR me wahi jo design me farak laaye); sale wale se badalta hai. Sale me 500 hai to 500 ko hi (Apple store wali kahani, apni banayi isliye aaj bhi yaad).
Technical me: FR = browse / stock -> cart -> checkout -> order; NFR = consistency (oversell nahi), alag sale service, payment latka na rahe.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      1  stock dekho (paas wale store ka), screen pe dikhe                -> kadam 1, 1b
        2  cart me daalo                                                    -> kadam 6
        3  pay karo                                                         -> kadam 8
        4  order pakka                                                      -> kadam 2
        5  sale: seemit maal; sale me ek user ko ek hi                      -> Faisla FR se, kadam 3, 7, 7c

NFR     1  ek dabba do logon ko kabhi nahi (consistency)                    -> kadam 1, 2, 4
        2  sale me 500 maal hai to wo 500 logon ko hi mile, baaki ko "out of stock"
           (availability nahi, ye FR me hi cut off)                         -> Faisla FR se, kadam 4, 5, 7, 7b
        3  sale ki bheed se normal orders na atkein                         -> kadam 3
        4  paisa kata to maal ya refund, net / PSP chup ho to bhi order latka na rahe  -> kadam 8, 8b-8e
Numbers Diwali 8 baje: 50 hazaar log, ghee sirf 500 · zyadatar log dekhne / cart me daalne aate, pay kam
```

**Faisla FR se (interview me pehle):** Sale me pehle hi bol do: ghee 500 hai to 500 ko hi milega, baaki ko "out of stock" dikhega. Yahan consistency chahiye: 500 hai to 500 hi bikenge, 400 hai to 400 hi, ek bhi zyada nahi. Sabko milna zaroori nahi, baaki ko "out of stock". Ye CAP wali baat nahi hai, kyunki CAP tab aata hai jab 3-4-5 system hon aur beech ka taar toote; yahan stock ki ginti ek hi jagah hai (alag dukaan, jaise Koramangala aur Whitefield, to unka stock bhi alag). FR me hi ye cut off kar do, phir design usi hisaab se.

**Nichod (Arpan):** E-commerce me sabse bada kaam inventory management hai. Wo kar liya to 90% design wahin sort, baaki to frontend pe dikhana hai.

&nbsp;

**1. Shelf pe aakhri ek ghee. Ravi ne app pe order kiya, aur usi minute aunty wahi dabba counter pe le gayi.**
Arpan: Shelf ka maal app pe mat dikhao, warna dikkat hogi. Aunty ke haath se cheen thodi lunga. Ravi ko godown ka dikhao, aunty ko shelf ka. ATM jaisa: ATM me paisa khatam hua to duniya ka paisa khatam nahi hua, gaadi se phir aa jaata hai. Godown me ek dabba hai to Ravi ko dikhega, nahi to out of stock.
Technical me: har raaste ka alag stock. Online = godown ki ginti, counter = shelf. Dono kabhi ek dabbe pe nahi takraate.

**Maan lo Lucknow ka store khaali, Kanpur me 20 dabbe.**
Arpan: Lucknow wale ko Lucknow ka store dikhega. Dark store ka matlab hi aas-paas ka store. Whitefield me baithe ko Koramangala wala dikhta.
Technical me: har store ka apna stock, app location dekh ke paas wala store chunta.

**2. Ladka godown se 10 dabbe shelf pe le gaya, kahin likha nahi. App pe abhi bhi 40, sach me 30.**
Arpan: Parchi pe likha hai kitna maal hai. Godown se uthaya to parchi pe kaat ke likho. Aur ye ek hi kaam bana do: godown se uthana aur parchi update karna alag kaam nahi, ek hi kaam hain. To bhoolne ka mauka hi nahi.
Technical me: parchi = DB. Maal hilaana aur ginti badalna ek hi transaction me (atomic), alag-alag kadam nahi.

**3. Diwali sale, 8 baje 50 hazaar log ghee pe. App atak gaya, aata-daal wale bhi atke.**
Arpan: Bheed sale wale maal ki wajah se hai. Use bagal wali khaali dukaan me le jaao. Jo sale ke liye aaya wo wahin aaye, wahin se nikle, badi dukaan me ghuse hi nahi. Aata-daal wale aaram se khareedein.
Technical me: sale ka alag raasta / alag service. Uski bheed baaki dukaan (normal orders) tak nahi pahunchti.

**4. Sale dukaan me ek counter, ek parchi "ghee: 500". Ek waqt me ek hi aadmi parchi chhoota, line 50 hazaar ki.**
Arpan: 10 counter bana do, har counter pe 50 ghee, line baant do. Har counter pe do aadmi: ek bechne wala, ek kaatne wala. Kaatna to haath chalane jitna tez, millisecond bhi nahi.
Technical me: ek row pe sab toot pade = hot row. Ginti 10 tukdon me todo. Kaatna tez jagah pe (Redis me ginti, DECR).

**Maan lo Redis me ghee ki ginti kat gayi (Ravi gate ke andar aaya), par DB me order likhne se pehle server gir gaya.**
Arpan: Ab Redis kehta ek ghee kam hai, aur DB me koi order hi nahi. Sach DB hai, jaise Cache aur Banking me kaha tha. Ek banda peeche se Redis aur DB ko milata rahe: Redis me jitni ginti kati, utne order DB me hain ya nahi. Koi kati ginti bina order ke gate ki ghadi (5 minute) se zyada purani hai, matlab wo banda na pay kar paaya na order bana, to wo ghee wapas ginti me jod do, taaki doosra le sake. Jiska paisa aa gaya, uska dabba uska hi rahega.
Technical me: banda milata rahe = reconciliation job, DB sach = Redis sirf tez ginti, ghadi se purani bina order wali kati ginti wapas = compensation (gate ki 5 min ki ghadi ke baad hi, chalte hue pay ko nahi chhedna).

**5. Counter 3 ke 50 khatam, uski line me 2000 log. Counter 7 pe abhi 30 pade.**
Arpan: Jiska khatam hua wo counter band, aur line ko bolo jahan ghee bacha hai wahan jaao. "Khatam" tab bolo jab sabka zero ho jaaye.
Technical me: ek tukda khaali to doosre tukde se lo. Sold out tabhi jab saare tukde 0.

**6. 500 kat gaye, "sold out" dikh gaya, par 60 logon ne pay hi nahi kiya.**
Arpan: Jab tak pay nahi hua, ginti nahi kategi. Cart me daalna sirf list hai. Cart kabhi khaali mat karo: Myntra pe joota daal ke mahine baad aao to bhi wahin dikhta. Blinkit pe mummy se list banwa raha hoon, 5 minute me sab gaayab ho gaya to kya baat hui? Galle pe panni le ke khada aadmi: jab tak paisa nahi diya aur bill nahi bana, maal uska nahi.
Technical me: cart = sirf list, koi reserve nahi. Stock pay ke waqt katta. Fail hua to wapas (atomic DB query).

**Maan lo ek order me ghee, aata, daal, aur aata khatam.**
Arpan: Poora order mat roko. Jo hai wo bhejo, aate pe out of stock dikha do. Wo chahe to baaki do le le.
Technical me: har cheez alag check. Adhoora order chalta.

**7. 3000 log ek hi second me Pay dabaate hain, ghee 500.**
Arpan: Pay ki request hamare server pe aati hai, wahan line lagao: jo pehle aaya wo pehle paaya, duniya ka dastoor. Jiska paisa aaya, ginti kaato, dabba uska. 500 khatam hone ke baad jiska paisa aaya uska turant rollback, do minute me paisa wapas, aur "out of stock" bol do. Out of stock to dikhna hi hai, chahe pehle dikhe chahe baad me. Asli hal: sale ke liye stock pehle se zyada mangwao. Samsung pe discount lagaunga to 500 nahi, 10-20 hazaar laaunga.
Technical me: paisa aane pe server pe ek-ek karke ginti kaato, 500 ke baad aaye paise ka refund (rollback). Doosra raasta: Pay dabaate hi dabba ghadi (10 min) ke saath rok do, refund kam, par dabba ghadi tak ruka. Dono ki keemat.

**7b. Rollback me bhi pech, 10 minute wale me bhi pech. Koi screwdriver?**
(Yahan se sale ka maal ghee ki jagah iPhone: mausi ke mart me Diwali sale pe 500 iPhone aaye, Apple store jaisa, bahar 3000 log. Ghee ho ya iPhone, baat wahi: maal kam, log zyada.)
Arpan: Jitne phone utne log andar, baaki bahar. Kisi ka card decline hua ya koi chala gaya to agla batch bulao, jitne bache utne. 3000 ko andar bula ke kya fayda. App pe poora app to rok nahi sakte, aata-daal wale bhi hain. To ghee ke liye ek alag counter lagao: sale wala maal us gate se guzrega, normal checkout chalta rahega. Us counter se sirf 500 ghee guzarne hain, to dikkat hi khatam.
Technical me: sale gate (Buy now, cart me nahi jaata). Gate sirf utne logon ko Pay tak jaane deta jitna stock (Redis DECR). Fail / chala gaya = agla batch. Gate ke bahar walon ka paisa kabhi katta hi nahi, aur koi dabba bina bike nahi padta. Gate ek baar banta, har sale pe sirf list badalti (item + ginti + waqt).
(Normal cart pe kadam 6 wahi: ginti pay pe. Sirf sale wale item pe gate ginti pehle kaatta, isliye yahan ghadi aur wapas jodna.)

**Maan lo Ravi ne 4 tab khole, Buy now 4 baar.**
Arpan: (Arpan, 8-Nov: "ID jodna bas, sab me ID jodna; same ID to idempotency, server reject kar dega; bas chhota connect rakh.") Gate pe user id hi key: ek user ka ek hi ghee rukega, Ravi ka pehle se ruka hai to wahi lauta do, naya mat roko.
Technical me: idempotency key (wahi ID dobara aaye to chhodo).

**Maan lo gate se 500 andar, Ravi Pay page pe gaya aur chai peene chala gaya, na pay kiya na cancel. Uska phone Redis me "gaya" gina hai, bahar 2500 khade.**
Arpan: Jab tak pay nahi karoge, phone bika nahi, stock me utna hi hai. Dukaan pe ghee ka dabba haath me pakad ke khada hoon, pay nahi kar raha, peeche wala dukaandar se bolta hai, to dukaandar kehta: ya to pay karke le ja, ya wapas rakh de. To andar aate hi ghadi, 5 minute. Pay kiya to phone uska, nahi kiya to wapas line me aur agla andar.
Technical me: "ya pay kar ya rakh de" = reservation expiry (TTL, 5 min), "wapas line me, agla andar" = release (Redis INCR) + agla batch. TTL khatam hone se ginti apne aap wapas nahi badhti; ek job (ya expiry ki khabar) INCR karti hai.

**Maan lo Ravi ki ghadi 10:05 pe khatam, phone Priya ko gaya, aur 10:05 + 1 second pe Ravi lauta aur Pay daba diya.**
Arpan: Saaf baat, 5 minute mera time. Andar pay kiya to phone Ravi ka. Code chalne me time lagta, to 1-2 second upar de sakte, zyada nahi. Uske baad pay dabaya to phone uska nahi, Priya ka; time window hai, cheez pakad ke baithe nahi reh sakte. BookMyShow me bhi aisa: seat rakh ke baitha raha to timer khatam, seat hat gayi, kisi aur ne le li, mujhe housefull. Aur agar time ke andar pay daba diya aur PSP dheema hai, to wo hamari zimmedari, user ki nahi.
Technical me: "5 min ke baad phone uska nahi" = Pay pe server check kare ki reservation abhi zinda hai (expiry check), "1-2 second upar" = chhota grace period, "andar pay, PSP dheema = hamari zimmedari" = Pay dabate hi PAYMENT_PENDING, hold ruk jaata (BMS jaisa).

**8. Net gaya / PSP chup: Ravi ko laga pay ho gaya, par PSP tak request pahunchi nahi, ya paisa kata aur PSP anjaan.**
Arpan: Design har jagah se thoda toot-ta hai. Chahe 10 minute rok lo, chahe paisa aane pe kaato, kinaare pe dikkat rehti hai. Aakhri second pe pay kiya to wahan BMS wali cheez hi leni padegi. Paisa kata to zimmedari hamari, PSP se hum baat karenge.
Technical me: PAYMENT_PENDING (BMS jaisa) + status job PSP se poochta rahe. Paisa kata mila to dabba uska; dabba na bacha ho to refund. Perfect design koi nahi, bas kitne refund.

(Payment wala hissa Myntra ki misaal se: mausi ke mart ki app bhi online dukaan hai, aur paisa wahan bhi Razorpay jaisa PSP hi kaatta. Neeche "Myntra" = hamari online dukaan.)

**Maan lo PSP 30-50 second chup, Myntra ko pata nahi Ravi ka paisa kata ya nahi.**
Arpan: Order ki khabar Myntra ki apni hai: Pay dabate hi apne paas order PENDING aur payment INITIATED likh diya, stock rok liya. Paisa kata ya nahi, ye PSP hi batayega, Myntra Ravi ka bank nahi dekh sakta. Razorpay kitni der lega, hamare haath me nahi; zyada der Ravi ko nahi rok sakte. Isliye ghadi hamari: 30-50 second, uske baad Ravi ko "time out, paisa kata to wapas aa jaayega, dobara pay karo" dikhao; dobara pay kare to Razorpay ki jagah doosre PSP pe bhejo, kyunki pata hai Razorpay abhi dheema hai. Baad me pata chala Razorpay pe bhi kata tha, to Razorpay ko bolo "lautao".
Technical me: order PENDING + payment INITIATED (Myntra ka apna DB), paise ki khabar sirf PSP se (webhook / status API, din ka settlement report), hamara client timeout (50 sec), der se "success" aaya to DB me REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe -> isliye dobara pay doosre PSP pe chal jaata (PSP failover), do baar kata to ek wapas.
Arpan: Payment fail hua to cart khaali mat karo. Amazon cart tabhi khaali karta jab payment SUCCESS ho; fail hua to cart wahin pada, bas bol do "dobara pay kar do". Raaste kai: dobara QR khulwa do doosre PSP pe, ya net banking, ya cash on delivery. Ek hi raasta nahi, bahut alag-alag ho sakte.
Technical me: cart sirf payment SUCCESS pe khaali, fail pe cart + order wahin, retry options = doosra PSP (failover) / net banking / COD (payment method fallback).

**Maan lo Ravi ka paisa kat gaya, par Myntra ko ye khabar der se mile ya mile hi na. Har baar Razorpay se poochhte rahoge?** (BookMyShow 6c wala hi jawab)
Arpan: Razorpay ko bolo "paisa katte hi mujhe chhota sa event bhej do", jaise S3 file aane pe event bhejta. Event aaya = kata, hum "mil gaya" bol dete. Event raste me kho gaya to wo Razorpay ka HLD hai, wo dobara bhejega. Aur Big Billion Day jaisi sale pe 3-4 company (Stripe, Amazon Pay) line me rakho, ek dheemi hui to doosri khadi.
Technical me: "paisa katte hi event" = webhook (PSP callback), "mil gaya" = ack (2xx), "dobara bhejega" = webhook redelivery, "3-4 company line me" = backup PSP (failover).

**Maan lo PSP ka "success" do baar aaya (ek retry se, ek status job se).**
Arpan: (Arpan, 8-Nov: "ID jodna bas, sab me ID jodna; same ID to idempotency, server reject kar dega; bas chhota connect rakh.") Payment id hi key: pehli baar aaya to order pakka, doosri baar wahi id dekh ke chhod do. Do order nahi, ginti do baar nahi kategi.
Technical me: idempotency key (wahi ID dobara aaye to chhodo).

**URL 10 / Twitter 19 se aaya:** teen copy A, B, C, do me likho do se padho (quorum, R + W > N). Neeche sirf naya. (Reel se: Amazon Dynamo, leader hi nahi.)

```
 ring pe 10 almari, Rahul ka cart = A, B, C      N = 3 · W = 2 · R = 2 (2 + 2 > 3)

 9   sale, raat 12 baje, B aur C band:
        A [chalu]   B [X]   C [X]   D [chalu]   E [chalu]  ...
        likhna:  A + D  (D = aage wali chalti almari, parchi: "ye B ka hai")  ->  W = 2 poora, Add to Cart chala

 9b  aadhe ghante baad B, C wapas:
        D ka chhotu  --"tum gire the, ye Rahul ka iPhone"-->  B, C       phir D apni parchi wali copy chhod de
        upar se ek job sab almari milaati rahe (jo parchi kho gayi wo bhi pakdi jaaye)
```

**9. Diwali sale aa gayi. Mausi ne bahut saara maal mangwa liya, sab ek hi godown me padta to wahan ek saath sabki bheed lag jaati. Isliye godown ko 10 hisson (almari) me tod diya, URL / cache wali ring pe, sab alag-alag jagah se lein. Maal ke saath grahakon ki cart-list bhi in 10 almari pe baant di; har grahak ka cart 3 almari pe copy, taaki ek almari gire to bhi cart na khoye: likhna 2 ki "haan" pe pakka, padhna 2 se. Usi sale ki raat 12 baje Rahul (naya grahak, pehli baar app pe) aaya. Ring pe 10 almari, Rahul ka cart teen almari A, B, C pe (N = 3, W = 2, R = 2, 2 + 2 > 3). Rahul ne iPhone pe "Add to Cart" dabaya, usi waqt B aur C dono band (mausi ko B aur C me kuch dikkat lagi, band karni padi). Sirf A chalu, 2 ki "haan" milegi hi nahi, D se J mast chal rahi par wo Rahul ki almari nahi. Kuch na kiya to "Add to Cart fail".**
Arpan: A, B, C teen hamare paas hain, to do me likho, do se hi poochho: kuch bhi likha to main 2 me likhunga, A aur B me; padhte waqt B aur C se poochunga, hamesha 2 pe, koi na koi common. Tune B aur C dono maar di, A akeli reh gayi; Rahul ki ek almari haan bol sakti, par condition poori nahi ho rahi. Ek raasta: B aur C ki ek copy bana do, A ki copy, B ki copy, sabki copy, to koi mar bhi jaaye to peeche chalta rahe. Par copy banane me nuksaan: apni sar-dardi aur badha di, aur D, E, F, G, H jo khaali padi hain unka use hi nahi kar rahe. To aisa karo: A, B, C hai aur D, E khaali padi mast chal rahi, to B aur C pe bhi rakho aur D, E pe bhi rakh lo. B aur C gire to D aur E apna chalte rahenge, W + R > N wali condition poori ho rahi.
Technical me: "B, C gire to ring pe aage wali chalti almari D, E pe likh do" = sloppy quorum (Amazon Dynamo, leaderless replication); "har almari ki ek aur copy" = replication factor badhana (kharcha zyada, khaali almari bekaar).

**9b. Aadhe ghante baad B aur C wapas chalu. Rahul ka naya iPhone wala cart D aur E pe pada hai (jo Rahul ki almari nahi), B aur C ke paas purana cart, unhe pata hi nahi iPhone juda. Agla padhna R = 2 B aur C se hua to iPhone gayab.**
Arpan: B aur C wapas aa gaye, par unhe pata nahi iPhone hai; iPhone D aur E pe hai, jo Rahul ki almari hai hi nahi. Isme do raaste. Ek: B aur C ko kahani se hata do, wapas aayenge to shuru se; aage D aur E hi poora karenge. Par phir sawaal: kal D aur E bhi gir gaye to? Isliye doosra: B C aur D E aapas me baat karein. Beech me ek chhotu laga denge: dekhega B aur C wapas aaye ya nahi; aa gaye to D ka chhotu B aur C ke paas jaayega, bolega "tum thodi der pehle gir gaye the, tumhara kaam D ne sambhal liya tha, ye Rahul hai, iska iPhone hai, ise tum dekh lo"; phir D apna aage kaam karega. Upar se ek job chal jaayegi jo sabko sync karti rahe, poora picture.
Technical me: "D ka chhotu B, C ke wapas aate hi unhe Rahul ka iPhone de aaye" = hinted handoff (D ke paas maal + parchi / hint ki ye asal me B, C ka hai), "sabko sync karne wali job" = anti-entropy / read repair.

**Padhai (reel se, R aur W ka dial):** N = 10 pe W1 R1 = bahut tez, par purana bhi mil sakta (1 + 1 < 10). W6 R5 = 11 > 10, pakka naya, par sirf 1 almari gir sakti, aur gire to likhna ruk jaata. Isliye balance tab jab R aur W dono N ke aadhe ke paas. Yahi dial consistency aur availability ke beech khud chunne deta (C side ya A side).

&nbsp;

Poora design = [17_ecommerce.md](17_ecommerce.md)
