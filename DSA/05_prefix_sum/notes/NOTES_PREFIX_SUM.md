# Pattern 5: Prefix Sum

## Ye kya hai?

Bhai dekh — tu shopkeeper hai. Har din ki kamaai note kar raha hai:

```
Din:     1    2    3    4    5
Kamaai: [100, 200, 150, 300, 250]
```

Customer puchta hai: "Din 2 se Din 4 tak kitna kamaya?"

**Bina prefix sum:** 200 + 150 + 300 = har baar loop lagao. 1000 baar puche toh 1000 baar loop.

**Prefix sum se:** Ek baar pehle se total bana ke rakh le:

```
Kamaai: [100, 200, 150, 300, 250]
Prefix: [100, 300, 450, 750, 1000]

prefix[i] = din 0 se din i tak ka total
```

Ab koi bhi range sum — O(1) mein!

## Kaise banate hain?

```
prefix[0] = nums[0]
prefix[i] = prefix[i-1] + nums[i]

Matlab har position pe = pichle tak ka total + current element
```

## Range sum kaise nikalte hain?

```
sum(L to R) = prefix[R] - prefix[L-1]

Lekin L=0 ho toh? prefix[-1] nahi hai!
L=0 toh seedha: prefix[R]
```

### Dry run:

```
nums =   [-2, 0, 3, -5, 2, -1]
prefix = [-2, -2, 1, -4, -2, -3]

sum(0, 2): L=0 → prefix[2] = 1 ✅
           Check: -2 + 0 + 3 = 1 ✅

sum(2, 5): L>0 → prefix[5] - prefix[1] = -3 - (-2) = -1 ✅
           Check: 3 + (-5) + 2 + (-1) = -1 ✅

sum(3, 3): L>0 → prefix[3] - prefix[2] = -4 - 1 = -5 ✅
           Check: -5 ✅
```

## Kab use karna hai?

- "Range sum nikaal" baar baar → Prefix Sum
- "Subarray sum equals k" → Prefix Sum + HashMap (advanced)
- Basically — **jab baar baar range sum pucha jaa raha ho**

## Kab NAHI?

- Ek baar max subarray sum → Kadane's
- Fixed size window sum → Sliding Window
- Ek value dhundhni hai → Linear Scan / Binary Search

## Time & Space

- **Prefix banana: O(n)** — ek baar
- **Har query: O(1)** — seedha formula
- **Space: O(n)** — prefix array

---

## Question Jo Kiya

### Q22: Range Sum Query

```
nums =   [-2, 0, 3, -5, 2, -1]

Constructor:
  prefix[0] = -2
  prefix[1] = -2 + 0 = -2
  prefix[2] = -2 + 3 = 1
  prefix[3] = 1 + (-5) = -4
  prefix[4] = -4 + 2 = -2
  prefix[5] = -2 + (-1) = -3

  prefix = [-2, -2, 1, -4, -2, -3]

sumRange(0, 2) = prefix[2] = 1 ✅
sumRange(2, 5) = prefix[5] - prefix[1] = -3 - (-2) = -1 ✅
sumRange(0, 5) = prefix[5] = -3 ✅
sumRange(3, 3) = prefix[3] - prefix[2] = -4 - 1 = -5 ✅
sumRange(0, 0) = prefix[0] = -2 ✅
```

---

## Silly Mistakes

1. **prefix[0] = 0 ya 1 likhna** — prefix[0] = nums[0] hona chahiye. Pehle element hi first total hai.
2. **nums use karna bhoolna prefix mein** — prefix[i] = prefix[i-1] + **nums[i]**. Bina nums ke sab 0 rahega.
3. **L=0 ka case handle na karna** — prefix[-1] exist nahi karta → crash. if(left==0) lagao.
4. **prefix array size allocate na karna** — resize(n) karo pehle.

---

## Ek Line Mein Yaad Rakh

> Prefix Sum = **"Ek baar pehle se total bana ke rakh le. Phir koi bhi range sum O(1) mein."**
> Formula = **"L=0 toh prefix[R]. L>0 toh prefix[R] - prefix[L-1]."**
