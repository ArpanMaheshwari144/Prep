# abstract keyword

## Tere bank se shuru

Tere SimpleBankSystem me `Account` abstract class hai. Agar tu kahin `new Account(...)` likhega, to code compile hi nahi hoga.
Kyunki `Account` adhoora hai: usme byaaj ka formula hai hi nahi. Savings ka 4% hai, FD ka 7% hai, Current ka 0 hai, par "Account"
ka koi byaaj nahi hota. `abstract` likh ke hum Java ko bata rahe hain ki ye class sirf aage badhane ke liye bani hai, iska seedha
object mat banane dena.

```
abstract class Account             new Account(...)   -> compile error, kyunki ye adhoora hai
  abstract calculateInterest()     new Savings(...)   -> theek, kyunki Savings ne formula bhar diya
```

## Ye kis dikkat se bana

Pehli dikkat: koi galti se aisa account na bana de jiska byaaj kisi ne likha hi nahi. Abstract class ka object Java banne hi nahi deta.

Doosri dikkat: kal koi naya account type banaye aur byaaj ka method likhna bhool jaaye. `calculateInterest()` abstract hai, isliye
jo bachcha ise nahi likhega, uska code compile pe hi ruk jaayega. Galti prod me nahi, likhte waqt hi pakdi jaati hai.

## Teen jodi jo kabhi saath nahi chalti

**abstract aur final:** `abstract` method ka matlab hai ki iski body abhi nahi hai, bachcha likhega. `final` method ka matlab hai ki
isko koi bachcha badal nahi sakta. Ek taraf tu bol raha hai ki bachcha zaroor likhe, doosri taraf bol raha hai ki bachcha haath bhi
na lagaye. Dono ek saath ho hi nahi sakte, isliye Java compile nahi hone deta.

**abstract aur private:** `private` method sirf usi class ke andar dikhta hai, bachche ko dikhta hi nahi. Agar wo `abstract` bhi hai,
to bachche ko use likhna padega. Par jo method bachche ko dikhta hi nahi, use wo likhega kaise? Isliye ye jodi bhi band hai.

**abstract aur static:** `static` method object ka nahi, class ka hota hai. Use bulane ke liye object nahi chahiye, seedha
`Account.show()` likh dete ho. Maan lo `show()` static bhi hai aur abstract bhi, aur tune `Account.show()` bulaya. Java kaunsi body
chalaye? `Account` me body hai nahi, kyunki wo abstract hai. Aur bachche wali body tak Java jaayega hi nahi, kyunki static method me
Java object nahi dekhta, sirf class ka naam dekhta hai. Na yahan body hai, na wahan tak pahunch sakta hai, isliye Java ye jodi rok deta hai.

## Prod wala trap: parent ke constructor me bachche ka method mat bulao

Maan lo `Account` ke constructor me hi byaaj print karwa diya, aur Savings me rate ek normal (instance) field hai:

```java
Account(double b) { balance = b; System.out.println("Account bana, byaaj = " + calculateInterest()); }
// Savings me:  double rate = 0.04;

new Savings(5000);
// output:  Account bana, byaaj = 0.0     <- galat
//          baad me byaaj = 200.0        <- sahi
```

Ye maine chala ke dekha. Galat isliye aata hai kyunki object bante waqt pehle parent ka constructor chalta hai, aur us waqt tak bachche
ki field (`rate`) set hi nahi hui hoti, to wo 0 hoti hai. Bachche ka method chal to jaata hai, par adhoore bachche pe. Tere asli code me
`INTEREST_RATE` `static final` hai, aur static cheez pehle hi ban jaati hai, isliye tu is trap se bach gaya. Niyam: parent ke constructor
me wo method kabhi mat bulao jise bachcha badal sakta hai.

## Interview line

```
An abstract class cannot be instantiated; in my bank, Account is abstract because it has no
interest formula, and each child must provide calculateInterest or it will not compile.
abstract cannot pair with final, private or static. One trap: never call an overridable
method from the parent constructor; the child's fields are not set yet, so it returns 0.
```
