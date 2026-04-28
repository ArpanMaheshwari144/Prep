# OOP Kya Hai

> **V90 Section 1 — Topic 1**

---

## 🟡 WHY — OOP Kyun Aaya?

C mein data alag (global variables), functions alag. **Koi connection nahi.** 1000 variables tairte, koi bhi access kare, koi bhi corrupt kare.

**OOP = data + behaviour EK jagah (class).**

C ke 3 problems → 1 concept se solve:
- **Protection** (private) — koi bhi corrupt nahi karega
- **Grouping** (related data ek jagah)
- **Reuse** (naya object banao)

---

## 📖 STORY

→ Ek baar ek codebase mila — **500 lines, ek hi file**
→ User ka naam, age, email — sab alag-alag variables
→ Functions mein 10 parameters. Kuch fix karo → doosri jagah toot jaati
→ Tab samjha — **data aur kaam alag-alag nahi chahiye**
→ Ek cheez ke saare data aur kaam **ek jagah** rakho
→ Ye hai OOP ka asli fayda
→ **Object = data + behaviour ek saath**
→ Jira ticket: fields = data, resolve/assign = behaviour. Java mein **sab object hai**.

---

## 🧠 4 Pillars (sirf naam — deep agle topics mein)

```
┌─────────────────┬─────────────────┬─────────────────┬─────────────────┐
│  ENCAPSULATION  │   INHERITANCE   │  POLYMORPHISM   │   ABSTRACTION   │
│   Protection    │   Code Reuse    │  Extensibility  │ Hide Complexity │
└─────────────────┴─────────────────┴─────────────────┴─────────────────┘
```

**Memory hook:**
- Encapsulation → data protect
- Inheritance → code reuse
- Polymorphism → same method, different behaviour
- Abstraction → hide HOW, show WHAT

---

## 💬 POWER PHRASE

> **"OOP represents real-world entities as objects — data and behaviour bundled together in a single class."**
