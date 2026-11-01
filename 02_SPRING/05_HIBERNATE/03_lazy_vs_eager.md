# LAZY vs EAGER Fetch

## Kahani: PA ki parchi

Arpan PA se lekhak #1 ki file maangta hai.
- **LAZY:** PA lekhak ki file laata, kitaabon ki jagah ek parchi: "maangoge to laaunga" (proxy).
- **EAGER:** PA saath me sab utha laata: kitaabein, publisher, publisher ka desh. Chahiye ya nahi, nahi dekhta.

Trap: Arpan ne file li, PA ghar chala gaya (session band). Ab parchi dikha ke kitaabein maange to laane wala koi nahi
= `LazyInitializationException` ("no session").

## Tasveer

```
LAZY                               EAGER
Author #1                          Author #1
  name: Arpan                        name: Arpan
  books: [parchi]  <- proxy          books: [B1, B2, B3]
  getBooks() -> TAB DB jaata         publisher: {...}  <- bina maange
```

## 4 khaas baat

1. Default: **"to MANY" = LAZY** (`@OneToMany`, `@ManyToMany`), **"to ONE" = EAGER** (`@ManyToOne`, `@OneToOne`).
2. Industry niyam: sab LAZY (`@ManyToOne(fetch = LAZY)` bhi), jahan chahiye wahan JOIN FETCH / `@EntityGraph`.
3. LazyInit ke hal: PA rok ke rakho (`@Transactional`) aur parchi andar hi kholo (`books.size()`, sirf return kaafi
   nahi) · ya JOIN FETCH · ya DTO (REST ke liye sabse achha).
4. **OSIV** (Open-Session-In-View): Boot me default ON = PA poori HTTP request tak ruka, isliye controller me LazyInit
   aata hi nahi. Par PA ke haath me record room ki chaabi (DB connection) bhi tab tak.

## Prod wala trap

```properties
spring.jpa.open-in-view=true   # default; DB connection poori request tak pakda
```

Wahi HikariCP wali dikkat jo @Transactional me HTTP call ke saath thi: slow client / bada JSON -> connection pakda ->
pool khaali -> app atki. Prod me `false` + data service layer me explicit.

## Tera project

usercrud `LazyDemoController` (9-Sep): `open-in-view=false` karke `/lazy/fail` = 500 ("no session") ·
`/lazy/fix-txn` = `books = 3` (2 query) · `/lazy/fix-fetch` = JOIN FETCH, 1 query.

## Interview line

```
LAZY access pe load karta, EAGER parent ke saath. Default: ToMany LAZY, ToOne EAGER; main sab
LAZY rakhta hoon aur jahan chahiye JOIN FETCH ya @EntityGraph. LAZY ka risk session band hone
pe LazyInitializationException; OSIV use chhupa deta par connection poori request pakde rehta,
isliye prod me OSIV off + service layer me explicit fetch.
```
