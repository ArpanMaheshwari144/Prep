# Abstract Class vs Interface

> **V90 Section 1 — Topic 3**

---

## 🟡 WHY — KAB Kya Use?

- **Abstract class** = partial implementation + STATE (fields). **IS-A** relationship (Dog IS-A Animal)
- **Interface** = contract, **CAN-DO** behaviour
- Multiple interfaces implement ho sakte
- Rule: **"kya hai"** = abstract class. **"kya kar sakta"** = interface
- Java 8 se interface mein default methods aaye lekin **STATE abhi bhi sirf abstract class** mein

---

## 📖 STORY — Notification System

→ Notification system banate waqt problem aayi
→ **Email, SMS, Push** — teeno mein retry logic same, logging same, timestamp same
→ Teeno mein copy-paste kiya
→ Kal retry logic badli — teeno jagah change karna pada
→ **Fix:** Abstract Class banaya — shared code ek jagah, teeno extend karo
→ Lekin ek aur cheez — kuch notifications "Approvable" thi, kuch nahi
→ Ye **IS-A nahi, CAN-DO** tha
→ Isliye **Interface** banaya
→ Rule: **Shared code chahiye = Abstract Class. Capability define karni = Interface.**
→ Shared code chahiye → Abstract Class (IS-A)
→ Capability define karni → Interface (CAN-DO)
→ Ek class → sirf 1 abstract extend, **multiple interface implement**

---

## 📊 Comparison Table

| Feature | Abstract Class | Interface |
|---------|----------------|-----------|
| **Constructor** | YES (via `super()`) | NO |
| **Methods** | abstract + normal dono | Java 8+: default + static bhi |
| **Inheritance** | SIRF ek parent | Multiple interfaces allowed |
| **Variables** | koi bhi type | `public static final` ONLY |
| **Use when** | Shared code + partial impl | Define a capability/contract |

---

## 🔴 TRAP

> **Abstract Class = IS-A relationship. Interface = CAN-DO capability.**

---

## 💬 POWER PHRASE

> *"Abstract class is for shared base implementation — IS-A relationship. Interface is a capability contract — CAN-DO. A class can implement multiple interfaces but extend only one abstract class."*
