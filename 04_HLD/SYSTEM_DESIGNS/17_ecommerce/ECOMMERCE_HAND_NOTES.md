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

**Isi pe ek aur raasta (round me FR / NFR):**
```
FR       1  stock dekho: DB me stock hai to app pe dikhao, warna mat dikhao (kahan se doge)
         2  cart me daalo (add karo)
         3  pay karo
         4  kabhi sale aaye to sale ki bheed sambhaalo
         main kaam do-teen: cart me kaise daalenge, aur order kaise hoga

NFR      1  do log ek hi cart na dekhein: Arpan aur Ravi ka cart alag-alag, har user ka apna
         2  ek hi stock bacha ho to do log use na le paayein
         asli dhyaan: inventory aur shelf

Scope    har shehar ka alag system (Lucknow, Bangalore, Mumbai, Pune, Goa, Delhi): Lucknow wale Lucknow se lenge,
         Delhi wale Delhi se
         Amazon nahi, Blinkit wala dark store jaisa, usi din delivery
         farak: Blinkit sab bechta (iPhone bhi), hamara area usse kaafi chhota, aur hamara apna physical store bhi

Dhaancha user -> CDN -> LB -> kai server (ek nahi) -> DB me stock dekha -> cart me add
              -> alag pay service -> PSP (Razorpay) ya COD
```
Technical me: cart user id pe (har user ka alag), dark store = paas ka store, pay = alag service + PSP, COD = payment method.

Arpan (API): API app pe hi, shelf pe koi nahi. GET stocks: har cheez ka stock, app me sab dikhe. GET stock id se: ghee ki id do, uska stock aa jaaye (500). POST cart me +1 (cart me daalo). POST pay. Ye 4-5 main API, aage aur bhi hongi.
```
API     GET  /stocks         -> saara stock, paas wale store ka (app me sab dikhe)
        GET  /stocks/{id}    -> ek cheez ka stock (ghee ki id -> 500)
        POST /cart           -> cart me +1
        POST /pay            -> PSP ko; stock -1 tab jab paisa kate
```

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      1  stock dekho (paas wale store ka), screen pe dikhe                -> kadam 1, 1b
        2  cart me daalo                                                    -> kadam 6
        3  pay karo                                                         -> kadam 8
        4  order pakka                                                      -> kadam 2
        5  sale: seemit maal; sale me ek user ko ek hi                      -> Faisla FR se, kadam 3, 7, 7c
        (pehle sawaal: Pay PSP (Razorpay jaisa) se hota; uska andar ka kaam design ka hissa nahi. Interviewer
         se poochho: PSP ka timeout kitna, ya PSP scope se bahar? Wo bole "30-60 sec bahut" to wahi timeout, uske baad
         FAILED aur alag column "kitni der baad kata" (BMS 6c jaisa))

NFR     1  ek dabba do logon ko kabhi nahi; sale me 500 maal to 500 ko hi, baaki "out of stock"
           (consistency)                                                    -> kadam 1, 2, 4, 5, 7, 7b
        2  sale ki bheed se normal orders na atkein                         -> kadam 3
        3  paisa kata to maal ya refund, net / PSP chup ho to bhi order latka na rahe  -> kadam 8, 8b-8e
Numbers Diwali 8 baje: 50 hazaar log, ghee sirf 500 · zyadatar log dekhne / cart me daalne aate, pay kam
```

**Faisla FR se (interview me pehle):** Sale me pehle hi bol do: ghee 500 hai to 500 ko hi milega, baaki ko "out of stock" dikhega. Yahan consistency chahiye: 500 hai to 500 hi bikenge, 400 hai to 400 hi, ek bhi zyada nahi. Sabko milna zaroori nahi, baaki ko "out of stock". Ye CAP wali baat nahi hai, kyunki CAP tab aata hai jab 3-4-5 system hon aur beech ka taar toote; yahan stock ki ginti ek hi jagah hai (alag dukaan, jaise Koramangala aur Whitefield, to unka stock bhi alag). FR me hi ye cut off kar do, phir design usi hisaab se.

**Nichod (Arpan):** E-commerce me sabse bada kaam inventory management hai. Wo kar liya to 90% design wahin sort, baaki to frontend pe dikhana hai.

&nbsp;

**1. Shelf pe aakhri ek ghee. Ravi ne app pe order kiya, aur usi minute aunty wahi dabba counter pe le gayi.**
Arpan: Shelf ka maal app pe mat dikhao, warna dikkat hogi. Aunty ke haath se cheen thodi lunga. Ravi ko godown ka dikhao, aunty ko shelf ka. ATM jaisa: ATM me paisa khatam hua to duniya ka paisa khatam nahi hua, gaadi se phir aa jaata hai. Godown me ek dabba hai to Ravi ko dikhega, nahi to out of stock. App sirf inventory dikhata: inventory me 100 aur shelf pe 1, to app pe 100. Ravi le to inventory se jaayega, shelf se nahi. Aunty ko inventory se matlab nahi, use kya pata inventory, wo shelf pe dekhegi.
Technical me: har raaste ka alag stock. Online = godown ki ginti, counter = shelf. Dono kabhi ek dabbe pe nahi takraate. · app = sirf inventory ki ginti.

**Aur neeche (shelf khaali ho gayi to):**
Arpan: Shelf pe ek hi tha aur aunty le gayi, shelf khaali. Aunty ko mana nahi karenge: inventory me pada hai to laake de denge. Shelf khaali ho to alert laga do; inventory me hai to chhotu wahan se laake shelf pe rakh de, nahi hai to koi dikkat nahi. Shelf pe isliye chahiye kyunki dukaan me maal tabhi bikta jab shelf pe ho, warna grahak chala jaayega.
Technical me: shelf khaali = alert (restock), inventory se shelf pe laana + likhna = kadam 2 wala ek kaam.

**Maan lo Lucknow ka store khaali, Kanpur me 20 dabbe.**
Arpan: Lucknow wale ko Lucknow ka store dikhega. Dark store ka matlab hi aas-paas ka store. Whitefield me baithe ko Koramangala wala thodi dikhega, wahan se aane ka.
Technical me: har store ka apna stock, app location dekh ke paas wala store chunta.

**2. Ladka godown se 10 dabbe shelf pe le gaya, kahin likha nahi. App pe abhi bhi 40, sach me 30.**
Arpan: Parchi pe likha hai kitna maal hai. Godown se uthaya to parchi pe kaat ke likho. Aur ye ek hi kaam bana do: godown se uthana aur parchi update karna alag kaam nahi, ek hi kaam hain. To bhoolne ka mauka hi nahi. Jaise inventory me 100 the, chhotu 50 laake shelf pe rakh gaya: register me 100 me se 50 kaat ke 50 likhe, app pe bhi 50 dikhe. Laana, le jaana, kaatna sab ek hi kaam me, taaki chhotu bhoole tab bhi dikkat na ho.
Technical me: parchi = DB. Maal hilaana aur ginti badalna ek hi transaction me (atomic), alag-alag kadam nahi.

**3. Diwali sale, 8 baje 50 hazaar log ghee pe. App atak gaya, aata-daal wale bhi atke.**
Arpan: Bheed sale wale maal ki wajah se hai. Use bagal wali khaali dukaan me le jaao. Jo sale ke liye aaya wo wahin aaye, wahin se nikle, badi dukaan me ghuse hi nahi. Aata-daal wale aaram se khareedein.
Technical me: sale ka alag raasta / alag service. Uski bheed baaki dukaan (normal orders) tak nahi pahunchti.

**Isi pe ek aur misaal (iPhone 50,000 ka 25,000 me):**
Arpan: Maine dukaan kholi, iPhone 50,000 ka 25,000 me. Socha 100-200 log aayenge, aa gaye 1000, dukaan chhoti. Jisko TV lena tha wo bhi bheed me atka. Dukaan me 150 log, unme 100 iPhone ke liye khade. Manager ko bola: iPhone alag dabbe me, alag dukaan me le jao, wahan becho; yahan TV, fridge bikega. Koi iPhone maange to bol do wahan hai, ya board laga do "iPhone lena hai to is gali me". Mausi ke mart me ghee pe yahi: ghee ki alag line, line aati jaaye, paise dete jaayein, nikalte jaayein; aata-daal wale normal me, faltu bheed nahi. Har bikri pe -1 ek hi transaction me, beech me race nahi: aakhri dabba pe do log aaye to jo pehle aaya usko, doosre ko "stock nahi". Customer naraaz hoga, ye mat bolna: 500 hai to 500 hi bikenge, 1000 ya 20,000 kahan se laayenge.
Technical me: board / gali = sale traffic ko alag service pe route karna · -1 ek transaction me = atomic decrement.

**4. Sale dukaan me ek counter, ek parchi "ghee: 500". Ek waqt me ek hi aadmi parchi chhoota, line 50 hazaar ki.**
Arpan: 10 counter bana do, har counter pe 50 ghee, line baant do. Har counter pe do aadmi: ek bechne wala, ek kaatne wala. Kaatna to haath chalane jitna tez, millisecond bhi nahi.
Technical me: counter = ginti ka ek tukda (sharded counter) · ek row pe sab toot pade = hot row. Ginti 10 tukdon me todo. Kaatna tez jagah pe (Redis me ginti, DECR).

**Maan lo Redis me ghee ki ginti kat gayi (Ravi gate ke andar aaya), par DB me order likhne se pehle server gir gaya.**
Arpan: Ab Redis kehta ek ghee kam hai, aur DB me koi order hi nahi. Sach DB hai, jaise Cache aur Banking me kaha tha. Ek banda peeche se Redis aur DB ko milata rahe: Redis me jitni ginti kati, utne order DB me hain ya nahi. Koi kati ginti bina order ke gate ki ghadi (5 minute) se zyada purani hai, matlab wo banda na pay kar paaya na order bana, to wo ghee wapas ginti me jod do, taaki doosra le sake. Jiska paisa aa gaya, uska dabba uska hi rahega.
Technical me: banda milata rahe = reconciliation job, DB sach = Redis sirf tez ginti, ghadi se purani bina order wali kati ginti wapas = compensation (gate ki 5 min ki ghadi ke baad hi, chalte hue pay ko nahi chhedna).

**Isi pe ek aur raasta (5 line, main peeche hisaab ke saath):**
Arpan: Ek hi aadmi parchi kaat raha to 500 ko 100-100 me baant do, 5 line. Har line ko bolo tere paas 100. Har line apna kaat-ti jaaye: 99, 98, 97. Main peeche 500 ki asli parchi lekar khada, poochhta rahunga kitne bike aur apna kaat-ta rahunga; kahin gadbad hui to asli parchi mere paas, main dekh lunga. Har line pe 1 bacha to mere paas total 5; agle aaye, sabke 0, khatam. Jaise 200 iPhone: 50-50, 4 line; tere kitne bike? 50 ke 50; tere? 40; tere? 30. Ya apne paas computer rakh lun: jahan bill kata, mere 500 me se bhi -1, 499, aur har line ka bacha dikhe.
Technical me: sharded counter (5 tukde) · peeche asli parchi / computer = DB sach + milaan (reconciliation).

**Aur neeche (ek phone wapas aaya to kis dabbe me):**
Arpan: 1000 iPhone 5 line me isliye baante ki bheed kam ho, hai to 1000 hi. Ek phone wapas aaya (pay fail ya lauta) to mere stock me wapas, kisi ek dabbe ka nahi; jo pehle pay kare uska.
Technical me: wapas aaya maal common pool me; sold out tabhi jab har tukda 0.

**5. Counter 3 ke 50 khatam, uski line me 2000 log. Counter 7 pe abhi 30 pade.**
Arpan: Jiska khatam hua wo counter band, aur line ko bolo jahan ghee bacha hai wahan jaao. "Khatam" tab bolo jab sabka zero ho jaaye. Counter 3 ke 0 hone pe sold out nahi: mere 500 me se 400 bache, sold out tab jab saare counter 0. Counter 3 wala aaya ki mere aage 50 aur khade hain, to unhe us line me bhejo jahan maal bacha (counter 7, counter 5). Wahan peeche lagna padega, ye trade-off hai: maal kam aur log zyada to ye hoga hi; doosre counter se maal utha ke nahi la sakte, wahan bhi bheed. Counter 7 pe 30 dabbe aur aage 10 log: 10 ko milega, 20 bachenge, aur counter 3 wale peeche lag ke apni baari ka intezaar karenge.
Technical me: counter = ginti ka tukda · ek tukda khaali to doosre tukde se lo. Sold out tabhi jab saare tukde 0. · keemat = doosri line me peeche lagna (trade-off).

**6. 500 kat gaye, "sold out" dikh gaya, par 60 logon ne pay hi nahi kiya.**
Arpan: Jab tak pay nahi hua, ginti nahi kategi. Cart me daalna sirf list hai. Cart kabhi khaali mat karo: Myntra pe joota daal ke mahine baad aao to bhi wahin dikhta. Blinkit pe mummy se list banwa raha hoon, 5 minute me sab gaayab ho gaya to kya baat hui? Galle pe panni le ke khada aadmi: jab tak paisa nahi diya aur bill nahi bana, maal uska nahi. Cart me 10 daalo, 30 daalo, 50 daalo, stock kabhi kam nahi hoga. Ravi ne cart me daala aur masti kar raha, itne me Arpan aaya, dhadadhad pay karke sab le gaya; Ravi lauta to out of stock. Stock tab kam jab pay ho, maal pe thappa lag jaaye "ye Arpan ka hai, is address pe jaayega", aur maal inventory se nikal jaaye.
Technical me: cart = sirf list, koi reserve nahi. Stock pay ke waqt katta. Fail hua to wapas (atomic DB query).

**Maan lo ek order me ghee, aata, daal, aur aata khatam.**
Arpan: Poora order mat roko. Jo hai wo bhejo, aate pe out of stock dikha do. Wo chahe to baaki do le le.
Technical me: har cheez alag check. Adhoora order chalta.

**7. 3000 log ek hi second me Pay dabaate hain, ghee 500.**
Arpan: Pay ki request hamare server pe aati hai, wahan line lagao: jo pehle aaya wo pehle paaya, duniya ka dastoor. Jiska paisa aaya, ginti kaato, dabba uska. 500 khatam hone ke baad jiska paisa aaya uska turant rollback, do minute me paisa wapas, aur "out of stock" bol do. Out of stock to dikhna hi hai, chahe pehle dikhe chahe baad me. Asli hal: sale ke liye stock pehle se zyada mangwao. Samsung pe discount lagaunga to 500 nahi, 10-20 hazaar laaunga.
Technical me: paisa aane pe server pe ek-ek karke ginti kaato, 500 ke baad aaye paise ka refund (rollback). Doosra raasta: Pay dabaate hi dabba ghadi (10 min) ke saath rok do, refund kam, par dabba ghadi tak ruka. Dono ki keemat.
**Aur neeche (line Pay se pehle kyun):**
Arpan: 3000 logon ko seedha pay nahi karne dunga; paise kaat liye aur saamaan nahi bheja to consumer court me ghaseet lenge. Isliye pehle line, ek-ek karke.
Technical me: oversell = kanooni / bharose ka khatra, isliye gate Pay se pehle.

**Ghadi wale raaste ki ek aur keemat:**
Arpan: Hold ke beech stock me us dabbe ko kya dikhaoge, +1 ya 0? Tumne stock me +1 dikhaya, aur wo uske cart me bhi hai, to kaun khareedega? Ek hi product hai aur do jagah dikh raha: ek ke cart me bhi, stock me bhi. Keemat to wahan bhi hai.

**Maan lo sabko mil hi nahi sakta:**
Arpan: Stock to khatam hoga aur kuch customer naraaz honge, sabko nahi mil sakta. Blinkit, Instamart, Amazon, Flipkart sab karte: Diwali sale pe iPhone book kar liya to baad me cancel hota hai. Ye never ending hai, iska poora solution waqai me hai nahi; jitna stock utne hi log andar, ya turant out of stock dikha do.
Technical me: "+1 ya 0" = hold ke beech dikhne wala stock (available vs reserved alag rakho), "sabko nahi mil sakta" = oversell / cancel ka 100% hal nahi, gate + jaldi out of stock.

**Isi pe ek aur raasta (stock tab kaato jab paisa kate, par andar sirf 500):**
Arpan: Mere raaste: (1) sabko line me lagao, ek ke baad ek, 499, 498; consistency rahegi, ek bhi zyada nahi bikega, bas user ko thoda time lagega. Kafka bhi laga sakte, par itni bheed me Kafka achhi cheez nahi. (2) bina line ke: har request ek hi transaction me dekhe ">= 1 hai?" aur -1 kare; ye sabse badhiya, kisi ko intezaar nahi, race kabhi nahi. Isliye SQL DB, jahan consistency aur transaction chahiye: 10 thread aaye to lock lagega, ek ek karke 499, 498. -1 tab hoga jab PSP se event aaye ki paisa kat gaya; cart me daalne ya Pay dabane se nahi. Par 3000 ko PSP pe jaane hi mat do: 500 hai to 500 ko andar lao, PSP pe 500 request hi jaayengi; Apple store Mumbai jaisa, kuch log andar, phir agla batch. 10-15 ka pay fail hua ya 50 log kuch nahi kar rahe (450 bike), to agle 50 bulao. Jo pehle pay kare use milega, ye general rule hai, Amazon Flipkart sab yahi karte. 0 hua to out of stock dikhao aur Pay ka button hata do. UI pe kya dikhana aur PSP andar kaise chalta, ye HLD ka hissa nahi.
Technical me: line = queue · ">= 1 to -1 ek transaction me" = conditional atomic update (stock >= 1 ho tabhi -1) · SQL = transaction + row lock · andar sirf stock jitne, phir agla batch = sale gate · paisa katne pe -1 = webhook pe kaatna.

**7b. Rollback me bhi pech, 10 minute wale me bhi pech. Koi screwdriver?**
(Yahan se sale ka maal ghee ki jagah iPhone: mausi ke mart me Diwali sale pe 500 iPhone aaye, Apple store jaisa, bahar 3000 log. Ghee ho ya iPhone, baat wahi: maal kam, log zyada.)
Arpan: Jitne phone utne log andar, baaki bahar. Kisi ka card decline hua ya koi chala gaya to agla batch bulao, jitne bache utne. 3000 ko andar bula ke kya fayda. App pe poora app to rok nahi sakte, aata-daal wale bhi hain. To ghee ke liye ek alag counter lagao: sale wala maal us gate se guzrega, normal checkout chalta rahega. Us counter se sirf 500 ghee guzarne hain, to dikkat hi khatam.
Technical me: alag counter = sale gate · sale gate (Buy now, cart me nahi jaata). Gate sirf utne logon ko Pay tak jaane deta jitna stock (Redis DECR). Fail / chala gaya = agla batch. Gate ke bahar walon ka paisa kabhi katta hi nahi, aur koi dabba bina bike nahi padta. Gate ek baar banta, har sale pe sirf list badalti (item + ginti + waqt).
(Normal cart pe kadam 6 wahi: ginti pay pe. Sirf sale wale item pe gate ginti pehle kaatta, isliye yahan ghadi aur wapas jodna.)

**Aur neeche (ek order me sale ka ghee + normal aata):**
Arpan: Kisi ne ek order me ghee aur aata dono liya to line nahi todunga. Checkout ek hi; ghee sale ka maal hai to sale counter se katega, aata normal counter se. Har sale pe naya counter nahi, bas us din kaunsa maal sale me hai wo badalta.
Technical me: har item ka raasta alag (sale item Redis gate se, baaki DB stock se), checkout ek.

**Maan lo Ravi ne 4 tab khole, Buy now 4 baar.**
Arpan: (Arpan, 8-Nov: "ID jodna bas, sab me ID jodna; same ID to idempotency, server reject kar dega; bas chhota connect rakh.") Gate pe user id hi key: ek user ka ek hi ghee rukega, Ravi ka pehle se ruka hai to wahi lauta do, naya mat roko. User id ek hai, to 4 tab ho, 5 ya 10, ek hi katega, 4 nahi. Ek tab se Buy now karke pay kar diya to baaki saare tab me cart 0: "order successful", khatam. Cart ko hamesha sync rakho; ye login, sign up aur user id ka khel hai.
Technical me: idempotency key (wahi ID dobara aaye to chhodo). · cart server pe user id pe (tab pe nahi), sab tab sync.

**Maan lo gate se 500 andar, Ravi Pay page pe gaya aur chai peene chala gaya, na pay kiya na cancel. Uska phone Redis me "gaya" gina hai, bahar 2500 khade.**
Arpan: Jab tak pay nahi karoge, phone bika nahi, stock me utna hi hai. Dukaan pe ghee ka dabba haath me pakad ke khada hoon, pay nahi kar raha, peeche wala dukaandar se bolta hai, to dukaandar kehta: ya to pay karke le ja, ya wapas rakh de. To andar aate hi ghadi, 5 minute. Pay kiya to phone uska, nahi kiya to wapas line me aur agla andar.
Technical me: ghadi = reservation ki umar (TTL) · "ya pay kar ya rakh de" = reservation expiry (TTL, 5 min), "wapas line me, agla andar" = release (Redis INCR) + agla batch. TTL khatam hone se ginti apne aap wapas nahi badhti; ek job (ya expiry ki khabar) INCR karti hai.

**Isi pe ek aur raasta (TTL nahi, stock paisa katne pe hi):**
Arpan: Ravi ne Google Pay ka app khola aur bahut der se kuch nahi kar raha. Uske dabbe pe TTL laga sakte hain ki wo gaayab ho ke wapas chala jaaye, par main nahi lagaunga, koi app aisa nahi karta (BookMyShow me seat HELD karte the, wahan theek). Stock 500 hi rahega jab tak paisa nahi katta; dabba tab kat-ta jab PSP bole paisa aa gaya. Aaj pay kare, kal kare, parson kare, tab tak 500; peeche log aate jaayenge, pay karte jaayenge, kisi waqt 0 hoga to cart me out of stock dikhega.
Technical me: reservation / TTL nahi; stock sirf payment success (webhook) pe kat-ta; bheed me andar sirf utne jitna stock (kadam 7 ka sale gate).

**Maan lo Ravi ki ghadi 10:05 pe khatam, phone Priya ko gaya, aur 10:05 + 1 second pe Ravi lauta aur Pay daba diya.**
Arpan: Saaf baat, 5 minute mera time. Andar pay kiya to phone Ravi ka. Code chalne me time lagta, to 1-2 second upar de sakte, zyada nahi. Uske baad pay dabaya to phone uska nahi, Priya ka; time window hai, cheez pakad ke baithe nahi reh sakte. BookMyShow me bhi aisa: seat rakh ke baitha raha to timer khatam, seat hat gayi, kisi aur ne le li, mujhe housefull. Aur agar time ke andar pay daba diya aur PSP dheema hai, to wo hamari zimmedari, user ki nahi.
Technical me: "5 min ke baad phone uska nahi" = Pay pe server check kare ki reservation abhi zinda hai (expiry check), "1-2 second upar" = chhota grace period, "andar pay, PSP dheema = hamari zimmedari" = Pay dabate hi PAYMENT_PENDING, hold ruk jaata (BMS jaisa).

**8. Net gaya / PSP chup: Ravi ko laga pay ho gaya, par PSP tak request pahunchi nahi, ya paisa kata aur PSP anjaan.**
Arpan: Design har jagah se thoda toot-ta hai. Chahe 10 minute rok lo, chahe paisa aane pe kaato, kinaare pe dikkat rehti hai. Aakhri second pe pay kiya to wahan BMS wali cheez hi leni padegi. Paisa kata to zimmedari hamari, PSP se hum baat karenge.
Technical me: PAYMENT_PENDING (BMS jaisa) + status job PSP se poochta rahe. Paisa kata mila to dabba uska; dabba na bacha ho to refund. Perfect design koi nahi, bas kitne refund.

(Payment wala hissa Myntra ki misaal se: mausi ke mart ki app bhi online dukaan hai, aur paisa wahan bhi Razorpay jaisa PSP hi kaatta. Neeche "Myntra" = hamari online dukaan.)

**Isi pe ek aur raasta (paisa kata to maal dena hamari zimmedari):**
Arpan: Ravi ke paise kat gaye to use ghee dena hamari zimmedari, PSP se hum baat kar lenge, dabbe pe chipka do "ye Ravi ka hai". Bahut companies "stock khatam" bol ke der se refund karti, par kabhi order cancel dikh bhi gaya to bolunga "tera order cancel nahi hua, ye apne paise pakdo, ghee free me de denge". Amazon shuru me aisa hi karta hoga.
Technical me: payment success = order nibhao; muavza = business ka faisla.

**Maan lo PSP 30-50 second chup, Myntra ko pata nahi Ravi ka paisa kata ya nahi.**
Arpan: Order ki khabar Myntra ki apni hai: Pay dabate hi apne paas order PENDING aur payment INITIATED likh diya, stock rok liya. Paisa kata ya nahi, ye PSP hi batayega, Myntra Ravi ka bank nahi dekh sakta. Razorpay kitni der lega, hamare haath me nahi; zyada der Ravi ko nahi rok sakte. Isliye ghadi hamari: 30-50 second, uske baad Ravi ko "time out, paisa kata to wapas aa jaayega, dobara pay karo" dikhao; dobara pay kare to Razorpay ki jagah doosre PSP pe bhejo, kyunki pata hai Razorpay abhi dheema hai. Baad me pata chala Razorpay pe bhi kata tha, to Razorpay ko bolo "lautao". Google Pay se itni baar pay kiya, 0 se 60 second me jawab aa hi jaata: ya fail, ya paisa kata; zyadatar 2 second me. Isliye had 60 second: peeche chhotu lagao jo PSP se poochhe, ya PSP ko bolo paisa katte hi event bhej de. 60 second me jawab nahi to DB me "payment fail" likh do, usse zyada wait nahi. PSP 2 minute baad bole paisa kat gaya, to ek alag column me likho kitni der me kata aur kitna, aur refund shuru karo.
Technical me: ghadi = hamara timeout · order PENDING + payment INITIATED (Myntra ka apna DB), paise ki khabar sirf PSP se (webhook / status API, din ka settlement report), hamara client timeout (50 sec), der se "success" aaya to DB me REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe -> isliye dobara pay doosre PSP pe chal jaata (PSP failover), do baar kata to ek wapas.
Arpan: Payment fail hua to cart khaali mat karo. Amazon cart tabhi khaali karta jab payment SUCCESS ho; fail hua to cart wahin pada, bas bol do "dobara pay kar do". Raaste kai: dobara QR khulwa do doosre PSP pe, ya net banking, ya cash on delivery. Ek hi raasta nahi, bahut alag-alag ho sakte.
Technical me: cart sirf payment SUCCESS pe khaali, fail pe cart + order wahin, retry options = doosra PSP (failover) / net banking / COD (payment method fallback).

**Maan lo Ravi ka paisa kat gaya, par Myntra ko ye khabar der se mile ya mile hi na. Har baar Razorpay se poochhte rahoge?** (BookMyShow 6c wala hi jawab)
Arpan: Razorpay ko bolo "paisa katte hi mujhe chhota sa event bhej do", jaise S3 file aane pe event bhejta. Event aaya = kata, hum "mil gaya" bol dete. Event raste me kho gaya to wo Razorpay ka HLD hai, wo dobara bhejega. Aur Big Billion Day jaisi sale pe 3-4 company (Razorpay jaisi doosri service) line me rakho, ek dheemi hui to doosri khadi.
Technical me: "paisa katte hi event" = webhook (PSP callback), "mil gaya" = ack (2xx), "dobara bhejega" = webhook redelivery, "3-4 company line me" = backup PSP (failover).

**Maan lo PSP ka "success" do baar aaya (ek retry se, ek status job se).**
Arpan: (Arpan, 8-Nov: "ID jodna bas, sab me ID jodna; same ID to idempotency, server reject kar dega; bas chhota connect rakh.") Payment id hi key: pehli baar aaya to order pakka, doosri baar wahi id dekh ke chhod do. Do order nahi, ginti do baar nahi kategi.
Technical me: idempotency key (wahi ID dobara aaye to chhodo).

**Maan lo aakhri ek iPhone pe Ravi aur Arpan dono ne pay kar diya:**
Arpan: Do raaste. Pehla: doosre store ki inventory dekho, Delhi me nahi to Haryana-Punjab wale store se mangwa ke Ravi ko de do, kyunki Ravi ko kya pata hamare paas ek hi tha. Doosra: kahin nahi hai to "out of stock" bol ke refund, bank ki timeline pe 2-7 din. Amazon jaisi badi company ko ek customer ke naraaz hone se farak nahi padta, wo cancel + refund karti; par chhota store hai ya naam banana hai to main kahin na kahin se arrange karke dunga.
Technical me: oversell = doosre store se pura karo (cross-store fulfilment), warna cancel + refund; business ka faisla.

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
Arpan: A, B, C teen hamare paas hain, to do me likho, do se hi poochho: kuch bhi likha to main 2 me likhunga, A aur B me; padhte waqt B aur C se poochunga, hamesha 2 pe, koi na koi common. Tune B aur C dono maar di, A akeli reh gayi; Rahul ki ek almari haan bol sakti, par condition poori nahi ho rahi. Ek raasta: B aur C ki ek copy bana do, A ki copy, B ki copy, sabki copy, to koi mar bhi jaaye to peeche chalta rahe. Par copy banane me nuksaan: apni sar-dardi aur badha di, aur D, E, F, G, H jo khaali padi hain unka use hi nahi kar rahe. To aisa karo: A, B, C hai aur D, E khaali padi mast chal rahi, to B aur C pe bhi rakho aur D, E pe bhi rakh lo. B aur C gire to D aur E apna chalte rahenge, W + R > N wali condition poori ho rahi. B, C band to D pe jao; D bhi band to F pe. Likhna A + D ya A + E me, poochhna bhi unhi do se, baat ek hi. Saari almari band ho gayi to server hi band, wo alag baat.
Technical me: "B, C gire to ring pe aage wali chalti almari D, E pe likh do" = sloppy quorum (Amazon Dynamo, leaderless replication); "har almari ki ek aur copy" = replication factor badhana (kharcha zyada, khaali almari bekaar).

**9b. Aadhe ghante baad B aur C wapas chalu. Rahul ka naya iPhone wala cart D aur E pe pada hai (jo Rahul ki almari nahi), B aur C ke paas purana cart, unhe pata hi nahi iPhone juda. Agla padhna R = 2 B aur C se hua to iPhone gayab.**
Arpan: B aur C wapas aa gaye, par unhe pata nahi iPhone hai; iPhone D aur E pe hai, jo Rahul ki almari hai hi nahi. Isme do raaste. Ek: B aur C ko kahani se hata do, wapas aayenge to shuru se; aage D aur E hi poora karenge. Par phir sawaal: kal D aur E bhi gir gaye to? Isliye doosra: B C aur D E aapas me baat karein. Beech me ek chhotu laga denge: dekhega B aur C wapas aaye ya nahi; aa gaye to D ka chhotu B aur C ke paas jaayega, bolega "tum thodi der pehle gir gaye the, tumhara kaam D ne sambhal liya tha, ye Rahul hai, iska iPhone hai, ise tum dekh lo"; phir D apna aage kaam karega. Upar se ek job chal jaayegi jo sabko sync karti rahe, poora picture.
Technical me: almari = DB ki machine (node) · "D ka chhotu B, C ke wapas aate hi unhe Rahul ka iPhone de aaye" = hinted handoff (D ke paas maal + parchi / hint ki ye asal me B, C ka hai), "sabko sync karne wali job" = anti-entropy / read repair.

**Isi pe ek aur raasta (B, C ka purana saaf, kaam D, E ke paas):**
Arpan: Ek chhotu bitha do jo dekhe B aur C wapas aaye ya nahi, warna kal B, C khade ho jaayein aur bagawat pe utar aayein to dikkat. Aaye to chhotu halla macha de aur unhe batao: tum thodi der band the, tumhara kaam D aur E ko de diya, tum chup raho; tumhare paas Rahul ka jo purana tha (cart me 10 iPhone ho to bhi) use 0 kar do. Ab line me aage khade ho jao, koi naya aayega to tumhe agla kaam milega. Jaise company: ek employee chala gaya to company nahi rukti, doosre se kaam karwa lete.
Technical me: chhotu = wapas aaye machine ko pakadne wali job · purana 0 + kaam D, E pe = ownership D, E ko (purani copy hatao).

**Padhai (reel se, R aur W ka dial):** N = 10 pe W1 R1 = bahut tez, par purana bhi mil sakta (1 + 1 < 10). W6 R5 = 11 > 10, pakka naya, par sirf 1 almari gir sakti, aur gire to likhna ruk jaata. Isliye balance tab jab R aur W dono N ke aadhe ke paas. Yahi dial consistency aur availability ke beech khud chunne deta (C side ya A side).

&nbsp;

Poora design = [17_ecommerce.md](17_ecommerce.md)
