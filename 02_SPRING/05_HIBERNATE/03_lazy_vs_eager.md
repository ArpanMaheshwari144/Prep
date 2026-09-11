# LAZY vs EAGER Fetch

---

## 1 Concept (Menu Analogy)

```
Restaurant mein 2 ways menu dekhna:

LAZY (default for collections):
   Tu sirf menu COVER dekhta (Author)
   Pasta page nahi kholta jab tak NEED
   Items LOAD on demand

EAGER:
   Tu menu uthate hi SAARE pages khol diya
   Pizza, pasta, dessert — sab dikh raha
   Items LOAD upfront
```

---

## 2 Code

```java
class Author {
    @Id Long id;
    String name;

    @OneToMany(fetch = FetchType.LAZY)    // ← DEFAULT for collections
    List<Book> books;

    @ManyToOne(fetch = FetchType.EAGER)   // ← DEFAULT for single ref
    Publisher publisher;
}
```

---

## 3 Visual — What Loads When?

```
findById(1L) called:

LAZY:
   ┌────────────────────┐
   │  Author #1          │  ← loaded immediately
   │  id: 1               │
   │  name: "Arpan"       │
   │  books: NOT LOADED│  ← placeholder
   └────────────────────┘

   Tu .getBooks() call kare → TAB DB query
   = on demand

EAGER:
   ┌────────────────────┐
   │  Author #1          │  ← loaded immediately
   │  id: 1               │
   │  name: "Arpan"       │
   │  books: [B1, B2, B3] │  ← ALSO loaded
   │  publisher: {...}    │  ← ALSO loaded
   └────────────────────┘

   Sab kuch upfront
```

---

## 4 Default Behavior (Yaad Rakh)

```
┌────────────────────┬────────────┐
│  Annotation        │  Default   │
├────────────────────┼────────────┤
│  @OneToMany        │  LAZY      │
│  @ManyToMany       │  LAZY      │
├────────────────────┼────────────┤
│  @ManyToOne        │  EAGER     │
│  @OneToOne         │  EAGER     │
└────────────────────┴────────────┘
```

```
Mnemonic:
   "TO MANY" → LAZY (many cheezein, sab load? nahi)
   "TO ONE"  → EAGER (sirf 1, le aao saath)
```

---

## 5 LAZY ka Problem — LazyInitializationException

```java
@Service
public class AuthorService {

    public List<Book> getBooks(Long authorId) {
        Author a = repo.findById(authorId).orElseThrow();
        // session CLOSE ho gayi yahaan

        return a.getBooks();    // LazyInitializationException!
    }
}
```

```
Why?
   Hibernate session/transaction band ho gayi
   LAZY books abhi load nahi hua
   Access karte time → no session → exception
```

---

## 6 LAZY Exception — Fix Options

### Option 1: @Transactional (session open rahegi)
```java
@Transactional
public List<Book> getBooks(Long authorId) {
    Author a = repo.findById(authorId).orElseThrow();
    return a.getBooks();   // works — session active
}
```

### Option 2: JOIN FETCH explicit
```java
@Query("SELECT a FROM Author a JOIN FETCH a.books WHERE a.id = :id")
Optional<Author> findByIdWithBooks(@Param("id") Long id);
```

### Option 3: @EntityGraph
```java
@EntityGraph(attributePaths = {"books"})
Optional<Author> findById(Long id);
```

### Option 4: DTO Projection (BEST for REST APIs)
```java
@Transactional(readOnly = true)
public AuthorBooksDTO getAuthor(Long id) {
    Author a = repo.findById(id).orElseThrow();
    return new AuthorBooksDTO(
        a.getId(),
        a.getName(),
        a.getBooks().stream()
            .map(Book::getTitle)
            .toList()
    );
}
```

---

## 7 EAGER ka Problem — Over-fetching

```java
@ManyToOne(fetch = FetchType.EAGER)
Publisher publisher;
```

```
Tu Author chahta sirf:
   findById(1L)

   Hibernate:
      "EAGER hai publisher — load karo"
      SELECT * FROM authors WHERE id = 1;
      SELECT * FROM publishers WHERE id = ?;
      = 2 queries (even though publisher kaam nahi)

Deep tree:
   Author → Publisher → Country → Continent (all EAGER)
   findById(1) → 4 joins
   = Massive waste if you only needed name
```

---

## 8 Best Practice (Industry Standard)

```
RULE 1: ALWAYS use LAZY (override defaults)
   @ManyToOne(fetch = FetchType.LAZY)
   @OneToOne(fetch = FetchType.LAZY)

RULE 2: Fetch what you need:
   • Just author → no fetch hint
   • Author + books → @EntityGraph or JOIN FETCH
   • Specific projection → DTO query

RULE 3: DTO for REST APIs
   Don't return entities directly
```

---

## 9 Comparison

```
┌──────────────────┬─────────────────┬─────────────────┐
│  Aspect          │  LAZY           │  EAGER          │
├──────────────────┼─────────────────┼─────────────────┤
│ When loaded      │ On access       │ With parent     │
│ Initial query    │ 1 (parent only) │ 1+ (with joins) │
│ Risk             │ LazyInitException│ Over-fetching   │
│ Performance      │ Better tuned    │ Risky default   │
│ Default          │ Collections     │ Single refs     │
│ Industry         │ PREFERRED    │ avoid        │
└──────────────────┴─────────────────┴─────────────────┘
```

---

## Interview Power Phrase

```
"LAZY loads related entities ON ACCESS, EAGER UPFRONT.
 Default: @OneToMany/@ManyToMany = LAZY, @ManyToOne/@OneToOne = EAGER.

 Industry: ALWAYS make all relations LAZY to avoid over-fetching.
 Use @EntityGraph or JOIN FETCH selectively when needed.

 LAZY risk: LazyInitializationException if accessed after
 session closes — fix with @Transactional or DTO projection."
```

---

## Memory Hook

```
LAZY    = load when needed (on access)
EAGER   = load upfront

Defaults:
   ToMany → LAZY
   ToOne  → EAGER

Rule: ALWAYS LAZY, fetch explicitly when needed
```

---

## ★ PROJECT CONNECT — usercrud Author/Book (8-Sep)
```
Author.books  @OneToMany(mappedBy="author")  -> LAZY (default ToMany) -> books tabhi load jab access
Book.author   @ManyToOne @JoinColumn         -> EAGER (default ToOne) -> author saath aa jaata
```
Tere note ka "ToMany=LAZY, ToOne=EAGER" LIVE. Author.books LAZY hi N+1 ki jad thi ([N1Controller](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/N1Controller.java) me dekha).

---

## ★ HANDS-ON DEMO — LazyInitException LIVE + 2 fix (usercrud, 9-Sep)

> [`LazyDemoController.java`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/LazyDemoController.java) — 3 endpoint, teeno Postman se chalaye. Exception khud laayi, phir 2 tareeke se fix.
> Ye "theory padhi" se aage — actual 500 dekha, phir 200 dekha. Yahi asli samajh.

### Endpoint 1 — `/lazy/fail` (exception KHUD laayi)
```java
@GetMapping("/lazy/fail")
public String fail() {
    Author author = authorRepo.findById(1L).orElseThrow();
    int n = author.getBooks().size();   // <- session band, LAZY chhua -> BOOM
    return "books = " + n;
}
```
**LIVE result — 500 Internal Server Error:**
```
"Cannot lazily initialize collection of role 'com.arpan.usercrud.model.Author.books'
 with key '1' (no session)"
```
Kyun: controller me `@Transactional` NAHI -> `findById` khatam hote hi session band ->
phir `getBooks()` pe Hibernate DB-trip chahta -> session hi nahi -> `LazyInitializationException`.

### Endpoint 2 — `/lazy/fix-txn` (@Transactional se fix)
```java
@Transactional                         // <- poore method me session ZINDA
@GetMapping("/lazy/fix-txn")
public String fixTxn() {
    Author author = authorRepo.findById(1L).orElseThrow();
    int n = author.getBooks().size();  // session khuli -> extra SELECT -> books aaye
    return "books = " + n;
}
```
**LIVE result — 200 OK:** `books = 3`
Trade-off: ye 2 query (author + books alag). 100 author hote to N+1 ban jaata.

### Endpoint 3 — `/lazy/fix-fetch` (JOIN FETCH — best)
```java
@GetMapping("/lazy/fix-fetch")
public String fixFetch() {
    List<Author> authors = authorRepo.findAllWithBooks();   // JOIN FETCH
    return "authors = " + authors.size() + ", first author books = " + authors.get(0).getBooks().size();
}
```
```java
// AuthorRepository
@Query("SELECT DISTINCT a FROM Author a LEFT JOIN FETCH a.books")
List<Author> findAllWithBooks();
```
**LIVE result — 200 OK:** `authors = 5, first author books = 3`
Author + books EK query me (LEFT JOIN) -> `getBooks()` pe koi extra DB-trip nahi -> N+1 bhi nahi.

**★ Ek line (delivery):** "LAZY default rakho; jahan collection sach me chahiye wahan JOIN FETCH se explicit le aao."

---

## ★★ OSIV — Open-Session-In-View (LAZY ka #1 interview follow-up)

> Upar `/lazy/fail` me exception AAYI. Par bahut logon ke real Boot-app me lazy-access controller me
> crash NAHI karti — kyun? **OSIV** ki wajah se. Ye interview me lazy ke turant baad poochte.

### Kya hai
```
OSIV = Open-Session-In-View. Spring Boot me DEFAULT = ON (spring.jpa.open-in-view=true).
ON  -> Hibernate session poore HTTP-REQUEST ke end tak khuli rehti (controller/view-render tak).
    -> to controller me LAZY chhuo tab bhi session khuli -> load ho jaata -> koi exception NAHI.
```

### To mera demo crash kyun hua?
```
Kyunki maine application.properties me set kiya:  spring.jpa.open-in-view=false
   -> session jaldi band (service/repo ke baad) -> controller me getBooks() = no session -> exception.
Agar ye true (default) hota to /lazy/fail bhi 200 deta.  <- ISI liye demo ke liye false kiya.
```

### Trade-off (interview GOLD — dono side bol)
```
OSIV ON (default):
   + lazy kabhi controller me crash nahi -> dev-comfort, kam boilerplate
   - DB CONNECTION poore request-bhar HOLD rehta (view-render / slow-client / JSON-serialize tak)
     -> connection-pool jaldi khatam -> high-load pe throughput girta, requests wait

OSIV OFF (production-clean):
   + connection service-layer ke baad TURANT release -> pool efficient -> scale achha
   - lazy-access controller-ke-bahar crash -> khud explicit laana padta (@Transactional / JOIN FETCH / DTO)
```

### ★ Power-phrase
```
"OSIV Boot me default ON — session request-end tak khuli, isliye lazy controller me crash nahi karti.
 Convenient, PAR DB-connection poore request hold rehta -> high-load pe pool-exhaustion.
 Production me main OFF karta + service-layer me @Transactional/JOIN FETCH se data explicit fetch karta,
 taaki connection jaldi release ho aur lazy-access kabhi controller me leak na ho."
```

### Connect
```
open-in-view=false -> session jaldi band -> ye WAHI "managed vs detached" line jo dirty-check/opt-lock me thi.
   (session ke andar = managed; bahar = detached -> lazy/dirty/stale sab issue wahin se). [[04_dirty_checking]]
```

> **Ek line:** OSIV ON = aaram par connection-hog; OFF = clean par lazy khud manage (@Transactional/fetch-join).
> Prod = OFF + explicit fetch = mature choice. (Mere demo ne OFF karke hi exception dikhayi.)
