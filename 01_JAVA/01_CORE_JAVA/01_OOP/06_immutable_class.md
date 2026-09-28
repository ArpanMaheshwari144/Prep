# Immutable Class

> **V90 — Core Java Extras: Topic 17**

---

## WHY — Immutable Class Kyun Banate?

→ **Thread safety FREE** (koi modify nahi kar sakta = no sync)
→ **HashMap key safe** (hashCode kabhi nahi badle)
→ **Defensive programming** — object pass karo, koi tamper nahi karega
→ **HOW:** final class + final fields + no setters + defensive copy

---

## STORY — BankAccount Bug

→ Tune BankAccount class banayi
→ Kisi function mein pass kiya — us function ne andar `account.setBalance(0)` kar diya
→ **Bug.**
→ Agar account **immutable** hota — koi change hi nahi kar sakta tha
→ **4 rules:** `final` class, `private final` fields, **no setter**, constructor se set karo
→ Aur andar List ho toh **defensive copy** lagao — warna reference se modify ho jaayega
→ BankAccount pass kiya, kisi ne balance 0 kar diya!

---

## Code (4 Rules)

```java
public final class BankAccount {              // Rule 1 — final class
    private final String owner;                // Rule 2 — private + final
    private final int balance;

    public BankAccount(String o, int b) {     // Rule 3 — constructor se set
        this.owner = o;
        this.balance = b;
    }

    public String getOwner() { return owner; } // Rule 4 — sirf getter
    public int getBalance() { return balance; } // koi setter NAHI
}

// Badlana ho? Naya object banao — purana safe
BankAccount old = new BankAccount("Arpan", 5000);
BankAccount updated = new BankAccount("Arpan", 4000);   // old wahi hai
```

---

## TRAP — List Field Hota Toh?

```java
// BAD
public BankAccount(List<String> txns) {
    this.txns = txns;                  // bahar wali list ka REFERENCE!
}                                       // koi bhi modify kar sakta!

// FIX — defensive copy in constructor + getter
public BankAccount(List<String> txns) {
    this.txns = new ArrayList<>(txns); // apni copy banao
}
public List<String> getTxns() {
    return new ArrayList<>(this.txns); // copy return karo
}
```

> **`final` sirf reassign rokta hai. Andar List ho toh content badal sakta hai!**

---

## Visualization — Reference Trap

```
                  Immutable Class — Reference Trap

╔════════════════════════════════════════════════════════════╗
║ BAD — same reference store                              ║
╚════════════════════════════════════════════════════════════╝

STACK              HEAP
                   ┌─────────────┐
caller's list ────►│  ["Java"]   │◄──── student.courses
                   └─────────────┘
                   DONO SAME object pe point!

caller list.add("Python") → student ki "immutable" state BHI badli!


╔════════════════════════════════════════════════════════════╗
║ FIX — defensive copy                                    ║
╚════════════════════════════════════════════════════════════╝

STACK              HEAP
                   ┌─────────────┐
caller's list ────►│  ["Java"]   │   ← caller ki original
                   └─────────────┘

                   ┌─────────────┐
student.courses ──►│  ["Java"]   │   ← Student ki APNI copy
                   └─────────────┘

ALAG objects → caller modify kare, Student safe.
```

---

## ★ TRAP — "final class + private final" phir bhi toot gayi (Student example, output ke saath)

```java
final class Student {
    private final List<String> courses;
    Student(List<String> c) { this.courses = c; }              // SAME reference!
    List<String> getCourses() { return courses; }              // SAME reference!
}

List<String> list = new ArrayList<>();
list.add("Java");
Student s = new Student(list);
list.add("Python");                                             // bahar se andar BADLA!
s.getCourses().size();    // 2 — Immutable TOOT GAYI!
```

→ Student ne **kabhi `add()` call nahi kiya** — phir bhi uski list 2 ki ho gayi, kyunki bahar wala caller SAME object modify kar raha tha.
→ Asli galti constructor mein: `this.courses = c;` = caller ki list ka reference store kar liya. Caller ki list aur Student ki list **EK HI OBJECT**.
→ Getter bhi same reference de raha → `s.getCourses().add(...)` se bhi andar badlega. Isliye copy **constructor + getter DONO** mein.

> **`final` = reference reassign nahi hota. `final` ≠ object ka content modify nahi hota.**
> Mutable field (List, Set, Map) ka reference share kiya = "immutable" actually mutable hai bahar ke caller ke through.

**WHY String field mein ye problem nahi?**
→ String **khud immutable** hai — `add()` jaisi koi method nahi
→ Reference share ho bhi gaya, content modify hi nahi hota
→ Lekin **List/Set/Map mutable** — `add/remove` se content badal sakta
→ Isliye **mutable fields ke liye defensive copy** ZAROOR

---

## ★ TRAP — Shallow copy vs Deep copy

```
SHALLOW COPY:
   Object copy — par andar ke (nested) REFERENCES shared
   User u1 = ...
   User u2 = u1.clone()   // shallow (default clone)
   u2.address = ...        → u1 aur u2 ka SAME address object (shared)

DEEP COPY:
   Object + SAARE nested objects ki FULL copy
   u1 aur u2 = INDEPENDENT, koi sharing nahi
```
→ Upar wala defensive copy isi ka kaam hai: reference share mat karo, apni copy rakho.

> *"For immutable classes with mutable fields like List, you must defensive-copy in both the constructor and getter — otherwise the external reference can mutate the internal state. `final` only prevents reference reassignment, not content modification."*


---

## POWER PHRASE

> *"An immutable class uses final class, private final fields, no setters, and constructor-only initialization. For mutable fields like List, always use defensive copies — otherwise the reference can be modified externally."*
