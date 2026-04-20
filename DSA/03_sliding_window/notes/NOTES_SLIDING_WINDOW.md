# Pattern 3: Sliding Window

## Ye kya hai?

Bhai dekh — ek window hai array pe jo slide karti hai. Left se ek nikalta hai, right se ek aata hai. Pura dobaara calculate nahi karna — bas update karo.

## Real Life Example

Tu train mein baith ke bahar dekh raha hai. Window se ek fixed area dikhta hai. Train aage badhti hai — **purana scene left se jaata hai, naya scene right se aata hai.** Window ka size same hai, bas slide ho rahi hai.

```
[2, 1, 5, 1, 3, 2]    k=3

[2, 1, 5]              sum = 8
   [1, 5, 1]           sum = 8 - 2 + 1 = 7   (2 gaya, 1 aaya)
      [5, 1, 3]        sum = 7 - 1 + 3 = 9   (1 gaya, 3 aaya) ← max
         [1, 3, 2]     sum = 9 - 5 + 2 = 6   (5 gaya, 2 aaya)
```

## Kyun use karte hain?

Brute force mein har window ka sum dobaara calculate karta — O(n * k).
Sliding window mein pichle sum se ek nikalo ek daalo — O(n). Bas!

## 2 Types hain:

### 1. Fixed Size (k diya hua hai)
Window ka size pata hai. k elements ka max sum, max average, etc.

### 2. Variable Size (k nahi diya)
Window bada/chhota hota hai condition ke basis pe.
Longest substring with k unique chars, longest without repeat, etc.

---

## Window ka size kaise nikalte hain?

```
j - i + 1

i=1, j=3 → elements: nums[1], nums[2], nums[3] → 3 elements
j - i + 1 = 3 - 1 + 1 = 3 ✅
```

---

## Fixed Size — Kaise karte hain?

Bhai dekh simple hai. i aur j dono 0 se start. j badhate jao, sum add karte jao.

1. **sum += nums[j]** — pehle add karo
2. **agar window badi ho gayi (j-i+1 > k)** — shrink karo (sum se nums[i] hatao, i++)
3. **agar window sahi size ki hai (j-i+1 == k)** — maxSum update karo
4. **j++** — aage badho

### ORDER BAHUT ZAROORI HAI!

Ye maine khud dry run karke discover kiya:

```
GALAT: add → update → shrink
  j=3 pe sum=9 tha, size=4. Pehle update kiya → 4==3? Nahi, miss!
  Phir shrink kiya → sum=7. Lekin update ho chuka, 7 kabhi count nahi hua.

SAHI: add → shrink → update
  j=3 pe sum=9. Pehle shrink → sum=7, size=3. Phir update → maxSum=7. Sab count hua.
```

**Yaad rakh: ADD → SHRINK → UPDATE → j++. Ye order kabhi mat badal.**

---

## Variable Size — Kaise karte hain?

Fixed jaisa hi hai! Bas 2 fark:

### Fark 1: shrink mein `if` nahi `while`

```
Fixed:    if(size > k) → ek baar shrink kaafi (ek element nikaal do)
Variable: while(condition toote) → multiple baar nikalna pad sakta

Kyun? Fixed mein window 1 hi zyada hoti hai. Variable mein kaafi badi ho sakti hai.
```

### Fark 2: shrink ki condition alag

```
Fixed:    j-i+1 > k → size dekho
Variable: mp[s[j]] > 1 → repeat dekho, ya mp.size() > k → unique count dekho
```

Baaki sab same: **add → shrink (while) → update → j++**

---

## Shrink mein ek confusion tha mera — clear kar leta hoon

Jab shrink karte hain (i++ karte jao), toh i pe same character ho ya alag — **farak nahi padta.** Bas nikaalte jao jab tak condition theek na ho. While loop khud handle karega.

```
"abba"  j=2, 'b' repeat hua

i=0: s[0]='a'. Nikala. 'b' nahi hai lekin nikalna padega — window chhoti karni hai
i=1: s[1]='b'. Nikala. Ab repeat khatam. STOP.
```

**Tujhe dekhna nahi ki i pe kya hai — bas shrink karte jao.**

---

## Update mein condition lagana zaroori hai ya nahi?

Depend karta hai:

**Longest no repeat:** while ke baad window valid hi hai → seedha update, if ki zaroorat nahi.

**Longest k unique:** while ke baad size < k bhi ho sakta hai → `if(mp.size() == k)` lagao tab update.

```
"aaa" k=2
Bina if: size=1 pe bhi update hoga → galat answer (3 aayega, -1 chahiye)
If ke saath: size==2 kabhi nahi hua → maxAns=INT_MIN → return -1 ✅
```

---

## int vs double

Average nikal raha hai toh double use kar. `int / int = int` → decimal kat jayega (12.75 → 12).

---

## Questions Jo Kiye

### Fixed Size:

| # | Question | Kya nikala |
|---|----------|-----------|
| 17 | Max Sum of K elements | max sum |
| 18 | Max Average of K elements | max average (double!) |

### Variable Size:

| # | Question | Shrink kab | Update kab |
|---|----------|-----------|-----------|
| 19 | Longest no repeat | mp[s[j]] > 1 | har valid window |
| 20 | Longest k unique | mp.size() > k | mp.size() == k |

---

## Dry Run — Max Sum (Q17)

```
[2, 1, 5, 1, 3, 2]  k=3
i=0, j=0, sum=0, maxSum=INT_MIN

j=0: sum=2. 1>3? Nahi. 1==3? Nahi. j=1.
j=1: sum=3. 2>3? Nahi. 2==3? Nahi. j=2.
j=2: sum=8. 3>3? Nahi. 3==3? Haan → maxSum=8. j=3.
j=3: sum=9. 4>3? Haan → sum=9-2=7, i=1. 3==3? Haan → maxSum=8. j=4.
j=4: sum=10. 4>3? Haan → sum=10-1=9, i=2. 3==3? Haan → maxSum=9. j=5.
j=5: sum=11. 4>3? Haan → sum=11-5=6, i=3. 3==3? Haan → maxSum=9.

Answer: 9 ✅
```

## Dry Run — Longest No Repeat (Q19)

```
"abcabcbb"
i=0, j=0, mp={}, ans=0

j=0: mp={'a':1}. Repeat? Nahi. ans=1.
j=1: mp={'a':1,'b':1}. Repeat? Nahi. ans=2.
j=2: mp={..'c':1}. Repeat? Nahi. ans=3.
j=3: mp={'a':2,..}. mp['a']>1! Shrink: mp['a']--, i=1. ans=3.
j=4: mp={'b':2,..}. mp['b']>1! Shrink: mp['b']--, erase 'a', i=2. ans=3.

Answer: 3 ✅
```

---

## Silly Mistakes Jo Hui

1. **Order galat kiya** — pehle update phir shrink. Shrink ke baad ka sum miss hua. **ADD → SHRINK → UPDATE → j++.**
2. **int use kiya jab double chahiye** — average mein decimal kata.
3. **mp.erase() bhool gaya** — mp.size() galat raha.
4. **Update mein if nahi lagaya** — invalid window bhi count hui.
5. **maxAns INT_MIN reh gaya** — empty/impossible case handle karo.
6. **Variable mein if lagaya while ki jagah** — ek baar shrink kaafi nahi hota.

---

## Ek Line Mein Yaad Rakh

> Fixed Size = **"ADD karo. SHRINK karo (if > k). UPDATE karo (if == k). j++. ORDER MAT BADLO."**
> Variable Size = **"ADD karo. SHRINK karo (while condition toote). UPDATE karo (if valid). j++."**
> Dono mein template SAME hai — bas shrink ki condition alag aur if vs while.
