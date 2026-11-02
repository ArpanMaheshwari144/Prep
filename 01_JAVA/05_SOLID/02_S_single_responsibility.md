# S: Single Responsibility (ek class, ek kaam)

**Niyam:** class ke badalne ki sirf EK wajah ho.

## Kahani

`PaymentService` me teen kaam the: paisa kaatna, DB me save karna, SMS bhejna. Marketing ne SMS ka format badla ->
PaymentService chhedni padi. DB team ne schema badla -> phir chhedni padi. Teen alag log, teen wajah, ek class. Ek
jagah badla aur doosra kaam toot gaya.

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

Ab SMS badla to sirf `NotificationService` khuli. Restaurant jaisa: cook, waiter, cashier alag; cook badla to cashier pe asar nahi.

## Tera project

Controller (HTTP), Service (logic), Repository (DB) ki teen parat isi wajah se. PR drill me "controller me hi DB
query" pakadta hai = yahi.

## Interview line

```
Each class has one reason to change: the controller handles HTTP, the service holds business logic and
the repository talks to the DB, so a change in one area does not break another.
```
