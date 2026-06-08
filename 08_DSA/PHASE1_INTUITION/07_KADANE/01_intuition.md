# Kadane's Algorithm (Maximum Subarray Sum) — Pattern Intuition


## WHAT IS KADANE

Pattern: array (positive + negative numbers) mein woh CONTIGUOUS subarray dhundo jiska SUM sabse bada ho. O(n) single pass.

```
   ┌────┬────┬────┬────┬────┬────┬────┬────┬────┐
   │ -2 │  1 │ -3 │  4 │ -1 │  2 │  1 │ -5 │  4 │
   └────┴────┴────┴────┴────┴────┴────┴────┴────┘
     0    1    2    3    4    5    6    7    8

   Best subarray = [4,-1,2,1] (idx 3-6) = 4-1+2+1 = 6
```

"Subarray" = contiguous tukda (lagataar, gap nahi).


## CORE SOCH — "purana bojh utha-u ya fresh shuru karu?"

Har element pe ek faisla: ab tak ka running sum MADAD kar raha ya BOJH?

```
   running sum = -3, agla = 4:
     saath le: -3+4 = 1   |  fresh: sirf 4   → 4 bada → FRESH
   running sum = 5, agla = 2:
     saath le: 5+2 = 7    |  fresh: sirf 2   → 7 bada → SAATH
```

```
   RULE (Kadane ka dil):
   running sum NEGATIVE → chhod do, FRESH shuru
   running sum POSITIVE → saath rakho, aage jodo
   = "negative bojh mat utha; positive madad rakho"
```


## DO VARIABLE (yahi ek twist)

Running sum to "current" batata — par answer = ab tak ka SABSE BADA sum (jo kabhi dekha). Isliye 2 cheez:

```
   current_sum = chalta-firta (reset hota jab negative)
   max_sum     = best ever (YEHI answer — snapshot karte raho)

   KYUN max alag: current 6 pe pahuncha, phir ghat ke 3 ho gaya
                  → answer abhi bhi 6 (jo dekha), 3 nahi
                  → max best yaad rakhe, current bhatakta rahe
```

```
   2 LINE (Kadane core):
   current_sum = max(element, current_sum + element)   // fresh ya saath
   max_sum     = max(max_sum, current_sum)             // best yaad
```


## DRY RUN — [-2, 1, -3, 4, -1, 2, 1, -5, 4]

```
   START: current=-2, max=-2

   i1 (1):   max(1, -2+1=-1) → 1   FRESH (purana -2 bojh)   | max=1
   i2 (-3):  max(-3, 1-3=-2) → -2  saath                    | max=1
   i3 (4):   max(4, -2+4=2)  → 4   FRESH (purana -2 bojh)   | max=4
   i4 (-1):  max(-1, 4-1=3)  → 3   saath                    | max=4
   i5 (2):   max(2, 3+2=5)   → 5   saath                    | max=5
   i6 (1):   max(1, 5+1=6)   → 6   saath                    | max=6 (best!)
   i7 (-5):  max(-5, 6-5=1)  → 1   saath                    | max=6 (yaad)
   i8 (4):   max(4, 1+4=5)   → 5   saath                    | max=6

   END: max = 6  ✓  (subarray [4,-1,2,1], idx 3-6)
```

Note: i7/i8 pe current ghata par MAX ne 6 yaad rakha = isiliye max alag track.


## ALTERNATE FORM (reset-to-0 style)

```
   currSum = 0,  maxSum = arr[0]   // 0 se init MAT karna
   har element:
       currSum += element
       maxSum = max(maxSum, currSum)   // TURANT update (pehle)
       if currSum < 0:  currSum = 0    // reset (baad mein)
```

ORDER zaroori: maxSum update PEHLE, currSum reset BAAD mein.


## KAB USE — RECOGNITION SIGNALS

| Signal | Approach |
|--------|----------|
| "Maximum subarray sum" (contiguous) | Kadane |
| "Largest sum of consecutive elements" | Kadane |
| "Best time to buy/sell stock once" (variant) | Kadane-like (max diff) |
| Array mein +ve aur -ve dono, best run chahiye | Kadane |

### ANTI-SIGNAL
- Non-contiguous (koi bhi elements) → NOT Kadane (sorting/DP alag)
- Subarray of FIXED size k          → Sliding Window
- All positive array                → poora array hi answer (trivial)


## TRAP BOX

```
┌─────────────────────────────────────────────────────────┐
│ TRAP 1 (THE classic): SAARE elements negative            │
│   [-3,-1,-2] → answer = -1 (largest single), NOT 0.      │
│   Fix: maxSum init = arr[0] (NOT 0), aur reset se PEHLE   │
│        maxSum update karo. Warna 0 aata = galat.          │
│                                                          │
│ TRAP 2: max_sum ko current_sum ke saath confuse karna    │
│   current bhatakta hai; max = best-ever (alag track).    │
│                                                          │
│ TRAP 3: reset-to-0 form mein order ulta                  │
│   reset PEHLE + max BAAD = all-negative pe bug.           │
│   Sahi: max update → phir reset.                         │
│                                                          │
│ TRAP 4: Sliding Window samajhna                          │
│   Kadane = VARIABLE-length best run (window fixed nahi). │
└─────────────────────────────────────────────────────────┘
```


## POWER PHRASES

- "Kadane: har element pe — fresh shuru karu ya saath lu (max of dono); aur best snapshot karta rahu."
- "Negative running sum = bojh → drop. Positive = madad → keep."
- "current bhatakta hai, max best-ever yaad rakhta — do alag variable."
- "All-negative trap: maxSum init arr[0], max update before reset — warna 0 (bug)."
- "O(n) single pass — ek baar array dekha, ho gaya."
