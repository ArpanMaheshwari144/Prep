# Future vs CompletableFuture — Visual Revision

---

## 1 Food Coupon vs Food Delivery Analogy

```
FUTURE (food coupon):
   Tu coupon liya restaurant se
   "Tumhare food ka token"

   Khaane ke liye:
      Tu khud restaurant jaake counter pe wait kare
      Until food ready
      Sirf TAB tujhe khana milega

   = BLOCKING (you wait)
```

```
COMPLETABLEFUTURE (food delivery app):
   Tu order kiya
   App ne kaha "wait ho jaye toh SMS karunga"

   Tu apna kaam karta rahe
   SMS aaya → "Order at door"
   Tu ne lagaya — done

   You can:
      Add instructions (extra spicy after step 1)
      Combine with another order
      Cancel
      Handle failure

   = NON-BLOCKING + CHAINABLE
```

---

## 2 Future Limitations

```java
Future<String> future = executor.submit(() -> fetch());

// Want to do something with result?
String result = future.get();        // BLOCKS thread
processResult(result);

// Problems with Future:
   get() = blocking
   No callback on completion
   Can't chain operations
   Can't combine multiple futures
   Manual exception handling
```

---

## 3 CompletableFuture (Java 8+)

```java
CompletableFuture<String> cf = CompletableFuture.supplyAsync(
    () -> fetch()
);

// Chain operations
cf.thenApply(result -> result.toUpperCase())
  .thenAccept(System.out::println);

// Main thread continues immediately!
// = NON-BLOCKING
```

---

## 4 Creating CompletableFuture

```java
// 1. Async with return value
CompletableFuture<Integer> cf1 = CompletableFuture.supplyAsync(
    () -> 42
);

// 2. Async without return
CompletableFuture<Void> cf2 = CompletableFuture.runAsync(
    () -> System.out.println("Done")
);

// 3. Already complete
CompletableFuture<String> cf3 = CompletableFuture.completedFuture("Hi");

// Custom executor:
CompletableFuture.supplyAsync(() -> task(), customPool);
```

---

## 5 Chaining Operations

```java
CompletableFuture.supplyAsync(() -> "Arpan")
    .thenApply(String::toUpperCase)        // transform
    .thenApply(s -> "Hello " + s)           // chain transform
    .thenAccept(System.out::println);       // consume

// Output: "Hello ARPAN"
```

```
thenApply()    → transform (Function)
thenAccept()   → consume (Consumer)
thenRun()      → just run (Runnable)
```

---

## 6 Visual — Chain Flow

```
   supplyAsync(() -> "Arpan")
        │
        ▼
   "Arpan"
        │
        ▼ thenApply(toUpperCase)
   "ARPAN"
        │
        ▼ thenApply(s -> "Hello " + s)
   "Hello ARPAN"
        │
        ▼ thenAccept(println)
   prints to console
```

---

## 7 Combining Multiple Futures

### thenCombine (2 → 1)
```java
CompletableFuture<Integer> price =
    CompletableFuture.supplyAsync(() -> fetchPrice());

CompletableFuture<Double> tax =
    CompletableFuture.supplyAsync(() -> fetchTax());

CompletableFuture<Double> total =
    price.thenCombine(tax, (p, t) -> p + (p * t));

total.thenAccept(System.out::println);
```

```
   price ─┐
          ├─→ combine → total
   tax ───┘

   Both run in parallel, result merged
```

### allOf (Wait for all)
```java
CompletableFuture<Void> all = CompletableFuture.allOf(
    cf1, cf2, cf3
);

all.thenRun(() -> System.out.println("All done"));
```

### anyOf (Wait for first)
```java
CompletableFuture<Object> first = CompletableFuture.anyOf(
    cf1, cf2, cf3
);
// Completes when ANY one finishes
```

---

## 8 Exception Handling

```java
CompletableFuture.supplyAsync(() -> {
    if (Math.random() > 0.5) {
        throw new RuntimeException("Failed");
    }
    return "Success";
})
.exceptionally(ex -> {                  // catch errors
    return "Default";
})
.thenAccept(System.out::println);
```

```java
// More control with handle()
cf.handle((result, ex) -> {
    if (ex != null) {
        return "Error: " + ex.getMessage();
    }
    return result;
});
```

---

## 9 Real Production Example

```java
// Fetch user details from 3 sources in parallel

CompletableFuture<UserBasic> basic =
    CompletableFuture.supplyAsync(() -> userService.fetch(id));

CompletableFuture<Orders> orders =
    CompletableFuture.supplyAsync(() -> orderService.fetch(id));

CompletableFuture<Wallet> wallet =
    CompletableFuture.supplyAsync(() -> walletService.fetch(id));

// Combine all 3 when ready
CompletableFuture<UserProfile> profile =
    basic.thenCombine(orders, (b, o) -> new UserProfile(b, o))
         .thenCombine(wallet, (p, w) -> p.withWallet(w));

profile.thenAccept(p -> sendResponse(p))
       .exceptionally(ex -> {
           sendError(ex);
           return null;
       });
```

```
3 services hit in parallel
   Total time = max(3 services), not sum
   = Massive speedup
```

---

## Future vs CompletableFuture Comparison

```
┌──────────────────────┬──────────────┬─────────────────────┐
│  Aspect              │ Future       │ CompletableFuture   │
├──────────────────────┼──────────────┼─────────────────────┤
│ Get result           │ Blocking .get│ Non-blocking + chain│
│ Chain operations     │ Manual     │ thenApply etc.   │
│ Combine futures      │ Manual     │ thenCombine, allOf│
│ Callback             │            │ thenAccept       │
│ Exception handling   │ Try-catch  │ exceptionally    │
│ Manual completion    │            │ complete(value)  │
└──────────────────────┴──────────────┴─────────────────────┘
```

---

## 11 When to Use Which?

```
FUTURE:
   Simple async with one result
   Legacy code

COMPLETABLEFUTURE:
   Multiple async calls to combine
   Chained transformations
   Need async error handling
   Modern production code
   = 99% cases
```

---

## Memory Hooks

```
Future            = food coupon (blocking get)
CompletableFuture = food delivery app (callback + chain)

Create:
   supplyAsync()  → returns value
   runAsync()     → no return
   completedFuture(x) → already done

Chain:
   thenApply  → transform
   thenAccept → consume
   thenRun    → just run

Combine:
   thenCombine → 2 → 1
   allOf       → all done
   anyOf       → first done

Errors:
   exceptionally → catch
   handle        → result OR exception
```

[← Back to JAVA](../../)
