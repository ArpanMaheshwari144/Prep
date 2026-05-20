# I = Interface Segregation Principle (ISP)

> **V90 — SOLID: Topic 65**

---

## Definition

> **Class ko unneeded methods implement karne pe FORCE mat karo.**
> Fat interfaces mat banao — **chhoti, focused interfaces** banao.

---

## STORY — Fat Repository Interface

→ Ek baar **`Repository` interface** banayi — har class ke liye:

```java
interface Repository {
    void save();
    void delete();
    void update();
    void generatePDF();
    void sendEmail();
}
```

→ Ek class banayi — **`ReadOnlyRepository`** — **sirf padhna** chahiye, modify nahi
→ Lekin interface implement karna padta — `delete()` likhna padta!

```java
class ReadOnlyRepository implements Repository {
    void save()       { /* dangerous + useless */ }
    void delete()     { /* dangerous! */ }
    void update()     { /* dangerous! */ }
    void generatePDF() { ... }
    void sendEmail()   { ... }
}
```

→ **Useless + dangerous methods implement karne pad rahe** — fat interface ka dard
→ ISP violation
→ **Fix:** Interface chhoti, focused karo

---

## Fix — Split Karo

```java
interface ReadRepo {
    Object find(String id);
}

interface WriteRepo {
    void save(Object o);
    void delete(String id);
    void update(Object o);
}

interface PDFExporter {
    byte[] generatePDF(Object o);
}

interface EmailNotifier {
    void sendEmail(Object o);
}
```

**Ab:**

```java
// Sirf padhna chahta? Sirf ReadRepo implement
class ReadOnlyRepository implements ReadRepo { /* find() likhi */ }

// Read + Write chahta? Dono
class FullRepository implements ReadRepo, WriteRepo { /* dono methods */ }

// Sirf PDF wala? Sirf PDFExporter
class ReportExporter implements PDFExporter { /* generatePDF() */ }
```

→ Class ko **sirf relevant methods** implement karne padte
→ Useless code gone

---

## ISP Visualization

```
                BAD — Fat interface

                  Repository (fat)
                  ┌──────────────┐
                  │ save()       │
                  │ delete()     │
                  │ update()     │
                  │ generatePDF()│
                  │ sendEmail()  │
                  └──────┬───────┘
                         │ implements
              ┌──────────┴──────────┐
              ▼                     ▼
        ReadOnlyRepo            FullRepo
        saare 5 likhne       sab kuch
           padte (useless +
           dangerous)


                GOOD — Focused interfaces

  ReadRepo     WriteRepo     PDFExporter    EmailNotifier
  (find)       (save,        (generatePDF)  (sendEmail)
                delete,
                update)
       │           │              │              │
       └───────────┼──────────────┘              │
                   │                              │
              ┌────┴─────┐                       │
              ▼          ▼                       ▼
      ReadOnlyRepo   FullRepo               ReportExporter
      ↑              ↑ ↑                     (sirf PDF)
      sirf find     dono
```

---

## ISP Spring Boot Mein

Spring Data JPA pre-built focused interfaces:

```java
CrudRepository              ← basic save, find, delete
JpaRepository               ← Crud + JPA-specific
PagingAndSortingRepository  ← pagination + sorting
```

**Tu chahe wo extend kar — fat interface nahi.**

---

## POWER PHRASE

> *"Interface Segregation means do not force a class to implement methods it does not need. Split fat interfaces into smaller focused ones. Spring's `CrudRepository`, `JpaRepository`, and `PagingAndSortingRepository` are perfect real-world examples."*
