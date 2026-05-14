# 💾 First-Level Cache (L1) — Session Cache

---

## 1️⃣ Concept (Office Filing Cabinet Analogy)

```
Imagine office cabinet — sirf TERI desk pe rakhi:
   
   Pehli baar file maange? → Storage room jaake la kar
   Phir desk pe rakhi
   
   Doosri baar same file? → Desk se utha le, no storage trip
   Teesri baar?           → Desk se utha le
   
   Tera shift khatam?     → Desk khali
```

```
Hibernate Session = Tera desk
   L1 cache = same session mein DB se ek baar fetch
              → memory mein cache
              → repeat queries DB hit NAHI karte
```

---

## 2️⃣ Visual Flow

```
   @Transactional method shuru → Session OPEN
        │
        ▼
   userRepo.findById(1L)
        │
        ├── L1 cache check
        │   "User#1 already cache mein?"
        │
        │   NAHI → DB query: SELECT * FROM users WHERE id=1
        │           Cache mein store
        │   HAAN → cache se return (NO DB hit)
        │
        ▼
   Same method mein:
   userRepo.findById(1L)
        │
        ├── L1 cache check
        │   HAAN → cache se return ✅
        │   = NO DB query!
        │
        ▼
   Method end → Session CLOSE
        │
        ▼
   Cache CLEARED
   Next request fresh start
```

---

## 3️⃣ Code Demo

```java
@Transactional
public void demo(Long id) {
    User u1 = userRepo.findById(id).orElseThrow();
    // → DB query: SELECT * FROM users WHERE id=?
    
    User u2 = userRepo.findById(id).orElseThrow();
    // → NO DB query — cache se return
    
    User u3 = userRepo.findById(id).orElseThrow();
    // → NO DB query
    
    u1 == u2;     // TRUE (same object reference)
    u2 == u3;     // TRUE
}
```

```
3 calls, 1 DB query
   = L1 cache magic
```

---

## 4️⃣ Important Properties

```
1. PER-SESSION (per @Transactional)
   Different session = different cache
   No sharing across sessions

2. AUTOMATIC
   No setup needed
   Built-in to Hibernate

3. CANNOT DISABLE
   It's how Hibernate works internally
   
4. CLEARED on session close
   Transaction end → cache gone
```

---

## 5️⃣ Visual — Per Session Cache

```
   Request 1 — Method A    Request 2 — Method B
   ────────────────────    ─────────────────────
   
   Session 1 OPEN          Session 2 OPEN
        │                       │
        ▼                       ▼
   ┌─────────────┐         ┌─────────────┐
   │  L1 Cache   │         │  L1 Cache   │
   │  User#1 ✓   │         │  User#1 ✗   │  ← separate
   │  User#2 ✓   │         │             │     cache
   └─────────────┘         └─────────────┘
        │                       │
   Session CLOSE           Session CLOSE
   Cache deleted           Cache deleted
```

---

## 6️⃣ Useful Use Case

```java
@Transactional
public void transfer(Long fromId, Long toId, BigDecimal amt) {
    Account from = accountRepo.findById(fromId).orElseThrow();
    
    // ... business logic, multiple operations
    
    Account fromAgain = accountRepo.findById(fromId).orElseThrow();
    // No DB query! L1 cache return karta
    
    from == fromAgain;   // TRUE (same object)
}
```

---

## 7️⃣ Common Gotcha — Stale Data

```
Inside same transaction:
   1. findById(1L) → User name "Suresh"
   2. SQL update from elsewhere — name now "Arpan"
   3. findById(1L) → STILL returns "Suresh" (cached!)
   
   = L1 cache doesn't auto-refresh
   = Stale data
```

```java
// Fix:
entityManager.refresh(user);   // force DB re-read
// OR
entityManager.clear();          // empty entire cache
```

---

## 8️⃣ L1 vs L2 Cache (Bonus)

```
┌──────────────────┬───────────────┬─────────────────┐
│  Aspect          │  L1 Cache     │  L2 Cache       │
├──────────────────┼───────────────┼─────────────────┤
│ Scope            │ Per session   │ Across sessions │
│ Default          │ Always ON     │ Disabled        │
│ Configuration    │ None          │ Explicit setup  │
│ Tools            │ Built-in      │ EHCache, Redis  │
│ Use case         │ Within tx     │ App-wide cache  │
└──────────────────┴───────────────┴─────────────────┘
```

```
L2 cache = optional, app-level (read-mostly entities)
L1 cache = automatic, transaction-level
```

---

## 🎤 Interview Power Phrase

```
"Session-scoped cache that's automatic and built-in. 
 Same entity fetched twice in same transaction = 1 DB query 
 (second fetch returns from cache, returns SAME Java object).

 Cleared on session close (transaction commit/rollback).

 Caveats: stale data if external SQL updates the row.
 Refresh with entityManager.refresh() or clear cache when needed."
```

---

## 🎯 Memory Hook

```
L1 Cache = 🗄️ desk drawer (per session)

Same query 5 times in transaction = 1 DB hit
Session end = cache cleared
Cannot disable, automatic
```
