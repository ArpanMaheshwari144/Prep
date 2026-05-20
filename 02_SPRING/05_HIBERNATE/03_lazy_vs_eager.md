# LAZY vs EAGER Fetch

---

## 1️⃣ Concept (Menu Analogy)

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

## 2️⃣ Code

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

## 3️⃣ Visual — What Loads When?

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

## 4️⃣ Default Behavior (Yaad Rakh)

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

## 5️⃣ LAZY ka Problem — LazyInitializationException

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

## 6️⃣ LAZY Exception — Fix Options

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

## 7️⃣ EAGER ka Problem — Over-fetching

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

## 8️⃣ Best Practice (Industry Standard)

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

## 9️⃣ Comparison

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
