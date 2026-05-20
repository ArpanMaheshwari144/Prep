# HashMap — Internal Working

> **V90 Section 1 — Topic 6**

---

## WHY — HashMap O(1) KYUN?

→ Array mein **index se access = O(1)**
→ HashMap = **key ko number mein convert (hashCode)** → number se index nikalo `((n-1) & hash)` → **seedha us index pe jao**
→ 10 lakh entries mein bhi **TURANT**
→ Capacity **power of 2** KYUN? Bitwise AND = **1 CPU instruction** = modulo se FAST
→ **Load factor 0.75** = Poisson distribution pe sweet spot

---

## put(key, value) FLOW

```
put("name", "Arpan")
       │
       ▼
key.hashCode()  →  raw hash number
       │
       ▼
hash ^ (hash >>> 16)  →  spread bits
       │
       ▼
(n-1) & hash  →  BUCKET INDEX
       │
       ▼
Bucket empty?
   ╱       ╲
 YES         NO (Collision!)
  │            │
  ▼            ▼
Store      equals() check chain mein
here          ╱       ╲
           MATCH    NO MATCH
             │         │
             ▼         ▼
          REPLACE    ADD to chain
          value     (LinkedList / Tree)

Java 8: chain 8+ nodes + table 64+  →  Red-Black Tree
Average: O(1)  |  Worst: O(log n) tree, O(n) chain
```

---

## STORY — Library Manage

→ Soch tu ek library manage kar raha — **10,000 books**
→ Koi aaya: "Harry Potter chahiye." Ek-ek check karega? **O(n)? Nahi.**
→ Library ne system banaya — book ka naam se **shelf number calculate**
→ "Harry Potter" → **Shelf 42**. Seedha wahan jao
→ **Exactly yehi HashMap karta** — hashCode → index → seedha bucket → **O(1)**
→ `put("name","Arpan")` → hashCode → index → store
→ `get("name")` → same index → **O(1)**
→ **Collision:** do keys ka same index → LinkedList us bucket mein
→ Java 8: 8+ entries → LinkedList → **Red-Black Tree** → O(n) se O(log n)

---

## TRAP

> **HashMap null key ALLOWED. ConcurrentHashMap mein null key/value → NullPointerException!**

**Null banned in CHM kyu?**
→ `get(key)` → null aaya. Key nahi hai ya value null?
→ Single thread mein `containsKey()` check karo
→ Multi thread mein `get()` aur `containsKey()` ke **BEECH doosre thread ne data badal diya** — answer galat!
→ **Race condition.** Null banned = ambiguity hi mat aane do.

---

## POWER PHRASE

> *"HashMap uses hashCode() to find the bucket index and equals() to resolve collisions in the LinkedList. Java 8 converts the bucket to a Red-Black Tree when it has more than 8 entries — O(n) becomes O(log n)."*

**Yaad rakh:** hashCode → index → store. Collision = LinkedList. Java 8: 8+ entries → Tree.
