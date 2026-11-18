# Factory pattern

**Koi poochhe "Factory kya hai" to aise bolo:**
```
Kaunsa object banega, ye faisla ek jagah. Bulane wala bolta "savings chahiye", new khud nahi
likhta, aur use Account milta hai, andar kaunsa bachcha pata nahi. Constructor badla to sirf
factory badlo. Spring khud ek badi factory hai.
```

## Tera bank: AccountFactory

```java
public static Account create(String type, String id, String name, double balance) {
    switch (type.toLowerCase()) {
        case "savings": return new SavingsAccount(id, name, balance);
        case "current": return new CurrentAccount(id, name, balance);
        case "fd":      return new FixedDeposit(id, name, balance);
        default: throw new IllegalArgumentException("Unknown account type: " + type);
    }
}
```

Agar 10 jagah `new SavingsAccount(...)` likha hota aur kal constructor badalta, to 10 jagah badalna padta. Ab sirf factory me.
Factory `Account` lautati hai, kisi khaas class ko nahi, isliye bulane wale ko pata hi nahi andar kaunsa bachcha hai (polymorphism).

## Trap: type hi nahi aaya (null)

Chala ke dekha:

```
SAVINGS -> SavingsAccount
gold -> IllegalArgumentException: Unknown account type: gold
null -> NullPointerException: Cannot invoke "String.toLowerCase()" because "<parameter1>" is null
null (jaanch ke baad) -> account type dena zaroori hai
```

"gold" pe saaf error (O wala sabak), par null pe `toLowerCase()` hi phat gaya: user ko 500, log me bas NPE. Ilaaj: factory ke shuru
me null jaanch + saaf message, controller pe `@NotNull` (400 ban jaata).

## 10 se 100 type ho gaye to? (Arpan ka sawaal)

**Koi poochhe to aise bolo:**
```
2-4 type ho to switch theek, padhne me seedha. Type badhte jaayein to jinka sirf data alag hai
wo DB / config ki row, ek class; jinka kaam alag hai wo apni class, Spring Map me khud register.
Factory ki file naye type pe kabhi nahi khulti.
```

100 type ka matlab 100 class nahi. Zyada tar ka kaam ek jaisa, farak sirf data me (byaj, minimum balance, withdraw hoga ya nahi).
Unhe ek class + DB / config ki row banao:

```java
record AccountType(String code, double interest, double minBalance, boolean withdraw) {}
Map<String, AccountType> types;   // DB / config se bhara
```

```
AccountType[code=savings, interest=3.0, minBalance=1000.0, withdraw=true]
AccountType[code=senior, interest=3.5, minBalance=500.0, withdraw=true]  | kul type: 3
```

Senior Citizen = ek row, koi code nahi khula.

Jis type ka KAAM sach me alag hai (jaise FD maturity pe paisa lautata): apni class, `@Component`, `Account` interface ke peeche.
Factory constructor me `Map<String, Account>` maange, Spring saare bean naam ke saath khud bhar deta (O wali file jaisa). Naya
type = nayi class, factory ki file nahi khulti.

## Spring me

Spring khud ek badi factory hai: tu `new UserService()` kabhi nahi likhta, Spring object banata hai aur de deta hai.
