# JPA vs Hibernate

---

## 1 Foundation Distinction

```
JPA (Java Persistence API — ab naam "Jakarta Persistence"):
   = SPEC (interface, rules)
   = Boot 3+ me package jakarta.persistence.* (pehle javax.persistence.*)
   = "Java mein DB-talk kaise hoga" — standard

Hibernate:
   = IMPLEMENTATION of JPA
   = Actual code that does the work
```

---

## 2 USB Cable Analogy

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

## 3 Visual

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
   │  Hibernate (sabse zyada)   │
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

## 4 Spring Data JPA Mein?

```java
@Repository
public interface UserRepository extends JpaRepository<User, Long> {
    Optional<User> findByEmail(String email);
}
```

```
Yeh code:
   JpaRepository  = SPRING DATA ka interface (JPA spec ka NAHI)
   JPA spec ka    = EntityManager, @Entity, @Id, @OneToMany ...
   Spring Data    = JPA ke upar patli layer (method naam se query khud banti)
   Underneath     = Hibernate by default (EntityManager ka implementation)

   = Tu JPA likhta, Hibernate execute karta
```

---

## 5 Why Use JPA (Not Hibernate Directly)?

```
VENDOR INDEPENDENCE
   Code JPA pe likha → Hibernate ya EclipseLink — koi farak nahi
   = Future swap possible

STANDARDIZATION
   Universal API (Java standard)
   = Industry-wide knowledge
```

---

## Memory Hook

```
JPA        = Spec (rules)
Hibernate  = Implementation (does the work)

Tera code  → JPA interface
Behind     → Hibernate executes
```

---

## ★ PROJECT CONNECT — usercrud (8-Sep)
```
Author/Book/Product = @Entity (JPA spec annotations — @Entity/@Id/@OneToMany/@ManyToOne).
BookRepository extends JpaRepository = Spring Data JPA (interface, Spring khud impl banata).
Hibernate = PROVIDER jo Spring Boot ne auto-configure kiya (starter-data-jpa se).
-> tune JPA likha, chalaya Hibernate ne. "spec vs impl" LIVE.
```
