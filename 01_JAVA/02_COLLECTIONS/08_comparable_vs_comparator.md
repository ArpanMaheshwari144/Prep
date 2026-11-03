# Comparable vs Comparator

## Tere DSA se

Meeting rooms aur merge intervals me tu C++ me ye likhta hai:

```cpp
sort(v.begin(), v.end(), [](auto& a, auto& b){ return a[0] < b[0]; });
```

Yahan tune sort ko bahar se bataya ki "start pe sort karo". Java me isi bahar se diye gaye rule ka naam **Comparator** hai.

## Do cheezein, do jagah

- **Comparable:** ye class ke andar likha jaata hai aur class ka ek default kram tay karta hai. Jaise `Panelist implements
  Comparable`, aur `compareTo` me age se compare kiya. Isme bas ek hi rule ho sakta hai.
- **Comparator:** ye class ke bahar likha jaata hai, aur jitne chaho utne rule bana sakte ho, aaj naam se, kal rating se.
  Iske liye class ko chhedna nahi padta.

```java
list.sort(Comparator.comparing(Panelist::getName));        // naam se
list.sort(Comparator.comparingInt(Panelist::getAge));      // umar se
```

Return number ka matlab aise samjho: sort do logon ko uthata hai aur poochhta hai "pehle kaun aayega?". Agar minus aaya
to pehla aage jaata hai, plus aaya to doosra aage jaata hai, aur 0 aaya to dono barabar hain. Agar bada pehle chahiye,
to bas a aur b ki jagah badal do.

## Trade-off

Comparable seedha hai, par usme ek hi kram ho sakta hai. Comparator lachila hai, par har jagah alag se likhna padta hai.
Isliye class ka asli kram (jaise id) class ke andar Comparable me rakho, aur baaki sab kram Comparator se do.

## Prod trap

```java
(a, b) -> (int)(a.createdAt - b.createdAt)   // createdAt = epoch millis, long hai
```

Do timestamp ka farak `int` me nahi samata. Lagbhag 24 din se zyada ka farak int ki had paar kar jaata hai. Tab number
ulta aa jaata hai, aur list chup-chaap galat kram me sort ho jaati hai, koi error nahi aata.

Ilaaj: `Long.compare(a.createdAt, b.createdAt)` likho. Int wale field me bhi `a - b` mat likho, `Integer.compare` likho.

## Interview line

```
Comparable gives a class one natural order through compareTo, while Comparator lets me sort the same
objects many ways without touching the class; I always use Integer.compare or Long.compare rather
than subtraction, because subtracting timestamps can overflow and silently sort in the wrong order.
```
