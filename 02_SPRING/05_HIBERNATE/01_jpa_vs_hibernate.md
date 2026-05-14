# 🛢️ JPA vs Hibernate

---

## 1️⃣ Foundation Distinction

```
JPA (Java Persistence API):
   = SPEC (interface, rules)
   = "Java mein DB-talk kaise hoga" — standard

Hibernate:
   = IMPLEMENTATION of JPA
   = Actual code that does the work
```

---

## 2️⃣ USB Cable Analogy

```
JPA       = USB cable specification
            (3 cm, 5V, pin layout, etc.)

Hibernate = Samsung's USB cable
            (one concrete implementation)

EclipseLink = another USB cable
              (different brand, same spec)

   Same spec, different concrete vendors
```

---

## 3️⃣ Visual

```
   Your code
        │
        │ uses
        ▼
   ┌───────────────────────────┐
   │  JPA (interface)           │
   │  @Entity, @OneToMany,      │
   │  EntityManager...          │
   └────────────┬──────────────┘
                │ implemented by
                ▼
   ┌───────────────────────────┐
   │  Hibernate (90% uses)      │
   │  Actual ORM code           │
   │  OR EclipseLink, OpenJPA   │
   └────────────┬──────────────┘
                │ uses
                ▼
   ┌───────────────────────────┐
   │  JDBC (low-level)          │
   │  Actual SQL execution      │
   └───────────────────────────┘
                │
                ▼
            Database
```

---

## 4️⃣ Spring Data JPA Mein?

```java
@Repository
public interface UserRepository extends JpaRepository<User, Long> {
    Optional<User> findByEmail(String email);
}
```

```
Yeh code:
   JpaRepository  = JPA interface
   Spring Data    = thin layer on top
   Underneath     = Hibernate by default
   
   = Tu JPA likhta, Hibernate execute karta
```

---

## 5️⃣ Why Use JPA (Not Hibernate Directly)?

```
✅ VENDOR INDEPENDENCE
   Code JPA pe likha → Hibernate ya EclipseLink — koi farak nahi
   = Future swap possible

✅ STANDARDIZATION
   Universal API (Java standard)
   = Industry-wide knowledge
```

---

## 🎯 Memory Hook

```
JPA        = 📋 Spec (rules)
Hibernate  = 🔧 Implementation (does the work)

Tera code  → JPA interface
Behind     → Hibernate executes
```
