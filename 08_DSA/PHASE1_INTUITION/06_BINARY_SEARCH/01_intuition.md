# Binary Search — Pattern Intuition


## WHAT IS BINARY SEARCH

Pattern: SORTED array mein value dhundo by repeatedly halving the search space. Beech dekho, ek taraf fenko, repeat. O(log n) — array kitna bhi bada, step bahut kam.

```
   Array:  ┌───┬───┬───┬───┬───┬───┬───┐
          │ 2 │ 5 │ 8 │12 │16 │23 │38 │
          └───┴───┴───┴───┴───┴───┴───┘
   index:   0   1   2   3   4   5   6
   target = 23
```

Naive = ek-ek check (O(n)). Binary Search = har baar AADHA kaat do (O(log n)).

DICTIONARY intuition: "Sharma" dhundne ko page 1 se nahi padhte — beech kholo, aage/peeche decide karo, aadha fenko, repeat. Sirf SORTED pe kaam karta (kyun: "mid se bada = dahine" tabhi pakka jab sorted).


## MECHANISM — low, high, mid

```
   low  = search-space left chhor  (start: 0)
   high = search-space right chhor (start: last index)
   mid  = (low + high) / 2
```

```
   arr[mid] == target  →  MIL GAYA, return mid
   arr[mid] <  target  →  target DAHINE → low = mid + 1
   arr[mid] >  target  →  target BAYEN  → high = mid - 1

   Repeat jab tak low <= high
   low > high  →  space khatam, target nahi (return -1)
```

```
   ┌──────────────────────────────────┐
   │  low <= high  →  abhi space hai    │
   │  low >  high  →  space khatam, -1  │
   └──────────────────────────────────┘
```


## DRY RUN #1 — target = 23 (FOUND)

```
   [2, 5, 8, 12, 16, 23, 38]   idx 0..6
   START: low=0, high=6
   STEP 1: mid=3, arr[3]=12 < 23 → low=4   (idx 0-3 fenke)
   STEP 2: mid=5, arr[5]=23 == 23 → return 5  ✓
   = 7 elements, 2 step. Halving: 7 → 3 → 1
```

```
   log(n): n=1000 → ~10 step, n=10 lakh → ~20 step
```


## DRY RUN #2 — target = 20 (NOT FOUND)

```
   [2, 5, 8, 12, 16, 23, 38]
   low=0,high=6: mid=3, arr[3]=12 < 20 → low=4
   low=4,high=6: mid=5, arr[5]=23 > 20 → high=4
   low=4,high=4: mid=4, arr[4]=16 < 20 → low=5
   low=5 > high=4 → space khatam → return -1
```


## KAB USE — RECOGNITION SIGNALS

| Signal | Approach |
|--------|----------|
| "SORTED array + search" | Binary Search (green signal) |
| "O(log n) chahiye / array bada" | halving needed |
| "First/last occurrence, insert position" | BS variant |
| "Ceiling / floor / closest" | BS variant (lower/upper bound) |
| "Search space halve ho sakta" (answer pe BS) | advanced — Phase 2 |

### ANTI-SIGNAL
- UNSORTED array  → pehle sort, ya HashMap
- Linked list     → no random access, mid = O(n)
- Bahut chhota n  → simple loop bhi theek

RULE: "SORTED" = binary search ka green signal.


## CLASSIC #1: FIRST OCCURRENCE (duplicates)

```
   [1, 2, 2, 2, 3, 4, 5]   target=2 → pehla 2 = index 1
```

TWIST: mila → ruko mat, BAYEN dhoondo (aur pehla ho sakta).
```
   arr[mid] == target → answer = mid, phir high = mid-1 (bayen)
   arr[mid] <  target → low = mid+1
   arr[mid] >  target → high = mid-1
```

```
   DRY RUN (target=2): answer=-1
   low0,high6: mid3 arr=2 == → answer=3, high=2
   low0,high2: mid1 arr=2 == → answer=1, high=0
   low0,high0: mid0 arr=1 <  → low=1
   low1 > high0 → answer = 1  ✓
```

LAST occurrence = MIRROR: mila → low = mid+1 (dahine dhoondo). Same skeleton, bas "mila to kya karo" badla.


## CLASSIC #2: SEARCH INSERT POSITION

```
   [1, 3, 5, 7]   target=4 → kahan fit? index 2 (3 aur 5 ke beech)
```

LOGIC: normal BS; "nahi mila" pe loop-end ka LOW = insert position.
```
   arr[mid] == target → return mid
   arr[mid] <  target → low = mid+1
   arr[mid] >  target → high = mid-1
   loop end → return low
```

```
   DRY RUN (target=4):
   low0,high3: mid1 arr=3 < 4 → low=2
   low2,high3: mid2 arr=5 > 4 → high=1
   low2 > high1 → return low=2  ✓
```

low loop-end pe "target se bada pehla element" pe khada = insert position.
Checks: target=0 → 0 (sabse aage), target=8 → 4 (sabse peeche). Yeh "lower bound" idea = ceiling/floor ka base.


## CLASSIC #3: SEARCH IN ROTATED SORTED ARRAY (medium)

```
   sorted [0,1,2,4,5,6,7] → rotated:
   [4, 5, 6, 7, 0, 1, 2]   idx 0..6   target=0
```

KEY OBSERVATION: rotated = DO sorted parts. Kisi bhi mid pe EK half hamesha sorted.

```
   arr[low] <= arr[mid]  →  LEFT half sorted
   warna                 →  RIGHT half sorted

   LEFT sorted:  target arr[low]..arr[mid] ke beech?
                 haan → high=mid-1 ; nahi → low=mid+1
   RIGHT sorted: target arr[mid]..arr[high] ke beech?
                 haan → low=mid+1 ; nahi → high=mid-1
```

```
   DRY RUN (target=0):
   low0,high6: mid3 arr=7. left sorted(4<=7). 0 in [4..7]? NAHI → low=4
   low4,high6: mid5 arr=1. left sorted(0<=1). 0 in [0..1)? HAAN → high=4
   low4,high4: mid4 arr=0 == 0 → return 4  ✓
```

INTUITION: sorted-half mein range-check easy → us half mein jao, warna doosre mein. Har step pe "kaunsa half sorted" DOBARA check hota (narrowed part bhi thoda rotated ho sakta — algorithm self-corrects). Phir bhi har step aadha = O(log n).


## CATEGORY 2: BINARY SEARCH ON ANSWERS

Ab tak = BS on a sorted ARRAY (value dhundo). Doosri category = BS on the ANSWER RANGE (array nahi — possible answers ki range mein dhundo).

```
   CATEGORY 1 (array):   sorted array mein VALUE dhundo
   CATEGORY 2 (answers): "minimum/maximum kya value chalegi?" — answer khud search
```

WHY POSSIBLE — monotonic "haan/naa":
```
   Answer range pe ek boundary hoti:
   chhoti value:  NAA NAA NAA | HAAN HAAN HAAN  :badi value
                              ↑ boundary (first HAAN)
   = is boundary ko binary search se dhoondo (feasibility check har mid pe)
```

### CLASSIC: KOKO EATING BANANAS (canonical)

```
   piles = [3, 6, 7, 11]   H = 8 ghante
   Koko speed K choose karti (K kele/ghanta, ek pile se).
   Goal: MINIMUM speed jisse saare piles H ghante mein khatam.
```

FEASIBILITY: ek pile ke hours = ceil(pile / K) [UPAR round — pile khatam, kela bacha = pura ghanta]. Total = sum. total <= H → HAAN.

```
   speed K=3 → 1+2+3+4 = 10 ghante → NAA (>8)
   speed K=4 → 1+2+2+3 = 8  ghante → HAAN (<=8)
   speed K=6 → 1+1+2+2 = 6  ghante → HAAN

   MONOTONIC: speed kam → ghante zyada (NAA); speed zyada → ghante kam (HAAN)
   answer = pehla HAAN = smallest feasible speed
```

```
   DRY RUN — range [1, 11], pehla HAAN (First-occurrence skeleton):
   low=1,high=11: mid=6 → 6 ghante <=8 HAAN → answer=6, high=5
   low=1,high=5:  mid=3 → 10 ghante >8 NAA → low=4
   low=4,high=5:  mid=4 → 8 ghante <=8 HAAN → answer=4, high=3
   low=4 > high=3 → answer = 4  ✓
```

WRAP — BS on answers recipe:
```
   1. Answer ki RANGE socho [min..max]
   2. Monotonic? (chhoti=NAA, badi=HAAN ya ulta)
   3. mid pe "feasible?" check (problem ka apna function)
   4. HAAN → answer yaad + ek taraf (min ke liye bayen) ; NAA → doosri taraf
   = First-occurrence binary search; bas "arr[mid]==target" ki jagah "feasible?"
```

### CLASSIC 2: SHIP PACKAGES WITHIN D DAYS (same skeleton, alag check)

```
   weights = [3, 2, 2, 4, 1, 4]   D = 3 din
   Packages ORDER mein (line fix). Har din ek ship, capacity C.
   Goal: MINIMUM capacity C jisse sab D din mein ship.
```

RANGE: low = max(weights) = 4 (isse kam = '4' fit hi nahi hoga), high = sum(weights) = 16 (sab ek din).

FEASIBILITY — "capacity C pe kitne din?" (greedy — formula nahi, chhota LOOP, kyunki order matters):
```
   days = 1            // pehla din
   W = 0               // current din ka load
   for each weights[i]:
       if (weights[i] + W > C)   // fit nahi hoga
           days++;               // naya din
           W = weights[i];       // ye package naye din ka pehla
       else
           W += weights[i];      // isi din load
   return days <= D;
```

```
   DRY RUN (BS on range [4,16], smallest C with days<=3):
   mid=10 → 2 din → HAAN → answer=10, high=9
   mid=6  → 3 din → HAAN → answer=6,  high=5
   mid=4  → 5 din → NAA  → low=5
   mid=5  → 4 din → NAA  → low=6
   low=6 > high=5 → answer = 6  ✓
```

### KOKO vs SHIP — feasibility-check ka FARAK (yahi confusion-point tha)

```
   KOKO:  formula per pile (har pile independent)
          hours = ceil(p1/K) + ceil(p2/K) + ...   → hours <= H ?

   SHIP:  greedy LOOP (order matters, running load + reset)
          (upar wala if/else)                     → days <= D ?

   = bahar ka binary-search structure DONO mein SAME
   = sirf andar ka "feasible?" check alag
```

SAME SKELETON family: "Split array largest sum", "sqrt(x)", "min days to make bouquets" — sab BS-on-answers, sirf feasibility-check badalta. Ek samjho → baaki transfer.


## POWER PHRASES

- "Sorted? → binary search. Beech dekho, aadha fenko, repeat. O(log n)."
- "Found = arr[mid]==target; not-found = low > high."
- "First occurrence: mila → high=mid-1 (bayen). Last: mila → low=mid+1 (dahine)."
- "Insert position = loop-end ka low (target se bada pehla element)."
- "Rotated array: ek half hamesha sorted — pehchaano, range-check, narrow."
- "BS on answers: answer-range pe search; monotonic NAA|HAAN boundary; mid pe feasibility-check. (Koko, Ship-packages — same skeleton, alag check.)"


## TRAP BOX

```
┌─────────────────────────────────────────────────────────┐
│ TRAP 1: Unsorted pe binary search                        │
│   → Sirf SORTED pe valid. Unsorted = sort/HashMap.       │
│                                                          │
│ TRAP 2: low <= high ki jagah low < high                  │
│   → low==high wala last element miss ho jaata.           │
│                                                          │
│ TRAP 3: First occurrence pe mila to turant return        │
│   → Duplicates mein galat index. answer=mid, phir bayen. │
│                                                          │
│ TRAP 4: mid = (low+high)/2 overflow (bade numbers)       │
│   → Safer: mid = low + (high-low)/2 (interview note).    │
│                                                          │
│ TRAP 5: Rotated array — ek hi baar half-check karna       │
│   → Har iteration "kaunsa half sorted" dobara check.     │
└─────────────────────────────────────────────────────────┘
```
