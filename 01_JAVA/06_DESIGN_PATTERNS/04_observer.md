# Observer Pattern — Pub-Sub (Event Notifications)

**Kahani:** Arpan ne Ravi ko 500 bheje. Ab Ravi ko email jaana hai, SMS jaana hai, aur audit me line likhni hai. Agar `transfer()` khud sabko call kare, to kal push notification jodna hoga aur transfer ka code phir se kholna padega. Observer me transfer bas ek baar chilla deta hai, "transfer hua!" (publish), aur jo-jo sun raha hai (subscribers) wo apna kaam kar leta hai. Bilkul YouTube jaisa: creator video daalta hai, kisko bhejna hai ye YouTube jaane.

```
TransactionService --publish(TransferEvent)--> EventPublisher
                                                 | list: [Email, SMS, Audit]
                                                 v  loop: har ek ka onEvent(e)
                                    EmailListener  SMSListener  AuditListener
```

**Khaas baat:**
1. Teen hisse hain: **Event** (kya hua), **Listener interface** (`onEvent`), aur **Publisher** (list + loop). Publisher sirf interface jaanta hai, concrete class nahi.
2. Naya listener jodna = sirf ek `subscribe()`. Publisher ka code nahi badalta, yahi OCP hai.
3. Ek listener crash hua to baaki na rukein, isliye har `onEvent` ko try-catch me lapeto. Aur jab publish ka loop chal raha ho tab koi unsubscribe kare, to ArrayList `ConcurrentModificationException` deta hai. Iska ilaaj `CopyOnWriteArrayList` hai.

**Prod wala trap:**
```java
@Transactional placeOrder() { save(order); publisher.publishEvent(new OrderPlaced(order)); }
@EventListener onOrderPlaced(e) { emailService.send(...); }   // commit se PEHLE chalta hai
// DB commit fail -> order bana hi nahi, par email chala gaya
// fix: @TransactionalEventListener(phase = AFTER_COMMIT)
```
Ye wahi dikkat hai jo banking kadam 6 me thi: DB me likhna aur bahar bhejna ek saath pakka nahi hota.

**Tera project:** SimpleBankSystem me [`observer/EventPublisher.java`](../../07_PROJECTS/SimpleBankSystem/src/com/arpan/bank/observer/EventPublisher.java). `AccountService.transfer()` event publish karta hai, aur `Main.java:101` pe `EmailListener` subscribe hota hai.

```
Interview: "Observer = one-to-many. Publisher event publish karta, subscribers apne aap notify hote,
publisher ko sirf interface pata. Mere SimpleBankSystem me EventPublisher transfer pe event fire karta,
EmailListener sunta; SMS / audit jodne me publisher nahi badalta. Spring me ApplicationEventPublisher +
@EventListener, aur DB ke saath ho to @TransactionalEventListener(AFTER_COMMIT). Bade scale pe yahi Kafka."
```
