# Rate Limiter — POORA ROUND (4 MOVE, jaise asli me hota hai)

> **NAV** — ARCHETYPE F · DIL: over-limit reject, legit allow. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [caching/Redis](../../FOUNDATIONS/04_caching.md) · [load-balancing](../../FOUNDATIONS/03_load_balancing.md) · [SPOF](../../FOUNDATIONS/11_reliability_spof_cloud.md)

> 15-Sep: asli mock-video ke hisaab se dobara likha — koi 7-step rail nahi, sirf 4 move:
> POOCHA -> do chhote block LIKHE -> BOXES banaye -> phir bolte-bolte JODTA gaya.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.
> HANDS-ON demos (Nginx + usercrud Java) end me — poore ke poore.
>
> Problem (1 line): over-limit requests REJECT (429), legit ALLOW. e.g. "5 login/min per IP".

```
   PUBLIC WATER TAP analogy:
   Normal user: 1 bottle | Pagal user: 10 truck, ghanton tak -> saara paani khatam, asli user vanchit
   -> Park ka niyam "5 bottle/din per banda" = RATE LIMITING
   SAME for APIs: hacker bot 10K login/sec (brute force) -> limit "5/min per IP".
```

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../HLD_APPROACH_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "Rate limiting ke kai pehlu hain — kis cheez pe limit lagani hai, kahan lagani hai,
        aur kitni sakht honi chahiye. Aap kis pe focus karwana chahenge?"

   TU: "Kuch cheezein confirm kar lun —
          - limit KIS PE? IP pe, user_id pe, ya API-key pe?
          - per-endpoint alag limit chahiye (login 5/min, search 60/min)?
          - tiered plans hain? (free vs pro vs enterprise)
          - limit ka maqsad SERVER BACHANA hai, ya paisa/security (exact hona chahiye)?"

   ★ aakhri sawaal sabse zaroori hai -- isi se aage ka poora trade-off tay hota hai
     (soft limit -> thoda over-allow chalega | paisa/security -> bilkul exact chahiye)
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌────────────────────────────┐    ┌──────────────────────────────────┐
   │ Rate Limiter               │    │ Use cases:                       │
   │   - Rule   (kya limit)     │    │   login          5/min  per IP   │
   │   - Counter (kitni ho gayi)│    │   password-reset 3/hr   per email│
   │   - Window  (kis samay me) │    │   public API     100/min per key │
   └────────────────────────────┘    │   signup         10/day per IP   │
                                     │   search         60/min per user │
   ┌──────────────────────────────┐  └──────────────────────────────────┘
   │ Kya chahiye (NFR):           │
   │  - LOW LATENCY <1ms  <- DIL  │
   │  - FAIL-OPEN (limiter mare   │
   │    to request PASS karo)     │
   │  - multi-server pe sahi count│
   └──────────────────────────────┘

   TU (dono NFR pe ungli rakh ke):
     "Do cheezein poore design ko chalayengi —
       1. Ye har request ke SAAMNE baithta hai. Thoda bhi slow hua to POORA API slow.
       2. Aur agar limiter khud mar jaaye to main request ALLOW karunga, block nahi —
          saare legit user block karne se achha hai thodi der abuse jhel lena.
          (FAIL-CLOSED sirf payment/security me, jahan galat guzarna mehnga hai.)"
```

```
   NUMBERS — yahan asli insight alag hai:

     TU: "Yahan main storage ka hisaab nahi karunga, kyunki data hai hi kitna --
          ek counter aur ek TTL per key, bas.
          Asli baat OPS-RATE hai: ye gateway pe baithta hai, matlab system ka
          SABSE HIGH-QPS component -- millions/sec tak."

     FAISLA -> "Ye STORAGE problem nahi, LATENCY problem hai.
                Isliye in-memory (Redis), DB nahi. Aur check atomic hona chahiye."
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple cheez se shuru karta hoon."

        USER ──► [ App server ]
                   counter andar hi (memory me)
                   count++ , limit paar -> 429

   TU: "Ek server pe ye chal jaata hai. Ab chala ke dekhte hain."
```

### dikkat 1 — "do server chal rahe hain, count baant gaya"

```
        USER ──► [ LB ] ──┬──► [ App-1 ]  count = 3
                          ├──► [ App-2 ]  count = 4
                          └──► [ App-3 ]  count = 2
                                           ─────────
                                total 9, par KISI EK ko 5 cross dikha hi nahi -> LIMIT TOOT GAYI

   FAISLA + KYUN:
        USER ──► [ LB ] ──► [ App x3 ] ──► [ REDIS ]  <- single source of truth
                                             counter + TTL

   TU: "Counter app ke andar nahi rakh sakta — har server ka apna count ho jaayega.
        Ek CENTRAL jagah chahiye, aur wo in-memory honi chahiye kyunki har request pe
        hit hogi. Isliye Redis."
```

### dikkat 2 — "do request ek saath aayi, dono ko jagah mil gayi"

```
        req A ──► count padha (4)  ──► +1 ──► likha (5)
        req B ──► count padha (4)  ──► +1 ──► likha (5)     <- limit 5 thi, 6 ghus gayi

   FAISLA: read-modify-write ko EK ATOMIC unit banao

        MULTI
          INCR   rate:login:userX
          EXPIRE rate:login:userX 60
        EXEC

   TU: "Redis single-threaded hai — ek waqt me ek hi command. Isliye INCR apne aap atomic hai,
        beech me koi doosri request ghus hi nahi sakti."

   ★ multi-step logic (token-bucket: refill + check + decrement) -> LUA SCRIPT
     (Redis poore script ko ek atomic unit ki tarah chalata hai)

   ★ INTERVIEW LINE:
     "Since Redis is single-threaded, check-and-decrement is atomic — INCR for a counter,
      or a Lua script for token-bucket, to avoid the read-modify-write race."

   ★★ CONNECT (2-Sep): ye WAHI race hai jo IDEMPOTENCY me thi (HDFC double-payment) —
      naive containsKey+put ka gap -> 2 request ghus -> double charge.
      Wahan ilaaj = putIfAbsent (atomic), yahan = INCR / Lua (atomic).
      Rate-limiter <-> idempotency = SAME race, same cure: "read+modify+write ko EK unit banao".
```

### dikkat 3 — "limiter ko app ke andar rakhoge to request poore system me ghoom chuki hogi"

```
        pehle (galat jagah):
            USER -> LB -> App (yahan check) -> ... request yahan tak aa hi gayi, resource kha liya

        ab (sahi jagah):
            USER -> LB -> [ API GATEWAY + rate limiter ] -> App
                                 │
                            REJECT yahin (429) -> backend tak jaane hi nahi diya

   TU: "Jise reject hi karna hai us pe backend ka compute kyun kharch karun?
        Isliye limiter sabse aage — API gateway pe."

   PLACEMENT ke 3 option: API Gateway | alag service | app-library
        -> ★ WINNER = API GATEWAY (sabse common)
        (optional: gateway pe mota GLOBAL limit + service ke andar fine limit -- par primary gateway)
```

### dikkat 4 — "Redis hi gir gaya to?"

```
        Redis DOWN
            │
            ├─► REPLICA auto-promote (Sentinel / cluster failover)   <- pehla ilaaj
            │
            └─► poori Redis layer hi gayi ──► FAIL-OPEN (sab allow)  <- aakhri sahara

        Redis OVERLOAD ──► SHARDING (user_id/region se: A-M -> R1, N-Z -> R2)

   ★ shard = scale + isolation   |   replica = recovery
   ★ SPOF-CHAIN: kisi bhi critical cheez ka SINGLE instance mat rakho -> har layer >= 2 + AUTO-FAILOVER
        LB down -> multiple LB (active-active/passive) + health-check / VIP failover
        (cloud ALB khud multi-AZ redundant hota hai)
     ★ MECHANISM zaroori hai: replicate karna kaafi nahi — koi failure DETECT karke REDIRECT bhi kare
       (Redis = Sentinel, LB = Route53/VIP). Warna replica bekaar pada rahega.
     TOP = DNS (Route 53) globally-managed -> khud single-box nahi -> top-level SPOF nahi.
```

### dikkat 5 — "user Bangalore + Berlin + US-VPN se maar raha hai"

```
        arpan_123  ──► INDIA region  : 50
                   ──► EU region     : 50
                   ──► US region     : 50
                                       ───
                                       150   par limit 100 thi -> TOOT GAYI

   FIX LADDER (teen, aur unka trade-off):
     1. CENTRALIZED Redis      -> accurate, PAR ek jagah (SPOF) + cross-region latency
     2. LOCAL + async sync     -> tez, PAR thoda OVER-ALLOW hoga
     3. ★ REGION-STICKY (BEST) -> hash(user_id) % regions = uska HOME region
                                  hash("arpan_123")%3=0 -> INDIA
                                  hash("john_456") %3=1 -> EU
                                  hash("alex_789") %3=2 -> US
                                  us user ke SAARE raaste ek hi region pe -> local Redis ke paas poori tasveer

   ★ ACCURACY vs LATENCY (ye trade-off bolna):
      local+async = har instance apne local count se allow karta, sync baad me -> limit APPROXIMATE
      KAB CHALEGA : limit sirf "server bachane" ke liye ho (general throttle/abuse) -> thoda upar-neeche chalega
      KAB NAHI    : limit = PAISA / SECURITY / correctness
                    ("3 OTP attempts" . "10 free API calls phir charge" . withdrawal limit)
                    -> CENTRAL ATOMIC (Redis + Lua), latency ki keemat bhugto
      1-LINE: protective/soft limit -> local+async (fast) | money/security limit -> central atomic (exact)
```

### dikkat 6 — "ek hi banda baar-baar maar raha hai, 429 se ruk hi nahi raha"

```
   Layer 1   RATE LIMIT      -> soft/temporary: "429, 60 sec baad try karo"
                 │
                 └─event─► KAFKA ─► Layer 2  PATTERN DETECTION  ("ye banda baar-baar?")
                                                   │
                                                   └─► Layer 3  WAF / IP-BLOCKLIST (PERMANENT ban)

   TU (kyun turant permanent ban nahi karte):
     "False positive ho sakta hai — asli user tez click kar raha ho.
      Ek NAT IP ke peeche 100 log baithe ho sakte hain.
      Aur marketing campaign me legit burst aata hai.
      Isliye rate-limit forgiving rakhta hoon (retry kar sakte ho), aur WAF sirf
      verified abuse pe — wo permanent hota hai."
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
                  USER
                   ▼
            ┌──────────────┐
            │  Route 53    │  DNS + health-check + nearest region
            └──────┬───────┘
            ┌──────▼───────┐
            │  CloudFront  │  CDN (static)
            └──────┬───────┘
            ┌──────▼───────┐
            │     ALB      │  Load Balancer
            └──────┬───────┘
     ┌─────────────▼────────────┐
     │  API GATEWAY             │  <- limiter YAHIN, sabse aage
     │  ┌────────────────────┐  │
     │  │  Rate Limiter      │──┼──────► [ REDIS CLUSTER ]  counter + TTL, atomic INCR
     │  └────────────────────┘  │              (replica + shard)
     └───────┬──────────┬───────┘
     ALLOWED │          │ REJECTED (429 + Retry-After)
             ▼          └──────────► USER
     ┌──────────────┐
     │  App Servers │
     └──────────────┘
                              [ REDIS ] ──event──► [ KAFKA ] ──► [ Pattern Svc ] ──► [ WAF ]

     Route 53   : mara hua LB hata deta -> SPOF khatam
     API Gateway: limiter sabse aage -> reject EARLY, backend ka compita bacha
     REDIS      : single source of truth + in-memory (<1ms) + atomic INCR
     replica    : Redis mare to failover . shard : load baantna
     KAFKA+WAF  : baar-baar wale abuser ke liye layered defense
```

```
   REQUEST FLOW (limiter ke andar):
     1. user pehchano       (IP / user_id / API-key)
     2. Redis key "rate:login:userX" -> INCR (atomic) + EXPIRE
     3. count > limit ?  HAAN -> 429 + Retry-After   |   NAHI -> App ko bhej do

   YAAD RAKHNE WALI EK LINE:
     "User -> Route53 -> CloudFront -> ALB -> API Gateway rate-limiter -> Redis atomic INCR+EXPIRE ->
      limit-andar? App | limit-cross? 429+Retry-After | abuse-pattern? Kafka | repeat-offender? WAF ban"
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "Response kaisa bhejoge?"

```
   limit OK   -> request aage bhej do
   limit HIT  -> HTTP 429 (Too Many Requests) + Retry-After: 30

   SUCCESS (200)                          REJECTED (429)
   ─────────────                          ──────────────
   X-RateLimit-Limit:     100             Retry-After:           30   (itne second ruko)
   X-RateLimit-Remaining: 47              X-RateLimit-Limit:     100
   X-RateLimit-Reset:     1715180400      X-RateLimit-Remaining: 0
                          (unix time)     X-RateLimit-Reset:     1715180400

   TU: "Client ko sirf 'na' mat bolo — batao kitna bacha hai aur kab dobara try kare.
        Warna wo turant retry maarta rahega."
```

## ► "Redis me kya rakhoge?"

```
   KEY  :  rate:{endpoint}:{user}  ──►  count
   TTL  :  window jitna (60 sec)   ──►  key apne aap gayab -> window reset ho gaya

   rate:login:192.168.1.5   ->  4
   rate:signup:user_456     ->  2
   rate:search:apikey_xyz   ->  47

   TU: "TTL hi mera window reset hai — alag se koi cleanup job nahi chahiye."
```

## ► "Kaunsa ALGORITHM lenge?"  (deep-dive ka dil)

```
   1. TOKEN BUCKET   ★ sabse common (AWS / Stripe)
        bucket me token apne aap bharte rehte (1/sec, max N)
        request aayi -> ek token lo -> allow . bucket khaali -> reject

             refill 1/sec
                  │
                  ▼
            ┌───────────┐
            │ ● ● ● ● ● │  max 5
            └───────────┘
                  │ request = 1 token
                  ▼
             allow / reject

        KYUN ACHHA: asli traffic SPIKY hota hai (user 10 request ek saath, phir shaant).
                    Jama huye token se burst nikal jaata -> user ko jhatka nahi lagta.

   2. LEAKY BUCKET
        request bucket me girti, neeche se FIXED rate pe nikalti (1/sec)
        bucket bhar gaya -> overflow -> reject
        -> bilkul SMOOTH, par burst BLOCK -> user ko laggy lagta

   3. FIXED WINDOW
        per-minute counter (0 -> 5, phir reset)
        ★ EDGE-SPIKE BUG: 10:00:59 pe 5 + 10:01:00 pe 5 = 2 second me 10 request
          (window ki seemā pe double limit) . sasta hai, par ye bug hai

   4. SLIDING WINDOW
        "last 60 second" me kitni -> har request ka timestamp rakho
        SMOOTH + accurate, PAR memory bhaari
```

```
   BUS-STAND ANALOGY (sliding window ACTUALLY shift kaise hoti hai):

     Watchman ka register: "last 1 ghante me 5 passenger MAX"
       entries: Ramesh 10:05 . Suresh 10:15 . ... . Dinesh 10:55

       11:00 pe naya aaya -> "60 min peeche = 10:00" -> count(10:00 ke baad) = 5 -> REJECT
       11:10 pe naya aaya -> "60 min peeche = 10:10" -> Ramesh(10:05) AB BAAHAR -> count = 4 -> ALLOW

     ★ WINDOW SHIFT = ON-DEMAND (request aane pe), koi timer/background job NAHI.
       Trigger = request ka aana, ghadi nahi.
```

```
   ┌──────────────────┬───────────┬─────────┬──────────┬────────────┐
   │  Algorithm       │ Bursts    │ Smooth  │ Memory   │ Kaun use   │
   ├──────────────────┼───────────┼─────────┼──────────┼────────────┤
   │ Token Bucket     │ YES       │ Variable│ Low      │ AWS, Stripe│
   │ Leaky Bucket     │ NO        │ YES     │ Low      │ Throttling │
   │ Fixed Window     │ Edge fail │ NO      │ Lowest   │ GitHub     │
   │ Sliding Window   │ Smooth    │ YES     │ High     │ Cloudflare │
   └──────────────────┴───────────┴─────────┴──────────┴────────────┘

   TU: "Main TOKEN BUCKET lunga — burst-friendly hai aur memory kam leta hai.
        Fixed-window sasta hai par boundary pe double limit nikal jaati hai;
        sliding-window sabse sahi hai par har request ka timestamp rakhna padta."
```

## ► "Alag-alag plan wale users ka kya?"

```
   TIERED LIMITS:
     anonymous    60 / hr
     free         5,000 / hr
     pro          10,000 / hr
     enterprise   custom

   user ka tier DB se aata -> Redis counter usi limit se compare hota
```

## ► "Kahan tootega / 10x traffic pe?"

```
   ★ RATTO MAT — request ka raasta chalao:

      request aayi
          │
          ├─► Gateway     -> limiter har request pe -> slow hua to POORA API slow -> in-memory + atomic
          ├─► Redis       -> ek node pe poora load -> SHARD (user/region se)
          │                  Redis mar gaya         -> REPLICA -> failover -> phir FAIL-OPEN
          ├─► multi-region-> ek user teen jagah se  -> REGION-STICKY hashing
          └─► repeat abuser -> 429 se ruk nahi raha -> Kafka pattern -> WAF permanent ban

   WRAP:
     "User -> Route53 -> CDN -> ALB -> API Gateway [rate limiter] -> Redis atomic counter -> App.
      Algorithm token-bucket. Key rate:{endpoint}:{user} with TTL. Reject = 429 + Retry-After.
      Distributed ke liye region-sticky; reliability ke liye replica + fail-open + shard;
      aur repeat abuse ke liye layered defense — rate-limit, Kafka pattern detection, WAF."
```

---

## ★★ "LIMITER LAGA THA, CHAL BHI RAHA THA — PHIR BHI NAHI BACHA" (20-Sep deep-dive)

> Upar ki 6 dikkatein limiter ko SAHI banane ki baat karti hain (distributed count, race,
> Redis down, multi-region, repeat abuser). Ye section alag sawaal ka jawab hai:
> *limiter sahi laga hua tha, chal raha tha, koi bug nahi tha — phir bhi server gaya. Kyun?*

### 1. ★★ PER-USER limit BHEED se nahi bachati (sabse bada, sabse kam samjha jaane wala)

```
limit          = 100 req/min per user
server ki had  = 5,000 req/s

ek ABUSER aaya        -> 100 pe ruk gaya            -> limiter ne bacha liya ✓
10,000 ASLI user aaye -> har ek apni limit ke ANDAR -> limiter khush ✓
                      -> server pe 12,000 req/s     -> SERVER GIR GAYA ✗
```

Limiter ne kuch galat nahi kiya. **Har user niyam ke andar tha.**

```
RATE LIMIT      "kis USER ne kitni bheji"        ->  ABUSE / fairness ke liye
LOAD SHEDDING   "SYSTEM abhi kitna jhel sakta"   ->  BACHNE ke liye
```

Ye DO alag cheezein hain aur aksar ek maan li jaati hain. Bheed ke liye chahiye: poore system ka
ek **global cap** (ya concurrency limit), aur load shedding jo **system ki sehat** dekh ke chale
("CPU 90% — ab nayi request mat lo"), na ki har user ki ginti dekh ke.

### 2. Request ki GINTI gini, LOAD nahi

```
100 req/min allow hai

user A:  100 chhoti request  (har ek 5ms)     =  0.5 second ka kaam
user B:  100 report query    (har ek 30 sec)  =  50 MINUTE ka kaam
```

Limiter ke liye dono BARABAR hain. **Ginti ka load se koi rishta nahi.**

```
ILAAJ:
   bhaari endpoint pe alag aur KADI limit
   ya har request ko WAZAN do    (report = 50 token, login = 1 token)
   ya ginti ki jagah CONCURRENCY seemit karo
      ("ek user ki 2 se zyada bhaari query EK WAQT me nahi")  <- aksar ye sabse behtar
```

### 3. FAIL-OPEN ka ulta chehra

Upar (dikkat 4) likha hai: Redis gaya → fail-open → sab allow. Availability ke liye sahi hai.
Par socho wo **KAB** hoga:

```
attack shuru  -> traffic 50x -> Redis pe bhi 50x -> Redis slow / down
              -> limiter FAIL-OPEN -> sab allow
              -> limiter THEEK USI WAQT GAYAB hua jab uski sabse zyada zaroorat thi
```

```
ILAAJ — fail-open akela kaafi nahi, uske SAATH local fallback:
   Redis zinda  ->  poora aur theek hisaab (global count)
   Redis gaya   ->  har node APNI MEMORY se motamoti rok lagaye

   ye poori tarah theek nahi hoga (har node apna-apna ginega)
   par attack ke waqt "kuch nahi" se "motamoti" BAHUT behtar hai
```

### 4. Galat cheez gin rahe the — IP

`rate:login:192.168.1.5` — IP pe limit sabse aasan hai aur sabse dhokhedeh.

```
NAT ka masla       ek daftar / ek mobile network ke HAZAARON log ek hi public IP se
                   -> ek banda limit kha gaya -> baaki SAB block -> asli user mara gaya

botnet ka masla    hamlavar ke paas hazaaron IP
                   -> har IP se 5 request -> kisi ki limit nahi tooti -> BILKUL nahi ruka
```

```
ILAAJ:
   jahan user LOGGED-IN hai      ->  user-id / API-key pe gino, IP pe NAHI
   jahan pehchaan hai hi nahi    ->  (login / signup se pehle) IP par majboori hai
      -> limit DHEELI rakho + asli faisla neeche WAF / bot-detection pe chhodo
```

### ★ Chhoti par asli — Retry-After ka thundering herd

Sab blocked client ko `Retry-After: 30` mila, aur sabne **theek 30 second baad EK SAATH** dobara
maara → window ki seema pe nayi laher. Isliye Retry-After me thoda **random farak** daalo —
kisi ko 28, kisi ko 33. (JITTER — wahi cheez jo avalanche ke TTL me lagti hai.)

---

### ★ EK HI SHAKAL — poore HLD me ghoomti hai

```
LB me      "BACHANE wali cheez ne maara"              (health check / retry / sticky)
cache me   "TEZ karne wali cheez ne raasta rok diya"  (slow Redis + no timeout)
limiter me "ROKNE wali cheez ne bheed ko roka hi nahi" (per-user limit vs aggregate load)
           "aur attack ke waqt wo KHUD gayab ho gayi"  (fail-open)
```
Poora dhaancha: `FOUNDATIONS/03_load_balancing.md` ka "CHHE TARIKE" + `04_caching.md` ka
"REDIS LAGATE HI SERVER DOWN" section.

---

## ═══ HANDS-ON — Nginx se rate-limiter LIVE chalaya (khud kiya, 21-Aug) ═══

> Upar sab THEORY padhi. Ye section = wahi cheez REAL TOOL me chala ke apni aankhon se dekhi.
> Koi program NAHI likha — sirf ek ready tool (Nginx) on kiya + config di + hammer maar ke 503 nikaala.
> (files: isi folder me `nginx.conf`)

### 0. Maqsad + tareeka
```
Rate-limiter ko PADHA to tha -> ab dekhna tha "practice me kaam kaise karta".
Tool = Nginx (real web-server, jisme rate-limiter PEHLE se built-in hai).
Nginx ko Docker container me chalaya, config di, curl se tez requests maari -> 503 aaya.
```

### 1. DOCKER kyun + kaise
```
Nginx laptop pe install karne ki zaroorat nahi -> Docker se ek command me container khada.
    docker run -d --name rl -p 8080:80 -v "<path>\nginx.conf:/etc/nginx/nginx.conf:ro" nginx

    -d            = background me chalao (detached)
    --name rl     = container ka naam "rl"
    -p 8080:80    = laptop ka 8080 -> container ke 80 se joda (localhost:8080 pe milega)
    -v "...:...:ro" = apni nginx.conf ko container ke andar wali jagah pe MOUNT karo (ro=read-only)
    nginx         = image (pehli baar auto-download hui: "Pulling from library/nginx")

    -> ek lambi container-ID print hui = chalu.
```

### 2. CONFIG file (nginx.conf) — ismein kya likha (2 line hi asli rate-limiter)
```
limit_req_zone $binary_remote_addr zone=mylimit:10m rate=1r/m;   <- (1) BUCKET banao
    $binary_remote_addr = client ka IP (har IP ka apna bucket)
    zone=mylimit:10m    = bucket ka naam + memory
    rate=1r/m           = REFILL speed (1 token per minute)

location / {
    limit_req zone=mylimit burst=5 nodelay;                       <- (2) LIMIT lagao
        burst=5   = bucket ka SIZE (ek saath 5 jhel lega)
        nodelay   = burst ko turant serve karo (queue me lataka mat)
    root /usr/share/nginx/html;  index index.html;               <- content serve
}

Do knob (dono token-bucket theory se):  rate = REFILL speed  |  burst = bucket SIZE
```

### 3. TEST kaise kiya (curl)
```
Normal (ek hit):
    curl http://localhost:8080                 -> "OK" / welcome page (bucket me token hai)

Hammer (30 request ek jhatke me, sirf status-code dikhao):
    for /L %i in (1,1,30) do @curl -s -o nul -w "%{http_code} " http://localhost:8080
        for /L (START,STEP,END) = (1,1,30) -> loop 30 baar -> 30 requests
        -s -o nul  = chup raho, body phenk do
        -w "%{http_code}" = sirf status code chhapo
```

### 4. Kya DEKHA (live)
```
200 200 200 200 200 200 503 503 503 503 ...
└──── bucket ke token ────┘ └──── khatam = BLOCKED (503) ────┘

Docker Desktop -> Containers -> rl -> Logs me nginx khud likhta:
    "GET / HTTP/1.1" 503 197
    [error] limiting requests, excess: 5.774 by zone "mylimit", client: 172.17.0.1
        503 197      = status 503, response sirf 197 byte
        excess: 5.77 = ye request bucket se kitne token UPAR thi
        zone mylimit = kis bucket ne roka
        client 172.17.0.1 = Docker gateway IP (saari requests ek hi IP se dikhi -> ek hi bucket share)
```

### 5. ★★ GEMS / gotchas (interview me bhi)
```
1. LIMIT tabhi kaatti jab ARRIVAL-rate > limit: pehle rate=2r/s pe slow curl-loop se sab 200 aaye
   (loop dheere tha, refill keep-up kar gaya). Tez maaro tabhi 503.
2. burst = N  ->  spike me N+1 pass (nginx apni taraf se +1 karta):
      burst=5 -> 6,   burst=100 -> 101.
      Wo +1 bucket me se NAHI hoti -> rate ki "live" 1 request bucket ke bahar se nikalti (alag slot).
      TEXTBOOK token-bucket = N (exact) | NGINX = N+1 (quirk).
      Aur EXACT bhi nahi -> timing/refill se thoda wobble (21, kabhi 22). Isiliye chala ke dekho, theory pe aankh-band bharosa nahi.
3. 1 IP = 1 bucket ($binary_remote_addr). Real world me har user ka apna IP -> apna bucket.
4. config badla -> "docker restart rl" karna PADTA (warna purana config chalta rehta).
```

### 6. Dobara kaise chalaye (quick)
```
docker start rl                                      (band ho to)
curl http://localhost:8080                           (normal check)
for /L %i in (1,1,30) do @curl -s -o nul -w "%{http_code} " http://localhost:8080   (hammer)
docker logs rl                                       (limiting-requests lines dekho)
docker restart rl                                    (config badla ho to reload)
docker stop rl                                        (band karna ho to)
```

---

## ═══ HANDS-ON #2 — APP-LEVEL fixed-window (usercrud, Java code) ═══
> Nginx wala (upar) = EDGE/INFRA pe limit (server ke bahar). Ye = APP ke ANDAR code me limit.
> Interview me "rate limiting kaise implement karoge" ka CODE-level jawab. Kiya 26-Aug usercrud pe.

### PROBLEM (kaunsa)
Ek endpoint pe 1 second me max N request allow; usse zyada -> 429 Too Many Requests.
Client abuse / accidental flood se server bachana.

### KYA BANAYA (fixed-window counter — 3 algorithm me se sabse simple)
```
STATE (3 field): LIMIT=5 (max/window) · windowStart (window shuru ka time, ms) · count (ab tak kitni)
LOGIC (har request pe):
   1. now - windowStart > 1000ms ?  -> nayi window: windowStart=now, count=0   (RESET)
   2. count++
   3. count > LIMIT ? -> return 429   :   return 200 "OK"
```
= "har 1 second ek nayi window, count zero se; limit paar -> 429." (window reset hote hi phir allow.)

### CODE (RateLimitController)
```java
@RestController
public class RateLimitController {
    private int LIMIT = 5;
    private long windowStart = 0;
    private int count = 0;

    @GetMapping("/rate-demo")
    public synchronized ResponseEntity<String> hit() {          // synchronized: shared counter
        long now = System.currentTimeMillis();
        if (now - windowStart > 1000) { windowStart = now; count = 0; }   // window reset
        count++;
        if (count > LIMIT)
            return ResponseEntity.status(HttpStatus.TOO_MANY_REQUESTS).body("429 - limit paar");
        return ResponseEntity.ok("OK - request #" + count);
    }
}
// SecurityConfig: .requestMatchers("/rate-demo").permitAll()
// ResponseEntity KYUN: status-code (200/429) khud set karne ko (plain String me control nahi).
```

### LIVE (jo dikha)
```
PS> 1..10 | % { curl.exe -s http://localhost:8080/rate-demo; "" }
   OK - request #1 ... #5          (pehli 5 allowed)
   429 - limit paar (count=6..10)  (6th se block)
   1 sec baad -> window reset -> phir OK
```

### 3 ALGORITHM (design-level, is code = pehla)
```
1. FIXED WINDOW   (banaya): per-second counter. SIMPLE. par window-BOUNDARY pe burst (999ms pe 5 + 1001ms pe 5 = 10 in ~2ms).
2. SLIDING WINDOW LOG: har request ka timestamp rakho, last-1sec wale gino. accurate, par memory zyada.
3. TOKEN BUCKET   (real prod): bucket me token bharte raho (rate), request = 1 token; token khatam -> reject. burst allow + smooth.
```

### ★ SCALE / PROD NOTES
```
- ye in-memory single-node -> restart pe reset + multi-server pe har server ka apna counter (galat total).
  MULTI-NODE -> REDIS me counter (INCR + EXPIRE) -> saare servers ek shared limit.
- per-USER/per-IP limit -> map<key, counter> (yahan global tha).
- prod-lib: Bucket4j (Java, token-bucket) / Redis + Lua (atomic).
```

### INTERVIEW LINE
```
"App-level rate limit fixed-window counter se kiya - per-second count, LIMIT paar -> 429 (ResponseEntity).
 Trade-off: fixed-window me boundary-burst; production me token-bucket (Bucket4j) ya sliding-window better.
 Multi-node pe Redis INCR+EXPIRE se shared counter. Nginx pe edge-level bhi kar sakte (limit_req).
 Maine usercrud pe live kiya - 10 rapid requests, pehli 5 OK phir 429."
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
