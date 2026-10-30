# Method Reference (`::`)

## Kahani

Wahi patients ki list hai. Sabke naam print karne hain:

```java
names.forEach(name -> System.out.println(name));   // lambda
names.forEach(System.out::println);                 // method reference
```

Lambda bas ek method ko aage pass kar rahi thi: jo `name` aaya, wahi `println` ko de diya. Jab lambda ka saara kaam itna hi ho, to `::` se seedha method ka naam de do.

## Tasveer

Lambda ek beech wala aadmi hai jo cheez lekar seedha aage pakda deta hai. `::` me wo aadmi hat jaata hai aur cheez seedha method tak pahunchti hai.

```
lambda:   name  ->  [ name -> println(name) ]  ->  println
::        name  ->  println                        (beech wala hata)
```

## 3 khaas baat

1. **Ye 4 tarah ka hota hai.** Pehchaan ye hai ki `::` ke baayi taraf kya likha hai:
   ```
   Math::abs              static method          x -> Math.abs(x)
   System.out::println    ek khaas object        x -> System.out.println(x)
   String::toUpperCase    jo object aaya, usi pe  s -> s.toUpperCase()
   Patient::new           constructor            name -> new Patient(name)
   ```
2. **Ye sirf tab chalta hai jab lambda me kaam sirf ek method call ho.** `name -> System.out.println("Hello " + name)` me string jodne ka kaam bhi hai, isliye ye lambda hi rahegi.
3. **Ye koi naya feature nahi, wahi functional interface hai.** `String::toUpperCase` ko `map` me do to wo `Function<String, String>` ban jaata hai. Pichhli file wali shakal milni chahiye.

## Prod wala trap

Dono neeche ek jaise dikhte hain, par null pe alag waqt fatte hain.

```java
User u = null;
Supplier<String> s1 = () -> u.getName();   // abhi kuch nahi, s1.get() pe NPE
Supplier<String> s2 = u::getName;          // isi line pe NPE
```

`u::getName` object ko usi waqt pakad leta hai jab line chalti hai, isliye null ho to wahin fat jaata hai. Lambda `u` ko tab dekhti hai jab use call kiya jaaye.

## Tera project

SimpleBankSystem `AccountService.getAccountHolderNames()` me tune likha hai: `.map(Account::getHolderName)`. Wo teesra type hai: jo `Account` aaya, usi pe `getHolderName()`.

## Interview line

```
A method reference is a shorter form of a lambda that only calls one existing method.
There are four kinds: static (Math::abs), a specific object (System.out::println),
any object of a type (String::toUpperCase), and constructor (Patient::new).
```
