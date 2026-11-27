# Inventory: hand notes (Arpan ki online dukaan, teen godown)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ki Flipkart jaisi online dukaan hai; Ravi, Priya uske grahak. Maal teen godown me pada hai: Delhi, Mumbai, Bangalore, aur ek main warehouse Delhi me. System ka kaam bas ek: har pal sach batana ki kaunsa maal, kis godown me, kitna pada hai.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
order / Pay  -->  server  -->  DB: "1 kam karo sirf tab jab 1 ho" (ek hi kaam), sale me ginti 5 dabbon me baanti
                                  |
           dekhne wale 1 crore = cache (Redis) se, asli jaanch Pay pe · har badlaav ledger me, raat ka chhotu milaye
           return darwaze pe jaanch · 5 se kam to alert · pehle apna shehar, phir main warehouse

 1   do log ek pal me aakhri iPhone              ->  "ek kam karo sirf tab jab 1 ho" (atomic conditional update)
 1b  Buy dabaya, pay hua nahi                    ->  cart me hai, kharida nahi; ginti pay pe
 1c  dono ne pay kiya, ek ko iPhone              ->  doosre godown se de do, ya cancel + refund
 2   sale, 2 lakh Pay ek row pe, taala           ->  1000 ko 5 dabbe, har counter apna (sharded counter)
 2b  dabba 3 khaali, baaki me 150                ->  doosre dabbe pe mod do, out of stock tab jab sab 0
 2c  sab dabbe 0, ek cancel                      ->  stock me +1, jo pehle pay kare uska (restock)
 3   rack pe 48, system me 50                    ->  har bill ledger me, raat ka chhotu milaye (reconciliation)
 4   Bangalore khaali, Delhi 50, Mumbai 10       ->  pehle apna shehar, phir main warehouse (hub and spoke)
 5   1 crore sirf page dekh rahe                 ->  cache / Redis, asli jaanch Pay pe
 6   return aate hi +1, tooti screen Priya ko    ->  jaanch darwaze pe, ginti sirf bechne layak ki
 7   5 se kam, ab kya                            ->  alert kai raaston se, grahak mat roko
```

**FR / NFR / Numbers (interview me pehle 2 minute):**
```
FR      maal aaya to ginti badhao · order pe ghatao (jitna sach me hai) · cancel / wapsi pe badhao
        · kabhi bhi "kitne, kahan" · kam ho to alert
NFR     ginti me galti nahi (pakka na ho to "abhi nahi", galat "haan" nahi) · dekhne me thoda purana chalega,
        ghatana ekdum sahi · har badlaav ka hisaab
Numbers product lakhon, godown kuch hi · aam din ek product pe second me 1-2 order
        · sale ke din ek iPhone pe second me hazaaron "Pay", sab ek hi row pe
```

**FR / NFR -> kadam (Arpan: "achha hai, badhiya likh rakha hai"):** maal aaya to badhao = 3 · order pe ghatao = 1, 1b, 2, 2b ·
cancel / wapsi pe badhao = 2c, 6 · "kitne, kahan" = 4, 5 · kam ho to alert = 7 · NFR ginti me galti nahi = 1, 1c, 2, 3 ·
dekhne me thoda purana, ghatana sahi = 5 · har badlaav ka hisaab = 3.

**Round me Arpan (FR / NFR + dhaancha, copy pe):** Inventory ka main matlab: jo mere inventory me saamaan ho, wo mere app me dikhe. Kisi ne order kiya to ek kam, return kiya to ek zyada, par condition dekh ke. Saamaan becho, return lao, app me dikhao, out of stock dikhao; kabhi thoda late bhi dikh sakta, kitna late (1-2 second ya turant hatana) wo discuss kar lenge. NFR me return policy (return ka kya hisaab), kitna consistent, kaisi availability; inventory me main cheez consistency: ginti me galti na ho, ginti hamesha sahi. CAP wala kuch nahi lagana: inventory ka ek hi hub hai, apne paas nahi to peeche main warehouse / head office se poochhe, wahan bhi nahi to out of stock, jaise Amazon me hota. Stock hi khatam to availability bol ke koi matlab nahi.

Dhaancha: user app me aaya, app server, DB. iPhone book kiya: dekhenge stock me hai ya nahi, hai to book, phir delivery. Server bahut saare, DB ki replica. Baat India ki; Amazon me bhi USA ki listing India me nahi dikhti, bahar wale chahiye to thoda alag system aur CDN, yahan CDN ka matlab nahi baithta, AWS pe hosted hai. DB = AWS RDS, Postgres jaisa, jo atomic transaction me kaam kare. Numbers: ~30-40 lakh DAU. iPhone launch hua to log bas daam dekhne aate (1.5 lakh ka hai), dukaan me bheed; to bahar ek dabba rakh do, wo dabba = Redis (local cache bhi). Dekhna aur ghatana ek atomic transaction me. Cart me daalne se ginti kam nahi hoti. Return me toota-foota phone aaya to reject, theek hai to 9 se 10.

API: POST pay (kaunsa product, kitni quantity) · GET product id se (kitne bache).

*Technical me:* app server (kai) + DB (RDS Postgres, replica, atomic transaction) + Redis cache; CDN nahi (sirf India, AWS).

**E-com se aaya:** wahan "cart sirf list hai, ginti pay pe" aur "counter khaali to doosra counter" padha tha. Yahan wahi ginti apne aap me poora system hai, saare godown ke saath.

&nbsp;

---

**1. Ravi ne Delhi ke aakhri iPhone pe "Buy" dabaya, payment page pe pahunchte hi phone ki battery khatam. Payment hua hi nahi.**

**Arpan:** Payment hua nahi matlab wo cheez uske cart me hi hai, kharidi nahi gayi. Cart me daalne ka matlab ye nahi ki tumne product kharid liya; jab tak Buy karke pay nahi karoge, maal tumhara nahi. Isi waqt maine app khola, iPhone dabaya aur pay kar diya, to iPhone mera. Ravi wapas aayega to use out of stock dikhega, jo hota bhi hai aur aam baat hai.

*Technical me:* ginti Buy pe nahi, payment pe ghate (koi reserve nahi). Ghatana ek hi kaam: "ek kam karo, sirf tab jab kam se kam 1 ho" (`UPDATE ... SET qty = qty - 1 WHERE ... AND qty >= 1`, atomic conditional update), do log ek saath aayein to ek ko mila, doosre ko "nahi hai".

**Ek aur kahani (pay ho gaya, parcel kal niklega):**

**Arpan:** Inventory me ek iPhone hai. Ravi ne order karke pay kar diya; parcel main kal nikaalunga, par wo abhi bhi inventory me pada hai. To kya wo kisi ke liye available hai? Nahi. Main use pack kar dunga, Ravi ke naam ka thappa laga dunga: ye Ravi ka iPhone hai, khatam. Inventory se ek hata do, ab jo bhi aaye use out of stock dikhe. Dukaan me koi paise de ke bole "kal aake le jaunga", to dukaandaar ke andar lalach aa sakta ki doosre ko bech dun aur baad me kahin se la dun; par na mila to dukaan ki saakh kharab. Inventory se tab niklega jab book ho ke pay ho gaya.

*Technical me:* ginti pay pe ghati (upar wala atomic update), parcel nikalne ka intezaar nahi; "thappa" = order ke naam pe alag rakha maal (allocated).

**Maan lo Ravi aur Arpan dono ne aakhri iPhone cart me daala aur ek hi second me pay kar diya. Dono ke paise kate, ghatane me Arpan jeeta, Ravi ko zero row mili. Ravi ka paisa to kat chuka.**

**Arpan:** Ye design ka hissa hai aur bahut jagah hota bhi hai, Amazon, Flipkart jaisi company me sale me bahut hota. Mere paas iPhone ek hi hai.

Do raaste: ya to kahin se arrange karke doosra iPhone de do; Delhi ki inventory me nahi hai par Haryana, Punjab ki inventory me pada hai, Ravi ko to iPhone chahiye, doosri inventory se de do.

Doosra case, kahin bhi iPhone nahi hai, to bolo "tumhare paise kat gaye, iPhone ka stock khatam ho gaya, out of stock tha", aur paise refund kar do; 2, 3 ya 7 din, jo bhi bank ki timeline ho.

Amazon jaisi badi company ko ek customer naraaz ho to farak nahi padta, order cancel, refund aa raha.

Par main chhota store hoon, ya abhi apni popularity badhana chahta hoon, to kahin na kahin se iPhone arrange karke dunga; Ravi ke paise kat gaye, Ravi ko nahi pata mere paas ek hi iPhone tha, use lena-dena nahi.

*Technical me:* "doosri inventory se de do" = doosre godown se sourcing (kadam 4 wala hub), "kahin nahi to refund" = order cancel + refund (E-com wala), store ka faisla = business rule (chhota store maal laaye, bada refund kare).

---

**2. Diwali se pehle Delhi godown me 1000 naye iPhone aaye. Diwali sale, raat 12 baje, ek second me 2 lakh log "Pay", sab ek hi row pe "ek kam karo". DB taala laga ke ek-ek ko line me chalata, log 20 second se spinner dekh rahe.**

**Arpan:** Ek counter pe lambi line lagi hai aur itna saamaan hi nahi. To 1000 ko baant do, 5 line kar lo, har counter ka apna dabba: 200 ek jagah, 200 ek jagah, 200 ek jagah. Log apne dabbe se uthayenge. Sab ek hi dabbe se uthayenge to DB pe taala dobara ek hi jagah lag jaayega, isliye alag-alag tukdon me baanto aur har counter apna dabba ghatata rahe.

*Technical me:* dabba = ginti ka tukda (bucket) · stock ko tukdon me baanto (bucketing / sharded counter), har tukda alag row, taala ek row pe nahi.

**Maan lo Ravi ki request dabba 3 pe gayi, wo khaali ho chuka, baaki dabbon me 150 pade.**

**Arpan:** Out of stock tab dikhega jab 1000 ke 1000 khatam ho jaayein, ek counter khatam hone pe nahi. Ek banda ghee lene aaya, teesre counter pe ghee nahi bacha, par baaki counter pe hai, to main use mana thodi karunga; usse kahunga is counter pe aa ja, ya uski request khud us dabbe pe mod dunga. Grahak ko maal chahiye, use pata hi nahi wo kis dabbe me hai. Online me kisi ko kuch nahi dikhta, use bas iPhone chahiye.

*Technical me:* khaali bucket -> request agle bucket pe, out of stock = saare bucket 0.

**Maan lo sab dabbe 0, page pe "Out of stock". Ravi ne order cancel kiya, ek iPhone wapas. +1 kis dabbe me, aur page dekh rahi Priya ko kaise mile?**

**Arpan:** 5 dabbe isliye ki bheed kam ho; hai to mere paas 1000 hi. Offline store jaisa: board laga diya, line lambi to 5 line me baant do, paanchon billing karein, bill dikhao iPhone lo. Ek phone wapas aaya to mere stock me aayega, kisi dabbe ka nahi, ek iPhone hai. Pay karo, jisne pehle pay kiya uska.

*Technical me:* "stock me wapas, kisi dabbe ka nahi" = restock / inventory release (koi bhi dabba +1, "out of stock" hata do), "jisne pehle pay kiya uska" = ginti payment pe (koi reserve nahi, kadam 1).

**Maan lo phone hi kam pad gaye:**

**Arpan:** Ya to shuruaat me hi zyada phone mangwao, tumhe pata hai sale pe zyada log aayenge; ya jitne phone hain usse kaam chalao, khatam hone pe out of stock dikha do. Cart me add hone ka matlab ye nahi ki khareed liya.

*Technical me:* sale ka stock pehle se plan (forecast), khatam = out of stock, cart = reservation nahi.

---

**3. Mahine ke ant me manager rack pe ginta hai: 48 iPhone. System bolta 50. "Ye 2 kab gaye, kis order me, ya kabhi aaye hi nahi?" Hamare paas sirf ek number, kitne = 50.**

**Arpan:** Jaise-jaise iPhone bikta rahe, bill banta rahe, register me likhta jaun: Ravi ne ek iPhone liya, bill ban gaya. Usi waqt system me bhi chadha do.

Ya raat ko dukaan pe chhotu chhod jao: "ye aaj ke bill hain, dekh kisne kya kharida aur system me kitne bache." Sab sync rakho.

Phir bhi kabhi fark aaya to register me sab likha hai, dekh lenge ki 2 iPhone bike par likhe nahi gaye, ya likha kuch aur gaya. Ye cheez dono taraf ho sakti hai, isliye register hi sach batayega.

*Technical me:* register = stock ledger (har badlaav ki entry: kab, kitna, kis order se), raat ka chhotu = reconciliation job (ledger aur ginti ka milaan).

---

**4. Ravi Bangalore me iPhone order karta hai. Bangalore godown khaali, Delhi godown me 50, Mumbai godown me 10.**

**Arpan:** Ek main warehouse hota hai, Amazon jaisi company ka maal asal me wahin se aata hai, phir chhote godown tak pahunchta hai. Pehle dekhunga Bangalore me iPhone hai kya, hai to wahin se.

Nahi hai to main warehouse dekhunga, wahan hai to wahan se utha ke Bangalore bhej do, Ravi ko out of stock nahi dikhega. Mumbai ka iPhone Bangalore bhejna, ye FR pe hai.

FR bole "maal hai to order poora karo", to Delhi ya Mumbai se bhejo.

FR bole har shehar ka maal usi shehar ke liye, to main warehouse aur Bangalore dono khaali hone pe out of stock dikha do, kyunki kal Mumbai me 10 log aaye to unke liye wahan maal rehna chahiye.

*Technical me:* main warehouse + shehar ke godown = hub and spoke, kahan se maal uthana = fulfilment / sourcing rule (FR tay kare).

**Aur neeche (Delhi se Bangalore pahunchega kaise):**

**Arpan:** Maal Delhi se Bangalore kaise pahunchega, delivery ki baat yahan nahi karenge; delivery scope se bahar rakhte hain, kyunki ye design sirf inventory ka hai.

*Technical me:* delivery / logistics = alag system, inventory design ke scope se bahar.

---

**5. Agli sale (Republic Day) se ek din pehle iPhone ka page 1 crore log khol rahe, "sirf X bache". Har page khulne pe DB se saare dabbon ka jod. Order koi nahi, sirf log dekh rahe, phir bhi DB pe bojh.**

**Arpan:** Bahut log aa rahe aur bas daam / ginti pata karke chale jaate, dukaan me bheed aur faltu ka drama. To ek banner pe chhapwa ke bahar taang do, padh ke nikal jao, andar mat aao; kharidna ho tab andar aao.

Ya chhotu ko baar-baar andar bhej raha "bata ye kitne hain", chhotu bina matlab thak raha; to chhotu ek baar copy pe sab likh ke de de, main copy saamne khol ke bata deta hoon.

Chhotu aaram se ice cream kha raha, main bhi, DB pe koi nahi ja raha.

*Technical me:* chhotu = har baar DB se ginti poochhna · banner / copy = cache (Redis), page pe thodi purani ginti chalti hai, asli jaanch Pay pe (NFR 2).

**Ek aur kahani (Nattu kaka ki parchi, Bagha):**

**Arpan:** Asli ginti dukaan ke andar hai. Nattu kaka iPhone ki parchi le ke bahar khade: "stock me 3". Andar mere (Jethalal) paas phone aaya, teeno iPhone phone pe hi bik gaye, par Nattu kaka ki parchi pe ab bhi 3; koi aaya to wo bolenge "haan 3 hai, paise de do". To Bagha ko bhejunga: jaldi ja, teeno bik gaye, Nattu kaka parchi phaad ke fenk dein aur andar aa jaayein.

**Maan lo Bagha pahuncha bhi nahi, usi beech Priya ne parchi pe "3" dekh ke paise de diye (Pay dabaya):**

**Arpan:** Faisla hamesha Jethalal hi karega. Nattu kaka ko pehle se bol dunga: dukaan ke andar phone aa ke iPhone bik sakta hai aur tujhe bahar pata na chale; to jab bhi koi iPhone ka hisaab karne aaye, ek baar chupchaap dukaan me phone karke pooch le, ya bahar se chilla de "seth ji, iPhone hai?". Main bolunga nahi bhai, teeno ka order ho gaya. Nattu kaka "sorry behen ji, iPhone khatam" bol ke parchi wahin phaad dega, Priya chali jaayegi, kisi ka galat iPhone book nahi.

*Technical me:* parchi = cache (Redis), Bagha = ginti badalte hi cache ko turant bata do (cache invalidation), "pehle andar pooch lo" = Pay pe asli jaanch DB me (kadam 1 ka atomic update); cache sirf dekhne ke liye.

---

**6. Ravi ne iPhone return kiya, courier ne box godown pe chhoda, aate hi ginti +1. 10 minute me wahi iPhone Priya ko bika: box khula, screen tooti.**

**Arpan:** Jaanch wahin darwaze pe: delivery wale ko bol do jis haalat me gaya usi haalat me phone chahiye. Phone khol ke dekh, kaisa hai; kuch dikkat hai to return wahin reject.

Inventory me maan ke daal dene ke baad nahi; agar maan ke inventory me daal diya aur doosre user ko bik gaya to Priya naraaz. Badi company jhel le, chhoti company ke liye ye bahut badi dikkat.

Jaise mere se koi ghee ka dabba le gaya, 2-3 ghante baad lautane aaya, seal khuli, thoda ghee nikla hua; nahi lunga, ise main kisi ko bech nahi sakta. Seal pack hai to lenge, nahi to nahi.

*Technical me:* return pe doorstep quality check, ginti sirf sellable stock me badhe, kharab maal alag khaane me (damaged).

---

**7. Delhi me iPhone 5 se neeche aa gaye. Ab kya: sirf manager ko email, ya kuch aur?**

**Arpan:** 5 se kam ka matlab hamara kaam badh gaya, par grahak ko mat roko. Pehle dekho saari inventory me iPhone hai ya nahi: Bangalore se order aaya to Bangalore, phir main warehouse.

Maal hai to order chalta rahe, 5 hon ya 100 order aayein. Kitni inventory hai, ye FR me tay karenge. 2-3 inventory hain to batao "is inventory me khatam, doosri inventory me dekh lo".

Ek hi inventory hai to usme sab jagah iPhone nahi bhar sakte, aur bhi maal chahiye; to 2-3 bache tab hi har jagah siren baja do, sabko alert.

Alert kisi bhi raaste pahunche: email nahi ja raha to SMS, WhatsApp, ek email company nahi to doosri; siren matlab bas alert.

Khatam ho gaya to out of stock, aur alert "inventory khaali, jahan se deal karte ho wahan se deal karo". iPhone kahan se mangwana hai, wo manager / purchase team tay karegi, is design ka scope nahi; inventory ka kaam utna hi hai.

Naya stock jaldi aa raha ho to order lete raho, 1 mahina lagega to out of stock dikhao.

*Technical me:* had pe alert (low stock / reorder point alert), line me, order nahi rukta (Kafka event, async), kai raaste (email, SMS, WhatsApp, backup provider). Order lene se pehle saare godown ka jod (available to promise across warehouses), kitne godown = FR. Maal mangwana = purchase / procurement system, inventory ke scope se bahar. Jaldi aane wala maal ho to order lete raho (backorder), der ho to out of stock (restock lead time, FR tay kare).

**Arpan (email server down ho to):** Kuch second ruko aur alert bajwa do, poore godown me siren; 2-3 email server, ek down to doosra; SMS bhi bhej do. Alert store ke manager se upar, CTO tak bhi chala jaaye. Ek khidki band ho to darwaza.

*Technical me:* alert ki seedhi (escalation: manager -> upar), kai channel failover.

&nbsp;

Poora design = [21_inventory_system.md](21_inventory_system.md)
