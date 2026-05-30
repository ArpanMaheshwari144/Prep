# Prefix Sum — Pattern Intuition


## WHAT IS PREFIX SUM

Pattern: Ek baar "running total" array bana lo (shuru se har index tak ka sum). Phir koi bhi range ka sum = O(1) — do number ka ghatav. Baar baar range query poochi jaaye = yahi tool.

```
   Original:  ┌───┬───┬───┬───┬───┐
             │ 2 │ 4 │ 1 │ 3 │ 5 │
             └───┴───┴───┴───┴───┘
   index:      0   1   2   3   4

   Prefix:    ┌───┬───┬───┬───┬───┐
             │ 2 │ 6 │ 7 │10 │15 │
             └───┴───┴───┴───┴───┘
              ↑   ↑   ↑    ↑    ↑
              2  2+4 6+1 7+3  10+5
```

prefix[i] = index 0 se i tak ka TOTAL sum
= "shuru se yahaan tak kitna jama hua" (bank balance jaisa)


## RANGE SUM — O(1) MAGIC

```
   FORMULA:  sum(i..j) = prefix[j] - prefix[i-1]
```

```
   sum(1..3) on [2,4,1,3,5]:
   prefix = [2,6,7,10,15]
   = prefix[3] - prefix[0]
   = 10 - 2 = 8     ✓ (4+1+3)
```

WHY: "bade total" (0..j) mein se "chhota total" (0..i-1) ghatao = beech ka hissa bachta hai.

```
   EDGE CASE (i=0):
   sum(0..j) = prefix[j]  seedha
   (prefix[i-1] = prefix[-1] = 0, kuch ghatana nahi)
```


## KAB USE — RECOGNITION SIGNALS

| Signal | Approach |
|--------|----------|
| "Range sum baar baar" (multiple queries) | prefix array, O(1)/query |
| "Subarray sum = K" (count/exist) | prefix + HashMap |
| "Pivot / equilibrium index" | left prefix vs (total - left) |
| "Cumulative anything in range" | prefix of that property |
| "Range product / XOR" (variant) | prefix product / prefix XOR |

### ANTI-SIGNAL (jab NA use ho)
- Sirf 1 baar range sum         → simple loop (precompute waste)
- Contiguous MAX sum            → Kadane's (alag pattern)
- Fixed-size window             → Sliding Window


## CLASSIC #1: RANGE SUM QUERY (Immutable)

```
   [-2, 0, 3, -5, 2]   queries: sum(0..2), sum(2..4), sum(1..3)
```

STEP 1 — prefix banao:
```
   prefix[0]=-2
   prefix[1]=-2+0=-2
   prefix[2]=-2+3=1
   prefix[3]=1+(-5)=-4
   prefix[4]=-4+2=-2
   prefix = [-2,-2,1,-4,-2]
```

STEP 2 — queries (prefix[j] - prefix[i-1]):
```
   sum(0..2): i=0 → prefix[2] = 1            ✓ (-2+0+3)
   sum(2..4): prefix[4]-prefix[1] = -2-(-2)=0 ✓ (3-5+2)
   sum(1..3): prefix[3]-prefix[0] = -4-(-2)=-2 ✓ (0+3-5)
```

SOUL: "Precompute once (O(n)), answer many (O(1) each)". 1000 queries = har ek still O(1).


## CLASSIC #2: PIVOT INDEX (Equilibrium)

```
   [1, 7, 3, 6, 5, 6]
   → woh index jahaan LEFT sum == RIGHT sum (pivot khud count nahi)
```

LOGIC: total = pura sum. left = 0 se. Har i pe right = total - left - current. left==right? → pivot. Nahi → left += current.

```
   DRY RUN:  total = 28, left = 0
   i=0: right=28-0-1=27, left(0)==27? NAHI → left=1
   i=1: right=28-1-7=20, left(1)==20? NAHI → left=8
   i=2: right=28-8-3=17, left(8)==17? NAHI → left=11
   i=3: right=28-11-6=11, left(11)==11? HAAN ✓ → PIVOT=3
   check: left 1+7+3=11, right 5+6=11 ✓
```

KEY: right ke liye alag loop NAHI — total - left - current = right (ek formula). left running banti jaati (prefix concept).


## CLASSIC #3: SUBARRAY SUM = K (Prefix + HashMap)

```
   [1, 2, 3, 1]   k=3   → kitne contiguous subarrays ka sum = 3?
```

PREFIX INTUITION (range formula ULTA):
```
   chahiye: prefix[j] - prefix[i-1] = k
   = prefix[i-1] = prefix[j] - k
   = "current prefix pe khade, (prefix - k) pehle dekha kya?" → HashMap
```

LOGIC: map={0:1} (empty prefix). prefix=0, count=0. Har element: prefix += current; need = prefix - k; need map mein → count += map[need]; map[prefix]++.

```
   DRY RUN (k=3) — map SIRF GROW karta, kuch hatta nahi:
   START:  map={0:1}
   i=0: prefix=1, need=-2 NAHI → map={0:1, 1:1}
   i=1: prefix=3, need=0 HAAN(0:1) count=1 → map={0:1,1:1,3:1}  ([1,2])
   i=2: prefix=6, need=3 HAAN(3:1) count=2 → map={0:1,1:1,3:1,6:1} ([3])
   i=3: prefix=7, need=4 NAHI → map={0:1,1:1,3:1,6:1,7:1}
   count = 2  ✓
```

map[prefix] = "yeh prefix kitni baar dekha". map={0:1} start = shuru se sum=k handle. NEGATIVE numbers safe (Sliding Window yahaan FAIL hota).

= Prefix + HashMap = subarray-sum counting ka gold combo (Hashing pattern se overlap).


## CLASSIC #4: PRODUCT OF ARRAY EXCEPT SELF (variant)

```
   [1, 2, 3, 4]
   → answer[i] = baaki SAB ka product (khud chhod ke), DIVISION nahi
```

IDEA: left[i] = i se pehle sab ka product; right[i] = i ke baad sab ka product; answer = left × right.

```
   LEFT (left→right, khud se pehle):
   left = [1, 1, 2, 6]      (1, 1, 1x2, 1x2x3)

   RIGHT (right→left, khud se baad):
   right = [24, 12, 4, 1]   (4x3x2, 4x3, 4, 1)

   ANSWER = left × right:
   [1x24, 1x12, 2x4, 6x1] = [24, 12, 8, 6]  ✓
```

KEY: prefix concept sum ke alawa PRODUCT pe bhi. Division avoid (zero = crash). Generalize: sum / product / XOR — koi bhi cumulative.


## POWER PHRASES

- "Precompute once, answer many — prefix ka soul."
- "Range sum = prefix[j] - prefix[i-1]. Bada total minus chhota total."
- "Subarray sum = K? Prefix + HashMap. Negative-safe (window fail karta)."
- "Pivot index = left prefix vs (total - left - current)."
- "Prefix generalize: sum, product, XOR — koi bhi cumulative property."


## TRAP BOX

```
┌─────────────────────────────────────────────────────────┐
│ TRAP 1: i=0 pe prefix[i-1] = prefix[-1]                  │
│   → Treat as 0. sum(0..j) = prefix[j] seedha.            │
│                                                          │
│ TRAP 2: Subarray Sum = K — map={0:1} bhul jaana          │
│   → Empty prefix miss, shuru-se-sum=k count nahi hoga.   │
│                                                          │
│ TRAP 3: Subarray Sum pe Sliding Window try karna         │
│   → Negative numbers pe window FAIL. Prefix+map use.     │
│                                                          │
│ TRAP 4: Single range query ke liye prefix banana         │
│   → Ek hi query = simple loop. Precompute overkill.      │
│                                                          │
│ TRAP 5: Product Except Self mein division use karna      │
│   → Zero element = crash. left×right (prefix/suffix).    │
└─────────────────────────────────────────────────────────┘
```
