# String Basics — Pattern Intuition


## WHAT — STRING = ARRAY OF CHARACTERS

String = characters ka array. Isliye array ke SAARE patterns (Two Pointer, Hashing, Sliding Window) string pe bhi lagte.

```
   "hello":
   ┌─────┬─────┬─────┬─────┬─────┐
   │ 'h' │ 'e' │ 'l' │ 'l' │ 'o' │
   └─────┴─────┴─────┴─────┴─────┘
     0     1     2     3     4
```

```
MENTAL MODEL:
   - str[0]='h', str.length=5 (array jaisa)
   - char array → known patterns transfer

STRING-SPECIFIC:
   - Java String IMMUTABLE — reverse/modify ko char[] ya StringBuilder
   - CHAR ARITHMETIC: chars ko number jaisa use kar sakte
     'c' - 'a' = 2  → frequency array index (26 letters)
```


## CLASSIC #1: REVERSE (Two Pointer — swap)

```
   "hello" → "olleh"
   left=0, right=last; swap str[left] str[right]; left++ right--; jab tak left<right
```

```
   DRY RUN:
   l0 r4: swap h,o → "oellh"  → l1 r3
   l1 r3: swap e,l → "olleh"  → l2 r2
   l2 r2: left<right? NAHI → ruk
   Result: "olleh"  ✓
```

Beech wala (index 2) apni jagah. O(n), in-place. = Two Pointer opposite ends.


## CLASSIC #2: PALINDROME CHECK (Two Pointer — compare)

```
   "racecar" — seedha == ulta?
   left=0, right=last; str[left]==str[right]? mismatch→false; warna l++ r--
```

```
   DRY RUN ("racecar"):
   l0 r6: r==r ✓ → l1 r5
   l1 r5: a==a ✓ → l2 r4
   l2 r4: c==c ✓ → l3 r3
   l3 r3: left<right? NAHI → ruk → PALINDROME ✓

   DRY RUN ("hello"):
   l0 r4: h==o? NAHI → turant FALSE
```

= Reverse jaisa skeleton, bas swap nahi COMPARE. Ek mismatch = turant false.


## CLASSIC #3: ANAGRAM CHECK (Hashing — frequency)

```
   "listen" vs "silent" — same letters, alag order?
   Length alag? → false. Warna frequency: str1 ++ , str2 -- ; sab 0 = anagram
```

```
   DRY RUN ("listen" vs "silent"):
   listen ++: l1 i1 s1 t1 e1 n1
   silent --: s0 i0 l0 e0 n0 t0
   sab 0 → ANAGRAM ✓

   DRY RUN ("rat" vs "car"):
   rat ++: r1 a1 t1
   car --: c-1 a0 r0 t1
   sab 0 NAHI (c-1, t1) → NOT anagram
```

CHAR-ARITHMETIC: 26-size array, index = char - 'a' ('c'→2). HashMap se light/fast (lowercase fixed 26).

= Anagram = frequency match → Hashing pattern. "++ one, -- other, sab 0 = match."


## KAB USE — RECOGNITION SIGNALS

| Signal | Approach |
|--------|----------|
| Reverse / palindrome / symmetry | Two Pointer (opposite ends) |
| Anagram / char frequency / "same letters" | Hashing (freq array/map) |
| Substring / longest-without-repeat | Sliding Window |
| "First unique char", counting | Hashing (frequency) |

KEY: string = char array → koi naya pattern nahi; known patterns transfer. Pehchaano kaunsa lagega.


## TRAP BOX

```
┌─────────────────────────────────────────────────────────┐
│ TRAP 1: Java String immutable — "str[i]=x" nahi chalega  │
│   → char[] ya StringBuilder use karo modify ke liye.     │
│                                                          │
│ TRAP 2: Anagram — length check bhulna                    │
│   → alag length = turant false (frequency dekhne se pehle)│
│                                                          │
│ TRAP 3: Case / spaces / unicode                          │
│   → palindrome/anagram mein normalize (lowercase, trim)  │
│     karna padta agar problem bole.                       │
│                                                          │
│ TRAP 4: char arithmetic sirf lowercase 26 maan lena      │
│   → uppercase/digits ho to range/size adjust karo.       │
└─────────────────────────────────────────────────────────┘
```


## POWER PHRASES

- "String = char array → Two Pointer / Hashing / Sliding Window sab lagte."
- "Reverse + palindrome = Two Pointer opposite ends (swap vs compare)."
- "Anagram = frequency: ++ one string, -- other, sab 0 = match."
- "char - 'a' = index → 26-size freq array (HashMap se light)."
- "Naya pattern nahi — known patterns ka string pe transfer."
