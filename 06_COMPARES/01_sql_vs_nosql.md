# SQL vs NoSQL — data kaisa hai, kaam kya hai, us hisaab se

---

## Koi poochhe to aise bolo

**SQL** (MySQL, Postgres): data table me, row aur column. Tables aapas me judi (user, order, payment), JOIN se milti, aur
transaction (ACID) milta: paisa kata to dono jagah sahi. **NoSQL** ek kism nahi, kai kism: **Redis** = key -> value, bahut
tez (parchi, cache, session). **MongoDB** = poora document ek jagah (product catalog, profile). **Cassandra** = likhai bahut
tez, machine jodte jao aur badhta (feed, location history, chat). **Neo4j** = rishton ka jaal. Chunav: data aapas me juda
aur galti nahi chalegi -> SQL; likhai bahut zyada, ya sirf ek key se padhna, ya dhaancha badalta rehta -> NoSQL. Asli
system me dono saath chalte.

---

## Tere hi faisle (24 design se)

```
BMS, banking, payment, file upload metadata  -> SQL    (paisa / seat, transaction chahiye)
Zomato                                        -> Postgres ("Cassandra big bull, laana hi nahi", scale chhota)
News                                          -> Postgres pehle; likhai bahut badhi to Cassandra (FR se faisla)
Uber raasta, chat message                     -> Cassandra (likhai bahut, chat_id / driver se baanto)
Uber abhi ki jagah, BMS parchi, feed          -> Redis (abhi ka, kaat ke nayi likho)
Search                                        -> Elasticsearch (News 13)
```

---

## Dikkat se hal

Sab kuch ek DB me thoonsna: location har 4 sec SQL me likhi to DB ghut gaya (Uber kadam 2). Hal: kaam ke hisaab se dabba;
ek system me SQL + Redis + Cassandra saath.

---

## Trap

"NoSQL me transaction hota hi nahi" galat: MongoDB me bhi hota. Par jahan row wala data aur transaction chahiye, wahan SQL
ye sab muft me deta (tera banking wala jawab). "SQL scale nahi hota" bhi adhoora: read replica, partition, zaroorat pade to
shard, sab hota; bas mehnat zyada.

[← HLD README](../README.md)
