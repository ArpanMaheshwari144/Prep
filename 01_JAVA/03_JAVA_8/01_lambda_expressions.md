# Lambda Expressions

## Scene

Mausi ke mart me 3 dabbe hain: ghee 550, aata 320, daal 140. Mausi boli, "list daam ke hisaab se laga."

Java 7 me ek chhoti si tulna ke liye poori anonymous class likhni padti thi:

```java
Collections.sort(l, new Comparator<Item>() {
    @Override public int compare(Item x, Item y) {
        return Integer.compare(x.price(), y.price());
    }
});
```

Java 8 me wahi kaam ek line me ho jaata hai:

```java
l.sort((x, y) -> Integer.compare(y.price(), x.price()));
```

Chala ke dekha:

```
Java 7 tareeka:         [daal 140, aata 320, ghee 550]
lambda (mehnga pehle):  [ghee 550, aata 320, daal 140]
```

## Dikkat -> hal

Comparator ek form hai jisme sirf ek khaali khaana hai (`compare`). Lambda bas wahi khaana bharta hai, baaki form ka dhaancha (`new`, `@Override`) dobara nahi likhna padta. Isliye lambda wahi chalti hai jahan interface me sirf ek abstract method ho (functional interface). Iska asli fayda line kam karna nahi hai, logic ko argument ki tarah bhejna hai (`filter(x -> ...)`, `sort(...)`). Streams isi pe tike hain.

## Prod wala trap

```java
int count = 0;
list.forEach(x -> count++);
// error: local variables referenced from a lambda expression must be final or effectively final
```

Lambda bahar ke variable ki copy leta hai, ye wahi `int bal = w.balance` wali kahani hai. Isliye use badal nahi sakte. Ginti chahiye to `list.size()` ya `stream().count()` lo.

## Tera project

Multithreading practical me `new Thread(() -> {...})` likha hai. Wahan `Runnable` ka ek hi khaana hai (`run`), aur wahi lambda se bhara gaya hai.

## Interview line

```
Lambda is a short way to implement a functional interface, one with a single abstract method.
It lets us pass behaviour as an argument, which is what Streams are built on.
Local variables used inside it must be effectively final.
```
