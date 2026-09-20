# HashMap vs ConcurrentHashMap

> **V90 Section 1 — Topic 7**

---

## STORY — The Library Disaster

> Imagine **2 librarians ek hi shelf pe** kitabein arrange kar rahe.
>
> Librarian A: "Java book yahan rakh deta"
>
> Librarian B (same time): "Yahi spot khaali hai, Python book yahan"
>
> **Crash!** Same spot pe dono ne rakhi → ek kitab gayab, ya overwrite.
>
> **HashMap = no rules library** — kabhi bhi kuch bhi ho sakta concurrent threads se.
>
> **ConcurrentHashMap = professional library** — har shelf pe lock, librarian access maange tab milta.

---

## RACE CONDITION VISUAL

### HashMap (NO synchronization)

```
   Thread A                         Thread B
   ─────────                        ─────────
   put("key1", "A")
        │ read bucket #5            put("key1", "B")
        │                                │
        │                                │ read bucket #5
        │ write "A" to bucket #5         │ (sees same state as A)
        │                                │ write "B" to bucket #5
        ▼                                ▼
        LOST UPDATE — only "B" remains

  Non-deterministic data corruption
  Could be: missing entries, infinite loop, ClassCastException
```

### ConcurrentHashMap (bucket-level locking)

```
   Thread A                         Thread B
   ─────────                        ─────────
   put("key1", "A")                 put("key2", "B")
        │ acquires lock              │ acquires lock
        │ on bucket #5               │ on bucket #7
        │ (key1 hash)                │ (key2 hash)
        │                            │
        │ writes safely              │ writes safely
        │ releases lock              │ releases lock
        ▼                            ▼
        Both succeed              No conflict

  Different buckets = parallel writes (high throughput)
  Same bucket = lock waits briefly, then safe
```

**Key insight:** ConcurrentHashMap **poora map lock NAHI karta** (`Hashtable` jaisa wo karta tha). **Sirf affected bucket lock** hota — performance much better.

---

## FULL COMPARISON

| Property | HashMap | ConcurrentHashMap |
|---|---|---|
| **Thread-safe?** | NO | YES |
| **Locking strategy** | None | Bucket-level (Java 8+: synchronized + CAS) |
| **null KEY** | Allowed (1 entry) | NPE thrown |
| **null VALUE** | Allowed (multiple) | NPE thrown |
| **Iteration** | Fail-fast (CME) | Fail-safe (weakly consistent) |
| **Performance (single-thread)** | Fastest | Slightly slower (lock overhead) |
| **Performance (multi-thread)** | Unsafe | High throughput |
| **Use case** | Local variable, single-thread | Shared state, multi-thread |
| **Internal in Java 8** | Array + LinkedList/TreeNode | Same + synchronized blocks per node |

---

## TRAP — null Behavior Difference

```java
HashMap<String, String> map = new HashMap<>();
map.put(null, "value");        // Works
map.put("key", null);          // Works

ConcurrentHashMap<String, String> cmap = new ConcurrentHashMap<>();
cmap.put(null, "value");       // NullPointerException
cmap.put("key", null);         // NullPointerException
```

**Why ConcurrentHashMap rejects null?**

- **Ambiguity:** `cmap.get(key) == null` → "key absent" YA "value is null"?
- Multi-threaded mein **`containsKey()` check** karne ka window mein state badal sakta
- Doug Lea (author): "Decision was to disallow null to prevent confusion"

---

## INTERNAL — Java 8 ConcurrentHashMap

```
Pre-Java 7: Segments (16 default) — segment-level lock
            (16 threads concurrent OK, 17th thread block)

Java 8+:    Per-bucket synchronized + CAS operations
            (Number of buckets = parallel threads possible)

            Bucket structure:
            ┌─────────────────────┐
            │ bucket[0] → null    │
            │ bucket[1] → Node ──→│  ← synchronized here
            │ bucket[2] → null    │
            │ bucket[3] → TreeNode│  ← treeified (>8 collisions)
            │ ...                 │
            └─────────────────────┘
```

**CAS (Compare-And-Swap):** Atomic CPU instruction — lock-free for empty buckets.

---

## USE CASE GUIDE

### When to use **HashMap**:
- Method-local variable (no thread sharing)
- Single-threaded application
- Read-only after initialization
- Configuration data loaded once

### When to use **ConcurrentHashMap**:
- Shared cache between request threads
- Counter / metrics (e.g., `compute()` for atomic increment)
- User session storage
- Connection pool tracking
- Spring `@Cacheable` underlying store

### When to use **Collections.synchronizedMap(map)**:
- Almost never in modern Java — `ConcurrentHashMap` better
- Legacy: wraps HashMap with global synchronized — slower than CHM

---

## INTERVIEW TALKING POINT

**Q: "HashMap thread-safe kaise banaye?"**

> *"3 options:
> 1. **`ConcurrentHashMap`** — best, bucket-level locking, high throughput
> 2. **`Collections.synchronizedMap()`** — wraps HashMap with global sync (slow)
> 3. **`Hashtable`** — legacy, full synchronization (almost never use)
>
> Modern Java mein **ConcurrentHashMap default choice** — performance + safety."*

**Q: "ConcurrentHashMap null kyu reject karta?"**

> *"Multi-threaded ambiguity. `get()` returns null could mean 'key absent' ya 'value is null' — racy state mein detect karne ka safe way nahi. Doug Lea ne explicitly decide kiya null disallow taaki confusion na ho. HashMap single-threaded mein null allow karta because ambiguity manageable hai."*

**Q: "Performance impact?"**

> *"Single-threaded mein HashMap thoda fast (no lock overhead). Multi-threaded mein ConcurrentHashMap **way faster** kyunki fine-grained locking — alag buckets parallel access. `synchronizedMap` global lock ki wajah se slowest in concurrent scenarios."*

---

## POWER PHRASE

> **"HashMap is NOT thread-safe — single-thread mein use karo. ConcurrentHashMap bucket-level locking + CAS use karta — multiple threads simultaneously read/write kar sakte without corruption. Production multi-threaded code mein default choice."**

---

## TRAP BOX

```
Trap 1: "Hashtable use karta hu thread-safe ke liye"
         Legacy — global sync, slow
         ConcurrentHashMap better

Trap 2: "Collections.synchronizedMap() = ConcurrentHashMap"
         NAHI — wo wrapper hai with global lock
         ConcurrentHashMap purpose-built, way faster

Trap 3: "ConcurrentHashMap mein null daal sakte"
         NPE
         Use sentinel value ya Optional

Trap 4: "Iteration safe hai HashMap mein"
         CME (ConcurrentModificationException) at runtime
         ConcurrentHashMap fail-safe iterator (weakly consistent)
```

---

## MEMORY HOOK

```
HashMap          =  "Library bina locks — fast but unsafe"
ConcurrentHashMap=  "Library shelf-level locks — safe + fast"
Hashtable        =  "Library full lock — old, slow, avoid"

null behavior:
  HashMap: null key + null values
  CHM:     both rejected (NPE)

Iterator:
  HashMap: fail-fast (CME on modify)
  CHM:     fail-safe (snapshot view)
```

---

## ★★ HANDS-ON — [`demo/concurrenthashmap/`](demo/concurrenthashmap/) (20-Sep, chala ke dekha)

> Upar wali saari baatein padhne ki cheezein hain. Ye section CHALANE ki cheez hai.
> Chaar file, chhoti-chhoti, ek ke baad ek. Har file ek sawaal chhodti hai aur agli uska jawab hai.
> Neeche jo code hai wo **file ka asli code** hai, aur jo output hai wo **asli run ka** — likha hua nahi.
>
> Chalane ka tarika (demo/ folder se):  `java concurrenthashmap/C1_Takkar.java`

| # | file | kya dikhata hai → *kaunsa sawaal khada karta hai* |
|---|------|--------------------------------------------------|
| C1 | [C1_Takkar.java](demo/concurrenthashmap/C1_Takkar.java) | plain HashMap do thread me TOOTTA hai → *to sahi dabba kaunsa* |
| C2 | [C2_Concurrent.java](demo/concurrenthashmap/C2_Concurrent.java) | ConcurrentHashMap poora 2000 deta hai → *ab sab theek ho gaya?* |
| C3 | [C3_CheckThenAct.java](demo/concurrenthashmap/C3_CheckThenAct.java) | sahi dabbe me bhi jawab galat → *phir ilaaj kya* |
| C4 | [C4_Ilaaj.java](demo/concurrenthashmap/C4_Ilaaj.java) | teen call ki jagah EK call → *poora* |

---

### C1 — [`demo/concurrenthashmap/C1_Takkar.java`](demo/concurrenthashmap/C1_Takkar.java)

**Kya kiya:** ek hi plain `HashMap` me do thread ne 1000-1000 entry daali. Keys JAAN-BOOJH KE alag
rakhi (0-999 aur 1000-1999), taaki "dono ne same key pe likh diya" wala bahana bache hi nahi.
Size 2000 aana chahiye.

```java
HashMap<Integer, Integer> hashMap = new HashMap<>();

Thread t1 = new Thread() {
    public void run() {
        for (int i = 0; i < 1000; i++) {
            hashMap.put(i, 1);
        }
    }
};

Thread t2 = new Thread() {
    public void run() {
        for (int i = 1000; i < 2000; i++) {
            hashMap.put(i, 1);
        }
    }
};

t1.start();
t2.start();
t1.join();
t2.join();

System.out.println("expected 2000, mila " + hashMap.size());
```

**Output (ek hi code, chhe alag run):**
```
expected 2000, mila 1656
expected 2000, mila 1911
expected 2000, mila 1678
expected 2000, mila 1768
expected 2000, mila 1818
expected 2000, mila 1791
```

**Isse kya nikla:**

- Koi exception nahi. `Process finished with exit code 0`. Code chupchaap **galat jawab** de gaya —
  is bug ki sabse gandi baat yahi hai.
- Number har baar alag hai kyunki takkar kab lagegi ye timing pe hai, aur timing kabhi same nahi hoti.
- Kaam do jagah gira:
  1. **Almari badalte waqt.** HashMap bharne pe nayi badi array banata hai aur purani ki saari entry
     usme shift karta hai. Thread-1 shift kar raha tha, aur usi beech thread-2 ne **purani** array me
     entry rakh di. Shift khatam, purani array phenk di — wo entry saath hi chali gayi.
     2000 put me ye shift 7-8 baar hota hai (16 → 32 → 64 ... 2048), isliye itna maal gira.
  2. **`size` ek saada `int` hai.** Har put pe `size++` hota hai, jo teen kaam hai — padho, jodo,
     likho. Do thread ek saath 800 padh lein to dono 801 likhenge. Do put hue, ginti ek badhi.
- ★ Java 7 me isi resize ke dauraan chain me **circle** ban jaata tha aur `get()` hamesha ke liye
  ghoomta reh jaata — CPU 100%. Java 8 ne wo loop wala hissa theek kar diya, **par data girna abhi
  bhi hota hai** — upar wala output usi ka hai.

---

### C2 — [`demo/concurrenthashmap/C2_Concurrent.java`](demo/concurrenthashmap/C2_Concurrent.java)

**Kya kiya:** bilkul wahi test, sirf dabba badla. Test ko ek method me daala jiska parameter
`Map` hai (`HashMap` nahi) — isi wajah se ek hi method dono dabbon pe chal jaata hai. Phir 10-10 baar
dono chalaye.

```java
public static int chalao(Map<Integer, Integer> map) throws InterruptedException{

    Thread t1 = new Thread() {
        public void run() {
            for (int i = 0; i < 1000; i++) {
                map.put(i, 1);
            }
        }
    };

    Thread t2 = new Thread() {
        public void run() {
            for (int i = 1000; i < 2000; i++) {
                map.put(i, 1);
            }
        }
    };

    t1.start();
    t2.start();
    t1.join();
    t2.join();

    return map.size();
}
```

```java
int ans1 = 0;
for (int i = 0; i < 10; i++) {
    Map<Integer, Integer> hashMap = new HashMap<>();
    ans1 = chalao(hashMap);
}

int ans2 = 0;
for (int i = 0; i < 10; i++) {
    Map<Integer, Integer> concurrentHashMaphashMap = new ConcurrentHashMap<>();
    ans2 = chalao(concurrentHashMaphashMap);
}

System.out.println(ans1);
System.out.println(ans2);
```

**Output (do run):**
```
1950        <- HashMap
2000        <- ConcurrentHashMap

2000        <- HashMap, dusre run me
2000        <- ConcurrentHashMap
```

**★ Ye dusra run hi is file ka sabse kaam ka hissa hai.** HashMap ne bhi 2000 de diya. Do wajah:
`ans1` loop ke BAHAR print hota hai, isliye sirf **10ve** chakkar ka number dikhta hai; aur kabhi-kabhi
do thread takraate hi nahi. Matlab **ek sahi output is bug ke na hone ka saboot nahi hai.**
Isi liye C1 ko chhe baar chalaya gaya tha.

**ConcurrentHashMap ne 2000 kyun diya — teen jagah alag kaam karta hai:**

```
1. taala poore map pe nahi, sirf EK BUCKET pe
      thread-1 bucket 5 me likh raha, thread-2 bucket 12 me — kisi ko rukna nahi padta
      khaali bucket me to taala bhi nahi, CAS se seedha node rakh diya jaata hai

2. ginti ek int nahi hai
      ginti kai alag cell me bant-ti hai, har thread apne cell me jodta hai
      size() poochho to sab jod ke batata hai  ->  ginti kabhi girti nahi

3. almari badalna MILKE hota hai
      resize ke beech jo thread aata hai wo shift karne me HAATH BATAATA hai
      jo bucket move ho chuka uspe forwarding-node ka nishaan hota hai
      -> naye aane wale ko pata chal jaata hai "ye ab nayi almari me dekho"
      -> koi andhere me purani almari me nahi likhta
```

★ `Hashtable` aur `Collections.synchronizedMap` bhi thread-safe hain, **par wo poore map pe ek hi
taala lagate hain** — ek waqt me ek thread. Farak "safe hai ya nahi" ka nahi, **throughput** ka hai.

---

### C3 — [`demo/concurrenthashmap/C3_CheckThenAct.java`](demo/concurrenthashmap/C3_CheckThenAct.java)

**Kya kiya:** map ab `ConcurrentHashMap` HI hai — wahi jo abhi 2000 de raha tha. Ek hi key `"hits"`,
aur dono thread 10,000-10,000 baar ginti badhayenge. Badhane ka tarika wahi rakha jo log asli code me
likhte hain: nikalo, jodo, wapas rakho.

```java
ConcurrentHashMap<String, Integer>  map = new ConcurrentHashMap<>();
map.put("hits", 0);

Thread t1 = new Thread(()->{
    for (int i=0;i<10000;i++) {
        int hits = map.get("hits");
        int val = hits + 1;
        map.put("hits",val);
    }
});
// t2 bilkul same

System.out.println("expected 20000, mila " + map.get("hits"));
```

**Output (paanch run):**
```
expected 20000, mila 12663
expected 20000, mila 10704
expected 20000, mila 11885
expected 20000, mila 16227
expected 20000, mila 12184
```

**Isse kya nikla — ye is poore section ka asli sabak hai:**

```
thread-1                         thread-2
get("hits")  ->  500
                                 get("hits")  ->  500
+1           ->  501
                                 +1           ->  501
put(501)
                                 put(501)

do baar badhaya, ginti EK hi badhi
```

`get` safe tha. `put` safe tha. Par beech me jo **gap** hai — jahan thread-1 ne padh liya aur abhi
likha nahi — us gap me thread-2 ghus gaya. Thread-1 purani value pe hisaab karke likh gaya.

> ★★ **Dabba thread-safe hone ka matlab ye NAHI ki tera CODE thread-safe hai.**
> Har call apne aap me safe hai, par kai call **milkar** ek kaam banate hain aur us poore kaam ka
> koi taala nahi hota. Isko **compound action** kehte hain.

Iske roop — PR review me yahi shakal dhoondhni hai, **ek hi key pe do lagataar map-call**:
```
containsKey(k)  phir  put(k, ...)
get(k)          phir  put(k, ...)
get(k)  phir  check  phir  remove(k)
```

★ Aur ye prod tak isliye pahunchta hai ki dev machine pe do request chalti hain aur kuch nahi hota.
Prod pe 200 request ek saath, aur ginti chupchaap kam ho jaati hai. Na exception, na log, na alert.

---

### C4 — [`demo/concurrenthashmap/C4_Ilaaj.java`](demo/concurrenthashmap/C4_Ilaaj.java)

**Kya kiya:** C3 wala hi code, teen baar. Har baar sirf wo ek line badli. Teen alag map (warna ginti
mil jaati).

```java
// TAREEKA 1 — merge
ConcurrentHashMap<String, Integer> map1 = new ConcurrentHashMap<>();
map1.put("hits", 0);
// dono thread me:
map1.merge("hits", 1, (purani, nayi) -> purani + nayi);


// TAREEKA 2 — compute
ConcurrentHashMap<String, Integer> map2 = new ConcurrentHashMap<>();
map2.put("hits", 0);
// dono thread me:
map2.compute("hits", (k, v) -> v == null ? 1 : v + 1);


// TAREEKA 3 — AtomicInteger
ConcurrentHashMap<String, AtomicInteger> map3 = new ConcurrentHashMap<>();
map3.put("hits", new AtomicInteger(0));
// dono thread me:
map3.get("hits").incrementAndGet();
```

**Output (do run, aur baar-baar chalane pe bhi hilta nahi):**
```
merge         = 20000
compute       = 20000
AtomicInteger = 20000
```

**`merge` ka syntax — teen cheezein leta hai:**
```
map1.merge("hits", 1, (purani, nayi) -> purani + nayi);
        //   ^key   ^2nd        ^function

key hai hi nahi   ->  2nd wali value (1) seedha rakh do. Function chalta hi NAHI.
key maujood hai   ->  function bulao:  purani = map me padi value
                                       nayi   = jo tune 2nd me bheja (1)
                      jo return hua, wahi map me chala jaata hai.
```
`purani` / `nayi` sirf naam hain — `(a, b) -> a + b` bhi wahi baat hai. Chhota likhna ho to `Integer::sum`.

★ Agar function `null` return kar de to **key map se HAT jaati hai** — isi se "ginti ghatao, zero ho
jaaye to entry hi uda do" wala kaam hota hai.

**AtomicInteger alag soch hai:** map ko chhedo hi mat. Entry ek hi baar banti hai, uske baad har
increment `AtomicInteger` ke andar hota hai aur map ko pata bhi nahi chalta. Isi liye ek hi key pe
bahut zyada likhai ho to ye sabse tez hai.

★ `merge` / `compute` ka lambda **bucket ka taala lage hue** chalta hai — usme DB call, network call
ya kisi doosre map ko chhedna mat likhna, poora bucket tab tak ruka rahega. Sirf hisaab.

---

### ★ EK SAANCHA — C1 se C4 tak ka nichod

```
SAWAAL 1: dabba sahi hai?          HashMap  ->  do thread me TOOTTA hai (C1)
                                   CHM      ->  bucket-level taala, safe (C2)

SAWAAL 2: kaam EK call me hai?     teen call (get/+1/put)  ->  gap khula = bug (C3)
                                   ek call  (merge/compute) ->  taale ke andar = theek (C4)

★ DONO chahiye. Sirf pehla kar ke ruk jaana = C3 wala haal.
```

**Kaunsa method kab:**
```
ginti badhani hai                 ->  merge
value se hisaab karke rakhna      ->  compute
nahi hai to bana do               ->  computeIfAbsent   (cache / list banane me)
sirf pehli baar claim karni hai   ->  putIfAbsent       (idempotency — usercrud me yahi hai)
ek key pe bahut zyada likhai      ->  value hi AtomicInteger bana do
```

---

## ★ PROJECT CONNECT — usercrud (8-Sep)
```java
IdempotencyController: ConcurrentHashMap<String,String> processed;   // thread-safe claim
                       processed.putIfAbsent(key, "PROCESSING");       // bucket-atomic (do request ek saath -> ek hi claim)
KafkaConsumer:         Set<String> processedIds = ConcurrentHashMap.newKeySet();  // dedup, duplicate msg skip
```
Real CHM LIVE — kyun plain HashMap nahi: multiple request/consumer-thread ek saath likhte -> HashMap pe race/CME aata; CHM ka bucket-level lock/CAS `putIfAbsent` ko atomic banata (idempotency + dedup dono isi pe tikte).
