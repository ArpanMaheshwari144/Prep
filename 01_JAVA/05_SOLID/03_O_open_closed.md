# O: Open / Closed (naya jodo, purana mat chhedo)

**Koi poochhe "Open / Closed kya hai" to aise bolo:**
```
Naya kaam jodne ke liye purana tested code nahi kholna chahiye. Har payment mode ek interface
ke peeche apni class; kal Crypto aaya to bas nayi class, if-else ki chain nahi badhti.
Board pe naya socket lagate ho, poori wiring nahi kholte.
```

## Kahani

Pehle sirf UPI tha, phir Wallet aaya. Har baar wahi file khuli aur `else if` judta gaya. Kisi ne aakhri me `else` likh diya:
"baaki sab card se kaato". Phir app ne naya mode CRYPTO bheja, jo backend me abhi likha hi nahi tha.

```java
if (type.equals("UPI"))         return "UPI se " + amt;
else if (type.equals("WALLET")) return "Wallet se " + amt;
else                            return "Card se " + amt;   // "baaki sab card"
```

## Fix

```java
interface PaymentMethod { String pay(int amt); }
Map<String, PaymentMethod> methods;              // UPI, WALLET, CARD ... har ek apni class

PaymentMethod m = methods.get(type);
if (m == null) throw new IllegalArgumentException("mode nahi pata: " + type);
return m.pay(amt);
// kal Crypto: bas nayi class CryptoPayment, purani koi file nahi khuli
```

Chala ke dekha:

```
--- if-else, app ne naya mode CRYPTO bheja ---
Card se 500
--- interface + map, wahi CRYPTO ---
ERROR: mode nahi pata: CRYPTO
--- CryptoPayment jodne ke baad ---
Crypto se 500 | UPI se 200
```

**Dikkat:** if-else ne user ke card se 500 chupchaap kaat liye, jabki usne crypto chuna tha. Koi crash nahi, isliye kisi ko pata bhi nahi.
**Hal:** interface + map me anjaan mode pe saaf error, aur CRYPTO jodne me sirf nayi class lagi; UPI wala code chhua tak nahi.

## Trap

if-else ki chain me aakhri `else` ko "baaki sab" bana dena: naya mode aayega to chupchaap galat raaste chala jaayega. Anjaan mode pe
hamesha error do, default pe mat bhejo.

## Tera project

Ye wahi Strategy pattern hai jo design patterns me padha. Spring me `Map<String, PaymentMethod>` constructor me maango, to saare
`PaymentMethod` bean apne bean naam ke saath khud bhar jaate hain, map khud nahi banana padta.
