# N+1 Problem

## Kahani: wahi PA, 5 lekhak

Arpan PA se: "saare lekhak ki list lao, phir har lekhak ki kitaabein gino." PA ek baar record room jaata, 5 lekhak ki
list laata. Phir Arpan pehle lekhak ki kitaabein maangta, PA dobara record room. Doosre ke liye phir. 5 lekhak = 5 chakkar aur.

Kul = 1 + 5 = **N+1**. PA bewakoof nahi, use bola tha "kitaabein tabhi lana jab maangi jaayein" (LAZY).

Hal: pehli baar me bolo "lekhak aur unki kitaabein, dono ek saath le aana" = `JOIN FETCH`, 1 chakkar.

## Tasveer

```
BAD (LAZY + loop)                     GOOD (JOIN FETCH)
select * from author        1         select a.*, b.* from author a
select book where author=1  +1          LEFT JOIN book b ...       1
select book where author=2  +1
...                                   = 1 query
= 1 + N
```

## 4 khaas baat

1. Jad = `@OneToMany` default LAZY + loop me `getBooks()`. EAGER se theek nahi hota (tab har jagah kitaabein load).
2. Hal: `LEFT JOIN FETCH` (sirf JOIN = jis lekhak ki kitaab nahi wo gaayab) · `@EntityGraph(attributePaths = "books")` ·
   `@BatchSize(10)` (100 lekhak = 1 + 10 query).
3. Pakadna: `show-sql=true`, ek hi select baar-baar dikhe = N+1.
4. Likhne wala bhai: loop me `setPrice` = N alag UPDATE. Hal = ek `@Modifying` bulk UPDATE (neeche).

## Likhne wala bhai + almaari ka trap

```java
List<Product> products = repo.findAll();     // PA 100 file le aaya
for (Product p : products)
    p.setPrice(p.getPrice() * 0.9);          // commit pe 100 alag UPDATE
```

Hal: PA ko ek hukum, "record room me saari files pe khud daam kam kar do":

```java
@Modifying(clearAutomatically = true)
@Query("UPDATE Product p SET p.price = p.price * 0.9")
void discountAll();                          // 1 UPDATE, DB khud 100 row badalta
```

PA jo file laata hai, apni **almaari** (1st-level cache) me rakhta, usi transaction me dobara maangi to almaari se deta.
Bulk UPDATE seedha record room me badalta, almaari ki file purani rehti:

```java
Product p = repo.findById(1L).get();   // almaari me: daam 100
repo.discountAll();                    // record room me ab 90
repo.findById(1L).get().getPrice();    // almaari se -> 100 (purana!)
```

`clearAutomatically = true` = bulk UPDATE ke baad almaari khaali, agli baar record room se naya (90).
HLD jod: DB badla par Redis me purana = wahi dikkat. Wahan key DELETE, yahan almaari khaali.

## Prod wala trap

```java
@ManyToOne                 // default EAGER!
Author author;
// bookRepo.findAll() -> har book ka author alag query = ulta N+1
```

Hal: `@ManyToOne(fetch = FetchType.LAZY)` + jahan author chahiye wahan JOIN FETCH.

## Tera project

usercrud `/n1/bad` aur `/n1/good` (21-Aug): 5 author pe logs me bad = 6 query, good = 1 query.

## Interview line

```
N+1 tab hota hai jab LAZY relation ko loop me access karte: 1 query parent ki, phir har
parent pe ek child query. Main show-sql logs me same query repeat dekh ke pakadta hoon, aur
JOIN FETCH ya @EntityGraph se ek query me laata hoon; bahut data ho to @BatchSize.
```
