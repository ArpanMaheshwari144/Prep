# Sliding Window — Pattern Intuition


## WHAT IS SLIDING WINDOW

Pattern: Array ke andar ek "window" maintain karo, window slide karte raho left to right, window ke andar ki property track karo.

```
         ┌───┬───┬───┬───┬───┬───┬───┐
         │ A │ B │ C │ D │ E │ F │ G │
         └───┴───┴───┴───┴───┴───┴───┘
               [─ window ─]
                    ↓ slide right
         ┌───┬───┬───┬───┬───┬───┬───┐
         │ A │ B │ C │ D │ E │ F │ G │
         └───┴───┴───┴───┴───┴───┴───┘
                   [─ window ─]
```

= Window khisakta jaata
= Har position pe property check


## WHY USE

- Subarray / substring problems = primary tool
- O(n) single pass (vs nested O(n²))
- Memory: in-place (no extra array usually)
- "Max sum of size k", "longest substring", etc. = textbook


## 2 FLAVORS

### FLAVOR 1: FIXED SIZE (size k always)

```
         ┌───┬───┬───┬───┬───┬───┐
         │ 2 │ 5 │ 1 │ 7 │ 3 │ 4 │
         └───┴───┴───┴───┴───┴───┘
           [─ k=3 ─]
               ↓ slide right
         ┌───┬───┬───┬───┬───┬───┐
         │ 2 │ 5 │ 1 │ 7 │ 3 │ 4 │
         └───┴───┴───┴───┴───┴───┘
                [─ k=3 ─]
```

- Window SIZE never changes
- Use: max sum of k, average of k, anagram check


### FLAVOR 2: VARIABLE SIZE (window expands/shrinks)

```
         ┌───┬───┬───┬───┬───┬───┐
         │ A │ B │ C │ A │ B │ D │
         └───┴───┴───┴───┴───┴───┘
           [─ window grows ─]
           left            right
```

- Condition break? → left aage badh, window SHRINK
- Condition OK? → right aage badh, window GROW
- Window SIZE varies based on condition
- Use: longest substring no-repeat, smallest sum >= target


### Key Difference

- FIXED:    size locked, just slide
- VARIABLE: size flex, condition controls

= Same TOOL, different control logic


## RECOGNITION SIGNALS

```
SIGNAL 1: Keyword "subarray" ya "substring"
         (always = contiguous chunk)

SIGNAL 2: "Of size K" — fixed
         = "max/min/avg/sum of k-sized subarray"
         → FIXED window

SIGNAL 3: "Longest / shortest with X property"
         = "longest substring no-repeat"
         = "shortest subarray with sum >= target"
         → VARIABLE window

SIGNAL 4: "Count of X over window"
         = "number of subarrays with sum = K"
         → mostly FIXED or hybrid

SIGNAL 5: Brute force O(n²) lag raha
         ek nested loop dekha = sliding window try
         = optimize to O(n)
```

### Quick Decision

```
"Size k subarray" mention?       → FIXED
"Longest / shortest valid"?      → VARIABLE
Contiguous required?              → sliding window candidate
Sorted + pair (not subarray)?    → Two Pointer (not SW)
```

### Anti-Signals (jab NA use ho)

- Subset / subsequence (non-contiguous)  = DP / Backtracking
- Tree / Graph traversal                  = BFS/DFS
- Sorted array pair sum                   = Two Pointer
- Random index access                     = HashMap


## CLASSIC #1: MAX SUM SUBARRAY OF SIZE K (fixed window)

```
Array:  ┌───┬───┬───┬───┬───┬───┐
       │ 2 │ 5 │ 1 │ 7 │ 3 │ 4 │
       └───┴───┴───┴───┴───┴───┘
k = 3
Goal: Size-3 ka window slide karo, max sum dhundo
```

LOGIC (FIXED WINDOW):
- Pehli window [0 to k-1] ka sum compute karo
- Slide karo: remove leftmost, add new rightmost
- Har step pe max update
- End mein max = answer

```
DRY RUN:

   Step 1: Initial window [0..2]
            ┌───┬───┬───┬───┬───┬───┐
            │ 2 │ 5 │ 1 │ 7 │ 3 │ 4 │
            └───┴───┴───┴───┴───┴───┘
              [─ window ─]
            sum = 2 + 5 + 1 = 8
            max = 8

   Step 2: Slide right — remove 2, add 7
            sum = 8 - 2 + 7 = 13
            max = 13

   Step 3: Slide right — remove 5, add 3
            sum = 13 - 5 + 3 = 11
            max = 13

   Step 4: Slide right — remove 1, add 4
            sum = 11 - 1 + 4 = 14
            max = 14

   END: max sum = 14  ✓
```

KEY INTUITION (FIXED WINDOW):
- Naive (O(n*k)): har window ka sum ek-ek karke add
- Sliding (O(n)): pehla sum once, phir reuse + delta
  - "Window shift = -leftmost +newrightmost"
  - redundant work eliminate
- Window = running total, slide = math op (not full recompute)


## CLASSIC #2: SMALLEST SUBARRAY WITH SUM >= TARGET (variable, sum)

```
Array:  ┌───┬───┬───┬───┬───┬───┐
       │ 2 │ 3 │ 1 │ 2 │ 4 │ 3 │
       └───┴───┴───┴───┴───┴───┘
target = 7
Goal: SMALLEST contiguous subarray jiska sum >= 7
```

LOGIC (VARIABLE — SUM TRACKING):
- left = 0, sum = 0, min_len = infinity
- right ko aage badhao, sum += array[right]
- JAB TAK sum >= target:
  - min_len update (current window size)
  - shrink: sum -= array[left], left++
- End mein min_len = answer

```
DRY RUN:

   right=0..2: sum builds to 6 (all < 7, just expand)

   right=3: sum = 8 (>= 7, enter shrink)
            window [2,3,1,2] len=4
            min_len = 4
            shrink: sum=6, left=1, stop (6<7)

   right=4: sum = 6+4 = 10 (>= 7, shrink)
            min_len stays 4
            shrink: sum=7, left=2
            min_len = 3 (window [1,2,4])
            shrink: sum=6, left=3, stop

   right=5: sum = 6+3 = 9 (>= 7, shrink)
            min_len = 3
            shrink: sum=7, left=4
            min_len = 2 (window [4,3])
            shrink: sum=3, left=5, stop

   END: min_len = 2  ✓  Window: [4, 3]
```

KEY INTUITION:
- Expand: right ko badhao jab tak condition MEET nahi
- Shrink: condition MILA = left ko aage karke try shrink
- Track: har valid window pe min/max update
- "Greedy expand till condition, then greedy shrink"
- Right + left dono aage chalte = O(2n) = O(n)


## CLASSIC #3: MAX CONSECUTIVE 1s WITH K FLIPS (variable, count)

```
Array:  ┌───┬───┬───┬───┬───┬───┐
       │ 1 │ 0 │ 1 │ 0 │ 1 │ 1 │
       └───┴───┴───┴───┴───┴───┘
K = 1 (at most 1 zero flip allowed)
Goal: LONGEST subarray of 1s (zeros K se zyada hone NA do)
```

LOGIC (VARIABLE — COUNT TRACKING):
- left = 0, zeros_count = 0, max_len = 0
- right ko expand
  - array[right] = 0? → zeros_count++
- JAB zeros_count > K:
  - shrink: agar array[left]=0, zeros_count--; left++
- max_len update har step

```
DRY RUN:

   right=0: 1, zeros=0, window=1, max=1
   right=1: 0, zeros=1, window=2, max=2
   right=2: 1, zeros=1, window=3, max=3

   right=3: 0, zeros=2 (>K!) Shrink:
            array[0]=1, left=1, zeros still 2
            array[1]=0, zeros=1, left=2, stop
            window left=2 to 3, size=2, max stays 3

   right=4: 1, zeros=1, window=3
   right=5: 1, zeros=1, window=4, max=4

   END: max = 4  ✓
   Best window: [1, 0, 1, 1] (one 0 flipped to 1)
```

KEY INTUITION:
- Condition = "zeros_count <= K"
- Track ZEROS_COUNT, not sum
- Same expand/shrink pattern as Classic #2
- Different "tracked quantity"


## CLASSIC #4: LONGEST SUBSTRING NO-REPEAT (variable, set)

```
String:  "abcabc"
         ┌───┬───┬───┬───┬───┬───┐
         │ a │ b │ c │ a │ b │ c │
         └───┴───┴───┴───┴───┴───┘
Goal: Longest substring with ALL unique chars
```

LOGIC (VARIABLE + SET):
- left = 0, set = {}, max = 0
- right expand
- char already in set?
  - YES: shrink — remove array[left] from set, left++, repeat
  - NO: add char to set
- max update

```
DRY RUN:

   right=0: 'a' not in set. set={a}. window=1. max=1.
   right=1: 'b' not in set. set={a,b}. window=2. max=2.
   right=2: 'c' not in set. set={a,b,c}. window=3. max=3.

   right=3: 'a' IN set! Shrink:
            remove array[0]='a', set={b,c}, left=1
            stop. add 'a': set={b,c,a}. window=3.

   right=4: 'b' IN set! Shrink:
            remove array[1]='b', set={c,a}, left=2
            stop. add 'b': set={c,a,b}. window=3.

   right=5: 'c' IN set! Shrink:
            remove array[2]='c', set={a,b}, left=3
            stop. add 'c': set={a,b,c}. window=3.

   END: max = 3  ✓
```

KEY INTUITION:
- Window mein "duplicates not allowed" rule
- Set track karta "kya hai window mein"
- Duplicate aaya = shrink till duplicate gone
- Set = window membership ka mirror


## CLASSIC #5: LONGEST SUBSTRING WITH K DISTINCT (variable, map)

```
String:  "eceba"
         ┌───┬───┬───┬───┬───┐
         │ e │ c │ e │ b │ a │
         └───┴───┴───┴───┴───┘
K = 2 (at most 2 distinct chars)
Goal: Longest substring with <= 2 distinct chars
```

LOGIC (VARIABLE + MAP):
- left = 0, map = {char → count}, max = 0
- right expand: map[char]++
- map.size > K? Shrink:
  - map[array[left]]--; if count=0 remove
  - left++
- max update

```
DRY RUN:

   right=0: 'e'. map={e:1}. distinct=1. window=1. max=1.
   right=1: 'c'. map={e:1,c:1}. distinct=2. window=2. max=2.
   right=2: 'e'. map={e:2,c:1}. distinct=2. window=3. max=3.

   right=3: 'b'. map={e:2,c:1,b:1}. distinct=3 (>K!). Shrink:
            array[0]='e', map[e]=1, distinct still 3. left=1.
            array[1]='c', map[c]=0 remove. map={e:1,b:1}. distinct=2. left=2.
            stop. window="eb". size=2. max stays 3.

   right=4: 'a'. map={e:1,b:1,a:1}. distinct=3. Shrink:
            array[2]='e', map[e]=0 remove. map={b:1,a:1}. distinct=2. left=3.
            stop. window="ba". size=2. max stays 3.

   END: max = 3  ✓
   Window: "ece" (e, c, e — only 2 distinct)
```

KEY INTUITION (SET vs MAP):

```
SET    = window mein "kya hai" (membership only)
         Use: "no repeat" (boolean)

MAP    = window mein "kya + kitna" (count tracking)
         Use: "at most K distinct", "exact count"
```

= Same SW pattern, alag tracker


## SUMMARY — COMPRESSED MENTAL MODEL

```
FIXED SIZE (K given):       → bas slide karo, easy
VARIABLE SIZE:              → condition-driven shrink
                            → condition kuch bhi:
                               - sum (Classic #2)
                               - count (Classic #3)
                               - set membership (Classic #4)
                               - map count (Classic #5)

= Bas yeh hai. Tool same, tracker alag.
```


## 5 CLASSICS COVERED

```
1. Max Sum Size K           - fixed
2. Smallest Sum >= Target   - variable, sum
3. Max Consecutive 1s flips - variable, count
4. Longest No-Repeat        - variable, set
5. K Distinct chars         - variable, map

= Pattern recognition + tool deployment ready
```
