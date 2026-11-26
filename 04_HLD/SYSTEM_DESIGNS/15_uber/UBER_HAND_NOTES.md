# Uber: hand notes (Connaught Place se ghar)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan shaam 7 baje Connaught Place pe khada hai, ghar ke liye Uber book karna hai. Sheher me 5 lakh driver ghoom rahe hain.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Raju (phone har 4 sec)  -->  Location Service  -->  Redis (abhi ki jagah, geohash dabbe) + Kafka -> raasta batch me Cassandra
                                                          |
Arpan "Book"  -->  matching: apna + aas-paas ke dabbe, sadak ki doori (ETA), 15-20 sec ki booking saath (batch)
                    driver pe parchi (hold, 15 sec umar, ghadi server ki) · bheed me line + surge
                    Raju live dikhe: WebSocket + pub/sub · trip khatam: kiraya jo dikhaya, poora kaato

 1   server ko pata nahi driver kahan            ->  phone khud bheje (Location Service)
 2   1.25 lakh update / sec, DB ghuta            ->  Cassandra; abhi ki jagah Redis, raasta Kafka (2b)
 2c  matching ko abhi chahiye, bhare ka raasta     ->  Redis se matching; sawari ka raasta Cassandra (SOS)
 3   5 lakh driver ka fasla naap raha            ->  naksha dabbon me (geohash)
 4   dabbe ke kinare, bagal me driver            ->  aas-paas ke dabbe bhi; nadi ke us paar = sadak se doori (4b)
 5   Raju pe ek second me do ride                ->  parchi, ek kadam me (hold / lock); 15 sec umar, net atka = wahi booking id
 6   concert khatam, 50 hazaar, cab 500          ->  line (virtual queue) + kiraya badhao (surge); stadium dabba = 2-3 extra
 6c  dabba toda, Arpan ke tukde me driver nahi   ->  aas-paas ke tukde bhi (scatter-gather)
 7   14.9 pe accept, server pe 16 pe             ->  ghadi server ki; 7b sabko bajao ya 20-25 sec
 8   Raju Janpath me, purana naam pada           ->  sach wahi jo GPS ne abhi bheja
 9   register driver ke naam se                  ->  jagah ke hisaab se (geohash Redis set), dabba badla to hatao + daalo
10   10 lakh phone "Raju kahan?"                 ->  push, WebSocket
11   Arpan server 12 pe, Raju 57 pe              ->  pub/sub
12   250 dikha, aakhir me 500 kate               ->  jo dikhaya wahi (upfront fare)
12b  900 dikha, traffic 2.5 ghante, 1400 kate     ->  laal batti pe nahi, sach me der to +100 / 30 min
13   wallet me 40, gateway chup                  ->  poora kaato; ride id key; 30-55 sec phir GPay / cash; PAYMENT_PENDING
13b  cash diya, 5 min baad GPay se bhi kata       ->  "kitni der baad kata" column, raat ki job lautaye
14   Raju ka phone band, naksha pe gayab         ->  location timeout
15   Car A Arpan se 2 km, Ravi se 1 km           ->  sab saath milao, kul doori kam (batch matching, 15-20 sec window)
15b  Arpan 7:00:00, Ravi 7:00:02                 ->  turant mat do, 15-20 sec ruk ke sab saath milao
15c  Arpan baar-baar peeche                      ->  aging, agli gaadi usi ki
15d  cancel karke turant dobara book             ->  Redis me "pehle kab ruka" + 30 sec umar, warna peeche
16   signal pe khada, har 4 sec wahi jagah       ->  15-20 sec (adaptive ping); 16b-d chhat pe / nadi paar / tunnel / basement
```

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      driver ki jagah lo · paas ka khaali driver do · ek driver ek sawari
        · Arpan ko Raju live dikhe · kiraya pehle dikhe · trip khatam pe pay
        · bheed me (concert) kiraya badh sakta hai, aur wo pehle hi dikh jaaye
        · ek saath kai booking aayein to aisi gaadi do ki sabki milake doori kam · jo der se ruka wo baar-baar peeche na rahe
        · traffic me sach me bahut der fanse to kiraya badh sakta, laal batti pe nahi (kadam 12 doosri kahani)
        (pehle sawaal: trip ka paisa PSP (Razorpay jaisa) kaatta; uska andar ka kaam design ka hissa nahi.
         Interviewer se poochho: PSP ka timeout kitna, ya PSP scope se bahar? Wo bole "30-60 sec bahut" to wahi timeout,
         uske baad FAILED aur alag column "kitni der baad kata" (kadam 13b))
NFR     driver turant mile (kuch second) · ek driver do jagah kabhi nahi
        · ride ke beech driver gaya to sambhalo
        · sawari wali ride ka poora raasta pakka rahe (SOS / shikayat), khaali driver ka zaroori nahi (kadam 2)
        · gateway chup ho to customer / driver atke nahi (30 sec, phir doosra raasta) (kadam 13)
        · concert jaisi bheed ho to bhi sambhle (kadam 6)
Numbers 5 lakh driver, har 4 sec jagah -> ~1.25 lakh likhai / sec · booking shaam ko hazaaron / sec
        -> likhai bahut zyada (jagah), booking pe paas wale jaldi chhaantne hain
        ~50 million user · peak (concert) 50-60 hazaar ek saath, normal din me bheed badi baat nahi
        ("driver tez aaye" na FR na NFR: traffic hamare haath me nahi)
API     GET  aas-paas ke driver  -> app khulte hi naksha pe    (FR driver ki jagah, paas ka khaali)
                       POST book                -> kahan jaana, kiraya dikhe  (FR ek driver ek sawari, kiraya pehle)
                       POST pay                 -> trip khatam pe paisa       (FR trip khatam pe pay)
        driver ki jagah = phone ka har 4 sec wala bhejna, pehle se design me (kadam 1)
```

**FR / NFR -> kadam (Arpan: "theek lag raha, achha lag raha"):** driver ki jagah = 1, 2, 2b, 8, 14, 16 · paas ka khaali driver =
3, 4, 4b, 6c, 9 · ek driver ek sawari = 5, 7 · Raju live = 10, 11, 16b-16d · kiraya pehle = 12 · trip khatam pe pay = 13 ·
bheed me kiraya = 6 · sabki milake doori kam = 15, 15b · der se ruka peeche na rahe = 15c, 15d · NFR turant mile = 3, 4, 7b ·
ek driver do jagah nahi = 5, 7 · ride ke beech driver gaya = 14, 16d.

**Dhaancha (round, copy pe):** Arpan ne Uber khola, use Whitefield ke aas-paas ke saare driver dikhne chahiye. Book dabaya to request server pe gayi, server ne andar process kiya, DB me likha, wapas aayi, Arpan ko ride mil gayi. Uber ke paas ek nahi, bahut saare server; DB ki replica. CDN aur S3 ki zaroorat nahi, inka yahan koi lena-dena nahi. Redis kahan lagega, wo aage dekhenge.
Technical me: client -> kai app server (LB ke peeche) -> DB + read replica; CDN / S3 nahi (koi badi file nahi).

&nbsp;

**1. "Book" dabaya, par server ko pata hi nahi kaunsa driver abhi kahan hai.**
Arpan: Location to phone me khud hai, GPS ka latitude-longitude. Phone khud bhejega, har hilne pe nahi, thodi-thodi der me. Beech me ek dabba rakh lo jiska kaam sirf ek: har driver ki sabse taaza jagah batana.
Technical me: "beech ka dabba" = Location Service, phone har ~4 sec (maan lo) lat/long bhejta.

**2. Har 4 sec, 5 lakh driver = 1.25 lakh update / sec. Normal DB ghut gaya, jagah 30 sec peeche.**
Arpan: Ya to chhoti parchi, kaat ke nayi jagah likho; ya bada dabba jo likhne me tez ho. Crore-crore likhai pe parchi dheemi ho sakti, gir sakti, aur kab tak replica lagaunga, kal driver aur badhenge. To bada bhai bulao, jo isi kaam ke liye bana hai, apni copies khud rakhta: Cassandra.
Technical me: chhoti parchi = Redis (kaat ke nayi likho = overwrite) · "bada dabba" = Cassandra (likhai tez, apni copies, machine jodo to badhta; andar ring = consistent hashing).

**Aur neeche (Zomato wali reel se): Raju ki har nayi jagah DB me nayi line ban ke jud rahi. Arpan ke map ko sirf abhi ki jagah chahiye, par DB laakhon purani lines me se sabse nayi dhoondh raha, map dheema. Aur company ko baad me poora raasta bhi chahiye (kis raaste gaya, kahan ruka), purani lines phenk nahi sakte.**
Arpan: DB me location likhne ka koi matlab nahi, ye to faltu ki cheez ho gayi. DB me to bas wo likhenge jo kaam ki hai: haan bhai tumhara driver pahunch gaya, order delivered, order pending.

Location to ek parchi pe daal do: 4 second baad request aayi, purani kaat ke nayi likh di, kaat ke likh di, jisse parchi bhi na bhare. Delivery ho jaaye to Redis se hata do, use rakhne ka koi matlab nahi.

Ab maan le company ko ye sab chahiye, to wo kahin aur lagega, apne alag storage box me. Aur ye cheez bahut zyada aa sakti hai, bheed lag sakti hai; bheed ka jawab hamare paas pehle se hai, Kafka.

Jitne bhi driver ki saari location har 4 second, Kafka me daal do, aur batch me alag storage box me daalte jao: 4 second baad driver yahan pahuncha, 4 second baad yahan, 10 minute se yahan khada tha, 20 minute yahan cigarette phooki, matlab jo bhi kiya (cigarette to map nahi dikhayega), cheezein alag-alag.

DB me location daali to DB pata nahi kitne guna bada karna padega, Cassandra jaisa koi bhi DB nahi jhel paayega, aur fayda bhi nahi.
Technical me: kadam 2 ke do raaste: pehli kahani me Cassandra me overwrite, is kahani me Redis me overwrite + raasta Kafka se alag storage. "parchi pe kaat ke nayi" = Redis me sirf aakhri jagah (overwrite), delivery ke baad hata do; "Kafka me daal, batch me alag storage box" = poore raaste ki history alag jagah (Kafka -> batch write); DB me sirf order ka status.

**Maan lo is doosri kahani me (location Kafka + batch se): Arpan ne book dabaya, abhi ke abhi 5 km ke khaali driver chahiye, par location pehle Kafka ki line me baithti aur Cassandra tak batch me der se pahunchti.**
Arpan: Ride book hi nahi hui to koi SOS nahi dabayega, kuch nahi; yahan mujhe jaldi chahiye. To Redis pe parchi: driver ki location har baar kaat ke latitude-longitude dhadadhad likhta rahunga.

5 km me 10, 50, 100 driver honge, 1 lakh nahi, aur Uber ke paas server aur paise ki kami nahi.

Ride book hui, Arpan cab me baith gaya, tab raasta thos chahiye: driver kahin aur le gaya, maine Uber ke SOS / support pe call kiya, to unke paas pakki location honi chahiye ki Whitefield se kahan le gaya.

Isliye DB / Cassandra me wo rakho jo important hai. Khaali ghoomne wala aur bhara (sawari wala) driver alag: bhare ka raasta zaroori, Kafka se batch me Cassandra; khaali ka bhi chahiye to alag Cassandra, padhai ke liye.

Aage Uber purane raaste padh ke ye bhi nikaal sakta: airport jaane ke 10 raste, sabse achha 9va, to zyada driver ko wahi dikhao.
Technical me: "jaldi ke liye parchi" = matching Redis se (abhi ki jagah, overwrite), "pakka raasta" = trip location history (Kafka -> batch -> Cassandra) for SOS / support / audit, "khaali vs bhara alag" = idle aur on-trip location alag store, "9va raasta" = purani trips se raaste ki padhai (routing / ETA).

**3. Book pe server 5 lakh driver ka fasla naap raha, 40 sec "dhoondh rahe hain".**
Arpan: Connaught Place pe khade ko ISBT ka driver kyun? Sirf 8-9 km ke andar wale chahiye. Aur 5 lakh me se chhaanto mat: naksha pehle se dabbon me kata ho, server sirf usi dabbe me dekhe jahan Arpan khada. Usme 3 driver, unme jo khaali wo Arpan ko. Baaki 5 lakh kahin bhi ghoomein, lena-dena nahi.
Technical me: "naksha dabbon me" = geohash (grid), "sirf apna dabba" = 5 lakh ki jagah gine-chune driver.

**Jaankari (sirf jaankari hai, design wahi; reel se):** Uber ke khaane square nahi, chhatte jaise 6-kone wale (hexagon), naam H3, Uber ne khud banaya. Square kyun nahi: square me bagal wale padosi paas, kone wale ~40% zyada door; hexagon me 6 ke 6 padosi barabar door, to har disha me ek jaisa dhoondhna. Apna khaana + 6 padosi = 7, driver na mile to ek ring aur (19).
Technical me: geohash = square khaane, H3 = hexagon khaane.

**Maan lo gole ke teeno driver booked:**
Arpan: Gole ke andar 3 driver hain, unme se jo bhi khaali ho wo Arpan ko mil jaayega; 5 lakh se lena-dena nahi. Teeno book hain to thodi der wait karna padega, jaise Uber karwata bhi hai, 15 minute wait.
Technical me: gole me khaali nahi = intezaar / dobara dhoondo (koi gaadi zabardasti nahi).

**Aur neeche (gola hi bahut bada, Whitefield):** Whitefield bahut bada ilaaka hai; ITPL me Samudra tower pe khada hoon, ek cab Agrahara ke paas, ek idhar, ek udhar.
Arpan: Area ko baant do, chhote-chhote hisse: ITPL alag, Agrahara alag, Nexus Forum mall alag. Whitefield ki saari cab mere phone pe thodi dikhengi, us waqt lakhon cab ho sakti, phone me dikhana namumkin. Jahan traffic / bheed zyada wahan dabba chhota-bada karna, wo baad me.
Technical me: "chhote hisse" = geohash cell, "bheed pe dabba chhota-bada" = cell size badalna (adaptive), wo aage kadam 6c.

**4. Arpan dabbe ke kinare pe, 200 meter door bagal wale dabbe me khaali driver, app bola "koi nahi".**
Arpan: Dabbe ki seema pe mat chalo. Arpan jahan khada wahan se 8 km ka gola khincho; jin dabbon ko gola chhoota, unke driver bhi dekho.
Technical me: "gola + chhoote dabbe" = apna geohash + aas-paas ke dabbe, phir asli fasla.

**Maan lo normal din bhi gole me koi cab khadi nahi:**
Arpan: Main jahan khada hoon, Uber ek gola bana dega; itne distance me koi cab nahi hai to gola bada kar do, phir bhi nahi to aur bada.
Technical me: search radius dheere-dheere badhao (expanding ring / neighbour cells), concert wale din jaisa hi par chhota.

**21-Nov round (Raju border paar karke Agrahara gaya):**
Arpan: Driver to aate-jaate rahenge, gaadi non-stop chal rahi. Main upar se dekh raha: Arpan jahan khada wahan se 5 km ka gola (7-8 km bhi chalega), us gole me jo bhi driver hain. 10 me se ek gaya, ek aaya, chalta rahega.

Jo sawari leke ja raha hai wo nahi, jo khaali hai unka phone baja do: "Arpan ki request aayi, ITPL se airport." Request ki umar, maan lo 30 second; jo accept kare usko ride.

Raju akela tha aur dabbe se nikal gaya to Arpan ko us waqt koi nahi dikhega, jaise naye aayenge dikhte jaayenge.
Technical me: "5 km gola" = aas-paas ke cell + asli fasla, "sirf khaali" = available drivers filter, "phone baja do" = broadcast dispatch, "30 second" = request TTL.

(Yahan se kadam 6 ke India-Pakistan wale Maan lo tak saare scene Lucknow ke: Arpan Lucknow me hai, wahan Gomti nadi aur Ekana stadium. Shehar badla, Uber ki baat wahi.)

**Maan lo (Gomti nadi) seedhi lakeer pe sabse paas Raju 500 meter, par nadi ke us paar, pul 4 km door; isi kinare Shyam 1.5 km. System ne Raju diya, Arpan 18 minute khada.**
Arpan: Sirf ye nahi dekhunga ki map pe kaun paas dikh raha. Gole ke driver ki asli doori nikaalunga: sadak se kitni door hai, kitni der lagegi. Nadi wala saamne dikh raha par sadak se 8 km, peeche wala dikh bhi nahi raha par 2 km; to Arpan ko peeche wala. Jaam achanak lag jaaye wo alag baat, wo Uber pehle se nahi bata sakta.
Technical me: "map pe paas" = seedhi lakeer (straight-line) doori, "sadak se kitni door" = road distance / ETA (routing se, sirf gole ke kuch paas wale driver pe).

**5. Gole me ek khaali driver Raju. Arpan aur Priya ne ek hi second me book dabaya, Raju ke phone pe do ride.**
Arpan: BMS wali parchi: "Raju khaali hai to Arpan ka" ek kadam me. Jiski pehle pahunchi uski parchi, doosre ko "Raju chala gaya", uske liye agla driver. Parchi ki umar: Raju 15 sec me accept na kare to parchi hatao, agla driver.
Technical me: "parchi" = driver pe hold / lock (atomic, sirf FREE ho to), "umar" = TTL.

**21-Nov round (Samudra tower: Arpan gate 3 pe, Priya gate 1 pe, saamne wali road se ek khaali driver):**
Arpan: Worst case dono request server pe ek hi millisecond me. Agar pehle dekha "driver khaali hai" aur phir alag se book kiya, to beech me Priya ke liye bhi wahi driver khaali dikh gaya, dono ko mil gaya.

Isliye dekhna aur book karna ek saath karo: lock lagao, select aur update ek hi kadam, khatam. Pehli request andar gayi to apna kaam poora karke aayegi, driver Arpan ka; Priya ke liye wo driver bacha hi nahi.

Alag-alag tod ke kiya to server Priya ki request le ke faltu andar jaata, din me hazaron baar ye extra kaam.
Technical me: "select aur update ek saath" = atomic conditional update / lock (WHERE driver FREE), "faltu kaam" = alag read-then-write me race + bekaar DB chakkar.

**Maan lo net atka, Arpan ne Book do baar daba diya.**
Arpan: ID jodna bas, sab me ID jodna. Booking ID jaayegi, same hi ID hai na, to wo idempotency ki ho jaayegi, server apne aap reject kar dega. Yahan booking ID hogi, wahan koi aur ID hogi; bas chhota connect rakh.
Technical me: idempotency key (wahi ID dobara aaye to chhodo).

**6. Diljit ka concert khatam, 50 hazaar log, cab sirf 500.**
Arpan: BMS jaisa: 500 cab hain to 500 log andar, baaki line me, agli baari. 50 hazaar ko server pe bitha ke kya karunga. Pehle 500 sambhalo, phir kiraya badha ke door ki cab bulao.
Technical me: "500 andar, baaki line" = virtual queue, "kiraya badhao, door ki cab aaye" = surge pricing.

**21-Nov round (Palace Grounds pe Arijit Singh ka concert, 50-60 hazaar log cab se aaye the, ab sab ek saath book kar rahe):**
Arpan: Sab ki request ek server pe aayi to server baith jaayega, to us garam jagah pe thodi der ke liye server badha do (autoscale, Kubernetes / AWS khud karte, uski andar ki baat design ka hissa nahi).

Cab 150-200 hain, log 50-60 hazaar, to dayra badhao: 5 km ki jagah 20-30, phir 50 km; Sankey Tank, idhar-udhar door se driver bulao, aur door se aane ka kiraya badhao (aane ka + jaane ka), warna driver tel phoonk ke aayega aur Uber ka bhi bhada nahi niklega.

Palace Grounds ko chhote khaano me kaato, upar ke 5 khaane ek server pe, neeche ke 5 doosre pe, taaki 50-60 hazaar ek hi server pe dhaava na bolein. Jo cab Kormangala chhod ke laut aayi wo turant phir available.

Bagal wale khaane me cab khaali ghoom rahi aur Arpan ke khaane me koi nahi, to Arpan ka dayra ek khaana aur badhao.

Aur sach: 50-60 hazaar ko turant cab namumkin, intezaar hoga hi, ek-do ghanta bhi; main ITPL se normal din me bhi 45 minute gola ghumte dekhta hoon.
Technical me: "server badhao" = autoscaling, "dayra badhao" = search radius badhana, "door ka kiraya" = surge pricing, "khaano me kaato, alag server" = cell / shard ke hisaab se baantna, thundering herd se bachao, "bagal ka khaana" = scatter-gather (kadam 6c), "laut ke turant available" = driver free after trip, "intezaar hoga" = NFR me saaf: bheed me turant cab ka vaada nahi.

**Maan lo Ekana stadium me match khatam, 2 minute me 15,000 booking ek hi dabbe se. Drivers ka data dabbe ke hisaab se alag-alag server pe baanta hai; stadium wale server ka CPU 100%, baaki shehar ke server khaali.**
Arpan: Pehle to itne driver us dabbe me honge hi nahi, gola bada karo, Gomti Nagar, Chinhat se bhi gaadi aaye. Aur ek server pe bojh: us ilaake ke liye 2-3 dabbe extra laga do taaki load ek machine pe na rahe. Har baar haath se nahi lagaunge: stadium, Connaught Place jaise hotspot pata hain, wahan pehle se 2-3 extra dabbe le ke chalo, aur 40-50 hazaar booking ki had paar ho to system apne aap dabbe badha de.
Technical me: "ek dabbe pe saara bojh" = hot cell (hot shard), "2-3 dabbe extra" = hot cell ko chhote dabbon me tod ke kai shard pe baanto, "pehle se le ke chalo" = jaane-pehchaane hotspot pe pre-scaling, "had paar to apne aap" = auto-scaling.

**Maan lo India-Pakistan match, 2 lakh log. Server badhane se kaam nahi chala, to stadium wala dabba tukdon me toda, har tukda apne server pe. Arpan Gate 3 pe, uske tukde me ek bhi khaali driver nahi; 300 meter door neeche wale tukde me 5 khaali driver, par wo doosre server pe. App bola "koi driver nahi".**
Arpan: Pehle server badhao, kaam na chale tab hi todo, aur 25-50 hazaar ke hisaab se, jitne zaroori. Toda to dikkat aayi: Arpan upar wale tukde me, khaali driver neeche wale me aaram se pastry kha raha. To ek papa ji le aao, bada aadmi jo sab tukde dekhta hai. Arpan ki request aaye to sirf uska tukda nahi, uske upar, neeche aur bagal wale tukde bhi dekho, unme khaali driver ho to Arpan ko. Ye har tukde ke liye. Phir bhi bheed zyada to gola bada.
Technical me: "pehle server, phir todo" = pehle scale out, zaroorat pe hi split, "papa ji jo sab tukde dekhe" = coordinator jo kai server se poochh ke jode (scatter-gather), "upar, neeche, bagal wale tukde" = neighbour cells (kadam 4 wala gola).

(Wapas Delhi, Connaught Place, wahi shaam 7 baje wali booking.)

**7. Raju ne 14.9 sec pe accept dabaya, server pe 16 sec pe pahuncha, tab tak Priya ko de diya.**
Arpan: Ghadi server ki. 15 sec tak ruka, phir chala gaya; der se aaya to uski parchi phaad do, accept radd. Uber me bhi aise booking cancel hoti dikhti.
Technical me: parchi = driver ko gayi request (TTL) · "ghadi server ki" = TTL ka faisla server pe, der se aaya accept reject.

**21-Nov round:**
Arpan: Request ki umar maan lo 30 second. Arpan ki request 29.3 pe Raju ne accept ki par server dheema tha, likh nahi paaya, 30 nikal gaya: ride nahi milegi, umar aage nahi badhayenge.

Agla driver le lega; ye sanyog lakhon me ek baar, Arpan ki cab agle driver se lag jaayegi. Doosra raasta: jab tak koi accept na kare, sabka phone bajta rahe, umar hi nahi.

Uber asal me kaise karta (1 minute, 1.5, 3, ya jab tak driver dayre me), pakka nahi pata; dono notes me.
Technical me: "30 second, phir nahi" = request TTL, server ki ghadi; "agla driver" = re-dispatch; "bajta rahe" = TTL ke bina broadcast jab tak accept.

Isi pe ek aur raasta (ek-ek driver nahi, sabke paas ek saath): Main server pe baitha hoon. Uber me ride ek driver ko nahi, aas-paas ke jitne khaali driver hain sabke paas jaati.

Raju ne 14.9 pe dabaya par server tak 16 pe aaya, itne me Suresh ka accept server pe aa gaya, to cab Suresh ko, kyunki server ne usse pehle dekha.

Jaise mere paas ek aakhri ice-cream bachi, do bachche daude; ek ne pehle shuru kiya par gir gaya, mujhe dukh hua, par doosra aake paise de deta hai.

Emotion me keh sakte ruk jaao, par technically ice-cream usko jo pehle mere paas aaya. Raju ko ye nahi bolenge "tumhari request nahi pahunchi", bas "tumse pehle kisi aur ne le li".
Technical me: "server ne pehle dekha" = server pe jo accept pehle pahuncha wo jeeta (first-write-wins / conditional update), "kisi aur ne le li" = Raju ko ride taken / expired message.

**Maan lo ek-ek driver ko 15 sec dene ki jagah kuch aur karna ho?**
Arpan: Jab tak Arpan ko cab nahi milti, aas-paas ke saare driver ke phone bajte raho: tum lo, tum lo. Ya time rakhna hai to 20-25 second rakh do. Par phir bhi accept server tak der se pahuncha to cab nahi milegi, kyunki server tak request pahunchna zaroori hai, tabhi server kuch kar payega.
Technical me: "sabke phone bajao" = broadcast dispatch (aas-paas ke kai driver ko ek saath, jo pehle accept kare uski ride), do ne ek saath accept kiya to BMS wali parchi (jo server pe pehle pahuncha wo jeeta), "20-25 sec" = TTL lamba, par faisla phir bhi server ki ghadi.

**8. Raju Connaught Place ke dabbe se Janpath ke dabbe me gaya, purane dabbe me naam pada; Arpan aur Priya dono ko dikh raha.**
Arpan: Sach wahi jo GPS ne abhi bheja: Raju Janpath me. Book pe Raju ki taaza jagah dekho; Arpan ke gole me nahi to use mat do, Arpan ke liye CP me koi aur. Dono ek saath dabaayein to wahi taala.
Technical me: taala = lock · "taaza jagah dekho" = assign se pehle driver ki latest location check (ek driver ki ek sach jagah), lock double se bachata.

**9. Dabbe kadam 3 me bana liye, par register abhi bhi driver ke naam se laga hai ("Raju -> Connaught Place"). Booking pe chahiye "Connaught Place me kaun-kaun", to phir 5 lakh line. Aur Raju dabba badle to purane panne me naam pada rehta, shaam tak 2,000 purane naam.**
Arpan: Jagah ke hisaab se baant do: Connaught Place me itne driver, Janpath me itne, AIIMS ke paas itne. 5 lakh ki jagah utni hi line jitne us jagah me. Raju us jagah se nikla to wahan se hata do, naye panne me daal do.

Raju booked hai to list se nikaal do, ab wo kisi ko nahi milna. Booked nahi aur ghoom raha to kaat ke nayi list me, U-turn le ke wapas aaya to phir purani list me.

Booked Raju ne Connaught Place pe sawari utaari to pata hai thodi der me khaali hoga, phir list me daal do.
Technical me: "jagah ke hisaab se panna" = geohash -> drivers (Redis set har dabbe ka), "nikla to hata do, naye me daal do" = har location update pe purane set se remove + naye me add (driver ki row me pichhla dabba yaad), "booked to list se bahar" = sirf khaali driver set me, sawari khatam pe wapas.

**10. Raju aa raha, Arpan ko gaadi naksha pe khisakti dikhni chahiye. 10 lakh phone har second "Raju kahan?" poochh rahe, 99% jawab "wahin", server dab gaya.**
Arpan: Server poochh kyun raha, poochne ka mauka hi mat do. Ek aisa dabba bitha do ki Raju hila to turant Arpan ko dikha de, hila to dikha de. Asal me hilta hai, jhooth nahi dikhate.
Technical me: dabba = WebSocket · "poochne ka mauka mat do, khud dikha do" = push (WebSocket, khula taar), polling band.

**11. Arpan ka taar server 12 pe, Raju ki jagah server 57 pe aati. 57 ko pata hi nahi Arpan kahan.**
Arpan: 100 server me dhoondhna nahi. Store karwa lo: Arpan server 12 pe, Raju ki ride Arpan ke saath. Raju aur Arpan ke taar jodo, Raju ko sirf Arpan se lena-dena. Jaise ghar ka raasta pata ho to seedha usi raaste.
Technical me: "store karwa lo" = session map (user -> server, Redis) + ride map (driver -> rider), "seedha usi raaste" = server 57 seedha server 12 ko (pub/sub channel), wahan se Arpan ke taar pe.

**12. Book pe ₹250 dikha, ride ke beech surge 2 guna, aakhir me ₹500 kate. Arpan: "maine 250 dekh ke haan kiya tha."**
Arpan: Jo dikhaya wahi lena meri zimmedari. 250 hi lo. Traffic me phase aur driver ki mehnat zyada lagi to rider ki marzi pe tip: 50-100 de de, na de to jitna dikhaya utna. (Uber / Rapido asal me badha deti hain, par main banata to aise.)
Technical me: "jo dikhaya wahi" = upfront fare: quote ride ke saath save (lock), aakhir me wahi kaato; "zyada mehnat" = tip alag, rider ki marzi.

**Aur neeche (surge nahi, traffic se raasta lamba): concert se Whitefield, book pe 900 dikha. Raste me bhaari traffic, 1 ghante ka raasta 2.5 ghante, aakhir me 1400 kate. Arpan bhadka.**
Arpan: Ye design ka hissa hai ya nahi, thoda controversy hai. Teen nazar se dekho.

Driver 1.5 ghanta traffic me khada raha, utne me 3-4 ride kar leta ya ek airport wali (Whitefield se 1200); use 900 me se Uber ka cut kat ke 100 mile to galat. Uber ka bhi nuksaan, cab fansi rahi.

Customer ka gussa jaayaz, use 1400 dene pade.

Par aage ka traffic pehle se nahi pata, Bangalore chhota bhida shahar hai, Chandigarh jaisa khula nahi; pehle hi 1400 bol diya aur traffic laga hi nahi to bezzati, bharosa gaya, isliye frontend pe pehle nahi dikha sakte.

Normal laal batti pe nahi badhao (30-50 second, har shahar me), thoda buffer rakho, warna log seedha auto le lenge.

Sach me der hui (1 ghante ka 1.5 ghante se upar) to har extra 30 minute pe +100 / 150: driver khush, Uber ka nuksaan kam, customer ko thoda nuksaan; ye to aam auto wala bhi karta.
Technical me: "traffic me fanse ka paisa" = time-based fare / waiting charge, "laal batti pe nahi" = buffer (free waiting), "pehle se nahi bol sakte" = upfront fare me aage ka traffic predict nahi hota; pehli kahani (jo dikhaya wahi + tip) aur ye, dono raaste.

**13. Trip khatam, ₹250 kaatne gaye, Arpan ke wallet me sirf ₹40. Payment fail, Arpan gaadi me hi baitha, Raju saamne, Raju ka petrol aur 40 minute.**
Arpan: Kaatna hai to poora kaato, aadha mat kaato: 40 kaat ke chhod diya to log jaan-boojh ke 1 rupaya rakhenge. Wallet me 250 nahi to app me saaf dikhao "payment nahi hua", aur Raju ke phone pe bhi dikhe. Arpan GPay se de, warna cash, kaise bhi. Jab tak paisa nahi aaya, ride khatam hi nahi hogi.
Technical me: "poora ya kuch nahi" = atomic charge (partial nahi), "ride tab tak khatam nahi" = ride status PAYMENT_PENDING jab tak SUCCESS, "GPay / cash" = fallback payment method, dono app pe dikhe.

**Isi pe ek aur misaal (Uber me kaun Pay dabata):**
Arpan: Uber me app apne aap paisa nahi kaatta, trip khatam pe rider khud Pay dabata. Jab tak pay nahi hua, driver utarne hi nahi dega.
Technical me: rider khud payment shuru kare, SUCCESS tak PAYMENT_PENDING.

**Maan lo 250 kaatne gaye, gateway chup, pata nahi kata ya nahi.**
Arpan: (Arpan, 8-Nov: "ID jodna bas, sab me ID jodna; same ID to idempotency, server reject kar dega; bas chhota connect rakh.") Dobara kaatne pe ride id hi key: gateway pehchaan lega ki ye wahi charge hai, do baar nahi katega. Dobara bhejne se pehle gateway se poochh lo kya hua (Payment wala status job).
Technical me: idempotency key (wahi ID dobara aaye to chhodo).

**Maan lo poochhte raho aur gateway ka jawab aaye hi na. Arpan gaadi me baitha, Raju saamne, dono intezaar kar rahe. Kab tak rokoge?** (BookMyShow 6c wala hi jawab)
Arpan: Gateway kitni der lega, hamare haath me nahi, to hum apni ghadi rakhte: 30 se 55 second, phir dono app pe "payment nahi hua, paisa kata to wapas aa jaayega", Arpan GPay / cash se de.

Paisa katte hi gateway chhota sa event bhejta, jaise S3 file aane pe bhejta. Event aaya = kata, hum "mil gaya" bol dete. Baad me kata nikla aur Arpan cash de chuka, to gateway ko bolo "lautao".

Onboard pe gateway se SLA lo: uptime aur down hone pe kitni der me khabar; bank ki der nahi.
Technical me: ghadi = hamara client timeout · "30-55 sec" = hamara client timeout, "paisa katte hi event" = webhook (PSP callback), "mil gaya" = ack (2xx), "baad me kata to lautao" = DB me REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe, "onboard pe SLA" = uptime SLA + incident response.

**21-Nov round: 1400 kaatne gaye, gateway chup, Raju agli sawari ke liye nikalna chahta hai.**
Arpan: Jab tak paisa nahi kata, ride poori nahi hoti; Uber me bhi pay ke baad hi ride poori hoti hai. Kuch second ruk sakte, hamesha nahi, aur bhi ride hain. 30 second jawab nahi to DB me FAILED likho.

Teen raaste: Uber ka apna wallet, doosra PSP (Razorpay, jo India me chalta ho), ya driver ko cash.

Arpan ne cash de diya aur 5 minute baad GPay se bhi 1400 kat gaye, to Razorpay event bhejega; table me ek extra column "kitni der baad kata" (5 minute), raat ki cron job usse pakde aur Razorpay ko bole lautao.

Kaise diya wo bhi rakho: cash (transaction id nahi, driver likhega) ya GPay (transaction id). Refund 5 din me na aaye to Arpan transaction id se bank se poochh sake.

Arpan ke wallet se kat gaya aur Razorpay ka event aa gaya to Arpan ko mat roko, ride poori; Uber ke account me der se aaye wo Uber aur bank ki dikkat. Gateway / bank ke andar kya hua wo PSP ka kaam, design ka hissa nahi.
Technical me: "30 sec" = client timeout -> FAILED, "wallet / doosra PSP / cash" = fallback + PSP failover, "kitni der baad kata" = late debit ka alag column (raat ki reconciliation job ke liye), "kaise diya" = payment method + transaction id, "event aaya to mat roko" = webhook SUCCESS pe ride complete.

**14. (Socha hua, hypothetical) Ride ke beech Raju ka phone switch off. GPS bhi band, naksha pe Raju gayab. Ride khatam kaun karega?**
Arpan: Phone band hona Uber ke haath me nahi, design ki dikkat nahi. Phone apni location khud bhejta hai, server poochhta nahi. Location aani band hui to server ek-do baar dekhega, phir apni taraf se bhi band. Jahan tak aaye utne paise Arpan ki app me dikhenge, wo de do. Phir Uber support ko phone: "driver ka phone band ho gaya". Support wahan se ride band kar dega, DB me update, Raju kahin nahi dikhega.
Technical me: "location aani band, ek-do baar dekha" = location timeout (phone push karta, server sirf intezaar karta), "utne paise" = jahan tak ka kiraya / jo dikhaya, "support ride band kare" = manual close, ride status DB me update, driver available list se bahar.

**15. Arpan aur Ravi ne saath me book kiya. Do gaadi: Car A aur Car B. Arpan ki booking pehle pahunchi, server ne Arpan ko paas wali Car A de di, Ravi ke liye Car B bachi jo 6 km door.**

```
   Sadak pe kaun kahan:

   [Car B]                 (Arpan)               [Car A]          (Ravi)
      |                       |                     |                |
      |<------- 3 km -------->|<------ 2 km ------->|<--- 1 km ----->|


   Car A se:   Arpan 2 km  ·  Ravi 1 km
   Car B se:   Arpan 3 km  ·  Ravi 6 km


   Purana niyam (jo pehle aaya, use paas wali):
       Arpan  <-  Car A   2 km
       Ravi   <-  Car B   6 km        kul 8 km

   Naya (dono ko saath milao):
       Ravi   <-  Car A   1 km
       Arpan  <-  Car B   3 km        kul 4 km
```

Arpan: Ek booking ko akele mat dekho.

Do booking aayi aur do gaadi hain, to dono user aur dono gaadi ki jagah (latitude / longitude) rakh ke har jodi ki doori nikaalo: Arpan se Car A kitni, Car B kitni, Ravi se Car A kitni, Car B kitni.

Phir jodi aisi do ki sabki milake doori kam aaye: Ravi ko beech wali Car A, Arpan ko bahar wali Car B. Doori km me naapo, minute me nahi, kyunki minute traffic se ghat-badh jaate hain.

Aur ek baar driver ne accept kar liya to gaadi pakki, use todna nahi; driver khud cancel kare tabhi dobara doori nikaalo.
Technical me: "dono ko saath milao, kul doori kam" = batch matching (kai booking aur kai gaadi ek saath, sabka milake pickup kam).

**Aur neeche (asli Uber accept ke baad nahi todta):**
Arpan: Asli Uber ka design public nahi. Customer ke naate itna dekha ki accept ke baad hamesha "driver ne cancel kiya" aata, "Uber ne cancel kiya" kabhi nahi; matlab accept ke baad Uber khud nahi todta. Par main Uber banata to accept se pehle dono ki doori dekh ke gaadiyan badal deta; ye do alag nazar hain, takraav nahi.
Technical me: accept ke baad assignment pakka; batch se badalna sirf accept se pehle.

**Maan lo Arpan ki booking 7:00:00 pe aayi aur Ravi ki 7:00:02 pe. Jab Arpan ki aayi tab Ravi ki thi hi nahi, to server ne turant Car A Arpan ko de di.**
Arpan: Booking aate hi gaadi mat do. Maan lo 15-20 second ruko, user itna ruk lega (Uber me bhi driver dhoondhne me itna lagta hi hai). Us beech jitni booking aayi, sab ko saath milao, phir gaadi do. Jaldbaazi me kisi ko galat gaadi na de do. Main upar se dekh raha: Arpan ki booking aayi, gaadi paas thi; 5-10 second ruka, tabhi Ravi aaya aur wahi gaadi Ravi se 1 km. To wo gaadi Ravi ko, Arpan ko agli gaadi.
Technical me: "15-20 second ruko, sab saath milao" = batching window.

**Maan lo shaam 7 baje office chhutne ka waqt. Har baar koi naya banda gaadi ke bilkul paas mil jaata, aur Arpan 7:05 tak "finding driver" pe hai.**
Arpan: Gaadi extra ho to dikkat hi nahi, sabko mil jaayegi. Gaadi ek hi ho aur Arpan ke saath 1-2 baar aisa ho chuka, to agli gaadi Arpan ki hi, chahe wo Ravi ya Priya ke zyada paas ho. Teesri baar Arpan ko gaadi milegi hi. Paas wale bande ko bol do "tu thodi der aur dekh le, tere liye aur gaadi aa rahi", par ye gaadi Arpan ke paas jaayegi. Arpan line me hi nahi hai to ye niyam lagta hi nahi.
Technical me: "baar-baar peeche reh gaya" = starvation, "jitna ruka utni baari upar" = aging (gaadi nahi badhti, sirf baari tay hoti).

**Maan lo Arpan ne 5 minute ruk ke gusse me booking cancel ki aur turant dobara book ki. Server ne nayi booking maan ke ginti 7:05 se shuru kar di.**
Arpan: Do raaste hain. (1) Cancel kiya to line se hata do, dobara aaye to peeche lage, kyunki beech me aur log aa gaye honge; use aage laana unke saath galat.

(2) Redis me parchi likh do "Arpan 5 minute ruka tha" aur us pe 30 second ki umar. 30 second ke andar wapas aaya to uske 5 minute chalte rahenge, warna parchi khatam aur wo line me peeche.

Uber crore customer sambhalta, sabka dekhna hai: 30 second ke baad bhi Arpan ko aage rakha to beech me aaye logon ka nuksaan. Cancel Arpan ne kiya, to dobara peeche lagna uski galti ka nateeja; baaki koi pareshaan nahi hota.
Technical me: "parchi" = Redis me pehle tap ka waqt (intezaar pehle tap se gino), "30 second ki umar" = TTL.

**16. (Zomato wali reel se aaya: Aman ki biryani, Raju ki bike) Arpan map pe Raju ki gaadi apni taraf aati dekh raha hai. Raju signal pe 15-20 minute khada, phone phir bhi har 4 second wahi jagah bhej raha, battery 15% pe.**
Arpan: Location mujhe kab chahiye? Driver bahut der se hil hi nahi raha to baar-baar location bhej ke karega kya, faltu hai. Jaise koi aadmi so raha hai to main uske kamre me baar-baar nahi jaunga; jagega tab jaunga. Dikh gaya ki Raju signal pe khada hai, to 4 second ki jagah 10, 15-20 second pe bhejo. Location band nahi hogi, bas der se aayegi; gaadi chali to phir har 4 second.
Technical me: "khada hai to der se, chala to jaldi" = adaptive ping (speed ke hisaab se location bhejne ka waqt, battery bachti).

**Maan lo Raju Connaught Place ki oonchi buildings ke beech hai. GPS buildings se takra ke galat jagah bata raha, Arpan ke map pe gaadi ek building ki chhat pe dikh rahi (ghost bike, helicopter ban gayi).**
Arpan: Main server pe baitha, dekha location ek badi building ke upar aa rahi. Asli jagah jaake dekha to neeche sadak hai, upar building; signal buildings me ulajh jaata. Hamein pata hai Raju is building ke aas-paas hai aur wahan sadak kaunsi hai. To 1 second ke liye wo chhat pe bhi dikha, use paas wali sadak pe wapas utaar do.
Technical me: "paas wali sadak pe chipka do" = map matching (GPS ka galat point sabse paas ki sadak pe, Raju ke raaste ke hisaab se).

**Aur neeche (Zomato wali reel se, GPS ek second me nadi paar): Raju sadak pe chal raha, achanak GPS ne galat point bheja aur Arpan ke map pe bike 1 second me 2 km door nadi ke us paar dikh gayi, agle second phir sadak pe.**
Arpan: Ho sakta hai kuch second ke liye Raju building ke upar, nadi ke upar ya nadi ke us paar dikhe: network theek nahi aaya aur app ne use kahin phenk diya.

Jaise Raju do building ke neeche se nikal raha, wahan tower me thodi dikkat aati, to gudda kahin bhi dikh sakta hai. To driver ko hum isi road pe rakhenge, animation aisa ki driver isi road pe aaye.

Ye hone se tu rok nahi sakta, Uber, Rapido me ye hota hai: 1-2 second ke liye Raju building ke upar dikha, par animation se use apne aap wapas road pe le aayenge.
Technical me: wahi map matching (galat point sabse paas ki sadak pe); GPS ka galat point aana rok nahi sakte, sirf dikhne se pehle sadak pe utaar do.

**Maan lo location har 4 second aati hai. Beech me gaadi 4 second khadi, phir achanak aage koodti, phir khadi: chal nahi rahi, teleport ho rahi.**
Arpan: Duniya me aisi technology nahi jo bilkul har pal ki asli jagah de; ho bhi to bahut paisa lagega. Main sadak pe hota aur bike chalaata to seedha-seedha aage jaata.

To apne gudde (map pe gaadi) ko halka-halka aage chalate raho: thoda aage, thoda aur aage. Nayi location aaye to use sahi jagah pe rakh do.

Par flyover ke neeche Raju ne U-turn le liya, hum to gudde ko aage chala rahe the; signal wapas aaya to asli jagah peeche nikli, to gudda wahan se uth ke wahan teleport. Ye har app me dikhta hai, Uber, Rapido, Zomato, Swiggy.
Technical me: "gudde ko andaze se aage chalao" = dead reckoning (speed + sadak dekh ke andaza), "nayi aayi to sahi jagah" = correction, "U-turn pe kooda" = andaza galat nikla, isliye jump.

**Aur neeche (Zomato wali reel se, tunnel me net gaya): Raju raaste me tunnel me ghusa, net 1 minute chala gaya. Is beech phone ne 15 jagah note ki, par bhej nahi paaya. Arpan ke map pe bike 1 minute se ek hi jagah khadi, Arpan soch raha Raju ruk gaya. (Basement alag hai: wahan Raju pahunch chuka, yahan abhi raaste me hai aur tunnel se nikal ke aage aayega.)**
Arpan: Tunnel me net strong nahi to Raju wahi dikhega, koi dikkat nahi, main thodi der refresh karunga. Jaise hi Raju tunnel se nikla, uski information aayi, Redis me bilkul aakhri wali information likh di.

Dekha bike tunnel se bahar nikal gayi, to bike wahin se seedha tunnel ke bahar: ek animation lagayenge ki bike thodi der tairti hui dikhe. Uber, Rapido, Swiggy jaise app me ye animation hota bhi hai.

Mere PG me lift hai: lift me net nahi aata, kuch nahi dikhta; neeche floor pe aaya, net aaya, to bike thodi si hawa me tairte hue zoom, PG ke paas aage khadi.

Matlab aakhri jo information mili, usne gudde ko animation se wahan se wahan dikha diya: last write wins.
Aur wo 15 jagah: net nahi chal raha to request aayegi kaise, phone net aayega tabhi bhejega. Bhej bhi di to unka kuch nahi karna.

Pattern / location chahiye to Kafka me daal do, worker / consumer ek tasveer bana dega ki ek line me ye kiya, Raju 15 minute yahan ruka tha.

Location update ke liye 15 se lena-dena nahi, mujhe wahi chahiye jab wo tunnel se bahar nikla, wahi gudda tunnel ke bahar dikhayega. Net hi band ho to wo infra ki cheez, usme hum nahi jaate.

Aur aakhri information kahin ruki hui hai to location 4 ki jagah 8 second me kar do, warna phone ki battery drain ho jaayegi.
Flyover aur tunnel dono almost same hain, gudde ko waise hi dikhana. Net poori tarah chala gaya, tunnel ke andar chala gaya, to driver wahi ruka dikhega; use chalta nahi dikha sakte, kyunki hamein pata hi nahi driver kya kar raha.

Ya tunnel me bhi gudda aage-aage badha sakte hain, par user ko galat gudda nahi dikha sakte: kya pata humne aage badhaya aur wo mod ke peeche nikal gaya.

Tab wahi lagana padega jo flyover me bola: nayi location aaye to yahan se seedha wahan dikha do. Dono ho sakte hain.
Technical me: "aakhri wali jeete" = last write wins (Redis me overwrite), "bike tairti hui" = purani se nayi jagah tak animation, "15 jagah Kafka -> worker / consumer ki tasveer" = sirf history ke liye, live map ke liye nahi, "khada to 8 second" = adaptive ping.

**Maan lo Raju Arpan ki society ki basement parking me ghusa aur phone 5 minute bilkul offline. Andaze se chalate rahe to gaadi shehar ke bahar; ruki dikhi to Arpan sochega Raju bhaag gaya.**
Arpan: Aakhri location pata hai: Raju building ke paas pahunch gaya. To gaadi wahin rok do aur map ki jagah dikha do "Raju tumhari building pahunch gaya". Arpan ko shaanti, wo map dekhna band kar dega. Zomato me Raju khana le ke upar bhi aata hai, isliye do cheez: pehle "order tumhari building tak pahunch gaya", phir jab haath me diya tab "order tumhe mil gaya". Uber me Arpan neeche aata hai, wahan "Raju pahunch gaya" kaafi.
Technical me: "aakhri jagah pe roko" = last known location (andaze se aage nahi), "map ki jagah status" = status update (arrived / delivered), jagah se pata chala pahuncha = geofence.

&nbsp;

Poora design = [15_uber.md](15_uber.md)
