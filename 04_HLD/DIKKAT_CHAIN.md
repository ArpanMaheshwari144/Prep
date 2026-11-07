# HLD dikkat chain

Ek nazar me yaad dilane ke liye: har line ek dikkat, aage tera hal. Bhool jaao to (notes) pe click.

## 1. URL shortener   ([notes](SYSTEM_DESIGNS/01_url_shortener/URL_HAND_NOTES.md))

```
 1  do logon ko same token                      ->  ek line wala counter, aage badhao
 2  token lamba ho raha                         ->  gaadi ki number plate (base62)
 3  1 lakh/sec, counter wala register palta     ->  chhota pehla register (Redis)
 4  chhota register bhar gaya                   ->  itni der rahega / jo poochha nahi (TTL, LRU)
 5  7 baje 1000 log ek saath mote register pe   ->  chhota ladka bhaag ke ja (single flight)
 6  ladka raste me gira, 999 khade              ->  ladke ki timeline, 999 me se ek (lock TTL)
 7  5 counter, dono ne "10" diya                ->  apni-apni range
 8  90 TB ek almari me nahi aata                ->  tukde, hash se almari (shard)
 9  10vi almari, sab galat jagah dhoondh rahe   ->  gol chakkar, paas ka shelter (ring)
10  almari me aag                               ->  copy + acknowledge (quorum)
11  C se padha, nahi mila                       ->  do se padho (R=2)
12  viral token, 1 crore log ek hi register pe  ->  register ki photocopy, sab me baant do (Redis replica)
13  counter wala har baar uth ke register jaata ->  apne paas parchi rakh le (local cache)
14  dhokhe wali site ka token                   ->  radd karo, aage se pehle hi jaancho (kaali list)
15  radd kiya, log phone wala pata khol rahe    ->  phone me likhne hi mat do (302)
16  machine se hazaar nakli chit                ->  gate pe bhaga do, had per user (WAF, rate limit)
```

## 2. Rate limiter   ([notes](SYSTEM_DESIGNS/02_rate_limiter/RATE_LIMITER_HAND_NOTES.md))

```
 1  script har second 1000 OTP, bill chadha  ->  ek bande ka threshold (rate limit, 429)
 2  10 server, har ek ke paas alag ginti     ->  tu chhod, hisaab main rakhunga (central counter, Redis)
 3  khaata badla, 3 second me 10 OTP         ->  dabbe se token nikaalo, apni raftaar se (token bucket)
 4  20 server ne ek saath "tokens = 1" padha ->  sab ek hi baari me, koi ghus na paaye (Lua script)
 5  guard andar kamre me, 1000 ghus gaye     ->  guard bahar gate pe bitha do (API gateway)
 6  Redis gira, sabko jaane de ya roke       ->  papa ke dost andar, chor bahar (fail-open / fail-closed)
 7  script 429 khaati, phir bhi maarti rehti ->  dabbe me gala pakad ke bitha do (ban list)
 8  office ke 500 log ek WiFi, sab atke      ->  login se pehle dheeli had, baad me userId pe
 9  script roz hazaar baar reset maar rahi   ->  insaan hazaar baar nahi karta, pattern (WAF)
10  SMS company bhi giri                     ->  line lagwa, ek-ek karke (queue, Kafka)
11  VPN badal ke teen region, 15 OTP         ->  ek ghar region, ginti wahin (home region)
12  US ko kaise pata ghar Mumbai             ->  +91 dekh lo, IP badle number nahi (country code)
```

## 3. Twitter feed   ([notes](SYSTEM_DESIGNS/03_twitter_feed/TWITTER_HAND_NOTES.md))

```
 1  500 logon ke tweet milao, feed 6-7 sec   ->  apne paas rakhunga, apne paas se bhejunga (fanout on write)
 2  Kohli ke 27 crore dabbe, ghanton lage    ->  ek chhoti parchi, sab yahin se padhein (fanout on read)
 3  doston ka dabba + 15 celeb parchiyan     ->  ek parchi pe, DSA jaisa sort (merge at read)
 4  8,000 dabbe, Post ka button ghoome       ->  stadium wala bhai, "post ho gaya, nikal" (Kafka)
 5  3,000 pe server gira, 5,000 ko nahi mila ->  parchi pe likha, Kafka hi pakki parchi (acks=all)
 6  six pe 5 crore, parchi wali machine hili ->  10-15 copy bana do, baant do (hot key replica)
 7  dabbe me 3 saal ke tweet, memory ka bill ->  parchi chhoti, bas last 500 (inbox had)
 8  30 crore mahino se app nahi khol rahe    ->  30 din nahi aaya to kuch taiyaar nahi (on-demand inbox)
 9  din me 50 crore tweet, likhai ki line    ->  bada bhai, "tu dekh le" (Cassandra)
10  kaun kisko follow karta, iska track      ->  ek aur bada bhai (Neo4j)
11  "Arpan ke naye 500" poori table me       ->  naam se nahi, id se baanto (partition key = user id)
12  Ravi Delhi me, server America me         ->  Lajpat Nagar me branch khol do (India region)
13  John New York me, Arpan ka tweet nahi    ->  SBI ki do branch aapas me baat karein (cross-region)
14  50,000 log 1 GB video, server bhara      ->  alag route pe ja, URL se dekhenge (S3, presigned URL)
15  John 4K TV pe, Ravi gaon me phone pe     ->  Ferrari highway, Alto gaon (transcoding, ABR)
16  ek machine 4 size banaye, 20 minute      ->  kai machine, video id ki parchi pe baanto (workers)
17  worker 7 gira, "processing" ghanton se   ->  "tu hai kya?", jawab nahi to naya banda (heartbeat)
18  2:00 pe khiskaya, phone 0:00 se kheenche ->  copy dekh ke seedha 51wa page (manifest, seek)
19  1 crore log, har tukda hamari API se?    ->  bas file hai, CDN pe rakh do (static)
20  1 crore bachche yaad rakhein kaunsa page ->  bookmark, 51 khud palat lo (player client side)
21  tukde US ke S3 me, samundar paar         ->  copy paas me rakho, paas se khule (CDN edge)
22  private video ka link WhatsApp group me  ->  taala-chaabi mere paas, 10 hi (allow list)
23  CDN ko allow list pata hi nahi           ->  CDN ke paas bhi taala (signed URL / cookie)
```

## 4. Notification system   ([notes](SYSTEM_DESIGNS/04_notification_system/NOTIFICATION_HAND_NOTES.md))

```
 1  order khud SMS call kare, button 8 sec   ->  beech me dabba, aaram se bhejunga (queue, Kafka)
 1b DB me order, Kafka se pehle gira        ->  order ke saath vasiyat likh do (outbox + relay)
 2  ek hi line, bill aadhe ghante baad       ->  ladies, gents, bachchon ki alag line (topic, group)
 3  commit se pehle crash, SMS do baar       ->  parchi zindabaad, ho gaya likh lo (idempotency)
 4  company bole "abhi bheed hai"            ->  papa busy, thodi der ruk ke (retry queue)
 5  SMS company poori band                   ->  teen-chaar company extra (failover)
 5b company 30 sec chup (timeout)          ->  OTP turant backup se, 2 OTP chalega, ek hi code; sale ruk sakti (SLA)
 6  OTP bhi baaki ke peeche line me          ->  VIP darshan, OTP ki priority (alag OTP topic)
 7  fail pe fail, company band kaise samjhun ->  2-3 baar fail to traffic backup se (circuit OPEN)
 8  main company wapas theek hui?            ->  beech-beech me "tu theek hai?" (HALF-OPEN)
 9  backup bhi same fail bole                ->  number galat, alag daal do (DLQ)
10  20 worker 300/sec, company ki had 100    ->  saanjha dabba, sab usi se guzrein (token bucket, Redis)
11  company ne "OK" bola, SMS pahuncha nahi  ->  "le liya", baad me sent ya fail bataye (webhook)
12  1 crore offer, 40 lakh pe crash          ->  50 worker, alag-alag range (chunks)
13  dobara chala to loop shuru se            ->  parchi pe likho "ye ghar ho gaya" (checkpoint DB)
14  har SMS se pehle saancha DB se           ->  ek hi saancha apne paas, bas naam badlo (template cache)
15  OTP worker bhi pasand padhne DB pe       ->  OTP seedha bhejo, DB se lena-dena nahi
```

## 5. Stock broker trading   ([notes](SYSTEM_DESIGNS/05_stock_broker_trading/STOCK_HAND_NOTES.md))

```
 1  Arpan Ravi ko jaanta nahi, kaun milaye   ->  beech me ek aadmi, bhaav milao (matching engine)
 2  net gaya, app ne dobara bheja, 20 share  ->  chaabi ka taala khareed liya (idempotency key)
 3  35,000 se do order, chahiye 70,000       ->  EMI jaisa paisa hold (funds block)
 4  4 thread, 10 share 20 me bike            ->  copy apne paas kheencho, ek jagah (1 thread per symbol)
 5  book RAM me, machine mar gayi            ->  permanent dabbe me rakho (DB)
 6  permanent dabba bhi gira to              ->  3-4 copy bana ke rakho (replica)
 7  adhoora order RAM ke saath uda           ->  bag ki parchi dekh ke dobara jamao (order log, replay)
 8  sauda pakka, engine khud sabko call kare ->  10 ladkon ki fauj (Kafka, consumer group)
 9  portfolio DB band, sauda ulta kar do?    ->  rollback nahi, consumer court (settlement PENDING)
10  paisa kata, share dikh nahi rahe         ->  parchi apne paas, DB aaye to de do (Kafka retry)
11  bhaav har second, 10 lakh screen         ->  chat wala hi (WebSocket + Redis pub/sub)
12  9:15 lakhon order, book DB me, line atki ->  chhoti parchi pe dhadadhad, baad me mota register (book RAM, batch, copy)
13  parchi pe match, DB me abhi nahi         ->  "processing" turant, mote register ke baad pakka (DB = sach)
```

## 6. Payment system   ([notes](SYSTEM_DESIGNS/06_payment_system/PAYMENT_HAND_NOTES.md))

```
 1  HDFC hamari sunta hi nahi                ->  centralized authority, uski API (PSP)
 2  PSP 30 sec chup, FAILED ya SUCCESS?      ->  apne paas parchi, INITIATED (payment status)
 3  PSP batana bhool gaya, message khoya     ->  beech me banda jo poochta rahe (status job)
 4  PSP-1 poora band, payment ruke           ->  PSP-2 pe bhej do (fallback PSP)
 5  INITIATED likha, call bhejte server gira ->  pehle register me likho, phir bhejo (commit first)
 6  dobara bhejna pada to do baar kate?      ->  idempotency key laga do (payment id)
 7  wallet se kata, ICICI ne FAILED bola     ->  sab ulta kar do, paise wapas (saga)
 8  2 minute 500 na yahan na wahan           ->  ledger me likh lo, paise yahan hain (hold)
 9  "sab taiyaar ho?" poochh ke kaato        ->  slow dost ki wajah se mera paper chhoota (2PC nahi)
10  Diwali 3,000/sec, ek register            ->  register ke chhote tukde (shard)
11  wallet tukda 1 me, dukaan tukda 3 me     ->  ladka diary le ke jaaye (saga do shard ke beech)
12  PSP-1 chup, mara ya dheema pata nahi    ->  processing dikhao, haan ya naa ka waqt do; pata ho gira tabhi PSP-2 (PENDING, timeout, refund)
```

## 7. File upload + validate   ([notes](SYSTEM_DESIGNS/07_file_upload_validate_system/FILE_UPLOAD_HAND_NOTES.md))

```
 1  server khud sab kare, 10 min spinner   ->  chai pi aao, peeche line (Kafka + worker)
 2  PDF bol ke exe chadha de               ->  chhota tukda dekh lo (magic bytes)
 3  1000 x 2 GB hamare server se           ->  seedha bade dabbe me (S3)
 4  8 GB pe net gaya                       ->  tukdon me todo, sirf baaki 2 GB (multipart)
 5  S3 kaise maane ye hamara user          ->  receptionist ka token (presigned URL)
 6  file S3 me aayi, server ko pata nahi   ->  S3 bataye "aa gayi" (event notification)
 6b UPLOADED likha, scan daalne se pehle gira ->  UPLOADED ke saath vasiyat (outbox + relay)
 6c S3 ne "aa gayi" do baar / ulta bataya ->  parchi, ho gaya to chhod; purana chhod (idempotency, sequencer)
 7  folder rename = 500 copy + 500 delete  ->  ped jaisa, sirf parent badlo (parent_id)
 8  1043 likha, Priya ka statement khula   ->  "Priya ko saath la" (owner / share check)
 9  download bhi hamare server se          ->  truck seedha ghar (S3 direct download)
10  link 200 logon ke WhatsApp group me    ->  link ki umar chhoti (presigned expiry)
11  scan baaki kaam ke peeche atka         ->  doctor line me nahi lagta (alag scan topic)
12  scan me 10 min baaki, download chahiye ->  scan poora tak link hi nahi (CLEAN tak band)
13  scan me virus nikla                    ->  file lo hi mat (quarantine / delete)
```

## 8. News aggregator   ([notes](SYSTEM_DESIGNS/08_news_aggregator/NEWS_HAND_NOTES.md))

```
 1  5000 site, khabar aayegi kaise              ->  chhotu, sab utha ke dabbe me (fetcher, DB)
 2  chhotu akela, ek chakkar 3 ghante           ->  bahut saare chhotu (kai fetcher)
 3  khabar pehle se taiyaar kahan               ->  chhoti parchi, akhbaar jaisi (Redis feed)
 4  ek khabar 40 baar                           ->  bada aadmi, "ye dobara hai, hatao" (dedup)
 5  shabd alag, khabar same                     ->  wordings dekh lo (near-duplicate check)
 6  bada aadmi akela, 1 min ek khabar           ->  stadium wali kahani, chhotu daal ke chale (Kafka)
 7  Ravi cricket, Priya tech, ek parchi         ->  category ke hisaab se baanto
 8  Priya pehli baar aayi                       ->  pehli baar DB, phir parchi (cache-aside)
 9  9:05 ki iPhone khabar purani parchi me nahi ->  aate hi tech walon ki feed me (push on write)
10  50 lakh alag parchi, 10 crore likhai        ->  saari tech khabar ek parchi (category feed)
11  iPhone launch, 10 lakh/sec, dabba 1 lakh    ->  tech ke dabbe badha do (aur Redis node + tech ki copy)
12  cricket wale dabbe khaali pade              ->  unme bhi tech ki copy (hot key copies)
13  A me khabar, B se refresh pe gayab          ->  do me likho, do se padho (quorum)
14  DB me likhi, parchi se pehle worker mara    ->  status rakh, agla aage badhaaye (state, outbox)
```

## 9. BookMyShow   ([notes](SYSTEM_DESIGNS/09_bookmyshow/BMS_HAND_NOTES.md))

```
 1  dono ko D5 "mil gayi"                      ->  seat pe parchi, dekhna + rokna ek saath (hold)
 2  parchi laga ke chai peene, kabhi nahi aaya ->  5 minute dekhunga, phir phaad do (hold_until)
 3  parchi phaadne wala chhutti pe             ->  chhota bhai / replacement (backup safai job)
 4  20 minute se purani parchi lagi            ->  parchi pe hi umar, apne aap gayab (expiry query me)
 5  sirf dekhne wale bhi bojh daal rahe        ->  dekhne wale ki alag diary (read replica)
 6  hall 1000 ka, saamne 5000 khade            ->  capacity jitne andar, baaki line (virtual queue, token)
 7  token khatam, parchi ke 3 min baaki        ->  token gate ka, seat ki ghadi parchi ki (do ghadiyan)
 8  Pay daba diya, PSP dheema, umar khatam     ->  parchi pe "pay chal raha", umar badhao (PAYMENT_PENDING)
 9  chhotu (PSP) 1-2 baar jagaya, phir bhi chup ->  FAILED dikha do, seat chhodo, "paisa kata ho to wapas"; naye log doosre PSP pe (breaker), baad me kata nikla = refund
10  baad me chhotu bole "fail ho gaya"         ->  user ko kuch nahi, khud dobara / doosra chhotu (retry, PSP-2)
```

## 10. Distributed cache   ([notes](SYSTEM_DESIGNS/10_distributed_cache/CACHE_HAND_NOTES.md))

```
 1  DB 50 ms, Ravi ka profile hazaron baar    ->  DB se pehle ek dabba (cache, Redis)
 2  dabba bhar gaya, purane profile pade      ->  jo purana, kam use, nikaal do (LRU)
 3  1 TB ek machine me nahi aata              ->  tod do, hash se same dabba (sharding)
 4  11vi machine judi, har key ki jagah badli ->  kho-kho, sabse paas wala (consistent hashing)
 5  naam badla, cache me purana "Ravi"        ->  pehle parchi kaato, phir DB (cache DELETE)
 6  beech me purana naam phir parchi pe       ->  banda check kare, DB sach hai (sync job)
 7  kuch second purana Ravi dikha             ->  paisa thodi hai (eventual consistency)
 8  Priya viral, 10,000 miss, sab DB pe       ->  ek chhotu bhejo, baaki yahin (single flight)
 9  chhotu raaste me gira                     ->  kuch second dekho, phir bheed me se ek (lock timeout)
10  1 crore log sirf machine 4 pe             ->  machine 4 ka replica, ek aur gaadi (hot key copies)
11  sync job ko kaunsi key dekhni             ->  pichhle ghante me jo update hui (updated_at)
```

## 11. Google Docs collab   ([notes](SYSTEM_DESIGNS/11_google_docs_collab/DOCS_HAND_NOTES.md))

```
 1  Save pe poora text, Arpan ki line gayab  ->  poora text nahi, operation bhejo (insert / delete)
 2  laakhon save, har badlaav DB pe          ->  chhota register, kuch der baad DB (Redis buffer, batch)
 3  "Hum" aur "kal", kal galat jagah         ->  shabdon ko khiska do (OT)
 4  do server, ek doc ki do shakal           ->  ek centralized system, ek version
 5  lakhon doc, sab ek hi jagah?             ->  har doc ka alag, uska maalik (owner server)
 6  "Saved" dikhaya, DB se pehle server gira ->  dabbe ki ek aur copy (Redis copy)
 7  owner server hi gira                     ->  "mujhe kuch ho to tu hai" (naya owner wahin se)
 8  Ravi hataya, server pe purani permission ->  chhota note, uska op accept nahi (permission check)
 9  Ravi doosre server se likh raha          ->  building ke bahar chilla de (pub/sub invalidation)
10  server 7 ne awaaz suni hi nahi           ->  copy note kar le, wapas aake dekh (standby, reconnect)
```

## 12. Message queue (Kafka)   ([notes](SYSTEM_DESIGNS/12_message_queue_kafka/KAFKA_HAND_NOTES.md))

```
 1  stadium ke bahar bheed, sab ghus rahe         ->  admin: line banao, ek-ek karke (Kafka)
 2  line me ek aadmi nikla, sab atke              ->  seat booked, wahi baithega (offset)
 3  ek admin se nahi sambhal rahi                 ->  aur gate, har gate ki apni line (partition)
 4  har ticket 3 baar punch                       ->  ek kamre me bitha do, 3 me se ek (consumer group)
 5  security wale bhi usi kamre me                ->  alag room (alag consumer group)
 6  admin ne "aa gaya" bola, phir gira            ->  vasiyat ki copy bhi likhe, tab "likh di" (acks=all)
 7  checker punch ke baad, register se pehle gira ->  pehle aaya hai? check kar le (idempotency)
 8  Order Service gate tak bhejne se pehle gira   ->  baap ki vasiyat, pehle likh de (outbox)
 9  galat ticket baar-baar, line atki             ->  line se hata (DLQ)
10  galat ticket gate tak aaye hi kyun            ->  bouncer, gate pe hi rok de (validation)
11  SMS counter 10 min band, sab DLQ me           ->  pehle check, waqai band? (circuit breaker)
12  counter waqai band hai                        ->  road band, traffic divert (fallback)
13  7 din peeche, purana panna phat jaata         ->  aur checker daalo, gate bhar gaye to gate bhi (consumer, phir partition)
14  aaj 1 din, kal 2 din peeche                   ->  alert laga do (consumer lag alert)
```

## 13. Banking system   ([notes](SYSTEM_DESIGNS/13_banking_system/BANKING_HAND_NOTES.md))

```
 1  500 kate, bijli gayi, Ravi ko nahi mila  ->  mere 500 wapas de do (rollback)
 2  receipt se pehle line kati, wahi parchi  ->  log hamare paas, dekh ke mana (pehla record)
 3  agle din sach me phir 500 bhejne         ->  key jo badle hi na, same parchi (idempotency key)
 4  600 me do counter pe 500-500             ->  dekhna aur kaatna ek kram me (row lock)
 5  "500 mile hi nahi", saabit kaise         ->  alag diary, kaato mat, neeche likho (ledger)
 6  khabar dene se pehle bijli gayi          ->  jo yaad nahi rehta, likh lo (outbox, relay)
 7  register jal gaya                        ->  laptop me save, ghar le jaao (replica, Multi-AZ)
 8  2 baje likha, 3 baje aag                 ->  turant laptop pe, tabhi pakka (sync copy)
 9  sab balance dekhne aaye, kaam atka       ->  photocopy alag desk pe (read replica)
10  photocopy peeche, ghabra ke dobara bheja ->  jisne likha, thodi der main file se (read-your-writes)
11  haath se 100 ko 600 kar diya             ->  ledger se milwao, wahi asli sach (reconciliation)
12  5 saal ki line roz jodna bhaari          ->  mahine ke mahine tod do (partition, snapshot)
13  almari me jagah nahi, 10 saal rakhni     ->  head office bhej do (S3 archive)
14  godown me -500 ko -50 kar diya           ->  kai copy chhupa ke, milao (copies)
15  ijazat wala file badal hi na paaye       ->  kohinoor, taala maar do (S3 Object Lock)
```

## 14. Chat messaging   ([notes](SYSTEM_DESIGNS/14_chat_messaging/CHAT_HAND_NOTES.md))

```
 1  phone har second "kuch aaya?"              ->  poochne ka mauka hi na do (WebSocket)
 2  Ravi ka phone band, 2 ghante               ->  uncle nahi mile, paise apne paas (message store)
 3  100 server, Ravi kis pe                    ->  DSA wala map, O(1) (session map, Redis)
 4  har server baaki 99 se taar jode?          ->  chhota ladka address le ke jaaye (Redis pub/sub)
 5  group ke 500, ek message 500 kaam          ->  ek dabbe me, log khud aake sunein (ek baar DB)
 6  lift me jawab kho gaya, "hi hi"            ->  ek key, same aaye to mana (clientMsgId)
 7  "kahan?" pehle dikha, ghadiyan aage-peeche ->  server lagayega number (sequence)
 8  "12 naye message" kaise dikhe              ->  ek ginti, itne padh liye (last_read_seq)
 9  kisne padha, 4.9 lakh entry                ->  978 matlab 975 padha, number ghata lo (watermark)
10  200 MB video, taar jam                     ->  alag raasta, chhota URL (S3, link)
11  phone band, 2 ghante se "online"           ->  haath maaro, 2-3 baar chup to kaato (heartbeat, TTL)
12  S1 dheema, "nahi, parso" pehle pahuncha    ->  server ka number, phone us kram me lagaye (seq, client sort)
```

## 15. Uber   ([notes](SYSTEM_DESIGNS/15_uber/UBER_HAND_NOTES.md))

```
 1  server ko pata nahi driver kahan        ->  phone khud bheje, ek dabba (Location Service)
 2  1.25 lakh update/sec, DB ghuta          ->  bada bhai bulao (Cassandra)
 3  5 lakh driver ka fasla naap raha        ->  naksha dabbon me, sirf apna dabba (geohash)
 4  dabbe ke kinare, bagal me driver        ->  8 km ka gola, chhoote dabbe bhi (neighbour geohash)
 5  Raju pe ek second me do ride            ->  BMS wali parchi, ek kadam me (hold / lock)
 6  Raju 15 sec me accept na kare           ->  parchi ki umar, agla driver (TTL)
 7  concert khatam, 50 hazaar, cab 500      ->  500 andar, baaki line (virtual queue)
 8  baaki ke liye cab hi nahi               ->  kiraya badha ke door ki cab (surge pricing)
 9  14.9 pe accept, server pe 16 pe         ->  ghadi server ki, parchi phaad do (server TTL)
10  Raju Janpath me, purana naam pada       ->  sach wahi jo GPS ne abhi bheja (latest location)
11  register driver ke naam se, 5 lakh line ->  jagah ke hisaab se panna (geohash ka Redis set)
12  dabba badla, purana naam pada           ->  nikla to hata do, naye me daal do (remove + add)
13  10 lakh phone "Raju kahan?"             ->  poochne ka mauka mat do (push, WebSocket)
14  Arpan server 12 pe, Raju 57 pe          ->  ghar ka raasta pata, seedha usi raaste (pub/sub)
15  250 dikha, aakhir me 500 kate           ->  jo dikhaya wahi lo (upfront fare)
16  wallet me sirf 40                       ->  poora kaato, aadha nahi (atomic charge)
17  payment fail, Arpan ghar ke andar       ->  paisa nahi aaya to ride khatam nahi (PAYMENT_PENDING)
18  Raju ka phone band, naksha pe gayab     ->  location band, ek-do baar dekho (location timeout)
```

## 16. Leaderboard   ([notes](SYSTEM_DESIGNS/16_leaderboard/LEADERBOARD_HAND_NOTES.md))

```
 1  1 crore sort kiye bina top 10 + rank     ->  apne aap sort wala dabba (Redis sorted set)
 2  Ravi, Priya dono 9800                    ->  score phir time, ek number me (thenComparing)
 3  lakhon update ek Redis pe                ->  region se baant do, chhotu mila ke badi screen (shard, scatter-gather)
 4  India me 3,000, duniya ka rank           ->  race wala: har line me mere upar kitne, jodo (count)
 5  poora 1 crore ka board                   ->  referee wala: chhotu har kuch minute (snapshot job)
 6  aaj / hafta / all-time                   ->  score teeno dabbon me
 7  raat 12 baje aaj wala khaali             ->  alag register, kal wala discard (key me tareekh, TTL)
 8  mere aage-peeche ke 5, alag dabbon me    ->  bheed me aas-paas scan (score range, merge)
 9  Redis gira                              ->  clone, phir clone, teeno gaye to DB se bharo (replica, DB sach)
```

## 17. E-commerce   ([notes](SYSTEM_DESIGNS/17_ecommerce/ECOMMERCE_HAND_NOTES.md))

```
 0  sale ka maal seemit                     ->  FR me hi cut off: 500 ko hi, baaki out of stock (consistency > availability)
 1  aakhri ghee, aunty + Ravi ek saath       ->  online ko godown, counter ko shelf (channel ka alag stock)
 2  ladka uthake likhna bhool gaya           ->  uthana + parchi ek hi kaam (atomic transaction)
 3  sale ki bheed, aata-daal bhi atka        ->  sale ki alag dukaan (alag service)
 4  ek parchi pe 50 hazaar                   ->  10 counter, tez kaatne wala (hot row todo, Redis DECR)
 5  counter 3 khatam, counter 7 pe 30        ->  khaali counter band, sold out tab jab sab 0
 6  60 ne pay nahi kiya                      ->  cart = list, ginti pay pe, fail to wapas
 7  3000 ek second me Pay                    ->  server pe line, 500 ke baad rollback (ya ghadi wala hold, dono ki keemat)
 7b rollback + ghadi dono me pech           ->  Apple store: sale gate, stock jitne hi Pay tak, fail = agla batch
 8  net gaya / PSP chup                      ->  BMS wala PAYMENT_PENDING + status job, kinaare pe refund
```

## 18. Netflix   ([notes](SYSTEM_DESIGNS/18_netflix/NETFLIX_HAND_NOTES.md))

```
 1  har tukda samundar paar se              ->  file apne shehar me, CA wala dost (Open Connect, ISP ke andar)
 1b AWS ka CDN kyun nahi                     ->  courier har litre ka paisa, apna fridge muft me (per-GB bill nahi)
 2  12:29 tak dabba khaali                  ->  pen drive pehle se, video badalta nahi (proactive fill)
 3  Ravi 3G pe, purana TV, Hindi awaaz      ->  Ferrari / Alto + har film ki apni seedhi + kai roop (ABR, encode ladder, codec)
 4  dabbe me poora maal nahi                ->  garam paas, thanda bade dabbe se (popularity, tier, evict)
 5  20 dabbe, kis se maange                 ->  hospital ka guard, server batata (steering; AWS pe, video bytes dabbe se)
 6  TV pe 27 min, phone pe wahi se          ->  account pe parchi (watch position DB me)
 6b har kuch second update, DB ki saans     ->  race me ruk ke likho (pehle cache, DB kabhi-kabhi)
 7  "tere liye" me 6 second                 ->  parlour wala GTA pehle se (raat ka batch, cache se uthao)
 7b 11 baje Dhurandhar dekhi                ->  turant parchi, dikhate waqt milao (abhi ka fast store + bani list)
 7c Priya naya user                         ->  India ka garam maal (cold start)
```
