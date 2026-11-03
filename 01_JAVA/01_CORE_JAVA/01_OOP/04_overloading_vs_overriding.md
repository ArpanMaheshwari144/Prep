# Overloading vs Overriding

```
OVERRIDING (tera bank)                       OVERLOADING (roz likhta hai)
Account.calculateInterest()                  System.out.println(200)      -> println(int)
  Savings -> bal*0.04   (same naam,          System.out.println("Arpan")  -> println(String)
  FD      -> bal*0.07    same parameter,     System.out.println(3.5)      -> println(double)
  Current -> 0           bachcha badalta)    (same naam, parameter alag)

kaun chalega: CHALTE waqt, object dekh ke    kaun chalega: COMPILE pe, argument dekh ke
```

Overriding me naam bhi same hota hai aur parameter bhi same hote hain, bas bachcha parent ka kaam apne tareeke se likh deta hai.
Kaunsa wala chalega, ye program chalte waqt object dekh ke tay hota hai.

Overloading me naam same hota hai par parameter alag hote hain. Kaunsa wala chalega, ye compile ke waqt hi argument dekh ke tay
ho jaata hai.

## Ye kis dikkat se bane

**Overriding** isliye hai kyunki parent ka ek kaam har bachche ke liye alag hota hai. Jaise byaaj: Savings ka alag, FD ka alag.

**Overloading** isliye hai kyunki ek hi kaam alag-alag input pe karna hota hai. Agar overloading na hoti, to tujhe `printInt`,
`printString`, `printDouble` jaise alag-alag naam yaad rakhne padte.

## Prod trap (Collections se juda)

```java
class User {
    public boolean equals(User other) { ... }   // Object nahi, User likha
}
```

Yahan override nahi hua, **overload** ho gaya. Kyunki parent ka method `equals(Object o)` hai, aur tune parameter `User` likh
diya, to parameter alag ho gaya aur Java ne ise naya method maan liya. `HashSet` aur `HashMap` andar se `equals(Object)` hi
bulate hain, isliye tera wala method kabhi chalta hi nahi. Natija ye ki same user set me do baar chala jaata hai, aur koi error
bhi nahi aata. Ye chala ke dekha tha: `set size = 2` aaya.

Bachne ka tareeka: method ke upar `@Override` likho. Agar parameter galat hua, to compiler turant bol dega ki "ye method kuch
override nahi kar raha".

**Chhota trap:** `static` method override nahi hota, wo bas chhup jaata hai, ise hiding kehte hain. Maan lo `A ref = new B();`
likha aur phir `ref.show()` bulaya. Agar `show()` static hai, to A wala chalega, B wala nahi. Kyunki static method me Java
reference ka type dekhta hai, object nahi dekhta.

## Interview line

```
Overriding is the child replacing a parent method with the same signature, decided at runtime
by the object: in my bank, calculateInterest differs for Savings, FD and Current. Overloading
is the same name with different parameters, decided at compile time, like println(int) and
println(String). The classic bug is writing equals(User) instead of equals(Object): it
overloads instead of overriding, and HashSet silently keeps duplicates. @Override catches it.
```
