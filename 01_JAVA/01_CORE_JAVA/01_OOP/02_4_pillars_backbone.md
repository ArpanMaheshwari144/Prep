# 4 Pillars — OOP ka Backbone

> **V90 Section 1 — Topic 2**

---

## 🟡 WHY — 4 Pillars Kyun?

Har pillar **ek specific problem** solve karta:

| Pillar | Problem solve |
|--------|---------------|
| **Encapsulation** | PROTECTION (koi bhi -5000 set na kare) |
| **Inheritance** | CODE REUSE (100 animals mein eat() copy-paste nahi) |
| **Polymorphism** | EXTENSIBILITY (naya shape add karo bina if-else) |
| **Abstraction** | SIMPLICITY (payment.pay() — UPI ya NEFT, caller ko fark nahi) |

**4 problems, 4 solutions.**

---

## 📖 STORY — Office Mein Pillars Kaise Aaye

→ **Manager ne bola** — "salary field public rakha hai"
→ "koi bhi -5000 set kar sakta hai." Tune private kiya, getter setter diya
→ — **Encapsulation**

→ Phir bola — "Manager bhi User hai"
→ "saara User ka code dobara likhna pad raha hai." Tune `extends` kiya
→ — **Inheritance**

→ Phir ek loop mein sab employees ka bonus calculate karna tha
→ Developer, Manager, Intern — **alag alag formula, ek hi method naam**
→ — **Polymorphism**

→ Aur HR ko sirf **result** chahiye tha, andar ka formula nahi
→ — **Abstraction**

→ Chaar alag problems. Chaar alag solutions.

---

## 💬 POWER PHRASES (har pillar ka 1-line)

| Pillar | Power Phrase |
|--------|--------------|
| **Encapsulation** | *"Encapsulation hides internal state with private fields and exposes behaviour through public methods — like an ATM where you press buttons without touching the circuit."* |
| **Inheritance** | *"Inheritance lets a child class reuse parent class code — Dog extends Animal and gets eat() and sleep() without rewriting them."* |
| **Polymorphism** | *"Polymorphism means the same method name behaves differently at runtime depending on the object — shape.draw() draws a circle or a square."* |
| **Abstraction** | *"Abstraction exposes what a class does, not how — like a TV remote, you press ON without knowing the internal circuit."* |

---

## 🔴 Memory Hooks (V90 ka 1-line har pillar)

- **Encapsulation** → Private fields + public getters/setters. Andar ki baat andar.
- **Inheritance** → Child extends Parent → parent ke sab features mil jaate hain.
- **Polymorphism** → Same naam, alag behavior. Runtime pe decide.
- **Abstraction** → Show WHAT, hide HOW.
