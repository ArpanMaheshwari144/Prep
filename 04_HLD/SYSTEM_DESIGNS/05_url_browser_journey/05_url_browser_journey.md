# "Type amazon.com -> Enter" — Browser Journey (Visual)

Classic interview question (JP / GS / FAANG). What happens between pressing Enter and the page showing up.

---

## FULL JOURNEY — OVERVIEW MAP

```
   [TU — Browser]
        │  URL type + Enter
        ▼
   ┌─────────────────┐
   │ 1. URL PARSE    │  kya site, kya path
   └────────┬────────┘
        │
        ▼  "IP kahaan hai?"
   ┌─────────────────┐
   │ 2. DNS LOOKUP   │  amazon.com -> 52.94.x.x
   └────────┬────────┘
        │  IP mil gaya
        ▼
   ┌─────────────────┐
   │ 3. TCP HANDSHAKE│  3-way (hello-hello-ok)
   └────────┬────────┘
        │  https = secure layer
        ▼
   ┌─────────────────┐
   │ 4. TLS HANDSHAKE│  lock lagao (encryption + identity)
   └────────┬────────┘
        │  ab request bhejo
        ▼
   ┌─────────────────┐
   │ 5. HTTP REQUEST │  GET /  (+ CDN / Load Balancer beech mein)
   └────────┬────────┘
        │
        ▼
   ┌─────────────────┐
   │ 6. SERVER ->    │  process + DB + response + render
   │    RESPONSE     │
   └─────────────────┘
```

```
   6 BADE STEP:
   1. URL Parse        2. DNS Lookup      3. TCP Handshake
   4. TLS Handshake    5. HTTP Request    6. Server->Response->Render
```

---

## STEP 1 — URL PARSE (browser URL todta hai)

```
   https://www.amazon.com/products?id=5

   ┌──────────┬─────────────────┬──────────┬────────┐
   │ https    │ www.amazon.com  │ /products│ ?id=5  │
   └──────────┴─────────────────┴──────────┴────────┘
   PROTOCOL       DOMAIN           PATH      QUERY
   (kaise         (kahaan         (kya       (extra
    baat ho)       jaana)          chahiye)   info)
```

```
   https     -> secure, port 443, TLS lagega
   domain    -> abhi NAAM hai, IP chahiye (next: DNS)
   path      -> us ghar mein kaunsa kamra
   query     -> us kamre mein product no. 5

   Browser yahaan check bhi karta:
   - apni memory (browser cache / history)
   - HSTS list (http -> https force)
```

KEY: domain abhi NAAM hai. Computer ko NUMBER (IP) chahiye -> isliye DNS.

---

## STEP 2 — DNS LOOKUP (naam -> IP)

DNS = internet ka PHONEBOOK (naam do -> number do).

```
   CACHE PEHLE (paas se door):
   ┌─────────────────┐
   │ 1. Browser cache│  abhi dekha tha?
   ├─────────────────┤
   │ 2. OS cache     │  computer ki memory
   ├─────────────────┤
   │ 3. Router cache │  ghar ka router
   ├─────────────────┤
   │ 4. ISP Resolver │  <- asli khoji (recursive)
   └─────────────────┘
```

```
   RESOLVER KI KHOJ (kahin cache nahi mila to):
   ┌──────────────┐
   │ ROOT server  │  "."   -> "com kaun? wahaan jao"
   └──────┬───────┘
        ▼
   ┌──────────────┐
   │ TLD server   │ ".com" -> "amazon.com? unke server"
   └──────┬───────┘
        ▼
   ┌──────────────┐
   │ AUTHORITATIVE│ amazon ka DNS -> "= 52.94.x.x"
   └──────┬───────┘
        ▼
   IP mil gaya -> tujhe wapas -> cache (TTL tak)
```

```
   SPATIAL: naye sheher mein "Sharma ji ka ghar" dhundna
   - jeb/diary (cache) -> info-center (ROOT)
   - mohalla office (TLD) -> watchman (Authoritative)
   - "ghar no. 52" (IP). Har level chhota, exact tak.
```

KEY: connection IP-to-IP hota. Domain naam baad mein bhi jaata (HTTP Host header / TLS SNI) kyuki ek IP pe kayi sites — server ko batane ko "kaunsi site chahiye".

---

## STEP 3 — TCP HANDSHAKE (connection banao)

```
   TU (Browser)                    SERVER
       │── 1. SYN ───────────────►│  "connect karein?"
       │◄─ 2. SYN-ACK ────────────│  "ok, main bhi ready"
       │── 3. ACK ───────────────►│  "confirm, shuru"
       │════ CONNECTION READY ════│
```

```
   SPATIAL (phone call):
   1. "Hello? sunai de raha?"        (SYN)
   2. "Haan, tu sun raha?"           (SYN-ACK)
   3. "Haan main bhi sun raha"       (ACK)
```

KEY: TCP = reliable pipe (data lost na ho, order mein aaye). http (port 80) hota to ab seedha request. Par https -> ek aur layer TLS.

---

## STEP 4 — TLS HANDSHAKE (encryption + identity)

TCP pipe khula hai — beech wala padh sakta. https = encrypted chahiye.

```
   TU (Browser)                      SERVER
       │─ 1. "secure baat?" ────────►│
       │◄ 2. "ok + CERTIFICATE" ─────│  (server ka ID-proof)
   ┌───┴───────────┐                 │
   │ 3. CERT CHECK │ "sach amazon?   │
   │ CA-signed?    │  CA pe bharosa?"│
   └───┬───────────┘                 │
       │─ 4. secret key exchange ───►│
       │═══ ENCRYPTED READY ═════════│
```

```
   2 KAAM:
   A. IDENTITY  -> Certificate (CA-signed) = "main sach amazon"
                   fake hota to cert fail = browser warning
   B. ENCRYPTION-> shared secret key, aage sab data lock
                   beech wala dekhe = scrambled gibberish
```

```
   SPATIAL: server ID card dikhata (cert) -> tu verify
   (sarkari mohar = CA?) -> dono ek secret code -> aage
   sab baat us code mein (koi padhe to samjhe na)
```

KEY: encrypted = scrambled with shared key. Sirf jiske paas key (tu + server) unscramble kar sakta. Yahi https ka 's'.

---

## STEP 5 — HTTP REQUEST (+ CDN / Load Balancer)

```
   ┌─────────────────────────────────────┐
   │ GET /products?id=5  HTTP/1.1         │ kya chahiye
   │ Host: amazon.com                     │ kaunsi site
   │ User-Agent: Chrome...                │ kaun maang raha
   │ Accept: text/html                    │ kis format
   │ Cookie: session=abc123               │ pehchaan/login
   └─────────────────────────────────────┘
```

```
   BEECH MEIN (seedha server tak nahi):
   [Browser]
       ▼
   ┌──────────┐  "static cheez cached hai?"
   │   CDN    │  image/css/js -> edge se TURANT
   └────┬─────┘  nahi to aage
       ▼
   ┌──────────┐  "kaunsa server free?"
   │  LOAD    │  traffic baant deta multiple servers
   │ BALANCER │
   └────┬─────┘
       ▼
   ┌──────────┐  asli kaam
   │  SERVER  │
   └──────────┘
```

```
   CDN -> static (image/css/js) paas wale edge se (fast, load kam)
   LB  -> ek server crash na ho, traffic baant ke free server pe
```

---

## STEP 6 — SERVER PROCESS -> RESPONSE -> RENDER

```
   (A) SERVER PROCESSING:
   ┌─────────────────────────────────────┐
   │ 1. Request samjha (GET /products?id=5)│
   │ 2. Business logic                     │
   │ 3. DATABASE se data ("id=5 detail")   │
   │ 4. Response banaya (HTML/JSON)        │
   └─────────────────────────────────────┘
```

```
   (B) RESPONSE WAPAS:
   ┌─────────────────────────────────────┐
   │ HTTP/1.1 200 OK                      │ status
   │ Content-Type: text/html              │ kya bheja
   │ <html>...amazon page...</html>       │ content
   └─────────────────────────────────────┘

   STATUS CODES:
   200 OK   301 moved   404 not found   500 server error
```

```
   (C) BROWSER RENDER:
   HTML aaya -> browser banata:
   1. HTML parse  -> DOM tree (structure)
   2. CSS         -> styling (kaise dikhe)
   3. JS          -> interactivity
   4. image/font  -> aur requests (aksar CDN se)
   5. Page DIKHA
```

```
   SPATIAL — LOOP COMPLETE:
   maanga (request) -> server banaya (process+DB) ->
   wapas bheja (response) -> screen pe rakha (render)
   = amazon.com khul gaya
```

---

## POWER PHRASES (interview bolne ko)

- "URL parse -> DNS -> TCP -> TLS -> HTTP -> server -> render."
- "Domain sirf naam; network connection IP-to-IP hota."
- "DNS = recursive resolver: ROOT -> TLD -> Authoritative, with caching."
- "TCP 3-way handshake = reliable connection before data."
- "TLS = certificate (identity, CA-signed) + shared key (encryption)."
- "CDN serves static from edge; Load Balancer spreads dynamic across servers."
- "Status codes: 200 OK, 301 moved, 404 not found, 500 server error."

---

## TRAP BOX

```
┌─────────────────────────────────────────────────────────┐
│ TRAP 1: "browser amazon.com se baat karta"               │
│   -> NAHI. DNS se IP nikaalta, baat IP-to-IP.            │
│                                                          │
│ TRAP 2: DNS resolver ko skip karna                       │
│   -> Cache miss pe ROOT->TLD->Authoritative chain.       │
│                                                          │
│ TRAP 3: TLS ko TCP se pehle batana                       │
│   -> Pehle TCP (connection), phir TLS (us pe encryption).│
│                                                          │
│ TRAP 4: CDN aur Load Balancer ko ek samajhna             │
│   -> CDN = static cache (edge). LB = traffic spread.     │
└─────────────────────────────────────────────────────────┘
```
