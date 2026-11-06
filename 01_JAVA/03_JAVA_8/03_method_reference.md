# Method Reference (`::`)

## Scene

Mausi boli: "saare dabbon ke naam bade akshar me board pe likh do."

Lambda se likho to har kadam pe beech me ek aadmi hai, jo cheez leta hai aur seedha ek method ko pakda deta hai:

```java
l.stream().map(i -> i.name()).map(s -> s.toUpperCase()).forEach(s -> System.out.println(s));
```

Jab lambda ka poora kaam bas ek method bulana ho, to us beech wale aadmi ko hata do aur seedha method ka naam de do:

```java
l.stream().map(Item::name).map(String::toUpperCase).forEach(System.out::println);
```

Chala ke dekha:

```
GHEE
AATA
DAAL
```

## Dikkat -> hal

`::` koi naya feature nahi hai. Ye wahi functional interface wala khaana hai, bas likhne ka chhota tareeka. `::` ke baayi taraf jo likha hota hai, usi se uski pehchaan hoti hai:

```
Math::abs             static method               x -> Math.abs(x)
System.out::println   ek khaas object             x -> System.out.println(x)
String::toUpperCase   jo object aaya, usi pe      s -> s.toUpperCase()
Item::new             constructor                 (n, p) -> new Item(n, p)
```

Ye sirf tab chalta hai jab kaam sirf ek method call ho. `s -> print("Board: " + s)` me string jodne ka kaam bhi hai, isliye wahan lambda hi rahegi.

## Prod wala trap

Dekhne me dono ek jaise hain, par null hone pe alag waqt pe fatte hain.

```java
Item khaali = null;
Supplier<String> s1 = () -> khaali.name();   // ban gaya, koi error nahi
Supplier<String> s2 = khaali::name;          // isi line pe NPE
```

Chala ke dekha:

```
s1 ban gaya, abhi koi error nahi
s2 wali line pe hi NPE
s1.get() pe NPE
```

`khaali::name` object ko usi waqt pakad leta hai jab line chalti hai, isliye null ho to wahin fat jaata hai. Lambda `khaali` ko tab dekhti hai jab use sach me bulaya jaaye. Log me NPE ki line alag aayegi, isliye debug karte waqt ye farak yaad rakhna.

## Tera project

SimpleBankSystem ke `getAccountHolderNames()` me `.map(Account::getHolderName)` likha hai. Ye teesra type hai, kyunki stream me jo `Account` aaya, usi pe `getHolderName()` chalta hai.

## Interview line

```
A method reference is a shorter form of a lambda that only calls one existing method.
There are four kinds: static (Math::abs), a specific object (System.out::println),
any object of a type (String::toUpperCase), and constructor (Item::new).
```
