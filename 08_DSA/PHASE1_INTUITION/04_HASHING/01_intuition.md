# Hashing (HashMap / HashSet) — Pattern Intuition


## WHAT IS HASHING

Pattern: Ek aisi storage jahaan "yeh element hai kya?" ka jawab O(1) — turant, ek step. Array mein dhundna = O(n) scan; hashing = direct jump.

```
         ┌─────────────────────────────┐
         │   HASH TABLE                │
         │                             │
         │   key  ──►  value           │
         │   "a"  ──►   3              │
         │   "b"  ──►   1              │
         │   "c"  ──►   5              │
         └─────────────────────────────┘
```

= Key do → value turant milta
= "Phonebook" — naam do, number turant


## O(1) MAGIC — HASH FUNCTION

Key ko ek number mein convert karo (hash function). Woh number = direct address (array index jaisa).

```
   "apple"  ──► hash() ──► 7 ──► slot 7
   "banana" ──► hash() ──► 2 ──► slot 2

   slot:  0   1   2      ...   7
         ┌──┬──┬──────┬──┬──┬──────┐
         │  │  │banana│  │  │apple │
         └──┴──┴──────┴──┴──┴──────┘

   "apple hai?" → hash("apple")=7 → slot 7 dekho → mil gaya
   = ek calculation, ek jump = O(1)
```

Array: index pata = O(1). HashMap: key → hash → index = O(1). Same direct-access power, bus index ki jagah koi bhi key (string/number).


## 2 FORMS

### HashMap (key → value)
- "kitni baar 'a' aaya?" → map['a'] = 3
- key ke saath DATA store (count / index / list / running-sum)

### HashSet (sirf membership)
- "kya 'a' hai?" → set.contains('a') = true
- sirf "hai ya nahi", no data


## KAB USE — RECOGNITION SIGNALS

| Signal | Tool |
|--------|------|
| "Count / frequency of X" | HashMap (key→count) |
| "Seen before? / duplicate?" | HashSet (membership) |
| "Two-sum on UNSORTED array" | HashMap (complement) |
| "Group by property" (anagrams) | HashMap (key→list) |
| Nested loop O(n²) dikhe | HashMap se O(n) |

### ANTI-SIGNAL (jab NA use ho)
- Sorted array + pair        → Two Pointer (no extra space)
- Contiguous subarray sum    → Sliding Window / Prefix Sum
- Order / range matters      → array index better


## CLASSIC #1: TWO SUM (unsorted)

```
   Array:  [ 2 , 7 , 11, 15 ]   target = 9   (UNSORTED)
   index:    0   1   2   3
```

LOGIC: map = (value→index). Har element pe complement = target - current. Complement map mein hai? → pair mil gaya. Nahi? → current map mein daal, aage.

```
   DRY RUN (target=9):
   map = {}
   i=0: current=2, complement=7 → 7 hai? NAHI → map={2:0}
   i=1: current=7, complement=2 → 2 hai? HAAN(idx 0)
        ✓ answer = [0, 1]
```

WHY HashMap not Two Pointer: array UNSORTED. Sort = O(n log n) + indices change. HashMap = O(n) single pass, "complement dekha kya pehle?"


## CLASSIC #2: FIRST NON-REPEATING CHARACTER

```
   "leetcode" → pehla char jo sirf EK baar aaya
```

LOGIC (2 PASS): Pass 1 = HashMap mein har char ka count. Pass 2 = array left-to-right, pehla char jiska count==1.

```
   DRY RUN "leetcode":
   PASS 1: l:1, e:3, t:1, c:1, o:1, d:1
   PASS 2: i=0 'l' count=1 ✓ → answer 'l'

   DRY RUN "aabbc":
   PASS 1: a:2, b:2, c:1
   PASS 2: a,a,b,b skip → 'c' count=1 ✓ → answer 'c'
```

Pass 2 array order pe (kyuki "first" chahiye); map mein order nahi hota.


## CLASSIC #3: GROUP ANAGRAMS

```
   ["eat","tea","tan","ate","nat"]
   anagram = same letters, alag order
```

LOGIC: Har word SORT karo → woh "signature" key. map[signature] = list of words.

```
   DRY RUN:
   "eat"→"aet"  map={aet:[eat]}
   "tea"→"aet"  map={aet:[eat,tea]}
   "tan"→"ant"  map={aet:[eat,tea], ant:[tan]}
   "ate"→"aet"  map={aet:[eat,tea,ate], ant:[tan]}
   "nat"→"ant"  map={aet:[eat,tea,ate], ant:[tan,nat]}

   groups = [ [eat,tea,ate], [tan,nat] ]  ✓
```

"Group by property" = HashMap key→list. Value list ho sakti (sirf number/count nahi).


## CLASSIC #4: SUBARRAY SUM EQUALS K

```
   [ 1 , 2 , 3 , 1 ]   k=3
   → kitne contiguous subarrays ka sum = 3?
```

INTUITION: running_sum = index 0 se current tak ka total. Agar pehle kisi point pe running_sum = X tha, aur ab = X+k, to beech wale subarray ka sum = k. "Yeh sum pehle dekha kya?" → HashMap.

LOGIC: map={0:1} (empty prefix). Har element: running_sum += current; need = running_sum - k; agar need map mein → count += map[need]; phir map[running_sum]++.

```
   DRY RUN (k=3):  map={0:1} sum=0 count=0
   i=0: sum=1, need=-2 NAHI → map={0:1,1:1}
   i=1: sum=3, need=0 HAAN → count=1   ([1,2])  → map+={3:1}
   i=2: sum=6, need=3 HAAN → count=2   ([3])    → map+={6:1}
   i=3: sum=7, need=4 NAHI → map+={7:1}
   answer count = 2  ✓
```

running_sum = "prefix sum"; map = "yeh prefix pehle dekha + kitni baar". map={0:1} start = empty prefix handle. Direct bridge to PREFIX SUM pattern.


## CLASSIC #5: LONGEST CONSECUTIVE SEQUENCE

```
   [100, 4, 200, 1, 3, 2]   (UNSORTED)
   → longest run of consecutive numbers (1,2,3,4 = len 4)
```

KEY IDEA — "START":
```
   START = sequence ka SABSE CHHOTA number
         = jiske neeche wala (num-1) set mein MISSING hai
```

LOGIC: saare numbers HashSet mein. Har num pe: (num-1) absent? → START → aage count num+1, num+2... jab tak set mein. (num-1) present? → SKIP (beech ka, kisi START ne gin liya).

```
   DRY RUN: set={100,4,200,1,3,2}
   100: 99 NAHI → START → 101? NAHI → len 1
   4  : 3 HAAN  → SKIP
   200: 199 NAHI→ START → 201? NAHI → len 1
   1  : 0 NAHI  → START → 2✓3✓4✓5✗ → len 4  ← max
   3  : 2 HAAN  → SKIP
   2  : 1 HAAN  → SKIP
   max = 4  ✓
```

WHY O(n) not O(n²): har chain SIRF apne START se ek baar puri ginti hoti; beech wale skip = no repeat. HashSet O(1) lookup.


## POWER PHRASES

- "Count / frequency / seen-before / unsorted-pair → HashMap/HashSet ka signal."
- "HashMap value = count, index, list, ya running-sum — jo problem maange."
- "Sorted hota to Two Pointer, unsorted hai isliye HashMap."
- "Longest Consecutive: sirf START se count — START = neeche wala missing."


## TRAP BOX

```
┌─────────────────────────────────────────────────────────┐
│ TRAP 1: Sorted array pe HashMap use karna                │
│   → Sorted = Two Pointer (O(1) space). HashMap waste.    │
│                                                          │
│ TRAP 2: First non-repeating — map order pe scan karna    │
│   → "First" = ARRAY order pe scan, map order pe nahi.    │
│                                                          │
│ TRAP 3: Subarray Sum — map={0:1} bhul jaana              │
│   → Empty prefix handle nahi hoga, shuru se sum=k miss.  │
│                                                          │
│ TRAP 4: Longest Consecutive — har num se count karna     │
│   → O(n²) ban jaata. Sirf START (num-1 absent) se count. │
└─────────────────────────────────────────────────────────┘
```
