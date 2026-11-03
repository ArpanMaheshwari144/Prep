# 4 Pillars: tere SimpleBankSystem me charon ek saath

Tere bank me 3 tarah ke account hain: Savings jisme 4% byaaj hai, Current jisme 0% hai, aur FD jisme 7% hai. Charon pillar
isi ek chhote se code me dikh jaate hain.

```
            Account (abstract)          <- ABSTRACTION: "byaaj kaise" nahi batata, bas
            balance (private)              "calculateInterest() hona chahiye"
            deposit() / withdraw()      <- ENCAPSULATION: paisa sirf darwaze se
               │ extends                <- INHERITANCE: deposit/withdraw teeno ko
     ┌─────────┼──────────┐                bina dobara likhe mil gaye
  Savings   Current    FixedDeposit
  bal*0.04    0         bal*0.07        <- POLYMORPHISM: ek hi call, teen alag jawab
```

`Main.java` me teeno account pe `calculateInterest()` bulaya jaata hai (`a1.calculateInterest()` jaisa). Savings pe 200 aata
hai, Current pe 0 aata hai, aur FD pe 3500 aata hai. Call ek hi hai, par kaunsa formula chalega, ye JVM chalte waqt (runtime pe) object dekh ke tay karta hai.

## Ye charon kis dikkat se bane

**Encapsulation** isliye hai taaki koi bahar se seedha `balance = 999999` na kar de.

**Inheritance** isliye hai taaki deposit aur withdraw ka code teen jagah copy-paste na karna pade. Ek baar `Account` me likha,
aur teeno account ko mil gaya.

**Polymorphism** isliye hai taaki kal "Senior Citizen 8%" wala naya account aaye, to bas ek nayi class bana do. Purane code me
`if (type == ...)` jodne ki zaroorat nahi padti.

**Abstraction** isliye hai taaki `Account` ka seedha `new` na ho sake, aur har bachche ko apna byaaj ka formula batana hi pade.
Agar koi bachcha nahi batayega, to uska code compile hi nahi hoga.

## Trade-off (tere code me tha)

Pehle `balance` `protected` tha, taaki bachche seedha `balance * 0.04` padh sakein. Par iski keemat ye thi ki darwaza bachchon
ke liye khula reh gaya. Koi bachcha `balance = -999` likh deta, to `withdraw` wala check bypass ho jaata, aur compiler use
bilkul nahi rokta.

Ab field `private` hai, aur bachche `getBalance() * INTEREST_RATE` se balance sirf padhte hain, badal nahi sakte. Seekh ye hai
ki inheritance tujhe code to deta hai, par saath me parent ke niyam todne ka raasta bhi de deta hai.

## Interview line

```
In my bank project, Account is abstract with deposit and withdraw written once, and each
child (Savings, Current, FD) gives its own calculateInterest, so one call returns a different
result by type. Adding a new account type means a new class, not an if-else in old code. One
trade-off I fixed: balance was protected for the children, which let a child bypass the
withdraw check; now it is private and children read it through getBalance().
```
