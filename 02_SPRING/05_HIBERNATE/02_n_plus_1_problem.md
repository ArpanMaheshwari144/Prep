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
