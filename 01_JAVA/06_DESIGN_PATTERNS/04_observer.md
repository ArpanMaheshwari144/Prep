# Observer Pattern — Pub-Sub (Event Notifications)

**Koi poochhe "Observer kya hai" to aise bolo:**
```
Ek ne chillaya "transfer hua", jo-jo sun raha hai wo apna kaam kare: email, SMS, audit.
Transfer ko pata nahi kaun sun raha; naya listener jodo to transfer ka code nahi khulta.
YouTube jaisa: creator video daalta, bhejna YouTube jaane. Bade scale pe yahi Kafka.
```

## Kahani

Arpan ne Ravi ko 500 bheje. Ab Ravi ko email jaana hai, SMS jaana hai, aur audit me line likhni hai. Agar `transfer()` khud sabko call
kare, to kal push notification jodna hoga aur transfer ka code phir se kholna padega. Observer me transfer bas ek baar chilla deta hai,
"transfer hua!" (publish), aur jo-jo sun raha hai (subscribers) wo apna kaam kar leta hai.

```
TransactionService --publish(TransferEvent)--> EventPublisher
                                                 | list: [Email, SMS, Audit]
                                                 v  loop: har ek ka onEvent(e)
                                    EmailListener  SMSListener  AuditListener
```

Teen hisse hain: **Event** (kya hua), **Listener interface** (`onEvent`), aur **Publisher** (list + loop). Publisher sirf interface
jaanta hai, concrete class nahi. Naya listener jodna = sirf ek `subscribe()`, publisher ka code nahi badalta, yahi OCP hai.

Usi waqt SMS gateway down tha. Chala ke dekha (teen listener: Email, SMS, Audit):

```
--- bina try-catch ---
  Email: Arpan -> Ravi 500
  transfer() tak exception: SMS gateway down
--- har onEvent try-catch me ---
  Email: Arpan -> Ravi 500
  (ek listener gira: SMS gateway down, baaki chalte rahe)
  Audit: Arpan -> Ravi 500
```

**Dikkat:** SMS ka listener gira to Audit tak baat pahunchi hi nahi, aur paise ka audit hi nahi likha gaya. Upar se exception `transfer()`
tak pahunch gaya. Ye wahi S wali dikkat hai: SMS ki galti paisa wale kaam ko gira rahi hai.

## Fix

Publisher har `onEvent` ko alag try-catch me lapete, taaki ek listener gire to baaki chalte rahein. Aur jab publish ka loop chal raha ho
tab koi unsubscribe kare, to ArrayList `ConcurrentModificationException` deta hai, isliye list `CopyOnWriteArrayList` rakhte hain.

## Trap

```java
@Transactional placeOrder() { save(order); publisher.publishEvent(new OrderPlaced(order)); }
@EventListener onOrderPlaced(e) { emailService.send(...); }   // commit se PEHLE chalta hai
// DB commit fail -> order bana hi nahi, par email chala gaya
// fix: @TransactionalEventListener(phase = AFTER_COMMIT)
```

Spring me `@EventListener` DB commit se pehle hi chal jaata hai, to commit fail hua to bhi email chala gaya. Ilaaj
`@TransactionalEventListener(phase = AFTER_COMMIT)`. Ye wahi soch hai jo S me SMS ko transaction ke bahar rakhne wali thi, aur banking
kadam 6 wali: DB me likhna aur bahar bhejna ek saath pakka nahi hota.

## Tera project

SimpleBankSystem me [`observer/EventPublisher.java`](../../07_PROJECTS/SimpleBankSystem/src/com/arpan/bank/observer/EventPublisher.java).
`AccountService.transfer()` event publish karta hai, aur `Main.java:101` pe `EmailListener` subscribe hota hai.
