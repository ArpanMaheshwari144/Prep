# HLD dikkat chain

Dikkat ke parivaar: upar dikkat, phir humne kya socha, phir wahi dikkat kis-kis design me aayi. Ek parivaar samjha to saare
design me wahi kaam aata. Design ka naam + kadam number har line me, bhool jaao to neeche notes ke link.

Do hisse: **0** = har design me kadam upar se neeche, tune kya bola aur uska asli naam (revision isi se). **1-21** = dikkat ke parivaar: ek dikkat kis-kis design me aayi.

## 0. Tere shabd -> asli naam (design ke kram se)

Har design me kadam upar se neeche, jaise round me chale. Har kadam: pehle dikkat, phir neeche "tune bola ...  -> asli naam".
Sirf hand notes ki Technical lines se, apni taraf se kuch nahi.

### URL shortener

```
FR / NFR / dhaancha
       tune bola "do logon ko same token na mile"         -> uniqueness (counter / range)
       tune bola "lambe ka chhota hamesha theek"          -> consistency (quorum read / write)
       tune bola "garam URL sambhale"                     -> hot key (cache, copies)
       tune bola "sab mote register pe na jaaye"          -> cache hit ratio
       tune bola "machine se hazaar chit nahi"            -> rate limit
       tune bola "jaldi"                                  -> low latency
       tune bola "copies"                                 -> replication / availability.

1    Counter wala token apne man se bana raha, kuch bhi chhota likh deta. Ek din do alag logon ko ...
       tune bola "ek line me, aage badhao"                -> counter (har link ko agla number)
       tune bola "kabhi repeat nahi"                      -> takrav nahi
       tune bola "random / hash me same code"             -> collision, har baar DB check; counter me check ki zaroorat nahi.

  -> maan lo: Arpan ne Delhi me YouTube ka ek gaana chhota kiya, Mumbai me kisi aur ne wahi link ...
       = same lamba link = har baar naya code (lambe link pe "pehle se hai?" ki jaanch nahi), ek lambe ke kai chhote chalte hain.

  -> maan lo: do log bilkul ek hi pal me aayein
       tune bola "insert if not exist"                    -> unique index / constraint on code (counter ke upar aakhri pehra).

2    Saal bhar me counter 10 arab pe. Token "10000000000", 11 akshar, aur lamba hota jaayega. ...
       tune bola "number plate jaise akshar bhi, combination" -> base62 (0-9, a-z, A-Z)
       tune bola "7 character, kaafi combination"         -> 62^7, ~3.5 lakh crore code.

3    Token lene kam aate, token dikha ke pata lene wale bahut: har second 1 lakh. Counter wala har ...
       tune bola "chhota pehla register"                  -> cache (Redis)
       tune bola "mota register"                          -> DB
       tune bola "bahut baar poochhe jaane wale"          -> garam / baar-baar padhe link.

  -> maan lo: pooche server ki memory me hi kyun nahi rakh lete, baar-baar DB kyun
       tune bola "restart pe gaayab"                      -> server ki memory volatile, link ko pakki jagah (DB) chahiye; tez padhne ke liye alag chhota register (Redis).

  -> maan lo: chhota register (Redis) gir gaya
       = Redis replica; dono gire to fail open (cache chhod ke seedha DB se), Redis wapas aate hi phir cache se.

4    Chhota register bhar gaya. Naya token likhna hai, kisi purane ko hatana padega. Kise?
       tune bola "itni der tak rahega, phir hata do"      -> TTL
       tune bola "jo poochha hi nahi ja raha, hatao"      -> LRU.

5    Ek mashhoor token ka waqt theek 7 baje khatam, chhote register se hat gaya. Usi pal 1,000 log ...
       tune bola "ek ladka jaaye, sab yahin ruken"        -> lock (single flight)
       tune bola "la ke chhote register me rakh de"       -> DB se laake Redis bharna.

6    Ladka bhaaga aur raste me gir gaya, kabhi wapas nahi aayega. Baaki 999 "ladka laayega" soch ke ...
       tune bola "ladke ki timeline, nahi aaya to agla"   -> lock TTL (umar khatam to lock khulta)
       tune bola "purani information do, baad me update"  -> soft / hard TTL.

7    Ek counter se bheed nahi sambhli, 5 counter khol diye. Har counter apni ginti 1, 2, 3 chala ...
       tune bola "apni-apni range"                        -> counter range (beech ka counter har app server ko number ka dabba deta).

  -> maan lo: ek counter apni range ke beech, 400 pe, crash ho gaya
       = range ke bache number kho jaate (gap), koi dikkat nahi; counter number wapas nahi maangta.

8    5 saal me mote register me itne token (90 TB) ki wo ek almari me aata hi nahi.
       tune bola "chhote tukde, alag almari"              -> shard
       tune bola "hash se baant do, same aaye to usi tukde pe" -> code ke hash se machine tay.
       = key-value + bahut likhai + saalon ka data = Cassandra (wide-column, apne aap replica + scale). Cassandra kyun: data key-value, likhai bahut, aur data me bank jaisi consistency (paisa nikaalna, transaction) chahiye hi nahi.

  -> maan lo: almari phir kam padi, 10vi lagani hai
       = Cassandra andar token ring (consistent hashing) se baantta, official doc: https://cassandra.apache.org/doc/latest/cassandra/architecture/dynamo.html
       tune bola "mod se har cheez badal jaayegi"         -> hash % N me N badla to lagbhag saare token nayi almari pe
       tune bola "gol chakkar, sabse paas wali"           -> consistent hashing (ring pe agli machine).

  -> isi pe ek aur raasta: hash ki jagah state ke hisaab se kaato).
       tune bola "region ke hisaab se kaato"              -> sharding by region (geo-partition)
       tune bola "register pe thappa"                     -> shard ka record (kaunsa data kahan).

  -> maan lo: Ravi ne Delhi me link banaya, Delhi ke register me gaya. Mumbai wala dost kholta ...
       tune bola "token me state ka code"                 -> shard key ID ke andar (shard id prefix)
       tune bola "bada bhai list dekh ke bheje"           -> router / lookup (shard map).

9    Almari number 3 me aag lagi, uske saare token jal gaye.
       tune bola "almari ki copy"                         -> 3 copy (replica)
       tune bola "acknowledge aaye tab pakka"             -> quorum write (W=2).

  -> aur neeche: poori building / shehar me aag to
       tune bola "copy alag-alag jagah"                   -> replica (alag zone)
       tune bola "central authority pehle likhe phir bheje" -> coordinator (jo request leta, ring me aas-paas ki copies ko aage bhejta)
       tune bola "copy peeche chale, baad me update"      -> async replication lag (save fail to user ko "dobara banao", number bekaar gaya to naya number, arabon hain)
       tune bola "authority ki bhi copy"                  -> wo gire to koi aur coordinator ban jaaye. Cassandra andar se kaise karta, wo uska kaam, design ka hissa nahi.

10   Teen copy A, B, C. Arpan ne naya token banaya, A aur B ne haan bola, C peeche. Turant dost ne ...
       tune bola "do se hi padhwao"                       -> R=2
       tune bola "ek ke paas to hai"                      -> W + R > 3, ek copy common.

11   Cricket match ka token viral, 1 crore log ek saath. Sab counter wale us token ke liye ek hi ...
       tune bola "chhote register ki photocopy, divide"   -> Redis replica (padhai copies me bantna).

  -> maan lo: copy 10 ho ya 100, har baar counter wale ko kursi se uth ke register tak jaana ...
       tune bola "counter wala apne paas parchi rakhe"    -> local cache (app server ki memory me garam link kuch second).

  -> maan lo: wahi cricket wala token roz ka garam hai, 10 photocopy pe pelam pel log. Theek 7 ...
       (jawab me
       tune bola "counter 7"                              -> photocopy 7
       tune bola "10 counter pe baant do"                 -> 10 photocopy
       tune bola "chhotu"                                 -> kadam 5 wala ladka.)
       tune bola "umar badhate raho"                      -> hot key TTL extend (refresh-ahead)
       tune bola "photocopy ki umar alag"                 -> TTL jitter
       tune bola "chupchaap doosri parchi"                -> stale copy (soft / hard TTL, stale-while-revalidate; kadam 6 wali purani information yahi)
       tune bola "counter ki parchi chhoti umar"          -> local cache short TTL
       tune bola "10 na aayein tab backoff"               -> backoff: ladka bheja, nahi aaya, thoda ruko, doosra bheja, nahi aaya, ab thoda zyada ruko, teesra bheja; har baar intezaar badhta (1, 2, 4 sec), mote register pe bombard nahi. 10 me se ek bhi na aaye tab maano mota register (DB) gira. Jitter iska hissa nahi: jitter upar wala "photocopy ki umar alag" hai, photocopy ki umar me thoda +1, +2 jod do taaki sab ek saath na mitein.

  -> maan lo: 50 counter hain, log kabhi kisi counter pe, kabhi kisi pe (LB). Photocopy 7 pe token ...
       (yahan
       tune bola "counter"                                -> counter wala, yaani hamara server
       tune bola "chhotu"                                 -> us counter ka ladka.)
       tune bola "sabke saamne ek banda, sirf ek ladka"   -> distributed lock (saajha lock, Redis me ek key: SET NX + umar). Kadam 5 ka ladka sirf ek counter ka ho to 50 counter ke 50 ladke jaate, isliye lock saajha.
       tune bola "ek chhotu laake sabke kaan me"          -> request coalescing (ek fetch, baaki ko wahi jawab), saajha lock ke saath.

  -> isi pe ek aur raasta: Redis tak aane hi mat do
       tune bola "server ke paas"                         -> local cache
       tune bola "us link ka alag dabba"                  -> hot key isolation (alag Redis / alag pool, baaki link na atkein)
       tune bola "1-2 copy"                               -> hot key replicas
       tune bola "copiyan ring me, gira to agli copy"     -> replicas ring pe (failover agli copy pe), consistent hashing nahi.

12   Kisi ka idea: har aadmi pata apne phone me hi likh le, counter pe aaye hi nahi. Hafte baad ...
       tune bola "token radd"                             -> code band
       tune bola "dene se pehle khud check"               -> kaali list (Google Safe Browsing).

  -> isi pe ek aur raasta: radd karne ki jagah banate waqt hi jaanch
       tune bola "banate waqt chupke se dekho"            -> create time pe URL safety check (reputation / kaali list)
       tune bola "gandi to mat do"                        -> reject.

  -> maan lo: token radd kiya, par log counter pe aa hi nahi rahe. Unhe kaise pata chale? Shuru me ...
       tune bola "phone me likhna hi mat do, har baar counter pe aaye" -> 302 + `Cache-Control: no-store` (save hi mat karna; 301 nahi)
       tune bola "phone me likh le"                       -> 301 (browser cache)
       tune bola "phone cache pe TTL"                     -> browser cache ki umar.

  -> maan lo: dashboard banana hai, har click ginna hai, par user click karke khada na rahe
       tune bola "302 se har click hamare paas"           -> click tracking / analytics
       tune bola "peeche queue, worker"                   -> async click events (Kafka -> consumer -> dashboard), redirect dheema nahi hota
       tune bola "fail to alag dabba"                     -> DLQ.

13   Ek aadmi ne machine laga di, har second hazaar chit, sab nakli ya dhokhe wale pate. Counter ka ...
       tune bola "pattern pata lag jaayega, gate pe bhaga do" -> WAF block.

  -> maan lo: pakadne me 3-4 chakkar lagte, tab tak wo laakhon chit de chuka. Shuru se har aadmi ...
       tune bola "per user had, group pe nahi"            -> rate limit user pe (IP pe nahi)
       tune bola "office ka WiFi"                         -> ek IP ke peeche kai asli user
       tune bola "5 se zyada nahi"                        -> had paar pe 429.

  -> maan lo: bina login ke bhi link banta, script har second hazaar link bana rahi, userId hai hi ...
       tune bola "login laga do"                          -> auth zaroori (tab userId pe had + tracking)
       tune bola "bina login IP pe dheeli had"            -> anonymous rate limit per IP
       tune bola "IP ban, desh block"                     -> WAF block list / geo-block.

  -> isi pe ek aur raasta: bodyguard
       tune bola "bodyguard"                              -> API gateway / WAF pe rate limit + anomaly check
       tune bola "jail"                                   -> IP / user ban.

```

### Rate limiter

```
FR / NFR / dhaancha
       tune bola "had paar to 429, baad me try"           -> 429 Too Many Requests + Retry-After
       tune bola "alag server pe gayi to bhi ek ginti"    -> central counter (Redis)
       tune bola "consistent"                             -> atomic ginti (Lua)
       tune bola "wahi gira to poora down nahi"           -> availability (fail-open + local fallback)
       tune bola "office ke IP se 500"                    -> per-user had (IP nahi)
       tune bola "VPN se region badle to ghar region"     -> home region.

1    Server dab gaya, har request pe SMS ja raha, SMS company ka bill chadh raha.
       tune bola "ek bande ke liye threshold"             -> rate limit per user
       tune bola "limit lagao"                            -> had paar to mana (429).

2    Ginti server apni memory me rakh raha. App 10 server pe, load balancer Ravi ki requests baant ...
       tune bola "tu chhod, mujhe bata, hisaab main rakhunga" -> central counter (Redis)
       tune bola "1 + 2 + 3 + 4"                          -> har request pe ek hi jagah ginti +1.

3    Ginti ka khaata ghadi se: 10:00-10:10 ek khaata, 10:10-10:20 agla, har khaate me 5. Ravi ...
       tune bola "dabbe se token nikaalo, aage badho"     -> token bucket
       tune bola "apni raftaar se badhega"                -> refill rate (OTP pe: max 5, har 2 minute me 1, aam raftaar 10 min me 5, khaali baitha user ek saath 5 le sakta)
       tune bola "user aaye nahi to bahut token"          -> burst allow.

  -> maan lo: pooche aur tareeke kaunse
       = fixed window (kinare pe dugni, upar wali dikkat), sliding window (ghadi khisakti), leaky bucket (ek raftaar se nikalta, burst nahi), token bucket (burst allow, simple), isliye token bucket chuna.

4    Ravi ke dabbe me 1 token. Script ne ek pal me 20 request maari, 10 server pe. Har server ne ...
       tune bola "Redis single thread"                    -> Redis ek command ek waqt (par alag-alag GET aur DECR ke beech doosra server ghus sakta, isliye single thread akela kaafi nahi)
       tune bola "sab ek hi baari me, andar koi ghus na paaye" -> Lua script (check + nikaalna atomic)
       tune bola "banking me update aur check ek me"      -> wahi atomic check-and-update.

  -> maan lo: Lua script hi dheemi ho gayi ya atak gayi
       = Lua chalte waqt Redis aur kuch nahi karta (single thread), isliye script chhoti, loop nahi; ek node ~lakh ops / sec, zyada to Redis cluster (shard).

  -> maan lo: counter wala tareeka (har request pe +1, pehli pe 60 sec ki umar). Ravi ki pehli ...
       tune bola "dono ek saath ya kuch nahi"             -> INCR + EXPIRE ek Lua script me (atomic, ya SET key 1 EX 60 NX + INCR); bina umar ka counter = key kabhi expire nahi, user hamesha block. (HANDS_ON 02 me chala ke dekha.)

5    Limiter OTP Service ke andar. Script ki 1000 request LB se, beech ke server se hote hue OTP ...
       tune bola "guard andar kamre me"                   -> limiter service ke andar
       tune bola "guard bahar gate pe"                    -> limiter API gateway pe.

6    Gate ka guard ginti Redis se dekhta, Redis gira. Ya sabko jaane de ya sabko rok de. OTP button ...
       tune bola "dono khule, bouncer 5-5 karke bheje"    -> Redis gira to fail-open, par har gateway apni memory me kachchi ginti se had lagaye (local fallback limiter, thoda dheela par pura khula nahi)
       tune bola "2-3 second me 5 OTP = shaq, sirf uska band" -> OTP pe band sirf us user ka jo pattern tode (per-user block), baaki sabke liye khula
       tune bola "suspicious kaun, limiter ka kaam nahi"  -> abuse / fraud detection alag hissa, limiter sirf had lagaye; Redis ki replica pehle.

7    Script gate pe rukti, 429 khaati, par ruki nahi. Din-raat har second maarti, gate ko roz ...
       tune bola "ek baar, do baar, teesri baar"          -> baar-baar had todne ka pattern
       tune bola "alag dabba, yahan se hil nahi sakta"    -> ban list
       tune bola "permanent band"                         -> ban wala gate pe bina gine lautta.

  -> maan lo: pattern pakadna kaise
       tune bola "Kafka se pattern"                       -> request log events -> Kafka -> consumer (abuse detection)
       tune bola "kaunse endpoint jhaanke"                -> endpoint scanning pakadna, phir WAF block list.
       tune bola "Ban kuch der ka"                        -> IP ban pe umar (TTL), kyunki IP badalta aur ek IP ke peeche asli log bhi; upar wala
       tune bola "permanent band"                         -> pakka pakda gaya user / account. Dono saath chalte.

8    OTP login se pehle, had phone number pe. Ravi ki script har baar naya number daalti, har ...
       tune bola "login ke baad userId hamare paas"       -> userId pe kadi had
       tune bola "login pe dheeli had"                    -> login se pehle IP pe dheeli had
       tune bola "insaan hazaar baar nahi, pattern notice" -> script pattern pakadna
       tune bola "IP ko WAF me daal do"                   -> WAF.

  -> maan lo: login ki had aur baaki ki had ek hi?
       = per-endpoint limit (login ki alag kadi had, baaki API ki alag), had paar = 429 + Retry-After.

  -> maan lo: had asli user pe hi bhaari padi
       tune bola "had asli user pe bhaari"                -> false positive (lockout)
       tune bola "passkeys / Authenticator"               -> MFA / passwordless (password guess ka raasta hi band), had + MFA saath.

9    Result ka din, subah 10 baje 1 crore bachche OTP maangte. Har bachcha apni had me, phir bhi ...
       tune bola "log badha do, server badha do"          -> apne server scale
       tune bola "line lagwa, ek-ek karke"                -> queue (Kafka)
       tune bola "beech me cheez jo spike rok le"         -> SMS company ki raftaar se worker / consumer, global had.

  -> maan lo: OTP line me atka, der se nikla, tab tak bekaar.
       = umar = message TTL / expiry (line se nikalte waqt jaancho), purana OTP SMS company tak jaata hi nahi.

  -> isi pe ek aur raasta: log badhane ki jagah upar ek had
       tune bola "global cap, upar 503"                   -> global / server-wide limit (503 Service Unavailable)
       tune bola "kam zaroori pehle hatao"                -> load shedding (priority: login / payment pehle, recommendation / analytics baad).

10   App 3 region me: Mumbai, US, Europe, har region ka apna Redis. Ravi VPN badal-badal ke teeno ...
       tune bola "ek particular region decide"            -> home region
       tune bola "count India ke Redis pe"                -> ginti ghar region ke Redis me.

  -> maan lo: OTP login se pehle, sirf phone number aata. Ravi VPN se US gateway pe. US ko kaise ...
       tune bola "+91 se pata"                            -> phone number ka country code se home region (koi alag register nahi chahiye)
       tune bola "IP badle, number nahi"                  -> VPN se bachav.

11   Ravi ka dabba Redis me, abhi khaali (saare token use kar chuka). Ye app sirf OTP login nahi, ...
       tune bola "feed alag Redis, Ravi ka alag"          -> sach (limiter ka dabba) aur cache alag Redis me
       tune bola "LRU hata do, Ravi ka mat hatao"         -> maxmemory-policy noeviction (Redis kuch nahi phenkta)
       tune bola "Diwali ke baad thanda, tab hatao"       -> cache pe TTL.

  -> maan lo: ek hi Redis, LRU hata diya (noeviction). Diwali ki raat page itne padhe gaye ki TTL ...
       tune bola "dekh liya to TTL kam kar do"            -> cache pe chhota TTL (bheed me jaldi expire)
       tune bola "product page DB se, thodi loading"      -> cache miss pe DB, thoda dheema chalega (graceful degradation)
       tune bola "Ravi ka dabba nahi phenk sakte"         -> sach wali key kabhi evict nahi
       tune bola "sabse baad me Redis tod do"             -> sach aur cache alag Redis.

```

### Twitter feed

```
FR / NFR / dhaancha
       tune bola "khaali page nahi, trending dikhao"      -> cold start / recommendation (follow ke alawa)
       tune bola "jo search kiya wahi aane laga"          -> interest se ranking
       tune bola "scroll me dohrav nahi"                  -> cursor (kadam 18).

1    Ravi app khole to server dhoondhe wo kis-kis ko follow karta (500 log), sabke tweet DB se ...
       tune bola "apne paas rakhunga, apne paas se bhej dunga" -> har user ka pehle se bana inbox (Redis)
       tune bola "dost apna mujhe de do"                  -> tweet aate hi followers ke inbox me (fanout on write)
       tune bola "dobara DB hit nahi"                     -> feed seedha inbox se.

  -> maan lo: Ravi ne aaj Amit ko follow kiya, Amit ke pichhle hafte ke 20 tweet Ravi ke dabbe me ...
       tune bola "follow karte hi purane push"            -> backfill on follow
       tune bola "naya waise hi"                          -> fanout on write (kadam 1).

  -> maan lo: Ravi ne Arpan ko unfollow kiya, par dabbe me Arpan ke 50 purane tweet pade hain.
       tune bola "turant gayab nahi, dheere kam"          -> dabba saaf nahi karte (lazy, purge nahi), naye fanout band, ranking (kadam 17) neeche karti; feed me follow ke alawa recommendation bhi.

  -> maan lo: bahut saare server, Ravi ki request kabhi kisi pe
       tune bola "har server yaad nahi rakhega"           -> stateless app server (session server me nahi)
       tune bola "JWT"                                    -> token me hi user ki pehchaan, koi bhi server padh le.

2    Kohli ne tweet kiya "Century for India". 27 crore dabbon me daalna, ghanton lagte. Ravi ko 2 ...
       tune bola "27 crore dabbe"                         -> fanout on write
       tune bola "100-200 ke liye theek, 27 crore ke liye nahi" -> followers ki had se do raaste
       tune bola "ek chhoti parchi, sab yahin se padhein" -> celeb ka tweet uski apni list me, feed khulne pe wahan se (fanout on read). Dono saath = hybrid fanout.

3    Ravi ke dabbe me doston ke tweet hain, par Kohli, Dhoni, SRK... 15 bade logon ki parchiyan ...
       tune bola "sabko ek saath lao, ek parchi pe"       -> inbox + follow kiye celeb ki list
       tune bola "DSA jaisa sort"                         -> time se merge
       tune bola "Ravi ki feed pe push"                   -> merge at read.

4    Arpan ke 8,000 followers (abhi bhi chhota user). "Post" dabaya, server ek-ek karke 8,000 dabbe ...
       tune bola "threshold, Kohli wala tareeka"          -> had ke upar celeb raasta
       tune bola "stadium wala bhai"                      -> Kafka (async fanout)
       tune bola "post ho gaya, nikal"                    -> turant OK
       tune bola "parchi pe likha"                        -> outbox ki soch (outbox = DB ki table jo kaam ke saath usi transaction me likhi jaati); yahan tweet Cassandra me (wahan tweet + outbox ek transaction me nahi likhe jaate), isliye Kafka hi pakki parchi: Tweet Service pehle Kafka me daale (acks=all), workers / consumers Cassandra + inbox likhein.

5    Kohli ne 99 se six maara aur tweet kiya. 1 minute me 5 crore log refresh, sabko wahi ek parchi ...
       tune bola "famous ho gayi, log aayenge"            -> hot key
       tune bola "10-15 copy alag machine pe"             -> key ki replicas
       tune bola "baant do"                               -> read unme bantna.

  -> maan lo: Kohli ne "Inida" likh ke 10 second me "India" kar diya, kuch copies purana dikha ...
       tune bola "thodi der galat"                        -> eventual consistency
       tune bola "peeche ek job"                          -> background job copies update kare.

6    Har account ka dabba memory me. Ravi ke dabbe me 3 saal ke tweet. 30 crore log mahino se app ...
       tune bola "last 500"                               -> inbox ki had
       tune bola "DB se query karke Redis me"             -> cache miss pe DB
       tune bola "permanent diary, S3, archive"           -> purana data S3
       tune bola "30 din se nahi khola to kuch taiyaar nahi" -> inactive user ka inbox nahi
       tune bola "lautne pe DB se Redis me"               -> on-demand inbox banana.

7    Saare tweet ek SQL DB me, din me 50 crore naye, likhne ki line itni lambi ki machine hile. Aur ...
       tune bola "likhai bahut, bada bhai"                -> Cassandra (write-heavy; andar ring = consistent hashing)
       tune bola "replica bhi wo kar leta"                -> Cassandra ki apni copy
       tune bola "naam se nahi, id se baant do"           -> partition key = user id
       tune bola "mera saara data ek jagah"               -> ek partition (author id se hissa, andar tweet id se tweet: key = author id + tweet id)
       tune bola "follow ka track"                        -> graph DB Neo4j.

  -> maan lo: har baar "Arpan ke naye 500" poori table me dhoondhna pad raha
       tune bola "naye tweet thodi der apne paas"         -> author ke recent tweets ka cache (Redis, TTL)
       tune bola "per day pattern"                        -> recent data hi garam (time-based: naye tweet baar-baar padhe jaate), poori table scan nahi.

  -> aur neeche: Cassandra hi kyun, aur key kya
       = write-heavy + read-heavy + TB data = Cassandra, partition key = user id.

8    Saare server America me. Ravi Delhi me, har request samundar paar, har kaam me aadha second ...
       tune bola "baatcheet India me rakh"                -> India region
       tune bola "Lajpat Nagar me branch khol do"         -> user ke paas wala region.

9    Arpan ne India branch me tweet kiya. Follower John New York me, America branch se baat karta, ...
       tune bola "register aapas me baat karein"          -> cross-region replication
       tune bola "us register pe bhi bhej do"             -> tweet doosre region ko copy.

  -> maan lo: dono branch ki baat 10 minute ruki aur Arpan ne beech me 5 tweet kiye.
       tweet tab tak rakhe rehna = queue me pending
       tune bola "pal-pal dekhta"                         -> retry
       tune bola "aa gaya to push kar diye"               -> link wapas aane pe replay.

10   Stadium se 50,000 log 1 GB ke video daal rahe, sab Tweet Service se guzarte, server ki ...
       tune bola "kahin aur bhej, apne server pe na la"   -> video seedha S3 me (presigned URL)
       tune bola "URL se access"                          -> tweet me sirf link.

11   S3 me ek hi 4K file. John WiFi + TV pe mast, Ravi gaon me 3G phone pe, 1 GB kheench nahi ...
       tune bola "kai size"                               -> transcoding
       tune bola "thoda-thoda kuch second ka"             -> segments (chhote tukde)
       tune bola "phone net dekh ke maange"               -> ABR (adaptive bitrate).

12   Ek machine 4K se 4 size banaye, 20 minute; Arpan ka tweet atka, aur 50,000 video usi ek ...
       tune bola "kai machine, kaam baanto"               -> video ke hisse, transcoding workers / consumers (parallel)
       tune bola "id ki parchi"                           -> har hissa video id + hissa number se.

13   10 worker / consumer, 9 ka hissa bana, worker / consumer 7 beech me gira; video na taiyaar na ...
       tune bola "har worker / consumer ka status"        -> DB me hisson ki ginti
       tune bola "tu hai kya"                             -> heartbeat
       tune bola "naya banda"                             -> job dobara (retry)
       tune bola "yahan tak parchi"                       -> checkpoint.

  -> maan lo: worker / consumer 7 zinda hai, heartbeat aa rahi, par 1 ghante se ek hissa bana ...
       tune bola "atka to replacement"                    -> timeout pe slow worker / consumer ka hissa doosre ko
       tune bola "kahan tak chhoda"                       -> checkpoint.

  -> isi maan lo me aur neeche: "zinda hoon" kaafi nahi, kaam kahan tak hua
       tune bola "zinda hoon != kaam ho raha"             -> heartbeat ke saath progress report
       tune bola "bada chunk"                             -> data skew (ek hisse pe zyada kaam)
       tune bola "2-3 chhotu madad"                       -> hissa tod ke baanto (work stealing / split).

  -> maan lo: worker / consumer 7 ke 50 me se 30 ban chuke, 2 naye chhotu bheje; unhe kaise pata ...
       tune bola "kahan tak hua ki parchi"                -> checkpoint (DB / Redis me), naya worker / consumer wahin se shuru (resume), baaki bacha kaam baant ke.

14   Ravi 2:00 pe khiskata hai six dekhne, par phone 0:00 se 2:00 tak sab kheench raha, faaltu ...
       tune bola "copy me kaunsa page kahan"              -> manifest (tukdon ki list)
       tune bola "hisaab, seedha 51"                      -> seek: time se tukda number, seedha wahi tukda.

  -> maan lo: 1 crore log dekh rahe, har ek ke ~150 tukde. Kya har tukda humare server ki API se ...
       tune bola "bas file"                               -> tukde CDN se seedhe (static)
       tune bola "bookmark"                               -> phone ka player khud yaad rakhta kahan hai
       tune bola "51 khud palto"                          -> manifest se agle tukde ka link, server sirf shuru me ek baar (manifest + link).
       tune bola "kahan aasaan"                           -> kaam wahan karo jahan sabse sasta + seedha (yahan client), server pe bojh + kharcha bina wajah nahi.

15   Six viral, 1 crore log India, US, Australia se; saare tukde US ke S3 me, samundar paar, bill ...
       tune bola "copy paas me rakho"                     -> CDN (edge cache).

  -> maan lo: private video (10 dost), Ravi ne CDN link bade WhatsApp group me daal diya.
       tune bola "taala-chaabi maalik ke paas"            -> har request pe access check
       tune bola "10 log hi"                              -> allow list (sirf link kaafi nahi).

  -> maan lo: tukde CDN se seedha jaate hain, CDN ko allow list pata hi nahi, aur link leak ho ...
       tune bola "CDN ke paas bhi taala"                  -> CDN signature jaanchta (signed URL / cookie)
       tune bola "10 user id"                             -> server allow list dekh ke hi link deta
       tune bola "5 minute umar"                          -> short expiry. Hamara faisla: kisko aur kitni der ka.

16   Ravi ke dabbe me sirf tweet ke number (#905, #902 ...). Screen pe chahiye kisne likha, photo, ...
       tune bola "ID se seedha page"                      -> hydration (ID se poori post, key se lookup; dabbe me tweet number ke saath kiska tweet bhi, "905, Arpan ka": author se hissa, number se tweet)
       tune bola "photo CDN se"                           -> post me CDN link
       tune bola "mota register"                          -> tweet store (Cassandra), like count saath me.

  -> maan lo: Arpan ne tweet delete kar diya, par 8000 logon ke dabbe me uska number (ID) abhi bhi ...
       = page pe deleted = tweet store me deleted flag (soft delete), dikhate waqt chhod do = hydration pe filter, 8000 dabbe saaf nahi karne padte.
       = kuch second purana dikhna = eventual consistency, "deleted" ki patti = tombstone / deleted placeholder.

17   Ravi roz dost Amit ke tweet pe like / comment karta. Amit ka tweet 3 ghante pehle aaya, uske ...
       tune bola "jisse zyada judta wo pehle"             -> ranking (kisse baat karta, post kitni nayi, likes)
       tune bola "algorithm design ka hissa nahi"         -> ranking model alag; hum list dete, ranked list lete. Netflix 6 ("tere liye") se juda.

18   Ravi scroll kar raha, 20 dekhe, agle 20 maange. Beech me upar 5 naye aa gaye, agle page pe ...
       tune bola "page number se agle 20"                 -> offset pagination (isi se khisakta)
       tune bola "kahan tak padha yaad rakh"              -> cursor (har page ke saath aakhri dikhayi post lautao, agla uske theek baad se)
       tune bola "thodi der yaad"                         -> ranked feed me ranked list thodi der save, cursor usi list me aage.

19   Reply ki 3 copy (A, B, C) (Twitter me Cassandra, koi leader nahi; SQL shard ho to leader + ...
       tune bola "thodi der leader se padhwa do"          -> naya likhne ke baad kuch der read leader se (read-your-writes / leader read)
       tune bola "leader pe bojh"                         -> trade-off: saari read leader pe = leader garam, isliye phir quorum.
       tune bola "do pe likho, do se padho"               -> quorum (3 copy me W = 2, R = 2, W + R > 3, isliye padhne wali do me se ek me naya reply zaroor)
       tune bola "leader aur 2 copy ne rakh liya tabhi pura mano" -> write tabhi OK jab 2 copy ne haan bola (ack)
       tune bola "Cassandra apne aap karta"               -> Cassandra me koi leader nahi, quorum ka level khud set hota (QUORUM read / write), URL kadam 9-10 wala hi. Instagram me
       tune bola "photo update ki, refresh pe purani"     -> replication lag (nayi photo leader me, refresh pe padhna copy se jo abhi peeche).

20   Priya ne kal hi Twitter join kiya, sirf 2 logon ko follow karti, un dono ne 2 hi tweet kiye. ...
       = follow wale khatam / naya user = trending + suggested (cold start), engagement se dheere-dheere apni pasand (personalized); following feed ke neeche doosron ke tweet.

```

### Notification

```
FR / NFR / dhaancha
       = FR = kya karta (functional), NFR = kitna achha / kis shart pe karta (priority, TTL, scale, delivery tracking).

1    Order Service khud DB me order likhta, phir khud SMS, email, push company ko call karta. SMS ...
       tune bola "beech me ek dabba"                      -> queue (Kafka)
       tune bola "sabko store kar lunga, aaram se bhejta rahunga" -> async bhejna, order turant ho jaata.

  -> maan lo: Order Service ne DB me order likh diya, par Kafka me khabar daalne se pehle server ...
       tune bola "order ke saath likh de"                 -> outbox (hamare DB ki table; order + khabar ek hi DB transaction me, server gire to bhi row bachi)
       tune bola "baad me koi utha ke daale"              -> relay (runner) outbox se Kafka ko bhejta.

2    Dabbe me ek hi line, SMS, email, push sab ek ke peeche ek. Ek worker / consumer. SMS company ...
       tune bola "sabki line alag"                        -> SMS, email, push ka alag topic
       tune bola "apni line sambhal lenge"                -> har topic ka apna consumer group.

3    SMS worker / consumer ne khabar uthayi, SMS Arpan ke phone pe pahunch gaya, par Kafka ko ...
       tune bola "kaam ho gaya, parchi pe likh liya"      -> idempotency (khabar ki id + "ho gaya" record)
       tune bola "dobara aaya, dekha ho gaya"             -> bhejne se pehle check, mila to skip (SMS gaya aur record likhne se pehle crash hua to ek baar dohra ab bhi ho sakta; parchi khidki chhoti karti, band nahi).

  -> maan lo: Redis (parchi) hi gir gaya
       tune bola "map me hai to skip"                     -> Redis SET NX eventId + chhota TTL
       tune bola "DB me unique"                           -> event id pe unique constraint (asli pehra), Redis = tez pehli jaanch.

4    Company ne OTP pe error lautaya: "abhi bheed hai, baad me try karo". Chhod de to OTP kabhi ...
       tune bola "alag cheez me, thodi der baad try"      -> retry queue (ruk ke dobara)
       tune bola "dikkat aayi to likh lo, kahin daal do"  -> queue / parchi / retry ka ek hi pattern. Galat number ka hal block 5 me.

  -> maan lo: OTP line me atka, der se nikla, tab tak bekaar.
       = OTP = priority queue (bheed pe 3-4 line = partition / worker / consumer badhao), umar ke andar retry. Umar khatam = line se nikalte waqt drop, bhejo mat (expiry check). App pe resend timer (60 sec) -> user naya OTP maange. SMS company ke paas atka = unki taraf validity period (kadam 5 ki shart).

  -> maan lo: "Order placed" SMS bheed me atka (retry line), aur "Shipped" push jaane ko taiyaar ...
       tune bola "order ki parchi"                        -> har order ka status (order id pe)
       tune bola "pehle wala gaya nahi to dono saath"     -> khabar milana (coalesce, kram bachao)
       tune bola "doosri SMS company"                     -> failover (kadam 5)
       tune bola "email wala"                             -> channel fallback.

5    SMS company poori band. Har call 30 second latke phir fail, saare worker / consumer atke, ...
       tune bola "teen-chaar company"                     -> failover (backup provider)
       tune bola "OTP VIP darshan"                        -> OTP ki alag priority line (topic)
       tune bola "2-3 baar fail to traffic backup se"     -> circuit breaker OPEN
       tune bola "beech-beech me main ko 'tu theek hai?'" -> HALF-OPEN
       tune bola "chal gaya"                              -> wapas CLOSED
       tune bola "backup bhi fail to number galat"        -> DLQ.

  -> maan lo: SMS company ne 30 second tak haan / na kuch nahi bola (timeout), humne backup se ...
       tune bola "30 second me haan / na, uptime kitna"   -> SLA + timeout threshold, "timeout = pata nahi gaya ya nahi" (error nahi)
       tune bola "OTP turant doosri company"              -> failover
       tune bola "2 OTP chalega, ek hi code"              -> idempotent OTP (same code dono SMS me)
       tune bola "sale wala ruk sakta"                    -> priority ke hisaab se retry.
       tune bola "30 second me nahi bheja to mat bhejna"  -> message validity period (TTL / expiry)
       tune bola "terms me tay, paisa credit"             -> SLA
       tune bola "fail wapas bata do"                     -> failure callback.
       = Twilio me ye setting `validity_period` (1 se 36,000 sec): itni der line me pada raha to bhejta hi nahi; carrier tak gaya to wapas nahi.

  -> maan lo: SMS company ka server jawab nahi de raha, aur hamare lakhon worker / consumer fail ...
       tune bola "1, 2, 5 second ruk ke"                  -> exponential backoff
       tune bola "thoda aage-peeche taaki sab ek saath na toote" -> jitter
       tune bola "2-3 company aur"                        -> multi-provider failover
       tune bola "dheeme ko kam kaam, baaki me baanto"    -> weighted routing (circuit breaker ke saath).

  -> maan lo: Arpan ka OTP gaya, SMS company ne "le liya" bola, par operator ki receipt (phone tak ...
       tune bola "WhatsApp / email"                       -> FR me channel tay (channel failover sirf tab)
       tune bola "umar ke saath bhejo, atka to mar jaaye" -> validity period (TTL)
       tune bola "screen pe timer"                        -> resend cooldown
       tune bola "success event nahi to gaya nahi"        -> delivery receipt
       tune bola "har OTP ka record"                      -> OTP store (code + expiry, DB / Redis)
       tune bola "naya banao"                             -> regenerate
       tune bola "wahi code nayi umar"                    -> resend with refreshed TTL.

6    Company ki had: ek second me 100 SMS. 20 worker / consumer apni raftaar se, sab milake 300, ...
       tune bola "company ko had badhane nahi bol sakte"  -> had apni taraf se
       tune bola "ruk-ruk ke, lambi kataar"               -> bachi khabar line me wait
       tune bola "saanjha dabba, sab usi se guzrein, ginti likhi" -> shared token bucket (Redis).

7    Company ne turant "OK, le liya" bola, record me SENT likha. Par Arpan ka phone network se ...
       tune bola "le liya, abhi bheja nahi"               -> ACCEPTED
       tune bola "ek time ke baad company batayegi"       -> webhook
       tune bola "sent ya fail"                           -> DELIVERED / FAILED
       tune bola "apni parchi me likh lo"                 -> Tracking DB.

  -> maan lo: company ne "le liya" bola, phir DELIVERED / FAILED kabhi aaya hi nahi.
       tune bola "beech ka banda, cron job"               -> status poll job (har kuch minute, ACCEPTED + purane record utha ke company ki status API se poochhe)
       tune bola "kuch na mile to FAILED"                 -> timeout ke baad fail maan ke failover (kadam 5). Payment kadam 3 jaisa.
       = write-heavy, read kam, consistency nahi chahiye, ~5 TB = Cassandra (NoSQL, write scale).

8    Diwali: ek khabar "1 crore users ko offer bhejo". Ek worker / consumer ne list nikaali, loop ...
       tune bola "tod do, alag range"                     -> campaign tukde (chunks)
       tune bola "turant DB me mark, moti diary"          -> checkpoint (DB)
       tune bola "10 me se 7, baaki 3 backup bheje"       -> crash pe wahin se
       tune bola "bhej diya tha? dobara nahi"             -> idempotency.

  -> maan lo: campaign shuru karne wala button bhi atka
       tune bola "turant 202"                             -> 202 Accepted (kaam async)
       tune bola "campaign row"                           -> campaign record (id, audience, template)
       tune bola "tukde Kafka me, kai worker"             -> chunked fan-out
       tune bola "provider ki raftaar"                    -> rate-limited send.

  -> maan lo: Big Billion Day ka 5 crore promo usi line me pada hai, aur ek user ka OTP uske ...
       tune bola "serious ki line alag"                   -> OTP / order ki alag high-priority line (topic) aur uske alag worker / consumer
       tune bola "parchi wale apni line me"               -> promo ka alag topic. Promo ki line kitni bhi lambi ho, OTP ki line pe asar nahi.
       = Kafka me message priority nahi -> priority = alag topic + alag consumer group; PriorityBlockingQueue = ek JVM tak.

9    Arpan ne settings me chuna "offer wale SMS mat bhejo". Har SMS se pehle worker / consumer DB ...
       tune bola "ek hi saancha apne paas, bas naam badlo" -> template cache
       tune bola "settings DB se hi"                      -> user preference DB se
       tune bola "OTP ki priority pehle se"               -> OTP ki alag line
       tune bola "queries badhi tab cache"                -> naap ke hi preference cache.

  -> maan lo: SMS band tha phir bhi raat 2 baje SMS chala gaya
       = preference check me channel opt-in + quiet hours (raat ko promo nahi), tab bhejo.

  -> maan lo: Diwali pe 1 crore offer, har ek se pehle DB me pasand, DB ki saans phooli. OTP ...
       tune bola "OTP seedha bhejo"                       -> OTP pe preference check skip (OTP band nahi kiya ja sakta), to OTP path pe DB hi nahi; pasand sirf offer / marketing pe.

```

### Stock broker

```
1    Arpan aur Ravi ek-doosre ko jaante hi nahi. Unke jaise lakhon log, koi 3490 pe kharidna ...
       tune bola "beech me ek aadmi"                      -> matching engine
       tune bola "Arpan is bhaav me, Ravi is bhaav me, dono theek to sauda" -> order book me bhaav milna (match).

  -> maan lo: pooche book dikhti kaisi
       = sell side = sabse kam bhaav upar (best ask), buy side = sabse zyada bhaav upar (best bid), dono milein to match.

2    Arpan ne "Buy 10 TCS" dabaya. Order engine tak pahuncha, par jawab aate waqt net gaya. App ne ...
       tune bola "ek key laga de"                         -> idempotency key (client_order_id)
       tune bola "chaabi ka taala khareed liya"           -> pehli baar wali key DB me save (UNIQUE)
       tune bola "wahi chaabi dobara aayi to nikal"       -> duplicate order reject.

3    Arpan ke account me 35,000. TCS ka 35,000 ka order book me pada, abhi match nahi hua. Isi ...
       tune bola "paisa hold kar lo, jaise EMI me bank"   -> funds block (available se kam, blocked me)
       tune bola "kharch hi na kar paaye"                 -> doosra order reject.

  -> maan lo: Arpan ko 10 TCS chahiye, 35,000 hold; Ravi sirf 4 bech raha, 4 mile, 6 baaki.
       tune bola "4 abhi, 6 baad me"                      -> partial fill (order PARTIALLY_FILLED, baaki 6 order book me)
       tune bola "10 turant warna nahi"                   -> all-or-nothing (Fill-or-Kill, hold wapas), FR tay kare. Partial me 4 ka paisa kata (14,000), baaki 6 ka hold (21,000) jaari.

  -> maan lo: Arpan ne 3500 pe 10 TCS ka order daala, TCS 3600 pe hi raha, 3
       tune bola "jab tak cancel na kare order khula, paisa hold" -> GTC (Good Till Cancelled) order, cancel pe hold wapas available me.

  -> maan lo: Ravi ke paas 10 TCS, usne 3600 pe 10 bechne ka order daala, bika nahi, phir 3650 pe ...
       tune bola "10 share usi order se bandhe"           -> share hold (holdings block, free se blocked me; doosra sell order reject)
       tune bola "usi order ka bhaav badlo"               -> modify order (naya order nahi, wahi order update).

  -> maan lo: order khula pada hai (35,000 hold) aur Arpan ne "withdraw to bank" se 20,000 nikalne ...
       = withdraw sirf available se (balance - blocked = 0, isliye reject + "order cancel / modify karo"), cancel = hold wapas available me, modify order = hold chhota.

4    9:15 baje bheed, engine tez karne ko 4 thread. Do thread ne ek saath TCS ki book dekhi, dono ...
       tune bola "copy door khadi, dono ne bech diya"     -> do thread ek book pe (race)
       tune bola "sabko ek line me, ek jagah, alag mat baanto" -> ek share ki book ek hi thread (single-threaded per symbol).

  -> aur neeche: hot IPO pe sab ek saath burst
       tune bola "ek line, counter"                       -> symbol ki ek queue, ek thread (sequencer)
       tune bola "baanta to race"                         -> shard ek symbol ke andar nahi.

5    Tez chalne ko TCS ki book RAM me. 10:30 baje machine mar gayi. Book ke saare order, Arpan ka ...
       tune bola "permanent dabba"                        -> DB
       tune bola "3-4 copy"                               -> replica / standby
       tune bola "numbers discuss nahi kiye, itni bheed nahi" -> pehle numbers, phir RAM ya DB ka faisla
       tune bola "subah se ek parchi, utni ki utni rehti" -> order log (append-only)
       tune bola "parchi dekh ke bag dobara jamao"        -> log replay se book wapas.

  -> isi pe ek aur raasta: RAM me daalne se pehle kahin aur likh lo
       tune bola "RAM me daalne se pehle kahin aur likh le" -> order log (pehle log me likho, phir RAM; append-only, sirf jodna)
       tune bola "restart pe wapas RAM me"                -> log replay se in-memory book rebuild.

  -> isi pe ek aur raasta: match ki book
       tune bola "har event book me likhna"               -> event log (append-only, Kafka ya disk)
       tune bola "light gayi, book se wapas"              -> replay
       tune bola "kaun kya kab"                           -> audit trail.

  -> maan lo: replay me wo order bhi wapas aa gaye jo crash se pehle match hoke trade ban chuke ...
       tune bola "pehle kahin aur likh, phir RAM"         -> order log pehle (write-ahead)
       tune bola "replay pe mota register dekho, ho chuke hatao" -> trade DB = source of truth, replay me sirf khule (unfilled) order book me wapas, dobara match nahi.

  -> maan lo: matching engine hi mar gaya
       = hot standby engine, failover pe promote, beech ka gap order log se replay.

6    Arpan aur Ravi ka sauda ho gaya. Khabar kai jagah: dono ke portfolio, paise ka hisaab, SMS, ...
       tune bola "10 ladkon ki fauj"                      -> Kafka (trades topic)
       tune bola "har ladka apna kaam"                    -> har service ka alag consumer group
       tune bola "main khud kuch nahi karunga"            -> engine sirf ek baar daal ke aage.

7    Sauda pakka. Arpan ke portfolio me "10 TCS" likhna tha, us waqt portfolio wala DB band. Paisa ...
       tune bola "rollback nahi, consumer court"          -> sauda ulta nahi
       tune bola "thodi der baad dikha denge"             -> settlement PENDING
       tune bola "parchi apne paas, DB aaye to de do"     -> message Kafka me pada, DB aate hi retry.

  -> maan lo: portfolio DB 2 ghante band
       tune bola "padhna replica pe"                      -> read replica se portfolio dikhao (likhna ruka)
       tune bola "retry + thodi der baad"                 -> retry + user ko PENDING.

8    TCS ka bhaav har second badal raha (3500, 3501, 3499), 10 lakh logon ki screen pe live ...
       tune bola "upar kar chuke"                         -> chat wala hi: khula taar (WebSocket) + Redis pub/sub.
       tune bola "follow kiya to khud aata"               -> push (pub/sub), poll nahi.

9    (Maan lo NSE jaisi bheed, kadam 5 wale ~10,000 / sec nahi) Subah 9:15, market khula, TCS pe ek ...
       tune bola "chhoti parchi, dhadadhad likho"         -> order book RAM me (in-memory)
       tune bola "baad me ek saath mote register pe"      -> DB me batch write
       tune bola "parchi ki 3-4-5 copy"                   -> standby / replica (aur order log). Kam bheed ho to block 5 jaisa DB pe chal jaata; NSE jaisi bheed pe RAM.

  -> maan lo: bheed 10x
       = app = horizontal (LB + server), engine = vertical scaling (ek symbol ek thread, baant nahi sakte).

  -> maan lo: Ravi ka sauda parchi pe match ho gaya, DB me abhi gaya nahi. Ravi ko "sauda ho gaya" ...
       tune bola "sach mota register"                     -> DB = source of truth
       tune bola "processing dikha do"                    -> ACCEPTED / PENDING turant
       tune bola "mote register ke baad pakka"            -> DB me commit ke baad FILLED / confirmed.

```

### Payment

```
FR / NFR / dhaancha
       = ek jagah = ek DB transaction (ACID), alag jagah = saga (har kadam ka ulta kadam), 2PC nahi (kadam 8). Double debit = idempotency se kam, par zero ka vaada nahi; reconciliation + auto refund; RBI TAT circular (20-Sep-2019): UPI merchant T+5, P2P T+1, der pe Rs 100 / din.

1    Arpan ne "pay 500" dabaya, app ko HDFC khaate se 500 nikalne hain. Par app HDFC ka hissa nahi, ...
       tune bola "centralized authority jo ye kaam khud kare" -> PSP (Razorpay, Juspay)
       tune bola "bas hum uski cheez use karein"          -> PSP ki API.
       = tokenization (payment method ID), card data PSP ke paas (PCI DSS), India me RBI ka niyam (1 Oct 2022): dukaan aur PSP dono card number nahi rakhte, sirf bank / card network; baaki ke paas token.
       = paisa = ACID chahiye = SQL (Postgres / MySQL); scale ka kram: read replica + index pehle, shard aakhir me.

2    App ne PSP ko bola "500 kaato", aur PSP 30 second se chup. FAILED dikhaya to Arpan dobara pay ...
       tune bola "apne paas ek parchi"                    -> payment table me status
       tune bola "INITIATED ho gaya"                      -> INITIATED status
       tune bola "fail hua to FAIL, pass hua to PASS mark" -> pakka jawab aane pe hi SUCCESS / FAILED.

3    Parchi pe jawab ka intezaar hai. PSP ka pakka jawab pata kaise chale? Aur PSP batana bhool ...
       tune bola "beech me banda jo batata rahe hua ya nahi" -> poochne wala job (PENDING utha ke PSP se status poochhe)
       tune bola "permanent dono se baat karta rahe"      -> PSP aur hamare DB ka milaan.

  -> maan lo: PSP ka message raste me kho gaya, ya beech wala banda bhi der se pahuncha.
       tune bola "paisa katte hi chhota event"            -> webhook (PSP callback, sign jaanch ke)
       tune bola "mil gaya"                               -> ack (2xx)
       tune bola "dobara bhejega"                         -> webhook redelivery, beech wala banda = status job backup ke liye.

4    Aaj PSP-1 poora band, uski har call fail. Dukaan pe log pay kar rahe aur sabka payment ruk ...
       tune bola "PSP-2 pe bhej do, fallback tareeka"     -> fallback PSP
       tune bola "status wale ko thodi der rok do"        -> PSP-1 pe atke payment wahin PENDING, PSP-2 pe dobara nahi
       tune bola "thodi der"                              -> 30-55 sec, phir neeche wala "Maan lo PSP-1 chup" jawab (chain me 11b) (FAILED + PSP-2, kata nikla to REFUND_PENDING).

5    Server parchi pe INITIATED likh chuka tha, aur PSP ko call bhejte waqt hi gir gaya. Restart pe ...
       tune bola "pehle register me likho, phir bhejo"    -> PSP call se pehle INITIATED commit
       tune bola "aake dekhega, PSP se dobara poochte"    -> restart pe job INITIATED utha ke status poochhe
       tune bola "idempotency key"                        -> payment id hi PSP ki idempotency key.

  -> maan lo: net atka, Arpan ne "Pay" do baar daba diya.
       tune bola "phone ki key"                           -> client idempotency key (app -> server), payment id = server -> PSP ki key. Do teer, dono pe key.

  -> maan lo: ek hi key ki do request bilkul ek saath pahunchi
       tune bola "atomic check then act"                  -> key pe unique constraint (INSERT ... ek hi jeetega), doosri ko "process ho rahi / ho chuki" lautao.

6    Arpan ke 500 hamare wallet me dukaan ke khaate me aa gaye. Ab dukaan wala ye 500 apne ICICI ...
       tune bola "ek hi bank = ek transaction me rollback" -> ek DB ke andar @Transactional / ACID
       tune bola "alag DB, ek transaction nahi"           -> isliye saga.
       tune bola "sab ulta kar do"                        -> saga (fail pe ulta kadam)
       tune bola "register me sab kaat diya"              -> ledger me ulti line
       tune bola "wapas paise bhej dunga"                 -> 500 wapas dukaan ke wallet me.

7    Wallet se 500 kaate, PSP ka jawab aane me 2 minute lage. Us 2 minute me 500 na dukaan ke ...
       tune bola "ledger me likh lo"                      -> ledger entry
       tune bola "tere paise abhi yahan hain"             -> hold (beech ka khaata): SUCCESS pe hold se nikle, FAILED pe wapas wallet.

8    Kisi ne kaha: saga ka jhanjhat kyun? Sabse pehle wallet, PSP, ICICI sabse poochh lo "taiyaar ...
       tune bola "sab taiyaar ho?"                        -> 2PC (prepare round)
       tune bola "ek slow ki wajah se sab khade, mera bhi paper chhoot jaayega" -> 2PC me ek dheema to sab ruke, isliye 2PC nahi, saga + hold.

9    Diwali pe har second 3,000 payment, saare wallet ek hi register (DB) me. Register itni likhai ...
       tune bola "register ke chhote tukde"               -> shard
       tune bola "tu ye likh, kaam baant diya"            -> har shard ka apna likhne wala, likhai bant gayi.

  -> maan lo: Diwali sale pe sirf DB nahi, sab pe pel
       = app server scale + LB, padhai read replica pe, likhai shard pe.

10   Arpan ka wallet tukda 1 me, dukaan ka tukda 3 me. Ek se kaatna, doosre me jodna. Do alag ...
       tune bola "ladka 500 le ke us tukde me"            -> saga do shard ke beech
       tune bola "diary"                                  -> saga ka har kadam likha (beech ka khaata, hold)
       tune bola "wapas aake paise de dega"               -> fail pe ulta kadam.

11   Ravi ka wallet top-up 500, PSP-1 30 second se chup. Pata nahi PSP-1 mara hai ya bas dheema, ...
       tune bola "pehle se pata down hai"                 -> circuit breaker OPEN, tab hi fallback PSP-2
       tune bola "pata nahi kata ya nahi"                 -> unknown state, PENDING (PSP-2 pe nahi, warna do baar katega) -> par user ko ghanton nahi rokte: 30-55 sec ke baad neeche wala "Maan lo PSP-1 chup" jawab (chain me 11b) (FAILED dikhao + PSP-2, kata nikla to REFUND_PENDING)
       tune bola "itna waqt, haan ya naa"                 -> timeout + status job PSP-1 se poochhe
       tune bola "naa to paise wapas"                     -> refund (PSP bola "naa" matlab kata hi nahi, to bas FAILED, wapas kuch nahi; kata tha par haan der se aaya, to refund).

  -> maan lo: PSP-1 chup hai, Ravi ko ghanton nahi rok sakte, aur PSP-2 pe bhejne se darr hai ki ...
       tune bola "30-55 sec"                              -> hamara client timeout
       tune bola "pehla DB me FAILED"                     -> us koshish ka attempt FAILED (payment ka sach event / poll se)
       tune bola "dobara PSP-2 pe"                        -> PSP failover
       tune bola "PSP-1 se poochte + event"               -> status poll + webhook
       tune bola "kata nikla to lautao"                   -> DB me REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe
       tune bola "onboard pe SLA"                         -> uptime SLA + incident response (latency ka nahi)
       tune bola "3-4 company"                            -> backup PSP.

```

### File upload

```
1    Server khud sab kar raha (lena, virus scan, thumbnail, save), Arpan 10 min spinner dekh raha, ...
       tune bola "line"                                   -> Kafka + worker / consumer (async; Kafka me file nahi jaati, sirf file ki id / S3 ka pata, file S3 me)
       tune bola "status"                                 -> DB me file ka status
       tune bola "chhota tukda"                           -> magic bytes
       tune bola "scan"                                   -> third party virus scan.

  -> maan lo: bheed badhi, kahan tootega
       = LB + zyada server, padhai pe cache, DB pe index; shard aakhri (mehnga, operational cost).

2    1000 log x 2 GB = 2 TB hamare server se guzar raha, network bhar gaya.
       tune bola "bada dabba"                             -> S3
       tune bola "tukde"                                  -> multipart upload (resume)
       tune bola "safai"                                  -> lifecycle rule (adhoora multipart hatao).

3    Phone seedha S3 me daal raha, par S3 kaise maane ye hamara user hai? App me AWS chaabi rakhi ...
       tune bola "receptionist ka token"                  -> presigned URL (hamara server deta, chaabi phone ko nahi)
       tune bola "had"                                    -> size limit / quota.

  -> maan lo: 10 GB ki file, upload 40 minute chal raha, aur receptionist ka token thodi der ka ...
       = har tukde ka token = har part ka alag presigned URL (multipart upload), resume pe naya token = sirf baaki parts ke liye naye URL.

4    File S3 me aa gayi, par server ko pata hi nahi, DB me "UPLOADING" pada, scan shuru hi nahi hua.
       tune bola "S3 bataye"                              -> S3 event notification -> DB status + Kafka me scan ka kaam.

  -> maan lo: server ne DB me UPLOADED likh diya, par Kafka me scan ka kaam daalne se pehle gira. ...
       tune bola "UPLOADED ke saath vasiyat"              -> outbox (hamare DB ki table; status + scan ka kaam ek hi DB transaction me, server gire to bhi row bachi)
       tune bola "baad me koi daal dega"                  -> relay.

  -> maan lo: resume me v1 phir v2 aaye aur event ulte pahunche. Arpan
       tune bola "parchi"                                 -> idempotency (file id + version pe ek hi baar), "kaunsa naya" /
       tune bola "apna thappa"                            -> S3 event ka `sequencer` ya hamara version number (bada = baad wala, usi file ke liye).

  -> maan lo: user har 2-3 second status poochh raha
       status poll = read-heavy -> status Redis me
       tune bola "validate hote hi update"                -> write-through / invalidate on status change, TTL backup.

  -> maan lo: worker beech me gira, file VALIDATING pe atki
       = sweeper / reaper job (status + updated_at dekh ke), S3 lifecycle rule (adhoora multipart abort).
       = file = S3, metadata + status = SQL (RDS Postgres), seedhi row + consistency, NoSQL ki zaroorat nahi.

5    "Goa trip" folder (Day1, Day2, 500 photo). S3 me folder hote hi nahi; naam path me likha to ...
       tune bola "tree"                                   -> DB me folders table (id, parent_id), file ki row me folder_id; S3 key file ki id se, naam se nahi.

  -> maan lo: Arpan ne "Goa trip" folder delete kiya. DB me folder aur 500 file ki rows "deleted", ...
       tune bola "saamne faad di, S3 baad me"             -> soft delete (DB me turant)
       tune bola "event jaaye, S3 se hatao"               -> async delete (queue + worker)
       tune bola "sync job"                               -> orphan cleanup / reconciliation job.

  -> maan lo: ek saath 10 file aayi
       tune bola "parent ka haal bachchon se"             -> folder status = sab child ka AND
       tune bola "copy badha do, baant ke"                -> kai worker parallel
       tune bola "kahan tak hua"                          -> har file ka apna status (resume).

6    Ravi ne URL me 1042 ki jagah 1043 likha aur Priya ka bank statement khul gaya. Download bhi ...
       tune bola "kiski hai"                              -> owner / share check (authorization, JWT sirf pehchaan)
       tune bola "link bekaar"                            -> presigned download URL (signature)
       tune bola "truck seedha ghar"                      -> S3 se direct download.

  -> maan lo: beech me CDN laga diya, Priya ka statement Jaipur ke CDN dabbe me pada, Ravi CDN ...
       = CDN pe bhi signed URL / signed cookie (server sign kare, CDN jaanche).

7    Ravi ne link 200 logon ke WhatsApp group me daal diya.
       tune bola "umar"                                   -> presigned URL expiry (file ki sensitivity pe).

8    Scan me 10 minute baaki, Ravi ko download chahiye. Scan me virus nikla to?
       tune bola "alag line"                              -> scan ka alag Kafka topic (Kafka me priority nahi hoti, isliye zaroori kaam ki alag line)
       tune bola "link nahi"                              -> status CLEAN tak download band
       tune bola "lo hi mat"                              -> INFECTED -> file hatao (quarantine / delete) + user ko batao.

  -> maan lo: bahar wala scanner 1 ghante se jawab hi nahi de raha, Ravi ki file "scan baaki" pe ...
       = dobara try = retry with backoff, dead letter queue = DLQ + alert (scan wali tez line se alag), link band = CLEAN tak download nahi (fail-closed).

  -> maan lo: complete kabhi aaya hi nahi
       = quarantine / temp prefix pe upload, pass hone pe asli bucket / prefix me copy.

9    App ko 6 mahine ho gaye. S3 ka bill pichhle mahine se double. S3 me ek hi meme lakhon baar ...
       tune bola "file ke content se number"              -> content hash (jaise SHA-256)
       tune bola "mote register me number"                -> DB me hash
       tune bola "pehle se hai to upload nahi"            -> deduplication (S3 khud nahi karta).

  -> maan lo: pooche S3 khud dedup kyun nahi karta
       = S3 ka bill = jitni jagah (GB-month), dedup hamari zimmedari.

  -> maan lo: Priya wahi viral meme upload karti hai jo Ravi sabse pehle daal chuka (aur uske baad ...
       tune bola "Priya ka upload mat roko, pehle uske hisaab me dekho" -> hash + userId pe check
       tune bola "poore DB me hai to usi pe redirect"     -> Priya ke naam nayi row jo purani S3 file ki taraf ishaara kare (reference / pointer, ek file kai maalik), alag users ke beech = cross-user dedup.

  -> maan lo: Ravi ne wo meme delete kiya, peeche ki safai ne S3 se file hata di. Agle din Priya ...
       tune bola "ye Ravi ka, ye Priya ka"                -> har user ki apni row (reference)
       tune bola "aur kisi ka bhi hai to mat hatao, aakhri wala hataye tab S3 se" -> reference counting (ginti 0 tabhi S3 se safai).

```

### News

```
1    NDTV, Times, Cricbuzz jaisi 5000 site kabhi bhi khabar daalti hain. Ravi 9 baje khole to 8:55 ...
       tune bola "chhotu"                                 -> fetcher (har site se khabar khinchne wala)
       tune bola "dabba"                                  -> DB.

2    Chhotu akela, 5000 site, ek site pe 2 second = ek chakkar 3 ghante. 8:55 wali khabar 11 baje ...
       tune bola "bahut saare chhotu"                     -> kai fetcher ek saath
       tune bola "chhoti parchi"                          -> Redis me taiyaar feed (cache).

  -> maan lo: koi site band hai ya bahut dheemi
       = per-source timeout + circuit breaker (ek dheemi site baaki ko na roke), queue = backlog / spike buffer.
       = shuruaat = Postgres (row: title, url, category, time, consistency); FR me write-heavy aaye = Cassandra; dono saath = polyglot persistence.

  -> maan lo: chhotu 7 ke paas 100 site thi, aur wo beech me mar gaya.
       tune bola "main zinda hoon"                        -> heartbeat
       tune bola "main site baant deta"                   -> coordinator (chhotuon ko kaam baantne wala, load balancer nahi), kaam ka reassignment (lease khatam to doosra le).

  -> maan lo: chhotu 7 ka net 20 second atka, wo mara nahi tha, aur uski site turant chhotu 12 ko ...
       tune bola "thodi der intezaar"                     -> lease / heartbeat timeout
       tune bola "purana wapas aaye to malik nahi"        -> fencing (har naye malik ka bada number, jaise epoch; purane number ki baat nahi maani jaati).

3    50 chhotu. Ravi ki feed me ek khabar 40 baar: 2 chhotu ek hi Cricbuzz se laaye, 38 alag site ...
       tune bola "bada aadmi"                             -> dedup
       tune bola "hubahu same"                            -> URL / title ka hash
       tune bola "wordings dekh lo"                       -> shabd milaan (near-duplicate check).
       = Bloom filter = "ye pehle dekha?" ka tez jawab (thodi jagah me; kabhi-kabhi galat "haan", "nahi" hamesha sahi), category tag.

4    Bada aadmi akela, har khabar ko hafte ki 5 lakh khabron se milata, 1 minute ek khabar. Line ...
       tune bola "isme daal, chalta rahe"                 -> Kafka (chhotu = producer)
       tune bola "peeche workers"                         -> consumer group
       tune bola "har dupe nahi hatega"                   -> dedup best effort, trade-off maana.

5    Ravi ko cricket, Priya ko tech, par sabki ek parchi. Aur 9 baje 1 crore log ek saath.
       tune bola "category me baant"                      -> har khabar pe category, user ki pasand DB me
       tune bola "manage karne wala banda"                -> load balancer.

  -> maan lo: Priya ne teen category chuni
       tune bola "pasand ki ek parchi"                    -> user preference store
       tune bola "shabdon ki list se chhaanto"            -> keyword-based classification (category tag)
       tune bola "feed me push"                           -> fan-out on write
       tune bola "neeche scroll pe load"                  -> pagination / infinite scroll
       tune bola "login nahi to normal khabar"            -> logged-out default / trending feed.

6    Priya app kholti hai. Chhotu site pe jaata hai ya parchi se deta hai?
       tune bola "pehli baar DB, phir parchi"             -> cache-aside
       tune bola "5 sec, 10 sec baad dobara"              -> retry + backoff, ek site band = baaki pe asar nahi.

7    Priya ki parchi 8 baje bani. 9:05 iPhone ki khabar DB me gayi. 9:10 Priya ko parchi se purani ...
       tune bola "aate hi feed me"                        -> likhte waqt push
       tune bola "milaan"                                 -> check job.

8    50 lakh tech wale, har ek ki apni parchi, har minute 20 khabar = 10 crore likhna. Workers / ...
       tune bola "ek tech parchi"                         -> category ka ek feed (ek key), likhna ek baar, padhte waqt user ki pasand se.

9    Tech parchi ek dabbe me. 9 baje iPhone launch, 10 lakh request / sec, dabba (maan lo) 1 lakh ...
       tune bola "dabbe badha do"                         -> aur Redis node, par us pe bhi tech ki copy (ek key ek hi node pe rehti, akela node load nahi baant-ta)
       tune bola "cricket dabbe me tech ki copy"          -> garam key ki copies, load baant.

10   Tech parchi 3 dabbon me. iPhone khabar sirf A me pahunchi. Priya ko A se dikhi, refresh pe B ...
       tune bola "do me likho, do se padho"               -> quorum (3 me W=2, R=2, kam se kam ek me taaza). Redis copies khud ye nahi karti; app do me likhe aur do se padhe (Cassandra jaisa DB ye khud karta).

11   Worker / consumer ne khabar DB me likhi, parchi me daalne se pehle mar gaya. Doosra worker / ...
       tune bola "status"                                 -> khabar ka state (DB me -> feed me), agla worker / consumer adhoora kaam poora karta
       tune bola "logging"                                -> outbox wali soch (outbox = DB ki table, kaam ke saath usi transaction me likhi).

12   App ko 6 mahine ho gaye. Roz 5000 site se ~1 khabar / sec, photo ke saath. "Khabar" table ...
       tune bola "FR me tay kab tak"                      -> data retention policy
       tune bola "raddi, register se nikaal ke alag"      -> archival (purana data DB se hata ke sasti jagah, jaise S3 / cold storage); jo data padha hi nahi jaata uska shard / partition bekaar.

  -> maan lo: headline alag-alag shabdon me
       Elasticsearch = alag search index (black box, PSP jaisa) (bana-banaya: AWS OpenSearch / Elastic Cloud; hum sirf sikhate kya aur kaise dhoondhna, Foundations 12); koi pooche to: har shabd ke saamne khabar ki list, jaise kitaab ke peeche ka index ("Kohli" -> khabar 3, 7, 9), isliye poori likhai nahi padhni padti (inverted index)
       tune bola "sabki jad"                              -> stemming / synonyms (analyzer)
       tune bola "ek baat ke 5 paper ek saath"            -> milti-julti khabar ka group (clustering, kadam 3 jaisa).

  -> maan lo: headline me ekdum "Kohli century" likha, khabar 9
       tune bola "dono me ek saath bharo"                 -> dual write
       tune bola "peeche-peeche"                          -> async indexing (eventual consistency)
       tune bola "kitni der chalegi, FR me"               -> search lag tay
       tune bola "sync fail to alert"                     -> alert + reindex.

```

### BookMyShow

```
FR / NFR / dhaancha
       tune bola "pehle partition"                        -> ek hi DB ke andar table partitioning (jaise show date se)
       tune bola "indexing"                               -> sahi index
       tune bola "shard ki keemat"                        -> cross-shard transaction, rebalancing, ops ka bojh; shard aakhri raasta.
       = GET seat map = read (padhne wali copy se), POST seat book = hold (dekhna + rokna ek query), POST payment = PSP ko bhejo, PAYMENT_PENDING.

1    Arpan ne D5 chuni aur "book" dabaya. Usi second ek aur ladke ne bhi D5 pe book dabaya. Dono ke ...
       tune bola "ek saath rakh, alag mat"                -> dekhna aur rokna ek hi query (UPDATE ... WHERE seat FREE)
       tune bola "seat pe parchi"                         -> hold
       tune bola "doosra parchi dekh ke mana"             -> doosre ko 0 row, "seat chali gayi".
       = atomic conditional UPDATE, consistency > availability = SQL (Postgres / MySQL).

  -> maan lo: Arpan ko family ke saath D5, D6, D7 teeno chahiye; counter wale ne D5, D6 pe parchi ...
       tune bola "teeno ek saath, beech me koi nahi"      -> ek transaction (all-or-nothing, UPDATE ... WHERE seat IN (...) AND FREE; ek bhi na mili to rollback, kisi pe parchi nahi)
       tune bola "ek parchi poore block ki"               -> ek booking id ke neeche saari seat.

  -> maan lo: Arpan seat map khol ke 1 ghanta baitha raha, aate waqt poora hall khaali tha, is ...
       = purana seat map = sirf UI ("already booked" / refresh), design nahi badalta; asli roka = kadam 1 ka "dekhna + rokna ek query".

2    Arpan ne D5 pe parchi laga di, phir pay kiye bina phone rakh ke chai peene chala gaya, kabhi ...
       tune bola "5 minute tak dekhta hoon"               -> hold expiry (hold_until)
       tune bola "parchi phaad ke phenk di, seat agle ke liye" -> hold hata ke seat wapas FREE.

  -> maan lo: pooche parchi ki umar Redis TTL me kyun nahi
       = hold_until DB column (ek hi sach, ek system), Redis = aur ek cheez sambhalni (failover, replica); zaroorat pe baad me.

3    Parchi phaadne wala counter wala har minute ghoom ke purani parchiyan dekhta hai. Aaj wo ...
       tune bola "chhota bhai / replacement"              -> backup safai job
       tune bola "parchi pe hi umar"                      -> shart query me hi (seat FREE ho YA hold_until nikal chuka), phaadne wale ka intezaar nahi.

4    Avengers ki advance booking subah 10 baje khuli. 10 lakh log ek saath app pe toot pade, seat ...
       tune bola "counter / server badha de"              -> machine badhana
       tune bola "dekhne wale ki alag diary"              -> read replica
       tune bola "capacity jitne andar, baaki line me"    -> virtual queue (waiting room)
       tune bola "ye teri parchi, agla number tera"       -> token.

  -> isi pe ek aur raasta: ek saath aane hi mat do, booking pehle se khol do) (Arpan
       tune bola "pehle se khol do, dheere-dheere aayein" -> load ko waqt pe phailana (advance booking / pre-load), peak ko chapta karna; "ek saath bheed" ka ilaaj sirf jhelna nahi, bheed ko aane se pehle baant dena.

5    Line me Arpan ka number aaya, andar jaane ka token mila jo 10 minute chalega. Usne 8ve minute ...
       tune bola "token diya, andar aa gaya, authenticate ho chuka" -> token sirf gate ka pass
       tune bola "seat ruk gayi, allot hue time me payment" -> pay ka faisla hold ki ghadi (hold_until) karti. Do alag ghadiyan: token = gate, hold = seat.

  -> isi pe ek aur raasta: der se seat chuni to 4-5 min ka grace
       tune bola "seat chuni to grace"                    -> hold ki apni umar (hold_until), token khatam hone se nahi kat-ti.

6a   Seat kiski?
       tune bola "status likh lo"                         -> HELD -> PAYMENT_PENDING (Pay dabate hi)
       tune bola "umar badha do"                          -> is status pe hold expire nahi hota (query aur safai job isko nahi chhoote).

  -> maan lo: network glitch, Pay do baar daba
       = client idempotency key (app banaye), server pe key dekh ke purana jawab.

6b   Maan lo chhotu (PSP) jawab hi nahi de raha, 1-2 baar jagaya phir bhi chup. User ko kab tak ...
       ghadi = client timeout ~60 sec (30-55 + buffer)
       tune bola "process ho raha"                        -> PAYMENT_PENDING UI message + status poll, 1 minute ke andar SUCCESS = BOOKED (refund nahi), 1 minute ke baad = payment attempt FAILED + booking HELD + UI popup "dobara pay?" (Haan = PSP-2 ka QR, Naa = turant EXPIRED, hold_until nahi badhaya), der se aaya SUCCESS = REFUND_PENDING + refund API (Razorpay bank se poochh ke lautaye), dobara pay = PSP-2.
       tune bola "1-2 baar ping"                          -> status check retry (timeout)
       tune bola "payment FAILED dikha do, seat mat chhodo" -> payment attempt FAILED, booking wapas HELD (hold_until tak, dobara pay), umar khatam to EXPIRED (paisa kata nikla to refund)
       tune bola "doosra ladka / naye log doosre PSP pe"  -> circuit breaker -> PSP-2
       tune bola "id hamare paas"                         -> payment id (idempotency key)
       tune bola "baad me kata nikla to wapas"            -> reconciliation job + refund.

6c   6b wala gola ghoom hi raha tha, popup aane se pehle hi Arpan ne thak ke back daba diya, app ...
       tune bola "30-55 sec gola"                         -> hamara client timeout
       tune bola "pehla DB me FAILED"                     -> payment attempt FAILED, booking HELD rehti (payment ka sach event / poll se, BMS 6f)
       tune bola "onboard pe SLA"                         -> PSP ka uptime SLA + incident response (latency ka nahi)
       tune bola "doosra pay doosre PSP pe"               -> PSP failover (backup PSP)
       tune bola "hum khud poochte"                       -> status poll (fetch payment API)
       tune bola "chhota event"                           -> webhook
       tune bola "mil gaya"                               -> ack (2xx)
       tune bola "baad me kata nikla to lautao"           -> REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe.
       tune bola "chhotu poochhe"                         -> status polling
       tune bola "Razorpay khud bataye"                   -> webhook (late ho to payment.authorized + late_authorized: true); Razorpay bank se 3 din tak poochhta rehta, capture na karo to khud auto-refund (Razorpay docs)
       tune bola "kitni der baad kata"                    -> late debit column (reconciliation + PSP latency ka saboot).

  -> aur neeche: UPI app ka apna gola, user back daba de to) (Arpan
       tune bola "GPay ka gola hamare server pe nahi"     -> UPI app (GPay) ka apna screen, hamara client timeout wahan nahi chalta
       tune bola "app me ruka to wahin popup, dobara pay doosre PSP pe" -> 6b wala popup + PSP failover
       tune bola "1.5 minute rokenge, chhotu dekhta"      -> status poll / webhook ka time
       tune bola "popup to dikhayenge hi, backend ko pata nahi app band hai" -> server user ki screen nahi jaanta, isliye hamesha popup bhejo
       tune bola "popup ka timer, utni der seat uski"     -> popup / hold ki umar (TTL, train booking jaisa countdown), timer khatam = hold release
       tune bola "booking fail"                           -> INITIATED -> FAILED
       tune bola "der se kata"                            -> late debit -> REFUND_PENDING + refund API (Razorpay lautaye)
       tune bola "agli baar app khole tab batao"          -> booking status jab user wapas aaye.

6d   Razorpay ka event aaya "paisa kat gaya", parchi pe SUCCESS likh diya. Ticket banane wale ko ...
       tune bola "SUCCESS aur parchi ek saath"            -> transactional outbox (hamare DB ki table; payment SUCCESS + "ticket banao" ek hi DB transaction me)
       tune bola "koi bhi aaye parchi dekh ke book kare"  -> relay (wo chhotu jo outbox ki row padh ke ticket service ko bheje); server gire to restart pe parchi wahin padi.

  -> isi pe ek aur raasta: do service, beech ka dabba, ticket PENDING
       tune bola "do service, ek transaction nahi"        -> distributed
       tune bola "beech ka dabba, ticket PENDING"         -> outbox / pending table
       tune bola "ticket service pending padh ke banaye"  -> consumer poll + retry
       tune bola "kaam ke baad dabbe se hatao"            -> outbox row processed.

6e   Arpan ne parchi ki umar ke andar Pay dabaya, bank ne paisa kaatne me 12 minute lagaye. 6a se ...
       tune bola "window khatam"                          -> 30-55 sec client timeout pe payment FAILED, phir popup pe Naa to seat turant khuli (EXPIRED)
       tune bola "chhotu poochta rahe"                    -> status poll job
       tune bola "paisa kata, seat nahi"                  -> payment SUCCESS + booking FAILED (do alag haalat)
       tune bola "refund dena hai"                        -> REFUND_PENDING
       tune bola "raat ki sync job"                       -> refund / reconciliation job
       tune bola "Razorpay ko bolo lautao"                -> refund API.

  -> maan lo: PENDING (1) raste me kho gaya, kabhi aaya hi nahi
       tune bola "khabar pe number, 2 ruke jab tak 1"     -> sequence number + ordering (out-of-order hold)
       tune bola "1 kho gaya to chhotu Razorpay se pooche" -> timeout ke baad status poll (fetch payment API)
       tune bola "ek operation"                           -> dono status ek saath (atomic).
       tune bola "ek mote register me sach"               -> source of truth (DB)
       tune bola "likhne se pehle DB dekho, SUCCESS ke upar PENDING nahi" -> conditional update (sirf tab likho jab abhi PENDING ho) = state machine, haalat sirf aage jaati
       tune bola "DB se parchi theek, ulta nahi"          -> reconciliation / sync job.

6g   Raat ki job ne Razorpay ko bola "Arpan ka 500 lautao", Razorpay bola "refund shuru". Job ne DB ...
       tune bola "pakka jawab tak REFUND_PENDING"         -> refund state (REFUND_PENDING -> REFUNDED / REFUND_FAILED, sirf refund.processed / refund.failed event pe)
       tune bola "5 din se purane pending, Razorpay se poochho" -> stuck refund job (reconciliation)
       tune bola "galat jawab mat likho"                  -> source of truth.

  -> aur neeche: refund ke do event
       tune bola "shuru"                                  -> refund.created (REFUND_PENDING / INITIATED)
       tune bola "poora"                                  -> refund.processed (REFUNDED)
       tune bola "ladke ki baat nahi, saamne wale se poochho" -> sirf pakke event pe final state.

```

### Cache

```
FR / NFR / dhaancha
       = FR = cache ka API (get / put / delete), baaki sab NFR (latency, DB offload, staleness, eviction, hot key, negative cache).

1    DB se ek baar padhne me ~50 ms, aur DB ek second me kuch hazaar query hi jhelta. Ravi ka ...
       tune bola "DB se pehle ek dabba"                   -> cache (Redis)
       tune bola "padhi bahut, update kam"                -> cache ke laayak data
       tune bola "dabbe me se de de, baar-baar DB nahi"   -> cache hit (cache-aside).

2    Dabba RAM ka hai, 64 GB. Log profile kholte gaye, sab jama hote gaye, dabba bhar gaya. Aaj ...
       tune bola "RAM khaali karo"                        -> eviction
       tune bola "jo purana, kam use ho raha, nikaal do"  -> LRU (Least Recently Used).

3    Ab 10 crore users, roz khulne wale profile hi 1 TB. Ek machine me 64 GB, LRU kitna bhi chalao, ...
       tune bola "tod do, alag dabbe me"                  -> sharding
       tune bola "hash, same pe chala jaayega"            -> key ka hash batata machine, same key hamesha same machine.

4    10 machine, formula `hash(key) % 10`, Ravi machine 3 pe. 11vi machine judi, formula `% 11`, ...
       tune bola "ring me machines paas-paas"             -> consistent hashing (ring)
       tune bola "sabse pehle khaane pe"                  -> key ghadi ki disha me agli machine pe (hamesha aage wali, peeche wali nahi, chahe peeche wali zyada paas ho; ek hi disha tay hai, isliye dhoondhne pe har baar wahi machine milti).
       tune bola "kho-kho, headlight, paas ka petrol pump" -> machine giri to uski keys aage wali agli machine sambhalti, door wali nahi.

  -> maan lo: key 47 pe giri. Peeche machine 45 pe (2 door), aage machine 52 pe (5 door). "Sabse ...
       tune bola "U-turn nahi, aage wala pump"            -> ring pe ek hi disha (clockwise), key hamesha aage wali machine (52) pe.

  -> maan lo: saare dabbe bhar gaye, to unhe ring se hata ke alag kamre me kyun nahi rakhte
       tune bola "bank locker, ek-ek karke bharna + beech ka aadmi" -> range partitioning + lookup / directory
       tune bola "saare ek saath bharte"                  -> hash se barabar baant.

  -> maan lo: ring ke saare dabbe phir bhar gaye
       tune bola "dabbe jodte jao"                        -> scale out (nayi node ring me)
       tune bola "ek hi hisaab"                           -> ring pe lookup. Cassandra bhi andar yahi (official doc: https://cassandra.apache.org/doc/latest/cassandra/architecture/dynamo.html).

5    Ravi ne apna naam badal diya. DB me naya naam likh gaya, par cache me abhi bhi purana "Ravi", ...
       tune bola "parchi se kaat do"                      -> cache DELETE
       tune bola "DB source of truth"                     -> DB = sach
       tune bola "banda har baar check kare, farak mila to theek" -> background sync (reconciliation) job
       tune bola "kuch second Ravi dikhe, paisa nahi"     -> eventual consistency chalegi
       tune bola "thodi der wala data, chhota TTL"        -> TTL. Sync job ke saath bhi ek lamba TTL rakho, taaki job chooke to purana data hamesha ke liye na atke (safety net).
       = TTL (samay pe khud mite) / delete (likhte waqt hum kaatein) / sync job (peeche se milaye) = teen raaste, jagah dekh ke chuno.

  -> maan lo: poochhe
       tune bola "pehle DB phir kaato"                    -> write-then-invalidate
       tune bola "kaatna fail to dobara"                  -> invalidation retry
       tune bola "pakka chahiye"                          -> version check / lock.

  -> maan lo: cache me 1 TB, crore-on key. Sync job ko kaise pata kaunsi key check karni hai? Har ...
       tune bola "pichhle ghante me update hui"           -> DB me updated_at pe query (index), sirf un key ka cache milao / theek karo
       tune bola "time badha do"                          -> window chhoti-badi.

6    Kuch hafte baad Priya ka craze utar gaya, log uska profile kam kholne lage, aur kadam 2 wale ...
       tune bola "10,000 ek saath DB pe"                  -> cache stampede
       tune bola "ek chhotu bhejo, baaki yahin se"        -> single flight (lock)
       tune bola "kuch second dekho, phir bheed me se ek" -> lock ki umar (timeout).

  -> maan lo: interviewer poochhe, aur raaste?
       = do chhotu = redundant fetch, hot key lambi umar = longer TTL, alag-alag expire = TTL jitter (staggered expiry).

  -> maan lo: wo ek chhotu DB pe jaake atak gaya, DB zinda hai par bojh se bahut dheema, 2 minute ...
       tune bola "DB ki copy, padhne ke liye"             -> read replica
       tune bola "dheemi copy chhod ke doosri pe"         -> timeout + retry on another replica
       tune bola "sync AWS ka kaam"                       -> managed replication (RDS).

  -> isi maan lo me aur neeche: saari copy bhi dheemi
       tune bola "thodi der purani jaankari"              -> serve stale (stale-while-revalidate)
       tune bola "Priya ko UI pe batao"                   -> read-your-writes ka UI message
       tune bola "local cache + TTL 30-45 sec"            -> local cache with short TTL
       tune bola "DB theek, chhotu dobara"                -> background refresh.

  -> maan lo: chhotu parchi bhar laaya, ab baaki bheed ka kya
       = refill ke baad traffic dheere-dheere wapas (ramp-up / warm-up), ek saath nahi.

  -> maan lo: ek bot har second 10,000 aise user id maang raha jo hain hi nahi (50 crore, 70 ...
       tune bola "nakli id, har baar andar (DB) bhaagna"  -> cache penetration.
       tune bola "parchi pe range, bahar wala wahin wapas" -> request aate hi id range check (input validation), cache aur DB se pehle; galat id na cache tak pahunchti na DB tak. (Yahan parchi = range ki parchi, cache wali key nahi.) Range ke andar par nahi mila = null ko bhi cache me rakho (negative caching, thodi der ki umar), taaki baar-baar wahi nakli id DB tak na pahunche.

7    Priya ki parchi cache me aa gayi, par ek second me 1 crore log. Ring pe uski key sirf machine ...
       tune bola "machine 4 ka replica, 8 aur 9 tu bhi dekh" -> hot key ki copies kai machines pe
       tune bola "ek aur gaadi"                           -> padhne ki bheed copies me bat-ti.

  -> maan lo: pehle se pata hai bheed kab aayegi (Diwali flash sale, iPhone launch, World Cup
       = cache pre-warm (event se pehle garam key cache me bhar do), capacity pehle se badhao.

  -> maan lo: Priya ki key machine 4, 8, 9 teeno pe hai (copies), usne naam badla, aur sirf ...
       = copies pe bhi delete = hot key ki har replica invalidate, sync job copies bhi = reconciliation sab replicas pe.

  -> maan lo: Priya ab roz ki mashhoor hai, uski parchi machine 4, 8, 9 pe garam padi rehti, pelam ...
       (jawab me
       tune bola "counter"                                -> Priya ki copy wali machine (4, 8, 9...)
       tune bola "10 counter pe baant do"                 -> 10 copy
       tune bola "counter 7 ki umar khatam"               -> jis copy ki parchi mit gayi, jaise machine 8.)
       tune bola "garam parchi pe nazar, umar badhate raho" -> hot key ka TTL extend (refresh-ahead)
       tune bola "copies ki umar alag"                    -> TTL jitter
       tune bola "sab ruko, ek chhotu jaaye"              -> single flight (lock, kadam 6).

  -> maan lo: chhotu gira, aur tune kaha thodi der purani information de do. Par umar khatam hote ...
       tune bola "chupchaap doosri parchi, thodi der purana" -> stale copy (soft / hard TTL: soft umar nikli to purana turant do aur peeche se naya lao = stale-while-revalidate)
       tune bola "app / browser ka purana, chhoti umar"   -> local cache pe chhota TTL
       tune bola "ek gira to doosra chhotu"               -> retry
       tune bola "10 na aayein tab backoff"               -> backoff: chhotu bheja, nahi aaya, thoda ruko, doosra bheja, nahi aaya, ab thoda zyada ruko, teesra bheja; har baar intezaar badhta (1, 2, 4 sec), mote register pe bombard nahi. 10 me se ek bhi na aaye tab maano mota register (DB) gira. Jitter yahan nahi: jitter kadam 7 wala hai (chain me 7c), counter / copies ki umar me thoda +1, +2 jod do taaki sab ek saath na mitein.

  -> maan lo: machine 8 tak log seedhe nahi aate, hamare 50 server ke raaste aate hain. Har server ...
       (yahan
       tune bola "counter"                                -> hamara server
       tune bola "50 counter"                             -> 50 server, har server ka apna chhotu.)
       tune bola "sabke saamne ek banda, sirf ek chhotu jaaye" -> distributed lock (sab server ke beech saajha lock: Redis me ek key, SET NX + umar; jisne pehle liya wahi DB jaaye, baaki ruk ke cache dekhein). Kadam 6 ka lock sirf ek server ke andar ho to 50 server ke 50 chhotu jaate; isliye lock saajha.

```

### Google Docs

```
1    Jo "Save" dabata hai, uska poora text server pe jaake purane ko hata deta hai. Arpan ne upar ...
       tune bola "last write wins nahi"                   -> dono ka badlaav bache
       tune bola "poora text nahi, operation bhej do: insert, delete" -> operation (op) bhejna
       tune bola "is letter ke baad add kar do"           -> op me jagah (position).

  -> maan lo: Ravi ka "budget 20k" wala op dobara chala gaya ("mil gaya" kho gaya).
       = idempotency key (wahi ID dobara aaye to chhodo).

  -> maan lo: Ravi ko Arpan ka likha dikh hi nahi raha, refresh pe bhi nahi
       version number = optimistic check (Jira / Confluence wala "kisi ne badla")
       tune bola "HTTP ek tarafa"                         -> request-response, "WebSocket two-way push"
       tune bola "judte hi chhoote ek saath"              -> reconnect pe missed ops replay (last version se aage).

2    (Ye dikkat Arpan ne khud uthayi) Hazaron-laakhon log dhadadhad save kar rahe, har chhota ...
       tune bola "chhota register, saari baatein likhta rahunga" -> buffer (server ki apni memory; gira to udega, isliye kadam 5 me Redis + copy)
       tune bola "kuch der baad DB me save"               -> batch write (Cassandra)
       tune bola "har write pe DB query nahi"             -> har akshar pe DB nahi.

  -> maan lo: trip plan doc 2 saal purana, op log me 50 lakh op; Ravi ne aaj khola to kya server ...
       = poore doc ki photo = snapshot, photo + baad ke op = snapshot + op log replay.

3    Doc me `Goa jaayenge`. Ek hi second me Arpan ne shuru me "Hum " joda, Ravi ne "Goa" ke baad ...
       tune bola "shabdon ko khiska diya"                 -> OT (Operational Transformation)
       tune bola "yaad rakhun kisne kahan likha"          -> op ki jagah pichhle op ke hisaab se aage.

  -> aur neeche: ek ne hataya, doosre ne usi pal joda
       = wahi OT: Ravi ka "B hatao" pehle laga, to Arpan ke E ki jagah ek peeche khisak gayi, dono jagah A E C D.

  -> maan lo: server ka ek banda bole
       = taala = locking (ek likhe, baaki ruke), mil ke likhne wale doc me nahi chalta; OT me sab ek saath likhte, kram server tay karta.

  -> maan lo: pooche OT ke alawa raasta
       = same jagah = tie-break (userId / timestamp), CRDT = har akshar ki id, merge commutative, server pe transform nahi (offline ke liye achha).

4    Lakhon doc, 100 server. Arpan server A se, Ravi server B se. Dono server apne kram me OT ...
       tune bola "centralized system, ek hi version"      -> ek jagah kram aur OT
       tune bola "har doc ka alag centralized system, uska maalik" -> har doc ka ek owner server.

  -> maan lo: ek doc pe 1000 log aa gaye
       = OT / transform sirf editors ke ops pe, viewers sirf broadcast sunte (fan-out), owner pe bojh likhne walon se.

5    Owner server ne 5 second ke badlaav register (apni memory) me jama kiye, Arpan ko "Saved" ...
       tune bola "ek dabba, saari information store"      -> Redis buffer
       tune bola "us dabbe ki ek aur copy"                -> Redis ki copy (replica), "Saved" tab jab copy ne bhi likh liya (Redis copy apne aap thodi der baad likhti; "copy ne bhi likh liya" pakka karne ke liye WAIT)
       tune bola "mujhe kuch ho to tu hai"                -> owner gira to naya owner usi copy se uthaaye. DB (Cassandra) me batch waise hi.

  -> maan lo: dabba aur uski copy dono gir gaye, system restart, batch abhi DB me nahi gaya tha?
       tune bola "disk pe bhi likh de"                    -> Redis AOF (persistence)
       tune bola "kuch second chhoot sakti"               -> chhota data loss window, trade-off: zyada durability = zyada copy / zyada paisa.

  -> maan lo: poore data center ki bijli gayi, Redis aur uski copy dono ek hi jagah the, dono gaye.
       tune bola "copy doosre shehar me"                  -> multi-region replica (alag data center me copy).

6    Arpan doc ka maalik, usne Ravi ko hata diya. Par Ravi ke server ke paas permission ki purani ...
       tune bola "centralized authority"                  -> permission ka asli sach (SQL)
       tune bola "iska operation accept nahi"             -> server pe permission check
       tune bola "tez bhaagne wala bachcha, building ke bahar chilla de" -> pub/sub invalidation.

  -> maan lo: Ravi hatne ke waqt offline tha, laptop pe likhta raha, aadhe ghante baad net aaya ...
       popup = client ko permission revoked ka event
       tune bola "har edit pe haq dekho"                  -> har op pe server-side permission check (aaj ka haq, purana nahi), phenk do = op reject; alag rakhna = FR pe (rejected ops ka log / draft copy).

7    Ladka chillaya "Ravi editor nahi", par server 7 us pal restart / net se bahar tha, awaaz suni ...
       tune bola "server ki copy jo peeche note kare"     -> standby / replica server bhi pub/sub sunta
       tune bola "wapas aake copy se dekh lunga"          -> reconnect pe chhoote hue badlaav copy (ya SQL) se le ke note theek, phir hi Ravi ka op maano.

```

### Kafka

```
1    Stadium ke bahar bahut bheed, sab ek saath andar ghusna chahte hain.
       tune bola "bheed ko kaabu karne wala"              -> Kafka (beech ki queue)
       tune bola "line banao, ek-ek karke jao"            -> khabar line me, padhne wale apni raftaar se (async)
       tune bola "spike aaya, rok diya"                   -> spike / load sambhalna.

  -> maan lo: pooche Kafka kab nahi chahiye
       = producer -> topic -> consumer (tube), in-process async thread = crash pe kaam gaya (durable nahi), RabbitMQ = padha to hata (ticket phaad), Kafka = log disk pe + offset, turant jawab chahiye = sync REST, managed Kafka (MSK) me broker / replica AWS ka kaam.

2    Line me ek aadmi ruk gaya, peeche sab atak gaye. Wo line se nikal ke bahar chala gaya. (Ye ...
       tune bola "seat booked, wahi baithega"             -> offset (bookmark)
       tune bola "line dobara shuru, koi gum nahi"        -> restart ke baad wahin se padhna, khabar padhne se mit-ti nahi.

3    Bheed itni ki ek admin se nahi sambhal rahi.
       tune bola "aur gate"                               -> partition
       tune bola "har gate ke andar apni line"            -> kram sirf ek partition ke andar
       tune bola "kaun kis gate pe, hash"                 -> key ka hash se partition.

  -> maan lo: bahar ka ticket counter (Order Service) har order ki do khabar bhejta hai, pehle ...
       tune bola "key ke zariye ek partition"             -> partition key (message key = order id)
       tune bola "tukde ke andar line"                    -> kram sirf ek partition ke andar.

  -> maan lo: ek user ki khabar bahut garam
       tune bola "fix seat"                               -> partition ke andar order guaranteed
       tune bola "key me number jodo"                     -> key salting (hot key ko kai partition me baanto), keemat = us key ka global order toota.

4    Gate ke andar 3 ticket checker, teeno har fan ka ticket punch kar rahe. Har ticket 3 baar ...
       tune bola "ek kamre me baitha de, ek bana de"      -> consumer group
       tune bola "3 me se ek pe bhej"                     -> ek partition group me ek hi consumer ko
       tune bola "baaki 2 dekhe hi nahi"                  -> group me ek khabar ek hi checker ke paas (par commit se pehle gira to dobara aa sakti, [kadam 6](12_message_queue_kafka.md#6-checker-register-me-likhne-se-pehle-gira-pehle-aaya-dekh-lo)).

  -> maan lo: har fan ki talaashi bhi honi hai, 2 security wale aaye. Unhe bhi checker ke kamre me ...
       tune bola "alag room"                              -> alag consumer group.
       tune bola "pehle ticket checker ka room, phir security ka room" -> checker jo paas kare use nayi line me daale (naya topic: ticket_ok), security group usi nayi line se padhe, kyunki talaashi ticket ke baad hi. Agar talaashi ka ticket se lena-dena na hota, to dono group seedha ek hi line se, apni-apni copy aur apni raftaar se padhte, ek dheema to doosra nahi rukta.

  -> maan lo: checker ke room me scanner hang, 10 minute sab atke.
       tune bola "2-3 scanner / room"                     -> group me zyada consumer (ek gira to uske partition baaki me bat jaate, rebalance)
       tune bola "pehle ticket phir talaashi"             -> do topic ki chain (ticket_ok).

5    Admin ne ticket counter ko "aa gaya" bol diya, phir wo gira, aur uski jagah lene wale ke paas ...
       tune bola "copy bhi likh de, tab likh di"          -> acks=all
       tune bola "replica ne bol diya likh liya"          -> in-sync replica ki haan (min.insync.replicas = 2).

6    Checker ne fan ko seat pe baitha diya (ticket punch), par register me likhne se pehle gir ...
       tune bola "check kar le ki pehle aaya hai"         -> idempotency (eventId se dobara wali khabar pakad ke chhodna).

  -> maan lo: worker / consumer ne Arpan ke order #501 ka SMS bhej diya, par bookmark (commit) ...
       tune bola "rok nahi sakte, dobara aayega hi"       -> Kafka at-least-once
       tune bola "pakki id, dobara aaye to discard"       -> idempotent consumer (idempotency key, eventId / order id se), wahi id SMS company ko bhi bhejo taaki wo bhi dobara na bheje.

  -> maan lo: pooche exactly once kyun nahi
       = Kafka EOS = sirf Kafka ke andar (read-process-write), bahar ka SMS = at-least-once + pakki id se dohra phenko.

7    Ticket dene wala (Order Service) gate tak khabar bhejne se pehle hi gir gaya.
       tune bola "pehle kahin likh de"                    -> outbox (hamare DB ki table; khabar order ke saath usi DB transaction me, server gire to bhi row bachi)
       tune bola "baap ki vasiyat, baad me mil jaaye"     -> relay outbox se utha ke Kafka ko bhejta hai.

  -> maan lo: wo dabba (outbox) hi gir jaaye
       = outbox table DB me hi (DB ki apni replica), relay gire to dobara uthata.

8    Ek fan ka ticket hi galat hai. Wo baar-baar gate pe ghusne ki koshish karta, har baar mana, ...
       tune bola "line se hata"                           -> DLQ (alag line, baaki line chale)
       tune bola "bouncer gate pe hi rok de"              -> galat khabar ko pehle hi rokna (validation).

  -> maan lo: Arpan ki "pay" khabar DLQ me gayi, peeche ki "cancel" chal gayi, kram toota.
       tune bola "3 logon ka group"                       -> ek order ki khabar (key = order id, wahi 3b wali, ek partition)
       tune bola "gadbad wala atka to baaki do bhi ruko"  -> us key ki aage ki khabar bhi park karo (DLQ + same key ki parking), theek hone pe kram se dobara; baaki users chalte rahein. Kafka kram ek partition ke andar pakka deta hai; yahan kram humare consumer ne toda, jab pay ko DLQ bhej ke cancel pe aage badh gaya.

9    Andar ka SMS counter 10 minute ke liye band. Jo fan wahan pahunchta hai sab fail, sab alag ...
       tune bola "sab DLQ me nahi, pehle check"           -> circuit breaker
       tune bola "check karunga kya waqai band"           -> heartbeat / health check
       tune bola "traffic divert"                         -> fallback (doosri SMS company).

10   Koi line me 7 din se zyada peeche reh gaya, aur gate ki 7 din purani list ka panna phaad diya ...
       tune bola "aur admin daalo"                        -> pehle checker (consumer) badhao, gate (30) tak; gate bhar gaye tab gate (partition) bhi badhao, isliye shuru se zyada gate rakhe the
       tune bola "itna peeche rehne hi mat do"            -> retention khatam hone se pehle lag pakadna.

  -> maan lo: wo dheere-dheere peeche ja raha
       tune bola "alert laga do"                          -> consumer lag alert.

  -> maan lo: alert ke baad bhi peeche hi rahe
       tune bola "bookmark aage mat khiskao"              -> offset skip nahi (data loss)
       tune bola "kahin aur daal do"                      -> archive / alag topic (S3 sink)
       tune bola "retention badha do"                     -> retention.ms.

```

### Banking

```
FR / NFR / dhaancha
       = statement = scope clarification (poochho), bheed = scaling (design me LB, NFR me nahi), NFR = consistency + audit + tez padhna.

1    Arpan ne parchi di "Ravi ko 500 bhej do". Cashier ne Arpan ke 500 kaate, Ravi me jodne jaata ...
       tune bola "mere 500 wapas de do"                   -> transaction rollback (ya dono kaam, ya koi nahi).

  -> maan lo: pooche saga kyun nahi, aur DB kaunsa
       = ek DB = ek transaction (ACID), saga sirf alag DB pe; row data + transaction = SQL; asli refund / reversal = peeche ki reconciliation job (din lag sakte).

2    Ravi ko 500 pahunch gaye, par receipt milne se pehle Arpan ki line kat gayi. Use pata nahi ...
       tune bola "log hamare paas hai, dekh ke mana"      -> DB me pehle wala record dekh ke dobara mat kaato.

  -> maan lo: agle din Arpan sach me Ravi ko phir 500 bhejna chahta hai. Diary me pehle se "Arpan ...
       tune bola "key jo badle hi na, same parchi"        -> idempotency key
       tune bola "ye to same hi hai, hatao"               -> key pehle aa chuki to purana jawab (UNIQUE), nayi parchi = nayi key.

  -> maan lo: wahi key abhi process ho hi rahi hai
       = unique key + "IN_PROGRESS" bhi ek jawab (doosri request ruk jaaye, dohra kaam nahi).

3    Branch me do counter. Arpan ke khaate me 600. Ek hi pal me do parchi, Netflix 500 aur UPI 500, ...
       tune bola "dekhna aur kaatna ek kram me"           -> ek query me check + kaato (`balance - 500 WHERE balance >= 500`)
       tune bola "doosre ko 600 dikhne nahi chahiye"      -> row lock.

  -> maan lo: ek hi pal me do parchi
       tune bola "dono ek doosre ka khaata pakad ke baithe" -> deadlock.
       tune bola "ek hi line, ek counter"                 -> ek ke baad ek (serialize); 10 counter bane rahein aur phir bhi na atkein, uske liye har counter pehle chhota account number pakde (lock ordering).
       tune bola "pehle ek poora, phir doosra"            -> DB atkan pakadta hai (deadlock detection), ek parchi rollback, doosri poori, phir pehli dobara (retry).

  -> maan lo: bank me roz 1 crore transfer, sab ek hi line me lage to Priya ka Amit ko bheja paisa ...
       tune bola "jo do takra rahe sirf unki line"        -> taala sirf un khaaton pe (per-account lock, row lock), baaki transfer saath-saath; kaam account ke hisaab se baanto (partition by account).
       tune bola "counter pe group ki line, kaam khatam to agla" -> thode workers / consumers, har group ek counter pe (account ke hisaab se counter tay, queue per counter), counter ek group pe bandha nahi (worker / consumer pool).

4    Hafte baad Ravi aaya: "Arpan ke 500 mile hi nahi". Register me sirf aaj ka balance, purana ...
       tune bola "alag diary, kaato mat, neeche likhte jao" -> ledger (append-only)
       tune bola "+500 / -500"                            -> double-entry
       tune bola "ek column me update"                    -> balance column, alag.

  -> maan lo: pooche ledger kaunsi dikkat solve karta
       = ledger = append-only audit trail (regulator / reconciliation ke liye), balance ka sach ussi se jodke milta.

5    Paisa jaate hi Ravi ko khabar deni thi. Cashier ne paisa bheja, aur ladke ko "Ravi ko khabar ...
       tune bola "jo yaad nahi rehta, likh lo"            -> outbox (hamare DB ki table, paise ke saath usi transaction me; bijli gaye to bhi row bachi)
       tune bola "koi bhi ladka aaye, pahuncha dega"      -> relay.

6    Mahine ki 1 tareekh, sabki salary, poora mohalla branch me. Roz ki bheed ke liye kadam 3 wale ...
       tune bola "10 cashier, 10 counter"                 -> kai service machine
       tune bola "doosre counter pe jaate"                -> LB khaali machine pe bheje.
       = consistency > availability (CP), bheed pe dheema / "baad me try" chalega, galat balance nahi.

7    Cashier 10 ho gaye, par register abhi bhi ek hi. Raat ko branch me aag lagi, register jal gaya.
       tune bola "laptop me save, ghar le jaao"           -> replica doosri jagah (Multi-AZ).

  -> maan lo: laptop me shaam ko ek baar save karta hai. 2 baje copy me zaroori line likhi, 3 baje ...
       tune bola "turant laptop pe, tabhi pakka"          -> sync copy
       tune bola "shaam ko ek baar save"                  -> async (aakhri likha kho jaata).

8    Salary ke din sab sirf balance dekhne aa rahe. Saare cashier register pe jhuke, jo sach me ...
       tune bola "photocopy alag desk pe"                 -> read replica
       tune bola "main register likhne walon ke liye"     -> primary.

9    Photocopy main register se thodi peeche chalti hai. Arpan ne abhi 500 bheje, turant photocopy ...
       tune bola "jisne update kiya, thodi der main file se" -> read-your-writes (primary se padhna)
       tune bola "sync ho jaaye to wahin se"              -> phir read replica. Instagram me
       tune bola "photo update ki, refresh pe purani"     -> replication lag (nayi photo leader me, refresh pe padhna copy se jo abhi peeche).

  -> maan lo: "thodi der" ke baad photocopy pe wapas bhejna hai. Bank ko kaise pata chale ki ...
       tune bola "replica khud bataye / chhotu poochhta rahe" -> replica apni sync position bataye (replication lag ka check).

  -> maan lo: main register kabhi band nahi hota, Priya, Ravi lagaatar likh rahe. Photocopy kabhi ...
       tune bola "replica bataye kahan tak pahunchi, 1000 me 980 = 20 peeche" -> replication lag (replica apni position batati)
       tune bola "mujhe apna 100 chahiye"                 -> Arpan ki likhai tak pahunchi to replica se, nahi to primary se (andaza nahi, seedha check). Log aur uska number DB apne andar khud rakhta, wo design ka hissa nahi.

  -> maan lo aur neeche: chalein
       = replica khud bataye = replica apni position bhejti (kam kadam, par replica atki ho to chup rehti, pata der se chalta); chhotu poochhe = poll / health check (ek kadam zyada, par jawab na aaye to turant pata, replica band). Dono sahi, trade-off.

10   Raat ko kisi bank wale ne main register me Arpan ka balance haath se 100 ki jagah 600 kar ...
       tune bola "ledger se milwao"                       -> raat ka milaan (reconciliation)
       tune bola "asli sach ledger, column nahi"          -> ledger = source of truth, balance bas copy.

11   Arpan ka khaata 5 saal purana, ledger me hazaaron line. Har raat milaan ke liye 5 saal ki line ...
       tune bola "mahine ke mahine tod do"                -> mahine ke partition
       tune bola "pichhla jud gaya, likh do"              -> snapshot (mahine ka total)
       tune bola "purana data badlega nahi"               -> purani line immutable.

12   5 saal me diary itni moti ki branch ki almari me jagah nahi. Niyam (maan lo): purani line ...
       tune bola "head office me bhej do"                 -> purana ledger S3 me (sasta, lambe samay ke liye)
       tune bola "data kahin gaya nahi"                   -> archive, phenka nahi.

13   Head office me purani diary ek godown (almari wala kamra) me rakhi hai. Ijazat wala aadmi hi ...
       tune bola "kai copy chhupa ke, milao"              -> copies (doosri jagah) se chhedchhaad pakadna
       tune bola "version ka lock"                        -> versioning
       tune bola "kohinoor, koi badal na sake"            -> S3 Object Lock.

14   31 March ko ek hi din 1 crore log statement maangne aa gaye. Statement tab banni shuru hui jab ...
       tune bola "pehle se bana ke rakh do"               -> precompute (batch job)
       tune bola "peeche chhotu"                          -> background worker / consumer (Kafka se kaam baanta)
       tune bola "turant chahiye kya"                     -> pehle requirement poochho.

15   28 Feb raat ka transaction der se aaya. Ravi ki Feb statement tab tak ban chuki thi, shayad ...
       tune bola "record se milaye"                       -> reconciliation
       tune bola "nayi statement, dobara download"        -> revised statement
       tune bola "thodi der se banao"                     -> cutoff + grace window
       tune bola "turant ya ekdam pakki"                  -> consistency vs immediacy.

16   Chhotu aadha kaam karke gir gaya.
       tune bola "DB me likhta jaata, wahin se utha le"   -> checkpoint, jo ban chuka wo dobara nahi = idempotency.

17   10 me se 9 chhotu kaam khatam karke baithe hain, ek bahut der se laga pada hai. Uske paas ...
       tune bola "ek ke paas bahut kaam"                  -> hot key / data skew
       tune bola "khaali ladke madad karo"                -> work stealing.

18   Wo bada kaam ek hi merchant ka hai: 5 crore transaction, aur statement ek hi banni hai.
       tune bola "kaam baant do"                          -> map
       tune bola "bada aadmi jod ke ek banaye"            -> reduce, dono milke = MapReduce.

19   Statement me har line ke saath running balance. May wale chhotu ko 1 May ka opening balance ...
       tune bola "mahine ke tukde"                        -> mahine ke partition
       tune bola "master register me likha balance"       -> balance snapshot (kadam 11 wala "pichhla jud gaya, likh do").

20   Raat 2 baje saare chhotu main register se arbon line padh rahe hain. Usi waqt log UPI kar rahe ...
       tune bola "photocopy se padhein"                   -> read replica (kadam 8 wala "photocopy ka alag desk")
       tune bola "main register asli payment ke liye"     -> primary.

21   Bank bada ho gaya, ab Mumbai ke saath Frankfurt (Germany) me bhi branch. Dono ka apna ...
       tune bola "do branch, dono likh sakti, raat ko milaan" -> multi-leader replication
       tune bola "har baar phone karke milao"             -> synchronous replication (dheema, line tooti to atka)
       tune bola "jahan khaata khula, wahi har kaam kare, baaki request aage bhejein" -> write affinity / home region
       tune bola "raat ko dono mile, -1000"               -> write conflict (business rule ka, AWS nahi pakadta).

```

### Chat

```
1    Arpan ne Ravi ko "kahan hai?" bheja. Ravi ka phone har second server se poochta "mere liye ...
       tune bola "poochne ka mauka hi na do"              -> polling band
       tune bola "beech ka dabba jo aate hi turant bhej de" -> WebSocket (khula taar, server khud push kare).

2    Arpan ne "kahan hai?" bheja, par Ravi ka phone band, 2 ghante baad on hoga. Taar toota pada ...
       tune bola "ek dabbe me rakh lo"                    -> message store (pehle DB me save)
       tune bola "online hoga to bhej denge"              -> offline delivery.

  -> maan lo: "college-gang" group 3 saal purana, 50 lakh message aur hazaron photo; roz log ...
       = DB = Cassandra, chat_id se baanta (ek chat ek machine pe, kram me); purani photo / video phone ki gallery me (client pe), server pe sirf link.

3    10 crore log, ek server sabke taar nahi pakad sakta, 100 server lage. Arpan ka taar server 3 ...
       tune bola "DSA wala map, key-value"                -> session map in Redis (userId -> server)
       tune bola "O(1) me mil gaya"                       -> seedha lookup.

  -> maan lo: ek server gira, uske 1 lakh log ek second me dobara judne aaye
       = reconnect storm -> exponential backoff + jitter (har phone alag waqt pe dobara jude).

4    Server 3 ko pata chal gaya Ravi server 71 pe. Message 71 tak kaise pahunche? Har server baaki ...
       tune bola "chhota ladka address le ke jaaye"       -> Redis pub/sub (har server apne channel ko sunta)
       tune bola "wahan wale ko pakda de"                 -> server 71 apne channel se le ke Ravi ke taar pe bheje.

5    Arpan ne 500 logon ke group me "kal party hai" bheja. 500 log 60 server pe bikhre. Har member ...
       tune bola "ek dabbe me rakh lo"                    -> group message DB me ek baar (chat_id = college-gang)
       tune bola "500 log khud aayenge sunne"             -> members wahi ek message padhein, 500 copy nahi.

5b   Group "college-gang" me Arpan ka message #980 "kal party hai" DB me save ho gaya (har message ...
       tune bola "1-2 chhotu extra"                       -> relay ki 1-2 copy (backup), ek gira to doosra kaam uthaye.

  -> maan lo: backup chhotu aa gaya, par #980 bhejna baaki hai ye baat to gire hue server ke ...
       = deewar ki parchi = outbox (hamare DB ki table, message ke saath ek saath, dono ya koi nahi (batch), DB me "ye bhejna hai" wali row), chhotu = relay (outbox ki row padh ke pub/sub pe daale). Server gire to bhi parchi DB me hai, jo chhotu aaye wahi padh le.

5c   Pehla chhotu gira nahi tha, bas der se tha. Wo bhi wapas aa gaya aur backup chhotu bhi; dono ...
       = parchi pe DONE = outbox (hamare DB ki table) ki row ka status (bhejna baaki -> DONE), mota register = messages table, dono ek saath (dono ya koi nahi, batch). Par "bhejna" DB ke bahar hota hai (pub/sub), isliye bhejna aur DONE likhna ek transaction nahi ban sakte: bhej diya aur DONE likhne se pehle gira to agla chhotu phir bhejega. Isliye agla kadam (5d).

5d   Dono chhotu ne ek hi pal me parchi padhi. Dono ko DONE nahi dikha, kyunki abhi kisi ne likha ...
       bahar wala aadmi = fan-out service (group ke 500 ko bhejne wala), message ka number #980 = idempotency key, wahi number dobara aaye to discard (dedup fan-out pe). Kadam 6 wali key ki hi doosri kahani: wahan phone pe key, yahan bhejne wale aadmi pe.
       tune bola "Time antar"                             -> chhotu alag-alag waqt pe utheinge, par asli bachav aadmi ka "ye chala gaya" wala check hai.

5e   Maan lo aage: do message hain, #982 "kal Goa chalte hain" aur #983 "shaam 5 baje taiyaar ...
       tune bola "kaunsa pehle aaya"                      -> server ka sequence number (#982, #983, kadam 7 / 12), phone usi kram me lagaye; #981 pehle aa gaya to bhi #980 ke baad hi dikhe.

6    Arpan lift me, Ravi ko "hi" bheja. Message server pe save ho gaya, par "pahuncha" wala jawab ...
       tune bola "ek key rakh lo, key same to mana"       -> clientMsgId (idempotency key)
       tune bola "doosri baar discard"                    -> dupe copy chhodo
       tune bola "network aaye to double tick"            -> "pahuncha" ka jawab dobara bhejo.

  -> maan lo: ulta
       = idempotency key (wahi ID dobara aaye to chhodo).
       = clientMsgId = retry ka dohra rokta; user ne khud do baar bheja = do alag message (do alag id), theek.

7    Group me Arpan ne "kal party" bheja, usi second Ravi ne "kahan?". Kuch members ko "kahan?" ...
       tune bola "numbering kar do"                       -> sequence number
       tune bola "server lagayega number"                 -> server ka sequence, har chat ka apna (#980, #981)
       tune bola "phone ko thodi pata"                    -> phone ki ghadi pe bharosa nahi.

  -> maan lo: "kal party" ko #980 mila par DB me save hone me atka, Ravi ka "kahan?" #981 pehle ...
       tune bola "server ka record, ye gaya ye nahi"      -> har user ka delivery record (store-and-forward; group me har member ka bas ek number "yahan tak pahuncha", kadam 9), reconnect pe "jo nahi gaya wo sab do", phone ke aakhri number se nahi
       tune bola "sort karke daal do"                     -> sequence number se phone pe ordering
       tune bola "FR me tay"                              -> ordering kitni strict, requirement ka faisla.

8    Ravi ko unread ginti dikhani hai: "12 naye message". Group me 980 message, 500 member.
       tune bola "ek ginti rakh lo, Ravi ne itne padh liye" -> read count (har member ka har chat me ek ginti)
       tune bola "12 aur hain"                            -> unread = chat ke kul message - Ravi ke padhe (980 - 968 = 12). Ginti hai, message ka ID nahi ghatate.

9    Arpan ne #980 "kal party" bheja, Info me dekhna kis-kis ne padha. Har message x har member ki ...
       tune bola "ginti 978, matlab 975 padha"            -> read watermark (number >= 975 to padha)
       tune bola "number ghata lo"                        -> har member ka bas ek number, 4.9 lakh ki jagah 500.

10   Arpan ne group me party ka 200 MB ka video bheja. Chat server ke raaste DB me save hoga aur ...
       tune bola "alag machine, alag raasta"              -> S3 (chat server ke bahar)
       tune bola "chhota URL sabme bhejo"                 -> message me sirf link
       tune bola "wahin se dekh lenge"                    -> video S3 / CDN se.
       = presigned URL (server ki key se sign, chhoti umar), thumbnail alag.

11   Ravi ke naam ke neeche "online". Ravi ka phone lift me band, taar tootne ka signal server tak ...
       tune bola "register me Ravi online"                -> presence entry (Redis)
       tune bola "har 3-5 second hilaunga"                -> heartbeat
       tune bola "2-3 baar jawab nahi to naam kaat diya"  -> heartbeat chhoote to entry mite (TTL), "last seen" dikhe.

  -> maan lo: Ravi chahta hi nahi ki sabko dikhe
       = last seen privacy setting (server bhejne se pehle setting dekhe).

11b  Ravi online aaya. Uske phone me 500 contacts. Server "Ravi online hai" ki khabar 500 ko bheje? ...
       tune bola "dukaan ke andar aaya"                   -> chat kholi to phone server ko bolta "Ravi ka status mujhe batate rehna" (subscribe), chat band = unsubscribe. Ravi online hua to server sirf subscribers ko bataye (presence pub/sub). Online dekhna ho to chat kholni padti, isiliye.

12   Ravi ne Arpan ko "kal milte hain" phir "nahi, parso" bheja. Pehla server S1 se gaya, doosra S2 ...
       tune bola "server pe pata kaun pehle aaya"         -> har chat ka sequence number (block 7)
       tune bola "phone sahi kram me lagaye"              -> client number se sort kare, aane ke kram se nahi
       tune bola "dheeme ke liye doosra mat roko"         -> deliver turant, baad me reorder.

  -> maan lo: interviewer kheeche
       tune bola "time ko hi number"                      -> Snowflake ID (time + machine + chhota counter, FOUNDATIONS 13)
       tune bola "har group ka apna server, ghadi usi ki" -> ek chat hamesha ek hi server pe (chat id se server tay), isliye usi server ki ghadi aur counter se kram theek; server 20 pe gaya to wahan number pichhle aakhri number ke aage se shuru.
       tune bola "ek ID"                                  -> har chat ka sequence number. Kamzor jawab = number / had ka daava ("long ki had itni, kabhi nahi bharega"), kyunki interviewer use kheench sakta. Mazboot jawab = sirf dhaancha. Had yaad aa gayi to bonus line: long (64-bit) me lagbhag 9.2 x 10^18 (9 ke baad 18 zero), aur counter har chat ka alag. Range jaisa kuch aur pooche to: "padhte waqt dekha tha isliye yaad hai, baaki net se dekh lete".

  -> maan lo: group me 500 log hain. Har message pe 500 logon ka server dhoondhna padega?
       tune bola "taar wala server"                       -> WebSocket gateway, jahan phone juda hai.
       tune bola "group wala server"                      -> chat server, jo number lagata hai. Dono alag hain.
       tune bola "server pe ek baar bhejo, aage wo apne logon ko de" -> per-server fan-out (Redis pub/sub: har gateway group ka channel sunta hai, chat server us channel pe ek baar daalta hai).

```

### Uber

```
FR / NFR / dhaancha
       = client -> kai app server (LB ke peeche) -> DB + read replica; CDN / S3 nahi (koi badi file nahi).

1    "Book" dabaya, par server ko pata hi nahi kaunsa driver abhi kahan hai.
       tune bola "beech ka dabba"                         -> Location Service, phone har ~4 sec (maan lo) lat/long bhejta.

2    Har 4 sec, 5 lakh driver = 1.25 lakh update / sec. Normal DB ghut gaya, jagah 30 sec peeche.
       tune bola "bada dabba"                             -> Cassandra (likhai tez, apni copies, machine jodo to badhta; andar ring = consistent hashing).

  -> aur neeche: Zomato wali reel se
       kadam 2 ke do raaste: pehli kahani me Cassandra me overwrite, is kahani me Redis me overwrite + raasta Kafka se alag storage.
       tune bola "parchi pe kaat ke nayi"                 -> Redis me sirf aakhri jagah (overwrite), delivery ke baad hata do
       tune bola "Kafka me daal, batch me alag storage box" -> poore raaste ki history alag jagah (Kafka -> batch write); DB me sirf order ka status.

  -> maan lo: is doosri kahani me (location Kafka + batch se
       tune bola "jaldi ke liye parchi"                   -> matching Redis se (abhi ki jagah, overwrite)
       tune bola "pakka raasta"                           -> trip location history (Kafka -> batch -> Cassandra) for SOS / support / audit
       tune bola "khaali vs bhara alag"                   -> idle aur on-trip location alag store
       tune bola "9va raasta"                             -> purani trips se raaste ki padhai (routing / ETA).

3    Book pe server 5 lakh driver ka fasla naap raha, 40 sec "dhoondh rahe hain".
       tune bola "naksha dabbon me"                       -> geohash (grid)
       tune bola "sirf apna dabba"                        -> 5 lakh ki jagah gine-chune driver.

  -> maan lo: gole ke teeno driver booked
       = gole me khaali nahi = intezaar / dobara dhoondo (koi gaadi zabardasti nahi).

  -> aur neeche: gola hi bahut bada, Whitefield
       tune bola "chhote hisse"                           -> geohash cell
       tune bola "bheed pe dabba chhota-bada"             -> cell size badalna (adaptive), wo aage kadam 6c.

4    Arpan dabbe ke kinare pe, 200 meter door bagal wale dabbe me khaali driver, app bola "koi ...
       tune bola "gola + chhoote dabbe"                   -> apna geohash + aas-paas ke dabbe, phir asli fasla.

  -> maan lo: normal din bhi gole me koi cab khadi nahi
       = search radius dheere-dheere badhao (expanding ring / neighbour cells), concert wale din jaisa hi par chhota.
       tune bola "5 km gola"                              -> aas-paas ke cell + asli fasla
       tune bola "sirf khaali"                            -> available drivers filter
       tune bola "phone baja do"                          -> broadcast dispatch
       tune bola "30 second"                              -> request TTL.

  -> maan lo: Gomti nadi) seedhi lakeer pe sabse paas Raju 500 meter, par nadi ke us paar, pul 4 ...
       tune bola "map pe paas"                            -> seedhi lakeer (straight-line) doori
       tune bola "sadak se kitni door"                    -> road distance / ETA (routing se, sirf gole ke kuch paas wale driver pe).

5    Gole me ek khaali driver Raju. Arpan aur Priya ne ek hi second me book dabaya, Raju ke phone ...
       tune bola "parchi"                                 -> driver pe hold / lock (atomic, sirf FREE ho to)
       tune bola "umar"                                   -> TTL.
       tune bola "select aur update ek saath"             -> atomic conditional update / lock (WHERE driver FREE)
       tune bola "faltu kaam"                             -> alag read-then-write me race + bekaar DB chakkar.

  -> maan lo: net atka, Arpan ne Book do baar daba diya.
       = idempotency key (wahi ID dobara aaye to chhodo).

6    Diljit ka concert khatam, 50 hazaar log, cab sirf 500.
       tune bola "500 andar, baaki line"                  -> virtual queue
       tune bola "kiraya badhao, door ki cab aaye"        -> surge pricing.
       tune bola "server badhao"                          -> autoscaling
       tune bola "dayra badhao"                           -> search radius badhana
       tune bola "door ka kiraya"                         -> surge pricing
       tune bola "khaano me kaato, alag server"           -> cell / shard ke hisaab se baantna, thundering herd se bachao
       tune bola "bagal ka khaana"                        -> scatter-gather (kadam 6c)
       tune bola "laut ke turant available"               -> driver free after trip
       tune bola "intezaar hoga"                          -> NFR me saaf: bheed me turant cab ka vaada nahi.

  -> maan lo: Ekana stadium me match khatam, 2 minute me 15,000 booking ek hi dabbe se. Drivers ka ...
       tune bola "ek dabbe pe saara bojh"                 -> hot cell (hot shard)
       tune bola "2-3 dabbe extra"                        -> hot cell ko chhote dabbon me tod ke kai shard pe baanto
       tune bola "pehle se le ke chalo"                   -> jaane-pehchaane hotspot pe pre-scaling
       tune bola "had paar to apne aap"                   -> auto-scaling.

  -> maan lo: India-Pakistan match, 2 lakh log. Server badhane se kaam nahi chala, to stadium wala ...
       tune bola "pehle server, phir todo"                -> pehle scale out, zaroorat pe hi split
       tune bola "papa ji jo sab tukde dekhe"             -> coordinator jo kai server se poochh ke jode (scatter-gather)
       tune bola "upar, neeche, bagal wale tukde"         -> neighbour cells (kadam 4 wala gola).

7    Raju ne 14.9 sec pe accept dabaya, server pe 16 sec pe pahuncha, tab tak Priya ko de diya.
       tune bola "ghadi server ki"                        -> TTL ka faisla server pe, der se aaya accept reject.
       tune bola "30 second, phir nahi"                   -> request TTL, server ki ghadi
       tune bola "agla driver"                            -> re-dispatch
       tune bola "bajta rahe"                             -> TTL ke bina broadcast jab tak accept.

  -> isi pe ek aur raasta: ek-ek driver nahi, sabke paas ek saath
       tune bola "server ne pehle dekha"                  -> server pe jo accept pehle pahuncha wo jeeta (first-write-wins / conditional update)
       tune bola "kisi aur ne le li"                      -> Raju ko ride taken / expired message.

  -> maan lo: ek-ek driver ko 15 sec dene ki jagah kuch aur karna ho?
       tune bola "sabke phone bajao"                      -> broadcast dispatch (aas-paas ke kai driver ko ek saath, jo pehle accept kare uski ride), do ne ek saath accept kiya to BMS wali parchi (jo server pe pehle pahuncha wo jeeta)
       tune bola "20-25 sec"                              -> TTL lamba, par faisla phir bhi server ki ghadi.

8    Raju Connaught Place ke dabbe se Janpath ke dabbe me gaya, purane dabbe me naam pada; Arpan ...
       tune bola "taaza jagah dekho"                      -> assign se pehle driver ki latest location check (ek driver ki ek sach jagah), lock double se bachata.

9    Dabbe kadam 3 me bana liye, par register abhi bhi driver ke naam se laga hai ("Raju -> ...
       tune bola "jagah ke hisaab se panna"               -> geohash -> drivers (Redis set har dabbe ka)
       tune bola "nikla to hata do, naye me daal do"      -> har location update pe purane set se remove + naye me add (driver ki row me pichhla dabba yaad)
       tune bola "booked to list se bahar"                -> sirf khaali driver set me, sawari khatam pe wapas.

10   Raju aa raha, Arpan ko gaadi naksha pe khisakti dikhni chahiye. 10 lakh phone har second "Raju ...
       tune bola "poochne ka mauka mat do, khud dikha do" -> push (WebSocket, khula taar), polling band.

11   Arpan ka taar server 12 pe, Raju ki jagah server 57 pe aati. 57 ko pata hi nahi Arpan kahan.
       tune bola "store karwa lo"                         -> session map (user -> server, Redis) + ride map (driver -> rider)
       tune bola "seedha usi raaste"                      -> server 57 seedha server 12 ko (pub/sub channel), wahan se Arpan ke taar pe.

12   Book pe ₹250 dikha, ride ke beech surge 2 guna, aakhir me ₹500 kate. Arpan: "maine 250 dekh ke ...
       tune bola "jo dikhaya wahi"                        -> upfront fare: quote ride ke saath save (lock), aakhir me wahi kaato
       tune bola "zyada mehnat"                           -> tip alag, rider ki marzi.

  -> aur neeche: surge nahi, traffic se raasta lamba
       tune bola "traffic me fanse ka paisa"              -> time-based fare / waiting charge
       tune bola "laal batti pe nahi"                     -> buffer (free waiting)
       tune bola "pehle se nahi bol sakte"                -> upfront fare me aage ka traffic predict nahi hota; pehli kahani (jo dikhaya wahi + tip) aur ye, dono raaste.

13   Trip khatam, ₹250 kaatne gaye, Arpan ke wallet me sirf ₹40. Payment fail, Arpan gaadi me hi ...
       tune bola "poora ya kuch nahi"                     -> atomic charge (partial nahi)
       tune bola "ride tab tak khatam nahi"               -> ride status PAYMENT_PENDING jab tak SUCCESS
       tune bola "GPay / cash"                            -> fallback payment method, dono app pe dikhe.

  -> maan lo: 250 kaatne gaye, gateway chup, pata nahi kata ya nahi.
       = idempotency key (wahi ID dobara aaye to chhodo).

  -> maan lo: poochhte raho aur gateway ka jawab aaye hi na. Arpan gaadi me baitha, Raju saamne, ...
       tune bola "30-55 sec"                              -> hamara client timeout
       tune bola "paisa katte hi event"                   -> webhook (PSP callback)
       tune bola "mil gaya"                               -> ack (2xx)
       tune bola "baad me kata to lautao"                 -> DB me REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe
       tune bola "onboard pe SLA"                         -> uptime SLA + incident response.
       tune bola "30 sec"                                 -> client timeout -> FAILED
       tune bola "wallet / doosra PSP / cash"             -> fallback + PSP failover
       tune bola "kitni der baad kata"                    -> late debit ka alag column (raat ki reconciliation job ke liye)
       tune bola "kaise diya"                             -> payment method + transaction id
       tune bola "event aaya to mat roko"                 -> webhook SUCCESS pe ride complete.

14   (Socha hua, hypothetical) Ride ke beech Raju ka phone switch off. GPS bhi band, naksha pe Raju ...
       tune bola "location aani band, ek-do baar dekha"   -> location timeout (phone push karta, server sirf intezaar karta)
       tune bola "utne paise"                             -> jahan tak ka kiraya / jo dikhaya
       tune bola "support ride band kare"                 -> manual close, ride status DB me update, driver available list se bahar.

15   Arpan aur Ravi ne saath me book kiya. Do gaadi: Car A aur Car B. Arpan ki booking pehle ...
       tune bola "dono ko saath milao, kul doori kam"     -> batch matching (kai booking aur kai gaadi ek saath, sabka milake pickup kam).

  -> maan lo: Arpan ki booking 7
       tune bola "15-20 second ruko, sab saath milao"     -> batching window.

  -> maan lo: shaam 7 baje office chhutne ka waqt. Har baar koi naya banda gaadi ke bilkul paas ...
       tune bola "baar-baar peeche reh gaya"              -> starvation
       tune bola "jitna ruka utni baari upar"             -> aging (gaadi nahi badhti, sirf baari tay hoti).

  -> maan lo: Arpan ne 5 minute ruk ke gusse me booking cancel ki aur turant dobara book ki. ...
       tune bola "parchi"                                 -> Redis me pehle tap ka waqt (intezaar pehle tap se gino)
       tune bola "30 second ki umar"                      -> TTL.

16   (Zomato wali reel se aaya: Aman ki biryani, Raju ki bike) Arpan map pe Raju ki gaadi apni ...
       tune bola "khada hai to der se, chala to jaldi"    -> adaptive ping (speed ke hisaab se location bhejne ka waqt, battery bachti).

  -> maan lo: Raju Connaught Place ki oonchi buildings ke beech hai. GPS buildings se takra ke ...
       tune bola "paas wali sadak pe chipka do"           -> map matching (GPS ka galat point sabse paas ki sadak pe, Raju ke raaste ke hisaab se).

  -> aur neeche: Zomato wali reel se, GPS ek second me nadi paar
       = wahi map matching (galat point sabse paas ki sadak pe); GPS ka galat point aana rok nahi sakte, sirf dikhne se pehle sadak pe utaar do.

  -> maan lo: location har 4 second aati hai. Beech me gaadi 4 second khadi, phir achanak aage ...
       tune bola "gudde ko andaze se aage chalao"         -> dead reckoning (speed + sadak dekh ke andaza)
       tune bola "nayi aayi to sahi jagah"                -> correction
       tune bola "U-turn pe kooda"                        -> andaza galat nikla, isliye jump.

  -> aur neeche: Zomato wali reel se, tunnel me net gaya
       tune bola "aakhri wali jeete"                      -> last write wins (Redis me overwrite)
       tune bola "bike tairti hui"                        -> purani se nayi jagah tak animation
       tune bola "15 jagah Kafka -> worker / consumer ki tasveer" -> sirf history ke liye, live map ke liye nahi
       tune bola "khada to 8 second"                      -> adaptive ping.

  -> maan lo: Raju Arpan ki society ki basement parking me ghusa aur phone 5 minute bilkul ...
       tune bola "aakhri jagah pe roko"                   -> last known location (andaze se aage nahi)
       tune bola "map ki jagah status"                    -> status update (arrived / delivered), jagah se pata chala pahuncha = geofence.

```

### Leaderboard

```
FR / NFR / dhaancha
       = pehle sawaal = scope / requirement clarification, FR = score update + top N, NFR = tie-break + eventual consistency (contest ke beech).
       = FR = score update + scoreboard (har user usi board me apni rank dekhe); "top kitne dikhane" sirf pehle sawaal (a), FR nahi.

  -> fr / nfr ki doosri kahani: wahi dikkat
       dhaancha = CDN + LB + kai app server + local cache + Redis + RDS Postgres (primary + replica)
       tune bola "consistent, JSON nahi"                  -> row wala data, SQL.

1    Top 10 aur "tera rank" chahiye. SQL me har baar 1 crore row sort, aur rank ke liye "mere upar ...
       tune bola "apne aap sort wala dabba"               -> Redis sorted set (har khiladi + score, kram me; daalna, top 10, rank sab log N). (Redis khud chhote se bade ke kram me rakhta; leaderboard me ulta (reverse) padhte, bade se chhote, taaki sabse zyada score upar.)

  -> maan lo: reel se) kisi ne kaha Redis kyun, SQL me score pe index laga do. Index laga diya, ...
       tune bola "har block me jaake dekhna"              -> rank ke liye "mere upar kitne" gin-na (index se bhi count scan)
       tune bola "har likhai pe index badle"              -> write-heavy pe index ka bojh (har update pe index update), isliye sorted set (daalna, rank, top sab log N).

  -> isi maan lo ka doosra jawab: wahi dikkat
       tune bola "apne aap sort wala dabba"               -> Redis sorted set (ek dabba jisme score daalte hi line apne aap kram me rehti).

2    Ravi aur Priya dono ke 9800. Line me aage kaun?
       = dabba har khiladi ke saath ek hi number leta, to "score phir time" ek number me pack: aage score, point ke baad time (jo pehle aaya uska hissa bada). Ravi 9800.95, Priya 9800.90.

  -> maan lo: Claude ka nitpick) pooche
       = jo pehle aaya uska hissa bada rakho (ya time ko ulta karke jodo), tabhi bade-se-chhote padhne pe pehle wala upar = composite score; misaal ka number sirf misaal.

3    Har second lakhon match khatam, saare score update ek hi Redis pe.
       tune bola "baant do"                               -> shard
       tune bola "har line se le ke milao"                -> scatter-gather
       tune bola "chhotu + badi screen"                   -> aggregator jo global top 10 bana ke cache me rakhta.

  -> aur neeche: kitne Redis chahiye, kaunse region, top 10 kaise jude
       = region se baanto = shard, har dabbe se lo = scatter-gather, 5 sorted list milana = merge k sorted (heap).

  -> maan lo: match ka +50 retry se do baar pahuncha.
       = idempotency key (wahi ID dobara aaye to chhodo).

  -> isi maan lo me aur neeche: server atka to? parchi pe likh lo
       = sawaal ki ID = idempotency key, parchi pe likh ke baad me bhejo = outbox (hamare DB ki table, usi transaction me likho; chhotu / relay baad me bheje) + retry.

  -> maan lo: server parchi pe likhne se pehle hi gir gaya, likh hi nahi paaya.
       = score + outbox (hamare DB ki table) ki line ek hi DB transaction me (dono ya koi nahi), isliye "score likha par parchi nahi" ho hi nahi sakta.

4    Main India wali line me 3,000 pe hoon. Screen pe poori duniya ka rank dikhana hai, aur lines ...
       tune bola "mere upar kitne"                        -> har shard se count (mere score se upar), sab jodo, +1.
       tune bola "Referee wala poora board"               -> background job jo har kuch minute me snapshot banata (thoda purana chalega).

  -> aur neeche: world rank turant kyun nahi niklegi
       = andaze wali rank = estimated rank, sab region mila ke board = aggregator / global board, rank dheere theek = eventual consistency.

5    Teen board chahiye: aaj ka, is hafte ka, all-time. Raat 12 baje aaj wala khaali ho ke naya din ...
       match jeeta to score teeno dabbon me jodo.
       tune bola "Alag register"                          -> har din ka alag dabba, naam me tareekh (board:5-Nov, board:week-45, board:all-time).
       tune bola "Kal wala side me"                       -> live Redis se TTL se hat sakta, par board history me rehta; 80 lakh ko ek-ek chhuna nahi padta.

  -> isi pe ek aur raasta: alag register ki jagah seedha overwrite
       = reset = naya key (din ki tareekh se) ya agla score aate hi overwrite; 80 lakh ko ek-ek zero karna nahi.

  -> aur neeche: game nahi, contest ho to board kaise
       = contest id se alag board (key me contest id), purana = DB me history, mahine wala = sab contest ka jod (aggregate).

  -> maan lo: Ravi ka match 11
       tune bola "parchi pe contest id + khatam hone ka waqt" -> event time
       tune bola "usi din ke register me"                 -> event time se bucket (pahunchne ke waqt se nahi)
       tune bola "purana register side me"                -> har din ka board history me rakho (discard nahi).
       tune bola "register kar ke aaye nahi"              -> contest ka sign-up
       tune bola "mota register"                          -> DB (kadam 7, score pehle DB me).

  -> isi maan lo me aur neeche: contest ka aakhri sawaal 12
       = parchi pe submit ka waqt = event time, gira to parchi se baad me bhejo = outbox (DB ki table), bheed me beech ka Kafka = buffer / queue (disk pe, worker = consumer).

  -> maan lo: contest raat 11 baje shuru, 1-3 baje khatam
       = contest ka din = shuru hone ki tareekh (contest id ke saath); duniya bhar ka online contest = ek hi time zone UTC.

6    Main 4,52,318 pe hoon, top 10 me kabhi nahi aaunga. Screen pe mere aage ke 5 aur peeche ke 5 ...
       tune bola "apne aas-paas scan"                     -> har shard se mere score ke thoda upar aur thoda neeche wale (score range query), sab mila ke sort, mere upar ke 5 aur neeche ke 5.

  -> maan lo: dabbe region ke hisaab se
       tune bola "dabbe score ki range se"                -> range partitioning (score range pe shard)
       tune bola "coordinator ka thappa"                  -> shard map / router jo batata kaunsi range kis shard me
       tune bola "seedha Delhi wale dabbe pe"             -> sirf matching shard pe range query (baaki shard skip)
       tune bola "andaze wali rank"                       -> estimated rank
       tune bola "galat jawab bhi ginte"                  -> penalty (LeetCode: har galat submission pe 5 minute).
       tune bola "availability zyada, rank baad me theek" -> AP + eventual consistency
       tune bola "sign-up se pehle pata kitne aayenge"    -> capacity planning / pre-partition
       tune bola "IP se desh, shehar"                     -> jagah ke hisaab se baantna (geo partitioning; shehar IP / profile se)
       tune bola "ek dabbe pe kai thappe"                 -> shard map me har shard ki kai score range
       tune bola "ye rank nahi, bas baantna"              -> partition key, rank global board pe.

  -> aur neeche: range se baantne ka matlab saaf
       tune bola "shehar ke gole"                         -> geo partitioning (location se shard)
       tune bola "dabbe pe range ka thappa"               -> shard map (kis shard me kaunsi range)
       tune bola "seedha Delhi ke dabbe pe"               -> query sirf us ek shard pe, poore 10 lakh pe nahi.
       poora world board = global leaderboard (aggregator sab region ke result mila ke ek sorted board)
       tune bola "pehle sawaal gino, phir waqt, ms, naam" -> sort key (solved desc, finish time asc, ms, naam) = composite score (kadam 2)
       tune bola "paanchon wale pehle, baaki baad me"     -> solved count ke hisaab se bucket
       tune bola "rank dheere theek"                      -> eventual consistency.

7    Redis gira to saare score?
       tune bola "clone"                                  -> Redis replica (gira to replica primary)
       tune bola "teeno gaye to DB"                       -> DB sach, Redis wahan se rebuild.

  -> aur neeche: kitni consistency chahiye, uske hisaab se kitni copy
       = copy = Redis replica, peeche DB me likhna = DB sach (Redis wahan se dobara), pehle parchi = log / outbox (DB ki table), kitni copy = consistency ka faisla.

```

### E-commerce

```
FR / NFR / dhaancha
       = FR = browse / stock -> cart -> checkout -> order; NFR = consistency (oversell nahi), alag sale service, payment latka na rahe.

1    Shelf pe aakhri ek ghee. Ravi ne app pe order kiya, aur usi minute aunty wahi dabba counter pe ...
       = har raaste ka alag stock. Online = godown ki ginti, counter = shelf. Dono kabhi ek dabbe pe nahi takraate.

  -> maan lo: Lucknow ka store khaali, Kanpur me 20 dabbe.
       = har store ka apna stock, app location dekh ke paas wala store chunta.

2    Ladka godown se 10 dabbe shelf pe le gaya, kahin likha nahi. App pe abhi bhi 40, sach me 30.
       = parchi = DB. Maal hilaana aur ginti badalna ek hi transaction me (atomic), alag-alag kadam nahi.

3    Diwali sale, 8 baje 50 hazaar log ghee pe. App atak gaya, aata-daal wale bhi atke.
       = sale ka alag raasta / alag service. Uski bheed baaki dukaan (normal orders) tak nahi pahunchti.

4    Sale dukaan me ek counter, ek parchi "ghee: 500". Ek waqt me ek hi aadmi parchi chhoota, line ...
       = ek row pe sab toot pade = hot row. Ginti 10 tukdon me todo. Kaatna tez jagah pe (Redis me ginti, DECR).

  -> maan lo: Redis me ghee ki ginti kat gayi (Ravi gate ke andar aaya), par DB me order likhne se ...
       = banda milata rahe = reconciliation job, DB sach = Redis sirf tez ginti, ghadi se purani bina order wali kati ginti wapas = compensation (gate ki 5 min ki ghadi ke baad hi, chalte hue pay ko nahi chhedna).

5    Counter 3 ke 50 khatam, uski line me 2000 log. Counter 7 pe abhi 30 pade.
       = ek tukda khaali to doosre tukde se lo. Sold out tabhi jab saare tukde 0.

6    500 kat gaye, "sold out" dikh gaya, par 60 logon ne pay hi nahi kiya.
       = cart = sirf list, koi reserve nahi. Stock pay ke waqt katta. Fail hua to wapas (atomic DB query).

  -> maan lo: ek order me ghee, aata, daal, aur aata khatam.
       = har cheez alag check. Adhoora order chalta.

7    3000 log ek hi second me Pay dabaate hain, ghee 500.
       = paisa aane pe server pe ek-ek karke ginti kaato, 500 ke baad aaye paise ka refund (rollback). Doosra raasta: Pay dabaate hi dabba ghadi (10 min) ke saath rok do, refund kam, par dabba ghadi tak ruka. Dono ki keemat.

  -> maan lo: sabko mil hi nahi sakta
       tune bola "+1 ya 0"                                -> hold ke beech dikhne wala stock (available vs reserved alag rakho)
       tune bola "sabko nahi mil sakta"                   -> oversell / cancel ka 100% hal nahi, gate + jaldi out of stock.

7b   Rollback me bhi pech, 10 minute wale me bhi pech. Koi screwdriver?
       = sale gate (Buy now, cart me nahi jaata). Gate sirf utne logon ko Pay tak jaane deta jitna stock (Redis DECR). Fail / chala gaya = agla batch. Gate ke bahar walon ka paisa kabhi katta hi nahi, aur koi dabba bina bike nahi padta. Gate ek baar banta, har sale pe sirf list badalti (item + ginti + waqt).

  -> maan lo: Ravi ne 4 tab khole, Buy now 4 baar.
       = idempotency key (wahi ID dobara aaye to chhodo).

  -> maan lo: gate se 500 andar, Ravi Pay page pe gaya aur chai peene chala gaya, na pay kiya na ...
       tune bola "ya pay kar ya rakh de"                  -> reservation expiry (TTL, 5 min)
       tune bola "wapas line me, agla andar"              -> release (Redis INCR) + agla batch. TTL khatam hone se ginti apne aap wapas nahi badhti; ek job (ya expiry ki khabar) INCR karti hai.

  -> maan lo: Ravi ki ghadi 10
       tune bola "5 min ke baad phone uska nahi"          -> Pay pe server check kare ki reservation abhi zinda hai (expiry check)
       tune bola "1-2 second upar"                        -> chhota grace period
       tune bola "andar pay, PSP dheema = hamari zimmedari" -> Pay dabate hi PAYMENT_PENDING, hold ruk jaata (BMS jaisa).

8    Net gaya / PSP chup: Ravi ko laga pay ho gaya, par PSP tak request pahunchi nahi, ya paisa ...
       = PAYMENT_PENDING (BMS jaisa) + status job PSP se poochta rahe. Paisa kata mila to dabba uska; dabba na bacha ho to refund. Perfect design koi nahi, bas kitne refund.

  -> maan lo: PSP 30-50 second chup, Myntra ko pata nahi Ravi ka paisa kata ya nahi.
       = order PENDING + payment INITIATED (Myntra ka apna DB), paise ki khabar sirf PSP se (webhook / status API, din ka settlement report), hamara client timeout (50 sec), der se "success" aaya to DB me REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe -> isliye dobara pay doosre PSP pe chal jaata (PSP failover), do baar kata to ek wapas.
       = cart sirf payment SUCCESS pe khaali, fail pe cart + order wahin, retry options = doosra PSP (failover) / net banking / COD (payment method fallback).

  -> maan lo: Ravi ka paisa kat gaya, par Myntra ko ye khabar der se mile ya mile hi na. Har baar ...
       tune bola "paisa katte hi event"                   -> webhook (PSP callback)
       tune bola "mil gaya"                               -> ack (2xx)
       tune bola "dobara bhejega"                         -> webhook redelivery
       tune bola "3-4 company line me"                    -> backup PSP (failover).

  -> maan lo: PSP ka "success" do baar aaya (ek retry se, ek status job se).
       = idempotency key (wahi ID dobara aaye to chhodo).

9    Diwali sale aa gayi. Mausi ne bahut saara maal mangwa liya, sab ek hi godown me padta to wahan ...
       tune bola "B, C gire to ring pe aage wali chalti almari D, E pe likh do" -> sloppy quorum (Amazon Dynamo, leaderless replication)
       tune bola "har almari ki ek aur copy"              -> replication factor badhana (kharcha zyada, khaali almari bekaar).

9b   Aadhe ghante baad B aur C wapas chalu. Rahul ka naya iPhone wala cart D aur E pe pada hai (jo ...
       tune bola "D ka chhotu B, C ke wapas aate hi unhe Rahul ka iPhone de aaye" -> hinted handoff (D ke paas maal + parchi / hint ki ye asal me B, C ka hai)
       tune bola "sabko sync karne wali job"              -> anti-entropy / read repair.

```

### Netflix

```
FR / NFR / dhaancha
       = NFR = jaldi shuru (low start-up time) + adaptive bitrate (ABR), buffering zero ka vaada nahi.

1    Saari episode US ke store me. 12:30 pe karodon log wahi file maang rahe, har tukda samundar ...
       tune bola "file apne shehar me"                    -> ISP ke andar Netflix ka dabba (Open Connect)
       tune bola "ghar nahi la sakta"                     -> poora video phone pe nahi rakhte
       tune bola "dost roz aata-jaata"                    -> dabba US ke store se bharta rehta.

  -> maan lo: pooche video cache (Redis) me kyun nahi
       = badi, kabhi na badalne wali file = CDN (edge), Redis = chhota garam data (KB), video nahi.

2    Dabba lag gaya, par season 12:30 pe aaya, 12:29 tak dabba khaali. 12:30 pe 2 crore ek saath, ...
       tune bola "pehle se laa ke rakho"                  -> release se pehle raat ko dabba bharna (proactive fill)
       tune bola "badlegi nahi"                           -> video static hai, isliye CDN pe rakhna safe.

3    Mumbai ke dabbe ki jagah seemit, Netflix ka saara maal (har film x kai size) nahi samaata.
       tune bola "garam paas"                             -> popularity se kya rakhna (region-wise)
       tune bola "bade dabbe se mangwa lo"                -> upar wala bada store / tier
       tune bola "garmaahat khatam to wapas"              -> thanda maal dabbe se nikalna.

4    Mumbai me 20 dabbe, kuch me Money Heist hai kuch me nahi, ek ki saans phool rahi. Ravi ke TV ...
       tune bola "zyada dabbon me"                        -> hot content ki kai copy
       tune bola "server mod dega"                        -> play pe server batata kaunse dabbe se lo (steering)
       tune bola "guard"                                  -> load + maal dekh ke sahi dabba.

  -> maan lo: aadhe episode pe dabba 3 band ya dheema.
       tune bola "2-3 dabbon ke naam"                     -> server player ko kai CDN dabbon ki list deta, ek atka to player khud agle pe (client side failover).

5    Ravi ne TV pe 27 minute dekha, band kiya. Subah metro me phone pe khola, 27:00 se chahiye.
       tune bola "account se yaad"                        -> watch position user ke account pe server me, device me nahi
       tune bola "parchi DB me"                           -> persistent store.

  -> maan lo: TV ka net gaya, TV ne "27
       tune bola "naya point jeete, purana 27 discard"    -> har position ke saath uska waqt (timestamp), baad wala jeete (last-write-wins, event time se, aane ke waqt se nahi)
       tune bola "main Netflix hoon, tukde main hi bhej raha" -> ISP wala dabba bhi Netflix ka (Open Connect), agla tukda nahi maanga to andaza 25-27 ke aas-paas.

  -> maan lo: karodon TV har kuch second "26
       tune bola "parchi paas"                            -> position pehle fast store me
       tune bola "ruk gaya to register"                   -> DB me sirf aakhri / kabhi-kabhi likhna (write kam).

  -> maan lo: tukde ISP ke dabbe se aa rahe, AWS ke server ko kaise pata ki ruk gaya?
       tune bola "dost sankshep me bataye"                -> dabba / player playback ki report server ko bhejta.

6    App khola, server pasand wali list banane baitha: 3 saal ka itihaas, karodon logon se milaan, ...
       tune bola "pehle se laga ke rakhta"                -> list raat ko pehle se banti (batch)
       tune bola "app khole to saamne"                    -> cache se uthao, sochna nahi.

  -> maan lo: list raat 2 baje bani, Ravi ne 11 baje Dhurandhar khatam ki, 11
       tune bola "turant parchi"                          -> abhi ka dekha alag fast store me
       tune bola "dikhate waqt milao"                     -> pehle se bani list + abhi ki baat mila ke dikhana.

  -> maan lo: Priya ne pehli baar join kiya, koi itihaas nahi.
       tune bola "garam maal"                             -> naye user ko region ka trending (cold start)
       tune bola "pattern banega"                         -> dekhne se apni list.

  -> maan lo: Priya Ravi ki behen hai
       = pasand account / profile ke hisaab se, rishta nahi pata; naye account pe region ke trending (cold start).

7    Ravi metro me Money Heist dekh raha, 4 sec ka tukda chal raha. Tunnel aayi, net 3 second poora ...
       tune bola "agla tukda pehle se paas rakh lo"       -> player buffer (aage ke kuch tukde pehle se; sirf ek tukda aage = tunnel me ruk jaata)
       tune bola "Mother Dairy, kaam chal jaayega"        -> kam quality ka tukda (ABR, P3 ka Alto)
       tune bola "raat ko de jaaya kar"                   -> raat ko bharai (kadam 2).
       = courier = per-GB bill wala CDN (CloudFront / Akamai), mohalle ka fridge = Open Connect (ISP ke andar, free), raat ko pehle se = proactive fill (off-peak).
       = AWS = login / browse / recommendation / steering (control plane), Open Connect = sirf video ke bytes.
       = Ferrari / Alto = transcoding (kai size) + ABR, 4 sec tukde + manifest = segments + seek, har film ki apni seedhi = per-title encode ladder, kai roop = codec, ek baar banao = AWS pe batch encoding (parallel workers / consumers), awaaz / subtitle alag = alag audio / subtitle tracks.
       = chaiwala = offline / batch recommendation (tera + tere jaise logon ka itihaas), thoda abhi ka mod = online halka re-rank, apni tasveer = personalized thumbnail, dekhna -> Kafka = events ka stream batch tak.

```

### YouTube

```
1    Upload dabaya, video hamare app server se hokar storage ja rahi. Usi waqt 10 hazaar creator ...
       tune bola "file seedha bade storage me"            -> pre-signed URL, phone seedha object storage (S3 / GCS) me upload karta
       tune bola "chhota hissa apne paas"                 -> DB me sirf video ki parchi (metadata), app server pe video nahi.

2    Video storage me aa gayi. Ab 360p, 720p, 1080p, 4K banana hai. Ek machine poori video ek-ek ...
       tune bola "upload ho gaya, khabar bhejo"           -> storage event, queue
       tune bola "file tod do, 20-25 log"                 -> segment me kaat ke parallel encoding (har tukda, har quality)
       tune bola "phir jod do"                            -> merge
       tune bola "pehle chhoti quality"                   -> 360p pehle publish, baaki baad me.

3    Vlog viral: 1 minute me 1 lakh view. Har view pe DB me ek hi row me views + 1. Ek lakh request ...
       tune bola "parchi pe +1"                           -> view event stream me (Kafka)
       tune bola "chhotu batch me jode"                   -> batch aggregation (memory me gino, kuch second me ek baar likho), count thodi der se badhta = trade-off
       tune bola "likhai zyada"                           -> Cassandra.

  -> isi pe ek aur raasta: Kafka ya Redis, pehle ek dabbe me jama
       tune bola "pehle dabbe me"                         -> Redis counter (INCR) / Kafka
       tune bola "gaddi jama hone pe ek saath"            -> batch flush to DB.

4    Ek creator ne script likhi: hazaar phone / bot se har 3 second refresh, ek ghante me 10 lakh ...
       tune bola "gateway pe rok do"                      -> rate limit per user / IP / video
       tune bola "WAF me daal do"                         -> WAF ban
       tune bola "worker / consumer pattern dekhe"        -> fraud detection worker / consumer (bot / repeat view ginti se pehle filter).

5    Roz lakhon video, har ek ki 40 GB original + 5 quality ki copies. 10 saal purani videos jo ...
       tune bola "garam / thanda storage"                 -> hot / cold storage tiering (lifecycle policy: itne din koi nahi dekha to sasti jagah)
       tune bola "pehle se India ke CDN pe"               -> popular video ko pehle se regional CDN pe.

6    2010 ka gaana thandi jagah pada tha. Aaj reels pe trend ho gaya, 1 ghante me 50 lakh log. ...
       tune bola "phat gaya, garam me lao"                -> trend detect (view spike, view count wala stream), promote to hot storage + CDN
       tune bola "ginti giri, wapas almaari"              -> demote (CDN se hatao, phir cold storage, TTL / lifecycle).

7    Arpan ka vlog chal gaya, uske subscriber nayi video ka intezaar karte. Par asli bojh tab, jab ...
       tune bola "sab subscribers ko push"                -> fan-out (Twitter design) + notification system. Ghanti ke liye pull nahi chalta (user ko pata hi nahi kuch aaya), isliye bade creator pe bhi push; crore subscribers ko queue me daal ke workers / consumers thoda-thoda bhejte (Notification design). Home feed me video dikhana alag baat: wahan Twitter wala hybrid (bade creator ka pull) chalta.

8    Arpan ne Goa vlog se 30 second ke shorts bhi kaat ke daale. Chhoti video ka swipe (YouTube ...
       tune bola "agle 20 page pehle se"                  -> pre-fetching (aage ki reels ki list ek saath aati, aur unke tukde pehle se phone pe, agli reel ka player bhi taiyaar, swipe kiya aur chali)
       tune bola "tukde paas me"                          -> CDN
       tune bola "kai quality"                            -> transcoding.

  -> maan lo: Ravi metro me mobile data pe hai, 1.5 GB ka pack. Zyadatar reels 1 second me skip ...
       tune bola "net ke hisaab se quality"               -> ABR (adaptive bitrate, pehle blur phir saaf)
       tune bola "1 second wali reel ab nahi"             -> recommendation ka engagement signal (kitni der dekhi), battery wala hissa public nahi.

  -> maan lo: Ravi ka net achha, reels 1080p me aa rahi, par 5 me se 4 reels 1 second me skip. App ...
       tune bola "shuru ke kuch chunks"                   -> pre-fetch me sirf pehle kuch second, baaki tab jab reel sach me chale
       tune bola "kitna laana"                            -> user ke pattern se (kitna skip karta). Trade-off: zyada laaye to data aur battery, kam laaye to spinner. Data saver on ho to pehle se laana band.

  -> maan lo: 1-2 reel achhi nahi lagi
       = region / sub-region pe trending (cold start), user signal se personal feed; ek-do skip pe nahi hatao, lambe waqt tak koi engagement nahi to hatao.

```

### PUBG

```
FR / NFR / dhaancha
       tune bola "button dabate hi chale"                 -> low input latency (client-side prediction)
       tune bola "goli ke pal tak peeche"                 -> fair hit registration (lag compensation)
       tune bola "faisla server ka"                       -> server authoritative.

1    Raat 9 baje 1 lakh log ek saath "Start" dabate hain. Ek match me 100 chahiye, Arpan ke match ...
       tune bola "America wala America ke server pe"      -> region-based matchmaking (har desh ka apna server, ping kam)
       tune bola "akele ko 3 wale squad me"               -> squad fill
       tune bola "2-3 ka squad bhi chalega"               -> flexible squad size.

2    Match shuru. Server har second (maan lo) 20 baar sabki jagah sabko bhej raha: 100 logon ko ...
       tune bola "khula taar"                             -> WebSocket / UDP connection (push)
       tune bola "sirf aas-paas walon ko event"           -> area of interest / interest management (map ko dabbon me baanto, Uber ke geohash jaisa, har player ko sirf aas-paas ke dabbon ki khabar).

  -> maan lo: pooche aas-paas kitna
       = area of interest radius ~500 m - 1 km.

2b   Arpan ka phone Mumbai server se 60 ms door, jaana-aana 120 ms. Arpan "aage chal" dabata, phone ...
       tune bola "player ko turant apni screen pe chala do, server ka intezaar nahi" -> client-side prediction
       tune bola "peeche chhoti line, server usse jagah pakde" -> phone apni chaal (input) server ko bhejta rehta, server apna hisaab chalata (server authoritative).

3    Ek hacker ka phone server ko bolta "meri goli Arpan ko lagi", jabki goli deewar pe lagi ya ...
       tune bola "server line kheenche, deewar dekhe, gudda bana ke dekhe" -> server-side hit validation (raycast: goli ki line, beech me deewar, player ka hitbox)
       tune bola "upar lagi to headshot"                  -> hitbox (sar / body / pair alag), phone sirf "maine is disha me goli chalayi" bhejta, faisla server ka.

3b   Phone ne Arpan ko turant 3 kadam aage dikha diya. 120 ms baad server ka jawab: us pal se pehle ...
       tune bola "server ki maano, 3 kadam peeche"        -> server reconciliation, jhatke se peeche aana = rubber banding
       tune bola "server scene recreate / replay kare, mujhe peeche karke dekhe" -> lag compensation (server rewind: goli wale ke ping jitna duniya peeche, Valve ne apne games me likha)
       tune bola "deewar ke peeche phir bhi mara"         -> lag compensation ka asar, corner se nikal ke maarne wale ko isi se fayda = peeker's advantage (Riot ne kam karne ko Valorant me 128 tick server).

4    Kadam 3 wala wahi hacker agle match me phir aaya. Is baar uski goli imaandaar thi. Dikkat ye ...
       tune bola "pichhli jagah se 1 second me itna hi"   -> server-side movement validation (speed ki had, namumkin chaal reject)
       tune bola "update phenk do"                        -> discard / server ki jagah sach
       tune bola "agli baar ghusne mat do"                -> anti-cheat ban.

5    "Winner winner chicken dinner." Raat 10 baje 5,000 match lagbhag ek saath khatam, 5 lakh ...
       tune bola "likhai dhadadhad"                       -> write-heavy DB (Cassandra)
       tune bola "match ke beech parchi, end me ek saath" -> game server memory me jama, match end pe batch
       tune bola "parchi line me + copy, phir DB"         -> queue (Kafka) + async write, copy = durable queue (Arpan ko DB ka intezaar nahi, agla match turant).

```

### Inventory

```
1    Ravi ne Delhi ke aakhri iPhone pe "Buy" dabaya, payment page pe pahunchte hi phone ki battery ...
       = ginti Buy pe nahi, payment pe ghate (koi reserve nahi). Ghatana ek hi kaam: "ek kam karo, sirf tab jab kam se kam 1 ho" (`UPDATE ... SET qty = qty - 1 WHERE ... AND qty >= 1`, atomic conditional update), do log ek saath aayein to ek ko mila, doosre ko "nahi hai".

  -> maan lo: Ravi aur Arpan dono ne aakhri iPhone cart me daala aur ek hi second me pay kar diya. ...
       tune bola "doosri inventory se de do"              -> doosre godown se sourcing (kadam 4 wala hub)
       tune bola "kahin nahi to refund"                   -> order cancel + refund (E-com wala), store ka faisla = business rule (chhota store maal laaye, bada refund kare).

2    Diwali se pehle Delhi godown me 1000 naye iPhone aaye. Diwali sale, raat 12 baje, ek second me ...
       = stock ko tukdon me baanto (bucketing / sharded counter), har tukda alag row, taala ek row pe nahi.

  -> maan lo: Ravi ki request dabba 3 pe gayi, wo khaali ho chuka, baaki dabbon me 150 pade.
       = khaali bucket -> request agle bucket pe, out of stock = saare bucket 0.

  -> maan lo: sab dabbe 0, page pe "Out of stock". Ravi ne order cancel kiya, ek iPhone wapas. +1 ...
       tune bola "stock me wapas, kisi dabbe ka nahi"     -> restock / inventory release (koi bhi dabba +1, "out of stock" hata do)
       tune bola "jisne pehle pay kiya uska"              -> ginti payment pe (koi reserve nahi, kadam 1).

  -> maan lo: phone hi kam pad gaye
       = sale ka stock pehle se plan (forecast), khatam = out of stock, cart = reservation nahi.

3    Mahine ke ant me manager rack pe ginta hai: 48 iPhone. System bolta 50. "Ye 2 kab gaye, kis ...
       = register = stock ledger (har badlaav ki entry: kab, kitna, kis order se), raat ka chhotu = reconciliation job (ledger aur ginti ka milaan).

4    Ravi Bangalore me iPhone order karta hai. Bangalore godown khaali, Delhi godown me 50, Mumbai ...
       = main warehouse + shehar ke godown = hub and spoke, kahan se maal uthana = fulfilment / sourcing rule (FR tay kare).

5    Agli sale (Republic Day) se ek din pehle iPhone ka page 1 crore log khol rahe, "sirf X bache". ...
       = banner / copy = cache (Redis), page pe thodi purani ginti chalti hai, asli jaanch Pay pe (NFR 2).

6    Ravi ne iPhone return kiya, courier ne box godown pe chhoda, aate hi ginti +1. 10 minute me ...
       = return pe doorstep quality check, ginti sirf sellable stock me badhe, kharab maal alag khaane me (damaged).

7    Delhi me iPhone 5 se neeche aa gaye. Ab kya: sirf manager ko email, ya kuch aur?
       = had pe alert (low stock / reorder point alert), line me, order nahi rukta (Kafka event, async), kai raaste (email, SMS, WhatsApp, backup provider). Order lene se pehle saare godown ka jod (available to promise across warehouses), kitne godown = FR. Maal mangwana = purchase / procurement system, inventory ke scope se bahar. Jaldi aane wala maal ho to order lete raho (backorder), der ho to out of stock (restock lead time, FR tay kare).
       = alert ki seedhi (escalation: manager -> upar), kai channel failover.

```

### Where is my train

```
1    Train me Arpan ka net band, GPS band. Phir bhi app ko batana hai train kahan hai.
       tune bola "kaunsa tower juda"                      -> cell ID (har tower ki pehchaan, bina net / GPS ke phone ko pata)
       tune bola "5-10 km ke dayre me"                    -> tower ki range, pakki jagah nahi (accuracy trade-off), GPS = battery + band jagah pe kamzor.

2    Train Rewari se nikli, agla station 40 km door. Beech me outer signal pe 25 minute ruki, phone ...
       = timetable = shuruaat ka andaza (offline, phone me), asli khabar jeetti hai: "delayed by X min" + expected time (ETA, pakka nahi).

3    Train me Arpan ka net band. Dost station pe khada, uska net chal raha. Dost ke app ko kahin se ...
       = tower ka data operator (Jio) ke paas, bahar nahi aata; khabar = train me jinka net chalu unka app server ko bheje (crowdsourcing), server se dost ke phone pe push (dost ka net chahiye). Mera apna hisaab mere phone pe offline.

4    Ye list kahan se: "tower 40817 = Jaipur se 25 km pehle". Railway ke paas tower ki list nahi, ...
       tune bola "tukdon me, route ke hisaab se"          -> route-wise data collection
       tune bola "app me daal do"                         -> tower -> patri ki jagah wali list, phone me offline (lookup).

  -> maan lo: tu akela ya 5 bande har route pe safar karke note karoge to saalon lagenge, aur Jio ...
       = list ka source = data partner (jiske paas permission ka record), updates = naya / hata tower aate rahein, app ki list taaza (data refresh).

  -> maan lo: railway bole
       = kai source se list jodna (railway + shehar ki authority + operator), jo mile wahi; asal me kaise bani wo public nahi.

```

### Yelp

```
FR / NFR / dhaancha
       = FR = geo search (fuzzy bhi) + sort; NFR = DB aur search ka sync (Closed badge, change event + raat ki job).

1    Raat 10 baje Ravi "burger" likhta hai. Abhi sirf Postgres hai, 50 lakh restaurant, har ek ke ...
       tune bola "aas-paas ke restaurant, latitude longitude" -> geo filter (geohash / radius)
       tune bola "kuch bhi likho sahi option"             -> Elasticsearch (inverted index) + fuzzy search (typo tolerance). (bana-banaya: AWS OpenSearch / Elastic Cloud; hum sirf sikhate kya aur kaise dhoondhna, Foundations 12)

2    Search ab Elasticsearch se, asli data Postgres me. "Burger Singh" ne aaj ke liye dukaan band ...
       tune bola "Postgres aur ES sync"                   -> DB -> search index sync (Postgres sach, ES search ki copy)
       tune bola "band dukaan Closed dikhe"               -> status field (filter / badge), thodi der ES peeche = eventual consistency.

3    Sync job har raat 2 baje. Paas wale "Burger Point" ne dopahar 1 baje menu me "Paneer Burger" ...
       tune bola "badlaav pe event"                       -> change event (Postgres me badlaav hua to event)
       tune bola "raat wali job bhi"                      -> reconciliation job (jo event chhoota wo pakde).

4    Ravi ne "burger" likha, 5 km ke andar 200 restaurant, screen pe 10. Upar wahi jinke naam me ...
       tune bola "sort ka option user chune"              -> sort param (rating / doori)
       tune bola "server ke paas saare, phone ko sorted pehle 10" -> server-side sort (Elasticsearch me) + pagination.

```

### Zomato

```
0    Arpan ka pehla dhaancha: user -> Zomato app -> server -> AWS RDS Postgres. Har restaurant ka ...
       tune bola "menu ek dabbe me, DB se aage"           -> cache (Redis)
       tune bola "DB me menu badla to cache update"       -> write pe cache update (writes kam, reads zyada).

1    Arpan Whitefield ke PG se app kholta hai. Server 15 km ke andar ke restaurant ki list kaise ...
       tune bola "lat / long se 15 km ka gola"            -> geo / radius search
       tune bola "sabko wahi list, ek dabbe me"           -> area ke naam pe list cache (Redis) + TTL
       tune bola "DB badla to dabbe me closed"            -> write pe cache update
       tune bola "ek server lock leke laaye"              -> cache stampede pe lock
       tune bola "app me push"                            -> fan-out.

2    Neha Whitefield ke kinare pe hai, KR Puram ki taraf. Uske 15 km me do-teen restaurant aate jo ...
       tune bola "area ke andar area, har ek ki list"     -> grid / geohash cell (Uber jaisa), har cell ki list cache me
       tune bola "har user ki alag nahi"                  -> cell ke naam pe key.

3    Menu dekh ke Burger Singh se burger order kiya. Order Burger Singh tak kaise pahunchega?
       tune bola "accepted -> ready -> delivering"        -> order status flow (state DB me)
       tune bola "mere phone pe"                          -> notification / webhook
       tune bola "restaurant 5 km ke driver bulaye"       -> driver dispatch (radius), tracking = Uber wali.

4    Burger 7:30 pe ready, sabse paas wala driver 5 km door, aane me 15 min. Tab tak burger counter ...
       tune bola "quantity se ready time, driver ko wahi time" -> prep-time estimate se driver dispatch (driver tab pahunche jab khana ready).

5    Raat 9 baje order gaya, par Burger Singh ki taraf se na accept aaya, na reject (app band pada, ...
       tune bola "2-3 min window, phir fail"              -> accept timeout + auto-cancel
       tune bola "paisa tabhi jab accept"                 -> accept ke baad hi capture (warna refund).

6    Diwali ki raat 9 baje 50,000 log ek saath order kar rahe, akele Burger Singh pe 2 minute me ...
       tune bola "subah stock poocho, utne hi andar"      -> capacity gate (baaki out of stock)
       tune bola "har restaurant ki line + order placed + worker peeche" -> restaurant-wise queue (Kafka, key = restaurant) + async processing.

7    Burger Singh ki ginti 350, abhi 349 ho chuke. Arpan aur Rahul ek hi pal me order dabate, dono ...
       tune bola "ek counter, check + ghatao ek saath"    -> atomic decrement (Redis DECR / Lua, ya `UPDATE ... SET bacha = bacha - 1 WHERE bacha >= 1`), pehle wale ko = first come first served; banking ke do counter aur inventory ke aakhri iPhone jaisa.

8    (Reel se, Swiggy; scene-mock nahi, Arpan: "dead letter queue padh rakhi, Kafka ke offset / ...
       tune bola "baar-baar fail, line atki"              -> poison message
       tune bola "had"                                    -> retry limit (SQS me maxReceiveCount; Kafka me N retry ke baad DLQ topic me daalo aur offset aage)
       tune bola "alag line"                              -> dead letter queue (DLQ)
       tune bola "team ko pata"                           -> DLQ pe alert.

```

---

## 1. Ek cheez, do log, ek hi second  (TAALA)

```
Dikkat:      cheez ek hai (seat, driver, aakhri ghee, 600 ka balance, ek token), dono ne ek saath dekha "khaali hai", dono le gaye.
Humne socha: dekhna aur rokna ek hi kadam me. Seat pe parchi laga do, jisne pehle lagayi uski, doosre ko turant "chali gayi".

Yahi dikkat yahan bhi:
  URL shortener 1   do logon ko same token                         ->  ek line wala counter, aage badhao
  URL shortener 7   5 counter, dono ne "10" diya                   ->  apni-apni range
  Rate limiter  4   20 server ne ek saath "tokens = 1" padha       ->  sab ek hi baari me, koi ghus na paaye (Lua script)
  Stock         4   4 thread, 10 share 20 me bike                  ->  copy apne paas kheencho, ek jagah (1 thread per symbol)
  BookMyShow    1   dono ko D5 "mil gayi"                          ->  seat pe parchi, dekhna + rokna ek saath (hold)
  BookMyShow    1b  D5 D6 D7 chahiye, D7 beech me Priya ki         ->  teeno ek saath, ek parchi, ek na mile to koi nahi (transaction)
  Banking       3   600 me do counter pe 500-500                   ->  dekhna aur kaatna ek kram me (row lock)
  Banking       3b  Arpan->Ravi, Ravi->Arpan, dono atke            ->  ek hi line, ek counter; ya pehle ek poora, phir doosra (serialize / lock ordering, deadlock detection + retry)
  Banking       3c  1 crore transfer, sab ek line me, bank dheema  ->  jo takra rahe sirf unka group, baaki 50 counter pe (per-account lock, partition)
  Uber          5   Raju pe ek second me do ride                   ->  BMS wali parchi, ek kadam me (hold / lock)
  Uber          13  wallet me sirf 40                              ->  poora kaato, aadha nahi (atomic charge)
  Uber          15  Car A Arpan se 2 km, Ravi se 1 km; Arpan ko di ->  dono ko saath milao, kul doori kam (batch matching)
  Uber          15b Arpan 7:00:00, Ravi 7:00:02, gaadi de chuke    ->  15-20 sec ruko, sab booking saath (batching window)
  E-commerce    1   aakhri ghee, aunty + Ravi ek saath             ->  online ko godown, counter ko shelf (channel ka alag stock)
  E-commerce    2   ladka uthake likhna bhool gaya                 ->  uthana + parchi ek hi kaam (atomic transaction)
  Inventory     1   do log ek pal me aakhri iPhone                 ->  "ek kam karo sirf tab jab 1 ho", ek hi kaam (atomic conditional update)
  Inventory     1c  dono ne pay kiya, ek ko iPhone, doosre ka paisa kata ->  doosre godown se de do, ya cancel + refund; chhota store arrange kare (sourcing / refund)
  E-commerce    6b  ek order me ghee, aata, daal; aata khatam      ->  poora order mat roko, jo hai wo bhejo, aate pe out of stock (adhoora order)
  Zomato        7   349 pe Arpan + Rahul ek pal me, 351 ho gaye     ->  ek hi counter, check + ghatao ek saath, pehle wale ko (atomic decrement)

Naam: lock / atomic update / transaction / row lock / counter range
Juda: parchi laga ke koi gayab ho jaaye to umar chahiye (2). Ek hi cheez pe bahut bheed to counter todo (7).
```

## 2. Parchi laga ke gayab  (UMAR)

```
Dikkat:      kisi ne parchi lagayi (seat, driver, lock, "online") aur phir aaya hi nahi, cheez hamesha ke liye ruki.
Humne socha: 5 minute dekhunga, phir phaad do. Parchi pe hi umar likh do, apne aap gayab. Ghadi server ki, phone ki nahi.

Yahi dikkat yahan bhi:
  Stock         3   35,000 se do order, chahiye 70,000             ->  EMI jaisa paisa hold (funds block)
  Stock         3b  10 chahiye, 4 mile, 6 baaki                    ->  FR: 4 abhi + 6 ka hold jaari (partial) ya 10 nahi to kuch nahi, hold wapas (Fill-or-Kill)
  Stock         3c  3500 ka order, market band, bika nahi          ->  order khula, hold jaari, cancel pe wapas (Good Till Cancelled)
  Stock         3d  Ravi ke 10 share, 2 sell order, 20 bike        ->  share bhi hold, naya nahi, wahi order ka bhaav badlo (modify order)
  Stock         3e  order khula (35,000 hold), 20,000 withdraw         ->  beech se nahi; cancel karo (poora wapas) ya modify karke chhota (withdraw sirf available se)
  BookMyShow    2   parchi laga ke chai peene, kabhi nahi aaya     ->  5 minute dekhunga, phir phaad do (hold_until)
  BookMyShow    3   parchi phaadne wala chhutti pe                 ->  chhota bhai / replacement (backup safai job)
  BookMyShow    3   20 minute se purani parchi lagi                ->  parchi pe hi umar, apne aap gayab (expiry query me)
  Chat          11  phone band, 2 ghante se "online"               ->  haath maaro, 2-3 baar chup to kaato (heartbeat, TTL)
  Uber          5   Raju 15 sec me accept na kare                  ->  parchi ki umar, agla driver (TTL)
  Uber          7   14.9 pe accept, server pe 16 pe                ->  ghadi server ki, parchi phaad do (server TTL)
  Uber          7b  ek-ek driver ko 15 sec ki jagah kuch aur?      ->  sabke phone bajao "tum lo"; ya 20-25 sec, ghadi server ki (broadcast dispatch)
  Uber          14  Raju ka phone band, naksha pe gayab            ->  location band, ek-do baar dekho (location timeout)
  Uber          15d cancel karke dobara, ginti 7:05 se             ->  hatao, ya Redis parchi "5 min ruka" 30 sec umar (TTL)
  Leaderboard   5   raat 12 baje aaj wala khaali                   ->  alag register, kal wala side me rakho (key me tareekh, history)
  E-commerce    6   60 ne pay nahi kiya                            ->  cart sirf list, paisa nahi diya to maal uska nahi; ginti pay pe (koi reserve nahi, atomic)
  Inventory     1   Buy dabaya, battery khatam, pay hua nahi       ->  cart me hai, kharida nahi; ginti pay pe, jisne pehle pay kiya uska (koi reserve nahi)
  E-commerce    7d  andar gaya, chai peene chala gaya              ->  "ya pay kar ya rakh de": 5 min ghadi, phir wapas line me (expiry + INCR)
  E-commerce    7e  ghadi khatam ke 1 sec baad Pay                 ->  1-2 sec upar bas, phir Priya ka; andar dabaya to PSP hamari zimmedari (grace period, PAYMENT_PENDING)
  Rate limiter  9b  OTP line me atka, der se nikla, bekaar         ->  har OTP pe umar, khatam to phenk do (message TTL)
  Notification  4b  OTP retry line se 3 ghante baad gaya           ->  OTP umar ke andar priority line (bhari to 3-4); umar khatam = bhejo mat, app pe Resend (priority queue, expiry)

Naam: TTL (umar) / hold expiry / heartbeat / grace period
Juda: single flight ka ladka gira to bhi yahi umar (6). Chat ka "online" bhi isi se (heartbeat).
```

## 3. Ek hi kaam do baar aa gaya  (DOBARA)

```
Dikkat:      net gaya, app ne dobara bheja, ya worker / consumer ne commit se pehle crash kiya, to wahi kaam do baar: do SMS, do baar paisa kata, 20 share.
Humne socha: parchi zindabaad: ho gaya likh lo. Same chaabi dobara aaye to dekh ke mana kar do.

Yahi dikkat yahan bhi:
  Notification  3   commit se pehle crash, SMS do baar             ->  parchi zindabaad, ho gaya likh lo (idempotency)
  Stock         2   net gaya, app ne dobara bheja, 20 share        ->  chaabi ka taala khareed liya (idempotency key)
  Payment       5   dobara bhejna pada to do baar kate?            ->  idempotency key laga do (payment id)
  Payment       5b  net atka, Pay do baar daba diya                ->  phone ki key, wahi parchi lauta do (client idempotency key)
  Uber          5b  net atka, Book do baar daba diya               ->  wahi booking id, wahi ride (idempotency key)
  Uber          13b 250 kaatne gaye, gateway chup                  ->  ride id hi key, pehle poochho phir dobara (idempotency)
  E-commerce    8d  PSP ka success do baar aaya                    ->  payment id dekh ke doosra chhodo (idempotency)
  E-commerce    7c  Ravi ne 4 tab khole, Buy now 4 baar            ->  user id pe ek hi rukega (idempotency)
  Leaderboard   3b  match ka +50 do baar pahuncha                  ->  match id dekh ke ek hi baar jodo (idempotency)
  Chat          6b  Ravi ka "mil gaya" kho gaya, dobara aaya       ->  Ravi ka phone message ka number dekhe, dobara mat dikhao (idempotency)
  Docs          1b  "budget 20k" ka op dobara gaya                 ->  op id dekh ke chhodo (idempotency)
  File upload   4c  S3 ne "aa gayi" do baar / ulta bataya          ->  parchi, ho gaya to chhod; apna thappa, purana phenko (idempotency, sequencer / version)
  News          3   ek khabar 40 baar                              ->  bada aadmi, "ye dobara hai, hatao" (dedup)
  News          3   shabd alag, khabar same                        ->  wordings dekh lo (near-duplicate check)
  Kafka         6   checker punch ke baad, register se pehle gira  ->  pehle aaya hai? check kar le (idempotency)
  Kafka         6b  #501 ka SMS gaya, bookmark se pehle gira       ->  pakki id, wahi id dobara aaye to discard (idempotent consumer)
  Banking       2   receipt se pehle line kati, wahi parchi        ->  log hamare paas, dekh ke mana (pehla record)
  Banking       2b  agle din sach me phir 500 bhejne               ->  key jo badle hi na, same parchi (idempotency key)
  Chat          6   lift me jawab kho gaya, "hi hi"                ->  ek key, same aaye to mana (clientMsgId)
  Chat          5c  late chhotu bhi aaya, #981 500 ko do baar      ->  parchi + register pe DONE, ek transaction (outbox status)
  Chat          5d  do chhotu ek hi pal me, dono bhejne nikle      ->  bahar wala aadmi: "ye to chala gaya", parchi phaado (dedup fan-out, message id)

Naam: idempotency key / dedup / idempotent consumer
Juda: dobara aata hi isliye ki beech me koi gira (4). Kafka ka retry bhi dobara bhejta, isliye har consumer pe yahi.
```

## 4. Beech me gira, kaam kho gaya  (VASIYAT)

```
Dikkat:      DB me likh diya, aage bhejne se pehle gira; ya paisa kata aur bijli gayi. Aadha kaam hua, aadha kho gaya.
Humne socha: jo yaad nahi rehta, likh lo. Order ke saath vasiyat likh do, peeche wala banda padh ke aage bhejta rahe. Aadha hua to ulta kar do.

Yahi dikkat yahan bhi:
  Twitter       4   3,000 pe server gira, 5,000 ko nahi mila       ->  parchi pe likha, Kafka hi pakki parchi (acks=all)
  Notification  1b  DB me order, Kafka se pehle gira               ->  order ke saath vasiyat likh do (outbox + relay)
  Stock         5   adhoora order RAM ke saath uda                 ->  bag ki parchi dekh ke dobara jamao (order log, replay)
  Stock         5b  replay me ho chuke trade bhi wapas aaye         ->  mota register dekho, ho chuke hatao (trade DB = source of truth)
  Payment       5   INITIATED likha, call bhejte server gira       ->  pehle register me likho, phir bhejo (commit first)
  File upload   4b  UPLOADED likha, scan daalne se pehle gira      ->  UPLOADED ke saath vasiyat (outbox + relay)
  News          11  DB me likhi, parchi se pehle worker / consumer mara       ->  status rakh, agla aage badhaaye (state, outbox)
  Kafka         5   admin ne "aa gaya" bola, phir gira             ->  vasiyat ki copy bhi likhe, tab "likh di" (acks=all)
  Kafka         7   Order Service gate tak bhejne se pehle gira    ->  baap ki vasiyat, pehle likh de (outbox)
  Banking       1   500 kate, bijli gayi, Ravi ko nahi mila        ->  mere 500 wapas de do (rollback)
  Banking       5   khabar dene se pehle bijli gayi                ->  jo yaad nahi rehta, likh lo (outbox, relay)
  Chat          2   Ravi ka phone band, 2 ghante                   ->  uncle nahi mile, paise apne paas (message store)
  Chat          5b  #981 DB me, pub/sub se pehle server gira       ->  extra chhotu + deewar pe parchi (backup, outbox + relay)

Naam: outbox (hamare DB ki table) + relay / acks=all / rollback / order log + replay
Juda: vasiyat padh ke dobara bheja to (3) chahiye. Notification, File upload, Kafka, Banking, News sab me same.
```

## 5. Baar-baar mote register tak bhaagna  (PARCHI)

```
Dikkat:      har padhai ya har chhoti likhai DB tak jaati: DB dheema, bheed lagi, row pe lock.
Humne socha: DB se pehle ek chhota dabba. Padhna ho to pehle parchi. Likhna bahut ho to pehle parchi pe dhadadhad, chhotu baad me batch me mote register me.

Yahi dikkat yahan bhi:
  URL shortener 3   1 lakh/sec, counter wala register palta        ->  chhota pehla register (Redis)
  URL shortener 4   chhota register bhar gaya                      ->  itni der rahega / jo poochha nahi (TTL, LRU)
  URL shortener 11b counter wala har baar uth ke register jaata    ->  apne paas parchi rakh le (local cache)
  Stock         9   9:15 lakhon order, book DB me, line atki       ->  chhoti parchi pe dhadadhad, baad me mota register (book RAM, batch, copy)
  News          6   Priya pehli baar aayi                          ->  pehli baar DB, phir parchi (cache-aside)
  Cache         1   DB 50 ms, Ravi ka profile hazaron baar         ->  DB se pehle ek dabba (cache, Redis)
  Cache         2   dabba bhar gaya, purane profile pade           ->  jo purana, kam use, nikaal do (LRU)
  Docs          2   laakhon save, har badlaav DB pe                ->  chhota register, kuch der baad DB (Redis buffer, batch)
  Netflix       5   TV pe 27 min, phone pe wahi se                 ->  account pe parchi (watch position DB me)
  Netflix       5c  har kuch second update, DB ki saans            ->  race me ruk ke likho (pehle cache, DB kabhi-kabhi)
  YouTube       3   1 lakh view, ek row ke lock pe line            ->  +1 parchi pe, chhotu batch me jode (Kafka, batch count)
  Inventory     5   1 crore sirf page dekh rahe, har baar DB jod   ->  bahar banner / chhotu ki copy, asli jaanch Pay pe (cache, Redis)
  Zomato        0   menu har baar DB se                             ->  menu Redis me, menu badle to job cache update kare (cache + write pe update)
  Zomato        1   Whitefield me sabko wahi 15 km list, 4 baar DB ->  area ki list ek dabbe me, TTL, band = closed nishaan (area list cache)
  Zomato        2   Neha kinare pe, Whitefield bahut bada           ->  area ke andar chhote area, har ek ki list (grid / geohash cell)

Naam: cache (Redis) / TTL + LRU / local cache / write buffer + batch
Juda: parchi bhar gayi to purana nikaalo; parchi aur DB alag ho sakte (20).
```

## 6. Parchi khaali, sab ek saath DB pe  (CHHOTA LADKA)

```
Dikkat:      7 baje 1000 log ek saath aaye, parchi me nahi mila, sab mote register pe tut pade.
Humne socha: ek chhota ladka bhaag ke jaaye, baaki yahin ruke. Ladka raaste me gira to kuch der dekho, phir bheed me se ek.

Yahi dikkat yahan bhi:
  URL shortener 5   7 baje 1000 log ek saath mote register pe      ->  chhota ladka bhaag ke ja (single flight)
  URL shortener 6   ladka raste me gira, 999 khade                 ->  ladke ki timeline, 999 me se ek (lock TTL)
  Cache         6   Priya viral, 10,000 miss, sab DB pe            ->  ek chhotu bhejo, baaki yahin (single flight)
  Cache         6c  chhotu dheeme DB pe atka, 9,999 ruke           ->  dikkat DB ki: padhne ki copy, dheemi chhod doosri pe (read replica, timeout)
  Cache         6c  saari copy dheemi                              ->  thodi der purani dikhao, local cache + chhoti umar (serve stale, short TTL)
  Cache         6   chhotu raaste me gira                          ->  kuch second dekho, phir bheed me se ek (lock timeout)
  Cache         6   aur raaste?                                    ->  do chhotu bhejo, hot key lambi umar, expire alag-alag (redundant fetch, TTL jitter)
  Cache         6b  bot nakli id maange, har baar DB               ->  parchi pe range + "500 = null" bhi parchi pe (cache penetration, negative caching)
  Cache         7c  Priya roz garam, machine 8 pe 8 baje umar khatam ->  nazar, umar badhao, copies ki umar alag (TTL extend, TTL jitter)
  Cache         7d  chhotu gira, purani parchi to mit gayi         ->  chupchaap doosri parchi, 10 chhotu na aayein tab backoff (stale copy, backoff)
  Cache         7e  50 server ke 50 chhotu mote register pe         ->  saamne ek banda, sirf ek chhotu (distributed lock)
  URL shortener 11c cricket token, photocopy 7 pe umar khatam      ->  umar badhao, photocopy umar alag, chupchaap doosri parchi, 10 ladke na aayein tab backoff (TTL extend, TTL jitter, stale copy, backoff)
  URL shortener 11d 50 counter ke 50 ladke                         ->  saamne ek banda, sirf ek ladka (distributed lock)

Naam: single flight (lock) / lock TTL / cache penetration / TTL jitter / stale copy
Juda: ladke ki timeline = parchi ki umar (2). URL aur Cache dono me same.
```

## 7. Ek hi jagah pe saari bheed  (EK AUR GAADI)

```
Dikkat:      viral cheez, ek hi key / ek hi row / ek hi counter pe crore log; baaki machine khaali baithi.
Humne socha: us ek ki photocopy bana do, ek aur gaadi; ya counter tod do (10 counter); khaali ladke laga pada wale ki madad karein.

Yahi dikkat yahan bhi:
  URL shortener 11  viral token, 1 crore log ek hi register pe     ->  register ki photocopy, sab me baant do (Redis replica)
  URL shortener 11e IPL link baaki links ko atka raha              ->  server ke paas + uska alag dabba + copy, ring me gira to agli copy; consistent hashing nahi (local cache, hot key isolation)
  Twitter       5   six pe 5 crore, parchi wali machine hili       ->  10-15 copy bana do, baant do (hot key replica)
  News          9   iPhone launch, 10 lakh/sec, dabba 1 lakh       ->  tech ke dabbe badha do (aur Redis node + tech ki copy)
  News          9   cricket wale dabbe khaali pade                 ->  unme bhi tech ki copy (hot key copies)
  Cache         7   1 crore log sirf machine 4 pe                  ->  machine 4 ka replica, ek aur gaadi (hot key copies)
  Banking       17  9 chhotu khaali, ek laga pada                  ->  khaali ladke madad karo (hot key, work stealing)
  Banking       6   salary ka din, ek cashier, line sadak tak      ->  cashier badha do, 10 cashier, 10 counter (kai machine + LB)
  Uber          6b  stadium chhuta, 15,000 booking ek dabbe pe      ->  2-3 dabbe extra, hotspot pe pehle se, had paar to apne aap (hot cell split, pre-scaling)
  E-commerce    4   ek parchi pe 50 hazaar                         ->  10 counter, tez kaatne wala (hot row todo, Redis DECR)
  E-commerce    5   counter 3 khatam, counter 7 pe 30              ->  khaali counter band, sold out tab jab sab 0
  Inventory     2   sale, 2 lakh Pay ek row pe, taala             ->  1000 ko 5 dabbe, har counter apna dabba (bucketing / sharded counter)
  Inventory     2b  dabba 3 khaali, baaki me 150                   ->  request doosre dabbe pe mod do, out of stock tab jab sab 0
  Inventory     2c  sab dabbe 0, ek cancel, iPhone wapas           ->  stock me +1, kisi dabbe ka nahi; jo pehle pay kare uska (restock)
  News          5   9 baje 1 crore log ek saath                    ->  bahut saare server + ek banda jo sabko manage kare (load balancer)

Naam: hot key copies / replica / sharded counter / kai machine + LB
Juda: machine badhane se nahi hota, kyunki ek key ek hi machine pe (8).
```

## 8. Ek machine me nahi aata  (BAANTO + RING)

```
Dikkat:      TB-TB data, ya lakhon likhai, ek machine / ek register me nahi samaata.
Humne socha: tod do, tukde baanto, hash se pata kis almari me. Machine jude to sab na hile: ring, kho-kho, paas ka petrol pump.
             Jahan RING, wahan CONSISTENT HASHING: Cassandra, DynamoDB, cache ki machines, Docs ka owner server, sab andar yahi.

Yahi dikkat yahan bhi:
  URL shortener 8   90 TB ek almari me nahi aata                   ->  tukde, hash se almari, same aaye to usi tukde pe (shard)
  URL shortener 8b  almari phir kam padi, 10vi lagani              ->  Cassandra khud baant deta, uska andar ka kaam (consistent hashing)
  URL shortener 8c  register state ke hisaab se, Mumbai se khula   ->  token me state code, bada bhai list se bheje (shard id in key, router)
  Twitter       7   din me 50 crore tweet, likhai ki line          ->  bada bhai, "tu dekh le" (Cassandra, andar ring = consistent hashing)
  Twitter       7   "Arpan ke naye 500" poori table me             ->  naam se nahi, id se baanto (partition key = user id)
  Payment       9   Diwali 3,000/sec, ek register                  ->  register ke chhote tukde (shard)
  Payment       10  wallet tukda 1 me, dukaan tukda 3 me           ->  ladka diary le ke jaaye (saga do shard ke beech)
  Cache         3   1 TB ek machine me nahi aata                   ->  tod do, hash se same dabba (sharding)
  Cache         4   11vi machine judi, har key ki jagah badli      ->  kho-kho, sabse paas wala, paas ka petrol pump (consistent hashing)
  Cache         4b  key 47: peeche 45 paas, aage 52                ->  highway pe U-turn nahi, aage wale pump pe (clockwise, ring ek disha)
  Cache         4c  bhare dabbe alag kamre me kyun nahi?           ->  bank locker jaisa ek-ek nahi, ring me sab ek saath bharte (hash se barabar baant)
  Cache         4d  ring ke saare dabbe phir bhar gaye             ->  aur dabbe jodte jao, ring kamra nahi niyam hai (scale out)
  Docs          4   lakhon doc, sab ek hi jagah?                   ->  har doc ka alag, uska maalik (owner server)
  Banking       11  5 saal ki line roz jodna bhaari                ->  mahine ke mahine tod do (partition, snapshot)
  Uber          2   1.25 lakh update/sec, DB ghuta                 ->  bada bhai bulao (Cassandra, andar ring = consistent hashing)
  Uber          2b  har location DB me nayi line, map dheema       ->  abhi ki jagah parchi (Redis overwrite), raasta Kafka -> batch storage, DB me sirf status
  Uber          3   5 lakh driver ka fasla naap raha               ->  naksha dabbon me, sirf apna dabba (geohash)
  Yelp          1   "burger" pe 8 sec, "burgr" pe zero             ->  aas-paas ke restaurant + Elasticsearch, galat spelling pe bhi (geo filter, fuzzy)
  News          13  "Kohli century" pe 8 sec gola                  ->  Elasticsearch, alag headline ki jad ek (search index, analyzer)
  Uber          4   dabbe ke kinare, bagal me driver               ->  8 km ka gola, chhoote dabbe bhi (neighbour geohash)
  Uber          6c  dabba toda, Arpan ke tukde me driver nahi       ->  papa ji: apna + upar / neeche / bagal ke tukde (scatter-gather, neighbour cells)
  Uber          4b  map pe paas Raju, par nadi ke us paar           ->  sadak se kitni door, kitni der (road distance / ETA)
  Uber          9   register driver ke naam se, 5 lakh line        ->  jagah ke hisaab se panna (geohash ka Redis set)
  Uber          9   dabba badla, purana naam pada                  ->  nikla to hata do, naye me daal do (remove + add)
  Leaderboard   3   lakhon update ek Redis pe                      ->  region se baant do, chhotu mila ke badi screen (shard, scatter-gather)
  Leaderboard   4   India me 3,000, duniya ka rank                 ->  race wala: har line me mere upar kitne, jodo (count)
  Leaderboard   6   mere aage-peeche ke 5, alag dabbon me          ->  bheed me aas-paas scan, paas ka petrol pump (score range, merge)
  Leaderboard   6b  region me baante, world ke aas-paas ke 5 + 5      ->  FR: poora world board; UTC, pehle sawaal gino, phir waqt, ms, naam; rank upar se dheere tay (Happy New Year board, rassi) (global board, sort key, eventual)
  Leaderboard   6c  chalte contest me aage-peeche 10               ->  dabbe score range se, coordinator ka thappa, seedha sahi dabba; andaze wali rank, galat jawab bhi ginte (range partition, shard map, penalty)
  Leaderboard   6d  1 crore contest aur tez kaise                  ->  availability > consistency; sign-up se pehle pata kitne, desh ko jagah baanto (rank nahi), shehar ke gole + range ka thappa (pre-partition, geo routing, AP)

Naam: shard / partition key / consistent hashing / scatter-gather
Juda: URL 8 aur Cache 4 dono me ring wahi (Cassandra andar khud karta). Bheed ek tukde pe ho to (7).
```

## 9. Machine giri, ya copy peeche  (COPY)

```
Dikkat:      ek hi dabba tha aur wo gira; ya copy se padha aur wahan abhi aaya hi nahi.
Humne socha: ek pe bharosa nahi: copy bana do, do me likho, do se padho. Jisne abhi likha, thodi der main file se padhe.

Yahi dikkat yahan bhi:
  URL shortener 9   almari me aag                                  ->  copy + acknowledge (quorum)
  URL shortener 9b  Delhi me aag, copy peeche chale                ->  copy alag jagah, authority pehle likhe phir bheje, uski bhi copy (coordinator, async copy)
  URL shortener 10  C se padha, nahi mila                          ->  do se padho (R=2)
  Rate limiter  6   Redis gira, sabko jaane de ya roke             ->  dono khule (OTP pe kisi ka bank transfer), bouncer 5-5 karke bheje (fail-open + local fallback limiter)
  Twitter       13  worker / consumer 7 gira, "processing" ghanton se         ->  "tu hai kya?", jawab nahi to naya banda (heartbeat)
  Twitter       13b worker / consumer zinda, 1 ghante se ek hissa bana raha   ->  "tu thoda rest le", replacement wahin se kare (timeout, checkpoint)
  Twitter       13c zinda par kaam kahan tak? 50 ka chunk            ->  "kahan tak hua" poochho, 2-3 chhotu madad, parchi dekh ke 31 se (progress, work stealing, checkpoint)
  Stock         5   book RAM me, machine mar gayi                  ->  permanent dabbe me rakho (DB)
  Stock         5   permanent dabba bhi gira to                    ->  3-4 copy bana ke rakho (replica)
  News          10  A me khabar, B se refresh pe gayab             ->  do me likho, do se padho (quorum)
  BookMyShow    4   sirf dekhne wale bhi bojh daal rahe            ->  dekhne wale ki alag diary (read replica)
  Docs          5   "Saved" dikhaya, DB se pehle server gira       ->  dabbe ki ek aur copy (Redis copy)
  Docs          5b  dabba + copy dono gire, batch baaki            ->  disk pe bhi likho; phir bhi gire to kuch second chhoot, 500 copy ka paisa (AOF, trade-off)
  Docs          5c  poore data center ki bijli gayi                 ->  copy doosre shehar (Mumbai + Delhi) (multi-region replica)
  Docs          5   owner server hi gira                           ->  "mujhe kuch ho to tu hai" (naya owner wahin se)
  Banking       7   register jal gaya                              ->  laptop me save, ghar le jaao (replica, Multi-AZ)
  Banking       7b  2 baje likha, 3 baje aag                       ->  turant laptop pe, tabhi pakka (sync copy)
  Banking       8   sab balance dekhne aaye, kaam atka             ->  photocopy alag desk pe (read replica)
  Banking       9   photocopy peeche, ghabra ke dobara bheja       ->  jisne likha, thodi der main file se; photocopy bataye kahan tak pahunchi (read-your-writes, replication lag)
  Banking       20  2 baje chhotu ne register gher liya            ->  chhotu photocopy se padhein (read replica)
  Leaderboard   7   Redis gira                                     ->  clone, phir clone, teeno gaye to DB se bharo (replica, DB sach)
  News          2b  chhotu 7 ki 100 site, wo mara                  ->  "main zinda hoon" band to site baaki chhotuon me (heartbeat)
  News          2c  chhotu 7 ka net 20 sec atka, site chhotu 12 ko ->  thodi der ruko; purana laute to malik nahi, leader jaisa (lease timeout, fencing)
  Twitter       5b  "Inida" se "India", kuch copy purana dikhaye   ->  thodi der galat, peeche ek ladka sabko theek kare (eventual consistency)
  News          13  feed me dikhi, search me 2 min baad            ->  dono me ek saath bharo ya peeche-peeche, fail pe alert (dual write, async index)

Naam: replica / sync copy / read-your-writes / replication lag / failover
Juda: sirf wahi "maan lo gira" jahan HUM faisla lete (sync ya async, fail-open), AWS wala nahi.
```

## 10. Kaam lamba, user ko mat roko  (LINE LAGWAO)

```
Dikkat:      button dabaya aur server khud sab kar raha (SMS, scan, fanout), user 8-10 second ghoomta dekh raha.
Humne socha: stadium ke bahar bheed: admin line banaye, ek-ek karke. User ko "ho gaya, nikal", kaam peeche chhotu kare.

Yahi dikkat yahan bhi:
  Rate limiter  9   result ka din 1 crore OTP, SMS company giri    ->  line lagwa, ek-ek karke (queue, Kafka)
  Twitter       4   8,000 dabbe, Post ka button ghoome             ->  stadium wala bhai, "post ho gaya, nikal" (Kafka)
  Notification  1   order khud SMS call kare, button 8 sec         ->  beech me dabba, aaram se bhejunga (queue, Kafka)
  Notification  2   ek hi line, bill aadhe ghante baad             ->  ladies, gents, bachchon ki alag line (topic, group)
  Notification  5   OTP bhi baaki ke peeche line me                ->  VIP darshan, OTP ki priority (alag OTP topic)
  Notification  8b  5 crore promo ke peeche OTP, 40 min baad       ->  hospital jaisa, serious ki line alag (high-priority topic)
  Stock         6   sauda pakka, engine khud sabko call kare       ->  10 ladkon ki fauj (Kafka, consumer group)
  File upload   1   server khud sab kare, 10 min spinner           ->  chai pi aao, peeche line (Kafka + worker / consumer)
  File upload   8   scan baaki kaam ke peeche atka                 ->  doctor line me nahi lagta (alag scan topic)
  File upload   5b  folder delete, S3 me 500 photo padi            ->  DB me turant hatao, S3 peeche: event ya sync job (soft delete, async delete, orphan cleanup)
  News          4   bada aadmi akela, 1 min ek khabar              ->  stadium wali kahani, chhotu daal ke chale (Kafka)
  Kafka         1   stadium ke bahar bheed, sab ghus rahe          ->  admin: line banao, ek-ek karke (Kafka)
  Kafka         3   ek admin se nahi sambhal rahi                  ->  aur gate, har gate ki apni line (partition)
  Kafka         4   har ticket 3 baar punch                        ->  ek kamre me bitha do, 3 me se ek (consumer group)
  Kafka         4b  security wale bhi usi kamre me                 ->  alag room (alag consumer group)
  Kafka         4c  checker ka scanner hang, talaashi bhi ruki     ->  pehle ticket phir talaashi = nayi line (ticket_ok); 2-3 scanner (zyada consumer)
  Kafka         10  7 din peeche, purana panna phat jaata          ->  aur admin daalo, itna peeche rehne hi mat do (consumer, phir partition)
  Kafka         10b aaj 1 din, kal 2 din peeche                    ->  alert laga do (consumer lag alert)
  PUBG          5   chicken dinner pe lakhon likhai, DB ghuta      ->  match me parchi, end pe line + copy, dheere DB (batch, Kafka, Cassandra)
  Inventory     7   5 se kam, ab kya: sirf email?           ->  saari inventory dekho, grahak mat roko, alert kai raaston se (line me); mangwana purchase team ka (scope se bahar)
  Zomato        6   Diwali, 500 order seedha Postgres pe            ->  "order placed" dikhao, restaurant-wise Kafka line, worker peeche (queue + async)

Naam: Kafka (queue) / consumer group / partition / alag (priority) topic
Juda: line me se gira to (4) aur (3). Line peeche reh gayi to alert.
```

## 11. Bahar wali company chup ya band  (PSP / SMS)

```
Dikkat:      company hamari nahi (PSP, SMS): chup ho gayi, dheemi, band, ya "abhi bheed hai".
Humne socha: apne paas parchi (PENDING), beech me banda poochta rahe. Thodi der ruk ke dobara; band ho to doosri company; baar-baar fail to traffic mod do. Paisa ulta karna ho to ek-ek karke wapas.

Yahi dikkat yahan bhi:
  Notification  4   company bole "abhi bheed hai"                  ->  papa busy, thodi der ruk ke (retry queue)
  Notification  5   SMS company poori band                         ->  teen-chaar company extra (failover)
  Notification  5b  company 30 sec chup (timeout)                  ->  OTP turant backup se, 2 OTP chalega, ek hi code; sale ruk sakti (SLA)
  Notification  5c  company chup, lakhon worker / consumer har second dobara  ->  1, 2, 5 sec ruk ke + aage-peeche; 2-3 company, dheeme ko kam kaam (backoff, jitter, weighted routing)
  Notification  5d  OTP ki receipt nahi aayi, umar 2 min           ->  umar ke saath bhejo; DB me record, naya OTP ya wahi nayi umar se (TTL, regenerate / resend)
  Notification  5   fail pe fail, company band kaise samjhun       ->  2-3 baar fail to traffic backup se (circuit OPEN)
  Notification  5   main company wapas theek hui?                  ->  beech-beech me "tu theek hai?" (HALF-OPEN)
  Notification  5   backup bhi same fail bole                      ->  number galat, alag daal do (DLQ)
  Notification  6   20 worker / consumer 300/sec, company ki had 100          ->  saanjha dabba, sab usi se guzrein (token bucket, Redis)
  Notification  7   company ne "OK" bola, SMS pahuncha nahi        ->  "le liya", baad me sent ya fail bataye (webhook)
  Notification  7b  le liya bola, DELIVERED kabhi aaya hi nahi     ->  cron job chhotu poochhe, na mile to FAILED + backup (status poll)
  Stock         7   portfolio DB band, sauda ulta kar do?          ->  rollback nahi, consumer court (settlement PENDING)
  Stock         7   paisa kata, share dikh nahi rahe               ->  parchi apne paas, DB aaye to de do (Kafka retry)
  Payment       1   HDFC hamari sunta hi nahi                      ->  centralized authority, uski API (PSP)
  Payment       2   PSP 30 sec chup, FAILED ya SUCCESS?            ->  apne paas parchi, INITIATED (payment status)
  Payment       3   PSP batana bhool gaya, message khoya           ->  beech me banda jo batata rahe: hua ya nahi (status job)
  Payment       3b  PSP ka message raste me kho gaya               ->  paisa katte hi chhota event, S3 jaisa; "mil gaya" (webhook + ack)
  Payment       4   PSP-1 poora band, payment ruke                 ->  PSP-2 pe bhej do (fallback PSP)
  Payment       6   wallet se kata, ICICI ne FAILED bola           ->  sab ulta kar do, paise wapas (saga)
  Payment       8   "sab taiyaar ho?" poochh ke kaato              ->  slow dost ki wajah se mera paper chhoota (2PC nahi)
  Payment       11  PSP-1 chup, mara ya dheema pata nahi           ->  "process ho raha"; chhotu, itna waqt, haan ya naa; pata ho down tabhi PSP-2 (PENDING, timeout, refund)
  Payment       11b PSP-1 chup, PSP-2 pe bheja to do baar katega?  ->  30-55 sec phir FAILED + PSP-2; PSP-1 pe bhi kata to wo lautaye (timeout, failover, refund)
  BookMyShow    6a  Pay daba diya, PSP dheema, umar khatam         ->  parchi pe "pay chal raha", umar badhao (PAYMENT_PENDING)
  BookMyShow    6b  chhotu (PSP) 1-2 baar jagaya, phir bhi chup    ->  ~1 min (30-55 + buffer), UI "process ho raha"; andar kata = BOOKED, baad me = fail + popup "dobara pay?" (Haan = PSP-2 QR, Naa = seat turant khali, TTL nahi badhana), der se kata = refund
  BookMyShow    6b  dheeme chhotu ne peeche se paisa kaat liya     ->  id hamare paas; "kata ho to wapas aayega", kata nikla to wapas (reconciliation job + refund)
  BookMyShow    6c  back dabaya, seat chhodi, phir paisa kata      ->  30-55 sec gola, phir FAILED + doosre PSP pe; kata nikla to Razorpay lautaye (timeout, failover, refund)
  BookMyShow    6d  SUCCESS likha, ticket bolne se pehle gira      ->  SUCCESS + parchi "D5 book karo" ek saath (transactional outbox)
  BookMyShow    6e  bank 12 min, seat Priya ki, paisa kata         ->  chhotu poochta; DB me REFUND, raat ki job lautaye (status poll, refund job)
  BookMyShow    6f  purana PENDING naye SUCCESS pe likh gaya       ->  sach ek jagah (DB), likhne se pehle dekho, sirf aage (source of truth, conditional update)
  BookMyShow    6f  SUCCESS pehle, PENDING baad me / kho gaya      ->  number lagao, 2 ruke jab tak 1; 1 na aaye to Razorpay se poochho (sequence, status poll)
  BookMyShow    6g  "refund shuru" ko "ho gaya" likha              ->  pakka jawab tak REFUND_PENDING, 5 din purane poochho (refund state, stuck refund job)
  Kafka         8   galat ticket baar-baar, line atki              ->  line se hata (DLQ)
  Kafka         9   SMS counter 10 min band, sab DLQ me            ->  pehle check, waqai band? (circuit breaker)
  Kafka         9   counter waqai band hai                         ->  road band, traffic divert (fallback)
  Uber          13  payment fail, Arpan gaadi me, Raju saamne      ->  paisa nahi aaya to ride khatam nahi (PAYMENT_PENDING)
  E-commerce    8   net gaya / PSP chup                            ->  BMS wala PAYMENT_PENDING + status job, kinaare pe refund
  File upload   8b  bahar wala scanner 1 ghante se chup            ->  ruk ke dobara, phir insaan ko alert; tab tak link band (retry + backoff, DLQ)
  News          6   jis site se khabar la rahe wo band             ->  5 sec, 10 sec baad dobara; na khule to chhodo (retry + backoff)
  E-commerce    8b  PSP 50 sec chup, kata pata nahi                ->  hamari ghadi 30-50 sec, phir "time out, kata to wapas", dobara pay doosre PSP pe; kata nikla to REFUND_PENDING (timeout, PSP failover, refund job)
  E-commerce    8e  payment fail, cart ka kya                      ->  cart khaali nahi (sirf SUCCESS pe), dobara pay: doosra PSP / net banking / COD (payment fallback)
  E-commerce    8c  PSP ka "kata" kaise turant pata chale          ->  paisa katte hi chhota event, S3 jaisa; badi sale pe 3-4 company (webhook, backup PSP)
  E-commerce    9   sale pe cart ki almari B, C giri, Add to Cart  ->  ring pe aage wali chalti almari pe likho (sloppy quorum, Dynamo)
  E-commerce    9b  B, C wapas, iPhone D, E pe                    ->  D ka chhotu lauta de + sync job (hinted handoff, anti-entropy)
  Zomato        5   Burger Singh chup, na accept na reject          ->  2-3 min window, phir order fail, paisa nahi kata / wapas (accept timeout)
  Zomato        8   address khaali order, saari line atki (reel)    ->  3 retry, phir alag line + alert (retry limit + DLQ, poison message)
  Uber          13c gateway ka jawab aaye hi na, kab tak rokoge?   ->  30-55 sec ghadi, phir GPay / cash; kata nikla to lautao (timeout, webhook, refund)

Naam: status poll / webhook / timeout + refund / retry + backoff / circuit breaker / reconciliation job
Juda: Payment, BMS, E-com, Uber me wahi PAYMENT_PENDING. Notification aur Kafka me wahi circuit breaker. PSP pe hamari ghadi 30-55 sec + chhotu poochta (status poll) + paisa katte hi event (webhook) + der se kata to DB me REFUND, raat ki job lautaye, charon me (BMS 6c, 6e); SUCCESS + parchi ek saath (outbox, BMS 6d); onboard pe SLA sirf uptime ka.
```

## 12. Phone baar-baar "kuch aaya?"  (KHULA TAAR)

```
Dikkat:      phone har second poochh raha, ya 10 lakh phone ek hi cheez dekh rahe.
Humne socha: poochne ka mauka hi mat do. Taar khula rakho, hum khud batayenge. Doosre server pe hai to chhota ladka address le ke jaaye.

Yahi dikkat yahan bhi:
  Stock         8   bhaav har second, 10 lakh screen               ->  chat wala hi (WebSocket + Redis pub/sub)
  Docs          6   Ravi doosre server se likh raha                ->  building ke bahar chilla de (pub/sub invalidation)
  Docs          7   server 7 ne awaaz suni hi nahi                 ->  copy note kar le, wapas aake dekh (standby, reconnect)
  Chat          1   phone har second "kuch aaya?"                  ->  poochne ka mauka hi na do (WebSocket)
  Chat          3   100 server, Ravi kis pe                        ->  DSA wala map, O(1) (session map, Redis)
  Chat          4   har server baaki 99 se taar jode?              ->  chhota ladka address le ke jaaye (Redis pub/sub)
  Chat          12c group ke 500, har message pe 500 dhoondho?     ->  har taar-server ko ek baar, aage wo de (per-server fan-out)
  Uber          1   server ko pata nahi driver kahan               ->  phone khud bheje, ek dabba (Location Service)
  Uber          10  10 lakh phone "Raju kahan?"                    ->  poochne ka mauka mat do (push, WebSocket)
  Uber          11  Arpan server 12 pe, Raju 57 pe                 ->  ghar ka raasta pata, seedha usi raaste (pub/sub)
  Netflix       5d  tukde ISP ke dabbe se, AWS ko kaise pata ruka  ->  dost sankshep me bata diya kar; kuch nahi bataya = aage nahi badha (playback report)
  PUBG          2   100 logon ko 99-99 ki khabar, har second       ->  khula taar, phone mangwa nahi raha, khabar apne aap (WebSocket / UDP push)

Naam: WebSocket / Redis pub/sub / session map
Juda: Chat, Stock, Uber, Docs, sab me ek hi cheez.
```

## 13. Pehle kya aaya, kram gadbad  (NUMBER)

```
Dikkat:      ghadiyan aage-peeche, server dheema, "nahi, parso" pehle pahuncha; do log ek saath likh rahe.
Humne socha: phone pe bharosa nahi, server number lagaye, phone us kram me lagaye. Ek doc ka ek maalik, sab uske hisaab se.

Yahi dikkat yahan bhi:
  Docs          3   "Hum" aur "kal", kal galat jagah               ->  shabdon ko khiska do (OT)
  Docs          3b  Ravi ne B hataya, Arpan ne E joda              ->  dono ke doc same: A E C D (OT)
  Docs          3c  "jo pehle aaya taala laga de"                  ->  phir sharing doc ka matlab hi nahi, OT se sab saath (locking nahi)
  Docs          4   do server, ek doc ki do shakal                 ->  ek centralized system, ek version
  Kafka         2   line me ek aadmi nikla, sab atke               ->  seat booked, wahi baithega (offset)
  Chat          7   "kahan?" pehle dikha, ghadiyan aage-peeche     ->  server lagayega number (sequence)
  Chat          7b  #980 atka, #981 pehle, phone "981 ke baad do"  ->  server ka record, jo nahi gaya sab do; phone sort kare (store-and-forward)
  Chat          12  S1 dheema, "nahi, parso" pehle pahuncha        ->  server ka number, phone us kram me lagaye (seq, client sort)
  Chat          12b number kaun lagayega, kabhi khatam to nahi?    ->  us time ko hi number, group ke server ki ghadi; daava nahi (Snowflake ID)
  Chat          5e  #980 wala chai peene gaya, #981 pehle gaya     ->  jo pehle aaya wahi pehle (sequence number, phone sort)
  Leaderboard   2   Ravi, Priya dono 9800                          ->  score phir time, ek number me (composite score)
  Twitter       18  scroll ke beech upar 5 naye, wahi 5 dobara     ->  kahan tak padha yaad rakh (cursor, offset nahi)
  Yelp          4   300 m wali 4.6 star 15ve number pe             ->  sort user chune, server saare pe kare, phone ko pehle 10 (server-side sort, pagination)
  Notification  4c  Shipped jaane wala, Order placed atka          ->  order ki parchi, gaya nahi to dono saath / doosri company / email (coalesce, failover)
  Kafka         8b  pay DLQ me, cancel chal gayi, kram toota       ->  A2 ruka to A3 bhi ruka, theek hone pe saath dobara (same key parking)
  Kafka         3b  #A7 ka paid aur shipped alag partition me      ->  key se ek partition, line ke andar kram (partition key = order id)
  Netflix       5b  TV ka purana 27, phone ke 37 ke upar           ->  baad wala jeete, purana discard (timestamp, last-write-wins)
  Twitter       19  "thank you" pehle dikha, "mast DP" gaayab      ->  do pe likho, do se padho (quorum)
  Leaderboard   5b  11:59:58 ka match, score 12:00:02 pe           ->  parchi pe khatam hone ka waqt, usi din ka register (event time)
  PUBG          3b  deewar ke peeche pahuncha, phir bhi mara       ->  server goli ke pal tak peeche ghuma ke dekhe (lag compensation, peeker's advantage)

Naam: sequence number / event time / OT / offset
Juda: Kafka ka offset bhi yahi number.
```

## 14. Badi file, door ka store  (LOCKER + PAAS)

```
Dikkat:      GB-GB ki file hamare server se guzar rahi; ya har tukda samundar paar se; ya purana maal bhi mehngi jagah.
Humne socha: badi file apne paas mat rakho, seedha locker (S3), watchman bas parchi rakhe. Tukdon me bhejo, tukdon me banao. Dekhne wale ke paas copy (CDN). Garam paas, thanda door.
             Upload: Twitter ka video, YouTube ka video, File upload = ek hi kahani. Netflix pe upload nahi (rights khareedta),
             wahan sirf paas rakhna (CDN) aur kai size (Ferrari / Alto).

Yahi dikkat yahan bhi:
  Twitter       10  50,000 log 1 GB video, server bhara            ->  alag route pe ja, URL se dekhenge (S3, presigned URL)
  Twitter       11  John 4K TV pe, Ravi gaon me phone pe           ->  Ferrari highway, Alto gaon (transcoding, ABR)
  Twitter       6   3-4 saal purane tweet bhi mote register me     ->  alag permanent diary, S3 (archive)
  Twitter       12  ek machine 4 size banaye, 20 minute            ->  kai machine, video id ki parchi pe baanto (workers / consumers)
  Twitter       14  2:00 pe khiskaya, phone 0:00 se kheenche       ->  copy dekh ke seedha 51wa page (manifest, seek)
  Twitter       14b 1 crore log, har tukda hamari API se?          ->  bas file hai, CDN pe rakh do (static)
  Twitter       14b 1 crore bachche yaad rakhein kaunsa page       ->  bookmark, 51 khud palat lo (player client side)
  Twitter       15  tukde US ke S3 me, samundar paar               ->  copy paas me rakho, paas se khule (CDN edge)
  File upload   2   1000 x 2 GB hamare server se                   ->  seedha bade dabbe me (S3)
  File upload   2   8 GB pe net gaya                               ->  tukdon me todo, sirf baaki 2 GB (multipart)
  File upload   2   adhoore tukde S3 me pade, client aaya hi nahi  ->  thodi der baad S3 ki safai hata de (lifecycle rule)
  File upload   3   S3 kaise maane ye hamara user                  ->  receptionist ka token (presigned URL)
  File upload   4   file S3 me aayi, server ko pata nahi           ->  S3 bataye "aa gayi" (event notification)
  File upload   6   download bhi hamare server se                  ->  truck seedha ghar (S3 direct download)
  File upload   7   link 200 logon ke WhatsApp group me            ->  link ki umar chhoti (presigned expiry)
  Banking       12  almari me jagah nahi, 10 saal rakhni           ->  head office bhej do (S3 archive)
  News          12  6 mahine, table mota, purani koi nahi padhta   ->  FR me kab tak, raddi jaisa nikaal ke alag (retention, archive)
  Chat          10  200 MB video, taar jam                         ->  alag raasta, chhota URL (S3, link)
  Netflix       1   har tukda samundar paar se                     ->  file apne shehar me, CA wala dost (Open Connect, ISP ke andar)
  Netflix       3   dabbe me poora maal nahi                       ->  garam paas, thanda bade dabbe se (popularity, tier, evict)
  Netflix       4   20 dabbe, kis se maange                        ->  hospital ka guard, server batata (steering; AWS pe, video bytes dabbe se)
  Netflix       4b  aadhe episode pe dabba 3 band                  ->  TV ke paas 2-3 dabbon ke naam, ek dheema to agla (client failover)
  Netflix       7   tunnel me 3 sec net gaya, video ruki           ->  Amul se 10 ki jagah 20 packet, nahi to Mother Dairy (player buffer, ABR)
  Netflix      (padhai, scene nahi) P1 AWS CDN kyun nahi: apna fridge muft · P2 AWS = control plane (login, list), video ISP ke dabbe se · P3 Ferrari / Alto + har film ki seedhi + kai roop · P4 har user ki apni thumbnail + list re-rank
  YouTube       1   10 hazaar creator, video server me phansi      ->  file seedha locker, watchman bas parchi rakhe (S3, presigned URL)
  YouTube       2   ek machine har quality, 3 ghante               ->  20-25 log, tukde baanto, chhoti quality pehle (parallel encoding)
  YouTube       5   10 saal purani video bhi mehngi jagah          ->  garam cheez garam jagah, thandi cheez thandi jagah (hot / cold storage, lifecycle)
  YouTube       6   2010 ka gaana aaj trend pe                     ->  fati jeans: thodi der garam + CDN, trend thanda to wapas almaari (promote / demote)
  YouTube       8   reels me har swipe pe spinner                  ->  agle 20 page pehle se, chhotu utha laaye (pre-fetch)
  YouTube       8b  mobile data, reels skip, pack khatam           ->  net ke hisaab se quality, 1 sec wali reel aage nahi (ABR, engagement)
  YouTube       8c  har reel poori pehle se, 29 sec bekaar         ->  shuru ke kuch chunks, pattern dekh ke kam (pre-fetch sirf shuru, trade-off)
  File upload   3b  10 GB upload, token beech me khatam            ->  har tukde ka apna token, resume pe baaki ke naye (presigned per part)

Naam: S3 + presigned URL / multipart / CDN / transcoding + ABR / hot-cold
Juda: link kisko mile wo gate pe (16).
```

## 15. Bhaari kaam, us waqt mat karo  (PEHLE SE BANA KE)

```
Dikkat:      user aaya tab jodna shuru kiya (500 logon ke tweet, 3 saal ka itihaas, 1 crore statement) to 6 second khaali screen.
Humne socha: jab pata hai kya maangega, pehle se bana ke rakh lo (parlour wala GTA). Bhaari kaam peeche chhotu raat ko, tukdon me, jahan ruka wahin se.

Yahi dikkat yahan bhi:
  Twitter       1   500 logon ke tweet milao, feed 6-7 sec         ->  apne paas rakhunga, apne paas se bhejunga (fanout on write)
  Twitter       1b  naya follow, purane tweet dabbe me nahi        ->  follow karte hi purane push (backfill)
  Twitter       1c  unfollow, purane 50 tweet dabbe me             ->  saaf nahi, naye band, dheere khisakte (lazy, ranking)
  Twitter       2   Kohli ke 27 crore dabbe, ghanton lage          ->  ek chhoti parchi, sab yahin se padhein (fanout on read)
  Twitter       3   doston ka dabba + 15 celeb parchiyan           ->  ek parchi pe, DSA jaisa sort (merge at read)
  Notification  8   1 crore offer, 40 lakh pe crash                ->  50 worker / consumer, alag-alag range (chunks)
  Notification  8   dobara chala to loop shuru se                  ->  parchi pe likho "ye ghar ho gaya" (checkpoint DB)
  News          1   5000 site, khabar aayegi kaise                 ->  chhotu, sab utha ke dabbe me (fetcher, DB)
  News          2   chhotu akela, ek chakkar 3 ghante              ->  bahut saare chhotu (kai fetcher)
  News          2   khabar pehle se taiyaar kahan                  ->  chhoti parchi, akhbaar jaisi (Redis feed)
  News          5   Ravi cricket, Priya tech, ek parchi            ->  category ke hisaab se baanto
  News          5b  Priya ki teen category, ek feed                ->  pasand ek parchi, khabar pe thappa, pasand walon ki feed me push; login nahi to aam (preference, fan-out on write)
  News          7   9:05 ki iPhone khabar purani parchi me nahi    ->  aate hi tech walon ki feed me (push on write)
  News          8   50 lakh alag parchi, 10 crore likhai           ->  saari tech khabar ek parchi (category feed)
  Banking       14  31 March 1 crore log ek saath                  ->  peeche chhotu, pehle se bana ke rakho (precompute, batch)
  Banking       16  chhotu aadha kaam karke gira                   ->  DB me likhta jaata, wahin se utha le (checkpoint)
  Banking       18  ek khaata 5 crore, statement ek                ->  kaam baanto, bada aadmi jode (MapReduce)
  Banking       19  May wale ko 1 May ka balance chahiye           ->  master register se utha le (balance snapshot)
  Chat          5   group ke 500, ek message 500 kaam              ->  ek dabbe me, log khud aake sunein (ek baar DB)
  Leaderboard   4   poora 1 crore ka board                         ->  referee wala: chhotu har kuch minute (snapshot job)
  Leaderboard   5   aaj / hafta / all-time                         ->  teeno ka kram alag, score teeno dabbon me
  Netflix       2   12:29 tak dabba khaali                         ->  pen drive pehle se, video badalta nahi (proactive fill)
  Netflix       6   "tere liye" me 6 second                        ->  parlour wala GTA pehle se (raat ka batch, cache se uthao)
  Netflix       6b  11 baje Dhurandhar dekhi                       ->  turant parchi, dikhate waqt milao (abhi ka fast store + bani list)
  Netflix       6c  Priya naya user                                ->  India ka garam maal (cold start)
  Twitter       20  naya user, 2 tweet ke baad feed khaali         ->  trending + suggested, engagement se apni pasand (cold start)
  YouTube       7   MrBeast ki video, crore subscriber             ->  Twitter wala fan-out, sabko push (fan-out + notification, queue + workers / consumers)
  Twitter       17  Amit ka tweet 40 ke neeche dab gaya            ->  jisse judta wo pehle (ranking)
  Docs          2b  2 saal purana doc, 50 lakh op                  ->  har kuch hazaar op pe doc ki photo, photo + baad ke op (snapshot)
  Train         4   tower ki list koi nahi deta                    ->  khud tukdon me, ya permission wali authority (railway, MCD, BMC), updates (data partner)
  Zomato        4   burger ready, driver 15 min door                ->  quantity se ready time, driver ko wahi time (prep-time dispatch)

Naam: precompute / fanout on write vs read / batch / checkpoint
Juda: Kohli jaise 27 crore pe ulta: padhte waqt milao (fanout on read).
```

## 16. Galat cheez andar aaye hi kyun  (BOUNCER)

```
Dikkat:      script 1000 OTP, nakli view, nakli chit, exe ko PDF bol ke, ya galat aadmi ka file khul gaya.
Humne socha: bouncer gate pe hi bitha do: had laga do, baar-baar kare to ban, galat cheez gate pe hi rok do, sahi aadmi hi dekhe.

Yahi dikkat yahan bhi:
  URL shortener 12  dhokhe wali site ka token                      ->  radd karo, aage se pehle hi jaancho (kaali list)
  URL shortener 12b radd kiya, log phone wala pata khol rahe       ->  phone me likhne hi mat do (302)
  URL shortener 12c dhokhe wala link bana hi kyun                  ->  library ki kitaab: banate waqt chupke se kholo (create-time safety check)
  URL shortener 13  machine se hazaar nakli chit                   ->  pattern pata lag jaayega, gate pe bhaga do (WAF)
  URL shortener 13b pakadne me 3-4 chakkar, laakhon chit de chuka  ->  had ek aadmi pe, group pe nahi, 5 hi (rate limit per user)
  URL shortener 13c bina login link, userId hai hi nahi            ->  pehle login laga do; bina login IP pe dheeli had (auth, IP rate limit)
  Rate limiter  1   script har second 1000 OTP, bill chadha        ->  ek bande ka threshold (rate limit, 429)
  Rate limiter  3   khaata badla, 3 second me 10 OTP               ->  dabbe se token nikaalo, apni raftaar se (token bucket)
  Rate limiter  4b  +1 hua, umar lagane se pehle server gira            ->  +1 aur umar ek hi kaam, ya dono ya kuch nahi (INCR + EXPIRE Lua, atomic)
  Rate limiter  5   guard andar kamre me, 1000 ghus gaye           ->  guard bahar gate pe bitha do (API gateway)
  Rate limiter  7   script 429 khaati, phir bhi maarti rehti       ->  dabbe me gala pakad ke bitha do (ban list)
  Rate limiter  8   office ke 500 log ek WiFi, sab atke            ->  login se pehle dheeli had, baad me userId pe
  Rate limiter  8   script roz hazaar baar reset maar rahi         ->  insaan hazaar baar nahi karta, pattern (WAF)
  Twitter       15b private video ka link WhatsApp group me        ->  taala-chaabi mere paas, 10 hi (allow list)
  Twitter       15c CDN ko allow list pata hi nahi                 ->  CDN ke paas bhi taala (signed URL / cookie)
  File upload   1   PDF bol ke exe chadha de                       ->  chhota tukda dekh lo (magic bytes)
  File upload   6   1043 likha, Priya ka statement khula           ->  "Priya ko saath la" (owner / share check)
  File upload   6b  CDN dabbe me Priya ka statement, Ravi ne URL khola ->  faisla server ka, token ke bina CDN na de (CDN signed URL)
  File upload   8   scan me 10 min baaki, download chahiye         ->  scan poora tak link hi nahi (CLEAN tak band)
  File upload   8   scan me virus nikla                            ->  file lo hi mat (quarantine / delete)
  Docs          6   Ravi hataya, server pe purani permission       ->  chhota note, uska op accept nahi (permission check)
  Docs          6b  Ravi offline likhta raha, aadhe ghante baad edits aaye ->  popup "hata diya", har edit pe aaj ka haq dekho, nahi to phenk do (op reject)
  Kafka         8   galat ticket gate tak aaye hi kyun             ->  bouncer, gate pe hi rok de (validation)
  YouTube       4   bot / phone se 10 lakh "view"                  ->  gino hi mat, gate pe rok, baar-baar to ban (rate limit, WAF)
  PUBG          3   hacker ka phone bole "goli lagi"               ->  server line kheenche, deewar, gudda (hit validation)
  PUBG          4   1 second me 500 meter                          ->  Koramangala se Whitefield namumkin, phenk do, ban (movement validation)
  Inventory     6   return aate hi +1, tooti screen Priya ko       ->  jaanch darwaze pe, kharab to wahin reject; ginti sirf bechne layak ki (doorstep QC, sellable vs damaged)

Naam: rate limit (token bucket) / API gateway / WAF / validation / auth
Juda: Rate limiter, URL, File, Kafka, Docs, Twitter video, YouTube views, PUBG ka hacker: ek hi gate.
```

## 17. Hall chhota, bheed badi  (LINE ME LAGAO)

```
Dikkat:      hall 1000 ka, bahar 5000; ghee 500, log 3000; cab 500, log 50 hazaar.
Humne socha: jitni jagah utne andar, baaki line me. Maal hi kam hai to FR me hi bol do: 500 ko hi milega.

Yahi dikkat yahan bhi:
  BookMyShow    4   hall 1000 ka, saamne 5000 khade                ->  capacity jitne andar, baaki line (virtual queue, token)
  BookMyShow    5   token khatam, parchi ke 3 min baaki            ->  token gate ka, seat ki ghadi parchi ki (do ghadiyan)
  Uber          6   concert khatam, 50 hazaar, cab 500             ->  500 andar, baaki line (virtual queue)
  Uber          6   baaki ke liye cab hi nahi                      ->  kiraya badha ke door ki cab (surge pricing)
  Uber          15c Arpan 7:05 tak "finding driver", naye aage     ->  1-2 baar peeche to agli gaadi usi ki (starvation -> aging)
  E-commerce    0   sale ka maal seemit                            ->  FR me hi cut off: 500 ko hi, baaki out of stock (consistency > availability)
  E-commerce    3   sale ki bheed, aata-daal bhi atka              ->  sale ki alag dukaan (alag service)
  E-commerce    7   3000 ek second me Pay                          ->  server pe line, 500 ke baad rollback (ya ghadi wala hold, dono ki keemat)
  E-commerce    7b  rollback + ghadi dono me pech                  ->  Apple store: sale gate, stock jitne hi Pay tak, fail = agla batch
  Zomato        6   Diwali, Burger Singh pe 2 min me 500            ->  subah stock poocho, 350 hi andar, baaki out of stock (capacity gate)

Naam: virtual queue / sale gate / token
Juda: andar gaya to ghadi (2), pay pe (11).
```

## 18. Zyada bhej rahe, zyada rakh rahe  (KAM KARO)

```
Dikkat:      poora text bhej rahe, 3 saal ke tweet dabbe me, 4.9 lakh "padha" entry, 500 copy rename pe.
Humne socha: poora mat bhejo, sirf jo badla. Poori list mat rakho, ek number se kaam chala lo.

Yahi dikkat yahan bhi:
  URL shortener 2   token lamba ho raha                            ->  gaadi ki number plate (base62)
  Twitter       6   dabbe me 3 saal ke tweet, memory ka bill       ->  parchi chhoti, bas last 500 (inbox had)
  Twitter       6   30 crore mahino se app nahi khol rahe          ->  30 din nahi aaya to kuch taiyaar nahi (on-demand inbox)
  Notification  9   har SMS se pehle saancha DB se                 ->  ek hi saancha apne paas, bas naam badlo (template cache)
  Notification  9b  OTP worker / consumer bhi pasand padhne DB pe             ->  OTP seedha bhejo, DB se lena-dena nahi
  File upload   5   folder rename = 500 copy + 500 delete          ->  tree jaisa, sirf parent badlo (parent_id)
  File upload   9   S3 bill double, ek hi file lakhon baar         ->  file ke maal se number, register me milao (content hash, dedup)
  File upload   9b  Priya wahi meme daale, list me dikhe nahi      ->  uske naam row, purani file pe bhejo (reference, cross-user dedup)
  File upload   9c  Ravi ne delete kiya, Priya ka download toota   ->  sirf Ravi ki row, aakhri wala hataye tab S3 se (reference counting)
  Docs          1   Save pe poora text, Arpan ki line gayab        ->  poora text nahi, operation bhejo (insert / delete)
  Chat          8   "12 naye message" kaise dikhe                  ->  ek ginti, itne padh liye (read count, kul - padhe)
  Chat          9   kisne padha, 4.9 lakh entry                    ->  978 matlab 975 padha, number ghata lo (watermark)
  Chat          11b Ravi online, khabar 500 contacts ko?           ->  jo dukaan me aaya usi ko LED dikhao, chat kholi = subscribe (presence pub/sub)
  Twitter       16  dabbe me sirf number, screen pe poora tweet    ->  ID se seedha page, photo CDN se (hydration)
  Twitter       16b tweet delete, 8000 dabbon me ID padi           ->  page pe "deleted" mile to dikhao mat (soft delete)
  Uber          16  Raju signal pe khada, har 4 sec wahi jagah     ->  sote ke kamre me baar-baar mat jao, 15-20 sec (adaptive ping)
  PUBG          2   Military Base ki har goli Pochinki tak         ->  6 km door ki awaaz aati hi nahi, sirf aas-paas ki (area of interest)

Naam: sirf badlaav bhejo / had lagao / soft delete / hydration
Juda: "padha" ka ek number kram wale number se hi banta (13). Kam bhejna bheed ka bojh bhi ghatata (7).
```

## 19. Server door baitha hai  (BRANCH)

```
Dikkat:      Ravi Delhi me, server America me; VPN badal ke teen region.
Humne socha: paas me branch khol do, branches aapas me baat karein. Ginti ek ghar region me.

Yahi dikkat yahan bhi:
  Rate limiter  10  VPN badal ke teen region, 15 OTP               ->  ek ghar region, ginti wahin (home region)
  Rate limiter  10b US ko kaise pata ghar Mumbai                   ->  +91 dekh lo, IP badle number nahi (country code)
  Rate limiter  11  feed ka cache usi Redis, Ravi ka dabba gaya     ->  alag Redis; ek me ho to LRU hatao + cache ka TTL chhota (noeviction)
  Twitter       8   Ravi Delhi me, server America me               ->  Lajpat Nagar me branch khol do (India region)
  Twitter       9   John New York me, Arpan ka tweet nahi          ->  SBI ki do branch aapas me baat karein (cross-region)
  Twitter       9b  branch ki baat 10 min ruki, beech me 5 tweet   ->  banda pal-pal dekhe "dost aaya?", aate hi 5 push (queue, retry, replay)
  PUBG          1   America ka ladka Mumbai match me, ping 300     ->  America wala America server pe; akela 3 wale squad me (region matchmaking, squad fill)
  E-commerce    1b  Lucknow ka store khaali, Kanpur me 20          ->  Lucknow wale ko Lucknow ka store, dark store = aas-paas ka (location se paas wala store)
  Inventory     4   Bangalore khaali, Delhi 50, Mumbai 10          ->  pehle apna shehar, phir main warehouse; doosra shehar FR tay kare (hub and spoke, sourcing rule)
  Banking       21  Mumbai + Frankfurt dono ne 1000 de diye        ->  jahan khaata khula wahi har kaam kare, baaki request aage bhejein (write affinity / home region)

Naam: multi-region / home region / cross-region
Juda: video ke liye yahi kaam CDN karta (14).
```

## 20. Sach kahan hai  (SACH)

```
Dikkat:      parchi me ek, DB me doosra; kisi ne haath se badal diya; GPS purana; paisa dikha kuch, kata kuch.
Humne socha: sach ek jagah. Kaato mat, neeche likho (ledger). Parchi purani ho sakti, DB sach; alag-alag ho to milao. Jo dikhaya wahi lo.

Yahi dikkat yahan bhi:
  Rate limiter  2   10 server, har ek ke paas alag ginti           ->  tu chhod, hisaab main rakhunga (central counter, Redis)
  Twitter       7   kaun kisko follow karta, iska track            ->  ek aur bada bhai (Neo4j)
  Stock         1   Arpan Ravi ko jaanta nahi, kaun milaye         ->  beech me ek aadmi, bhaav milao (matching engine)
  Stock         9b  parchi pe match, DB me abhi nahi               ->  "processing" turant, mote register ke baad pakka (DB = sach)
  Payment       7   2 minute 500 na yahan na wahan                 ->  ledger me likh lo, paise yahan hain (hold)
  Cache         5   naam badla, cache me purana "Ravi"             ->  pehle parchi kaato, phir DB (cache DELETE)
  Cache         5   TTL laga ke chhod dein?                        ->  form jaisa thodi der ka = chhota TTL; baar-baar badle to sync job (TTL)
  Cache         5   beech me purana naam phir parchi pe            ->  banda check kare, DB sach hai (sync job)
  Cache         5   kuch second purana Ravi dikha                  ->  paisa thodi hai (eventual consistency)
  Cache         5b  "pehle DB phir kaato, ulta kyun nahi?"         ->  chhed dono me, banda dono me chahiye (write-then-invalidate + retry)
  Cache         5c  sync job ko kaunsi key dekhni                  ->  pichhle ghante me jo update hui (updated_at)
  Banking       4   "500 mile hi nahi", saabit kaise               ->  alag diary, kaato mat, neeche likho (ledger)
  Banking       10  haath se 100 ko 600 kar diya                   ->  ledger se milwao, wahi asli sach (reconciliation)
  Banking       13  godown me -500 ko -50 kar diya                 ->  kai copy chhupa ke, milao (copies)
  Banking       13  ijazat wala file badal hi na paaye             ->  kohinoor, taala maar do (S3 Object Lock)
  Banking       15  28 Feb ka transaction der se aaya              ->  record se milao, nayi statement (reconciliation, revised)
  Uber          8   Raju Janpath me, purana naam pada              ->  sach wahi jo GPS ne abhi bheja (latest location)
  Uber          12  250 dikha, aakhir me 500 kate                  ->  jo dikhaya wahi lo (upfront fare)
  Leaderboard   1   1 crore sort kiye bina top 10 + rank           ->  apne aap sort wala dabba (Redis sorted set)
  Leaderboard   1b  score pe index laga do, rank phir bhi dheema     ->  rank = gin-na, aur har second likhai pe index bhi badle; write-heavy, isliye sorted set
  E-commerce    4b  Redis me ginti kati, DB me order nahi          ->  DB sach, banda milaye, 5 min ghadi ke baad bina order ki ginti wapas (reconciliation)
  Inventory     3   rack pe 48, system me 50, "2 kab gaye?"       ->  har bill register me (ledger), raat ka chhotu milaaye (reconciliation job)
  Cache         7b  key 3 machine pe, sirf 1 se kati               ->  parchi har copy se kaato, sync job copies bhi dekhe (har replica invalidate)
  Uber          16b buildings ke beech gaadi chhat pe (ghost bike) ->  paas wali sadak pe wapas utaar do (map matching)
  Uber          16b GPS ne bike nadi ke us paar phenki, 1 second   ->  rok nahi sakte, animation se wapas isi road pe (map matching)
  News          7   Priya ki parchi me aaj ki badi khabar chhooti  ->  milaan: chhoot gayi to DB se le ke seedha feed me (check job)
  Yelp          2   Burger Singh band, search me abhi bhi          ->  Postgres -> ES sync, "Closed" dikhao, Zomato jaisa (DB -> search sync)
  Yelp          3   dopahar ka naya burger, ES ko raat 2 baje       ->  badlaav pe change event, raat ki job bhi (change event, reconciliation)

Naam: ledger / reconciliation / DB = sach / cache delete on write
Juda: paisa ho to sach turant chahiye; profile ho to thodi der purana chalega.
```

## 21. Beech me khabar nahi aayi, screen pe kya dikhayein  (ANDAZA + STATUS)

```
Dikkat:      location har 4 sec, beech me gaadi teleport; basement me 5 min kuch nahi aaya.
Humne socha: beech me andaze se chalao, nayi aaye to sahi jagah. Kuch aa hi nahi raha to aakhri jagah pe roko aur map ki jagah status batao.

Yahi dikkat yahan bhi:
  Uber          16c har 4 sec location, beech me gaadi koodti      ->  gudde ko halka-halka aage, nayi aayi to sahi jagah (dead reckoning)
  Uber          16c flyover ke neeche U-turn, gudda aage chala     ->  signal aaya, asli peeche, kooda; har app me hota (correction)
  Uber          16c raaste me tunnel, net gaya, 15 jagah           ->  ruka dikhao ya aage badhao, mod pe seedha wahan; bahar aakhri jagah, bike tair ke (last write wins); 15 jagah Kafka -> tasveer
  Uber          16d basement me 5 min offline                      ->  aakhri jagah pe roko, "Raju pahunch gaya" (last known + status)
  PUBG          2b  har kadam pe 120 ms, kichad me chalna          ->  player ko turant apni screen pe chala do, server peeche hisaab kare (client-side prediction)
  PUBG          3b  phone pe 3 kadam aage, server bole ruk gaya    ->  server ki maano, 3 kadam peeche (reconciliation, rubber banding)
  Train         1   net / GPS band, train kahan                    ->  phone ka tower, range jitna andaza (cell ID, 5-10 km)
  Train         2   outer pe 25 min, timetable bole aage           ->  asli khabar jeete, delayed by X, expected time (ETA)
  Train         3   mera net band, dost ko khabar                  ->  jinka net chalu unka app bheje, server se push (crowdsourcing)
  Zomato        3   order Burger Singh tak, user ko kya dikhe       ->  placed -> accepted -> ready -> delivering + notification (order status flow)

Naam: dead reckoning / client-side prediction / last known location / status
Juda: server ki taraf location band ho to Uber 14 (location timeout); Zomato me do status, building tak + haath me.
```

---

## Notes ke link

- URL shortener: [notes](SYSTEM_DESIGNS/01_url_shortener/URL_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/01_url_shortener/01_url_shortener.md)
- Rate limiter: [notes](SYSTEM_DESIGNS/02_rate_limiter/RATE_LIMITER_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md)
- Twitter: [notes](SYSTEM_DESIGNS/03_twitter_feed/TWITTER_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md)
- Notification: [notes](SYSTEM_DESIGNS/04_notification_system/NOTIFICATION_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/04_notification_system/04_notification_system.md)
- Stock: [notes](SYSTEM_DESIGNS/05_stock_broker_trading/STOCK_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/05_stock_broker_trading/05_stock_broker_trading.md)
- Payment: [notes](SYSTEM_DESIGNS/06_payment_system/PAYMENT_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/06_payment_system/06_payment_system.md)
- Yelp search: [notes](SYSTEM_DESIGNS/23_yelp_search/YELP_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/23_yelp_search/23_yelp_search.md)
- Zomato: [notes](SYSTEM_DESIGNS/24_zomato/ZOMATO_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/24_zomato/24_zomato.md)
- File upload: [notes](SYSTEM_DESIGNS/07_file_upload_validate_system/FILE_UPLOAD_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/07_file_upload_validate_system/07_file_upload.md)
- News: [notes](SYSTEM_DESIGNS/08_news_aggregator/NEWS_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/08_news_aggregator/08_news_aggregator.md)
- BookMyShow: [notes](SYSTEM_DESIGNS/09_bookmyshow/BMS_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/09_bookmyshow/09_bookmyshow.md)
- Cache: [notes](SYSTEM_DESIGNS/10_distributed_cache/CACHE_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/10_distributed_cache/10_distributed_cache.md)
- Docs: [notes](SYSTEM_DESIGNS/11_google_docs_collab/DOCS_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/11_google_docs_collab/11_google_docs_collab.md)
- Kafka: [notes](SYSTEM_DESIGNS/12_message_queue_kafka/KAFKA_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/12_message_queue_kafka/12_message_queue_kafka.md)
- Banking: [notes](SYSTEM_DESIGNS/13_banking_system/BANKING_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/13_banking_system/13_banking_system.md)
- Chat: [notes](SYSTEM_DESIGNS/14_chat_messaging/CHAT_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/14_chat_messaging/14_chat_messaging.md)
- Uber: [notes](SYSTEM_DESIGNS/15_uber/UBER_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/15_uber/15_uber.md)
- Leaderboard: [notes](SYSTEM_DESIGNS/16_leaderboard/LEADERBOARD_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/16_leaderboard/16_leaderboard.md)
- E-commerce: [notes](SYSTEM_DESIGNS/17_ecommerce/ECOMMERCE_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/17_ecommerce/17_ecommerce.md)
- Netflix: [notes](SYSTEM_DESIGNS/18_netflix/NETFLIX_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/18_netflix/18_netflix.md)
- YouTube: [notes](SYSTEM_DESIGNS/19_youtube/YOUTUBE_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/19_youtube/19_youtube.md)
- PUBG: [notes](SYSTEM_DESIGNS/20_pubg/PUBG_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/20_pubg/20_pubg.md)
- Inventory: [notes](SYSTEM_DESIGNS/21_inventory_system/INVENTORY_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/21_inventory_system/21_inventory_system.md)
- Where Is My Train: [notes](SYSTEM_DESIGNS/22_where_is_my_train/TRAIN_HAND_NOTES.md) · [design file](SYSTEM_DESIGNS/22_where_is_my_train/22_where_is_my_train.md)
