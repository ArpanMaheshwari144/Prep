# Arrays — Intuition (from scratch)


## 1. ARRAY KYA HAI

Ek line mein rakhe hue identical boxes.
Har box mein ek item.
Har box ka apna number (index) — 0 se shuru.

```
Index:    0     1     2     3     4
         ┌───┬───┬───┬───┬───┐
         │ 10│ 20│ 30│ 40│ 50│
         └───┴───┴───┴───┴───┘
```

= Ek line, fixed boxes, har box ek value, har box ka apna address.


## 2. PEHLA POWER — DIRECT ACCESS

Index pata = box turant mil jaata.
Pehla box check karke aage nahi jaana padta.

```
Index 3 chahiye?
         ┌───┬───┬───┬─⭢─┬───┐
         │ 10│ 20│ 30│ 40│ 50│
         └───┴───┴───┴───┴───┘
```

= Seedha jaake utha lo. Ek step.
= Index 0 ho ya index 1000 — same speed.

```
array[3] = 40
```


## 3. DIRECT ACCESS QYUN POSSIBLE — MEMORY LAYOUT

Boxes memory mein saath-saath rakhe hote (contiguous).
Har box ka size same hota (e.g. 4 bytes for int).

```
address:  1000  1004  1008  1012  1016
         ┌───┬───┬───┬───┬───┐
         │ 10│ 20│ 30│ 40│ 50│
         └───┴───┴───┴───┴───┘
index:     0     1     2     3     4
```

= Start address + box size = math se kisi bhi index ka address
= array[3] = 1000 + (3 × 4) = 1012
= Computer ke liye = ek calculation, ek jump


## 4. ARRAY KA RULE — SIZE FIXED

Banate time bolna padta — kitne boxes chahiye.
Phir badha NAHI sakte.

```
"Mujhe 5 boxes chahiye"
         ┌───┬───┬───┬───┬───┐
         │ 10│ 20│ 30│ 40│ 50│
         └───┴───┴───┴───┴───┘
```

6th item add karna hai? Possible NAHI.
= Yeh slot already booked, around mein dusra data padha
= Array bigaad nahi sakte

Why fixed?
- Contiguous memory chahiye direct access ke liye
- "Bagal ka address khali hai?" = guarantee nahi
- OS ne kahin aur data rakh diya hoga
= Banate time hi commit karna padta


## 5. DUSRA PAIN — INSERT/DELETE MIDDLE = COSTLY

Index 2 pe naya item ghusao = baaki sab ko right shift karna padta.

```
Before:
         ┌───┬───┬───┬───┬───┐
         │ 10│ 20│ 30│ 40│ 50│
         └───┴───┴───┴───┴───┘

Insert 99 at index 2:
         ┌───┬───┬───┬───┬───┐
         │ 10│ 20│ 99│ 30│ 40│  ← 50 baahar gir gaya (fixed size)
         └───┴───┴───┴───┴───┘
                    ⭢ ⭢ ⭢
         = 30, 40, 50 ko ek-ek karke khiskana
```

= 1 lakh boxes = 1 lakh shifts worst case
= Beech mein chhed na = mehnga

Delete bhi same — left shift to fill gap.


## 6. SEARCH — VALUE DHUNDNA

Tujhe pata: index 3 = 40 ka value
Tujhe NAHI pata: 40 kis index pe hai

Toh kya karega?

```
         ┌───┬───┬───┬───┬───┐
         │ 10│ 20│ 30│ 40│ 50│
         └───┴───┴───┴───┴───┘
          ⭡    ⭡    ⭡    ⭡
         10?  20?  30?  40? = mil gaya, index 3
```

= Ek-ek box dekho jab tak nahi milta
= Worst case: last box mein milega (ya nahi mile)


KEY DISTINCTION:
- Index pata    → direct jump (1 step)
- Value pata    → ek-ek scan (n steps worst)


## 7. ARRAY KA PERSONALITY SUMMARY

```
FAST  (1 step):
- Index se value uthana          array[3]
- Index pe value badalna          array[3] = 99

SLOW (n steps worst case):
- Value dhundna (no index)        scan one by one
- Beech mein insert               shift karna
- Beech mein delete               shift karna

FIXED:
- Size banate time decide
- Baad mein bada/chhota nahi
```


ARRAY = MUSCLE MEMORY:
- Strong = random access (index ke saath)
- Weak   = modification + size change


## KAB ARRAY USE KARO

- Size pata pehle se
- Bahut padhna (read), kam likhna (modify)
- Index-based access primary need

KAB NA KARO:
- Size unpredictable (grow/shrink karega)
- Bahut insert/delete middle mein


## 8. ITERATION — ARRAY MEIN GHOOMNA

Sab boxes pe kaam karna hota = ek-ek visit karte ja.

```
Forward (left to right):
         ┌───┬───┬───┬───┬───┐
         │ 10│ 20│ 30│ 40│ 50│
         └───┴───┴───┴───┴───┘
           ⭢    ⭢    ⭢    ⭢    ⭢
         i=0  i=1  i=2  i=3  i=4

Backward (right to left):
         ┌───┬───┬───┬───┬───┐
         │ 10│ 20│ 30│ 40│ 50│
         └───┴───┴───┴───┴───┘
         ⭠    ⭠    ⭠    ⭠    ⭠
        i=4  i=3  i=2  i=1  i=0
```

= "i" ko start se end, ek-ek karke
= Har step pe array[i] mein value
= "Har step pe kya kaam" — operation pe depend


## 9. MAX FIND — CHAMPION TRACK

```
Array:    ┌───┬───┬───┬───┬───┐
         │ 10│ 50│ 20│ 80│ 40│
         └───┴───┴───┴───┴───┘
```

LOGIC:
- max = pehla element
- Iterate har box pe
- Current > max? → max update
- End mein max = answer

```
DRY RUN:
   i=0: array[0]=10, max=10        → max=10
   i=1: array[1]=50, 50 > 10?      → haan, max=50
   i=2: array[2]=20, 20 > 50?      → nahi, max=50
   i=3: array[3]=80, 80 > 50?      → haan, max=80
   i=4: array[4]=40, 40 > 80?      → nahi, max=80
   END: max = 80   ✓
```

KEY INTUITION:
- "Champion" track karte ja
- Naya challenger aaye to compare
- Bada mile = champion replace
- End mein bach gaya = winner


## 10. SUM — BUCKET FILL

```
Array:    ┌───┬───┬───┬───┬───┐
         │ 10│ 20│ 30│ 40│ 50│
         └───┴───┴───┴───┴───┘
```

LOGIC:
- sum = 0 (empty bucket)
- Iterate har box pe
- Har box ka value bucket mein daal
- End mein bucket = answer

```
DRY RUN:
   sum = 0
   i=0: sum = 0+10  = 10
   i=1: sum = 10+20 = 30
   i=2: sum = 30+30 = 60
   i=3: sum = 60+40 = 100
   i=4: sum = 100+50 = 150
   END: sum = 150   ✓
```

KEY INTUITION:
- Empty bucket = 0 se start
- Har step pe value daalte ja
- End mein bucket = total


## 11. REVERSE — TWO POINTER INTRO

```
Original:  ┌───┬───┬───┬───┬───┐
          │ 10│ 20│ 30│ 40│ 50│
          └───┴───┴───┴───┴───┘
Want:      ┌───┬───┬───┬───┬───┐
          │ 50│ 40│ 30│ 20│ 10│
          └───┴───┴───┴───┴───┘
```

LOGIC (do hath):
- Ek hath left se (start)
- Ek hath right se (end)
- Dono boxes swap karo
- Left ko right move, right ko left move
- Jab dono mil jaye = done

```
DRY RUN:

   left=0, right=4
            ┌───┬───┬───┬───┬───┐
            │ 10│ 20│ 30│ 40│ 50│
            └───┴───┴───┴───┴───┘
              L              R
   Swap 10 ↔ 50
            ┌───┬───┬───┬───┬───┐
            │ 50│ 20│ 30│ 40│ 10│
            └───┴───┴───┴───┴───┘

   left=1, right=3
            ┌───┬───┬───┬───┬───┐
            │ 50│ 20│ 30│ 40│ 10│
            └───┴───┴───┴───┴───┘
                   L         R
   Swap 20 ↔ 40
            ┌───┬───┬───┬───┬───┐
            │ 50│ 40│ 30│ 20│ 10│
            └───┴───┴───┴───┴───┘

   left=2, right=2 → STOP (mil gaye)

   FINAL: 50 40 30 20 10   ✓
```

KEY INTUITION (TWO POINTER concept):
- Do indices ek saath chalti
- One end → other end
- Swap / compare beech mein
= Sliding window / palindrome / etc. ka base


## 12. COUNT OCCURRENCES — COUNTER INCREMENT

```
Kitni baar "20" aata array mein?

Array:    ┌───┬───┬───┬───┬───┬───┐
         │ 10│ 20│ 30│ 20│ 40│ 20│
         └───┴───┴───┴───┴───┴───┘
Index:    0    1    2    3    4    5
```

LOGIC:
- count = 0
- Iterate har box pe
- Box value == target? → count++
- End mein count = answer

```
DRY RUN (target=20):
   count = 0
   i=0: 10 == 20? nahi             count=0
   i=1: 20 == 20? haan, count++    count=1
   i=2: 30 == 20? nahi             count=1
   i=3: 20 == 20? haan, count++    count=2
   i=4: 40 == 20? nahi             count=2
   i=5: 20 == 20? haan, count++    count=3
   END: count = 3   ✓
```


## 13. CHECK IF SORTED — NEIGHBOR COMPARE

Sorted means har element pichle se BADA ya EQUAL.

```
Sorted:    ┌───┬───┬───┬───┬───┐
          │ 10│ 20│ 30│ 40│ 50│   ✓ haan
          └───┴───┴───┴───┴───┘

Not sorted:┌───┬───┬───┬───┬───┐
          │ 10│ 50│ 30│ 40│ 20│   ✗ nahi (50 ke baad 30)
          └───┴───┴───┴───┴───┘
```

LOGIC:
- Iterate index 1 se (index 0 ka pichla nahi)
- Har step pe: array[i] < array[i-1]?
  - haan = NOT sorted, turant return false
- Loop khatam = sorted, return true

```
DRY RUN (sorted case):
   array = [10, 20, 30, 40, 50]
   i=1: 20 < 10?   nahi    ✓
   i=2: 30 < 20?   nahi    ✓
   i=3: 40 < 30?   nahi    ✓
   i=4: 50 < 40?   nahi    ✓
   END: return TRUE   ✓
```

```
DRY RUN (not sorted):
   array = [10, 50, 30, 40, 20]
   i=1: 50 < 10?   nahi    ✓
   i=2: 30 < 50?   HAAN    ✗ → return FALSE turant

= Pure array scan nahi karna padta
= Pehla violation = exit
```

KEY INTUITION:
- "Neighbor compare" pattern
- Index i vs index i-1 (ya i vs i+1)
- Sliding window / monotonic checks ka building block
- Early exit: jaise hi violation = stop (efficient)


## 14. MIN FIND — WEAKEST TRACK (MIRROR OF MAX)

```
Array:    ┌───┬───┬───┬───┬───┐
         │ 30│ 10│ 50│ 20│ 40│
         └───┴───┴───┴───┴───┘
```

LOGIC (mirror of max):
- min = pehla element
- Iterate har box pe
- Current < min? → min update
- End mein min = answer

```
DRY RUN:
   i=0: min=30
   i=1: 10 < 30?   haan, min=10
   i=2: 50 < 10?   nahi
   i=3: 20 < 10?   nahi
   i=4: 40 < 10?   nahi
   END: min = 10   ✓
```

KEY INTUITION:
- MAX = champion track (bigger replaces)
- MIN = weakest track (smaller replaces)
- Same shape, opposite comparison


## 15. MOVE ZEROS TO END — SAME-DIRECTION TWO POINTER

Goal: 0s ko end mein push, non-zeros ka order preserve.

```
Original:  ┌───┬───┬───┬───┬───┐
          │ 0 │ 1 │ 0 │ 3 │ 12│
          └───┴───┴───┴───┴───┘
Want:      ┌───┬───┬───┬───┬───┐
          │ 1 │ 3 │ 12│ 0 │ 0 │
          └───┴───┴───┴───┴───┘
```

LOGIC — DONO POINTER LEFT SE START (new flavor):
- SLOW: next non-zero kahaan rakhna
- FAST: scan karta — non-zero dhundta
- FAST ko 0 mile → skip, aage badh
- FAST ko non-zero mile → SLOW se swap → slow aage badh
- FAST hamesha aage badhta

```
DRY RUN:

   start: slow=0, fast=0
            ┌───┬───┬───┬───┬───┐
            │ 0 │ 1 │ 0 │ 3 │ 12│
            └───┴───┴───┴───┴───┘
              S/F

   fast=0: 0 → skip, fast++
            ┌───┬───┬───┬───┬───┐
            │ 0 │ 1 │ 0 │ 3 │ 12│
            └───┴───┴───┴───┴───┘
              S   F

   fast=1: 1 → swap with slow, slow++, fast++
            ┌───┬───┬───┬───┬───┐
            │ 1 │ 0 │ 0 │ 3 │ 12│
            └───┴───┴───┴───┴───┘
                   S   F

   fast=2: 0 → skip, fast++
   fast=3: 3 → swap with slow, slow++, fast++
            ┌───┬───┬───┬───┬───┐
            │ 1 │ 3 │ 0 │ 0 │ 12│
            └───┴───┴───┴───┴───┘
                        S      F

   fast=4: 12 → swap with slow, slow++, fast++
            ┌───┬───┬───┬───┬───┐
            │ 1 │ 3 │ 12│ 0 │ 0 │
            └───┴───┴───┴───┴───┘
                              S/F+1

   END: [1, 3, 12, 0, 0]   ✓
```

KEY INTUITION (NEW TWO-POINTER FLAVOR):

REVERSE (opposite direction):
- Left + right meeting in middle
- Swap karke andar badhte

MOVE ZEROS (same direction):
- Dono left se start
- Different SPEED — fast scans, slow places
- Fast hamesha aage, slow sirf jab non-zero mile

= Same TOOL (two pointer), alag FLAVOR
= "Slow + Fast" pattern bahut places mein use


## 16. SECOND LARGEST — TWO CHAMPIONS (GOLD + SILVER)

```
Array:    ┌───┬───┬───┬───┬───┐
         │ 10│ 50│ 20│ 80│ 40│
         └───┴───┴───┴───┴───┘
```

LOGIC — Track 2 medals:
- max1 = -inf (gold)
- max2 = -inf (silver)
- Current > max1?
  - max2 = max1 (gold demote to silver)
  - max1 = current (new gold)
- Else current > max2 AND != max1?
  - max2 = current

```
DRY RUN:
   max1 = -inf, max2 = -inf

   i=0: 10 > -inf?  haan
        max2 = -inf, max1 = 10

   i=1: 50 > 10?    haan
        max2 = 10, max1 = 50

   i=2: 20 > 50?    nahi
        20 > 10?    haan
        max2 = 20

   i=3: 80 > 50?    haan
        max2 = 50, max1 = 80

   i=4: 40 > 80?    nahi
        40 > 50?    nahi

   END: second largest = 50   ✓
```

KEY INTUITION:
- Single MAX = 1 variable (1 medal)
- SECOND MAX = 2 variables (gold + silver)
- Naya gold aaye = purana gold ko silver mein demote
- Promotion ladder logic
= Top-K pattern ka base (k=N generalize)


## SHAPE INSIGHT — ALL OPERATIONS SAME SKELETON

```
Iterate + "har step pe kya kaam":

   Max            → champion track       (compare + update)
   Min            → weakest track         (compare + update, flip)
   Sum            → bucket fill           (add)
   Count          → counter increment     (++ if match)
   Search         → return on match       (== check + exit)
   Reverse        → 2-pointer opposite    (swap + move both inward)
   Move zeros     → 2-pointer same dir    (slow places, fast scans)
   Second largest → 2-tracker promote     (gold/silver demote ladder)
   Check sorted   → neighbor compare      (array[i] vs array[i-1])

= Bunyad fix (iteration), kaam variable
= Two-pointer 2 flavors: opposite (meeting) + same-direction (slow/fast)
```
