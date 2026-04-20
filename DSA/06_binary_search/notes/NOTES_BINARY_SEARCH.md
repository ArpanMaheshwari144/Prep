# Pattern 6: Binary Search

## Ye kya hai?

Bhai dekh — tu 1000 pages ki dictionary mein "mango" dhundh raha hai. Tu page 1 se nahi dhundhta — beech mein kholega:
- Page 500 → "P" section. "M" pehle aata hai → left half.
- Page 250 → "G" section. "M" baad mein → right half.
- Page 375 → "M" section. MIL GAYA!

**Har baar aadha kaat diya.** 1000 pages mein 10 steps. 10 lakh mein 20 steps. Bas!

## Kyun use karte hain?

```
Linear Search: poora array ghoom → O(n). 10 lakh mein 10 lakh steps.
Binary Search: aadha aadha kaat → O(log n). 10 lakh mein 20 steps. Bas!
```

## ZAROORI: Array SORTED hona chahiye. Bina sorted ke BS nahi lagta.

## 3 Types hain:

### 1. Simple BS — target dhundho
### 2. Boundary Finding — pehli/aakhri position dhundho
### 3. BS on Answer — answer ki range pe BS lagao

---

## Simple BS — Kaise karte hain?

```
low = 0, high = n-1
while(low <= high)
    mid = low + (high-low)/2
    nums[mid] == target → MIL GAYA! return mid.
    nums[mid] < target  → left half chhod. low = mid+1.
    nums[mid] > target  → right half chhod. high = mid-1.
return -1 (nahi mila)
```

### Dry run:

```
[1, 3, 5, 7, 9, 11, 13]    target = 11

low=0, high=6
mid=3: nums[3]=7. 7 < 11 → low=4.

low=4, high=6
mid=5: nums[5]=11. 11==11 → MIL GAYA! return 5 ✅
```

---

## Boundary Finding — Pehli/Aakhri position

Normal BS mein target milte hi return kar dete. Lekin agar target kai baar aaya toh?

```
[5, 7, 7, 8, 8, 10]   target=8

8 pehli baar → index 3
8 aakhri baar → index 4
```

**Ek BS se dono nahi mil sakti.** Kyun? Target milne pe ya toh left jao ya right — dono ek saath nahi.

Isliye **2 BS lagao:**

**Pehla BS — pehli position:**
```
target mila → ans[0] = mid, high = mid-1 (aur LEFT jao, shayad pehle bhi ho)
```

**Dusra BS — aakhri position:**
```
target mila → ans[1] = mid, low = mid+1 (aur RIGHT jao, shayad baad mein bhi ho)
```

### Dry run — pehli position:

```
[5, 7, 7, 8, 8, 10]  target=8
ans = [-1, -1]

low=0, high=5
mid=2: 7 < 8 → low=3.

low=3, high=5
mid=4: 8==8 → ans[0]=4. high=3. (aur left jao)

low=3, high=3
mid=3: 8==8 → ans[0]=3. high=2. (overwrite! 3 sahi hai — leftmost)

low=3, high=2 → STOP. ans[0] = 3 ✅
```

### Dry run — aakhri position:

```
low=0, high=5
mid=2: 7 < 8 → low=3.

low=3, high=5
mid=4: 8==8 → ans[1]=4. low=5. (aur right jao)

low=5, high=5
mid=5: 10 > 8 → high=4.

low=5, high=4 → STOP. ans[1] = 4 ✅
```

---

## BS on Answer — Ship Packages / Koko Bananas / Sqrt

Kabhi kabhi target array mein nahi hota — **answer ki range pe BS lagao.** Mid try karo, helper function se check karo — possible hai ya nahi.

### Structure hamesha same:

```
low = minimum possible answer
high = maximum possible answer
while(low <= high)
    mid try kar
    canDo(mid) true  → ans=mid, high=mid-1 (aur KAM try kar — minimum dhundh raha)
    canDo(mid) false → low=mid+1 (badhao)
return ans
```

### Helper function — 2 types hain:

**CONTINUOUS (Ship Packages):** Packages order mein uthao. Ek ke baad ek add karo. Limit cross → naya din.

```
Tu delivery boy hai. Gaadi mein packages daalta ja.
Capacity cross ho → naya din, current package se restart.

day = 1, sum = 0
sum += weight[i]
sum > capacity? → day++, sum = weight[i]
return day <= days
```

**Kyun day=1?** Pehle din se deliver kar raha hai — wo bhi count hona chahiye.

**INDEPENDENT (Koko Bananas):** Har pile alag hai. Har pile ka ceil(pile/speed) hours lagenge.

```
Koko bandar hai. Har pile independently khaata hai.
Har pile mein ceil(pile/speed) hours lagenge.

hours = 0
hours += ceil((double)piles[i] / speed)
return hours <= h
```

**Kyun hours=0?** Abhi kuch khaaya nahi. Hours add honge jab pile process karega.

**Kyun (double) cast?** `7/4 = 1` (int division). `7.0/4 = 1.75`, `ceil(1.75) = 2`. Bina cast ke galat answer.

### CONTINUOUS vs INDEPENDENT — kaise decide karu?

```
CONTINUOUS: Elements ek ke baad ek uthao, limit cross → reset
  → sum karte jao, cross → day/trip++, current element se restart
  → Ship Packages, Painters Partition, Book Allocation

INDEPENDENT: Har element alag process hota hai
  → Har element ka ceil nikaal ke total karo
  → Koko Bananas
```

**Koi formula nahi hai — real life soch. Delivery boy hai toh gaadi mein add karte ja. Bandar hai toh har pile alag kha.**

### BS on Answer mein direction yaad rakh:

```
Minimum dhundh raha (ship/koko):
  canDo true  → ans=mid, HIGH=mid-1 (aur kam try kar)
  canDo false → LOW=mid+1 (badhao)

GALAT direction = infinite loop ya galat answer!
```

### Sqrt bhi BS on Answer hai:

```
x = 8. 1 se 8 tak BS. mid*mid <= x? low=mid+1. mid*mid > x? high=mid-1.

mid=4: 16>8 → high=3.
mid=2: 4≤8 → low=3.
mid=3: 9>8 → high=2.
STOP. return high=2 ✅
```

---

## IMPORTANT: `<=` kyun? Tune khud dry run se samjha:

```
[5]  target=5
low=0, high=0

while(low < high) → 0 < 0? Nahi → MISS!
while(low <= high) → 0 <= 0? Haan → mid=0, found! ✅

Jab kuch DHUNDH rahe — last element bhi check zaroori. Isliye <=.
```

---

## IMPORTANT: mid kaise nikalte hain?

```
GALAT: (low + high) / 2         → bade numbers pe overflow
SAHI:  low + (high - low) / 2   → safe, same result
```

---

## Questions Jo Kiye

| # | Question | Type | Key trick |
|---|----------|------|-----------|
| 23 | Simple BS | Simple | target mila → return mid |
| 24 | First/Last Position | Boundary | 2 BS. Target mila → return mat karo, aur dhundho |
| 25 | Square Root | BS on Answer | 1 se x tak BS. mid*mid <= x? Return high |
| 26 | Ship Packages | BS on Answer | CONTINUOUS: add karte jao, cross → day++, sum=weights[i]. day=1 se start |
| 27 | Koko Bananas | BS on Answer | INDEPENDENT: ceil(pile/speed) add karo. hours=0 se start. (double) cast |

---

## Silly Mistakes

1. **`<` lagana `<=` ki jagah** — last element miss. BS mein hamesha `<=`.
2. **`(low+high)/2` likhna** — overflow. `low + (high-low)/2` use kar.
3. **Boundary finding mein target milte hi return karna** — aur dhundhna hai! Ruko.
4. **Ek BS se first aur last dono dhundhna** — nahi hoga. 2 BS lagao.
5. **Default ans 0 rakhna** — -1 se initialize karo.
6. **`nums[mid] > target` ka else missing** — sab cases handle kar.
7. **Ship mein day=0 se start** — pehla din miss hoga. day=1.
8. **Koko mein hours=1 se start** — extra hour count hoga. hours=0.
9. **Koko mein (double) cast bhoolna** — int division mein decimal katega. ceil galat aayega.
10. **days++ likhna day++ ki jagah** — input parameter badal diya, counter nahi!
11. **BS direction ulta** — minimum dhundh raha toh true pe high=mid-1, false pe low=mid+1. Ulta kiya toh galat.

---

## Ek Line Mein Yaad Rakh

> Simple BS = **"Sorted hai? Aadha kaat. Left ya right? Repeat."**
> Boundary = **"Target mila toh ruko mat — aur dhundho. 2 BS lagao."**
> BS on Answer = **"Answer ki range pe BS lagao. Helper function — CONTINUOUS toh add karo, INDEPENDENT toh ceil karo."**
