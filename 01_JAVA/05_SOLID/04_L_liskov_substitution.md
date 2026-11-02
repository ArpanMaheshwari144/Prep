# L: Liskov Substitution (bachcha baap ki jagah lag sake)

**Niyam:** jahan parent chalta hai, wahan child daalo to kuch na toote.

## Kahani

`Account.withdraw()` sabke paas. `SavingsAccount`, `CurrentAccount` theek. `FixedDeposit extends Account` ne socha
"FD se maturity se pehle nikaal nahi sakte" aur `withdraw()` me exception phenk di.

```java
class FixedDeposit extends Account {
    @Override void withdraw(double amt) { throw new RuntimeException("FD se withdraw nahi"); }
}
void payBill(Account a) { a.withdraw(500); }   // FD aaya to yahin toota
```

Jo code "koi bhi Account" maan ke withdraw karta, wo FD pe toot gaya. Bird / Penguin jaisa: Penguin Bird hai par ud nahi sakta.

## Fix

```java
class Account { double balance; }
class WithdrawableAccount extends Account { void withdraw(double amt) { ... } }
class SavingsAccount extends WithdrawableAccount { }
class FixedDeposit extends Account { }   // withdraw hai hi nahi
```

Seedha pehchaan: "extends kiya aur override me exception phenk di / kuch nahi kiya" = LSP toota.

## Interview line

```
A subclass must keep the parent promise; if a fixed deposit cannot withdraw, it should not inherit
withdraw, so I split the hierarchy instead of throwing an exception in the override.
```
