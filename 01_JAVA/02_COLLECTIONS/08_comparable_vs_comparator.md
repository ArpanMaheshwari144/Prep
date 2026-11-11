# Comparable vs Comparator

## Scene: tere DSA se

Meeting rooms aur merge intervals me tu C++ me ye likhta hai:

```cpp
sort(v.begin(), v.end(), [](auto& a, auto& b){ return a[0] < b[0]; });
```

Yahan tune sort ko BAHAR se bataya ki "start pe sort karo". Java me isi bahar se diye rule ka naam **Comparator** hai.

## Do cheezein, do jagah

- **Comparable:** class ke ANDAR likha jaata hai, class ka ek default kram tay karta hai. Jaise `Panelist implements
  Comparable` aur `compareTo` me age se compare. Bas ek hi rule.
- **Comparator:** class ke BAHAR, jitne chaho utne rule (aaj naam se, kal rating se). Class ko chhedna nahi padta.

```java
Collections.sort(list);                                       // Comparable (age)
list.sort(Comparator.comparing(Panelist::getName));           // naam se
list.sort(Comparator.comparingInt(Panelist::getAge).reversed());  // umar, bada pehle
```

```
Comparable (age)     : [Arpan(28), Zoya(31), Kabir(35)]
Comparator naam se   : [Arpan(28), Kabir(35), Zoya(31)]
umar, bada pehle     : [Kabir(35), Zoya(31), Arpan(28)]
```

Return number ka matlab: sort do logon ko uthata hai aur poochhta hai "pehle kaun?".
- Minus = pehla (a) aage.
- Plus = doosra (b) aage.
- 0 = dono barabar.

Bada pehle chahiye to a aur b ki jagah badal do, ya `.reversed()`.

## Trade-off

Comparable seedha hai par ek hi kram. Comparator lachila hai par har jagah alag likhna padta hai. Isliye class ka asli kram
(jaise id) andar Comparable me, baaki sab kram Comparator se.

## Prod trap: `(int)(a - b)` timestamp pe

```java
(a, b) -> (int)(a.createdAt - b.createdAt)   // createdAt = epoch millis, long hai
```

```
(int)(a-b) purana pehle: [aaj, 30 din pehle, 10 din pehle]
Long.compare          : [30 din pehle, 10 din pehle, aaj]
30 din ka farak (int) = -1702967296
```

- Do timestamp ka farak `int` me nahi samata. Lagbhag 24 din se zyada ka farak int ki had paar kar jaata hai.
- 30 din ka farak plus hona chahiye tha, int me daala to minus ban gaya. Sort ko ulta jawab mila.
- List chup-chaap galat kram me: "aaj" sabse pehle aa gaya. Koi error nahi.

Ilaaj: `Long.compare(a.createdAt, b.createdAt)`. Int wale field me bhi `a - b` mat likho, `Integer.compare` likho.
