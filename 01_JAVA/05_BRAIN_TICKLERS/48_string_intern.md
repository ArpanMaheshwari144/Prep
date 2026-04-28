# String `intern()` — Pool Mein Force Karo

> **V90 — Brain Ticklers: Topic 48**

---

## 📖 STORY

→ `new String("Hello")` heap mein bana, **pool se alag**
→ Lekin `s.intern()` call kiya toh pool mein jaake check karta — "Hello" hai? **Haan**
→ Toh **pool wala SAME object return** kar deta. Ab `==` bhi `true` aayega

```java
String s1 = "Hello";                  // pool mein
String s2 = new String("Hello");      // heap mein — alag object
String s3 = s2.intern();              // pool mein jaake uthaya

s1 == s2     // false — alag objects
s1 == s3     // true  — dono pool ka SAME object
```

---

## 🟡 WHY `intern()`? — Performance

→ Bahut saari **duplicate strings** hain (XML parsing, CSV reading)
→ `intern()` se sab pool mein **ek hi object share karein** → memory save
→ **Overuse mat karo** — pool PermGen/Metaspace mein hai, bhar gaya toh problem

---

## 💬 POWER PHRASE

> *"`intern()` returns the canonical pool reference of a string — useful when many duplicate strings exist, but overuse risks filling the string pool memory."*
