# HOW TREE RECURSION WORKS — VISUAL (Arpan ke liye, aankhon se)

> Confusion clear karne ko: jab `root->left` likhte recursion me, wo KYA karta?
> aur jo `left` me MILTA, wo kya hai? Bina kisi formula ke — sirf FLOW.

```
        [A]  <- root
       /   \
     [B]   [C]
     /
   [D]
```

## 1. Ek line ka matlab

```
   left  = f(root->left)
   │        │      │
   │        │      └── root->left = ASLI child NODE [B]  (structure me jo bachcha hai)
   │        └───────── f(...)     = us [B] ke POORE subtree me recursion GHUSAO (neeche jao, solve karo)
   └────────────────── left       = us subtree ka JAWAB jo WAPAS aaya
```

- `root->left` = **BHEJO** (child node — input)
- `left`       = **MILA** (us subtree ka answer — output)

## 2. FLOW — sirf 2 baatein

**(1) CALL neeche jaati** (jab tak null na aaye):
```
   f([A]) --> f([B]) --> f([D]) --> f(null)
                                     ^ yahan RUKTI (base case)
```

**(2) null pe base -> JAWAB upar BUBBLE hota** (peeche aate hue):
```
   f(null)=base  -->  f([D]) jawab  -->  f([B]) jawab  -->  f([A]) jawab = ANSWER
                       ▲                  ▲                  ▲
              har node apne left/right ka jawab le ke, APNA jawab UPAR bhejta
```

- Ek branch **poori neeche** (null/leaf) tak jaati -> phir **doosri** branch.
- Neeche se **jawab bubble** hoke upar aata -> root pe **final answer**.

## 3. Mantra (yaad rakh)

```
   root->left  = "neeche jao, us child ka subtree solve karo"   (CALL  / BHEJO)
   left        = "us subtree ka JAWAB jo wapas aaya"            (RESULT / MILA)
```

- Recursion ka **flow HAR problem me SAME**: neeche jaao -> base -> upar combine.
- Sirf **combine wali line** problem-wise badalti:
  `1+max` (max-depth) · `&&` (same-tree) · `||` (path-sum) · `left+right` (diameter).
- **TRUST ONE LEVEL**: poora trace mat karo. Maano `left` aur `right` ka sahi jawab mil gaya
  -> bas socho *"is node pe main kya karu?"*. Baaki recursion sambhaal legi.

## 4. Debugger = tera COPY-PEN (recursion ke liye)

Iterative code -> paper pe copy-pen trace. Recursion -> **debugger** se call-stack dekh.
Debugger use karna bilkul LEGIT hai — wahi tera trace-tool hai recursion ke liye.
