# S: Single Responsibility (ek class, ek kaam)

**Koi poochhe "Single Responsibility kya hai" to aise bolo:**
```
Ek class ke badalne ki ek hi wajah honi chahiye. Payment class me paisa, DB aur SMS teeno ho
to marketing ka SMS badlega aur paisa wali class khulegi. Isliye Controller, Service, Repository
alag rakhte hain: ek badla to baaki pe asar nahi.
```

## Kahani

Tere payment app me ek hi `PaymentService` teen kaam karti thi: paisa kaatna, DB me likhna, aur SMS bhejna. Marketing ne naya
SMS format maanga ("Hi ARPAN, Rs 500 kate"), to SMS wali line me `naam.toUpperCase()` aa gaya. Ek din kisi user ka naam null aaya.

```java
class PaymentServiceMota {             // galat: teen kaam
    void pay(String user, String naam, int amt) {
        balance -= amt;                                    // paisa
        db.add(user + " -" + amt);                         // DB
        String sms = "Hi " + naam.toUpperCase() + ...;     // SMS (marketing ki nayi line)
    }
}
```

Chala ke dekha (mota class vs teen class, dono me naam null):

```
--- mota class, naam null aaya ---
payment: FAILED (NullPointerException)
balance = 500, db = [arpan -500]

--- teen class, wahi null ---
payment: SUCCESS
sirf SMS fail, baad me dobara bhejo
balance = 500, db = [arpan -500]
```

**Dikkat:** mota class me paisa kat gaya aur DB me bhi likh gaya, par user ko "FAILED" dikha, kyunki SMS ki galti ne poora `pay()`
gira diya. Ab user dobara pay karega aur 500 do baar kategi. Aur ye sab marketing ki ek line se hua, paisa wala code kisi ne chhua bhi nahi.

## Fix

```java
class PaymentService      { void pay(...) { ... } }        // sirf paisa
class PaymentRepository   { void save(Payment p) { ... } } // sirf DB
class NotificationService { void sendSms(...) { ... } }    // sirf SMS
```

Ab SMS fail hua to sirf SMS fail: payment SUCCESS, SMS baad me dobara. SMS ka format badla to sirf `NotificationService` khulegi.
Restaurant jaisa: cook, waiter aur cashier alag log hain, cook badla to cashier pe asar nahi.

## Trap

Class alag karne ke baad bhi ye galti ho sakti hai: `@Transactional pay()` ke andar hi `notificationService.sendSms()` bula diya, to
SMS ka exception poore transaction ko rollback kar deta hai. SMS hamesha payment pakka hone ke BAAD, transaction ke bahar bhejo.
Yahi soch HLD me outbox / Kafka se notification bhejne ki hai.

## Tera project

Tere projects me Controller (HTTP), Service (logic) aur Repository (DB) ki teen parat isi wajah se hain. PR drill me "controller me
hi DB query likh di" wali galti pakadta hai, to wo yahi niyam toot raha hota hai.
