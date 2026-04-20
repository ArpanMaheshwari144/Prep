# HLD 05: Twitter Feed (Timeline)
### By Arpan Maheshwari

---

## KYA KARNA HAI?
```
Tu Instagram/Twitter kholta → posts dikhti
Jo follow kiya unki posts — sorted, fast

Tu 500 log follow karta
Sab milake 10,000 posts/day
Tujhe TOP relevant dikhane — fast
```

---

## FEED KYA HAI?

```
Feed = ek LIST. Bas. ArrayList jaisa.

ArrayList<Post> feed = new ArrayList<>();

feed = [
  { Rahul: "nayi bike", time: 12:05 },
  { Priya: "beach trip", time: 12:03 },
  { Amit: "new job", time: 12:01 }
]

Ye list REDIS mein stored hai:
  KEY:   "feed:arpan"
  VALUE: [ Rahul ki post, Priya ki post, Amit ki post ]

Tu app kholta → Redis se ye list le → dikha do. YEHI FEED HAI.
```

---

## "FEED MEIN DAAL" KA MATLAB?

```
Rahul ne post kiya: "nayi bike"

System kya karta:
  Rahul ke followers kaun? → Arpan, Priya, Amit

  Redis mein jaake:
    "feed:arpan" ki list mein ADD → { Rahul: "nayi bike" }
    "feed:priya" ki list mein ADD → { Rahul: "nayi bike" }
    "feed:amit"  ki list mein ADD → { Rahul: "nayi bike" }

  = 3 followers → 3 writes. Done.

Ab Arpan app khole:
  Redis se GET "feed:arpan" → list READY → dikha do
  FAST. Dhundhna nahi. Pehle se pada hai.
```

```mermaid
graph TD
    A[👤 Rahul post kiya] --> B[🧠 System: Rahul ke followers kaun?]
    B --> C[Arpan, Priya, Amit]
    C --> D[📬 feed:arpan mein ADD]
    C --> E[📬 feed:priya mein ADD]
    C --> F[📬 feed:amit mein ADD]
    D --> G[✅ 3 writes DONE]
    E --> G
    F --> G

    H[👤 Arpan app khola] --> I[⚡ Redis: GET feed:arpan]
    I --> J[📤 List READY → dikha do]
```

---

## 2 TARIKE — PUSH vs PULL

```
PUSH (pehle se daal do):
  Rahul post kiya → TURANT followers ke feed mein daal
  Fan app khole → READY hai
  
  WhatsApp jaisa — message AATA hai. Tu dhundhta nahi.

PULL (jab maange tab de):
  Rahul post kiya → sirf DB mein save
  Fan app khole → TAB DB se dhundh ke la
  
  Jaise tu khud restaurant jaake khana leke aaye.
```

```mermaid
graph LR
    subgraph PUSH["📢 PUSH — Pehle se ready"]
        A1[Rahul post kiya] --> B1[Followers ke feed mein DAAL]
        B1 --> C1[Fan app khole → READY ⚡]
    end
    
    subgraph PULL["🔍 PULL — Jab maange tab dhundh"]
        A2[Rahul post kiya] --> B2[Sirf DB mein SAVE]
        B2 --> C2[Fan app khole → DB se DHUNDH 🐌]
    end
```

---

## VIRAT WALA PROBLEM

```
Rahul ke 200 followers.
Rahul post kiya → 200 logon ke feed mein daal.
200 writes. 1 second. EASY.

Virat ke 5 CRORE followers.
Virat post kiya → 5 crore logon ke feed mein daal?
5 crore writes = 10-15 MINUTE lagega.
System pe BHAARIYA load. PROBLEM.
```

```mermaid
graph TD
    A[👤 Rahul post kiya<br/>200 followers] --> B[200 writes<br/>1 second ✅ EASY]
    
    C[👤 Virat post kiya<br/>5 CRORE followers] --> D[5 crore writes<br/>10-15 min ❌ IMPOSSIBLE]
```

---

## SOLUTION — DB SE FETCH KARO (Tera idea!)

```
Virat ka tweet → PUSH mat kar (5 crore writes bekar)
Virat ka tweet → DB mein SAVE kar bas. 1 write.

Jab fan app khole TAB:
  DB se Virat ke latest tweets le lo
  = PULL. On demand. Jab chahiye tab.

Rule:
  Chhota user (< 10K followers) → PUSH
  Bada user (> 10K followers)   → PULL (DB se jab chahiye)
```

```mermaid
flowchart TD
    A[📝 Naya Post aaya] --> B{👥 Kitne followers?}
    B -->|< 10K<br/>Rahul jaisa| C[📢 PUSH<br/>Followers ke feed mein daal]
    B -->|> 10K<br/>Virat jaisa| D[💾 Sirf DB mein SAVE<br/>Push NAHI]
    
    C --> E[Fan app khole → Feed READY ⚡]
    D --> F[Fan app khole → DB se PULL 🔍]
    
    E --> G[🔀 MERGE dono → sort → dikha]
    F --> G

    style C fill:#90EE90
    style D fill:#FFD700
```

---

## FAN APP KHOLE TAB KYA HOTA?

```
Arpan app khola:
  Step 1: Redis se feed le
          (Rahul, Priya ke posts READY — push se aaye)
  
  Step 2: DB se Virat ke posts le
          (abhi nikale — pull)
  
  Step 3: Dono MIX kar → time se sort → Top 100 dikha

= HYBRID. Push + Pull dono.
```

```mermaid
sequenceDiagram
    participant Arpan
    participant FeedService
    participant Redis
    participant DB

    Arpan->>FeedService: App khola — feed do
    FeedService->>Redis: feed:arpan do (normal posts)
    Redis-->>FeedService: Rahul, Priya ke posts READY
    FeedService->>DB: Virat ke latest posts do (celeb)
    DB-->>FeedService: Virat ke posts
    FeedService->>FeedService: MERGE + sort by time
    FeedService-->>Arpan: Top 100 posts dikha ⚡
```

---

## 2 BILLION USERS KA CACHE? CRASH NAHI?

```
Instagram: 2 billion users
Sab ka cache? = 200 PETABYTE. EK Redis? IMPOSSIBLE.

TRICK — SAB KA CACHE NAHI RAKHTE:

2 billion mein se:
  500 million ACTIVE (roz kholte) → cache RAKH
  1.5 billion INACTIVE (mahino se nahi khola) → cache NAHI

500 million × 100 KB = 50 TB
50 TB / 100 GB per server = 500 Redis servers

500 servers. Manageable.

AUR — TTL (Time to Live):
  "feed:arpan" → TTL: 24 hours
  Arpan roz kholta → cache refresh
  Koi 7 din se nahi khola → cache AUTO DELETE
  = Memory save
```

```mermaid
graph TD
    A[2 Billion Users] --> B{Active?}
    B -->|500M ACTIVE<br/>roz kholte| C[✅ Cache RAKH<br/>Redis mein]
    B -->|1.5B INACTIVE<br/>mahino se nahi| D[❌ Cache NAHI<br/>memory bachao]
    
    C --> E[500 Redis Servers<br/>50 TB total]
    E --> F[TTL 24hr<br/>nahi khola → auto delete]
```

---

## CELEBRITY CACHE TRICK

```
Virat ka tweet = SAB ko SAME dikhta.
5 crore log same tweet dekhenge.

Toh 5 crore baar DB query kyun?

TRICK:
  Virat tweet kiya → "celeb:virat:latest" Redis mein cache (1 write)
  5 crore log app khole → SAME cache se le
  DB query ZERO. 1 cache = 5 crore reads serve.

Jaise YouTube video — 10 crore views
Har view pe DB nahi — CDN cache se.
```

```mermaid
graph TD
    A[👤 Virat tweet kiya] --> B[💾 DB save]
    A --> C[⚡ celeb:virat:latest<br/>Redis cache — 1 write]
    
    D[👤 Fan 1 app khole] --> C
    E[👤 Fan 2 app khole] --> C
    F[👤 Fan 5 crore app khole] --> C
    
    C --> G[SAME cache se sab ko de<br/>DB touch NAHI ⚡]
```

---

## REGION WISE SHARDING (Tera idea!)

```
Virat ke 5 crore followers:
  3 crore India
  1 crore UK
  50 lakh US
  50 lakh rest

Region wise DB shard:
  India DB → sirf Indian followers serve
  UK DB → sirf UK followers serve

  India ka user → India DB se Virat ka tweet
  UK ka user → UK DB se

  Har DB pe load KAM. Fast.
```

```mermaid
graph TD
    A[👤 Virat tweet kiya] --> B[💾 India DB<br/>3 crore fans]
    A --> C[💾 UK DB<br/>1 crore fans]
    A --> D[💾 US DB<br/>50 lakh fans]
    
    E[🇮🇳 Indian Fan] --> B
    F[🇬🇧 UK Fan] --> C
    G[🇺🇸 US Fan] --> D
    
    B --> H[Har DB pe load KAM ✅]
    C --> H
    D --> H
```

---

## POORA SYSTEM — EK PICTURE

```
  WRITE (post kiya):
    Chhota user → Push → followers ke Redis feed mein daal
    Bada user → DB save + celeb cache (1 write)

  READ (app khola):
    Redis se feed (normal posts — push se ready)
    + Celeb cache se (bade users ke posts)
    + MERGE → sort → Top 100 dikha
```

```mermaid
graph TD
    subgraph WRITE["✍️ POST KIYA"]
        A[👤 Chhota User<br/>< 10K followers] --> B[📢 PUSH<br/>Followers ke feed mein]
        C[👤 Bada User<br/>Celeb] --> D[💾 DB Save<br/>+ Celeb Cache]
    end

    subgraph READ["👁️ APP KHOLA"]
        E[👤 Fan app khola] --> F[⚡ Redis Feed<br/>Normal posts READY]
        E --> G[⚡ Celeb Cache<br/>Virat ke posts]
        F --> H[🔀 MERGE + Sort]
        G --> H
        H --> I[📤 Top 100 dikha]
    end
```

---

## INTERVIEW MEIN YE BOLO (6 lines)
```
1. Hybrid — Push for normal users, Pull for celebrities
2. Feed = Redis Sorted Set — pre-computed list per user
3. Sirf active users ka cache — inactive ka nahi (TTL 24hr)
4. Celebrity cache — 1 write, crore reads serve
5. Region wise sharding — India/UK/US alag DB
6. Merge at read — cache + celeb posts mix → sort → dikha
```

---

*HLD 05 — Twitter Feed | by Arpan Maheshwari*
*"Feed = Redis mein list. Chhota user → push. Bada user → DB se pull + celeb cache. Dono merge → dikha."*
