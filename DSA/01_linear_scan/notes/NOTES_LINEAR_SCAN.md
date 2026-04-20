# Pattern 1: Linear Scan

## Ye kya hai?

Sabse basic pattern. Array mein **ek baar start se end tak ghoom** aur kuch kaam kar.

3 cheezein use hoti hain:
- **Loop** — har box (element) ek ek karke dekh
- **If** — decide kar kaam ka hai ya nahi
- **Variable** — kuch yaad rakh (max, sum, count)

Bas. Itna hi hai Linear Scan.

## Real Life Example

Tu ek shelf pe rakhey boxes dekh raha hai. Ek ek box khol ke dekh. Jo kaam ka hai wo yaad rakh. End mein answer de.

## Kab use karna hai?

- "Sabse bada/chhota dhundh" → variable mein max/min track kar
- "Sum nikaal" → variable mein add karte ja
- "Kitni baar aaya?" → count karte ja
- "Sorted hai ya nahi?" → pichle wale se compare kar
- "Maximum subarray sum?" → Kadane's Algorithm
- Basically — **jab ek baar ghoom ke answer mil jaaye**

## Kab use NAHI karna hai?

- Array sorted hai aur search karna hai → Binary Search
- Do elements ka pair dhundhna hai → Two Pointer ya Hashing
- Continuous window ka kuch karna hai → Sliding Window
- Elements rearrange karna hai → Two Pointer

## Time & Space

- **Time: O(n)** — ek baar ghoom
- **Space: O(1)** — 1-2 variables bas

## Core Idea

> "Ek loop, kuch variables, har element pe ek decision — kuch karo ya skip karo."

---

## Sub-patterns:

### 1. Ek variable track karo (Max, Min, Sum, Count)

**Kya karna hai:** Loop mein ghoom, ek variable update karte ja.

**Find Max:**
```
maxNum = INT_MIN
Har element dekh:
  Bada hai maxNum se? → maxNum = nums[i]
```

**Find Sum:**
```
sum = 0
Har element dekh:
  sum += nums[i]
```

**Count Target:**
```
count = 0
Har element dekh:
  Target ke equal hai? → count++
```

**Kyun kaam karta hai:** Ek baar poora array dekha, answer mil gaya. Sort ya extra array ki zaroorat nahi.

---

### 2. Do variables track karo (Second Max)

**Kya karna hai:** Max aur secondMax dono track karo.

**Soch:** Jab naya max mile:
1. Pehle purana max secondMax mein daalo
2. Phir naya max set karo

Aur agar element max se chhota hai lekin secondMax se bada? Tab bhi secondMax update karo.

```
maxNum = INT_MIN, secondMaxNum = INT_MIN

Har element dekh:
  nums[i] > maxNum? → secondMaxNum = maxNum, maxNum = nums[i]
  else nums[i] > secondMaxNum && nums[i] != maxNum? → secondMaxNum = nums[i]
```

**Kyun `!= maxNum`?** Duplicates mein max aur secondMax same ho jayenge warna.

---

### 3. Adjacent pairs compare (Check Sorted)

**Kya karna hai:** Har element ko uske pehle wale se compare karo.

```
i = 1 se start (0 nahi — pehle wala chahiye compare ke liye)

Har pair dekh:
  nums[i-1] > nums[i]? → return false (sorted nahi hai)

Loop khatam bina false ke → return true
```

**Kyun i=1 se?** Kyunki nums[i-1] access karna hai. i=0 pe nums[-1] hoga — crash.

---

### 4. Running sum with decision (Kadane's Algorithm)

**Problem:** Continuous subarray ka maximum sum dhundh.

**Real Life:** Tu business kar raha hai. Kuch din profit, kuch din loss. Kaunse consecutive dinon mein sabse zyada kamaya?

**Soch:** Har din pe decision — purane sum ke saath continue karu ya fresh start karu?

```
Agar sum negative ho gaya → chhod de, naya shuru kar (sum = 0)
Kyun? Negative sum aage le jaane se future profit kam hoga.
```

```
sum = 0, maxSum = INT_MIN

Har element:
  sum += nums[i]          ← add karo
  maxSum = max(maxSum, sum) ← PEHLE update karo
  sum < 0? → sum = 0       ← PHIR reset karo
```

**Kyun pehle maxSum update, phir reset?**
Agar pehle reset karo toh all-negative array mein maxSum kabhi update nahi hoga.

```
[-3, -1, -4]

GALAT (pehle reset):
  sum=-3, reset to 0, maxSum=0 ← 0 answer nahi hai, -1 hai

SAHI (pehle update):
  sum=-3, maxSum=-3, reset to 0
  sum=-1, maxSum=-1, reset to 0 ← -1 sahi answer
```

---

## Questions Jo Kiye

| # | Question | Kya track kiya | Key trick |
|---|----------|----------------|-----------|
| 01 | Find Max | maxNum | nums[i] > maxNum → update |
| 02 | Find Max Index | maxNum + index | index bhi saath track karo |
| 03 | Array Sum | sum | sum += nums[i] |
| 04 | Even Sum | sum | nums[i] % 2 == 0 check |
| 05 | Count Target | count | nums[i] == target → count++ |
| 06 | Has Duplicate | HashSet | set mein hai → duplicate |
| 07 | Second Max | maxNum + secondMaxNum | 2 variables, shift down |
| 08 | Check Sorted | adjacent pairs | nums[i-1] > nums[i] → false |
| 09 | Kadane's | sum + maxSum | sum < 0 → reset. Pehle maxSum update. |

## Common Galtiyan

1. **maxNum = 0 se start** — negative array mein fail. INT_MIN use kar.
2. **return true loop ke andar** — pehle hi pair mein return ho jayega, poora array check nahi hoga.
3. **Second max mein != maxNum bhoolna** — duplicates mein galat answer.
4. **Kadane's mein pehle reset phir update** — all-negative case fail.
5. **i=0 se start jab i-1 chahiye** — crash. i=1 se start kar.
6. **Variable naam aur function naam same** — secondMax function hai, secondMaxNum variable.

---

## Ek Line Mein Yaad Rakh

> Linear Scan = **"Ek loop. Kuch variables. Har element pe decision. Answer mil gaya."**
