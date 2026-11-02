# O: Open / Closed (naya jodo, purana mat chhedo)

**Niyam:** class naye kaam ke liye khuli (extend), par purana tested code chhedne ke liye band.

## Kahani

Pehle sirf UPI tha. Phir Card aaya, phir Wallet, phir Crypto. Har baar wahi file khuli aur `else if` badhta gaya,
aur har baar purane tested raaste me bug ka darr.

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

Bijli ka board jaisa: naya appliance = naya socket, poori building ki wiring nahi kholte.

## Tera project

Ye wahi Strategy pattern hai (design patterns me padha). Spring me saare `PaymentMethod` bean ek `Map` me inject ho
jaate, type se utha lo, `if-else` khatam.

## Interview line

```
New behaviour comes in as a new class behind an interface, like a new PaymentMethod, so I extend the
system without editing tested code; it is the Strategy pattern.
```
