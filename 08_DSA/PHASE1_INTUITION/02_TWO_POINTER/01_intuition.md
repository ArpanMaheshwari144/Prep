# Two Pointer — Pattern Intuition


## WHAT IS TWO POINTER

Pattern: Ek hi array pe DO positions track karo, kuch logic ke saath move karo.

```
         ┌───┬───┬───┬───┬───┬───┐
         │   │   │   │   │   │   │
         └───┴───┴───┴───┴───┴───┘
           ↑               ↑
         ptr1            ptr2
```

= Bas itna. Pattern simple, application versatile.


## WHY USE

- Single pass mein 2 ends/positions handle
- Time efficient: O(n) vs nested loop O(n²)
- Memory efficient: no extra array
- Bahut classic problems ka primary tool


## 2 FLAVORS

### FLAVOR 1: OPPOSITE DIRECTION (meeting)

```
         ┌───┬───┬───┬───┬───┬───┐
         │ A │ B │ C │ D │ E │ F │
         └───┴───┴───┴───┴───┴───┘
           L → → →       ← ← ← R
                    ↓
                Beech mein milte
```

Use: Reverse, palindrome, sorted pair sum, container water


### FLAVOR 2: SAME DIRECTION (slow + fast)

```
         ┌───┬───┬───┬───┬───┬───┐
         │ A │ B │ C │ D │ E │ F │
         └───┴───┴───┴───┴───┴───┘
           S → →
           F → → → → →
           (slow slowly, fast tezi se)
```

Use: Move zeros, remove duplicates, sliding window base, cycle detection


### Key Difference

- OPPOSITE: dono ends se → milne tak (boundary shrinks)
- SAME DIR: dono left se → fast scans, slow places (boundary grows)

= Same TOOL, opposite movement directions


## RECOGNITION SIGNALS

```
SIGNAL 1: "SORTED array mein pair/triplet dhundo"
         = sum target, closest pair
         → OPPOSITE direction

SIGNAL 2: "Symmetry check — start vs end"
         = palindrome, reverse, mirror
         → OPPOSITE direction

SIGNAL 3: "Container / area / range optimize"
         = max water, biggest interval
         → OPPOSITE direction

SIGNAL 4: "In-place rearrange / filter"
         = remove duplicates, move zeros, partition
         → SAME direction (slow places, fast scans)

SIGNAL 5: "Substring / subarray with property"
         = sliding window's base (later pattern)
         → SAME direction (window expands/shrinks)
```

### Quick Decision

```
Sorted + pair/triplet?      → opposite
Symmetry check?             → opposite
In-place modify?            → same direction
Range/area maximize?         → opposite
Window-like requirement?     → same direction
```

### Anti-Signals (jab NA use ho)

- Unsorted array + pair  = HASHMAP better
- Tree / Graph traversal = different patterns
- Multi-dimensional      = matrix/grid patterns


## CLASSIC #1: SORTED PAIR SUM (opposite direction)

```
SORTED Array:  ┌───┬───┬───┬───┬───┐
              │ 2 │ 7 │ 11│ 15│ 19│
              └───┴───┴───┴───┴───┘
Index:          0   1   2   3   4

Target: 18
Question: koi 2 elements aise hai jinka sum = 18?
```

LOGIC:
- left = 0, right = n-1
- sum = array[left] + array[right]
- sum == target → mil gaya, return
- sum < target  → left++ (sum badhana hai, bada element chahiye)
- sum > target  → right-- (sum ghatana hai, chhota element chahiye)
- left >= right → not found

```
DRY RUN:

   left=0, right=4
            ┌───┬───┬───┬───┬───┐
            │ 2 │ 7 │ 11│ 15│ 19│
            └───┴───┴───┴───┴───┘
              L              R
   2 + 19 = 21 > 18  → right--

   left=0, right=3
            ┌───┬───┬───┬───┬───┐
            │ 2 │ 7 │ 11│ 15│ 19│
            └───┴───┴───┴───┴───┘
              L          R
   2 + 15 = 17 < 18  → left++

   left=1, right=3
            ┌───┬───┬───┬───┬───┐
            │ 2 │ 7 │ 11│ 15│ 19│
            └───┴───┴───┴───┴───┘
                   L     R
   7 + 15 = 22 > 18  → right--

   left=1, right=2
            ┌───┬───┬───┬───┬───┐
            │ 2 │ 7 │ 11│ 15│ 19│
            └───┴───┴───┴───┴───┘
                   L  R
   7 + 11 = 18  ✓ MIL GAYA!
```

KEY INTUITION:
- SORTED hone se yeh kaam karta (warna useless)
- sum chhota = left side se bigger chahiye → left aage
- sum bada  = right side se smaller chahiye → right peeche
- O(n) single pass — vs nested loop O(n²)


## CLASSIC #2: REMOVE DUPLICATES FROM SORTED (same direction)

```
SORTED Array:  ┌───┬───┬───┬───┬───┬───┐
              │ 1 │ 1 │ 2 │ 3 │ 3 │ 4 │
              └───┴───┴───┴───┴───┴───┘
Index:          0   1   2   3   4   5

Goal: In-place rakho first k unique values
Return: k (count of unique)
```

LOGIC (SLOW + FAST):
- slow = 0 (unique position pointer)
- fast = 1 (scan pointer)
- array[fast] == array[slow]? → duplicate, skip, fast++
- array[fast] != array[slow]? → new unique, slow++, array[slow] = array[fast]
- End: first (slow+1) elements = unique

```
DRY RUN:

   slow=0, fast=1
            ┌───┬───┬───┬───┬───┬───┐
            │ 1 │ 1 │ 2 │ 3 │ 3 │ 4 │
            └───┴───┴───┴───┴───┴───┘
              S   F
   array[1]=1 == array[0]=1?  haan, skip, fast++

   slow=0, fast=2
            ┌───┬───┬───┬───┬───┬───┐
            │ 1 │ 1 │ 2 │ 3 │ 3 │ 4 │
            └───┴───┴───┴───┴───┴───┘
              S       F
   array[2]=2 != array[0]=1?  haan
   slow++, array[slow]=array[fast]
            ┌───┬───┬───┬───┬───┬───┐
            │ 1 │ 2 │ 2 │ 3 │ 3 │ 4 │
            └───┴───┴───┴───┴───┴───┘
                   S   F

   fast=3: array[3]=3 != array[1]=2? haan
   slow++, array[slow]=3
            ┌───┬───┬───┬───┬───┬───┐
            │ 1 │ 2 │ 3 │ 3 │ 3 │ 4 │
            └───┴───┴───┴───┴───┴───┘
                        S  F

   fast=4: array[4]=3 == array[2]=3? haan, skip

   fast=5: array[5]=4 != array[2]=3? haan
   slow++, array[slow]=4
            ┌───┬───┬───┬───┬───┬───┐
            │ 1 │ 2 │ 3 │ 4 │ 3 │ 4 │
            └───┴───┴───┴───┴───┴───┘
                            S      F

   END: slow=3, count = slow+1 = 4
   First 4 elements unique: [1, 2, 3, 4]   ✓
```

KEY INTUITION:
- SORTED hai = duplicates adjacent
- Slow = "next unique kahaan place karna"
- Fast = "scan + check duplicate"
- Beech ke duplicate ignore, unique aage place
- O(n) in-place, no extra array


## CLASSIC #3: PALINDROME CHECK (opposite direction)

Word/array reverse karke same?

Examples:
- "RACECAR" = palindrome (reverse bhi same)
- "HELLO"   = NOT palindrome

```
String "RACECAR" ko array maano:
         ┌───┬───┬───┬───┬───┬───┬───┐
         │ R │ A │ C │ E │ C │ A │ R │
         └───┴───┴───┴───┴───┴───┴───┘
Index:    0   1   2   3   4   5   6
```

LOGIC:
- left = 0, right = n-1
- array[left] == array[right]?
  - haan: left++, right--
  - nahi: NOT palindrome, return false
- left >= right: palindrome, return true

```
DRY RUN ("RACECAR"):

   left=0, right=6
            ┌───┬───┬───┬───┬───┬───┬───┐
            │ R │ A │ C │ E │ C │ A │ R │
            └───┴───┴───┴───┴───┴───┴───┘
              L                       R
   R == R?  haan, left++, right--

   left=1, right=5
            ┌───┬───┬───┬───┬───┬───┬───┐
            │ R │ A │ C │ E │ C │ A │ R │
            └───┴───┴───┴───┴───┴───┴───┘
                   L                R
   A == A?  haan, left++, right--

   left=2, right=4
            ┌───┬───┬───┬───┬───┬───┬───┐
            │ R │ A │ C │ E │ C │ A │ R │
            └───┴───┴───┴───┴───┴───┴───┘
                        L     R
   C == C?  haan, left++, right--

   left=3, right=3
   left >= right → STOP
   = PALINDROME ✓
```

```
DRY RUN ("HELLO"):

   left=0, right=4
            ┌───┬───┬───┬───┬───┐
            │ H │ E │ L │ L │ O │
            └───┴───┴───┴───┴───┘
              L              R
   H == O?  NAHI → return FALSE
   = NOT palindrome
```

KEY INTUITION:
- Symmetry check = opposite direction NATURAL fit
- "Mismatch milte hi exit" — pure scan nahi karna
- Reverse problem ka cousin (same mechanism)


## CLASSIC #4: CONTAINER WITH MOST WATER (opposite, optimization)

Heights array — kaunse 2 bars sabse zyada paani roken?

```
heights = [4, 1, 6, 3, 2, 5]

   6 │     ┌─┐
   5 │     │ │            ┌─┐
   4 │┌─┐  │ │            │ │
   3 ││ │  │ │  ┌─┐       │ │
   2 ││ │  │ │  │ │  ┌─┐  │ │
   1 ││ │┌─┐│ │  │ │  │ │  │ │
     └┴─┴┴─┴┴─┴──┴─┴──┴─┴──┴─┴──
index: 0   1   2   3   4   5

Water = min(left_h, right_h) × (right - left)
Goal: max water
```

LOGIC (OPPOSITE + SMART MOVE):
- left = 0, right = n-1
- water = min(h[left], h[right]) × (right - left)
- max_water update
- MOVE SHORTER SIDE INWARD
  - Longer side waste hota agar shorter taller hota
  - Shorter ko replace karke hi badi chance

```
DRY RUN: heights = [4, 1, 6, 3, 2, 5]

   left=0(h=4), right=5(h=5)
   water = min(4,5) × (5-0) = 4 × 5 = 20    max=20
   shorter = left (4<5), left++

   left=1(h=1), right=5(h=5)
   water = min(1,5) × (5-1) = 1 × 4 = 4     max=20
   shorter = left, left++

   left=2(h=6), right=5(h=5)
   water = min(6,5) × (5-2) = 5 × 3 = 15    max=20
   shorter = right (5<6), right--

   left=2(h=6), right=4(h=2)
   water = min(6,2) × (4-2) = 2 × 2 = 4     max=20
   shorter = right, right--

   left=2(h=6), right=3(h=3)
   water = min(6,3) × (3-2) = 3 × 1 = 3     max=20
   shorter = right, right--

   left=2, right=2 → STOP

   max water = 20   ✓
```

KEY INTUITION:
- Width sabse bada start mein (extremes se)
- Width ghatne wali = height badhani padegi
- SHORTER side bottleneck = usi ko move karo
- Longer side ko hata diya = guarantee water kam
= "Greedy move shorter" intuition lock


## CLASSIC #5: 3-SUM (extends two pointer)

```
SORTED Array:  ┌────┬────┬───┬───┬───┐
              │ -2 │ -1 │ 0 │ 1 │ 3 │
              └────┴────┴───┴───┴───┘
Index:           0    1    2   3   4

Goal: All unique triplets jinka sum = 0
```

LOGIC (FIX 1 + TWO POINTER ON REST):
- Outer loop: i = 0 to n-3 (fix one element)
- Inner: two pointer on remaining
  - left = i+1, right = n-1
  - target for pair = 0 - array[i]
- Same opposite two-pointer logic
- sum == target → triplet mil gaya, save + move both
- sum < target  → left++
- sum > target  → right--

```
DRY RUN:

   i=0, array[0]=-2, need pair sum = 0-(-2) = 2

     left=1(-1), right=4(3):  -1 + 3 = 2  ✓
     TRIPLET 1: (-2, -1, 3)
     left++, right--

     left=2(0), right=3(1):   0 + 1 = 1   < 2, left++
     left=3, right=3 STOP


   i=1, array[1]=-1, need pair sum = 0-(-1) = 1

     left=2(0), right=4(3):   0 + 3 = 3   > 1, right--
     left=2(0), right=3(1):   0 + 1 = 1   ✓
     TRIPLET 2: (-1, 0, 1)
     left++, right--

     left=3, right=2 STOP


   i=2, array[2]=0, need pair sum = 0

     left=3(1), right=4(3):   1 + 3 = 4   > 0, right--
     left=3, right=3 STOP


   i=3, array[3]=1: positive — sorted mein aage sab positive
                    sum=0 possible nahi, skip
```

```
RESULT: 2 triplets
   (-2, -1, 3)
   (-1, 0, 1)   ✓
```

KEY INTUITION (EXTENSION TO 3D):

```
   2-SUM:    1 pair, 2 pointers
   3-SUM:    1 fixed + 1 pair, 3 elements
   K-SUM:    K-2 fixed + 1 pair (recursive pattern)
```

- Two pointer outer loop ke saath nest karke extend hota
- SORTED hone se kaam karta (warna O(n³) brute)
- O(n²) total — significant improvement


## SUMMARY

```
5 CLASSICS COVERED:

   Sorted Pair Sum         - opposite, sum comparison
   Remove Duplicates       - same direction, slow places
   Palindrome Check        - opposite, symmetry check
   Container Most Water    - opposite, greedy move shorter
   3-Sum                   - outer loop + two pointer (extension)

= Dono flavors + optimization + extension covered
= Pattern recognition + tool deployment ready
```
