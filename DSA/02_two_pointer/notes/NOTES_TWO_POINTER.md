# Pattern 2: Two Pointer

## Ye kya hai?

Linear Scan mein 1 pointer tha. Two Pointer mein **2 pointers** hain — dono apni jagah se kaam karte hain.

## 3 Types:

### 1. Opposite Direction

**Real Life:** Tu aur tera dost ek raste ke dono end pe khade ho. Dono ek dusre ki taraf chalo. Beech mein milo.

```
[1, 2, 3, 4, 5]
 ^              ^
left           right
 →              ←
```

**Kab use karna:**
- Reverse karna hai → swap karo, dono andar aao
- Palindrome check → compare karo, dono andar aao
- Two Sum (SORTED array) → sum bada hai toh right--, chhota hai toh left++

**Template:**
```
low = 0, high = n-1
while(low < high)
    → swap/compare
    → low++, high--
```

**Kyun `<` aur `<=` nahi?**
```
low == high pe ek hi element hai.
Swap: khud se khud swap — bekar.
Compare: khud se khud compare — hamesha true.
Dono mein koi kaam nahi. Isliye < kaafi.
<= lagaoge toh bhi sahi answer aayega — ek extra bekar step hoga bas.
```

---

### 2. Same Direction (Slow-Fast)

**Real Life:** Factory mein conveyor belt. 2 aadmi:
- **Fast** — har item check karta hai. "Kaam ka hai ya nahi?"
- **Slow** — khaali dabba leke khada. Fast bole "kaam ka hai" → slow dabbe mein rakh, dabba aage badha.

```
[🍎, 🗑️, 🍌, 🗑️, 🍇]
  s
  f →→→

Fast scan karta hai poora array.
Kaam ka mila → slow ki jagah rakh/swap, slow aage.
Bekar mila → skip.
```

**Kab use karna:**
- Duplicates hatao (sorted array) → alag mile toh rakh
- Zeroes end mein le ja → non-zero mile toh swap
- Koi value hatao → val nahi hai toh rakh

**Template:**
```
slow = 0, fast = 0
while(fast < n)
    → nums[fast] kaam ka hai?
      Haan → slow ki jagah rakh/swap, slow++
    → fast++
```

**Important:**
- Fast HAMESHA aage badhta hai — har step mein
- Slow SIRF tab badhta hai jab kaam ka element mile
- Condition hamesha nums[fast] pe lagti hai — fast scanner hai
- Pehle swap/assign, PHIR slow++ — order ulta kiya toh pehli position skip

---

### 3. Two Arrays

**Real Life:** 2 sorted lines hain. Dono ke saamne khada ho. Chhota element utha, result mein rakh.

**Trick:** Agar same array mein merge karna hai aur extra space END mein hai → **END se start karo.** Bada element peeche daalo. Start se karoge toh purane elements overwrite ho jayenge.

```
nums1 = [1, 3, 5, 0, 0, 0]   m=3
nums2 = [2, 4, 6]              n=3

i = m-1 (nums1 ka last real)
j = n-1 (nums2 ka last)
k = m+n-1 (last position)

End se — bada element nums1[k] pe daalo, k--
```

**Kab use karna:**
- Merge two sorted arrays
- Intersection of two sorted arrays

**Template:**
```
i = m-1, j = n-1, k = m+n-1
while(i >= 0 && j >= 0)
    → bada wala nums1[k] pe daalo, k--
while(j >= 0)
    → baaki nums2 ke elements nums1[k] pe daalo
```

**Important:**
- `k = m+n-1` — last position. `m-n+1` GALAT hai.
- Equal case handle karo — `<=` ya `>=` use karo, sirf `<` `>` se infinite loop.
- `j` bacha toh nums2 se uthao (`nums2[j]`), nums1 se nahi (`nums1[j]` GALAT).
- `i` bacha toh kuch nahi karna — already nums1 mein sahi jagah hai.

---

## Linear Scan vs Two Pointer

| Linear Scan | Two Pointer |
|---|---|
| 1 pointer | 2 pointers |
| Kuch TRACK karo (max, sum, count) | Kuch COMPARE ya SWAP karo |
| Variable update karo | Pointers move karo |

---

## Questions Jo Kiye

### Opposite Direction:

| # | Question | Kya kiya |
|---|----------|----------|
| 10 | Reverse Array | swap(low, high), dono andar |
| 11 | Palindrome | compare(low, high), alag → false |
| 12 | Two Sum Sorted | sum == target → found, bada → high--, chhota → low++ |
| 38 | Container With Most Water | min(h[l],h[r]) * distance, chhoti wall move, max() se update |

### Same Direction:

| # | Question | Fast kya dhundha | Slow ne kya kiya |
|---|----------|-----------------|-----------------|
| 13 | Remove Duplicates | nums[fast] != nums[slow] (alag) | slow++, assign |
| 14 | Move Zeroes | nums[fast] != 0 (non-zero) | swap, slow++ |
| 15 | Remove Element | nums[fast] != val (hatana nahi hai) | assign, slow++ |

---

## Common Galtiyan

1. **fast=1 se start karna** — pehla element skip ho jayega. fast=0 se start kar.
2. **Pehle slow++ phir assign/swap** — pehli position skip. Pehle assign/swap, phir slow++.
3. **swap ki jagah assign (ya ulta)** — Remove duplicates mein assign kaafi. Move zeroes mein swap chahiye (0s peeche jaane chahiye).
4. **return slow vs slow+1** — Remove duplicates mein slow+1 (slow last unique ka index). Remove element mein slow (slow count hai directly).
5. **sawp typo** — swap likhna hai 😄

---

## Dry Run = DSA ka King

> Agar tum ek test case pe dry run karke visualize kar lo — kaam khatam. DSA ka 95% kaam done.
> Code syntax pata hai, pattern pata hai, bas dry run karo — code khud aayega.

---

### Two Arrays:

| # | Question | Kya kiya |
|---|----------|----------|
| 16 | Merge Sorted Arrays | End se start, bada element peeche, k-- |

---

## Silly Mistakes Jo Hui (Yaad Rakh)

1. **k = m-n+1** — GALAT. k = m+n-1 hona chahiye (last position).
2. **nums1[i] pe likhna nums1[k] ki jagah** — purana element overwrite.
3. **Equal case miss karna** — `<` aur `>` se equal handle nahi hoga → infinite loop. `<=` ya `else` use kar.
4. **nums1[j] likhna nums2[j] ki jagah** — j nums2 ka pointer hai.
5. **swap typo** — `sawp` nahi `swap`.
6. **Pehle slow++ phir assign** — pehli position skip.
7. **fast=1 se start** — pehla element skip.
8. **return secondMax (function naam) vs secondMaxNum (variable)** — naam confuse mat kar.

---

## Ek Line Mein Yaad Rakh

> Opposite Direction = **"Dono sides se aao. Swap ya compare. Jab tak na milo."**
> Same Direction = **"Fast scan kare. Kaam ka mile toh slow ke paas rakh. Slow builder, fast scanner."**
> Two Arrays = **"Do sorted lines. Chhota utha, result mein rakh. End se start agar same array mein merge."**
