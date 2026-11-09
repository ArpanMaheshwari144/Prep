# LAZY vs EAGER Fetch

## Scene: usercrud ka `LazyDemoController` (9-Sep)

`open-in-view=false` karke `/lazy/fail` chalaya. Controller ne `author.getBooks()` bulaya aur jawab aaya
**500: `LazyInitializationException: could not initialize proxy - no session`**.

Hua kya: N+1 me dekha tha ki LAZY ka matlab hai "kitaabein tabhi lao jab maangi jaayein". Author laate waqt Hibernate kitaabon ki jagah ek
**parchi (proxy)** rakh deta hai, jis pe likha hai "maangoge to laaunga". Repository se author aaya, transaction khatam hua aur session
(DB se baat karne wala) band ho gaya. Ab controller me parchi kholi to laane wala koi nahi bacha.

```
LAZY                                   EAGER
Author #1                              Author #1
  books: [parchi]  <- proxy              books: [B1, B2, B3]
  getBooks() -> TAB DB jaata             publisher: {...}  <- bina maange bhi
```

## Default (yaad ki cheez)

Jo rishta "to MANY" hai wo LAZY (`@OneToMany`, `@ManyToMany`), aur jo "to ONE" hai wo EAGER (`@ManyToOne`, `@OneToOne`). Niyam ye hai ki
sab LAZY rakho, aur jahan chahiye wahan khud mangao.

## Teen hal (tera output)

- `/lazy/fix-txn`: `@Transactional` se session khula rakha aur andar hi `books.size()` bula ke parchi khol li. Output `books = 3`, 2 query.
  Sirf list return karna kaafi nahi, kyunki parchi bina khule hi bahar chali jaati hai.
- `/lazy/fix-fetch`: JOIN FETCH se kitaabein pehli query me hi aa gayi. 1 query.
- DTO: service me hi zaroori field nikaal ke bhejo. REST ke liye ye sabse saaf hai.

## Prod wala trap

```properties
spring.jpa.open-in-view=true   # Boot me default
```

Ye ON ho to session poori HTTP request tak khula rehta hai, isliye controller me LazyInit aata hi nahi. Isi wajah se ye error tabhi dikha jab
ise `false` kiya. Par iska matlab ye bhi hai ki DB connection poori request tak pakda rehta hai. Client slow ho ya JSON bada ho to connection
utni der atka rehta hai, aur bheed me Hikari pool khaali, app atak jaati hai. Ye wahi dikkat hai jo `@Transactional` ke andar HTTP call ke
saath thi. Prod me ise `false` rakho aur data service me hi mangao.
