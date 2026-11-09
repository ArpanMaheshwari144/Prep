# N+1 Problem

## Scene: tera usercrud, `/n1/bad`

DB me 5 author hain. Code ye hai:

```java
List<Author> authors = authorRepo.findAll();   // 1 query: saare author
for (Author a : authors)
    count += a.getBooks().size();              // har author pe 1 aur query
```

21-Aug ko logs me ye dikha tha:

```
/n1/bad   ->  select * from author             1
              select book where author_id=1    +1
              ...  (5 author)                  +5    = 6 query
/n1/good  ->  select a.*, b.* from author a
              left join book b ...                   = 1 query
```

## Kyun hua

`@OneToMany` default me LAZY hai, yaani "kitaabein tabhi lao jab maangi jaayein". Loop me har author ki kitaabein maangi gayi, to har baar
ek nayi query chali. 1 (author) + N (har author ki kitaab) = **N+1**. 5 author pe 6 query, 10,000 author pe 10,001.

## Hal

Pehli hi query me bolo "author aur unki kitaabein dono ek saath lao":

- `LEFT JOIN FETCH` (tera `findAllWithBooks()`). Sirf `JOIN` likha to jis author ki koi kitaab nahi, wo list se gaayab ho jaata hai.
- Ya `@EntityGraph(attributePaths = "books")`, jo yahi kaam annotation se karta hai.
- Ya `@BatchSize(10)`, jo ek query me 10 author ki kitaabein laata hai, to 100 author pe 1 + 10 query.

EAGER kar dena hal nahi hai, kyunki tab har jagah kitaabein aayengi, chahe zaroorat ho ya nahi.

Pakadna kaise: `show-sql=true` lagao. Logs me ek hi select baar-baar dikhe, sirf id badle, to wahi N+1 hai.

## Likhne wala bhai

Loop me `p.setPrice(...)` kiya to 100 product pe 100 alag UPDATE jaate hain. Hal hai ek bulk UPDATE, jisme DB khud 100 row badal deta hai:

```java
@Modifying(clearAutomatically = true)
@Query("UPDATE Product p SET p.price = p.price * 0.9")
void discountAll();                          // 1 UPDATE
```

`clearAutomatically = true` kyun: Hibernate jo row ek baar laata hai, use apni almaari (1st-level cache) me rakh leta hai. Bulk UPDATE seedha
DB badalta hai, almaari nahi, to usi transaction me dobara `findById` kiya to purana daam (100) milega, naya (90) nahi. Redis me purani key
padi rehne wali wahi dikkat; wahan key DELETE karte hain, yahan almaari khaali.

## Prod wala trap

```java
@ManyToOne        // default EAGER
Author author;
```

`bookRepo.findAll()` karte hi har alag author ke liye ek alag query chalti hai. Ye ulta N+1 hai, aur ye tab bhi hota hai jab tune author
maanga hi nahi. 100 book aur 5 author hon to 1 + 5 query, kyunki Hibernate ek hi author dobara aaye to almaari se de deta hai. Hal:
`@ManyToOne(fetch = FetchType.LAZY)` lagao, aur jahan author sach me chahiye wahan JOIN FETCH karo.
