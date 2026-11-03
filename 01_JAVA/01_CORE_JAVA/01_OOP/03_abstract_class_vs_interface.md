# Abstract class vs Interface: dono tere bank me pehle se

```
Account (ABSTRACT CLASS)                 AccountRepository (INTERFACE)
"ye kya HAI"                             "ye kya KAR SAKTA hai"
 - balance, accountId  (state)            save() · findById() · findAll() · delete()
 - deposit/withdraw    (likha hua code)   (sirf wada, code nahi)
 - calculateInterest() (bachcha bharega)        │ implements
      │ extends                          InMemoryAccountRepository  (aaj)
 Savings / Current / FD                  JpaAccountRepository       (kal, MySQL)
```

## Kis dikkat se

- **Abstract class:** teeno account me `balance` + deposit/withdraw ka code ek. Ek baar likho, sab le lein. State (field) aur constructor yahin.
- **Interface:** `AccountService` ko sirf "save / find ho jaata" jaanna. HashMap ya MySQL, fark nahi. Aaj `InMemory`, kal MySQL,
  service ki ek line nahi badlegi. `EventListener` me bhi wahi: aaj `EmailListener`, kal `SmsListener`.
- todoapp: `TodoRepository extends JpaRepository<Todo, Long>`. Spring interface deta, andar ka code khud banata.

## Trade-off

Class sirf EK abstract class extend kar sakti, interface kai. Ulta, interface me state nahi (na field, na constructor, sirf constant).

## Prod trap

Interface me naya method joda -> jitni class use implement karti, sab ek saath compile band.
Java 8 me `Collection` me `stream()` / `forEach()` jodne the, duniya bhar ki list classes toot jaati -> isliye `default` method:
interface me hi body, purani class bina chhede chalti.

```java
// AccountRepository me count() jodna ho:
default long count() { return findAll().size(); }   // warna InMemory + aage ki har class tootegi
```

## Interview line

```
In my bank project, Account is an abstract class because all accounts share state and the
same deposit and withdraw code. AccountRepository is an interface because the service only
needs the contract; today it is in-memory, tomorrow it can be MySQL without changing the
service. Rule of thumb: shared state and code -> abstract class, a capability or a swap
point -> interface. Adding a method to a live interface breaks every implementer, which is
why Java 8 added default methods.
```
