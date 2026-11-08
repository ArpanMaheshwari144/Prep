# HLD dikkat chain

Dikkat ke parivaar: upar dikkat, phir humne kya socha, phir wahi dikkat kis-kis design me aayi. Ek parivaar samjha to saare
design me wahi kaam aata. Design ka naam + kadam number har line me, bhool jaao to neeche notes ke link.

## 1. Ek cheez, do log, ek hi second  (TAALA)

```
Dikkat:      cheez ek hai (seat, driver, aakhri ghee, 600 ka balance, ek token), dono ne ek saath dekha "khaali hai", dono le gaye.
Humne socha: dekhna aur rokna ek hi kadam me. Seat pe parchi laga do, jisne pehle lagayi uski, doosre ko turant "chali gayi".

Yahi dikkat yahan bhi:
  URL      1   do logon ko same token                         ->  ek line wala counter, aage badhao
  URL      7   5 counter, dono ne "10" diya                   ->  apni-apni range
  Rate     4   20 server ne ek saath "tokens = 1" padha       ->  sab ek hi baari me, koi ghus na paaye (Lua script)
  Stock    4   4 thread, 10 share 20 me bike                  ->  copy apne paas kheencho, ek jagah (1 thread per symbol)
  BMS      1   dono ko D5 "mil gayi"                          ->  seat pe parchi, dekhna + rokna ek saath (hold)
  Banking  4   600 me do counter pe 500-500                   ->  dekhna aur kaatna ek kram me (row lock)
  Uber     5   Raju pe ek second me do ride                   ->  BMS wali parchi, ek kadam me (hold / lock)
  Uber     16  wallet me sirf 40                              ->  poora kaato, aadha nahi (atomic charge)
  Ecom     1   aakhri ghee, aunty + Ravi ek saath             ->  online ko godown, counter ko shelf (channel ka alag stock)
  Ecom     2   ladka uthake likhna bhool gaya                 ->  uthana + parchi ek hi kaam (atomic transaction)

Naam: lock / atomic
Juda: parchi laga ke koi gayab ho jaaye to umar chahiye (2). Ek hi cheez pe bahut bheed to counter todo (7).
```

## 2. Parchi laga ke gayab  (UMAR)

```
Dikkat:      kisi ne parchi lagayi (seat, driver, lock, "online") aur phir aaya hi nahi, cheez hamesha ke liye ruki.
Humne socha: 5 minute dekhunga, phir phaad do. Parchi pe hi umar likh do, apne aap gayab. Ghadi server ki, phone ki nahi.

Yahi dikkat yahan bhi:
  Stock    3   35,000 se do order, chahiye 70,000             ->  EMI jaisa paisa hold (funds block)
  BMS      2   parchi laga ke chai peene, kabhi nahi aaya     ->  5 minute dekhunga, phir phaad do (hold_until)
  BMS      3   parchi phaadne wala chhutti pe                 ->  chhota bhai / replacement (backup safai job)
  BMS      4   20 minute se purani parchi lagi                ->  parchi pe hi umar, apne aap gayab (expiry query me)
  Chat     11  phone band, 2 ghante se "online"               ->  haath maaro, 2-3 baar chup to kaato (heartbeat, TTL)
  Uber     6   Raju 15 sec me accept na kare                  ->  parchi ki umar, agla driver (TTL)
  Uber     9   14.9 pe accept, server pe 16 pe                ->  ghadi server ki, parchi phaad do (server TTL)
  Uber     18  Raju ka phone band, naksha pe gayab            ->  location band, ek-do baar dekho (location timeout)
  Leader   7   raat 12 baje aaj wala khaali                   ->  alag register, kal wala side me rakho (key me tareekh, history)
  Ecom     6   60 ne pay nahi kiya                            ->  cart = list, ginti pay pe, fail to wapas
  Ecom     7c  andar gaya, chai peene chala gaya              ->  "ya pay kar ya rakh de": 5 min ghadi, phir wapas line me (expiry + INCR)
  Ecom     7d  ghadi khatam ke 1 sec baad Pay                 ->  1-2 sec upar bas, phir Priya ka; andar dabaya to PSP hamari zimmedari

Naam: TTL / hold expiry
Juda: single flight ka ladka gira to bhi yahi umar (6). Chat ka "online" bhi isi se (heartbeat).
```

## 3. Ek hi kaam do baar aa gaya  (DOBARA)

```
Dikkat:      net gaya, app ne dobara bheja, ya worker ne commit se pehle crash kiya, to wahi kaam do baar: do SMS, do baar paisa kata, 20 share.
Humne socha: parchi zindabaad: ho gaya likh lo. Same chaabi dobara aaye to dekh ke mana kar do.

Yahi dikkat yahan bhi:
  Notif    3   commit se pehle crash, SMS do baar             ->  parchi zindabaad, ho gaya likh lo (idempotency)
  Stock    2   net gaya, app ne dobara bheja, 20 share        ->  chaabi ka taala khareed liya (idempotency key)
  Payment  6   dobara bhejna pada to do baar kate?            ->  idempotency key laga do (payment id)
  Payment  6b  net atka, Pay do baar daba diya                ->  phone ki key, wahi parchi lauta do (client idempotency key)
  Uber     5b  net atka, Book do baar daba diya               ->  wahi booking id, wahi ride (idempotency key)
  Uber     16b 250 kaatne gaye, gateway chup                  ->  ride id hi key, pehle poochho phir dobara (idempotency)
  Ecom     8b  PSP ka success do baar aaya                    ->  payment id dekh ke doosra chhodo (idempotency)
  Ecom     7e  Ravi ne 4 tab khole, Buy now 4 baar            ->  user id pe ek hi rukega (idempotency)
  Leader   3b  match ka +50 do baar pahuncha                  ->  match id dekh ke ek hi baar jodo (idempotency)
  Chat     6b  Ravi ka "mil gaya" kho gaya, message dobara aaya  ->  Ravi ka phone number dekh ke chhode (idempotency)
  Docs     1b  "kal" ka op dobara gaya, "kal kal"             ->  op id dekh ke chhodo (idempotency)
  File     6c  S3 ne "aa gayi" do baar / ulta bataya          ->  parchi, ho gaya to chhod; purana chhod (idempotency, sequencer)
  News     4   ek khabar 40 baar                              ->  bada aadmi, "ye dobara hai, hatao" (dedup)
  News     5   shabd alag, khabar same                        ->  wordings dekh lo (near-duplicate check)
  Kafka    7   checker punch ke baad, register se pehle gira  ->  pehle aaya hai? check kar le (idempotency)
  Banking  2   receipt se pehle line kati, wahi parchi        ->  log hamare paas, dekh ke mana (pehla record)
  Banking  3   agle din sach me phir 500 bhejne               ->  key jo badle hi na, same parchi (idempotency key)
  Chat     6   lift me jawab kho gaya, "hi hi"                ->  ek key, same aaye to mana (clientMsgId)

Naam: idempotency key
Juda: dobara aata hi isliye ki beech me koi gira (4). Kafka ka retry bhi dobara bhejta, isliye har consumer pe yahi.
```

## 4. Beech me gira, kaam kho gaya  (VASIYAT)

```
Dikkat:      DB me likh diya, aage bhejne se pehle gira; ya paisa kata aur bijli gayi. Aadha kaam hua, aadha kho gaya.
Humne socha: jo yaad nahi rehta, likh lo. Order ke saath vasiyat likh do, peeche wala banda padh ke aage bhejta rahe. Aadha hua to ulta kar do.

Yahi dikkat yahan bhi:
  Twitter  5   3,000 pe server gira, 5,000 ko nahi mila       ->  parchi pe likha, Kafka hi pakki parchi (acks=all)
  Notif    1b  DB me order, Kafka se pehle gira               ->  order ke saath vasiyat likh do (outbox + relay)
  Stock    7   adhoora order RAM ke saath uda                 ->  bag ki parchi dekh ke dobara jamao (order log, replay)
  Payment  5   INITIATED likha, call bhejte server gira       ->  pehle register me likho, phir bhejo (commit first)
  File     6b  UPLOADED likha, scan daalne se pehle gira      ->  UPLOADED ke saath vasiyat (outbox + relay)
  News     14  DB me likhi, parchi se pehle worker mara       ->  status rakh, agla aage badhaaye (state, outbox)
  Kafka    6   admin ne "aa gaya" bola, phir gira             ->  vasiyat ki copy bhi likhe, tab "likh di" (acks=all)
  Kafka    8   Order Service gate tak bhejne se pehle gira    ->  baap ki vasiyat, pehle likh de (outbox)
  Banking  1   500 kate, bijli gayi, Ravi ko nahi mila        ->  mere 500 wapas de do (rollback)
  Banking  6   khabar dene se pehle bijli gayi                ->  jo yaad nahi rehta, likh lo (outbox, relay)
  Chat     2   Ravi ka phone band, 2 ghante                   ->  uncle nahi mile, paise apne paas (message store)

Naam: outbox + relay / acks=all / rollback
Juda: vasiyat padh ke dobara bheja to (3) chahiye. Notification, File upload, Kafka, Banking, News sab me same.
```

## 5. Baar-baar mote register tak bhaagna  (PARCHI)

```
Dikkat:      har padhai ya har chhoti likhai DB tak jaati: DB dheema, bheed lagi, row pe lock.
Humne socha: DB se pehle ek chhota dabba. Padhna ho to pehle parchi. Likhna bahut ho to pehle parchi pe dhadadhad, chhotu baad me batch me mote register me.

Yahi dikkat yahan bhi:
  URL      3   1 lakh/sec, counter wala register palta        ->  chhota pehla register (Redis)
  URL      4   chhota register bhar gaya                      ->  itni der rahega / jo poochha nahi (TTL, LRU)
  URL      12  counter wala har baar uth ke register jaata    ->  apne paas parchi rakh le (local cache)
  Stock    12  9:15 lakhon order, book DB me, line atki       ->  chhoti parchi pe dhadadhad, baad me mota register (book RAM, batch, copy)
  News     8   Priya pehli baar aayi                          ->  pehli baar DB, phir parchi (cache-aside)
  Cache    1   DB 50 ms, Ravi ka profile hazaron baar         ->  DB se pehle ek dabba (cache, Redis)
  Cache    2   dabba bhar gaya, purane profile pade           ->  jo purana, kam use, nikaal do (LRU)
  Docs     2   laakhon save, har badlaav DB pe                ->  chhota register, kuch der baad DB (Redis buffer, batch)
  Netflix  5   TV pe 27 min, phone pe wahi se                 ->  account pe parchi (watch position DB me)
  Netflix  5b  har kuch second update, DB ki saans            ->  race me ruk ke likho (pehle cache, DB kabhi-kabhi)
  YouTube  3   1 lakh view, ek row ke lock pe line            ->  +1 parchi pe, chhotu batch me jode (Kafka, batch count)

Naam: cache (Redis) / LRU, TTL / write buffer + batch
Juda: parchi bhar gayi to purana nikaalo; parchi aur DB alag ho sakte (20).
```

## 6. Parchi khaali, sab ek saath DB pe  (CHHOTA LADKA)

```
Dikkat:      7 baje 1000 log ek saath aaye, parchi me nahi mila, sab mote register pe tut pade.
Humne socha: ek chhota ladka bhaag ke jaaye, baaki yahin ruke. Ladka raaste me gira to kuch der dekho, phir bheed me se ek.

Yahi dikkat yahan bhi:
  URL      5   7 baje 1000 log ek saath mote register pe      ->  chhota ladka bhaag ke ja (single flight)
  URL      6   ladka raste me gira, 999 khade                 ->  ladke ki timeline, 999 me se ek (lock TTL)
  Cache    8   Priya viral, 10,000 miss, sab DB pe            ->  ek chhotu bhejo, baaki yahin (single flight)
  Cache    9   chhotu raaste me gira                          ->  kuch second dekho, phir bheed me se ek (lock timeout)

Naam: single flight / lock timeout
Juda: ladke ki timeline = parchi ki umar (2). URL aur Cache dono me same.
```

## 7. Ek hi jagah pe saari bheed  (EK AUR GAADI)

```
Dikkat:      viral cheez, ek hi key / ek hi row / ek hi counter pe crore log; baaki machine khaali baithi.
Humne socha: us ek ki photocopy bana do, ek aur gaadi; ya counter tod do (10 counter); khaali ladke laga pada wale ki madad karein.

Yahi dikkat yahan bhi:
  URL      11  viral token, 1 crore log ek hi register pe     ->  register ki photocopy, sab me baant do (Redis replica)
  Twitter  6   six pe 5 crore, parchi wali machine hili       ->  10-15 copy bana do, baant do (hot key replica)
  News     11  iPhone launch, 10 lakh/sec, dabba 1 lakh       ->  tech ke dabbe badha do (aur Redis node + tech ki copy)
  News     12  cricket wale dabbe khaali pade                 ->  unme bhi tech ki copy (hot key copies)
  Cache    10  1 crore log sirf machine 4 pe                  ->  machine 4 ka replica, ek aur gaadi (hot key copies)
  Banking  19  9 chhotu khaali, ek laga pada                  ->  khaali ladke madad karo (hot key, work stealing)
  Ecom     4   ek parchi pe 50 hazaar                         ->  10 counter, tez kaatne wala (hot row todo, Redis DECR)
  Ecom     5   counter 3 khatam, counter 7 pe 30              ->  khaali counter band, sold out tab jab sab 0

Naam: hot key copies / replica / hot row todo
Juda: machine badhane se nahi hota, kyunki ek key ek hi machine pe (8).
```

## 8. Ek machine me nahi aata  (BAANTO + RING)

```
Dikkat:      TB-TB data, ya lakhon likhai, ek machine / ek register me nahi samaata.
Humne socha: tod do, tukde baanto, hash se pata kis almari me. Machine jude to sab na hile: ring, kho-kho, paas ka petrol pump.
             Jahan RING, wahan CONSISTENT HASHING: Cassandra, DynamoDB, cache ki machines, Docs ka owner server, sab andar yahi.

Yahi dikkat yahan bhi:
  URL      8   90 TB ek almari me nahi aata                   ->  tukde, hash se almari (shard; Cassandra khud baantta, andar ring = consistent hashing)
  Twitter  9   din me 50 crore tweet, likhai ki line          ->  bada bhai, "tu dekh le" (Cassandra, andar ring = consistent hashing)
  Twitter  11  "Arpan ke naye 500" poori table me             ->  naam se nahi, id se baanto (partition key = user id)
  Payment  10  Diwali 3,000/sec, ek register                  ->  register ke chhote tukde (shard)
  Payment  11  wallet tukda 1 me, dukaan tukda 3 me           ->  ladka diary le ke jaaye (saga do shard ke beech)
  Cache    3   1 TB ek machine me nahi aata                   ->  tod do, hash se same dabba (sharding)
  Cache    4   11vi machine judi, har key ki jagah badli      ->  kho-kho, sabse paas wala, paas ka petrol pump (consistent hashing)
  Docs     5   lakhon doc, sab ek hi jagah?                   ->  har doc ka alag, uska maalik (owner server)
  Banking  12  5 saal ki line roz jodna bhaari                ->  mahine ke mahine tod do (partition, snapshot)
  Uber     2   1.25 lakh update/sec, DB ghuta                 ->  bada bhai bulao (Cassandra, andar ring = consistent hashing)
  Uber     3   5 lakh driver ka fasla naap raha               ->  naksha dabbon me, sirf apna dabba (geohash)
  Uber     4   dabbe ke kinare, bagal me driver               ->  8 km ka gola, chhoote dabbe bhi (neighbour geohash)
  Uber     11  register driver ke naam se, 5 lakh line        ->  jagah ke hisaab se panna (geohash ka Redis set)
  Uber     12  dabba badla, purana naam pada                  ->  nikla to hata do, naye me daal do (remove + add)
  Leader   3   lakhon update ek Redis pe                      ->  region se baant do, chhotu mila ke badi screen (shard, scatter-gather)
  Leader   4   India me 3,000, duniya ka rank                 ->  race wala: har line me mere upar kitne, jodo (count)
  Leader   8   mere aage-peeche ke 5, alag dabbon me          ->  bheed me aas-paas scan, paas ka petrol pump (score range, merge)

Naam: shard / partition key / consistent hashing
Juda: URL 8 aur Cache 4 dono me ring wahi (Cassandra andar khud karta). Bheed ek tukde pe ho to (7).
```

## 9. Machine giri, ya copy peeche  (COPY)

```
Dikkat:      ek hi dabba tha aur wo gira; ya copy se padha aur wahan abhi aaya hi nahi.
Humne socha: ek pe bharosa nahi: copy bana do, do me likho, do se padho. Jisne abhi likha, thodi der main file se padhe.

Yahi dikkat yahan bhi:
  URL      9   almari me aag                                  ->  copy + acknowledge (quorum)
  URL      10  C se padha, nahi mila                          ->  do se padho (R=2)
  Rate     6   Redis gira, sabko jaane de ya roke             ->  papa ke dost andar, chor bahar (fail-open / fail-closed)
  Twitter  17  worker 7 gira, "processing" ghanton se         ->  "tu hai kya?", jawab nahi to naya banda (heartbeat)
  Stock    5   book RAM me, machine mar gayi                  ->  permanent dabbe me rakho (DB)
  Stock    6   permanent dabba bhi gira to                    ->  3-4 copy bana ke rakho (replica)
  News     13  A me khabar, B se refresh pe gayab             ->  do me likho, do se padho (quorum)
  BMS      5   sirf dekhne wale bhi bojh daal rahe            ->  dekhne wale ki alag diary (read replica)
  Docs     6   "Saved" dikhaya, DB se pehle server gira       ->  dabbe ki ek aur copy (Redis copy)
  Docs     6b  dabba + copy dono gire, batch baaki            ->  disk pe bhi likho; phir bhi gire to kuch second chhoot, 500 copy ka paisa (AOF, trade-off)
  Docs     7   owner server hi gira                           ->  "mujhe kuch ho to tu hai" (naya owner wahin se)
  Banking  7   register jal gaya                              ->  laptop me save, ghar le jaao (replica, Multi-AZ)
  Banking  8   2 baje likha, 3 baje aag                       ->  turant laptop pe, tabhi pakka (sync copy)
  Banking  9   sab balance dekhne aaye, kaam atka             ->  photocopy alag desk pe (read replica)
  Banking  10  photocopy peeche, ghabra ke dobara bheja       ->  jisne likha, thodi der main file se (read-your-writes)
  Banking  22  2 baje chhotu ne register gher liya            ->  chhotu photocopy se padhein (read replica)
  Leader   9   Redis gira                                     ->  clone, phir clone, teeno gaye to DB se bharo (replica, DB sach)

Naam: replica / quorum / sync copy / read-your-writes
Juda: sirf wahi "maan lo gira" jahan HUM faisla lete (sync ya async, fail-open), AWS wala nahi.
```

## 10. Kaam lamba, user ko mat roko  (LINE LAGWAO)

```
Dikkat:      button dabaya aur server khud sab kar raha (SMS, scan, fanout), user 8-10 second ghoomta dekh raha.
Humne socha: stadium ke bahar bheed: admin line banaye, ek-ek karke. User ko "ho gaya, nikal", kaam peeche chhotu kare.

Yahi dikkat yahan bhi:
  Rate     10  SMS company bhi giri                           ->  line lagwa, ek-ek karke (queue, Kafka)
  Twitter  4   8,000 dabbe, Post ka button ghoome             ->  stadium wala bhai, "post ho gaya, nikal" (Kafka)
  Notif    1   order khud SMS call kare, button 8 sec         ->  beech me dabba, aaram se bhejunga (queue, Kafka)
  Notif    2   ek hi line, bill aadhe ghante baad             ->  ladies, gents, bachchon ki alag line (topic, group)
  Notif    6   OTP bhi baaki ke peeche line me                ->  VIP darshan, OTP ki priority (alag OTP topic)
  Stock    8   sauda pakka, engine khud sabko call kare       ->  10 ladkon ki fauj (Kafka, consumer group)
  File     1   server khud sab kare, 10 min spinner           ->  chai pi aao, peeche line (Kafka + worker)
  File     11  scan baaki kaam ke peeche atka                 ->  doctor line me nahi lagta (alag scan topic)
  News     6   bada aadmi akela, 1 min ek khabar              ->  stadium wali kahani, chhotu daal ke chale (Kafka)
  Kafka    1   stadium ke bahar bheed, sab ghus rahe          ->  admin: line banao, ek-ek karke (Kafka)
  Kafka    3   ek admin se nahi sambhal rahi                  ->  aur gate, har gate ki apni line (partition)
  Kafka    4   har ticket 3 baar punch                        ->  ek kamre me bitha do, 3 me se ek (consumer group)
  Kafka    5   security wale bhi usi kamre me                 ->  alag room (alag consumer group)
  Kafka    13  7 din peeche, purana panna phat jaata          ->  aur checker daalo, gate bhar gaye to gate bhi (consumer, phir partition)
  Kafka    14  aaj 1 din, kal 2 din peeche                    ->  alert laga do (consumer lag alert)

Naam: Kafka / queue + worker / consumer group
Juda: line me se gira to (4) aur (3). Line peeche reh gayi to alert.
```

## 11. Bahar wali company chup ya band  (PSP / SMS)

```
Dikkat:      company hamari nahi (PSP, SMS): chup ho gayi, dheemi, band, ya "abhi bheed hai".
Humne socha: apne paas parchi (PENDING), beech me banda poochta rahe. Thodi der ruk ke dobara; band ho to doosri company; baar-baar fail to traffic mod do. Paisa ulta karna ho to ek-ek karke wapas.

Yahi dikkat yahan bhi:
  Notif    4   company bole "abhi bheed hai"                  ->  papa busy, thodi der ruk ke (retry queue)
  Notif    5   SMS company poori band                         ->  teen-chaar company extra (failover)
  Notif    5b  company 30 sec chup (timeout)                  ->  OTP turant backup se, 2 OTP chalega, ek hi code; sale ruk sakti (SLA)
  Notif    7   fail pe fail, company band kaise samjhun       ->  2-3 baar fail to traffic backup se (circuit OPEN)
  Notif    8   main company wapas theek hui?                  ->  beech-beech me "tu theek hai?" (HALF-OPEN)
  Notif    9   backup bhi same fail bole                      ->  number galat, alag daal do (DLQ)
  Notif    10  20 worker 300/sec, company ki had 100          ->  saanjha dabba, sab usi se guzrein (token bucket, Redis)
  Notif    11  company ne "OK" bola, SMS pahuncha nahi        ->  "le liya", baad me sent ya fail bataye (webhook)
  Notif    11b le liya bola, DELIVERED kabhi aaya hi nahi     ->  cron job chhotu poochhe, na mile to FAILED + backup (status poll)
  Stock    9   portfolio DB band, sauda ulta kar do?          ->  rollback nahi, consumer court (settlement PENDING)
  Stock    10  paisa kata, share dikh nahi rahe               ->  parchi apne paas, DB aaye to de do (Kafka retry)
  Payment  1   HDFC hamari sunta hi nahi                      ->  centralized authority, uski API (PSP)
  Payment  2   PSP 30 sec chup, FAILED ya SUCCESS?            ->  apne paas parchi, INITIATED (payment status)
  Payment  3   PSP batana bhool gaya, message khoya           ->  beech me banda jo poochta rahe (status job)
  Payment  4   PSP-1 poora band, payment ruke                 ->  PSP-2 pe bhej do (fallback PSP)
  Payment  7   wallet se kata, ICICI ne FAILED bola           ->  sab ulta kar do, paise wapas (saga)
  Payment  9   "sab taiyaar ho?" poochh ke kaato              ->  slow dost ki wajah se mera paper chhoota (2PC nahi)
  Payment  12  PSP-1 chup, mara ya dheema pata nahi           ->  processing dikhao, haan ya naa ka waqt do; pata ho gira tabhi PSP-2 (PENDING, timeout, refund)
  BMS      8   Pay daba diya, PSP dheema, umar khatam         ->  parchi pe "pay chal raha", umar badhao (PAYMENT_PENDING)
  BMS      9   chhotu (PSP) 1-2 baar jagaya, phir bhi chup    ->  FAILED dikha do, seat chhodo, "paisa kata ho to wapas"; naye log doosre PSP pe (breaker), baad me kata nikla = refund
  BMS      10  baad me chhotu bole "fail ho gaya"             ->  user ko kuch nahi, khud dobara / doosra chhotu (retry, PSP-2)
  Kafka    9   galat ticket baar-baar, line atki              ->  line se hata (DLQ)
  Kafka    11  SMS counter 10 min band, sab DLQ me            ->  pehle check, waqai band? (circuit breaker)
  Kafka    12  counter waqai band hai                         ->  road band, traffic divert (fallback)
  Uber     17  payment fail, Arpan ghar ke andar              ->  paisa nahi aaya to ride khatam nahi (PAYMENT_PENDING)
  Ecom     8   net gaya / PSP chup                            ->  BMS wala PAYMENT_PENDING + status job, kinaare pe refund

Naam: PENDING + status job / retry / fallback / circuit breaker / saga / DLQ
Juda: Payment, BMS, E-com, Uber me wahi PAYMENT_PENDING. Notification aur Kafka me wahi circuit breaker.
```

## 12. Phone baar-baar "kuch aaya?"  (KHULA TAAR)

```
Dikkat:      phone har second poochh raha, ya 10 lakh phone ek hi cheez dekh rahe.
Humne socha: poochne ka mauka hi mat do. Taar khula rakho, hum khud batayenge. Doosre server pe hai to chhota ladka address le ke jaaye.

Yahi dikkat yahan bhi:
  Stock    11  bhaav har second, 10 lakh screen               ->  chat wala hi (WebSocket + Redis pub/sub)
  Docs     9   Ravi doosre server se likh raha                ->  building ke bahar chilla de (pub/sub invalidation)
  Docs     10  server 7 ne awaaz suni hi nahi                 ->  copy note kar le, wapas aake dekh (standby, reconnect)
  Chat     1   phone har second "kuch aaya?"                  ->  poochne ka mauka hi na do (WebSocket)
  Chat     3   100 server, Ravi kis pe                        ->  DSA wala map, O(1) (session map, Redis)
  Chat     4   har server baaki 99 se taar jode?              ->  chhota ladka address le ke jaaye (Redis pub/sub)
  Uber     1   server ko pata nahi driver kahan               ->  phone khud bheje, ek dabba (Location Service)
  Uber     13  10 lakh phone "Raju kahan?"                    ->  poochne ka mauka mat do (push, WebSocket)
  Uber     14  Arpan server 12 pe, Raju 57 pe                 ->  ghar ka raasta pata, seedha usi raaste (pub/sub)

Naam: WebSocket / Redis pub/sub / session map
Juda: Chat, Stock, Uber, Docs, sab me ek hi cheez.
```

## 13. Pehle kya aaya, kram gadbad  (NUMBER)

```
Dikkat:      ghadiyan aage-peeche, server dheema, "nahi, parso" pehle pahuncha; do log ek saath likh rahe.
Humne socha: phone pe bharosa nahi, server number lagaye, phone us kram me lagaye. Ek doc ka ek maalik, sab uske hisaab se.

Yahi dikkat yahan bhi:
  Docs     3   "Hum" aur "kal", kal galat jagah               ->  shabdon ko khiska do (OT)
  Docs     4   do server, ek doc ki do shakal                 ->  ek centralized system, ek version
  Kafka    2   line me ek aadmi nikla, sab atke               ->  seat booked, wahi baithega (offset)
  Chat     7   "kahan?" pehle dikha, ghadiyan aage-peeche     ->  server lagayega number (sequence)
  Chat     7b  #980 atka, #981 pehle, phone "981 ke baad do"  ->  server ka record, jo nahi gaya sab do; phone sort kare (store-and-forward)
  Chat     12  S1 dheema, "nahi, parso" pehle pahuncha        ->  server ka number, phone us kram me lagaye (seq, client sort)
  Leader   2   Ravi, Priya dono 9800                          ->  score phir time, ek number me (thenComparing)
  Twitter  26  scroll ke beech upar 5 naye, wahi 5 dobara     ->  kahan tak padha yaad rakh (cursor, offset nahi)
  Notif    4b  Shipped pehle, Order placed baad me            ->  order ki parchi, gaya nahi to dono saath / doosri company / email
  Notif    4c  OTP retry line se 3 ghante baad gaya           ->  har khabar ki umar, khatam to phenko; OTP VIP line (TTL, priority)
  Rate     10b line lambi, OTP 6 min baad, bekaar             ->  wahi umar + VIP line (Notification 4c jaisa)
  Kafka    9b  pay DLQ me, cancel chal gayi, kram toota       ->  group ka ek atka to us user ki baaki bhi park (same key parking)
  Netflix  5c  TV ka purana 27, phone ke 37 ke upar           ->  baad wala jeete, purana discard (timestamp, last-write-wins)
  Leader   7b  11:59:58 ka match, score 12:00:02 pe           ->  parchi pe khatam hone ka waqt, usi din ka register (event time)

Naam: sequence number / offset / OT
Juda: Kafka ka offset bhi yahi number.
```

## 14. Badi file, door ka store  (LOCKER + PAAS)

```
Dikkat:      GB-GB ki file hamare server se guzar rahi; ya har tukda samundar paar se; ya purana maal bhi mehngi jagah.
Humne socha: badi file apne paas mat rakho, seedha locker (S3), watchman bas parchi rakhe. Tukdon me bhejo, tukdon me banao. Dekhne wale ke paas copy (CDN). Garam paas, thanda door.
             Upload: Twitter ka video, YouTube ka video, File upload = ek hi kahani. Netflix pe upload nahi (rights khareedta),
             wahan sirf paas rakhna (CDN) aur kai size (Ferrari / Alto).

Yahi dikkat yahan bhi:
  Twitter  14  50,000 log 1 GB video, server bhara            ->  alag route pe ja, URL se dekhenge (S3, presigned URL)
  Twitter  15  John 4K TV pe, Ravi gaon me phone pe           ->  Ferrari highway, Alto gaon (transcoding, ABR)
  Twitter  16  ek machine 4 size banaye, 20 minute            ->  kai machine, video id ki parchi pe baanto (workers)
  Twitter  18  2:00 pe khiskaya, phone 0:00 se kheenche       ->  copy dekh ke seedha 51wa page (manifest, seek)
  Twitter  19  1 crore log, har tukda hamari API se?          ->  bas file hai, CDN pe rakh do (static)
  Twitter  20  1 crore bachche yaad rakhein kaunsa page       ->  bookmark, 51 khud palat lo (player client side)
  Twitter  21  tukde US ke S3 me, samundar paar               ->  copy paas me rakho, paas se khule (CDN edge)
  File     3   1000 x 2 GB hamare server se                   ->  seedha bade dabbe me (S3)
  File     4   8 GB pe net gaya                               ->  tukdon me todo, sirf baaki 2 GB (multipart)
  File     5   S3 kaise maane ye hamara user                  ->  receptionist ka token (presigned URL)
  File     6   file S3 me aayi, server ko pata nahi           ->  S3 bataye "aa gayi" (event notification)
  File     9   download bhi hamare server se                  ->  truck seedha ghar (S3 direct download)
  File     10  link 200 logon ke WhatsApp group me            ->  link ki umar chhoti (presigned expiry)
  Banking  13  almari me jagah nahi, 10 saal rakhni           ->  head office bhej do (S3 archive)
  Chat     10  200 MB video, taar jam                         ->  alag raasta, chhota URL (S3, link)
  Netflix  1   har tukda samundar paar se                     ->  file apne shehar me, CA wala dost (Open Connect, ISP ke andar)
  Netflix  3   dabbe me poora maal nahi                       ->  garam paas, thanda bade dabbe se (popularity, tier, evict)
  Netflix  4   20 dabbe, kis se maange                        ->  hospital ka guard, server batata (steering; AWS pe, video bytes dabbe se)
  Netflix  7   tunnel me 3 sec net gaya, video ruki           ->  10 ki jagah 20 packet, aage ke tukde paas (player buffer)
  Netflix      (padhai, scene nahi) P1 AWS CDN kyun nahi: apna fridge muft · P3 Ferrari / Alto + har film ki seedhi + kai roop
  YouTube  1   10 hazaar creator, video server me phansi      ->  file seedha locker, watchman bas parchi rakhe (S3, presigned URL)
  YouTube  2   ek machine har quality, 3 ghante               ->  20-25 log, tukde baanto, chhoti quality pehle (parallel encoding)
  YouTube  5   10 saal purani video bhi mehngi jagah          ->  garam garam jagah, thandi thandi jagah (hot / cold storage)
  YouTube  6   2010 ka gaana aaj trend pe                     ->  thodi der garam + CDN pe, phir wapas (promote / demote)

Naam: S3 + presigned URL / multipart / transcoding + ABR / CDN / hot-cold
Juda: link kisko mile wo gate pe (16).
```

## 15. Bhaari kaam, us waqt mat karo  (PEHLE SE BANA KE)

```
Dikkat:      user aaya tab jodna shuru kiya (500 logon ke tweet, 3 saal ka itihaas, 1 crore statement) to 6 second khaali screen.
Humne socha: jab pata hai kya maangega, pehle se bana ke rakh lo (parlour wala GTA). Bhaari kaam peeche chhotu raat ko, tukdon me, jahan ruka wahin se.

Yahi dikkat yahan bhi:
  Twitter  1   500 logon ke tweet milao, feed 6-7 sec         ->  apne paas rakhunga, apne paas se bhejunga (fanout on write)
  Twitter  2   Kohli ke 27 crore dabbe, ghanton lage          ->  ek chhoti parchi, sab yahin se padhein (fanout on read)
  Twitter  3   doston ka dabba + 15 celeb parchiyan           ->  ek parchi pe, DSA jaisa sort (merge at read)
  Notif    12  1 crore offer, 40 lakh pe crash                ->  50 worker, alag-alag range (chunks)
  Notif    13  dobara chala to loop shuru se                  ->  parchi pe likho "ye ghar ho gaya" (checkpoint DB)
  News     1   5000 site, khabar aayegi kaise                 ->  chhotu, sab utha ke dabbe me (fetcher, DB)
  News     2   chhotu akela, ek chakkar 3 ghante              ->  bahut saare chhotu (kai fetcher)
  News     3   khabar pehle se taiyaar kahan                  ->  chhoti parchi, akhbaar jaisi (Redis feed)
  News     7   Ravi cricket, Priya tech, ek parchi            ->  category ke hisaab se baanto
  News     9   9:05 ki iPhone khabar purani parchi me nahi    ->  aate hi tech walon ki feed me (push on write)
  News     10  50 lakh alag parchi, 10 crore likhai           ->  saari tech khabar ek parchi (category feed)
  Banking  16  31 March 1 crore log ek saath                  ->  peeche chhotu, pehle se bana ke rakho (precompute, batch)
  Banking  18  chhotu aadha kaam karke gira                   ->  DB me likhta jaata, wahin se utha le (checkpoint)
  Banking  20  ek khaata 5 crore, statement ek                ->  kaam baanto, bada aadmi jode (MapReduce)
  Banking  21  May wale ko 1 May ka balance chahiye           ->  master register se utha le (balance snapshot)
  Chat     5   group ke 500, ek message 500 kaam              ->  ek dabbe me, log khud aake sunein (ek baar DB)
  Leader   5   poora 1 crore ka board                         ->  referee wala: chhotu har kuch minute (snapshot job)
  Leader   6   aaj / hafta / all-time                         ->  score teeno dabbon me
  Netflix  2   12:29 tak dabba khaali                         ->  pen drive pehle se, video badalta nahi (proactive fill)
  Netflix  6   "tere liye" me 6 second                        ->  parlour wala GTA pehle se (raat ka batch, cache se uthao)
  Netflix  6b  11 baje Dhurandhar dekhi                       ->  turant parchi, dikhate waqt milao (abhi ka fast store + bani list)
  Netflix  6c  Priya naya user                                ->  India ka garam maal (cold start)
  YouTube  7   MrBeast ki video, crore subscriber             ->  Twitter wala fan-out, sabko push
  Twitter  25  Amit ka tweet 40 ke neeche dab gaya            ->  jisse judta wo pehle (ranking)

Naam: fanout on write / precompute / batch / checkpoint / MapReduce
Juda: Kohli jaise 27 crore pe ulta: padhte waqt milao (fanout on read).
```

## 16. Galat cheez andar aaye hi kyun  (BOUNCER)

```
Dikkat:      script 1000 OTP, nakli view, nakli chit, exe ko PDF bol ke, ya galat aadmi ka file khul gaya.
Humne socha: bouncer gate pe hi bitha do: had laga do, baar-baar kare to ban, galat cheez gate pe hi rok do, sahi aadmi hi dekhe.

Yahi dikkat yahan bhi:
  URL      13  dhokhe wali site ka token                      ->  radd karo, aage se pehle hi jaancho (kaali list)
  URL      14  radd kiya, log phone wala pata khol rahe       ->  phone me likhne hi mat do (302)
  URL      15  machine se hazaar nakli chit                   ->  gate pe bhaga do, had per user (WAF, rate limit)
  Rate     1   script har second 1000 OTP, bill chadha        ->  ek bande ka threshold (rate limit, 429)
  Rate     3   khaata badla, 3 second me 10 OTP               ->  dabbe se token nikaalo, apni raftaar se (token bucket)
  Rate     5   guard andar kamre me, 1000 ghus gaye           ->  guard bahar gate pe bitha do (API gateway)
  Rate     7   script 429 khaati, phir bhi maarti rehti       ->  dabbe me gala pakad ke bitha do (ban list)
  Rate     8   office ke 500 log ek WiFi, sab atke            ->  login se pehle dheeli had, baad me userId pe
  Rate     9   script roz hazaar baar reset maar rahi         ->  insaan hazaar baar nahi karta, pattern (WAF)
  Twitter  22  private video ka link WhatsApp group me        ->  taala-chaabi mere paas, 10 hi (allow list)
  Twitter  23  CDN ko allow list pata hi nahi                 ->  CDN ke paas bhi taala (signed URL / cookie)
  File     2   PDF bol ke exe chadha de                       ->  chhota tukda dekh lo (magic bytes)
  File     8   1043 likha, Priya ka statement khula           ->  "Priya ko saath la" (owner / share check)
  File     12  scan me 10 min baaki, download chahiye         ->  scan poora tak link hi nahi (CLEAN tak band)
  File     13  scan me virus nikla                            ->  file lo hi mat (quarantine / delete)
  Docs     8   Ravi hataya, server pe purani permission       ->  chhota note, uska op accept nahi (permission check)
  Kafka    10  galat ticket gate tak aaye hi kyun             ->  bouncer, gate pe hi rok de (validation)
  YouTube  4   ek phone se 10 lakh "view"                     ->  gino hi mat, gate pe rok, baar-baar to ban (rate limit, WAF)

Naam: rate limit / token bucket / API gateway / WAF / validation / permission check
Juda: Rate limiter, URL, File, Kafka, Docs, Twitter video, YouTube views: ek hi gate.
```

## 17. Hall chhota, bheed badi  (LINE ME LAGAO)

```
Dikkat:      hall 1000 ka, bahar 5000; ghee 500, log 3000; cab 500, log 50 hazaar.
Humne socha: jitni jagah utne andar, baaki line me. Maal hi kam hai to FR me hi bol do: 500 ko hi milega.

Yahi dikkat yahan bhi:
  BMS      6   hall 1000 ka, saamne 5000 khade                ->  capacity jitne andar, baaki line (virtual queue, token)
  BMS      7   token khatam, parchi ke 3 min baaki            ->  token gate ka, seat ki ghadi parchi ki (do ghadiyan)
  Uber     7   concert khatam, 50 hazaar, cab 500             ->  500 andar, baaki line (virtual queue)
  Uber     8   baaki ke liye cab hi nahi                      ->  kiraya badha ke door ki cab (surge pricing)
  Ecom     0   sale ka maal seemit                            ->  FR me hi cut off: 500 ko hi, baaki out of stock (consistency > availability)
  Ecom     3   sale ki bheed, aata-daal bhi atka              ->  sale ki alag dukaan (alag service)
  Ecom     7   3000 ek second me Pay                          ->  server pe line, 500 ke baad rollback (ya ghadi wala hold, dono ki keemat)
  Ecom     7b  rollback + ghadi dono me pech                  ->  Apple store: sale gate, stock jitne hi Pay tak, fail = agla batch

Naam: virtual queue / sale gate / surge
Juda: andar gaya to ghadi (2), pay pe (11).
```

## 18. Zyada bhej rahe, zyada rakh rahe  (KAM KARO)

```
Dikkat:      poora text bhej rahe, 3 saal ke tweet dabbe me, 4.9 lakh "padha" entry, 500 copy rename pe.
Humne socha: poora mat bhejo, sirf jo badla. Poori list mat rakho, ek number se kaam chala lo.

Yahi dikkat yahan bhi:
  URL      2   token lamba ho raha                            ->  gaadi ki number plate (base62)
  Twitter  7   dabbe me 3 saal ke tweet, memory ka bill       ->  parchi chhoti, bas last 500 (inbox had)
  Twitter  8   30 crore mahino se app nahi khol rahe          ->  30 din nahi aaya to kuch taiyaar nahi (on-demand inbox)
  Notif    14  har SMS se pehle saancha DB se                 ->  ek hi saancha apne paas, bas naam badlo (template cache)
  Notif    15  OTP worker bhi pasand padhne DB pe             ->  OTP seedha bhejo, DB se lena-dena nahi
  File     7   folder rename = 500 copy + 500 delete          ->  tree jaisa, sirf parent badlo (parent_id)
  Docs     1   Save pe poora text, Arpan ki line gayab        ->  poora text nahi, operation bhejo (insert / delete)
  Chat     8   "12 naye message" kaise dikhe                  ->  ek ginti, itne padh liye (last_read_seq)
  Chat     9   kisne padha, 4.9 lakh entry                    ->  978 matlab 975 padha, number ghata lo (watermark)
  Twitter  24  dabbe me sirf number, screen pe poora tweet    ->  ID se seedha page, photo CDN se (hydration)

Naam: operation / inbox had / watermark / parent_id / template cache
```

## 19. Server door baitha hai  (BRANCH)

```
Dikkat:      Ravi Delhi me, server America me; VPN badal ke teen region.
Humne socha: paas me branch khol do, branches aapas me baat karein. Ginti ek ghar region me.

Yahi dikkat yahan bhi:
  Rate     11  VPN badal ke teen region, 15 OTP               ->  ek ghar region, ginti wahin (home region)
  Rate     12  US ko kaise pata ghar Mumbai                   ->  +91 dekh lo, IP badle number nahi (country code)
  Twitter  12  Ravi Delhi me, server America me               ->  Lajpat Nagar me branch khol do (India region)
  Twitter  13  John New York me, Arpan ka tweet nahi          ->  SBI ki do branch aapas me baat karein (cross-region)

Naam: multi-region / home region
Juda: video ke liye yahi kaam CDN karta (14).
```

## 20. Sach kahan hai  (SACH)

```
Dikkat:      parchi me ek, DB me doosra; kisi ne haath se badal diya; GPS purana; paisa dikha kuch, kata kuch.
Humne socha: sach ek jagah. Kaato mat, neeche likho (ledger). Parchi purani ho sakti, DB sach; alag-alag ho to milao. Jo dikhaya wahi lo.

Yahi dikkat yahan bhi:
  Rate     2   10 server, har ek ke paas alag ginti           ->  tu chhod, hisaab main rakhunga (central counter, Redis)
  Twitter  10  kaun kisko follow karta, iska track            ->  ek aur bada bhai (Neo4j)
  Stock    1   Arpan Ravi ko jaanta nahi, kaun milaye         ->  beech me ek aadmi, bhaav milao (matching engine)
  Stock    13  parchi pe match, DB me abhi nahi               ->  "processing" turant, mote register ke baad pakka (DB = sach)
  Payment  8   2 minute 500 na yahan na wahan                 ->  ledger me likh lo, paise yahan hain (hold)
  Cache    5   naam badla, cache me purana "Ravi"             ->  pehle parchi kaato, phir DB (cache DELETE)
  Cache    6   beech me purana naam phir parchi pe            ->  banda check kare, DB sach hai (sync job)
  Cache    7   kuch second purana Ravi dikha                  ->  paisa thodi hai (eventual consistency)
  Cache    7b  "pehle DB phir kaato, ulta kyun nahi?"         ->  chhed dono me, banda dono me chahiye (write-then-invalidate + retry)
  Cache    11  sync job ko kaunsi key dekhni                  ->  pichhle ghante me jo update hui (updated_at)
  Banking  5   "500 mile hi nahi", saabit kaise               ->  alag diary, kaato mat, neeche likho (ledger)
  Banking  11  haath se 100 ko 600 kar diya                   ->  ledger se milwao, wahi asli sach (reconciliation)
  Banking  14  godown me -500 ko -50 kar diya                 ->  kai copy chhupa ke, milao (copies)
  Banking  15  ijazat wala file badal hi na paaye             ->  kohinoor, taala maar do (S3 Object Lock)
  Banking  17  28 Feb ka transaction der se aaya              ->  record se milao, nayi statement (reconciliation, revised)
  Uber     10  Raju Janpath me, purana naam pada              ->  sach wahi jo GPS ne abhi bheja (latest location)
  Uber     15  250 dikha, aakhir me 500 kate                  ->  jo dikhaya wahi lo (upfront fare)
  Leader   1   1 crore sort kiye bina top 10 + rank           ->  apne aap sort wala dabba (Redis sorted set)

Naam: ledger / reconciliation / eventual consistency / sahi dabba
Juda: paisa ho to sach turant chahiye; profile ho to thodi der purana chalega.
```

---

## Notes ke link

- URL: [notes](SYSTEM_DESIGNS/01_url_shortener/URL_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/01_url_shortener/01_url_shortener.md)
- Rate: [notes](SYSTEM_DESIGNS/02_rate_limiter/RATE_LIMITER_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md)
- Twitter: [notes](SYSTEM_DESIGNS/03_twitter_feed/TWITTER_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md)
- Notif: [notes](SYSTEM_DESIGNS/04_notification_system/NOTIFICATION_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/04_notification_system/04_notification_system.md)
- Stock: [notes](SYSTEM_DESIGNS/05_stock_broker_trading/STOCK_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/05_stock_broker_trading/05_stock_broker_trading.md)
- Payment: [notes](SYSTEM_DESIGNS/06_payment_system/PAYMENT_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/06_payment_system/06_payment_system.md)
- File: [notes](SYSTEM_DESIGNS/07_file_upload_validate_system/FILE_UPLOAD_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/07_file_upload_validate_system/07_file_upload.md)
- News: [notes](SYSTEM_DESIGNS/08_news_aggregator/NEWS_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/08_news_aggregator/08_news_aggregator.md)
- BMS: [notes](SYSTEM_DESIGNS/09_bookmyshow/BMS_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/09_bookmyshow/09_bookmyshow.md)
- Cache: [notes](SYSTEM_DESIGNS/10_distributed_cache/CACHE_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/10_distributed_cache/10_distributed_cache.md)
- Docs: [notes](SYSTEM_DESIGNS/11_google_docs_collab/DOCS_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/11_google_docs_collab/11_google_docs_collab.md)
- Kafka: [notes](SYSTEM_DESIGNS/12_message_queue_kafka/KAFKA_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/12_message_queue_kafka/12_message_queue_kafka.md)
- Banking: [notes](SYSTEM_DESIGNS/13_banking_system/BANKING_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/13_banking_system/13_banking_system.md)
- Chat: [notes](SYSTEM_DESIGNS/14_chat_messaging/CHAT_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/14_chat_messaging/14_chat_messaging.md)
- Uber: [notes](SYSTEM_DESIGNS/15_uber/UBER_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/15_uber/15_uber.md)
- Leader: [notes](SYSTEM_DESIGNS/16_leaderboard/LEADERBOARD_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/16_leaderboard/16_leaderboard.md)
- Ecom: [notes](SYSTEM_DESIGNS/17_ecommerce/ECOMMERCE_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/17_ecommerce/17_ecommerce.md)
- Netflix: [notes](SYSTEM_DESIGNS/18_netflix/NETFLIX_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/18_netflix/18_netflix.md)
- YouTube: [notes](SYSTEM_DESIGNS/19_youtube/YOUTUBE_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/19_youtube/19_youtube.md)
