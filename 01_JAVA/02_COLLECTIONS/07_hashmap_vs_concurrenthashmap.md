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
