# Pass by Value — Java ka Rule

> **V90 — Topic 24**

---

## WHY — Java Reference Pass Kyu Nahi Karta?

→ **Safety.**
→ Agar actual reference pass hota toh method andar **`s = null`** kar deta → caller ka variable BHI null! **Dangerous.**
→ Java ne socha — **reference ki COPY bhejo**. Copy pe kuch bhi karo, **caller safe**
→ State change hogi (`s.name = "X"`) — kyunki **dono same object point karte**
→ Lekin **reference khud nahi badlega**

---

## STORY — Method Mein Object Pass

→ Tune method mein Student object pass kiya — method ne andar `name` badal diya
→ Wapas aaya — **name badal gaya**
→ Tune socha "Java pass by reference hai!" **Galat.**
→ Java **HAMESHA pass by value** hai
→ Object pe **reference ki copy** pass hoti hai
→ Dono same object point karte hain → **state change hoti**
→ Lekin method andar `s = new Student()` karo → **original nahi badlega**, sirf copy ne naya object point kiya

---

## Visualization

```
STACK                          HEAP
┌──────────────┐              ┌────────────────┐
│ main: st  ──┼─────────────►│ Student         │
│              │              │ name = "Arpan"  │
│ change: s ──┼─────────────►└────────────────┘
└──────────────┘
```

**Dono `st` aur `s` SAME object pe → `s.name = "X"` = st bhi change.**

```
Lekin agar:  s = new Student()
                       │
                       ▼
                ┌────────────────┐
              s │ NEW Student    │   ← s ab idhar point karta
                │ name = "Rahul" │
                └────────────────┘

st abhi bhi pehle wale "Arpan" pe → ORIGINAL SAFE
```

---

## 3 Cases

### Case 1 — Primitive (int, boolean, double)
```java
void change(int x) { x = 99; }       // copy badli, original nahi
int num = 10;
change(num);
System.out.println(num);              // 10 — nahi badla!
```

### Case 2 — Object (reference ki copy pass)
```java
void change(Student s) { s.name = "Rahul"; }   // andar ka data badla 
Student st = new Student("Arpan");
change(st);
System.out.println(st.name);                    // "Rahul" — badla!
```

### Case 3 — TRAP: Naya object assign karna
```java
void change(Student s) { s = new Student("Rahul"); }   // copy ne naya object point kiya
Student st = new Student("Arpan");
change(st);
System.out.println(st.name);                            // "Arpan" — NAHI badla!
```

---

## Kahani

> **Dost ko apni notebook ki photocopy di**
> → Dost ne photocopy pe kuch likha — **teri original nahi badi**
> → Lekin dost ne **photocopy ke andar ke drawer ka lock khola** aur drawer mein kuch rakha — **original drawer bhi badal gayi!**
> → Yehi Java hai

---

## TRAP

> **"Java pass by reference hai" — GALAT.**
> **Java HAMESHA pass by value. Object pe reference ki COPY pass hoti hai.**

---

## POWER PHRASE

> *"Java is always pass by value. For primitives, the actual value is copied. For objects, the reference is copied — both point to the same object, so state changes reflect. But reassigning the parameter inside the method does not affect the original reference."*

> **3-line yaad:**
> Primitive → actual value copy → original safe
> Object → reference copy → state change yes, naya assign nahi
> TRAP → `s = new Student()` method andar → copy ne naya point, original untouched
