# 🔥 `@Transactional` — Complete Interview Reference

> **Most-asked Spring annotation in interviews.** 90% chances yeh kuch na kuch puchega.

---

## 📌 WHY — Asli zaroorat

Tu apne bhai ko ₹1000 transfer kar raha hai:

```
Step 1: Tere account se ₹1000 minus
Step 2: Bhai ke account mein ₹1000 plus
```

**Imagine:**
- Step 1 successful ✅ (₹1000 cut ho gaye)
- Step 2 fail ❌ (server crash / network issue)

**Result:** Tere paise gone, bhai ko mile nahi. **DISASTER.**

Yahan se transaction concept aata hai — **"All or nothing"** group of DB operations.

---

## 🎯 Transaction kya hai?

> Either **saare steps successful** → COMMIT (permanent save)
> Ya **koi bhi step fail** → ROLLBACK (saare changes revert, jaise kuch hua hi nahi)

Bich-bich mein partial save **NEVER**.

### 🎨 Visual — Bank Transfer (Tu ₹1000 bhej raha)

```
                  ┌─────────────────────────────────┐
                  │  TRANSACTION = ALL OR NOTHING   │
                  └─────────────────────────────────┘

  WITHOUT @Transactional:                WITH @Transactional:
  ─────────────────────────              ─────────────────────────
                                                                   
  Tu (₹5000)         Bhai (₹2000)        Tu (₹5000)        Bhai (₹2000)
       │                  │                    │                  │
       │ -1000 ✅         │                    │ -1000 ✅         │
       │                  │                    │                  │
       ▼                  │                    ▼                  │
   ₹4000              ₹2000                ₹4000              ₹2000
                                                                   
   💥 CRASH 💥                              💥 CRASH 💥           
                                                                   
       │                  │                    │   ROLLBACK ⏪    │
       │           +1000 ❌                    │                  │
       │                  │                    ▼                  │
       ▼                  ▼                ₹5000              ₹2000
   ₹4000              ₹2000                                        
                                          (Original state restored
   😱 ₹1000 GONE                            jaise kuch hua hi nahi)
   FOREVER                                  ✅ Safe!
```

### 🔒 ACID — Transaction ke 4 properties (interview bonus)

```
     A   ───  ATOMICITY     "All or nothing" — ya saare ya koi nahi
     │
     C   ───  CONSISTENCY   Pre + post state valid (rules pass)
     │
     I   ───  ISOLATION     Concurrent TX one another se isolate
     │
     D   ───  DURABILITY    Commit = permanent (crash bhi ho toh saved)
```

---

## 🔄 Spring se pehle — Manual boilerplate

```java
Connection conn = dataSource.getConnection();
try {
    conn.setAutoCommit(false);

    // Step 1: debit
    // Step 2: credit

    conn.commit();          // ← saare successful → save
} catch (Exception e) {
    conn.rollback();        // ← koi fail → reverse
} finally {
    conn.close();
}
```

**Har method mein yeh boilerplate** — error-prone, ugly, repetitive.

---

## ✨ Spring ka magic — `@Transactional`

```java
@Transactional
public void transferMoney(Long fromId, Long toId, double amount) {
    accountRepo.debit(fromId, amount);
    accountRepo.credit(toId, amount);
}
```

**Spring AOP proxy automatically:**
1. Method shuru → transaction begin
2. Method successful → `commit()`
3. Method exception → `rollback()`
4. Method end → connection close

**No try/catch, no boilerplate.**

---

## 💎 ASLI SACH — `@Transactional` = MySQL transaction wrapper!

> **Important realization:** Spring kuch naya magic nahi kar raha. Andar wahi MySQL transaction commands chal rahe hain jo tu manually karta — bas tujhe likhna nahi padta.

### Tera mental model = sahi hai

```
Tu MySQL mein manually karta:           Spring tujhe is se bachata:
━━━━━━━━━━━━━━━━━━━━━━━━━              ━━━━━━━━━━━━━━━━━━━━━━━━━

START TRANSACTION;                      @Transactional
                                        public void doStuff() {
INSERT INTO users (...);                    userRepo.save(u);
INSERT INTO orders (...);                   orderRepo.save(o);
UPDATE inventory ...;                       inventoryRepo.update(i);
                                        }
COMMIT;     -- ya
ROLLBACK;
```

**Bilkul same kaam.** Annotation ek shorthand hai.

### 🔍 Andar kya hota hai — JDBC layer

`@Transactional` ka actual implementation **JDBC `Connection` API** use karta hai:

```java
Connection conn = dataSource.getConnection();

conn.setAutoCommit(false);    // ← yahi "START TRANSACTION" ka equivalent
                              //   (queries fire honge but DB pe permanent
                              //    nahi honge tab tak commit() na call ho)

// queries chalti hain (Hibernate INSERT/UPDATE)

if (success) {
    conn.commit();             // ← COMMIT — ab permanent
} else {
    conn.rollback();           // ← ROLLBACK — sab revert
}

conn.close();
```

### 📊 3 Layers — Same concept, different syntax

| Layer | Begin | Commit | Rollback |
|---|---|---|---|
| **MySQL CLI** | `START TRANSACTION;` | `COMMIT;` | `ROLLBACK;` |
| **JDBC raw Java** | `conn.setAutoCommit(false)` | `conn.commit()` | `conn.rollback()` |
| **Spring** | `@Transactional` annotation | proxy auto-commit | proxy auto-rollback |

**Sab ek hi DB-level concept hai**, alag-alag abstractions. Spring sirf boilerplate kam karta.

### 🎨 Visual — Spring Behind the Curtain

```
       Tu likhta:
       @Transactional
       public void doStuff() {
           // queries
       }
            │
            │ (Spring AOP proxy ne wrap kiya)
            ▼
       Andar yeh execute hota:

       Connection conn = ds.getConnection();
       conn.setAutoCommit(false);   ← Begin (= MySQL: START TRANSACTION)
            │
            ▼
       try {
           // tera method body chala
           // INSERT/UPDATE queries fire hui

            ↓ success
            conn.commit();           ← MySQL: COMMIT;

       } catch (RuntimeException e) {
            conn.rollback();         ← MySQL: ROLLBACK;
            throw e;
       } finally {
            conn.close();
       }
```

### 💎 Power phrase (interview gold)

> **"`@Transactional` actually MySQL/JDBC transaction commands ka wrapper hai. Spring AOP proxy `Connection.setAutoCommit(false)` set karta hai shuru mein, queries fire karta, success pe `commit()`, exception pe `rollback()`. Khud raw JDBC ya MySQL mein same kaam karenge — Spring sirf boilerplate bachata hai."**

**Yeh interviewer ko impress karega** kyunki most candidates abstraction ke peeche jhaake nahi dekhte — tu **DB layer tak deep** soch raha hai.

### 🔑 Yaad rakhne ka tarika

```
   @Transactional   ≡   START TRANSACTION ... COMMIT/ROLLBACK
   
   Spring ka kaam   =   Tujhe `setAutoCommit(false)`,
                        `commit()`, `rollback()` manually
                        nahi likhna padta
   
   "Magic" nahi    =   Pure JDBC/SQL ka wrapper
```

### 🎨 Visual — Spring Proxy Interception

```
  Client Code (Controller)
        │
        │  service.transferMoney(...)
        ▼
  ┌───────────────────────────────────────┐
  │       SPRING PROXY (wrapper)          │
  │                                       │
  │   ┌─────────────────────────┐         │
  │   │ 1. TX BEGIN             │ ⚙️       │
  │   └─────────────────────────┘         │
  │                                       │
  │   ┌─────────────────────────┐         │
  │   │ 2. Call REAL method ─┐  │         │
  │   └─────────────────────┬┘  │         │
  │                         │   │         │
  │                         ▼   │         │
  │   ┌─────────────────────────┐         │
  │   │  REAL UserService       │         │
  │   │  transferMoney() {      │         │
  │   │    debit(...)           │         │
  │   │    credit(...)          │         │
  │   │  }                      │         │
  │   └─────────┬───────────────┘         │
  │             │                         │
  │   ┌─────────▼───────────────┐         │
  │   │ 3. Exception?           │         │
  │   │    YES → ROLLBACK ⏪    │         │
  │   │    NO  → COMMIT ✅      │         │
  │   └─────────────────────────┘         │
  │                                       │
  └───────────────────────────────────────┘

  💡 Tu sirf @Transactional likhta — Spring proxy
     baaki sab automatic kar deta. Magic AOP se hai.
```

---

## 🔥 PROPAGATION — 7 Types (interview gold)

**Question:** Jab `serviceA` se `serviceB` call hua, dono `@Transactional` → kya `serviceB` nayi transaction banayega ya same use karega?

**Answer = Propagation type.**

### 🥇 Tier 1 (90% interview yahan se)

| Type | Meaning | Example use case |
|---|---|---|
| **REQUIRED** *(default)* | Parent TX hai → join. Nahi hai → naya banao. | Default — 90% cases |
| **REQUIRES_NEW** | Hamesha NAYA TX. Parent ko suspend kar do. | Audit logs (independent commit) |
| **NESTED** | Parent ke andar **savepoint**. Sub-fail → savepoint tak rollback. | Try-and-fallback patterns |

### 🥈 Tier 2 (kabhi-kabhi)

| Type | Meaning |
|---|---|
| **SUPPORTS** | Parent hai → join. Nahi hai → bina TX chalo. |
| **NOT_SUPPORTED** | Koi TX nahi chahiye. Parent hai → suspend. |
| **MANDATORY** | Parent TX HONA chahiye. Nahi hai → exception. |
| **NEVER** | Parent TX NAHI hona chahiye. Hai → exception. |

### 🎨 Visual — All 7 Propagation Types Side-by-Side

```
  Caller has TX?              No TX?

  REQUIRED       Join existing       Create new
                 ────────────        ───────────
  
  REQUIRES_NEW   Suspend + new       Create new
                 ──────────────      ───────────
  
  NESTED         Savepoint           Create new
                 ─────────           ───────────
  
  SUPPORTS       Join existing       Run without TX
                 ────────────        ──────────────
  
  NOT_SUPPORTED  Suspend + no TX     Run without TX
                 ────────────────    ──────────────
  
  MANDATORY      Join existing       💥 Exception
                 ────────────        ────────────
  
  NEVER          💥 Exception        Run without TX
                 ────────────        ──────────────
```

### 🌳 Decision Tree — Konsa Propagation Use Karu?

```
                    ┌──────────────────────────┐
                    │ Method DB work karta hai?│
                    └────────────┬─────────────┘
                                 │
                    ┌────────────┴───────────────┐
                   YES                          NO
                    │                            │
                    ▼                            ▼
       ┌───────────────────────┐       NOT_SUPPORTED
       │ Independent commit    │       (rare)
       │ chahiye even if       │
       │ parent fail?          │
       └────┬─────────────┬────┘
            │             │
           YES            NO
            │             │
            ▼             ▼
       REQUIRES_NEW    REQUIRED  ← 90% cases yeh
       (audit logs,    (default,
        notifications) joint commit)
```

### Visual: REQUIRED (default flow)

```
serviceA() shuru
  ├─ Spring: TX-1 begin (naya banaya)
  ├─ DB work (TX-1)
  ├─ serviceB() call → REQUIRED → join TX-1
  │   └─ DB work (same TX-1)
  └─ serviceB() return → still in TX-1

serviceA() end
  └─ Spring: TX-1 commit / rollback
```

**Key:** Dono ek hi transaction. ServiceB mein exception → **dono rollback**.

### Visual: REQUIRES_NEW

```
serviceA() shuru
  ├─ TX-1 begin
  ├─ DB work (TX-1)
  ├─ serviceB() call → REQUIRES_NEW → TX-1 SUSPEND, TX-2 begin
  │   ├─ DB work (TX-2)
  │   └─ TX-2 commit (independent)
  ├─ TX-1 RESUME
  └─ DB work continue (TX-1)
```

**Key:** ServiceB independent. Even if serviceA later fails, **TX-2 already committed**.

### Code example

```java
@Service
public class OrderService {

    @Transactional   // default = REQUIRED
    public void createOrder(Order order) {
        orderRepo.save(order);
        auditService.log(order);   // joins same TX
    }
}

@Service
public class AuditService {

    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void log(Order order) {
        // independent TX — main flow fail ho bhi log save rahega
        auditRepo.save(new AuditLog(order));
    }
}
```

### 💎 Power phrase
> **"Default REQUIRED hai — 90% cases mein yahi sahi. REQUIRES_NEW use karte hain audit logs jaisi independent operations ke liye, jo main flow fail hone par bhi save honi chahiye."**

---

## 🔥 ISOLATION — DB Concurrency

**Concurrent transactions ek doosre ko kaise dekhte hain.**

### 3 Concurrency Problems

#### 1. DIRTY READ 🤢
```
Tx-A: UPDATE balance = 9999 (uncommitted)
Tx-B: SELECT balance → reads 9999  😱
Tx-A: ROLLBACK
→ Tx-B has WRONG data based on stuff that never existed
```

#### 2. NON-REPEATABLE READ 🔁
```
Tx-A: SELECT balance WHERE id=5 → 100
Tx-B: UPDATE balance = 200, COMMIT
Tx-A: SELECT balance WHERE id=5 → 200  😵
→ Same Tx-A ne 2 different values dekhe
```

#### 3. PHANTOM READ 👻
```
Tx-A: SELECT * WHERE age > 18 → 5 rows
Tx-B: INSERT new user age=25, COMMIT
Tx-A: SELECT * WHERE age > 18 → 6 rows  👻
→ New "phantom" row appear
```

### 4 Isolation Levels — Matrix

| Level | Dirty Read | Non-Repeatable | Phantom | Performance |
|---|---|---|---|---|
| **READ_UNCOMMITTED** | ❌ allow | ❌ allow | ❌ allow | 🚀 fastest |
| **READ_COMMITTED** *(Postgres/Oracle default)* | ✅ block | ❌ allow | ❌ allow | Fast |
| **REPEATABLE_READ** *(MySQL InnoDB default)* | ✅ block | ✅ block | ❌ allow | Medium |
| **SERIALIZABLE** | ✅ block | ✅ block | ✅ block | 🐢 slowest |

**Pattern:** Higher level = stronger consistency, slower performance.

### 🎨 Visual — Isolation Ladder (locking + speed trade-off)

```
       Strict (slow)
          ▲
          │
  ┌───────┴───────────────────────────────┐
  │ SERIALIZABLE                          │  ← banking/financial
  │ All locks. All problems blocked.      │     (rare, very slow)
  ├───────────────────────────────────────┤
  │ REPEATABLE_READ        (MySQL default)│  ← strong by default
  │ Row locks. Phantom still possible.    │
  ├───────────────────────────────────────┤
  │ READ_COMMITTED   (Postgres/Oracle def)│  ← 90% production
  │ Read latest committed only.           │     this is enough
  ├───────────────────────────────────────┤
  │ READ_UNCOMMITTED                      │  ← analytics dashboards
  │ Anything goes. Dirty reads allowed.   │     (rare, super fast)
  └───────────────────────────────────────┘
          │
          ▼
       Loose (fast)
```

### 🔍 Concurrency Problems — Quick Visual Recap

```
DIRTY READ:
  Tx-A:  UPDATE x=99 (uncommitted) ──► Tx-B reads 99 ──► Tx-A rollback
                                       ❌ B has fake data
                                       
NON-REPEATABLE:
  Tx-A: SELECT x ──► 100      Tx-B: UPDATE x=200 COMMIT
                                            │
                                            ▼
              Tx-A: SELECT x ──► 200  ❌ same TX, different value
              
PHANTOM:
  Tx-A: SELECT WHERE age>18 ──► [5 rows]    Tx-B: INSERT new row
                                                    │
                                                    ▼
              Tx-A: SELECT WHERE age>18 ──► [6 rows] 👻 new row appeared
```

### Spring mein set karna

```java
@Transactional(isolation = Isolation.READ_COMMITTED)
public void someMethod() { ... }
```

**Default = `Isolation.DEFAULT`** → DB ka apna default use karta.

### 💎 Power phrase
> **"99% cases mein default chodna best — DB ka level kaafi hai. SERIALIZABLE sirf banking/financial mein. High level = strong consistency but locking zyada → throughput kam."**

---

## 🔥 ROLLBACK RULES — Classic Gotcha

> **Default rule: Spring rollback karta hai SIRF `RuntimeException` (Unchecked) aur `Error` pe. Checked exception pe rollback NAHI.**

### The Surprise

```java
@Transactional
public void saveUser(User user) {
    userRepo.save(user);
    if (somethingWrong) {
        throw new IOException("file error");   // Checked exception
    }
}
```

**Kya hua?**
- `IOException` (Checked) throw hua
- Spring: "Yeh checked hai, mera rollback rule trigger nahi hoga"
- **`save(user)` COMMIT** ho gaya 😱

### 🎨 Visual — Rollback Decision Flow

```
              Method threw an exception?
                       │
              ┌────────┴────────┐
              │                 │
            YES                NO
              │                 │
              ▼                 ▼
         What kind?         COMMIT ✅
              │
   ┌──────────┴──────────┐
   │                     │
   ▼                     ▼
 Unchecked            Checked
 (RuntimeException,   (IOException,
  NullPointerExc,     SQLException,
  Custom RtException) custom Exception)
   │                     │
   ▼                     ▼
ROLLBACK ⏪           COMMIT ✅ 😱
(automatic)           (DEFAULT — surprising!)
                      
                      Override:
                      @Transactional(
                        rollbackFor = Exception.class
                      )
                      → ROLLBACK ⏪
```

### Fix Options

#### Option 1: `rollbackFor`
```java
@Transactional(rollbackFor = IOException.class)
public void saveUser(User user) throws IOException { ... }
```

#### Option 2: `rollbackFor = Exception.class` (catch-all)
```java
@Transactional(rollbackFor = Exception.class)   // saari exceptions pe rollback
```

#### Option 3: Throw RuntimeException (most common)
```java
public class UserNotFoundException extends RuntimeException {
    public UserNotFoundException(Long id) {
        super("User not found: " + id);
    }
}
```

### Reverse — `noRollbackFor`
```java
@Transactional(noRollbackFor = NotFoundException.class)
public void someMethod() {
    // NotFoundException pe COMMIT (rollback nahi)
}
```

### 💎 Power phrase
> **"Default sirf RuntimeException pe rollback. Custom exceptions hamesha RuntimeException extend karne chahiye — Spring philosophy ke saath align, automatic rollback bhi mil jata, signatures clean rehte."**

### ⚠️ TRAP BOX

```
🪤 90% candidates yeh galat sochte:
   "Saari exceptions pe rollback hota"
   
   ❌ NAHI — sirf Unchecked (RuntimeException + Error)
   ✅ Checked exceptions silently commit
```

---

## 🔥 SELF-INVOCATION PITFALL — Deepest Gotcha

> **95% candidates yahin fasate hain. Senior dev filter question.**

### Setup
```java
@Service
public class UserService {

    public void doStuff() {
        saveUser(new User("Arpan"));   // ← internal call (this.saveUser)
    }

    @Transactional
    public void saveUser(User user) {
        userRepo.save(user);
        if (somethingFails) {
            throw new RuntimeException("oops");
        }
    }
}
```

**Naive expectation:** `@Transactional` hai → rollback hoga
**Reality:** ❌ **NO rollback. User save ho gaya.**

### Why?

`@Transactional` Spring **AOP Proxy** se kaam karta hai.

```
External call (Controller → Service):
  Controller → PROXY → [TX intercept] → real method
                ↑
              "yahan magic"

Self-invocation:
  methodA() → this.methodB()  ← `this` = real instance, NOT proxy!
            → Direct call, proxy bypassed
            → @Transactional IGNORED
```

**Reason:** `this.method()` direct invocation hai — proxy ke through nahi jata. Annotation hook trigger nahi hota.

### 🎨 Visual — Self-Invocation Bypass (deepest gotcha)

```
  EXTERNAL CALL (Controller → Service):
  ─────────────────────────────────────
  
       Controller
           │
           ▼
   ┌──────────────────────────────┐
   │  PROXY of UserService 🛡️     │  ← Spring magic
   │  • TX intercept ✅           │
   │  • Annotation read ✅        │
   └──────────┬───────────────────┘
              │
              ▼
   ┌──────────────────────────────┐
   │  REAL UserService            │
   │  saveUser() { ... }          │  ✅ TRANSACTIONAL
   └──────────────────────────────┘
   
   
  SELF-INVOCATION (within same class):
  ────────────────────────────────────
  
       Controller
           │
           ▼
   ┌──────────────────────────────┐
   │  PROXY of UserService 🛡️     │
   │  • Intercept doStuff() ✅    │
   └──────────┬───────────────────┘
              │
              ▼
   ┌──────────────────────────────┐
   │  REAL UserService            │
   │                              │
   │  doStuff() {                 │
   │    this.saveUser(); ─────┐   │  ← `this` = REAL instance
   │  }                       │   │     (proxy is NOT involved)
   │                          │   │
   │  @Transactional          │   │
   │  saveUser() { ... } ◄────┘   │  ❌ NOT TRANSACTIONAL
   │                              │     (proxy bypass!)
   └──────────────────────────────┘
   
   💀 Kaaran: this.saveUser() proxy ke through nahi jata
              direct method invocation hai
              Spring ka @Transactional hook trigger nahi hota
```

### Solutions

#### Solution 1: Move to different class (CLEANEST)
```java
@Service
public class UserService {
    @Autowired private SaveService saveService;

    public void doStuff() {
        saveService.saveUser(new User("Arpan"));   // proxy via injection ✅
    }
}

@Service
public class SaveService {
    @Transactional
    public void saveUser(User user) { ... }
}
```

#### Solution 2: Self-injection (works, hacky)
```java
@Service
public class UserService {
    @Autowired private UserService self;   // ← self-injection

    public void doStuff() {
        self.saveUser(new User("Arpan"));   // through proxy ✅
    }

    @Transactional
    public void saveUser(User user) { ... }
}
```

#### Solution 3: AspectJ mode (advanced)
```java
@EnableTransactionManagement(mode = AdviceMode.ASPECTJ)
```
Bytecode-level injection — self-invocation bhi intercept. Rare in production.

### Related Gotchas

```java
@Service
public class UserService {
    @Transactional
    private void privateMethod() { ... }   // ❌ proxy can't intercept private
    
    @Transactional
    public final void finalMethod() { ... }   // ❌ proxy can't override final
    
    @Transactional
    public static void staticMethod() { ... }   // ❌ proxy intercepts non-static
}
```

### 💎 Power phrase
> **"`@Transactional` self-invocation pe fail hota — proxy bypass ho jata. Always public methods, alag service mein nikal do. Private/final/static pe bhi useless."**

---

## 🔥 CLASS vs METHOD LEVEL

### Method-level (most common)
```java
@Service
public class UserService {

    @Transactional
    public void saveUser(User user) { ... }     // transactional ✅

    public List<User> getUsers() { ... }         // NOT transactional
}
```

### Class-level (shorthand)
```java
@Service
@Transactional   // ← class pe lagaya
public class UserService {

    public void saveUser(User user) { ... }      // transactional ✅
    public void deleteUser(Long id) { ... }      // transactional ✅
}
```

### Combination — Method overrides Class
```java
@Service
@Transactional                                   // default for all
public class UserService {

    public void saveUser() { ... }               // uses class default

    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void auditLog() { ... }               // overrides

    @Transactional(readOnly = true)
    public List<User> getUsers() { ... }         // overrides
}
```

**Most specific wins.** Method-level beats class-level.

---

## ⚡ BONUS — `readOnly = true`

```java
@Transactional(readOnly = true)
public List<User> getAllUsers() {
    return userRepo.findAll();
}
```

**3 fayde:**
1. **Hibernate optimization** — dirty checking skip (no UPDATE generated)
2. **DB hint** — replicas pe route ho sakta (read replica setup)
3. **Documentation** — code padhne wale ko clear

**Best practice:** Saare SELECT methods pe `readOnly = true` lagao.

---

## ⚡ BONUS — `timeout`

```java
@Transactional(timeout = 5)   // 5 seconds
public void slowOperation() { ... }
```

**Use case:** Long-running operations pe limit — infinite hang se bachao.

---

## 🎯 PUTTING IT ALL TOGETHER

```java
@Transactional(
    readOnly = true,
    propagation = Propagation.REQUIRED,
    isolation = Isolation.READ_COMMITTED,
    timeout = 10,
    rollbackFor = Exception.class
)
public List<Order> findRecentOrders(Long userId) {
    return orderRepo.findByUserId(userId);
}
```

---

## 📋 INTERVIEW ANSWER TEMPLATES

### Q: "`@Transactional` kya karta hai?"
> "Method ko **atomic database transaction** mein wrap karta hai. Spring AOP proxy automatically transaction begin karta method shuru pe, commit karta successful end pe, aur rollback karta exception pe. Boilerplate `try/catch/commit/rollback` code remove ho jata."

### Q: "Default propagation kya hai aur kyu?"
> "Default = **REQUIRED**. Most use cases mein outer + inner method ek logical unit hote — ek fail = saara fail. Yahi business expectation hota."

### Q: "REQUIRED vs REQUIRES_NEW?"
> "REQUIRED → join existing TX (default behavior). REQUIRES_NEW → independent TX, audit logs jaisi cheezein jo main flow fail hone par bhi save honi chahiye."

### Q: "Default rollback behavior?"
> "Sirf `RuntimeException` (Unchecked) aur `Error` pe automatic rollback. **Checked exceptions pe NAHI** — `rollbackFor` lagana padta. Best practice: custom exceptions hamesha `RuntimeException` extend karo."

### Q: "Self-invocation problem?"
> "`@Transactional` AOP proxy se trigger hota. Same class ke andar `this.method()` call kare → proxy bypass → annotation ignore. Solution: alag service mein nikalo, ya self-injection."

### Q: "Konse cases mein `@Transactional` work nahi karta?"
> "**3 cases:** (1) Self-invocation, (2) Private/final methods (proxy override nahi kar sakta), (3) Static methods."

### Q: "Konsa isolation use karoge?"
> "99% cases mein DB ka default — Postgres ka READ_COMMITTED kaafi hai. Banking/financial → SERIALIZABLE. High level = strong consistency but locking zyada."

---

## 💎 KEY TAKEAWAYS

| # | Lock these |
|---|---|
| 1 | **Default propagation = REQUIRED**, default isolation = DB ka |
| 2 | **REQUIRES_NEW** = independent TX (audit logs) |
| 3 | **Default rollback = sirf RuntimeException** (Checked NAHI) |
| 4 | **Self-invocation pe FAIL** — proxy bypass |
| 5 | **Always public methods** — private/final useless |
| 6 | **`readOnly = true`** SELECT methods pe lagao |
| 7 | Custom exceptions hamesha **`RuntimeException` extend** karo |

---

## ⚠️ TRAP SUMMARY

```
🪤 Trap 1: "Saari exceptions pe rollback hota"
         ❌ Nahi — sirf Unchecked
         
🪤 Trap 2: "Same class ke method call mein bhi @Transactional kaam karta"
         ❌ Nahi — proxy bypass
         
🪤 Trap 3: "Private method pe @Transactional kaam karta"
         ❌ Nahi — proxy override nahi kar sakta
         
🪤 Trap 4: "MySQL aur Postgres ka isolation default same hai"
         ❌ Nahi — MySQL = REPEATABLE_READ, Postgres = READ_COMMITTED
```

---

## 🧠 MEMORY HOOK — One-line per concept

```
@Transactional       =  "Auto try-catch-commit-rollback wrapper"

REQUIRED             =  "Join karo ya naya banao" (default, 90%)
REQUIRES_NEW         =  "Hamesha alag, parent ko suspend"
NESTED               =  "Parent ke andar savepoint"

DIRTY READ           =  "Uncommitted dekh liya" 🤢
NON-REPEATABLE READ  =  "Same query 2x = different result" 🔁
PHANTOM READ         =  "New row appeared in range" 👻

READ_UNCOMMITTED     =  "Sab dikha raha hai, even uncommitted"
READ_COMMITTED       =  "Sirf committed dikhao" (Postgres default)
REPEATABLE_READ      =  "Same TX = same answer" (MySQL default)
SERIALIZABLE         =  "Ek time mein ek hi banda" (banking)

ROLLBACK DEFAULT     =  "Sirf RuntimeException + Error"
SELF-INVOCATION      =  "this.method() = proxy bypass = no TX"
PRIVATE METHOD       =  "Proxy override nahi kar sakta = no TX"
readOnly = true      =  "Hibernate optimization for SELECT"
```

---

## 🎯 FINAL VISUAL — `@Transactional` ka full flow

```
                    @Transactional
                        │
                        ▼
        ┌───────────────────────────────────┐
        │      Spring Boot Application      │
        └───────────────┬───────────────────┘
                        │
                        ▼
            ┌──────────────────────┐
            │  AOP Proxy Created   │
            │  (CGLIB / JDK Proxy) │
            └──────────┬───────────┘
                       │
       ┌───────────────┼───────────────────┐
       ▼               ▼                   ▼
  Method begin    Method success      Method exception
       │               │                   │
       ▼               ▼                   ▼
  TX BEGIN         COMMIT ✅         Unchecked? Yes ⏪
                                     Checked?   No ✅
                                                 │
                                            (use rollbackFor
                                             to override)

  Settings to tune:
    • propagation  = REQUIRED / REQUIRES_NEW / NESTED ...
    • isolation    = READ_COMMITTED / SERIALIZABLE ...
    • rollbackFor  = Exception.class / IOException.class
    • readOnly     = true / false
    • timeout      = seconds

  Common pitfalls to avoid:
    🚫 Self-invocation (this.method())
    🚫 Private/final/static methods
    🚫 Default rollback (only Unchecked)
```
