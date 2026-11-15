# `@Transactional`

## Koi poochhe "@Transactional kya hai" to aise bolo

```
Ek method ke andar ke saare DB kaam ek packet: ya sab hoga, ya kuch nahi.
Spring method ke aage ek proxy bithata hai: pehle transaction shuru, phir method,
sab theek to commit, RuntimeException aayi to rollback.
Jaise banking me 1000 kate aur credit pe bijli gayi, to paise wapas.
```

## Kahani: banking wala Arpan, Ravi ko ₹1000

Banking mock me cashier ne Arpan ke 500 kaat liye aur tabhi bijli chali gayi. Tab jawab tha "mere 500 wapas de do".
`@Transactional` code me wahi "wapas de do" hai.

```java
@Transactional
public void transfer(Long from, Long to, BigDecimal amt) {
    accountRepo.debit(from, amt);   // Arpan -1000
    accountRepo.credit(to, amt);    // Ravi +1000  <- yahan exception
}                                   // -> dono undo, Arpan ke 1000 wapas
```

Debit ho gaya aur credit pe exception aa gayi, to Spring dono kaam undo kar deta hai aur Arpan ke 1000 wapas aa jaate hain.

Chala ke dekha (Spring Boot 4 + H2, Arpan 5000, Ravi 0, debit ke baad exception):

```
bina @Transactional -> Arpan=4000, Ravi=0     <- 1000 hawa me gayab
@Transactional      -> Arpan=5000, Ravi=0     <- proxy ne rollback kiya
checked exception   -> Arpan=4000, Ravi=0     <- @Transactional laga tha, phir bhi gaya
this. se call       -> Arpan=4000, Ravi=0     <- proxy ke bagal se call, transaction bana hi nahi
```

Neeche ke do kyun gaye, wo agle hisse me.

## Andar kya hota hai: kaam proxy karta hai, method nahi

```
Controller --> [ Spring PROXY ] --> asli transfer()
                 1. BEGIN (setAutoCommit false)
                 2. method chalao
                 3. sab theek -> COMMIT
                    RuntimeException -> ROLLBACK
```

Controller seedha `transfer()` ko nahi bulata, beech me Spring ka proxy baitha hai. Proxy pehle BEGIN karta hai, phir method chalata hai, aur sab theek raha to COMMIT, RuntimeException aayi to ROLLBACK.
Andar wahi MySQL ka `START TRANSACTION ... COMMIT / ROLLBACK` chalta hai. Spring bas tujhe ye baar-baar likhne se bachata hai.

## Chhoti dikkatein aur unke hal

- **Checked exception (jaise `IOException`) pe rollback nahi hota,** data commit ho jaata hai. Hal: apni exception `RuntimeException` se banao, ya `rollbackFor = Exception.class` laga do.
- **`this.save()` (same class ke andar se call) pe kaam nahi karta,** kyunki wo call proxy ke saamne se jaati hi nahi. Private method pe bhi nahi chalta. Hal: method alag service me nikaal do.
- **Propagation:** default `REQUIRED` hai, matlab bahar pehle se transaction chal rahi ho to usi me jud jaao. Audit log ke liye `REQUIRES_NEW` lagate hain, taaki order fail ho tab bhi log bacha rahe.
- **`readOnly = true`** SELECT wale methods pe lagao. Hibernate badlaav check karna chhod deta hai, aur kaam tez ho jaata hai.

**Ravi doosre bank me ho (UPI):** wahan do alag DB hain, aur ek `@Transactional` dono ko nahi pakad sakta. SBI apna debit commit kar deta hai, aur agar HDFC me credit fail ho gaya, to ek job baad me dono ka milaan karti hai aur refund deti hai. Isliye wahan SAGA aur milaan wali job lagti hai. HLD banking ke deadlock wale scene se bhi yahi nikla tha: atkan sirf ek hi bank ke DB ke andar hota hai.

## Prod wala trap

```java
@Transactional
public void placeOrder(Order o) {
    orderRepo.save(o);
    paymentClient.charge(o);   // 3 sec ki HTTP call, transaction ke ANDAR
}
```

Jab tak HTTP call chalti hai, tab tak DB connection pakda rehta hai. Aisi 50 request ek saath aayi to HikariCP ka pool khaali ho jaata hai aur
poori app atak jaati hai (ye 31-Oct ke order refund PR wala kissa hai). Hal: transaction ke andar sirf DB ka kaam rakho, aur bahar ki call transaction ke bahar karo.

## Tera project

- **usercrud:** write methods pe `@Transactional` hai, aur read methods pe `readOnly = true`. `demoRollback` endpoint se tune dekha ki
  RuntimeException aane pe user DB me nahi bacha. Yahi iska chalta hua saboot hai.
- **mini_payment:** yahan order aur payment alag service hain, aur dono ka DB bhi alag hai. Isliye ek `@Transactional` dono ko cover nahi kar sakta,
  aur wahan SAGA aur idempotency key lagayi hai.
