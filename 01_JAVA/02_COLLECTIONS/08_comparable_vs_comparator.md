# Comparable vs Comparator

## Tere DSA se

Meeting rooms / merge intervals me C++:

```cpp
sort(v.begin(), v.end(), [](auto& a, auto& b){ return a[0] < b[0]; });
```

Bahar se bataya "start pe sort karo". Java me isi ka naam **Comparator**.

## Do cheezein, do jagah

- **Comparable:** class ke andar ek default kram (`Panelist implements Comparable`, `compareTo` me age). Bas ek rule.
- **Comparator:** class ke bahar, jitne chaho rule (aaj naam, kal rating). Class nahi chhedni.

```java
list.sort(Comparator.comparing(Panelist::getName));        // naam se
list.sort(Comparator.comparingInt(Panelist::getAge));      // umar se
```

Return number: sort do logon ko utha ke poochhta "pehle kaun?". Minus = pehla aage, plus = doosra aage, 0 = barabar.
Bada pehle chahiye = a, b ki jagah badlo.

## Trade-off

Comparable seedha, par ek kram. Comparator lachila, par har jagah likhna. Asli kram (id) class me, baaki Comparator se.

## Prod trap

```java
(a, b) -> (int)(a.createdAt - b.createdAt)   // epoch millis, long
```

Do timestamp ka farak `int` me nahi samata (~24 din se zyada ka farak int ki had paar). Number ulta, list chup-chaap
galat kram me. Ilaaj: `Long.compare(a.createdAt, b.createdAt)`. Int me bhi `a - b` nahi, `Integer.compare`.

## Interview line

```
Comparable gives a class one natural order through compareTo, while Comparator lets me sort the same
objects many ways without touching the class; I always use Integer.compare or Long.compare rather
than subtraction, because subtracting timestamps can overflow and silently sort in the wrong order.
```
