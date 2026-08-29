# Optimistic Locking (@Version) — lost-update roko bina lock lagaye

> 2 log ek hi row ek saath badlein -> ek ka update doosre ka mita de = LOST UPDATE.
> @Version = ek counter. Update tabhi lage jab version match kare; na match -> reject (koi beech me badal gaya).
> No lock. Sirf version compare.

---

## PROBLEM — Lost Update
```
Stock = 100.
User-A: "10 becha -> 90 karo"    |   User-B: "20 becha -> 80 karo"
dono ne ek saath 100 padha -> dono save -> ek ka update UD jaata (lost). Galat.
```

## @Version = "edition number"
```
Row me ek counter: version. Har update pe version +1 (JPA khud manage karta).
Update ke waqt: teri version == DB ki version?  HAAN -> save + version++  |  NAHI -> reject.
```

## PESSIMISTIC vs OPTIMISTIC
```
PESSIMISTIC (row-lock / SELECT ... FOR UPDATE): pehle LOCK lo, phir update. (deadlock/wait ka risk)
OPTIMISTIC (@Version): lock NAHI. update pe version-check. clash rare ho to fast.
  -> high-contention = pessimistic | low-contention = optimistic (retry on conflict).
```

---

## ═══ HANDS-ON — OptimisticLock LIVE (usercrud, 22-Aug) ═══

> Product me @Version. 2 "users" ne version=0 padha. A ne save kiya (version 0->1).
> B ne bhi version=0 bheja (stale) -> OptimisticLockException -> reject.

### 1. Entity (@Version)
```java
@Entity @Data
public class Product {
    @Id @GeneratedValue(strategy = GenerationType.IDENTITY) Long id;
    String name;
    int stock;
    @Version Long version;   // JPA manage -> har update pe auto +1
}
```

### 2. Repository
```java
public interface ProductRepository extends JpaRepository<Product, Long> {}
```

### 3. Controller (GET + version-wala PUT)
```java
@RestController
public class OptLockController {
    private final ProductRepository repo;   // ctor inject

    @GetMapping("/product/{id}")
    public Product get(@PathVariable Long id) { return repo.findById(id).orElse(null); }

    @PutMapping("/product/{id}")
    public String update(@PathVariable Long id, @RequestParam int stock, @RequestParam Long version) {
        Product p = new Product();
        p.setId(id); p.setName("Widget"); p.setStock(stock);
        p.setVersion(version);              // client ki (shायद stale) version
        try {
            repo.save(p);                  // merge -> DB-version se match; stale -> exception
            return "OK updated -> stock=" + stock;
        } catch (ObjectOptimisticLockingFailureException e) {
            return "409 CONFLICT: version STALE! kisi aur ne beech me update kiya. Dobara padho + retry.";
        }
    }
}
```
+ SecurityConfig: `.requestMatchers("/product/**").permitAll()`
+ seed: `INSERT INTO product (name, stock, version) VALUES ('Widget', 100, 0);`

### 4. RUN + TRACE (jo hua)
```
Shuru: id=1, stock=100, version=0.  (dono users ne v0 padha)

curl -X PUT ".../product/1?stock=90&version=0"
   -> DB check: A-version(0) == DB(0)?  HAAN -> save. stock=90. version 0->1.
   -> "OK updated -> stock=90"

curl -X PUT ".../product/1?stock=80&version=0"     (B ne bhi v0 bheja)
   -> DB check: B-version(0) == DB(1)?  NAHI -> OptimisticLockException
   -> "409 CONFLICT: version STALE!"
```
B ka stale-write REJECT -> A ka 90 bacha -> lost-update ruk gaya.

### 4b. CONFUSION CLEAR — "B version=1 bhej de to update ho jaayega na?"
```
Technically HAAN: B ?version=1 bheje -> DB-version(1) == 1 -> match -> save. version 1->2.
PAR ye GALAT hoga agar B ne bas number 0->1 badal diya (bina fresh padhe):
   B ne stock=80 SOCHA jab usne 100 padha tha. Par A ne 90 kar diya.
   B blindly v1 bheje -> 90 ko 80 se OVERWRITE -> apni STALE soch pe -> wahi lost-update wapas!
   (version-check ka poora fayda khatam.)

version = magic KEY nahi, "maine LATEST padha" ka SABOOT hai. manually bump = safety toot gayi.

SAHI (retry on conflict):
   409 mila -> B DOBARA READ kare fresh (stock=90, version=1)
            -> logic FRESH pe (90-20 = 70)
            -> ab version=1 ke saath bheje -> match -> save. stock=70, version=2.  SAHI.

EK LINE: 409 pe version badalna nahi -> RE-READ + RECOMPUTE + RESUBMIT.
```

### 5. Andar ka mechanism (SQL)
```sql
-- Hibernate WHERE me version bhi daalta:
UPDATE product SET stock=80, version=1 WHERE id=1 AND version=0;
-- DB me version ab 1 hai -> WHERE match NAHI -> 0 rows updated -> Hibernate exception phenkta.
```

### Real-life anchor
```
Wikipedia edit-conflict: 2 log ek page edit. pehle ne save. doosre ne save dabaaya ->
"page badal gaya, dobara load karo." = version match nahi hui -> save reject. Bilkul wahi.
```

### ★ MERA EXPERIENCE (interview me bolna — "khud face kiya") — Confluence / Jira
```
Confluence page (ya Jira ticket) do log ek saath edit karein -> ek save kare ->
doosre ko error: "This page has been modified since you started editing — reload."
= EXACTLY optimistic locking. Page ki ek version hoti; tumne jab kholi wo version padhi,
  beech me kisi ne save kar diya -> version aage badh gayi -> tumhara save version-mismatch pe REJECT.
Fix wahi: page dobara load (re-read latest) -> apna change fresh pe -> phir save.
-> Interview power: "I've hit this in Confluence/Jira — it's optimistic locking with a version check."
```

## ★★ GEMS
```
1. @Version = counter, har update pe +1. Update pe version-match; mismatch -> OptimisticLockException.
2. Mechanism = UPDATE ... WHERE id=? AND version=? -> 0 rows -> exception. (no lock!)
3. Pessimistic (lock-first, SELECT FOR UPDATE) vs Optimistic (version-check-at-write). low-clash = optimistic.
4. Conflict pe app RETRY kare (dobara padho -> naye version se update).
5. Real 409 status: ResponseEntity se bhejo (demo me String return kiya -> 200, par exception fire hua).
```

## INTERVIEW LINE
```
"Optimistic locking @Version se: har row me version counter, update pe UPDATE ... WHERE version=? chalta.
 Beech me kisi ne badla to version match nahi hoti -> 0 rows -> OptimisticLockException -> lost-update ruka.
 Lock nahi lagta (pessimistic ki tरह), isliye low-contention me fast; conflict pe retry karo.
 Pessimistic = lock-first (SELECT FOR UPDATE), optimistic = check-at-write."
```

### Dobara kaise
```
usercrud run -> product seed (version 0) -> PUT ?version=0 (OK, v->1) -> PUT ?version=0 again (409 stale).
```

[← HIBERNATE folder](.)
