# Relationship Mappings — @OneToMany / @ManyToOne / @ManyToMany

---

## 1 4 Relationship Types

```
ek-to-ek         (@OneToOne)
   User ↔ Profile (1 user, 1 profile)

ek-to-many       (@OneToMany)
   Author → many Books (1 author, many books)

many-to-ek       (@ManyToOne)
   many Books → Author (many books, 1 author)

many-to-many     (@ManyToMany)
   Student ↔ Course (many students, many courses)
```

---

## 2 Bidirectional Relationship — Author + Book

```java
@Entity
class Author {
    @Id Long id;
    String name;

    @OneToMany(mappedBy = "author")
    List<Book> books;       // ← "from author's perspective"
}

@Entity
class Book {
    @Id Long id;
    String title;

    @ManyToOne
    @JoinColumn(name = "author_id")
    Author author;          // ← "from book's perspective"
}
```

```
Same relationship — 2 angles:
   Author → many books   (@OneToMany)
   Book → 1 author       (@ManyToOne)
```

---

## 3 Visual — DB Table Structure

```
   authors table:
   ┌────┬────────┐
   │ id │ name   │
   ├────┼────────┤
   │ 1  │ Arpan  │
   │ 2  │ Suresh │
   └────┴────────┘

   books table:
   ┌────┬─────────┬───────────┐
   │ id │ title   │ author_id │  ← Foreign Key
   ├────┼─────────┼───────────┤
   │ 1  │ Java    │     1     │  ← belongs to Arpan
   │ 2  │ Spring  │     1     │
   │ 3  │ Python  │     2     │
   └────┴─────────┴───────────┘

   FK column = author_id in books table
```

---

## 4 Owning Side (Critical)

```
Owning side = WHO holds the foreign key
            = WHO controls the relationship in DB

In Author + Book:
   books table has author_id column
   = Book is OWNING side
   = Author is INVERSE side
```

---

## 5 `mappedBy` — Inverse Side Marker

```java
@OneToMany(mappedBy = "author")
List<Book> books;
```

```
mappedBy = "author"
   → "I'm INVERSE side"
   → "Book class ka 'author' field me FK exists"
   → Don't create separate join table
```

---

## 6 `@JoinColumn` — Owning Side

```java
@ManyToOne
@JoinColumn(name = "author_id")
Author author;
```

```
@JoinColumn(name = "author_id")
   → "FK column ka naam — author_id"
   → "Book table mein yeh column rakhega"
   → I'm OWNING side
```

---

## 7 CRITICAL PITFALL #1 — Forget `mappedBy`

```java
// WRONG
@Entity
class Author {
    @OneToMany
    List<Book> books;   // ← NO mappedBy
}

@Entity
class Book {
    @ManyToOne
    Author author;
}
```

```
Without mappedBy:
   Hibernate confused — "kaun owns this?"
   Creates EXTRA join table: author_books
   = 3 tables now: authors, books, author_books
   = Wasted table, slower queries
```

```
FIX:
   @OneToMany(mappedBy = "author")    ← tell Hibernate inverse side
   List<Book> books;
```

---

## 8 CRITICAL PITFALL #2 — Setting Only One Side

```java
Author a = new Author("Arpan");
Book b = new Book("Java");

a.getBooks().add(b);
// b.setAuthor(a) NAHI kiya!

authorRepo.save(a);
// books mein book add hua Java memory mein
// But DB mein author_id NULL (Book owns FK)
// = Inconsistency!
```

```java
// FIX — Helper method:
class Author {
    public void addBook(Book b) {
        books.add(b);
        b.setAuthor(this);    // BOTH sides set
    }
}

a.addBook(b);   // both sides synced
```

---

## 9 @ManyToMany — Many to Many

```java
@Entity
class Student {
    @Id Long id;
    String name;

    @ManyToMany
    @JoinTable(
        name = "student_course",
        joinColumns = @JoinColumn(name = "student_id"),
        inverseJoinColumns = @JoinColumn(name = "course_id")
    )
    List<Course> courses;
}

@Entity
class Course {
    @Id Long id;
    String name;

    @ManyToMany(mappedBy = "courses")
    List<Student> students;
}
```

```
@JoinTable = "extra junction table banao"

DB Structure:
   students table:
      id, name

   courses table:
      id, name

   student_course (JOIN table):
      student_id, course_id   ← combination
```

---

## Visual — ManyToMany Tables

```
   students            courses          student_course (join)
   ┌──┬─────┐         ┌──┬──────┐      ┌────────────┬───────────┐
   │ 1│Arpan│         │ 1│Java  │      │ student_id │ course_id │
   │ 2│Sure │         │ 2│Spring│      ├────────────┼───────────┤
   │ 3│Muke │         │ 3│Python│      │    1       │    1       │
   └──┴─────┘         └──┴──────┘      │    1       │    2       │
                                        │    2       │    1       │
                                        │    3       │    3       │
                                        └────────────┴───────────┘

   Arpan  → Java + Spring
   Suresh → Java
   Mukesh → Python
```

---

## 11 PITFALL #3 — @ManyToMany Performance

```
@ManyToMany with collections:
   N+1 queries common
   Hard to add extra fields to junction table
      (e.g., enrolledDate, grade)
```

```
INDUSTRY PREFERENCE:
   Replace @ManyToMany with 2 × @ManyToOne to junction entity:

   Student → @OneToMany → Enrollment ← @ManyToOne ← Course

   Enrollment table has:
      student_id, course_id, enrolledDate, grade

   = Full control over join, can add fields
```

---

## 12 Pitfalls Summary

```
┌──────────────────────────────┬──────────────────────────────┐
│  Pitfall                     │  Fix                          │
├──────────────────────────────┼──────────────────────────────┤
│ Missing mappedBy             │ Add mappedBy on inverse side │
│ Only one side updated        │ Helper method (sync both)    │
│ @ManyToMany + extra fields   │ Use junction entity instead  │
│ Bidirectional toString loop  │ Exclude from toString        │
│ Bidirectional equals loop    │ Use id only in equals        │
└──────────────────────────────┴──────────────────────────────┘
```

---

## Interview Power Phrase

```
"4 JPA mappings — @OneToOne, @OneToMany, @ManyToOne, @ManyToMany.
 Critical concept: owning side has FK column, inverse side uses mappedBy.

 Common pitfalls:
   • Forgetting mappedBy = extra join table created
   • Setting only one side = data inconsistency (use helper methods)
   • @ManyToMany at scale = replace with junction entity for control

 At work, I default to @ManyToOne with junction entity for many-to-many
 because we usually need extra fields like enrolledDate, audit timestamps."
```

---

## Memory Hook

```
Owning side  = has FK column
Inverse side = uses mappedBy

@OneToMany   = Author's books
@ManyToOne   = Book's author (owning)
@ManyToMany  = Student-Course (use junction entity in real prod)

ALWAYS set both sides in bidirectional
Use helper methods for sync
```
