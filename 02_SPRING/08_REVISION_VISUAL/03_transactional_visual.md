# @Transactional — Visual Revision

---

## 1 Problem (Bank Transfer Story)

```
   Tu ne Suresh ko ₹500 transfer kiya:

   Step 1: Tera account → ₹500 debit
   Step 2: Suresh account → ₹500 credit

   Both must succeed TOGETHER.

   Imagine Step 1 ho gaya, Step 2 ke time SERVER CRASH:
      Tera ₹500 gone
      Suresh ko mila NAHI
      = ₹500 vanished into thin air!
```

```
SOLUTION: Atomic operation = "Sab ya kuch nahi"
```

---

## 2 Transaction Concept

```
   ┌─────────────────────────────────┐
   │  TRANSACTION START              │
   ├─────────────────────────────────┤
   │  Step 1: Debit Arpan ₹500       │
   │  Step 2: Credit Suresh ₹500     │
   ├─────────────────────────────────┤
   │  TRANSACTION END                │
   │     SUCCESS? → COMMIT (save)    │
   │     FAILURE? → ROLLBACK (undo)  │
   └─────────────────────────────────┘
```

---

## 3 Spring = MySQL Wrapper (Reality Check)

### MySQL Manual
```sql
START TRANSACTION;

UPDATE accounts SET balance = balance - 500 WHERE id = 1;
UPDATE accounts SET balance = balance + 500 WHERE id = 2;

COMMIT;       -- or ROLLBACK on error
```

### Spring @Transactional (Under The Hood)
```
   conn.setAutoCommit(false);   ← START TRANSACTION
       ↓
   Method execute
       ↓
   Success?    → conn.commit()    ← COMMIT
   Exception?  → conn.rollback()  ← ROLLBACK
       ↓
   conn.setAutoCommit(true);
```

```
┌────────────────────┬─────────────────────────────┐
│  Manual SQL        │  Spring @Transactional      │
├────────────────────┼─────────────────────────────┤
│ START TRANSACTION  │ Method enter (proxy)         │
│ queries            │ Method body                   │
│ COMMIT             │ Method success → auto commit │
│ ROLLBACK           │ Exception → auto rollback    │
└────────────────────┴─────────────────────────────┘

   Spring SAME thing kar raha
   Bus tu ne boilerplate likhna nahi
   = Same JDBC under the hood
```

---

## 4 Code Reality

```java
@Service
public class TransferService {

    @Transactional
    public void transfer(Long from, Long to, BigDecimal amount) {
        accountRepo.debit(from, amount);    // Step 1
        accountRepo.credit(to, amount);     // Step 2
        // Spring automatically:
        //   Method success → COMMIT
        //   Exception thrown → ROLLBACK
    }
}
```

---

## 5 ACID Properties

```
A — ATOMICITY
   "Sab ya kuch nahi"
   ┌───┬───┬───┐         ┌───┬───┬───┐
   │ │ │ │   OR    │ │ │ │
   └───┴───┴───┘         └───┴───┴───┘
   All commit             All rollback

C — CONSISTENCY
   DB rules respected
   (foreign keys, constraints)

I — ISOLATION
   Concurrent transactions don't mess each other

D — DURABILITY
   Once committed = permanent
   Server crash? Data safe.
```

---

## 6 Propagation — Sirf 3 Yaad Rakh

```
ANALOGY: Tu meeting mein hai. Naya kaam aaya — kya karega?
```

```
┌─────────────────────┬──────────────────────────────┐
│  Type               │  Behavior                     │
├─────────────────────┼──────────────────────────────┤
│ REQUIRED (default)  │ Existing TX join, OR new     │
│  90% use case       │ SAFE default                 │
├─────────────────────┼──────────────────────────────┤
│ REQUIRES_NEW        │ ALWAYS new (suspend parent)  │
│                     │ Use: audit log               │
├─────────────────────┼──────────────────────────────┤
│ NESTED              │ Savepoint inside parent      │
│                     │ Inner rollback OK            │
└─────────────────────┴──────────────────────────────┘
```

```
   ┌──────────────────────────────────────┐
   │  REQUIRED (default)                   │
   │     Same TX or new TX                 │
   │                                        │
   │  REQUIRES_NEW                         │
   │     ALWAYS new (parent suspended)     │
   │                                        │
   │  NESTED                                │
   │     Savepoint inside parent           │
   └──────────────────────────────────────┘
```

---

## 7 Isolation Levels

```
PROBLEM: 2 transactions running simultaneously
   What can they SEE from each other?
```

```
┌─────────────────────┬──────────────────────────────┐
│  Level              │  Allows                       │
├─────────────────────┼──────────────────────────────┤
│ READ_UNCOMMITTED    │ Dirty reads (uncommitted     │
│  (lowest)           │  data visible)                │
├─────────────────────┼──────────────────────────────┤
│ READ_COMMITTED      │ Only committed data visible  │
│  (Postgres default) │                               │
├─────────────────────┼──────────────────────────────┤
│ REPEATABLE_READ     │ Same query = same result    │
│  (MySQL default)    │ within transaction           │
├─────────────────────┼──────────────────────────────┤
│ SERIALIZABLE        │ Like single-threaded         │
│  (highest)          │ Full isolation, slow         │
└─────────────────────┴──────────────────────────────┘
```

```
USE:
   90%        → default
   Banking    → SERIALIZABLE
   Reporting  → READ_COMMITTED
```

---

## 8 Rollback Rules (INTERVIEW TRAP)

```
DEFAULT BEHAVIOR:
   Spring rollback ONLY on:
      RuntimeException
      Error
      Checked Exception (IOException, SQLException)
         = NO ROLLBACK!
```

```java
// THIS WILL NOT ROLLBACK:
@Transactional
public void method() throws IOException {
    repo.save(...);
    throw new IOException();   // Checked exception
    // DB save COMMITTED, exception propagated
    // = Data corruption
}
```

```java
// FIX:
@Transactional(rollbackFor = Exception.class)
public void method() throws IOException {
    // Now ALL exceptions trigger rollback
}
```

```
PRODUCTION RULE:
   @Transactional(rollbackFor = Exception.class)
```

---

## 9 Self-Invocation Pitfall (BIGGEST GOTCHA)

```
PROBLEM:
   @Transactional method called from SAME class
   = Transaction NOT applied!
```

```java
@Service
public class UserService {

    public void outer() {
        inner();   // ← @Transactional IGNORED here!
    }

    @Transactional
    public void inner() {
        // No transaction wrapped
    }
}
```

```
WHY?
   Spring proxy works on EXTERNAL method calls
   Internal call (this.method) bypasses proxy
   = @Transactional doesn't trigger
```

```
   ┌──────────┐    @Transactional method
   │  PROXY   │  ← only intercepts here (external call)
   └────┬─────┘
        │
        ▼
   ┌──────────┐
   │ Real Svc │
   │          │ → method calling another method
   │          │   (internal — no proxy)
   │          │   = @Transactional ignored
   └──────────┘
```

```
FIX OPTIONS:
   1. Move method to different class (proxy works) ← simplest
   2. Inject SELF reference
   3. Use AspectJ instead of proxy
```

---

## Read-Only Optimization

```java
@Transactional(readOnly = true)
public List<User> findAll() {
    return repo.findAll();
}
```

```
WHY readOnly = true?
   DB driver knows = no write needed
   Hibernate skips dirty checking
   Faster for read queries
   Some DBs route to read replicas

USE: All getter/finder methods
```

---

## 11 Real Production Pattern

```java
@Service
public class TransferService {

    @Autowired AccountRepository repo;
    @Autowired AuditService auditService;

    @Transactional(rollbackFor = Exception.class)
    public void transfer(Long from, Long to, BigDecimal amount)
            throws InsufficientFundsException {

        Account fromAcc = repo.findById(from).orElseThrow();
        Account toAcc = repo.findById(to).orElseThrow();

        if (fromAcc.balance.compareTo(amount) < 0) {
            throw new InsufficientFundsException();
            // → automatic ROLLBACK
        }

        fromAcc.debit(amount);
        toAcc.credit(amount);

        repo.save(fromAcc);
        repo.save(toAcc);

        auditService.logTransfer(from, to, amount);
    }
}

@Service
public class AuditService {

    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void logTransfer(...) {
        // Independent transaction
        // Even if parent rolls back, audit saved
    }
}
```

---

## Memory Hooks

```
@Transactional       → "Sab ya kuch nahi"
                       JDBC wrapper (BEGIN/COMMIT/ROLLBACK)

REQUIRED             → 90% default, safe

REQUIRES_NEW         → audit log pattern (independent)

Self-invocation      → INTERVIEW TRAP — proxy bypass

rollbackFor=Exception → checked exception fix

readOnly=true        → optimization for finders
```

---

## One-Line Summary

```
"@Transactional = MySQL ka START TRANSACTION + COMMIT/ROLLBACK
                  ka annotation wrapper.
                  Spring AOP proxy intercepts, manages connection.
                  Same JDBC under the hood."
```

[← Back to SPRING](../)
