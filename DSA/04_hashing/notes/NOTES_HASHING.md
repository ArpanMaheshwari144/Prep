# Pattern 4: Hashing

## Ye kya hai?

Bhai dekh — tu dictionary mein "mango" dhundh raha hai. Tu page 1 se nahi dhundhta — seedha "M" section mein jaata hai. **Direct access.** O(1) mein mil jaata hai.

Array mein kuch dhundhna ho toh poora ghoomna padta hai — O(n). HashMap/HashSet mein **seedha mil jaata hai** — O(1).

## 2 cheezein hain:

**HashSet** — sirf "hai ya nahi?" (haan/nahi)
```
set = {3, 7, 1, 9}
set mein 7 hai? → Haan! O(1)
```

**HashMap** — "hai toh kahan hai?" ya "kitni baar aaya?" (key:value)
```
map = {3:0, 7:1, 1:2, 9:3}   (element:index)
7 kahan hai? → index 1 pe! O(1)
```

## Kab use karna hai?

- "Duplicate hai kya?" → HashSet
- "Element kitni baar aaya?" → HashMap (count store)
- "Koi pair hai jiska sum target hai?" (unsorted array) → HashMap
- Basically — **jab O(1) mein lookup chahiye**

## Kab NAHI?

- Array sorted hai aur pair dhundhna hai → Two Pointer (O(1) space bacha)
- Continuous subarray → Sliding Window
- Max/min dhundhna → Linear Scan

## Two Pointer vs Hashing:

```
Two Sum SORTED array   → Two Pointer (O(1) space)
Two Sum UNSORTED array → HashMap (O(n) space, lekin kaam ho jaata hai)
```

## Complement Search Pattern — Two Sum jaisa:

Har element ke liye:
1. Complement nikaal: `target - nums[i]`
2. Map mein dhundh — hai? Answer mil gaya!
3. Nahi? Khud ko map mein daal de.

```
[2, 7, 11, 15]  target=9
mp={}

i=0: complement = 9-2 = 7. mp mein 7? Nahi. mp={2:0}
i=1: complement = 9-7 = 2. mp mein 2? HAAN! → answer [0, 1] ✅
```

**"Mera complement map mein hai? Haan → mil gaya. Nahi → khud ko daal de."**

---

## Question Jo Kiya

### Q21: Two Sum Unsorted

```
[2, 7, 11, 15]  target=9
mp={}

i=0: 9-2=7. mp.count(7)? Nahi. mp={2:0}.
i=1: 9-7=2. mp.count(2)? Haan! → ans = {mp[2], 1} = {0, 1} ✅
```

**Seekh:**
- `mp[target - nums[i]]` se complement ka index nikaal — `mp[nums[i]]` nahi
- `ans(2, -1)` se initialize — phir `ans[0]` aur `ans[1]` mein daal
- push_back mein pair nahi daal sakta — seedha index se assign kar

---

## Silly Mistakes

1. **mp[nums[i]] likhna complement ki jagah** — tujhe complement ka index chahiye
2. **push_back mein pair daalna** — push_back ek value leta hai. ans[0], ans[1] use kar
3. **Pehle map mein daalna phir check karna** — element khud se match ho sakta hai

---

## Ek Line Mein Yaad Rakh

> Hashing = **"Complement map mein hai? Haan → mil gaya. Nahi → khud ko daal de."**
