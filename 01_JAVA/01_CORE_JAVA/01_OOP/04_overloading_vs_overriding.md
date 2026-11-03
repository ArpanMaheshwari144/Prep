# Overloading vs Overriding

```
OVERRIDING (tera bank)                       OVERLOADING (roz likhta hai)
Account.calculateInterest()                  System.out.println(200)      -> println(int)
  Savings -> bal*0.04   (same naam,          System.out.println("Arpan")  -> println(String)
  FD      -> bal*0.07    same parameter,     System.out.println(3.5)      -> println(double)
  Current -> 0           bachcha badalta)    (same naam, parameter alag)

kaun chalega: CHALTE waqt, object dekh ke    kaun chalega: COMPILE pe, argument dekh ke
```

## Kis dikkat se

- **Overriding:** parent ka kaam har bachche ke liye alag (byaaj).
- **Overloading:** ek hi kaam alag input pe. Warna `printInt` / `printString` / `printDouble` alag naam yaad rakho.

## Prod trap (Collections se juda)

```java
class User {
    public boolean equals(User other) { ... }   // Object nahi, User likha
}
```

Override nahi, **overload** ho gaya (parent ka `equals(Object o)`, parameter alag). `HashSet` / `HashMap` andar `equals(Object)` bulate,
tera wala kabhi nahi chalta -> same user do baar set me, koi error nahi. Chala ke dekha: `set size = 2`.
Bachao: upar `@Override`; galat parameter pe compiler turant: "kuch override nahi kar raha".

**Chhota trap:** `static` method override nahi hota, chhup jaata (hiding). `A ref = new B(); ref.show()` -> static ho to A ka,
Java reference ka type dekhta, object nahi.

## Interview line

```
Overriding is the child replacing a parent method with the same signature, decided at runtime
by the object: in my bank, calculateInterest differs for Savings, FD and Current. Overloading
is the same name with different parameters, decided at compile time, like println(int) and
println(String). The classic bug is writing equals(User) instead of equals(Object): it
overloads instead of overriding, and HashSet silently keeps duplicates. @Override catches it.
```
