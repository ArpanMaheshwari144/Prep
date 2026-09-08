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

## ★ RESIZE + LOAD-FACTOR MATH (8-Sep, deep-grill)

> HashMap andar array (buckets), default size **16**, load factor **0.75**.

**Kab double hoti (parking-lot):**
```
THRESHOLD = capacity * loadFactor = 16 * 0.75 = 12
entries 12 paar -> array DOUBLE (16->32), threshold->24. Phir 24 paar->64... har baar DOUBLE.
```
16-spot lot: 12 (75%) bharte hi bhari lagne lagta (collision badhta) -> bharne se PEHLE double kar dete.

**0.75 KYUN (trade-off):**
```
Chhota (0.5) -> jaldi resize + bahut khaali spot -> MEMORY waste
Bada  (1.0)  -> kam memory PAR chain lambi -> SLOW
0.75         -> sweet spot (Poisson: 0.75 pe bucket me 8+ collision ~0.00000006) = speed vs memory
```

### ★ Java-8 SPLIT-trick — resize pe koi dobara-hashCode NAHI
Capacity power-of-2 hai; double karne pe index-mask me sirf **ek naya bit** judta = value **oldCap (16)** khud. Har entry ko us ek bit se do me baant do:
```
hash & oldCap(16):
    == 0  -> LO list -> WAHI index (i)
    != 0  -> HI list -> index (i + oldCap) = i+16
```

**Numeric example — 2 keys (hash 5 aur 21) jo old table me SAME bucket the:**
```
hash 5   = 0 0101      hash 21 = 1 0101
OLD index (& 1111): 5&15=5 , 21&15=5   -> DONO bucket 5 (collision, chain me saath)

resize 16->32, split-bit = 16 (1 0000):
  A(5) : 0 0101 & 1 0000 = 0      -> LO -> index 5   (wahi)
  B(21): 1 0101 & 1 0000 = 16     -> HI -> index 5+16=21

verify seedha naya index (& 11111): 5&31=5 ✓ , 21&31=21 ✓

OLD(16): bucket[5]-> A(5)-> B(21)        NEW(32): bucket[5]->A , bucket[21]->B
```
Koi dobara-hashCode nahi — bas ek bit dekha. Chain apne-aap 2 bucket me bat gayi (collision kam).

**"16 kyun" (do alag kaam, confuse mat ho):**
```
Old INDEX me:  hash & (16-1)=01111   <- neeche ke 4 bit (mask)
Split me:      hash & 16   =10000    <- sirf naya 5th bit
```
16->32 pe mask 01111->11111 hua = ek naya bit juda, uski value = 16 = oldCap KHUD.
`naya-index = purana-index (bit 0) YA purana-index + oldCap (bit 1)`.
Isiliye **hash & oldCap** = wahi akela deciding bit — random 16 nahi.

**Aur "i+16 kyun, 32 nahi" (16+16=32 halves):**
```
NEW array (32) = do 16-16 aadhe:
   index:  0 ...... 15  |  16 ...... 31
           LOWER half        UPPER half

Purane buckets 0..15 the. Har purana bucket i split:
   LO -> i        -> LOWER half (0..15) me rehta
   HI -> i + 16   -> UPPER half (16..31) me jaata   (i=0..15 -> i+16=16..31)
```
i+16 hamesha upar wale aadhe me girta -> isiliye +oldCap. Aur 16+16=32 = pura array bhara (neeche+upar).

### treeify / untreeify (2 number)
```
treeify:   bucket 8+ nodes -> TREE, PAR sirf jab table size >= 64
           (table <64 + bucket bhara -> tree nahi, RESIZE karo -> usse hi collision kam)
untreeify: tree nodes 6 pe wapas LinkedList. (8 banao/6 todo -> gap taaki border pe flip-flop na ho)
```

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

---

## RED-BLACK TREE — bucket 8+ pe kya banta (visual, 2-Jul sekha)

```
   ek bucket me 8+ nodes -> LinkedList ko RED-BLACK TREE bana deta (treeify) -> us bucket search O(n) -> O(log n).
   sort kis pe: HASH value pe.

   Red-Black tree = self-balancing BINARY SEARCH TREE -> SORTED (left chhota, right bada).
   Rules (4):
     1. root HAMESHA black
     2. RED node ke bachche BLACK (do red lagataar nahi)
     3. har path me BLACK nodes ki count SAME
     -> in rules ka maqsad: tree BALANCED rahe -> height ~log n -> O(log n) guarantee.

   Insert 10,20,30 (seedha chain -> unbalanced -> rotation+recolor -> balance):
                    20 (B)          <- black root
                   /      \
               10 (R)     30 (R)    <- red children, dono taraf barabar black-height
```

```
   B-TREE vs RED-BLACK (dono sorted+balanced -> O(log n), bas shape/jagah alag):
   B-TREE     -> MULTI-WAY (kai keys/node), mota-chauda -> DISK (DB index).
   RED-BLACK  -> BINARY (2 child, 1 key), patla + colored -> RAM (HashMap bucket).
```
