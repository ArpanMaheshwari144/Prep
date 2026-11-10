# Uber: hand notes (Connaught Place se ghar)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan shaam 7 baje Connaught Place pe khada hai, ghar ke liye Uber book karna hai. Sheher me 5 lakh driver ghoom rahe hain.

&nbsp;

**1. "Book" dabaya, par server ko pata hi nahi kaunsa driver abhi kahan hai.**
Arpan: Location to phone me khud hai, GPS ka latitude-longitude. Phone khud bhejega, har hilne pe nahi, thodi-thodi der me. Beech me ek dabba rakh lo jiska kaam sirf ek: har driver ki sabse taaza jagah batana.
Technical me: "beech ka dabba" = Location Service, phone har ~4 sec lat/long bhejta.

**2. Har 4 sec, 5 lakh driver = 1.25 lakh update / sec. Normal DB ghut gaya, jagah 30 sec peeche.**
Arpan: Ya to chhoti parchi, kaat ke nayi jagah likho; ya bada dabba jo likhne me tez ho. Crore-crore likhai pe parchi dheemi ho sakti, gir sakti, aur kab tak replica lagaunga, kal driver aur badhenge. To bada bhai bulao, jo isi kaam ke liye bana hai, apni copies khud rakhta: Cassandra.
Technical me: "bada dabba" = Cassandra (likhai tez, apni copies, machine jodo to badhta; andar ring = consistent hashing).

**3. Book pe server 5 lakh driver ka fasla naap raha, 40 sec "dhoondh rahe hain".**
Arpan: Connaught Place pe khade ko ISBT ka driver kyun? Sirf 8-9 km ke andar wale chahiye. Aur 5 lakh me se chhaanto mat: naksha pehle se dabbon me kata ho, server sirf usi dabbe me dekhe jahan Arpan khada. Usme 3 driver, unme jo khaali wo Arpan ko. Baaki 5 lakh kahin bhi ghoomein, lena-dena nahi.
Technical me: "naksha dabbon me" = geohash (grid), "sirf apna dabba" = 5 lakh ki jagah gine-chune driver.

**4. Arpan dabbe ke kinare pe, 200 meter door bagal wale dabbe me khaali driver, app bola "koi nahi".**
Arpan: Dabbe ki seema pe mat chalo. Arpan jahan khada wahan se 8 km ka gola khincho; jin dabbon ko gola chhoota, unke driver bhi dekho.
Technical me: "gola + chhoote dabbe" = apna geohash + aas-paas ke dabbe, phir asli fasla.

**5. Gole me ek khaali driver Raju. Arpan aur Priya ne ek hi second me book dabaya, Raju ke phone pe do ride.**
Arpan: BMS wali parchi: "Raju khaali hai to Arpan ka" ek kadam me. Jiski pehle pahunchi uski parchi, doosre ko "Raju chala gaya", uske liye agla driver. Parchi ki umar: Raju 15 sec me accept na kare to parchi hatao, agla driver.
Technical me: "parchi" = driver pe hold / lock (atomic, sirf FREE ho to), "umar" = TTL.
**Maan lo net atka, Arpan ne Book do baar daba diya.**
Arpan: Payment wali idempotency key: phone Book dabate hi booking id banaye, dono request me wahi. Server pehle id dekhe, pehle aa chuki to nayi ride nahi, wahi purani ride lauta do.
Technical me: idempotency key (wahi ID dobara aaye to chhodo).

**6. Diljit ka concert khatam, 50 hazaar log, cab sirf 500.**
Arpan: BMS jaisa: 500 cab hain to 500 log andar, baaki line me, agli baari. 50 hazaar ko server pe bitha ke kya karunga. Pehle 500 sambhalo, phir kiraya badha ke door ki cab bulao.
Technical me: "500 andar, baaki line" = virtual queue, "kiraya badhao, door ki cab aaye" = surge pricing.

**7. Raju ne 14.9 sec pe accept dabaya, server pe 16 sec pe pahuncha, tab tak Priya ko de diya.**
Arpan: Ghadi server ki. 15 sec tak ruka, phir chala gaya; der se aaya to uski parchi phaad do, accept radd. Uber me bhi aise booking cancel hoti dikhti.
Technical me: "ghadi server ki" = TTL ka faisla server pe, der se aaya accept reject.
**Maan lo ek-ek driver ko 15 sec dene ki jagah kuch aur karna ho?**
Arpan: Jab tak Arpan ko cab nahi milti, aas-paas ke saare driver ke phone bajte raho: tum lo, tum lo. Ya time rakhna hai to 20-25 second rakh do. Par phir bhi accept server tak der se pahuncha to cab nahi milegi, kyunki server tak request pahunchna zaroori hai, tabhi server kuch kar payega.
Technical me: "sabke phone bajao" = broadcast dispatch (aas-paas ke kai driver ko ek saath, jo pehle accept kare uski ride), do ne ek saath accept kiya to BMS wali parchi (jo server pe pehle pahuncha wo jeeta), "20-25 sec" = TTL lamba, par faisla phir bhi server ki ghadi.

**8. Raju Connaught Place ke dabbe se Janpath ke dabbe me gaya, purane dabbe me naam pada; Arpan aur Priya dono ko dikh raha.**
Arpan: Sach wahi jo GPS ne abhi bheja: Raju Janpath me. Book pe Raju ki taaza jagah dekho; Arpan ke gole me nahi to use mat do, Arpan ke liye CP me koi aur. Dono ek saath dabaayein to wahi taala.
Technical me: "taaza jagah dekho" = assign se pehle driver ki latest location check (ek driver ki ek sach jagah), lock double se bachata.

**9. Register driver ke naam se laga hai ("Raju -> Connaught Place"). Booking pe chahiye "Connaught Place me kaun-kaun", to phir 5 lakh line. Aur Raju dabba badle to purane panne me naam pada rehta, shaam tak 2,000 purane naam.**
Arpan: Jagah ke hisaab se baant do: Connaught Place me itne driver, Janpath me itne, AIIMS ke paas itne. 5 lakh ki jagah utni hi line jitne us jagah me. Raju us jagah se nikla to wahan se hata do, naye panne me daal do. Raju booked hai to list se nikaal do, ab wo kisi ko nahi milna. Booked nahi aur ghoom raha to kaat ke nayi list me, U-turn le ke wapas aaya to phir purani list me. Booked Raju ne Connaught Place pe sawari utaari to pata hai thodi der me khaali hoga, phir list me daal do.
Technical me: "jagah ke hisaab se panna" = geohash -> drivers (Redis set har dabbe ka), "nikla to hata do, naye me daal do" = har location update pe purane set se remove + naye me add (driver ki row me pichhla dabba yaad), "booked to list se bahar" = sirf khaali driver set me, sawari khatam pe wapas.

**10. Raju aa raha, Arpan ko gaadi naksha pe khisakti dikhni chahiye. 10 lakh phone har second "Raju kahan?" poochh rahe, 99% jawab "wahin", server dab gaya.**
Arpan: Server poochh kyun raha, poochne ka mauka hi mat do. Ek aisa dabba bitha do ki Raju hila to turant Arpan ko dikha de, hila to dikha de. Asal me hilta hai, jhooth nahi dikhate.
Technical me: "poochne ka mauka mat do, khud dikha do" = push (WebSocket, khula taar), polling band.

**11. Arpan ka taar server 12 pe, Raju ki jagah server 57 pe aati. 57 ko pata hi nahi Arpan kahan.**
Arpan: 100 server me dhoondhna nahi. Store karwa lo: Arpan server 12 pe, Raju ki ride Arpan ke saath. Raju aur Arpan ke taar jodo, Raju ko sirf Arpan se lena-dena. Jaise ghar ka raasta pata ho to seedha usi raaste.
Technical me: "store karwa lo" = session map (user -> server, Redis) + ride map (driver -> rider), "seedha usi raaste" = server 57 seedha server 12 ko (pub/sub channel), wahan se Arpan ke taar pe.

**12. Book pe ₹250 dikha, ride ke beech surge 2 guna, aakhir me ₹500 kate. Arpan: "maine 250 dekh ke haan kiya tha."**
Arpan: Jo dikhaya wahi lena meri zimmedari. 250 hi lo. Traffic me phase aur driver ki mehnat zyada lagi to rider ki marzi pe tip: 50-100 de de, na de to jitna dikhaya utna. (Uber / Rapido asal me badha deti hain, par main banata to aise.)
Technical me: "jo dikhaya wahi" = upfront fare: quote ride ke saath save (lock), aakhir me wahi kaato; "zyada mehnat" = tip alag, rider ki marzi.

**13. Trip khatam, ₹250 kaatne gaye, Arpan ke wallet me sirf ₹40. Payment fail, Arpan gaadi me hi baitha, Raju saamne, Raju ka petrol aur 40 minute.**
Arpan: Kaatna hai to poora kaato, aadha mat kaato: 40 kaat ke chhod diya to log jaan-boojh ke 1 rupaya rakhenge. Wallet me 250 nahi to app me saaf dikhao "payment nahi hua", aur Raju ke phone pe bhi dikhe. Arpan GPay se de, warna cash, kaise bhi. Jab tak paisa nahi aaya, ride khatam hi nahi hogi.
Technical me: "poora ya kuch nahi" = atomic charge (partial nahi), "ride tab tak khatam nahi" = ride status PAYMENT_PENDING jab tak SUCCESS, "GPay / cash" = fallback payment method, dono app pe dikhe.
**Maan lo 250 kaatne gaye, gateway chup, pata nahi kata ya nahi.**
Arpan: Dobara kaatne pe ride id hi key: gateway pehchaan lega ki ye wahi charge hai, do baar nahi katega. Dobara bhejne se pehle gateway se poochh lo kya hua (Payment wala status job).
Technical me: idempotency key (wahi ID dobara aaye to chhodo).
Aur gateway (Razorpay) se onboard pe hi vaada: 30-55 second me haan ya naa, aur paisa katte hi chhota sa event bhej do, jaise S3 bhejta (BookMyShow 6c wala hi jawab). Event aaya = kata, "mil gaya"; tab poochhne ki zaroorat hi kam.
Technical me: SLA / timeout + webhook (PSP callback) + ack.

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

Arpan: Ek booking ko akele mat dekho. Do booking aayi aur do gaadi hain, to dono user aur dono gaadi ki jagah (latitude / longitude) rakh ke har jodi ki doori nikaalo: Arpan se Car A kitni, Car B kitni, Ravi se Car A kitni, Car B kitni. Phir jodi aisi do ki sabki milake doori kam aaye: Ravi ko beech wali Car A, Arpan ko bahar wali Car B. Doori km me naapo, minute me nahi, kyunki minute traffic se ghat-badh jaate hain. Aur ek baar driver ne accept kar liya to gaadi pakki, use todna nahi; driver khud cancel kare tabhi dobara doori nikaalo.
Technical me: "dono ko saath milao, kul doori kam" = batch matching (kai booking aur kai gaadi ek saath, sabka milake pickup kam).
**Maan lo Arpan ki booking 7:00:00 pe aayi aur Ravi ki 7:00:02 pe. Jab Arpan ki aayi tab Ravi ki thi hi nahi, to server ne turant Car A Arpan ko de di.**
Arpan: Booking aate hi gaadi mat do. 15-20 second ruko, user itna ruk lega (Uber me bhi driver dhoondhne me itna lagta hi hai). Us beech jitni booking aayi, sab ko saath milao, phir gaadi do. Jaldbaazi me kisi ko galat gaadi na de do.
Technical me: "15-20 second ruko, sab saath milao" = batching window.
**Maan lo shaam 7 baje office chhutne ka waqt. Har baar koi naya banda gaadi ke bilkul paas mil jaata, aur Arpan 7:05 tak "finding driver" pe hai.**
Arpan: Gaadi extra ho to dikkat hi nahi, sabko mil jaayegi. Gaadi ek hi ho aur Arpan ke saath 1-2 baar aisa ho chuka, to agli gaadi Arpan ki hi, chahe wo Ravi ya Priya ke zyada paas ho. Teesri baar Arpan ko gaadi milegi hi. Paas wale bande ko bol do "tu thodi der aur dekh le, tere liye aur gaadi aa rahi", par ye gaadi Arpan ke paas jaayegi. Arpan line me hi nahi hai to ye niyam lagta hi nahi.
Technical me: "baar-baar peeche reh gaya" = starvation, "jitna ruka utni baari upar" = aging (gaadi nahi badhti, sirf baari tay hoti).
**Maan lo Arpan ne 5 minute ruk ke gusse me booking cancel ki aur turant dobara book ki. Server ne nayi booking maan ke ginti 7:05 se shuru kar di.**
Arpan: Do raaste hain. (1) Cancel kiya to line se hata do, dobara aaye to peeche lage, kyunki beech me aur log aa gaye honge; use aage laana unke saath galat. (2) Redis me parchi likh do "Arpan 5 minute ruka tha" aur us pe 30 second ki umar. 30 second ke andar wapas aaya to uske 5 minute chalte rahenge, warna parchi khatam aur wo line me peeche. Uber crore customer sambhalta, sabka dekhna hai: 30 second ke baad bhi Arpan ko aage rakha to beech me aaye logon ka nuksaan. Cancel Arpan ne kiya, to dobara peeche lagna uski galti ka nateeja; baaki koi pareshaan nahi hota.
Technical me: "parchi" = Redis me pehle tap ka waqt (intezaar pehle tap se gino), "30 second ki umar" = TTL.

**16. (Zomato wali reel se aaya: Aman ki biryani, Raju ki bike) Arpan map pe Raju ki gaadi apni taraf aati dekh raha hai. Raju signal pe 15-20 minute khada, phone phir bhi har 4 second wahi jagah bhej raha, battery 15% pe.**
Arpan: Location mujhe kab chahiye? Driver bahut der se hil hi nahi raha to baar-baar location bhej ke karega kya, faltu hai. Jaise koi aadmi so raha hai to main uske kamre me baar-baar nahi jaunga; jagega tab jaunga. Dikh gaya ki Raju signal pe khada hai, to 4 second ki jagah 10, 15-20 second pe bhejo. Location band nahi hogi, bas der se aayegi; gaadi chali to phir har 4 second.
Technical me: "khada hai to der se, chala to jaldi" = adaptive ping (speed ke hisaab se location bhejne ka waqt, battery bachti).
**Maan lo Raju Connaught Place ki oonchi buildings ke beech hai. GPS buildings se takra ke galat jagah bata raha, Arpan ke map pe gaadi ek building ki chhat pe dikh rahi (ghost bike, helicopter ban gayi).**
Arpan: Main server pe baitha, dekha location ek badi building ke upar aa rahi. Asli jagah jaake dekha to neeche sadak hai, upar building; signal buildings me ulajh jaata. Hamein pata hai Raju is building ke aas-paas hai aur wahan sadak kaunsi hai. To 1 second ke liye wo chhat pe bhi dikha, use paas wali sadak pe wapas utaar do.
Technical me: "paas wali sadak pe chipka do" = map matching (GPS ka galat point sabse paas ki sadak pe, Raju ke raaste ke hisaab se).
**Maan lo location har 4 second aati hai. Beech me gaadi 4 second khadi, phir achanak aage koodti, phir khadi: chal nahi rahi, teleport ho rahi.**
Arpan: Duniya me aisi technology nahi jo bilkul har pal ki asli jagah de; ho bhi to bahut paisa lagega. Main sadak pe hota aur bike chalaata to seedha-seedha aage jaata. To apne gudde (map pe gaadi) ko halka-halka aage chalate raho: thoda aage, thoda aur aage. Nayi location aaye to use sahi jagah pe rakh do. Par flyover ke neeche Raju ne U-turn le liya, hum to gudde ko aage chala rahe the; signal wapas aaya to asli jagah peeche nikli, to gudda wahan se uth ke wahan teleport. Ye har app me dikhta hai, Uber, Rapido, Zomato, Swiggy.
Technical me: "gudde ko andaze se aage chalao" = dead reckoning (speed + sadak dekh ke andaza), "nayi aayi to sahi jagah" = correction, "U-turn pe kooda" = andaza galat nikla, isliye jump.
**Maan lo Raju Arpan ki society ki basement parking me ghusa aur phone 5 minute bilkul offline. Andaze se chalate rahe to gaadi shehar ke bahar; ruki dikhi to Arpan sochega Raju bhaag gaya.**
Arpan: Aakhri location pata hai: Raju building ke paas pahunch gaya. To gaadi wahin rok do aur map ki jagah dikha do "Raju tumhari building pahunch gaya". Arpan ko shaanti, wo map dekhna band kar dega. Zomato me Raju khana le ke upar bhi aata hai, isliye do cheez: pehle "order tumhari building tak pahunch gaya", phir jab haath me diya tab "order tumhe mil gaya". Uber me Arpan neeche aata hai, wahan "Raju pahunch gaya" kaafi.
Technical me: "aakhri jagah pe roko" = last known location (andaze se aage nahi), "map ki jagah status" = status update (arrived / delivered), jagah se pata chala pahuncha = geofence.

&nbsp;

Poora design = [15_uber.md](15_uber.md)
