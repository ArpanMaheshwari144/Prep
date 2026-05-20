# Java 8 — Lambda + Functional Interfaces

---

## 1️⃣ Problem Before Lambda

```
Imagine button click handler:

OLD Java 7 style:
   button.setOnClick(new ClickHandler() {
       @Override
       public void onClick() {
           System.out.println("Clicked!");
       }
   });
   
   = 5 lines for ONE action
   = Boilerplate kaafi
```

```
WITH Lambda (Java 8+):
   button.setOnClick(() -> System.out.println("Clicked!"));
   
   = 1 line same kaam
```

---

## 2️⃣ Lambda Syntax — Visual

```
   (parameters) -> { body }
   ─────────────    ──────
   inputs            what to do
```

### Examples
```java
// 0 parameters
() -> System.out.println("Hi");

// 1 parameter (parens optional for single)
x -> x * 2
(x) -> x * 2

// 2 parameters
(a, b) -> a + b

// Multiple statements (body needs braces)
(x) -> {
    int doubled = x * 2;
    return doubled + 1;
}
```

---

## 3️⃣ Functional Interface

```
A functional interface = interface with EXACTLY ONE abstract method
```

```java
@FunctionalInterface
interface Calculator {
    int calculate(int a, int b);   // ONE abstract method
}

// Implement using lambda:
Calculator add = (a, b) -> a + b;
Calculator sub = (a, b) -> a - b;
Calculator mul = (a, b) -> a * b;

add.calculate(5, 3);   // 8
sub.calculate(5, 3);   // 2
mul.calculate(5, 3);   // 15
```

```
KEY:
   Functional interface = lambda-compatible
   Lambda body = implementation of that ONE method
```

---

## 4️⃣ Built-in Functional Interfaces — PFCS

```
   Java 8 ne 4 famous diye:
   
   P = Predicate   (test something, return boolean)
   F = Function    (transform, take + return)
   C = Consumer    (take, return nothing)
   S = Supplier    (no input, return something)
```

---

## 5️⃣ Predicate (test → boolean)

```java
Predicate<Integer> isEven = x -> x % 2 == 0;

isEven.test(4);    // true
isEven.test(7);    // false
```

```
Use case:
   Filter list:
      list.stream()
          .filter(isEven)
          .toList();
```

---

## 6️⃣ Function<T, R> (input → output)

```java
Function<Integer, Integer> square = x -> x * x;
Function<String, Integer> length = s -> s.length();

square.apply(5);      // 25
length.apply("Hi");   // 2
```

```
Use case:
   Map transformation:
      list.stream()
          .map(square)
          .toList();
```

---

## 7️⃣ Consumer<T> (take → no return)

```java
Consumer<String> printer = s -> System.out.println(s);
Consumer<Integer> doubler = x -> System.out.println(x * 2);

printer.accept("Hello");   // prints Hello
doubler.accept(5);          // prints 10
```

```
Use case:
   forEach iteration:
      list.forEach(printer);
```

---

## 8️⃣ Supplier<T> (no input → output)

```java
Supplier<String> greeting = () -> "Hello World";
Supplier<Integer> randomInt = () -> new Random().nextInt(100);

greeting.get();       // "Hello World"
randomInt.get();      // some random int
```

```
Use case:
   Lazy initialization:
      Optional.orElseGet(supplier);
```

---

## 9️⃣ PFCS Summary

```
┌────────────┬──────────────┬──────────────┐
│  Interface │ Method        │ Signature    │
├────────────┼──────────────┼──────────────┤
│ Predicate  │ test(T)       │ T → boolean  │
│ Function   │ apply(T)      │ T → R        │
│ Consumer   │ accept(T)     │ T → void     │
│ Supplier   │ get()         │ () → T       │
└────────────┴──────────────┴──────────────┘

Memory: PFCS = Predicate, Function, Consumer, Supplier
```

---

## Method References (::)

```
Lambda ka shortest form jab method already exists.
```

### Before (Lambda)
```java
list.forEach(s -> System.out.println(s));
```

### After (Method Reference)
```java
list.forEach(System.out::println);
```

```
:: means "use this method as a lambda"
```

### Types

```java
// Static method reference
Function<String, Integer> parse = Integer::parseInt;

// Instance method (specific object)
Consumer<String> log = logger::info;

// Instance method (class type)
Function<String, Integer> len = String::length;

// Constructor reference
Supplier<List<String>> newList = ArrayList::new;
```

---

## 1️⃣1️⃣ Real Examples

```java
// Filter even numbers
List<Integer> nums = List.of(1, 2, 3, 4, 5);
nums.stream()
    .filter(n -> n % 2 == 0)
    .forEach(System.out::println);
// Output: 2, 4

// Transform names to uppercase
List<String> names = List.of("arpan", "suresh");
names.stream()
     .map(String::toUpperCase)
     .forEach(System.out::println);
// Output: ARPAN, SURESH

// Custom sort
people.sort((a, b) -> a.age - b.age);
people.sort(Comparator.comparingInt(p -> p.age));
```

---

## 1️⃣2️⃣ Lambda Capture (Closure)

```java
int multiplier = 3;

Function<Integer, Integer> times = x -> x * multiplier;
//                                       ↑ captures from outer scope

times.apply(5);   // 15
```

```
RULE:
   Lambda can read enclosing variables
   They must be EFFECTIVELY FINAL
   (not changed after lambda created)
   
   multiplier = 3;     // OK to capture
   multiplier = 5;     // compile error if used in lambda
```

---

## 1️⃣3️⃣ Visual — Lambda Evolution

```
Anonymous class:
   new Comparator<Integer>() {
       public int compare(Integer a, Integer b) {
           return a - b;
       }
   }
   
   ↓ become

Lambda:
   (a, b) -> a - b
   
   ↓ if pattern matches method...

Method Reference:
   Integer::compare
```

---

## Memory Hooks

```
Lambda      = anonymous function (shortcut)
              (params) -> body

Functional Interface = ONE abstract method
                     = lambda target

PFCS = 4 famous functional interfaces:
   P  → Predicate  (test, returns bool)
   F  → Function   (transform)
   C  → Consumer   (take, no return)
   S  → Supplier   (give, no input)

::  → method reference (use existing method)
      Class::method
      Class::new

Capture = lambda reads enclosing variables
          (must be effectively final)
```

[← Back to JAVA](../)
