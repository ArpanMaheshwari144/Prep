# Pattern 7: Stack

## Ye kya hai?

Bhai dekh — plates ka stack soch. Upar se plate rakh, upar se utha. **Last In First Out (LIFO).**

```
push(1) → [1]
push(2) → [1, 2]
push(3) → [1, 2, 3]
pop()   → 3 nikla. [1, 2]
top()   → 2 (upar wala dekho, nikalo mat)
```

## Kab use karte hain?

- **"Sabse recent/closest dhundho"** → stack (last in first out)
- **Brackets matching** → open bracket push, close aaye toh pop se match
- **Next Greater Element** → stack mein rakh, bada mile toh pop karke answer daal
- **Undo operation** → last action pop karo

---

## Question 1: Valid Parentheses (Q28)

### Problem:
String mein brackets hain — `()`, `{}`, `[]`. Check kar sab properly open aur close ho rahe?

### Soch:
- Open bracket aaye → push karo stack mein
- Close bracket aaye → stack se top dekho, match karta hai? → pop. Nahi? → false.
- End mein stack khaali → true. Nahi → false.

### IMPORTANT — 3 cheezein dhyan rakh:

**1. Close bracket aaye aur stack KHAALI hai → false**
```
"}{" → pehle '}' aaya, stack khaali — kisse match karega? CRASH ya false.
st.top() se pehle HAMESHA st.empty() check kar.
```

**2. Match nahi hua toh return false**
```
"(]" → '(' push hua. ']' aaya — '(' se match nahi karta.
Agar return false nahi kiya toh galat answer aayega.
```

**3. End mein stack khaali hona chahiye**
```
"(((" → sab push hue, koi close nahi aaya. Stack mein 3 elements bache.
return st.empty() → false ✅
```

### Dry Run:

```
"({[]})"
st = {}

'(' → push. st=['(']
'{' → push. st=['(', '{']
'[' → push. st=['(', '{', '[']
']' → top='[', match! pop. st=['(', '{']
'}' → top='{', match! pop. st=['(']
')' → top='(', match! pop. st=[]

st.empty()? Haan → true ✅
```

```
"(]"
st = {}

'(' → push. st=['(']
']' → top='(', '(' aur ']' match? NAHI → false ✅
```

---

## Question 2: Next Greater Element (Q29)

### Problem:
Har element ke liye — uske RIGHT mein sabse pehla bada element kaunsa hai?

```
[4, 5, 2, 10, 8] → [5, 10, 10, -1, -1]

4 ka next greater = 5
5 ka next greater = 10
2 ka next greater = 10
10 ka next greater = koi nahi = -1
8 ka next greater = koi nahi = -1
```

### Soch:
Stack mein elements daalo. Jab koi BADA element aaye — stack mein jo chhote pade hain unka answer mil gaya!

### IMPORTANT — 3 cheezein jo maine seekhi:

**1. Stack mein INDEX push karo, VALUE nahi**
```
Kyun? Kyunki answer array mein DALNA hai — ans[index] = value.
Agar stack mein value hai toh pata kaise chalega ki ans mein KAHAN daalna hai?

st.push(i) — index daalo
ans[st.top()] = nums[i] — st.top() se index niklo, wahan answer daalo
```

**2. if nahi WHILE lagao**
```
[4, 5, 2, 10, 8]

i=3: nums[3]=10. Stack mein [1, 2] hai (index of 5 and 2).
     2 ka answer 10 hai → pop
     5 ka answer bhi 10 hai → pop
     
     Ek baar mein MULTIPLE elements ka answer mil sakta hai.
     if se sirf ek pop hoga — while se sab pop honge.
```

**3. Har element push karo — chahe chhota ho ya bada**
```
2 chhota hai 5 se — push mat karu? GALAT!
Kyunki 2 ka next greater aage kabhi bhi aa sakta hai (10 aaya).
Agar push nahi kiya toh uska answer kabhi nahi milega.
```

### Template:

```
ans array bana, sab -1 se fill (default: koi greater nahi)
for i = 0 to n:
    while(!st.empty() && nums[st.top()] < nums[i]):
        ans[st.top()] = nums[i]   ← stack wale ka answer mil gaya
        st.pop()
    st.push(i)                     ← current index push
```

### Dry Run:

```
[4, 5, 2, 10, 8]
st={}, ans=[-1,-1,-1,-1,-1]

i=0: st empty. Push 0. st=[0].

i=1: nums[0]=4 < 5? Haan → ans[0]=5. Pop. st=[].
     Push 1. st=[1].

i=2: nums[1]=5 < 2? Nahi.
     Push 2. st=[1,2].

i=3: nums[2]=2 < 10? Haan → ans[2]=10. Pop. st=[1].
     nums[1]=5 < 10? Haan → ans[1]=10. Pop. st=[].
     Push 3. st=[3].

i=4: nums[3]=10 < 8? Nahi.
     Push 4. st=[3,4].

Loop khatam. Stack mein [3,4] — inke liye koi greater nahi mila → ans mein -1 already hai.

ans = [5, 10, 10, -1, -1] ✅
```

---

## Silly Mistakes

1. **st.top() pehle st.empty() check nahi karna** — khaali stack pe top() → CRASH.
2. **Stack mein value push karna index ki jagah** — answer array mein kahan daalna hai pata nahi chalega.
3. **if lagana while ki jagah** — ek baar mein multiple elements ka answer mil sakta hai, if se sirf ek pop hoga.
4. **Chhota element push nahi karna** — uska next greater aage aa sakta hai, push zaroori hai.
5. **Brackets mein match nahi hua toh return false bhoolna** — loop chalta rahega, galat answer.

---

## Question 3: Min Stack (Q30)

### Problem:
Stack banao jisme push, pop, top normal ho — aur **getMin() O(1)** mein minimum bataye.

### Soch:
2 stacks — st1 normal, st2 har step pe min track.

### Push mein st2 pe kya daalna:
```
st2 khaali hai           → val push (pehla element hi min)
val > st2.top()          → st2.top() push (min same raha)
val <= st2.top()         → val push (naya min aaya)
```

### Dry Run:
```
push(5):  st1=[5]      st2=[5]       ← pehla, seedha push
push(3):  st1=[5,3]    st2=[5,3]     ← 3 chhota, naya min
push(7):  st1=[5,3,7]  st2=[5,3,3]   ← 7 bada, min same (3)
getMin() → 3 ✅
pop():    st1=[5,3]    st2=[5,3]     → getMin() → 3 ✅
pop():    st1=[5]      st2=[5]       → getMin() → 5 ✅
push(1):  st1=[5,1]    st2=[5,1]     → getMin() → 1 ✅
```

### Galtiyan:
1. **st2 khaali pe st2.top()** — CRASH! Pehle empty check.
2. **st2 khaali pe skip** — pehla element push nahi hua.
3. **top() mein st2.top()** — wo min hai, st1.top() chahiye.

---

## Questions Jo Kiye

| # | Question | Key trick |
|---|----------|-----------|
| 28 | Valid Parentheses | Open push, close match, end khaali = true |
| 29 | Next Greater Element | Index push, while pop, har element push |
| 30 | Min Stack | 2 stacks, st2 min track, naya chhota → val, warna top repeat |

---

## Ek Line Mein Yaad Rakh

> Valid Parentheses = **"Open → push. Close → top se match. End mein stack khaali → true."**
> Next Greater Element = **"Stack mein index daalo. Bada aaye → while se pop karo, answer daalo. Har element push karo."**
> Min Stack = **"2 stacks. st1 normal. st2 min track. Naya chhota → val push, warna top repeat."**
