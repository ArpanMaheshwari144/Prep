# Streams API

## Kahani

Manager bola: "60 se upar wale patients chuno, unke naam nikalo, A-Z sort karo, pehle 5 do."

Loop se karo to pehle naya list, phir `for`, uske andar `if`, phir `add`, aur end me `Collections.sort`. Saare kaam ek hi loop me ghuse hote hain. Stream me har kaam ek station hai:

```java
List<String> top5 = patients.stream()       // belt pe daalo
        .filter(p -> p.getAge() > 60)        // station 1: chuno
        .map(Patient::getName)               // station 2: naam nikalo
        .sorted()                            // station 3: A-Z
        .limit(5)                            // station 4: pehle 5
        .collect(Collectors.toList());       // maal utha lo
```

## Tasveer

Stream ek conveyor belt hai. Har item ek-ek karke **poori line** guzarta hai. Pehle saare items filter nahi hote, phir saare map nahi hote.

```
patient1:  filter -> map -> ...
patient2:  filter -> map -> ...
            (station)  (station)          collect = belt ke end pe maal uthane wala
```

## 3 khaas baat

1. **Belt tabhi chalti hai jab end pe koi maal uthane wala ho.** `filter`, `map` aur `sorted` sirf naksha banate hain (intermediate). `collect`, `count` ya `forEach` (terminal) aaye tabhi kuch chalta hai. Terminal na ho to `filter` ke andar ka `println` ek baar bhi nahi chhapega.
2. **Item ek-ek karke poori line guzarta hai, isliye jawab milte hi belt ruk jaati hai.** `findFirst()`, `anyMatch()` ya `limit(5)` lagao to baaki items chhue bhi nahi jaate. `sorted()` iska apvaad hai: sort karne ke liye use saare items pehle jama karne padte hain.
3. **Ek stream ek hi baar chalti hai.** Wahi stream dobara use karo to `IllegalStateException` aata hai. Har baar `list.stream()` se nayi stream banao.

## Prod wala trap

```java
orders.parallelStream()
      .map(o -> paymentApi.check(o))   // har item pe API call (rukne wala kaam)
      .collect(...);
```

Saari `parallelStream` poori JVM me ek hi chhota thread pool share karti hain (common ForkJoinPool, size = cores - 1). Usme API ya DB jaisa rukne wala kaam daala to pool ke saare thread intezaar me baith jaate hain, aur app ki baaki har `parallelStream` bhi atak jaati hai. Ye wahi thread pool ki kahani hai jo multithreading me dekhi thi. `parallelStream` sirf tab lagao jab data bada ho, kaam CPU wala ho aur items ek-doosre se alag hon.

## Tera project

SimpleBankSystem `AccountService.getAccountHolderNames()`:

```java
return repository.findAll()          // source
        .stream()
        .map(Account::getHolderName) // intermediate
        .collect(Collectors.toList()); // terminal
```

## Interview line

```
A stream is a lazy pipeline: intermediate operations like filter and map only build it,
and nothing runs until a terminal operation like collect or count.
Elements flow one by one, so findFirst or limit can stop early.
A stream can be used only once, and parallelStream uses the shared common pool,
so it should not be used for blocking calls.
```
