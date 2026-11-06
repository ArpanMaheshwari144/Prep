# O: Open / Closed (naya jodo, purana mat chhedo)

**Niyam:** class naye kaam ke liye khuli honi chahiye (extend kar sako), par purana tested code chhedne ke liye band honi chahiye.

## Kahani

Pehle sirf UPI tha. Phir Card aaya, phir Wallet, phir Crypto. Har baar wahi file kholni padi aur `else if` ki chain lambi hoti
gayi. Aur har baar ye darr rehta tha ki purane tested raaste me koi naya bug na aa jaaye.

```java
if (type.equals("UPI"))         { ... }
else if (type.equals("CARD"))   { ... }
else if (type.equals("WALLET")) { ... }   // har naye mode pe tested code chhedo
```

## Fix

```java
interface PaymentMethod { void pay(double amount); }
class UpiPayment  implements PaymentMethod { public void pay(double a) { ... } }
class CardPayment implements PaymentMethod { public void pay(double a) { ... } }
// kal Crypto: bas nayi class CryptoPayment, purani koi file nahi khuli
```

Ab har payment ka apna class hai, sab ek `PaymentMethod` interface ke peeche. Kal naya mode aaya to bas ek nayi class banegi,
purani koi file nahi khulegi. Ye bijli ke board jaisa hai: naya appliance lagana hai to naya socket laga do, poori building ki
wiring nahi kholte.

## Tera project

Ye wahi Strategy pattern hai jo design patterns me padha tha. Spring me saare `PaymentMethod` bean ek `Map` me apne aap inject
ho jaate hain, to tu type ke hisaab se map se sahi wala utha leta hai, aur `if-else` ki zaroorat hi khatam ho jaati hai.
