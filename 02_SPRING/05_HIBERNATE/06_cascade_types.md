# 🌊 Cascade Types — Parent → Child Auto Effect

---

## 1️⃣ Concept (Family Analogy)

```
Imagine parent kid:
   Parent ne kuch decide kiya (move to new city)
   Child ko bhi automatically move karna padega
   
   = CASCADE — parent ka action child pe auto effect
```

---

## 2️⃣ Problem Without Cascade

```java
Author a = new Author("Arpan");
Book b1 = new Book("Java");
b1.setAuthor(a);

authorRepo.save(a);          // ✅ author save
// b1 SAVE NAHI hua!         // ❌ book lost

bookRepo.save(b1);           // manually karna padega
```

```
= Verbose
= Easy to forget
= Bug-prone
```

---

## 3️⃣ With CASCADE

```java
@Entity
class Author {
    @OneToMany(cascade = CascadeType.PERSIST)
    List<Book> books;
}

// Now:
Author a = new Author("Arpan");
a.getBooks().add(new Book("Java"));
a.getBooks().add(new Book("Spring"));

authorRepo.save(a);
// = Author saved
// = Books AUTO saved (cascade PERSIST)
```

```
CASCADE = "Parent action child pe automatically apply"
```

---

## 4️⃣ 6 Cascade Types

```
┌──────────────┬──────────────────────────────────────┐
│  Type        │  Behavior                             │
├──────────────┼──────────────────────────────────────┤
│ PERSIST      │ save parent → save children          │
│ MERGE        │ merge parent → merge children        │
│ REMOVE       │ delete parent → delete children      │
│ REFRESH      │ reload parent → reload children      │
│ DETACH       │ detach parent → detach children      │
│ ALL          │ All 5 above                          │
└──────────────┴──────────────────────────────────────┘
```

---

## 5️⃣ Visual Flow — PERSIST Example

```
   authorRepo.save(author)
        │
        ▼
   Hibernate detects:
      "cascade = PERSIST on books"
        │
        ▼
   For each book in author.books:
      → INSERT INTO books (...)
        │
        ▼
   INSERT INTO authors (...)
        │
        ▼
   Commit — all saved together
```

---

## 6️⃣ REMOVE Cascade (Dangerous!)

```java
@OneToMany(cascade = CascadeType.REMOVE)
List<Book> books;
```

```
authorRepo.delete(author)
        │
        ▼
   "cascade REMOVE on books"
        │
        ▼
   DELETE FROM books WHERE author_id = ?
   DELETE FROM authors WHERE id = ?
   
   = Author + all books DELETED
```

```
⚠️ DANGER:
   Accidentally delete parent → kids gone
   Books shared with someone else? — DELETED too
   
   Use carefully
```

---

## 7️⃣ Cascade.ALL — Common but Risky

```java
@OneToMany(cascade = CascadeType.ALL)
List<Book> books;
```

```
Behaviour:
   save author    → save books
   delete author  → delete books
   merge author   → merge books
   detach author  → detach books
   refresh author → refresh books
   
   = ALL operations cascade
```

```
✅ Convenient
❌ Dangerous (especially REMOVE)
   Industry: Be SPECIFIC, not ALL
```

---

## 8️⃣ orphanRemoval (Extra Cascade)

```java
@OneToMany(cascade = CascadeType.ALL, orphanRemoval = true)
List<Book> books;
```

```
orphanRemoval = "orphaned children DELETE kar do"

Example:
   author.getBooks().remove(book1);
   // book1 has no parent now (orphaned)
   // orphanRemoval = true → AUTO DELETE from DB
```

```
WITHOUT orphanRemoval:
   List se remove ho jata Java memory mein
   DB mein book row still exists
   = Inconsistency

WITH orphanRemoval:
   List se remove → DB se bhi DELETE
   = Clean
```

---

## 9️⃣ Common Use Cases

```
parent OWNS children completely:
   ✅ Use CascadeType.ALL + orphanRemoval
   Examples:
      • Order → OrderItems (item belongs only to order)
      • Post → Comments
      • User → UserSettings

children SHARED (independent):
   ❌ NO cascade
   Examples:
      • Author → Books (book might be referenced elsewhere)
      • Category → Products
```

---

## 🔟 Real Example (Order System)

```java
@Entity
class Order {
    @OneToMany(
        mappedBy = "order",
        cascade = CascadeType.ALL,
        orphanRemoval = true
    )
    List<OrderItem> items;
}

@Entity
class OrderItem {
    @ManyToOne
    Order order;
    
    String product;
    int quantity;
}
```

```
Behaviour:
   orderRepo.save(order)        → items also saved
   orderRepo.delete(order)      → items also deleted
   order.getItems().remove(it)  → item auto-deleted (orphan)
   
   = Order owns items completely
   = Lifecycle bound together
```

---

## 1️⃣1️⃣ Decision Tree

```
   Child entity ki existence parent pe DEPEND?
        │
   ┌────┴────┐
   YES        NO
   │          │
   ▼          ▼
   Use cascade   Don't cascade
   (children     (manually
    bound to     manage children)
    parent)
```

---

## 🎤 Interview Power Phrase

```
"Cascade types control parent-child operation propagation.
 PERSIST/MERGE/REMOVE/REFRESH/DETACH/ALL — common ones.

 Use cascade when child entity lifecycle bound to parent 
 (e.g., Order-OrderItem). Add orphanRemoval=true to delete 
 children automatically when removed from collection.

 Don't cascade ALL blindly — REMOVE especially dangerous 
 for shared entities."
```

---

## 🎯 Memory Hook

```
Cascade = parent action → child auto affect

Types:
   PERSIST → save together
   REMOVE  → delete together (dangerous)
   ALL     → everything together (risky)

orphanRemoval = list se remove → DB se delete

Use when: child OWNED by parent
Avoid when: child shared (Books, Categories)
```
