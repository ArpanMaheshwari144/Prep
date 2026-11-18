# L: Liskov Substitution (bachcha baap ki jagah lag sake)

**Koi poochhe "Liskov kya hai" to aise bolo:**
```
Jahan baap class chalti hai, wahan bachcha daal do to kuch nahi tootna chahiye. FD bhi Account
hai, par withdraw nahi kar sakta; agar FD ne withdraw() me exception phenki to L toota.
Isliye withdraw sirf un account me do jo sach me nikaal sakte.
```

## Kahani

Bank me mahine ke ant me ek job sab account se 500 ka bill kaatti hai. List me Arpan (Savings), Ravi (FD), Priya (Savings).
`FixedDeposit extends Account` ne socha "FD se maturity se pehle paisa nahi nikalta", aur `withdraw()` me exception phenk di.

```java
class FixedDeposit extends Account {
    @Override void withdraw(double amt) { throw new RuntimeException("FD se withdraw nahi"); }
}
for (Account a : list) a.withdraw(500);   // FD aaya to yahin toota
```

Chala ke dekha:

```
--- mahine ka bill job: sab account se 500 ---
Arpan ka bill kata, bacha 500.0
JOB GIRI: FD se withdraw nahi
Priya ka balance: 800.0 (bill kata hi nahi)
```

**Dikkat:** galti sirf Ravi ke FD ki, par poori job gir gayi aur Priya ka bill bhi nahi kata. Job ne maana tha "har Account se
withdraw hoga", FD ne wo wada tod diya. Bird / Penguin jaisa: Penguin Bird hai par ud nahi sakta.

## Fix

```java
class Account { double balance; }
class WithdrawableAccount extends Account { void withdraw(double amt) { ... } }
class SavingsAccount extends WithdrawableAccount { }
class FixedDeposit extends Account { }   // withdraw hai hi nahi
void payBill(WithdrawableAccount a) { a.withdraw(500); }
```

Ab FD ko `payBill()` me daala to compile hi nahi hota:

```
error: incompatible types: FixedDeposit cannot be converted to WithdrawableAccount
```

Galti raat ki job me nahi, likhte waqt hi pakdi gayi (IntelliJ laal kar deta).

## Trap

Override karke `throw new UnsupportedOperationException()`, ya method khaali chhod dena. Khaali to aur bura: FD ka bill "kat gaya"
dikhega, asal me kuch nahi kata. PR me ye do dikhein to samjho L toot raha hai.
