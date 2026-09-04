# SOLID Principles — Kya Hai, Kyu Aaya?

> **V90 — SOLID: Topic 61 (foundation)**

---

## STORY — OrderService Sab Kuch Kar Rahi Thi

→ Ek baar **OrderService** likhi — order ka **sab kuch** ek hi class mein
→ Order logic + DB mein save + Email bhejo + PDF generate + log + audit
→ **Sab ek jagah, sab handle hoga.** Kya badiya code hai!
→ **Kal Marketing ne email format badla** — OrderService **toot gayi**
→ **Parso DB team ne schema badla** — phir **toot gayi**
→ **Tarso Audit team ne log format badla** — **phir toot gayi**
→ Realize hua: ek class = **3 alag teams toot sakti hain**
→ Bug fix karne mein 10 jagah change karna padta — testing nightmare
→ **SOLID = ye dard avoid karne ke 5 principles**

---

## WHY SOLID — Pehle Yeh Dard Samjho

### Bina SOLID = Spaghetti Code

```
class OrderService {
    saveOrder()         ← DB team ka code
    sendEmail()         ← Marketing ka code
    generatePDF()       ← Audit ka code
    calculateTax()      ← Finance ka code
    notifyAdmin()       ← Operations ka code
}
```

**Problems:**
1. **TIGHT coupling** — sab interconnected, ek change = doosra toota
2. **Hard to test** — mock karna mushkil (sab ek saath chalta)
3. **Hard to extend** — naya feature = existing code chedo
4. **Multiple reasons to change** — har team ke liye ek hi class

### SOLID Aaya — 5 Principles, 5 Solutions

```
S — Single Responsibility   →  ek class, ek kaam
O — Open / Closed           →  extension OK, modification NO
L — Liskov Substitution     →  child parent ki jagah safe use ho
I — Interface Segregation   →  fat interface mat banao
D — Dependency Inversion    →  abstraction pe depend, concrete nahi
```

**5 problems → 5 solutions. Maintainable code.**

---

## SOLID = Robert C. Martin (Uncle Bob) ka invention

→ 2000 mein paper publish kiya
→ Industry-standard ban gaya
→ **Modern frameworks** (Spring, Angular, etc.) **already SOLID follow karte**
→ Spring Boot mein:
  - `@Service` — SRP
  - `@Repository` interface — OCP, DIP
  - `@Autowired` — DIP achieve karne ka tool

---

## POWER PHRASE

> *"SOLID is five design principles for maintainable and scalable code. Bina SOLID, ek class mein multiple responsibilities ghus jaate, multiple teams se changes aate, code spaghetti ban jata. SOLID solves this with clear separation: one reason to change, extension over modification, substitutability, focused interfaces, and dependency on abstractions. Spring Boot ki entire architecture SOLID-driven hai."*

---

## Bottom Line

> **SOLID = 5 rules jo "ek change = sab kuch toot gaya" wala dard rokta.**
> Pehle ek-ek principle deeply samjho, fir project mein apply.
