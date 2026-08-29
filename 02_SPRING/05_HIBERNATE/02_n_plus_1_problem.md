# N+1 Problem — Biggest Hibernate Gotcha

---

## 1 Setup — Relationship

```java
@Entity
class Author {
    @Id Long id;
    String name;

    @OneToMany(mappedBy = "author")
    List<Book> books;     // 1 author, many books
}

@Entity
class Book {
    @Id Long id;
    String title;

    @ManyToOne
    Author author;
}
```

---

## 2 The Problem

```java
// Get all authors
List<Author> authors = authorRepo.findAll();   // 1 query

// Loop, access books
for (Author a : authors) {
    System.out.println(a.getBooks().size());   // 1 query EACH author!
}
```

### DB Mein Kya Hota?

```
Query 1:  SELECT * FROM authors;                 ← 1 query
          Returns 10 authors

Query 2:  SELECT * FROM books WHERE author_id=1; ← N queries
Query 3:  SELECT * FROM books WHERE author_id=2;
Query 4:  SELECT * FROM books WHERE author_id=3;
...
Query 11: SELECT * FROM books WHERE author_id=10;

Total = 1 + 10 = 11 queries
= N + 1 (where N = 10 authors)
```

---

## 3 Visual

```
Tera code: "saare authors do, fir har author ki books"

Hibernate dimaag:
     │
     ├── First fetch authors (1 query)
     │
     ├── Author 1 ki books? → DB hit → query 2
     ├── Author 2 ki books? → DB hit → query 3
     ├── Author 3 ki books? → DB hit → query 4
     │   ...
     └── Author N ki books? → DB hit → query N+1

= SLOW, DB hammered, network waste
```

---

## 4 Why Happens?

```
Default: LAZY fetching for @OneToMany
   Books NOT loaded with author initial fetch
   Books load ONLY when accessed

   Loop mein .getBooks() → trigger DB query
   = Per author = 1 query
```

---

## 5 Solution 1 — JOIN FETCH (Most Common)

```java
@Query("SELECT a FROM Author a JOIN FETCH a.books")
List<Author> findAllWithBooks();
```

### DB Single Query

```sql
SELECT a.*, b.*
FROM authors a
LEFT JOIN books b ON b.author_id = a.id;
```

```
JOIN FETCH = "Author lao + uski books bhi same query mein"
   = 1 query total
```

---

## 6 Solution 2 — @EntityGraph (Spring Data)

```java
@EntityGraph(attributePaths = {"books"})
List<Author> findAll();
```

```
attributePaths = "konsi fields saath mein load karna"
   "books" = Author class ka books field

   Spring annotation magic — same JOIN FETCH effect
```

---

## 7 Solution 3 — Batch Fetching

```java
@OneToMany
@BatchSize(size = 10)
List<Book> books;
```

```
Authors fetch (1 query)
Books fetch IN BATCHES of 10:
   SELECT * FROM books WHERE author_id IN (1,...,10);
   SELECT * FROM books WHERE author_id IN (11,...,20);

100 authors → 1 + 10 = 11 queries (not 101)
```

---

## 8 Comparison

```
┌──────────────────┬──────────────┬──────────────┐
│  Approach        │  Queries     │  Best for     │
├──────────────────┼──────────────┼──────────────┤
│ N+1 (default)    │  N+1         │  Anti-pattern │
│ JOIN FETCH       │  1           │  Most cases   │
│ @EntityGraph     │  1           │  Spring Data  │
│ Batch fetching   │  1 + N/batch │  Many entities│
└──────────────────┴──────────────┴──────────────┘
```

---

## 9 Detection

```
1. Enable SQL logging:
   spring.jpa.show-sql=true
   spring.jpa.properties.hibernate.format_sql=true

2. Console mein watch:
   Same query template repeating? → N+1

3. Tools:
   • Hibernate Statistics
   • p6spy
   • Hypersistence Optimizer
```

---

## Production Impact

```
Without fix:
   100 authors → 101 DB queries
   Each ~5ms → 505ms latency
   = SLOW

With JOIN FETCH:
   100 authors → 1 query
   Latency = 5ms
   = 100x faster
```

---

## Interview Power Phrase

```
"Default LAZY fetching causes N+1 queries when accessing
 related entities in a loop. 1 query for parent, then N for children.

 Solutions: JOIN FETCH (JPQL), @EntityGraph annotation,
 or @BatchSize for batched fetching.

 Always enable SQL logging during dev to catch N+1."
```

---

## Memory Hook

```
N+1 = 1 parent query + N child queries

Detection: SQL logs mein same query repeat
Fix:       JOIN FETCH ya @EntityGraph
```

---

## ═══ HANDS-ON — N+1 LIVE dekha usercrud me (khud kiya, 21-Aug) ═══

> Upar theory. Ye section = wahi cheez REAL Spring Boot me chala ke Hibernate SQL logs me dekhi:
> 5 authors ke books laane pe -> BAD = 6 queries (1+5) | GOOD (JOIN FETCH) = SIRF 1 query.
> (project: usercrud. `spring.jpa.show-sql=true` on tha -> har SQL console me print hui.)

### 0. Maqsad
```
1 parent -> kai child (Author -> kai Book) ka relationship banao.
Saare authors laao + har author ke books access karo -> logs me dekho kitni queries chali.
LAZY loading = default -> N+1. JOIN FETCH -> 1 query.
```

### 1. ENTITIES (relationship + LAZY = N+1 ki jad)
```java
@Entity @Data
class Author {
    @Id @GeneratedValue(strategy = GenerationType.IDENTITY) Long id;
    String name;
    @OneToMany(mappedBy = "author")   // 1 author -> kai books. LAZY (default) = books tabhi load jab access
    List<Book> books;
}
@Entity @Data
class Book {
    @Id @GeneratedValue(strategy = GenerationType.IDENTITY) Long id;
    String title;
    @ManyToOne @JoinColumn(name = "author_id")   // kai books -> 1 author (FK author_id)
    Author author;
}
```

### 2. REPOSITORY — ek normal, ek JOIN-FETCH (fix)
```java
interface BookRepository extends JpaRepository<Book, Long> {}

interface AuthorRepository extends JpaRepository<Author, Long> {
    // FIX: JOIN FETCH -> authors + books SAB ek query me. distinct = duplicate authors hatao.
    @Query("SELECT DISTINCT a FROM Author a LEFT JOIN FETCH a.books")
    List<Author> findAllWithBooks();
}
```

### 3. CONTROLLER — bad vs good endpoint
```java
@RestController
class N1Controller {
    private final AuthorRepository authorRepo;   // constructor inject

    @GetMapping("/n1/bad")     // 1 + N
    int bad() {
        List<Author> authors = authorRepo.findAll();            // Query 1 (authors)
        int total = 0;
        for (Author a : authors) total += a.getBooks().size();  // har author -> +1 LAZY query = N
        return total;
    }
    @GetMapping("/n1/good")    // 1
    int good() {
        List<Author> authors = authorRepo.findAllWithBooks();   // Query 1 (JOIN FETCH, sab andar)
        int total = 0;
        for (Author a : authors) total += a.getBooks().size();  // already loaded -> koi extra query nahi
        return total;
    }
}
```
+ SecurityConfig me permit: `.requestMatchers("/n1/**").permitAll()`  (JWT-guard block na kare)

### 4. Kya DEKHA (Hibernate SQL logs)
```
curl /n1/bad  -> BAD (N+1):
   select ... from author                       <- Query 1
   select ... from book where author_id=?       <- Author 1 ke books
   select ... from book where author_id=?       <- Author 2
   select ... from book where author_id=?       <- Author 3
   select ... from book where author_id=?       <- Author 4
   select ... from book where author_id=?       <- Author 5
   = 1 + 5 = 6 QUERIES

curl /n1/good -> FIX:
   select distinct a.*, b.* from author a LEFT JOIN book b on a.id=b.author_id
   = 1 QUERY

   6 -> 1.
```

### 5. KYUN (mechanism)
```
LAZY loading (default @OneToMany): books tabhi load hote jab a.getBooks() ACCESS karo.
 -> loop me har author pe alag SELECT-book query -> N extra queries.
JOIN FETCH: ek hi SQL me author + books dono utha lo (LEFT JOIN) -> per-author query khatam -> 1 query.
 -> distinct: join se ek author uski har book ke saath repeat hota -> distinct duplicate authors hataata.

SCALE: 1000 authors -> BAD = 1001 queries (DB tabaah) | GOOD = 1 query. Yehi silent perf-killer.
```

### 6. ★★ GEMS / gotchas
```
1. N+1 ki JAD = LAZY loading + loop me child access. (EAGER bhi fix nahi -> wo har jagah load karega, ulta bura.)
2. FIX = JOIN FETCH (query me) ya @EntityGraph (annotation) -> ek query me parent+child.
3. DETECT = SQL logs (show-sql=true) me "same select repeat" dikhe -> N+1 hai. (interview: "kaise pakadoge?" -> logs.)
4. distinct JOIN FETCH me zaroori -> warna join-multiplication se parent duplicate.
5. Connect: ye query-COUNT ka masla hai (kitni queries), INDEXING = per-query SPEED ka. Dono = DB performance.
```

### 7. Dobara kaise chalaye
```
usercrud/ me: mvn spring-boot:run   (MySQL local chalu)
curl http://localhost:8080/n1/bad    -> logs me 1+N queries
curl http://localhost:8080/n1/good   -> logs me 1 query
```

---

## ═══ 8. COUSIN — "Update entities one by one" (N+1 ka WRITE-side, 29-Aug bonus) ═══

> N+1 = READ-side (kai SELECT). Ye cousin = WRITE-side (kai UPDATE). Dono ki jad SAME:
> list pe loop -> Hibernate har entity pe alag SQL. Interview me "N+1 ke alawa aur kya log me dikhega?" -> ye.

### Problem — list update karo, har entity pe alag UPDATE
```java
List<Product> products = repo.findAll();          // 1 SELECT
for (Product p : products)
    p.setPrice(p.getPrice() * 0.9);               // dirty-checking -> har entity pe alag UPDATE
```
Logs me:
```
select ... from Product                                 <- 1 query
update Product set name=?, price=?, version=? where id=? and version=?   <- entity 1
update Product set name=?, price=?, version=? where id=? and version=?   <- entity 2
update Product set name=?, price=?, version=? where id=? and version=?   <- entity 3
...
= 1 SELECT + N UPDATE  (same UPDATE template repeat = red flag)
```

### Fix — ek JPQL BULK UPDATE (DB pe ek hi statement)
```java
@Modifying @Transactional
@Query("UPDATE Product p SET p.price = p.price * 0.9")
void discountAll();
// ya: em.createQuery("UPDATE Product p SET p.price = p.price*0.9").executeUpdate();
```
```
N alag UPDATE -> 1 UPDATE. DB khud saari rows badal deta.
```

### ★ Gotcha (bulk-update ka trap)
```
Bulk JPQL/native UPDATE 1st-level cache ko BYPASS karta -> Hibernate ko pata nahi kaunsi entity badli.
 -> SAME session me us entity ko read + bulk-update MAT karo (stale cache milega).
 -> zaroorat ho to entity DETACH kar / session clear kar.
```

### Detection (same as N+1)
```
show-sql logs me "same UPDATE template baar-baar" -> ye issue.
Hibernate Statistics (hibernate.generate_statistics=true) -> JDBC-statement count expected se zyada?
   (par ye PRODUCTION me OFF -> khud slow karta.)
```

### Memory hook
```
N+1 (read)   = 1 SELECT + N SELECT   -> fix: JOIN FETCH / @EntityGraph
one-by-one (write) = 1 SELECT + N UPDATE -> fix: JPQL bulk @Modifying UPDATE
Dono ki jad = loop pe per-entity SQL. Dono log me "same query repeat" se pakdo.
```
