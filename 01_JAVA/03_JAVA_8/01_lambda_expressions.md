# Lambda Expressions

## Kahani

Manager bola, "100 patients ki list naam se sort karo." Java 7 me iske liye ek poori anonymous class likhni padti thi, sirf ek tulna (compare) ke liye:

```java
Collections.sort(patients, new Comparator<Patient>() {
    @Override
    public int compare(Patient a, Patient b) {
        return a.getName().compareTo(b.getName());
    }
});
```

Java 8 me wahi kaam ek line me:

```java
Collections.sort(patients, (a, b) -> a.getName().compareTo(b.getName()));
```

## Tasveer

Comparator ek form hai jisme sirf **ek khaali khaana** hai: `compare`. Lambda bas us khaane ko bharta hai, form ka naam aur `@Override` dobara nahi likhna padta.

```
Comparator  [ compare(a, b) : ______ ]   <- sirf ek khaana
lambda           (a, b) -> a.getName().compareTo(b.getName())
```

## 3 khaas baat

1. **Lambda tabhi chalti hai jab interface me sirf ek abstract method ho** (functional interface). Do khaali khaane hon to compiler ko pata hi nahi chalega ki kaunsa bharna hai. Default aur static method gine nahi jaate, kyunki unki body pehle se bhari hoti hai.
2. **"Ek method" ka matlab "ek line" nahi hai.** `{}` ke andar loop, if-else, jitna chahe code likh sakte ho. Lambda us ek method ki poori body hai.
3. **Asli fayda line kam karna nahi, logic ko argument ki tarah bhejna hai** (`filter(x -> ...)`, `sorted(...)`). Lambda koi nayi taakat nahi deti aur tez bhi nahi chalti. Bas ye logic bhejne ka aasaan tareeka hai, aur Streams isi pe tike hain.

## Prod wala trap

Lambda ke andar bahar ka local variable badal nahi sakte.

```java
int count = 0;
list.forEach(p -> count++);   // compile error: count "effectively final" hona chahiye
```

Wajah ye hai ki lambda variable ki copy leta hai. Ye wahi `int bal = w.balance` wali copy ki kahani hai jo pass by value me dekhi thi. Ginti karni ho to `list.size()` ya `stream().count()` lo.

## Tera project

Multithreading practical (`DeadlockDemo`, `ProducerConsumer`) me `new Thread(() -> {...})` likha hai. Wahan `Runnable` ka ek khaana (`run`) lambda se bhara gaya hai.

## Interview line

```
Lambda is a short way to implement a functional interface (one abstract method).
It lets us pass behaviour as an argument, which is what Streams are built on.
Local variables used inside must be effectively final.
```
