# Zomato (food delivery): hand notes (Whitefield ke PG se Burger Singh ka burger)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan Bangalore me Whitefield ke PG me baitha Zomato kholta hai. Use 15 km ke andar ke restaurant dikhne chahiye, rating ya doori se sort. Wo 10 km door "Burger Singh" se burger mangata hai. Peeche teen log hain: user (Arpan), restaurant (Burger Singh) aur driver. (Pehle se aaya: aas-paas ka area / geohash aur driver ki tracking = Uber; ek counter pe atomic ghatana = banking, inventory; "order placed" dikha ke Kafka line = BMS, e-com.)

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Arpan (Whitefield) --> Zomato app --> server --> Redis (menu + area ki restaurant list, TTL)
                                         |          ^  menu / khula-band badle to DB se update
                                         v          |
                                   AWS RDS Postgres (2 replica)  <-- ek counter: Burger Singh ke aaj ke burger
                                         |
                    Diwali pe: Kafka line (restaurant-wise) --> worker --> Burger Singh ka app --> driver (5 km)

 0   menu baar-baar DB se                         ->  menu Redis me, menu badle to job DB se cache update kare (cache + write pe update)
 1   15 km ke restaurant kaise                    ->  lat / long se gola, area ki ek list cache me, TTL, band = "closed" nishaan (area list cache)
 2   Neha kinare pe, Whitefield bahut bada        ->  bade area ke andar chhote area, har ek ki apni list (grid / geohash cell, Uber jaisa)
 3   order Burger Singh tak                       ->  accept -> notification -> ready -> 5 km ke driver -> delivering (order status flow)
 4   burger ready, driver 15 min door             ->  quantity se ready time, driver ko wahi time bhejo (prep-time dispatch)
 5   Burger Singh chup, na accept na reject       ->  2-3 min window, phir order fail, paisa nahi kata / wapas (accept timeout)
 6   Diwali, Burger Singh pe 2 min me 500         ->  subah stock poocho, zyada ko gate pe mana, andar walon ki Kafka line (capacity gate + queue)
 7   349 pe Arpan + Rahul ek pal me               ->  ek hi counter, check + ghatao ek saath, pehle wale ko (atomic decrement)
 8   address khaali wala order, saari line atki   ->  3 baar retry, phir alag line, alert; line chalti rahe (retry limit + DLQ, reel se)
```

**Arpan ka nichod:** "Yahan do system design hain: ek user ke order ka (Arpan wala), ek restaurant aur driver ka (Zomato andar wala). User ko driver kaun hai ye sab nahi dikhta, restaurant ko dekhna padta. Zomato andar kya karta mujhe nahi pata, docs padh ke jawab nahi dunga; main hota to ye karta."

**FR / NFR kahan se (Arpan):** Food delivery ka main kaam hi requirement hai. Koi bhi aadmi food order kar raha, uske aas-paas ke restaurant dikhenge. "Driver tez aaye" FR / NFR me nahi daal sakte, wo design ka part hi nahi; 40 km door se aa raha to late aayega, jaldi ka scope nahi. Rating / doori se sort aur 15 km tak = interviewer se poocha (Claude: haan, rating aur doori dono se; 15 km tak).

**FR / NFR (interview me pehle 2 minute):**
```
FR      1  aas-paas (15 km) ke restaurant dikhao, rating ya doori se sort        -> kadam 1, 2
        2  menu dekho, order karo, khana pahunche                                -> kadam 0, 3

NFR     1  driver ko track kar sake                                              -> kadam 3 (tracking Uber wali)
        2  khana kaisa pahunche: door se aaye to user ko bata do, thoda thanda ho sakta -> kadam 4
```
**Numbers (Arpan):** bahut bada scale nahi. Bangalore me ~5,000, average 8-10 hazaar DAU; India level pe 60-80 hazaar ya 1 lakh tak. Diwali / Navratri pe peak, maan lo 50 hazaar ek saath. Isliye NoSQL / Cassandra / MongoDB / DynamoDB ki zaroorat nahi: Cassandra "big bull" hai, use laana hi nahi, ladai karna hoga. Simple Postgres, AWS RDS le liya (wo replica, backup sab sambhal leta), 2 replica bahut.

&nbsp;

**0. Arpan ka pehla dhaancha: user -> Zomato app -> server -> AWS RDS Postgres. Har restaurant ka menu bhi DB se aa raha.**
Arpan: Zomato pe restaurant bahut hain, area wise depend karta; main Whitefield me rehta hoon to wahan thode kam, Electronic City jaise kahin aur zyada. Har restaurant ka ek menu hota hai, Burger Singh ka ek. Isko kahin aisi jagah store kar doon, cache ya Redis me, jisse baar-baar meri DB query hit na ho. Menu update ho sakta hai, par menu roz update nahi hota; Navratri, Diwali pe alag, par saal ke 365 din menu roz nahi badalta, to itne zyada writes nahi. To DB se cache me job chala denge: DB me menu change hua to wo cache me update kar denge. Itna cache pe kyun? 5,000 user to DB aaram se sambhal leta, par Diwali sale pe 50,000 ho gaye to bekaar ki bheed, DB bojh utha raha sirf menu dikhane ke liye. Menu ko ek dabbe me rakh do, Redis me, DB se aage ek dabba.
Technical me: "menu ek dabbe me, DB se aage" = cache (Redis), "DB me menu badla to cache update" = write pe cache update (writes kam, reads zyada).

**1. Arpan Whitefield ke PG se app kholta hai. Server 15 km ke andar ke restaurant ki list kaise nikalega, rating / doori se sort? Har baar Postgres me har restaurant ki doori nikaalega?**
Arpan: App khula, location on karwao (off hai to popup), GPS se latitude longitude aa jaayega, GPS ka hisaab alag cheez, uske peeche nahi jaata. Lat / long se uske aas-paas 15 km ka gola banao, gole ke andar jitne restaurant, maan lo 10, unme 8 open 2 closed; 8 dikha do, 2 pe closed ka mark laga do. Band restaurant list se hatenge nahi, closed dikhenge. Khula / band onboarding pe hi pata hota: restaurant Zomato pe aate waqt batata hai ki is window me khulta, 5 PM pe band; Arpan ne 5:30 pe khola to closed dikhe. Aur 8 PM pe band hona hai, kisi ne 7:30 pe khola, to banner dikha do "jaldi order karo, band ho raha"; ye UI ka hissa, alag baat.
Arpan (har baar DB kyun): Whitefield ke 15 km me jo restaurant hain wahi hain, ab Arpan ho chahe Rahul neeche wale kamre me, chahe Neha ya Ravi alag PG me, sabko wahi list. Arpan ne app khola DB hit, Rahul ne khola DB hit, Neha, Ravi: 4 baar DB hit aur list ek hi nikli, koi fayda nahi. Use ek dabbe me daal do, DB se aage, sab wahin se lo. Dabbe ka TTL, thodi der baad list expire ho jaaye kyunki restaurant ne kuch badla ho sakta. Aur DB me kuch badla (restaurant achanak band hua) to DB dabbe ko bole, wo dabbe me use closed mark kar de, hatana nahi. Cache gira to replica; stale ho gaya to TTL; expire hone pe ek hi server lock leke DB se laaye aur dabbe me rakhe, baaki ruk ke wahi le lein. Ek soch ye bhi: list taiyaar karke Twitter jaisa app me push kar do (fan-out), app khola to list turant.
Technical me: "lat / long se 15 km ka gola" = geo / radius search, "sabko wahi list, ek dabbe me" = area ke naam pe list cache (Redis) + TTL, "DB badla to dabbe me closed" = write pe cache update, "ek server lock leke laaye" = cache stampede pe lock, "app me push" = fan-out.

**2. Neha Whitefield ke kinare pe hai, KR Puram ki taraf. Uske 15 km me do-teen restaurant aate jo Arpan ke 15 km me nahi. "Whitefield" ki seema kaun tay karega, Neha ko kaunsi list?**
Arpan: Ye cheez Uber me ho chuki. Kinare wale ki bhi ek hi location hoti hai, uske PG ka address ya Whitefield me aayega ya KR Puram me; do location nahi ho sakti. Jo bhi location hai uske lat / long se 15 km wali list ban jaayegi, uske app me aayegi, cache me rakhi jaayegi. Ho sakta hai Arpan ki list aur iski list thodi alag ho, bilkul maanta hoon. Aur Whitefield bahut bada area hai: ek chhor ITPL, doosra, teesra. ITPL pe 4 user, doosre pe 3, teesre pe 8; har user ki alag list nahi. Teen area kaate to teen list: ITPL ki, doosre ki, teesre ki. Cache ek hi, par area wise alag. Area ke andar area, jaise Uber me poora area kaata phir uske andar bhi.
Technical me: "area ke andar area, har ek ki list" = grid / geohash cell (Uber jaisa), har cell ki list cache me; "har user ki alag nahi" = cell ke naam pe key.

**3. Menu dekh ke Burger Singh se burger order kiya, paisa kata. Order Burger Singh tak kaise pahunchega? Unhone accept hi nahi kiya (kitchen full, maal khatam) to?**
Arpan: Zomato pe restaurant onboard hota hai to sab baat hoti: roz ki capacity kitni, kitna serve kar sakte; jaise Uber driver ke liye license chahiye. Burger out of stock ho to restaurant ko app me out of stock update karna padega. Arpan ka order Burger Singh pe gaya, unhone dekha Whitefield se order aaya, Uber jaisa accept kiya. Mere phone pe notification / webhook: tumhara order ho gaya. Cash on delivery ya pay, wo alag; pay to Google Pay, PhonePe, PSP hi hai, uspe faltu sawaal ka maza nahi. Order ban raha, usi waqt restaurant aas-paas ke driver ki request bhejta: Whitefield ka order, le jao. Driver magic se nahi aata, bulane pe aata; mujhe lagta restaurant hi bulata jab khana ready ho ya hone wala ho. Aas-paas 5 km ke driver: 10 km bahut door, 3 km kam, 5 km sweet spot. Driver ne accept kiya, aaya, order liya, status "delivering" DB me update. Phir map pe aata hai, wahi tunnel / ghost bike wala case, Uber me ho chuka. Neeche aaya, phone kiya, order de diya.
Technical me: "accepted -> ready -> delivering" = order status flow (state DB me), "mere phone pe" = notification / webhook, "restaurant 5 km ke driver bulaye" = driver dispatch (radius), tracking = Uber wali.

**4. Burger 7:30 pe ready, sabse paas wala driver 5 km door, aane me 15 min. Tab tak burger counter pe thanda ho raha. NFR me khana garam likha tha. Driver ko kab bulayega?**
Arpan: Ek burger banne me maan lo 15 min, chhota order. Bada order hai, 10, 20, 30 burger, to driver ko turant nahi bula sakte, 15 min me 10 burger nahi bante. Matlab driver ka aana order ki quantity pe depend karta. Restaurant khud dekhe: ek burger hai, 10-15 min lagenge, to driver bula lo "aa jao, 15 min me ready rakh denge"; 2 burger hain to ek ban gaya phir bulao. Jab restaurant Zomato ke app se driver bulata, mujhe lagta wo quantity aur time bhi bhejta hoga: "15 burger, Whitefield, 20 min lagenge"; "1 burger, 5 min". Driver usi hisaab se aayega. 30 burger mangaye to user ko bhi pata, time lagega, kuch thande bhi honge; ekdum perfect system nahi ki bilkul thanda na ho. Traffic design ka part nahi. Zomato andar kya karta mujhe nahi pata, jaanna bhi nahi, docs padh ke jawab nahi dunga.
Technical me: "quantity se ready time, driver ko wahi time" = prep-time estimate se driver dispatch (driver tab pahunche jab khana ready).

**5. Raat 9 baje order gaya, par Burger Singh ki taraf se na accept aaya, na reject (app band pada, ya counter pe koi nahi). Arpan "order placed" ki screen dekh raha. System kya karega?**
Arpan: Yahan jawab hamare paksh ya vipaksh me nahi. Onboarding pe poochh lenge kitni der me jawab doge; unhone kaha 1-2 minute. Us din 2-3 min me jawab nahi diya, to ek time window hogi, kyunki user ko atka ke nahi rakh sakte. Order 2-3 min (ya 1-1.5 min, bahut hai) unki screen pe rahega, user ko gola ghoomta dikhega "abhi final nahi hua". Window me accept nahi hua to user ko bol do order fail ho gaya, dobara try karo, kya pata is baar screen pe koi ho. Order place tabhi hota jab restaurant accept kare, to paise kate hi nahi; kate hain to refund wapas. App hi band hai to wo order hum kar hi nahi paayenge. Zomato ki terms andar kya hain mujhe nahi pata, wo net pe bhi nahi milti; main hota to ye karta.
Technical me: "2-3 min window, phir fail" = accept timeout + auto-cancel, "paisa tabhi jab accept" = accept ke baad hi capture (warna refund).

**6. Diwali ki raat 9 baje 50,000 log ek saath order kar rahe, akele Burger Singh pe 2 minute me 500 aaye, sab seedha server se Postgres me. Kya karega, Kafka lagayega ya nahi, kyun?**
Arpan: Itna traffic ek saath server, Postgres nahi jhel sakte, app hang, sabko gola ghoomta rahega. Pehli baat quantity: Burger Singh ke paas 500 burger hain ya 300? 300 hain to 500 ka matlab nahi. Zomato ke app me Amazon jaisa "itne bache" nahi hota; to Diwali ki subah Burger Singh aur aas-paas ke restaurant ko notification bhejo: aaj Diwali hai, kiske paas kitna stock, bata do taaki app me update karein, utne hi bhejenge. Burger Singh ne kaha 300-350 bana dunga, kisi ne 300 rasmalai. Ab 500 aaye to 350 ko hi line ke andar jaane do, baaki 150 ko pehle hi hata do, out of stock; baad me fail aur refund karne se bachna hai, ek-do order ke liye theek, zyada ke liye dikkat. 50,000 ko ek saath serve karne ka matlab nahi, line lagwa do: ek line Burger Singh ki, ek is restaurant ki, ek us ki. Andar walon ko "order placed" dikha do, gola ghumane ka matlab nahi, aur beech me Kafka laga do, Kafka worker peeche apna kaam karta rahega: ek-ek order Burger Singh tak, accept hua, ready hua, driver tak gaya.
Technical me: "subah stock poocho, utne hi andar" = capacity gate (baaki out of stock), "har restaurant ki line + order placed + worker peeche" = restaurant-wise queue (Kafka, key = restaurant) + async processing.

**7. Burger Singh ki ginti 350, abhi 349 ho chuke. Arpan aur Rahul ek hi pal me order dabate, dono ko "1 bacha" dikhta, dono andar, 351. Kaise rokega? (Claude ne dikhaya: Diwali pe kai server, dono server ne ginti ek hi pal me 349 padhi, likhne se pehle.)**
Arpan: Ginti ka counter ek hi hai, chahe Redis me chahe kisi me; dono server usi se baat kar rahe. Redis ek baar me ek hi command chalata, do ko ek saath nahi le sakta. Postgres me rakho to bhi same, transaction DB hai: update maaro "bacha >= 1 hai? haan, ek ghatao", 0 ho gaya. Jiski request ek millisecond bhi pehle pahunchi usko mil gaya, doosre ko 0 dikha: "out of stock, order nahi ho paaya". Dono bilkul ek hi pal me aaye (bahut rare) to bhi ek ko hi milega, jo lucky; trade-off ye hai ki kaun Arpan kaun Ravi, kismat. (Waise 50,000 ke liye alag-alag server mujhe theek nahi lagta, AWS jaisa server itna jhel leta, par chalo kai server maan liye.)
Technical me: "ek counter, check + ghatao ek saath" = atomic decrement (Redis DECR / Lua, ya `UPDATE ... SET bacha = bacha - 1 WHERE bacha >= 1`), pehle wale ko = first come first served; banking ke do counter aur inventory ke aakhri iPhone jaisa.

**8. (Reel se, Swiggy; scene-mock nahi, Arpan: "dead letter queue padh rakhi, Kafka ke offset / partition pata, seedha Zomato me daal do".) Dinner rush, kisi ko "order confirmed" message nahi aa raha. Server sab chal rahe, kuch crash nahi. Ek order ka address khaali tha: worker padhte hi fail, message line se hata nahi, timeout ke baad wahi wapas, phir fail. Line kram se chalti, pehla nahi hata to peeche wale atke.**
Reel ka jawab (Arpan ka padha hua, Kafka 8 / 8b jaisa): retry se network ki gadbad theek hoti, kharab data nahi; wo har baar fail, retry = kabhi na khatam hone wala chakkar. Fix 1: retry ki had (jaise 3 baar), had paar to message alag line me (dead letter queue), line khul jaati, baaki order ke message turant; kharab order engineer baad me aaram se dekhte. Fix 2: dead letter line pe alert, ek bhi aaye to team ko turant pata. Ek pech: kram wali line se message hataya to kram toot sakta (Kafka 8b: usi order / key ke baaki message bhi parking me).
Technical me: "baar-baar fail, line atki" = poison message, "had" = retry limit (SQS me maxReceiveCount; Kafka me N retry ke baad DLQ topic me daalo aur offset aage), "alag line" = dead letter queue (DLQ), "team ko pata" = DLQ pe alert.

&nbsp;

Poora design = [24_zomato.md](24_zomato.md)
