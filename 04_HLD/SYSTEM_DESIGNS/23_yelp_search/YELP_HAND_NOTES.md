# Yelp search: hand notes (5 km ke andar burger)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Ravi raat 10 baje app kholta hai aur "burger" likhta hai. Use aas-paas ke burger wale restaurant chahiye, galat spelling pe bhi. Restaurant ki asli jaankari (naam, menu, khula / band) Postgres me hai. (Reel se aaya: Yelp search design; Elasticsearch pehle Foundations 12 me padha, aas-paas wala geohash Uber se.)

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Ravi  -->  server  -->  Elasticsearch (search ki copy: aas-paas + text + sort)  -->  pehle 10
                              ^
                              |  badlaav aate hi change event  (+ raat ki job, jo chhoota wo bhi)
                              |
                          Postgres (asli sach: naam, menu, khula / band)

 1   "burger" pe 8 sec, "burgr" pe zero          ->  aas-paas ke restaurant (geohash, Uber jaisa) + Elasticsearch, kuch bhi likho sahi option (geo filter, fuzzy)
 2   Burger Singh band, search me abhi bhi       ->  Postgres aur ES sync, band dukaan "Closed" dikhe, Zomato jaisa (DB -> search sync)
 3   dopahar ka naya burger, ES ko raat 2 baje   ->  badlaav aate hi event, raat ki job bhi chale (change event, reconciliation)
 4   300 m wali 4.6 star 15ve number pe          ->  sort user chune, server saare pe sort kare, phone ko pehle 10 (server-side sort + pagination)
```

**Arpan ka nichod:** "Yelp = Uber aur search ka mixture. Aas-paas ka salon / restaurant dhoondho, rating padho, wahan chale jao ya contact karo. Uber me driver chalta, yahan dukaan khadi rehti; isliye bas aas-paas + search."

**FR / NFR kahan se:** upar ki dikkat chain dekh ke nikale; har line ke aage likha hai wo kis kadam se aayi.
Arpan: Koi bhi kuch dhoondhe to use aas-paas ka search milna chahiye, jaise Zomato karta; kisi ne kuch dhoondha aur mil gaya aas-paas ki range me, geo filter wahi. Iska FR simple hi hai, aas-paas ka dhoondha mila, bas. Yelp chhota hai, FR / NFR bhi chhote hi banenge. Kadam 1 aur 4 FR me, 2 aur 3 NFR me.
Technical me: FR = geo search (fuzzy bhi) + sort; NFR = DB aur search ka sync (Closed badge, change event + raat ki job).

**FR / NFR (interview me pehle 2 minute):**
```
FR      1  kuch bhi dhoondho (galat spelling pe bhi), aas-paas ki range me mile   -> kadam 1
        2  sort user chune: doori se ya rating se                                 -> kadam 4

NFR     1  band dukaan search me "Closed" dikhe (search thodi der peeche chalega) -> kadam 2
        2  menu / dukaan badle to search me jaldi aaye; event chhoote to raat ki job pakde -> kadam 3
```
**Numbers (Arpan):** Zomato jaisa app India ke shehron me chalta, worldwide nahi. Ek area me 5 lakh log ek saath "pizza" faltu number hai; 10-20 hazaar maano, itna server sambhal leta, iske liye alag hal nahi chahiye.

&nbsp;

**1. Raat 10 baje Ravi "burger" likhta hai. Abhi sirf Postgres hai, 50 lakh restaurant, har ek ke naam aur menu me "burger" dhoondhna (LIKE '%burger%'), 8 second. Upar se "burgr" likha to ek bhi result nahi.**
Arpan: Ravi ne burger likha, to uske aas-paas ka, geohash, uska latitude longitude hoga, aur uske aas-paas ke jitne restaurant me burger hai wo dikha denge. Ye wahi cheez hai jo Uber me karte, same. Ab usne "burger" nahi "burgr" likha, to Elasticsearch hai, ek cheez jo kuch bhi likho sahi option de degi: burger. Almost easy hai, pehle se padha hua.
Technical me: "aas-paas ke restaurant, latitude longitude" = geo filter (geohash / radius), "kuch bhi likho sahi option" = Elasticsearch (inverted index) + fuzzy search (typo tolerance). (bana-banaya: AWS OpenSearch / Elastic Cloud; hum sirf sikhate kya aur kaise dhoondhna, Foundations 12)

**2. Search ab Elasticsearch se, asli data Postgres me. "Burger Singh" ne aaj ke liye dukaan band ki (chhutti), Postgres me "closed". Par Ravi ke search me Burger Singh abhi bhi aa raha, ES ke paas purani jaankari.**
Arpan: Postgres aur Elasticsearch ko sync kar do: Burger Singh close ho gaya to close dikha do. Ye thodi hoga ki Burger Singh search me aana hi band; Zomato me restaurant aate hain, bas Closed dikhta hai, dikkat kya hai. To Postgres aur Elasticsearch ke beech sync job chala do: ye ab close ho chuka, to close.
Technical me: "Postgres aur ES sync" = DB -> search index sync (Postgres sach, ES search ki copy), "band dukaan Closed dikhe" = status field (filter / badge), thodi der ES peeche = eventual consistency.

**3. Sync job har raat 2 baje. Paas wale "Burger Point" ne dopahar 1 baje menu me "Paneer Burger" joda, Postgres me turant. Shaam 8 baje Ravi "paneer burger" search kare, nahi aata; ES ko raat 2 baje pata chalega. Restaurant wala shikayat kar raha.**
Arpan: Koi badlaav hua to ek single job chala do. Raat ko jo job chalegi wo to chalegi, par badlaav ho rahe to jaise hi Postgres me aaye, ek event bhej do Elasticsearch ko: ye bhi aa gaya, ye bhi kar de. Jaise S3 me hota hai, same jawab. (Yahan queue / Kafka nahi, wo kisi ne bola hi nahi; Zomato itna bada nahi ki Kafka uthaye; bas change event.)
Arpan (Kafka kab): Kafka numbers dekh ke decide karunga. Zomato jaise food delivery me ek saath zyada se zyada 20 hazaar order maano, wo seedha sambhal leta, Kafka ka matlab nahi. Haan, ekdum peak pe 1 lakh log ek saath aa gaye to gola ghoomta dikhao, "order place ho gaya", Kafka peeche sambhal lega. Amazon jaise ke liye Kafka soch sakta; Yelp kahan chalta (US / Europe) pata nahi, to wo alag baat.
Technical me: "badlaav pe event" = change event (Postgres ke badlaav ko seedha padhna = CDC), "raat wali job bhi" = reconciliation job (jo event chhoota wo pakde).

**4. Ravi ne "burger" likha, 5 km ke andar 200 restaurant, screen pe 10. Upar wahi jinke naam me "burger" zyada; 4.9 km door 2 star sabse upar, 300 meter wali 4.6 star 15ve number pe. Kaun upar?**
Arpan: Rating wale upar aayenge, par ye depend karta hai. Frontend pe sort ka option laga sakte: kilometer se sort ya rating se, user jo chune wo upar. Server ke paas saare restaurant rahenge, phone ke paas wahi jaayenge jo sort hue. Burger likha to saare aate hain: Burger King, naam me burger, menu me burger, neeche hi sahi par aate. Ravi ne "rating se sort" kiya to rating wale upar, chahe 4 km ho, 10 km ya 15 km.
Technical me: "sort ka option user chune" = sort param (rating / doori), "server ke paas saare, phone ko sorted pehle 10" = server-side sort (Elasticsearch me) + pagination.

&nbsp;

Poora design = [23_yelp_search.md](23_yelp_search.md)
