# L: Liskov Substitution (bachcha baap ki jagah lag sake)

**Niyam:** jahan parent chalta hai, wahan agar child daal do, to kuch bhi nahi tootna chahiye.

## Kahani

`Account.withdraw()` sab account ke paas hai. `SavingsAccount` aur `CurrentAccount` me ye theek chalta hai. Par
`FixedDeposit extends Account` ne socha ki "FD se maturity se pehle paisa nikaal nahi sakte", aur isliye usne `withdraw()` me
exception phenk di.

```java
class FixedDeposit extends Account {
    @Override void withdraw(double amt) { throw new RuntimeException("FD se withdraw nahi"); }
}
void payBill(Account a) { a.withdraw(500); }   // FD aaya to yahin toota
```

Dikkat ye hai ki jo code ye maan ke chalta hai ki "koi bhi Account aaye, withdraw ho jaayega", wo FD aate hi toot gaya. Ye Bird
aur Penguin wali misaal jaisa hai: Penguin Bird to hai, par ud nahi sakta. To agar Bird me `fly()` hai, to Penguin us wade ko
nibha hi nahi sakta.

## Fix

```java
class Account { double balance; }
class WithdrawableAccount extends Account { void withdraw(double amt) { ... } }
class SavingsAccount extends WithdrawableAccount { }
class FixedDeposit extends Account { }   // withdraw hai hi nahi
```

Ab `withdraw()` sirf `WithdrawableAccount` me hai. FD seedha `Account` se aata hai, to uske paas `withdraw()` hai hi nahi, aur
koi use galti se bula bhi nahi sakta.

Pehchaanne ka seedha tareeka: agar kisi class ne extend kiya aur override me exception phenk di ya method khaali chhod diya, to
samjho LSP toot gaya hai.

## Interview line

```
A subclass must keep the parent promise; if a fixed deposit cannot withdraw, it should not inherit
withdraw, so I split the hierarchy instead of throwing an exception in the override.
```
