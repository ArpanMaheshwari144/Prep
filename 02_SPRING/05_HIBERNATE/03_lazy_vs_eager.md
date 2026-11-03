# LAZY vs EAGER Fetch

## Kahani: PA ki parchi

Arpan PA se lekhak #1 ki file maangta hai.
- **LAZY:** PA sirf lekhak ki file laata hai, aur kitaabon ki jagah ek parchi rakh deta hai jis pe likha hai "maangoge to laaunga". Yahi parchi proxy hai.
- **EAGER:** PA saath me sab kuch utha laata hai: kitaabein, publisher, aur publisher ka desh bhi. Chahiye ya nahi, ye wo nahi dekhta.

Trap ye hai: Arpan ne file le li, aur PA ghar chala gaya (session band ho gaya). Ab Arpan parchi dikha ke kitaabein maangta hai, par laane wala koi nahi hai.
Isi ko `LazyInitializationException` ("no session") kehte hain.

## Tasveer

```
LAZY                               EAGER
Author #1                          Author #1
  name: Arpan                        name: Arpan
  books: [parchi]  <- proxy          books: [B1, B2, B3]
  getBooks() -> TAB DB jaata         publisher: {...}  <- bina maange
```

LAZY me books ki jagah parchi (proxy) hai, aur jab `getBooks()` bulate ho tabhi DB jaata hai. EAGER me books pehle se bhari hain, aur publisher bhi bina maange aa gaya.

## 4 khaas baat

1. Default ye hai: jo rishta **"to MANY"** hai wo LAZY hota hai (`@OneToMany`, `@ManyToMany`), aur jo **"to ONE"** hai wo EAGER hota hai (`@ManyToOne`, `@OneToOne`).
2. Industry ka niyam hai ki sab kuch LAZY rakho, `@ManyToOne(fetch = LAZY)` bhi. Phir jahan data chahiye, wahan JOIN FETCH ya `@EntityGraph` lagao.
3. LazyInit ke teen hal hain. Pehla: PA ko rok ke rakho (`@Transactional`) aur parchi andar hi khol lo, jaise `books.size()` bula ke. Sirf list return kar dena kaafi nahi
   hai, kyunki parchi tab bhi band rehti hai. Doosra: JOIN FETCH. Teesra: DTO, jo REST ke liye sabse achha hai.
4. **OSIV** (Open-Session-In-View) Boot me default ON hota hai. Iska matlab PA poori HTTP request tak ruka rehta hai, isliye controller me LazyInit
   aata hi nahi. Par iska matlab ye bhi hai ki PA ke haath me record room ki chaabi (DB connection) bhi tab tak rehti hai.

## Prod wala trap

```properties
spring.jpa.open-in-view=true   # default; DB connection poori request tak pakda
```

Ye wahi HikariCP wali dikkat hai jo @Transactional me HTTP call ke saath thi. Client slow hai ya JSON bada hai, to connection utni der pakda rehta hai. Aisi bahut requests aayi to
pool khaali ho jaata hai aur app atak jaati hai. Isliye prod me ise `false` rakho, aur data service layer me hi explicit fetch karo.

## Tera project

usercrud ka `LazyDemoController` (9-Sep). `open-in-view=false` karke `/lazy/fail` chalaya to 500 aaya ("no session").
`/lazy/fix-txn` pe `books = 3` aaya aur 2 query chali. `/lazy/fix-fetch` me JOIN FETCH tha, to sirf 1 query chali.

## Interview line

```
LAZY access pe load karta, EAGER parent ke saath. Default: ToMany LAZY, ToOne EAGER; main sab
LAZY rakhta hoon aur jahan chahiye JOIN FETCH ya @EntityGraph. LAZY ka risk session band hone
pe LazyInitializationException; OSIV use chhupa deta par connection poori request pakde rehta,
isliye prod me OSIV off + service layer me explicit fetch.
```
