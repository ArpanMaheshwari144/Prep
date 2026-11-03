# 4 Pillars: tere SimpleBankSystem me charon ek saath

Tere bank me 3 account: Savings (4% byaaj), Current (0%), FD (7%).

```
            Account (abstract)          <- ABSTRACTION: "byaaj kaise" nahi batata, bas
            balance (private)              "calculateInterest() hona chahiye"
            deposit() / withdraw()      <- ENCAPSULATION: paisa sirf darwaze se
               │ extends                <- INHERITANCE: deposit/withdraw teeno ko
     ┌─────────┼──────────┐                bina dobara likhe mil gaye
  Savings   Current    FixedDeposit
  bal*0.04    0         bal*0.07        <- POLYMORPHISM: ek hi call, teen alag jawab
```

`Main.java` me `a1.calculateInterest()` -> 200 · 0 · 3500. Call ek, formula kaunsa chalega ye JVM chalte waqt object dekh ke tay karta.

## Kis dikkat se bane

- **Encapsulation:** koi `balance = 999999` na kar de.
- **Inheritance:** deposit/withdraw teen jagah copy-paste na karna pade.
- **Polymorphism:** kal "Senior Citizen 8%" aaye to bas nayi class. Purane code me `if (type == ...)` nahi.
- **Abstraction:** `Account` ka seedha `new` nahi, aur har bachche ko apna byaaj batana hi padega, warna compile nahi.

## Trade-off (tere code me tha)

`balance` pehle `protected` tha taaki bachche `balance * 0.04` padh sakein. Keemat: darwaza bachchon ke liye khula.
Koi bachcha `balance = -999` likh de to `withdraw` ka check bypass, compiler kuch nahi rokta.

Ab: field `private`, bachche `getBalance() * INTEREST_RATE` se sirf padhte. Inheritance code deta hai, par parent ke niyam todne ka raasta bhi.

## Interview line

```
In my bank project, Account is abstract with deposit and withdraw written once, and each
child (Savings, Current, FD) gives its own calculateInterest, so one call returns a different
result by type. Adding a new account type means a new class, not an if-else in old code. One
trade-off I fixed: balance was protected for the children, which let a child bypass the
withdraw check; now it is private and children read it through getBalance().
```
