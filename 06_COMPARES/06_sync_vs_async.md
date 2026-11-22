# Sync vs Async — ruk ke jawab lo ya kaam daal ke aage badho

---

## Koi poochhe to aise bolo

**Sync** me bhejne wala jawab aane tak ruka rehta hai: Order Service ne Payment ko REST call kiya, to jab tak Payment jawab
nahi deta, Order wahin ruka hai. Seedha hai aur jawab turant milta hai; par samne wala dheema ho ya gir jaaye to bhejne wala
bhi atak jaata hai. **Async** me bhejne wala kaam Kafka / queue me daal ke aage badh jaata hai; kaam baad me hota hai, khabar
baad me aati hai. Bheed jhel lete hain, aur samne wala gira ho tab bhi kaam line me pada rehta hai. Keemat: jawab turant nahi,
error pakadna mushkil, kram ki guarantee nahi. Asal me dono saath chalte hain: jahan user ruka hai aur jawab chahiye wahan sync
(login, payment ka haan / naa, order ID), peeche ka kaam async (email, SMS, ginti, warehouse ko khabar).

---

## Kahani: phone vs WhatsApp

```
Phone (sync):     call kiya, jab tak wo uthaye aur bole, tu line pe ruka; wo bijli me hai to tu bhi atka
WhatsApp (async): message daala, apna kaam karne laga; reply jab aaye tab aaye, wo offline ho to message pada rehta
```

---

## Tere design aur project se

- **Notification kadam 1:** order sync me banta hai, user ko turant "order ho gaya". SMS / email async, Kafka ke raaste;
  order ke saath outbox (hamare DB ki table) taaki khabar na khoye.
- **BMS:** seat aur payment ka haan / naa sync; ticket banana async (outbox + relay).
- **mini_payment_ms:** Order -> Payment Feign se sync, isliye circuit breaker (Payment dheema ho to Order na atke).
  Baaki event Kafka se async.

---

## Dikkat se hal

Checkout me email bhi sync rakh diya. Email server 10 second atka, to user 10 second "loading" dekhta raha, aur bheed me
saare Tomcat thread isi intezaar me bhar gaye. Hal: email ko Kafka me daal do (ya `@Async`); user ko turant jawab, email peeche.

---

## Trap

`@Async` wala method usi class ke andar se `this.sendEmail()` bulaya, to wo async chala hi nahi, chupchaap ruk ke chala.
Wajah wahi jo Spring 13 cache ke `update()` me dekhi: Spring ka proxy bahar se aane wali call pakadta hai, class ke andar ki
call proxy se guzarti hi nahi. Hal: method alag bean me rakho.
