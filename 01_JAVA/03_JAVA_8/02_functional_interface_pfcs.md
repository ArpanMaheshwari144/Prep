# Functional Interface (PFCS)

## Kahani

Wahi 100 patients aage le chalte hain. Hospital me roz yahi 4 kaam hote hain:

```java
Predicate<Patient> isSerious = p -> p.getAge() > 60;       // chuno: haan ya nahi
Function<Patient, String> toName = p -> p.getName();        // badlo: Patient se naam
Consumer<String> sendSms = name -> sms.send(name);          // karo: bhej do, kuch lautana nahi
Supplier<Patient> newPatient = () -> new Patient();         // banao: input kuch nahi

patients.stream()
        .filter(isSerious)     // Predicate
        .map(toName)           // Function
        .forEach(sendSms);     // Consumer
```

## Tasveer

Har interface ek alag shakal ka khaana hai. Lambda usi khaane me fit hogi jiski shakal mile:

```
             andar jaata     bahar aata      method
Predicate     T          ->   true/false      test()
Function      T          ->   R               apply()
Consumer      T          ->   (kuch nahi)     accept()
Supplier      (kuch nahi)->   T               get()
```

## 3 khaas baat

1. **Lambda ka koi type nahi hota, wo jis interface me rakhi jaaye wahi uska type ban jaata hai.** `p -> p.getAge() > 60` Predicate me rakho to Predicate hai. Pichhli file ka "ek khaana" yahi interface hai.
2. **Stream ke methods yahi 4 maangte hain:** `filter` Predicate maangta hai, `map` Function, `forEach` Consumer, aur `orElseGet` / `generate` Supplier. Method ka kaam pata hai to interface apne aap pata chal jaata hai.
3. **`@FunctionalInterface` sirf ek pehredaar hai.** Iske bina bhi interface functional ho sakta hai. Annotation lagane se bas itna hota hai ki koi doosra abstract method jode to compiler wahin rok deta hai.

## Prod wala trap

```java
interface MathOperation {             // @FunctionalInterface nahi lagaya
    int operate(int a, int b);
    int undo(int a, int b);           // teammate ne baad me joda
}
```

Is interface ki khud koi galti nahi dikhti, error poore codebase me har us jagah aata hai jahan `(a, b) -> a + b` likha tha. Annotation laga hota to error usi ek line pe aata jahan `undo` joda gaya.

## Tera project

InsuranceManagementSystem `ClaimService` me `.orElseThrow(() -> new EntityNotFoundException("Claim Not Found " + id))`. Ye `() -> ...` ek Supplier hai: input kuch nahi, exception bana ke deta hai, aur sirf tab chalta hai jab claim na mile.

## Interview line

```
A functional interface has exactly one abstract method, and a lambda takes its type from it.
Java 8 gives four core ones: Predicate (test, returns boolean), Function (apply, T to R),
Consumer (accept, returns nothing) and Supplier (get, takes nothing).
@FunctionalInterface makes the compiler enforce the single-method rule.
```
