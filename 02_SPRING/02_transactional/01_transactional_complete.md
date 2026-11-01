# `@Transactional`

## Kahani: banking wala Arpan, Ravi ko ₹1000

Banking mock me cashier ne Arpan ke 500 kaate aur bijli chali gayi, jawab tha "mere 500 wapas de do".
`@Transactional` wahi "wapas de do" hai, code me.

```java
@Transactional
public void transfer(Long from, Long to, BigDecimal amt) {
    accountRepo.debit(from, amt);   // Arpan -1000
    accountRepo.credit(to, amt);    // Ravi +1000  <- yahan exception
}                                   // -> dono undo, Arpan ke 1000 wapas
```

**Maan lo Ravi doosre bank me hai (UPI):** do alag DB, ek `@Transactional` dono ko nahi pakad sakta.
SBI debit commit karta, HDFC credit fail -> job milaan karke 2-3 din me refund. Isliye SAGA + job.

## Tasveer: kaam proxy karta hai, method nahi

```
Controller --> [ Spring PROXY ] --> asli transfer()
                 1. BEGIN (setAutoCommit false)
                 2. method chalao
                 3. sab theek -> COMMIT
                    RuntimeException -> ROLLBACK
```

Andar wahi MySQL ka `START TRANSACTION ... COMMIT / ROLLBACK`, Spring bas likhne se bachata hai.

## 4 khaas baat

1. **Rollback sirf RuntimeException pe.** Checked exception (jaise `IOException`) aayi to data commit ho jaata hai.
   Isliye apni exception `RuntimeException` extend karo, ya `rollbackFor = Exception.class` lagao.
2. **Same class ke andar call (`this.save()`) pe kaam nahi karta.** Call proxy ke saamne se jaati hi nahi.
   Private method pe bhi nahi chalta. Hal: method alag service me nikaal do.
3. **Propagation:** default `REQUIRED` = bahar wali transaction me jud jaao. `REQUIRES_NEW` audit log ke liye:
   order fail ho jaaye tab bhi log bacha rahe.
4. **`readOnly = true`** SELECT wale methods pe. Hibernate badlaav check karna chhod deta hai, kaam tez.

## Prod wala trap

```java
@Transactional
public void placeOrder(Order o) {
    orderRepo.save(o);
    paymentClient.charge(o);   // 3 sec ki HTTP call, transaction ke ANDAR
}
```

Jab tak HTTP call chalti hai, DB connection pakda rehta hai. 50 aisi request aayi to HikariCP ka pool khaali aur
poori app atak jaati hai (31-Oct order refund PR wala). Hal: transaction me sirf DB ka kaam, bahar ki call bahar.

## Tera project

- **usercrud:** write methods pe `@Transactional`, read pe `readOnly = true`. `demoRollback` endpoint se dekha ki
  RuntimeException pe user DB me nahi bacha.
- **mini_payment:** order aur payment alag service, alag DB. Wahan ek `@Transactional` dono ko cover nahi kar sakta,
  isliye SAGA + idempotency key.

## Interview line

```
@Transactional ek DB ka all-or-nothing hai: Spring proxy BEGIN karta, sab theek to COMMIT,
RuntimeException pe ROLLBACK. Do dhyaan: checked exception pe rollback nahi hota, aur
same class ke andar this.method() pe proxy bypass ho jaata hai. Alag service ya alag DB ho
to @Transactional kaafi nahi, wahan SAGA + idempotency key.
```
