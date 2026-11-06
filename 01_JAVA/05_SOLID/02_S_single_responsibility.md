# S: Single Responsibility (ek class, ek kaam)

**Niyam:** kisi class ke badalne ki sirf EK wajah honi chahiye.

## Kahani

Tere `PaymentService` me teen kaam the: paisa kaatna, DB me save karna, aur SMS bhejna. Marketing ne SMS ka format badla, to
PaymentService kholni padi. Phir DB team ne schema badla, to phir se PaymentService kholni padi. Teen alag log the, badalne ki
teen alag wajah thi, par class ek hi thi. Natija ye hua ki ek jagah kuch badla aur doosra kaam toot gaya.

```java
class PaymentService {            // galat: teen kaam
    void pay(...)     { ... }     // paisa
    void saveToDb()   { ... }     // DB team ka
    void sendSms()    { ... }     // marketing ka
}
```

## Fix

```java
class PaymentService      { void pay(...) { ... } }        // sirf paisa
class PaymentRepository   { void save(Payment p) { ... } } // sirf DB
class NotificationService { void sendSms(...) { ... } }    // sirf SMS
```

Ab agar SMS ka format badla, to sirf `NotificationService` khulegi, baaki dono ko koi chhoota bhi nahi. Ye restaurant jaisa hai:
cook, waiter aur cashier alag-alag log hain, isliye cook badal gaya to cashier ke kaam pe koi asar nahi padta.

## Tera project

Tere projects me Controller (HTTP sambhalta hai), Service (logic rakhta hai) aur Repository (DB se baat karta hai) ki teen parat
isi wajah se hain. PR drill me jab tu "controller me hi DB query likh di" wali galti pakadta hai, to wo yahi niyam toot raha hota hai.
