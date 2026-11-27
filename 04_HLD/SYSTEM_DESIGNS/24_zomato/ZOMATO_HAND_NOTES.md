# Zomato (food delivery): hand notes (Whitefield ke PG se Burger Singh ka burger)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan Bangalore me Whitefield ke PG me baitha Zomato kholta hai. Use 15 km ke andar ke restaurant dikhne chahiye, rating ya doori se sort. Wo 10 km door "Burger Singh" se burger mangata hai. Peeche teen log hain: user (Arpan), restaurant (Burger Singh) aur driver. (Pehle se aaya: aas-paas ka area / geohash aur driver ki tracking = Uber; ek counter pe atomic ghatana = banking, inventory.)

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Arpan (Whitefield) --> Zomato app --> server --> Redis (menu + area ki restaurant list, TTL)
                                         |          ^  menu / khula-band badle to DB se update
                                         v          |
                                   AWS RDS Postgres (2 replica)  <-- ek counter: Burger Singh ke aaj ke burger
                                         |
                    order: counter pe ginti ghatao (stock ho to) --> order placed --> Burger Singh ka app --> driver (5 km)

 0   menu baar-baar DB se                         ->  menu Redis me, menu badle to job DB se cache update kare (cache + write pe update)
 1   15 km ke restaurant kaise                    ->  lat / long se gola, area ki ek list cache me, TTL, band = "closed" nishaan (area list cache)
 2   Neha kinare pe, Whitefield bahut bada        ->  bade area ke andar chhote area, har ek ki apni list (grid / geohash cell, Uber jaisa)
 3   order Burger Singh tak                       ->  accept (auto / restaurant) -> notification -> ready -> 5 km ke driver -> delivering (order status flow)
 4   burger ready, driver 15 min door             ->  quantity se ready time, driver ko wahi time bhejo (prep-time dispatch)
 5   (khud accept wala restaurant) chup rahe      ->  2-3 min window, phir order fail, paisa nahi kata / wapas (accept timeout)
 6   Diwali, 300 burger, 2 min me 800 log         ->  har order pe ginti atomic ghatao, stock tha to order placed (auto-accept), 0 pe out of stock; Kafka nahi (capacity gate)
 7   349 pe Arpan + Rahul ek pal me               ->  ek hi counter, check + ghatao ek saath, pehle wale ko (atomic decrement)
```

**Arpan ka nichod:** "Yahan do system design hain: ek user ke order ka (Arpan wala), ek restaurant aur driver ka (Zomato andar wala). User ko driver kaun hai ye sab nahi dikhta, restaurant ko dekhna padta. Zomato andar kya karta mujhe nahi pata, docs padh ke jawab nahi dunga; main hota to ye karta."

**FR / NFR kahan se (Arpan):** Food delivery ka main kaam hi requirement hai. Koi bhi aadmi food order kar raha, uske aas-paas ke restaurant dikhenge. "Driver tez aaye" FR / NFR me nahi daal sakte, wo design ka part hi nahi; 40 km door se aa raha to late aayega, jaldi ka scope nahi. Rating / doori se sort aur 15 km tak = interviewer se poocha (rating aur doori dono se; 15 km tak).

**FR / NFR (interview me pehle 2 minute):**
```
FR      1  aas-paas (15 km) ke restaurant dikhao, rating ya doori se sort        -> kadam 1, 2
        2  menu dekho, order karo, khana pahunche                                -> kadam 0, 3
        3  driver ko map pe track kar sake (Uber wali tracking)                  -> kadam 3
        4  restaurant kitni door hai (jaise 15 km), app pe pehle se dikhe        -> kadam 1
        (pehle sawaal: restaurant khud accept kare ya stock dekh ke auto-accept? hamara: stock ho to auto-accept, warna out of stock.)
        (pehle sawaal: order ka paisa PSP (Razorpay jaisa) se; uska andar ka kaam design ka hissa nahi.
         Interviewer se poochho: PSP ka timeout kitna, ya PSP scope se bahar? Wo bole "30-60 sec bahut" to wahi timeout,
         uske baad FAILED aur alag column "kitni der baad kata" (BMS 6c jaisa))

NFR     1  door se aaya khana thoda thanda ho sakta, garam ka jhootha vaada nahi -> kadam 4
        2  aakhri plate do logon ko na bike: ginti ek jagah, "bacha >= 1 to ghatao" ek saath    -> kadam 7
```
**Numbers (Arpan):** bahut bada scale nahi. Bangalore me ~5,000, average 8-10 hazaar DAU; India level pe 60-80 hazaar ya 1 lakh tak. Diwali / Navratri pe peak, maan lo 50 hazaar ek saath. Isliye NoSQL / Cassandra / MongoDB / DynamoDB ki zaroorat nahi: Cassandra "big bull" hai, use laana hi nahi, ladai karna hoga. Simple Postgres, AWS RDS le liya (wo replica, backup sab sambhal leta), 2 replica bahut.

**Dhaancha (tasveer):**

```
   +-----------------+     +------+     +--------------------+     +---------------+     +---------+
   | User (Arpan)    | --> |  LB  | --> | App server (2-3)   | --> | Local cache   | --> |  Redis  |
   | Whitefield,     |     +------+     | har shehar ka alag |     +---------------+     | menu,   |
   | address bhara   |                  +--------------------+                           | TTL     |
   +-----------------+                           |                                       +---------+
                                                 v                                            ^
                                   +----------------------------+                             |
                                   | DB: AWS RDS Postgres       | -- menu / stock badla: event (parchi phaad)
                                   | menu, ginti (stock),       |
                                   | order  + replica           |
                                   +----------------------------+

   Auto-accept: restaurant ka POS (UrbanPiper jaisa) Zomato se juda; andar ka hissa design se bahar
```

**Round me Arpan:** App kholo to aas-paas ke restaurant aur menu dikhe, rating ya doori se sort, keemat dikhe, order aur pay, out of stock pehle hi dikhe. NFR: door se khana thanda ho sakta hai, aakhri order do logon ko na bike. Address hamesha bhara hota (use my location), design pe asar nahi. 2-3 server, LB, replica; har shehar ka alag, 10-20 lakh order bhi AWS RDS sambhal leta. 15 km ka gola geohash / hexagon se. Feed ke liye fan-out (Twitter jaisa) bhi soch sakte, par restaurant itne nahi, isliye Redis. Menu Redis me TTL (3-4 min), chhotu dobara laaye; menu badle to DB se event, Redis update. Aakhri burger: atomic transaction, check aur ghatao ek saath. Auto-accept POS integration se hota hai (net pe dekha), wo Zomato aur vendor ke andar ka, design me nahi.

*Technical me:* "fan-out vs Redis" = push model vs pull + cache · "TTL + chhotu dobara le" = cache-aside with TTL · "DB se event, Redis update" = write pe cache invalidation.

&nbsp;

---

**0. Arpan ka pehla dhaancha: user -> Zomato app -> server -> AWS RDS Postgres. Har restaurant ka menu bhi DB se aa raha.**

**Arpan:** Zomato pe restaurant bahut hain, area wise depend karta; main Whitefield me rehta hoon to wahan thode kam, Electronic City jaise kahin aur zyada. Har restaurant ka ek menu hota hai, Burger Singh ka ek.

Isko kahin aisi jagah store kar doon, cache ya Redis me, jisse baar-baar meri DB query hit na ho.

Menu update ho sakta hai, par menu roz update nahi hota; Navratri, Diwali pe alag, par saal ke 365 din menu roz nahi badalta, to itne zyada writes nahi.

To DB se cache me job chala denge: DB me menu change hua to wo cache me update kar denge. Itna cache pe kyun?

5,000 user to DB aaram se sambhal leta, par Diwali sale pe 50,000 ho gaye to bekaar ki bheed, DB bojh utha raha sirf menu dikhane ke liye. Menu ko ek dabbe me rakh do, Redis me, DB se aage ek dabba.

*Technical me:* "menu ek dabbe me, DB se aage" = cache (Redis), "DB me menu badla to cache update" = write pe cache update (writes kam, reads zyada).

---

**1. Arpan Whitefield ke PG se app kholta hai. Server 15 km ke andar ke restaurant ki list kaise nikalega, rating / doori se sort? Har baar Postgres me har restaurant ki doori nikaalega?**

**Arpan:** App khula, location on karwao (off hai to popup), GPS se latitude longitude aa jaayega, GPS ka hisaab alag cheez, uske peeche nahi jaata.

Lat / long se uske aas-paas 15 km ka gola banao, gole ke andar jitne restaurant, maan lo 10, unme 8 open 2 closed; 8 dikha do, 2 pe closed ka mark laga do. Band restaurant list se hatenge nahi, closed dikhenge.

Khula / band onboarding pe hi pata hota: restaurant Zomato pe aate waqt batata hai ki is window me khulta, 5 PM pe band; Arpan ne 5:30 pe khola to closed dikhe.

Aur 8 PM pe band hona hai, kisi ne 7:30 pe khola, to banner dikha do "jaldi order karo, band ho raha"; ye UI ka hissa, alag baat.

**Arpan (har baar DB kyun):** Whitefield ke 15 km me jo restaurant hain wahi hain, ab Arpan ho chahe Rahul neeche wale kamre me, chahe Neha ya Ravi alag PG me, sabko wahi list.

Arpan ne app khola DB hit, Rahul ne khola DB hit, Neha, Ravi: 4 baar DB hit aur list ek hi nikli, koi fayda nahi. Use ek dabbe me daal do, DB se aage, sab wahin se lo.

Dabbe ka TTL, thodi der baad list expire ho jaaye kyunki restaurant ne kuch badla ho sakta. Aur DB me kuch badla (restaurant achanak band hua) to DB dabbe ko bole, wo dabbe me use closed mark kar de, hatana nahi.

Cache gira to replica; stale ho gaya to TTL; expire hone pe ek hi server lock leke DB se laaye aur dabbe me rakhe, baaki ruk ke wahi le lein.

Ek soch ye bhi: list taiyaar karke Twitter jaisa app me push kar do (fan-out), app khola to list turant.

*Technical me:* "lat / long se 15 km ka gola" = geo / radius search, "sabko wahi list, ek dabbe me" = area ke naam pe list cache (Redis) + TTL, "DB badla to dabbe me closed" = write pe cache update, "ek server lock leke laaye" = cache stampede pe lock, "app me push" = fan-out.

---

**2. Neha Whitefield ke kinare pe hai, KR Puram ki taraf. Uske 15 km me do-teen restaurant aate jo Arpan ke 15 km me nahi. "Whitefield" ki seema kaun tay karega, Neha ko kaunsi list?**

**Arpan:** Ye cheez Uber me ho chuki. Kinare wale ki bhi ek hi location hoti hai, uske PG ka address ya Whitefield me aayega ya KR Puram me; do location nahi ho sakti.

Jo bhi location hai uske lat / long se 15 km wali list ban jaayegi, uske app me aayegi, cache me rakhi jaayegi. Ho sakta hai Arpan ki list aur iski list thodi alag ho, bilkul maanta hoon.

Aur Whitefield bahut bada area hai: ek chhor ITPL, doosra, teesra. ITPL pe 4 user, doosre pe 3, teesre pe 8; har user ki alag list nahi. Teen area kaate to teen list: ITPL ki, doosre ki, teesre ki. Cache ek hi, par area wise alag.

Area ke andar area, jaise Uber me poora area kaata phir uske andar bhi.

*Technical me:* "area ke andar area, har ek ki list" = grid / geohash cell (Uber jaisa), har cell ki list cache me; "har user ki alag nahi" = cell ke naam pe key.

---

**3. Menu dekh ke Burger Singh se burger order kiya. Order Burger Singh tak kaise pahunchega? Kitchen full ho ya maal khatam ho to?**

**Arpan:** Zomato pe restaurant onboard hota hai to sab baat hoti: roz ki capacity kitni, kitna serve kar sakte; jaise Uber driver ke liye license chahiye.

Burger out of stock ho to restaurant ko app me out of stock update karna padega. Arpan ka order Burger Singh pe gaya, unhone dekha Whitefield se order aaya, Uber jaisa accept kiya.

Mere phone pe notification / webhook: tumhara order ho gaya. Cash on delivery ya pay, wo alag; pay to Google Pay, PhonePe, PSP hi hai, uspe faltu sawaal ka maza nahi.

Order ban raha, usi waqt restaurant aas-paas ke driver ki request bhejta: Whitefield ka order, le jao. Driver magic se nahi aata, bulane pe aata; mujhe lagta restaurant hi bulata jab khana ready ho ya hone wala ho.

Aas-paas 5 km ke driver: 10 km bahut door, 3 km kam, 5 km sweet spot. Driver ne accept kiya, aaya, order liya, status "delivering" DB me update. Phir map pe aata hai, wahi tunnel / ghost bike wala case, Uber me ho chuka. Neeche aaya, phone kiya, order de diya.

*Technical me:* "accepted -> ready -> delivering" = order status flow (state DB me), "mere phone pe" = notification / webhook, "restaurant 5 km ke driver bulaye" = driver dispatch (radius), tracking = Uber wali.

---

**4. Burger 7:30 pe ready, sabse paas wala driver 5 km door, aane me 15 min. Tab tak burger counter pe thanda ho raha. NFR me khana garam likha tha. Driver ko kab bulayega?**

**Arpan:** Ek burger banne me maan lo 15 min, chhota order. Bada order hai, 10, 20, 30 burger, to driver ko turant nahi bula sakte, 15 min me 10 burger nahi bante. Matlab driver ka aana order ki quantity pe depend karta.

Restaurant khud dekhe: ek burger hai, 10-15 min lagenge, to driver bula lo "aa jao, 15 min me ready rakh denge"; 2 burger hain to ek ban gaya phir bulao.

Jab restaurant Zomato ke app se driver bulata, mujhe lagta wo quantity aur time bhi bhejta hoga: "15 burger, Whitefield, 20 min lagenge"; "1 burger, 5 min". Driver usi hisaab se aayega.

30 burger mangaye to user ko bhi pata, time lagega, kuch thande bhi honge; ekdum perfect system nahi ki bilkul thanda na ho. Traffic design ka part nahi.

Zomato andar kya karta mujhe nahi pata, jaanna bhi nahi, docs padh ke jawab nahi dunga.

*Technical me:* "quantity se ready time, driver ko wahi time" = prep-time estimate se driver dispatch (driver tab pahunche jab khana ready).

---

**5. (Jab FR me restaurant khud accept dabaye; auto-accept wale design me ye scene nahi banta.) Raat 9 baje order gaya, par Burger Singh ki taraf se na accept aaya, na reject (app band pada, ya counter pe koi nahi). Arpan "order placed" ki screen dekh raha. System kya karega?**

**Arpan:** Yahan jawab hamare paksh ya vipaksh me nahi. Onboarding pe poochh lenge kitni der me jawab doge; unhone kaha 1-2 minute. Us din 2-3 min me jawab nahi diya, to ek time window hogi, kyunki user ko atka ke nahi rakh sakte.

Order 2-3 min (ya 1-1.5 min, bahut hai) unki screen pe rahega, user ko gola ghoomta dikhega "abhi final nahi hua". Window me accept nahi hua to user ko bol do order fail ho gaya, dobara try karo, kya pata is baar screen pe koi ho.

Order place tabhi hota jab restaurant accept kare, to paise kate hi nahi; kate hain to refund wapas. App hi band hai to wo order hum kar hi nahi paayenge.

Zomato ki terms andar kya hain mujhe nahi pata, wo net pe bhi nahi milti; main hota to ye karta.

*Technical me:* "2-3 min window, phir fail" = accept timeout + auto-cancel, "paisa tabhi jab accept" = accept ke baad hi capture (warna refund).

---

**6. Diwali ki raat 9 baje. Subah Burger Singh ne app me bataya aaj 300 burger bana sakta hai. 9 baje 2 minute me 800 log Burger Singh pe "order" dabate. Order Burger Singh tak kaise, "order placed" kab, aur baaki 500 ka kya?**

**Arpan:** Pehli baat quantity: onboarding pe aur roz restaurant batata aaj kitni capacity, kitni dish bana sakta. Burger Singh ne kaha 300, to 300 hi andar.

Ek aaya, quantity dekhi, hai, to atomic transaction me ek kam: 300 -> 299 -> 298 ... -> 1 -> 0. 0 hote hi khatam, agle ko order hi nahi hoga, out of stock.

Kafka ki zaroorat hi nahi padi: DB transaction milli second me hota, Postgres / AWS RDS ke liye chutkiyon ka kaam, 1 lakh pe bhi. Kafka wahan jahan 10-20 million log ek saath aa rahe; 1 lakh order ke liye Kafka lagaya to design bekaar me complex.

Order placed tabhi dikhao jab stock ho: stock tha to order placed, khatam. Burger Singh ke app me "Arpan ka order aaya, taiyaar karo, driver bulao". Accept ka button nahi: 100 ek saath aaye to wo 100 baar dabayega? Stock hai to order hoga hi, Burger Singh cancel nahi kar sakta; stock nahi to out of stock apne aap, use cancel ka mauka hi nahi. Maal khatam to restaurant app me out of stock laga de.

*Technical me:* "har order pe ginti dekho, ek kam" = atomic conditional update (`UPDATE ... SET qty = qty - 1 WHERE qty > 0`), "300 hi andar" = capacity gate, "stock tha to order placed, button nahi" = auto-accept (asal Zomato partner app me accept hota, kai restaurant POS se auto-accept lagaate), "Kafka nahi" = is scale pe queue ki zaroorat nahi (zaroori nahi, galat nahi).

---

**7. Burger Singh ki ginti 300, abhi 299 ho chuke. Arpan aur Rahul ek hi pal me order dabate, dono ko "1 bacha" dikhta, dono andar, 301. Kaise rokega? (Diwali pe kai server, dono server ne ginti ek hi pal me 299 padhi, likhne se pehle.)**

**Arpan:** Ginti ka counter ek hi hai, chahe Redis me chahe kisi me; dono server usi se baat kar rahe. Redis ek baar me ek hi command chalata, do ko ek saath nahi le sakta.

Postgres me rakho to bhi same, transaction DB hai: update maaro "bacha >= 1 hai? haan, ek ghatao", 0 ho gaya. Jiski request ek millisecond bhi pehle pahunchi usko mil gaya, doosre ko 0 dikha: "out of stock, order nahi ho paaya".

Dono bilkul ek hi pal me aaye (bahut rare) to bhi ek ko hi milega, jo lucky; trade-off ye hai ki kaun Arpan kaun Ravi, kismat.

(Waise 50,000 ke liye alag-alag server mujhe theek nahi lagta, AWS jaisa server itna jhel leta, par chalo kai server maan liye.)

*Technical me:* "ek counter, check + ghatao ek saath" = atomic decrement (Redis DECR / Lua, ya `UPDATE ... SET bacha = bacha - 1 WHERE bacha >= 1`), pehle wale ko = first come first served; banking ke do counter aur inventory ke aakhri iPhone jaisa.


---

**8. Raat 9:00:10 pe Burger Singh ka aakhri burger bik gaya. Redis ka menu abhi "burger available" (TTL 9:03 tak). 9:01 pe Ravi app kholta, burger dikhta, order dabata. "Out of stock pehle hi dikhe" wala FR kaise bachega?**

**Arpan:** Redis me ginti rakhta hi nahi, sirf menu; ginti DB me. Ginti bhi Redis me rakhi to bahut saare restaurant, Redis bahut bada. DB me out of stock hua to event jaaye, chhotu Redis ko bole parchi phaad de: menu hata do ya out of stock kar do. Peeche ek job Redis aur DB ka milaan karti rahe; TTL khatam pe naya menu aa hi jaayega.

*Technical me:* cache invalidation (event se, TTL ka intezaar nahi), source of truth DB, reconciliation job.

&nbsp;

Poora design = [24_zomato.md](24_zomato.md)
