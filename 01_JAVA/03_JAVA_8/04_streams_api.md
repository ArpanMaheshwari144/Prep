# Streams API

## Scene

Mart me 4 dabbe hain: ghee 550, daal 140, aata 320, chai 400. Mausi boli: "300 se mehnge dabbe chuno aur unke naam do."

Loop se karo to naya list banao, `for` lagao, uske andar `if`, phir `add`. Saare kaam ek hi loop me ghuse rehte hain. Stream me har kaam ek alag station hai, aur dabbe ek belt pe chalte hain:

```java
Stream<String> s = l.stream()
        .filter(i -> { print("filter " + i.name()); return i.price() > 300; })  // station: chuno
        .map(Item::name);                                                         // station: naam
// abhi tak terminal nahi lagaya
s.collect(Collectors.toList());                                                   // maal uthao
```

Chala ke dekha:

```
== terminal nahi ==
                         <- kuch nahi chhapa, belt chali hi nahi
== ab collect ==
filter ghee
filter daal
filter aata
filter chai
[ghee, aata, chai]
```

## Dikkat -> hal

`filter` aur `map` sirf naksha banate hain, ye intermediate hain. Belt tabhi chalti hai jab end pe koi maal uthane wala ho, jaise `collect`, `count` ya `forEach` (ye terminal hain). Aur har dabba ek-ek karke poori line se guzarta hai, isliye jawab milte hi belt ruk jaati hai:

```
== findFirst ==
dekha ghee
mila ghee            <- daal, aata, chai ko chhua bhi nahi
```

`sorted()` is niyam ka apvaad hai, kyunki sort karne ke liye use saare dabbe pehle jama karne padte hain. Aur ek stream ek hi baar chalti hai. Wahi `s` dobara chalaya to ye aaya:

```
stream has already been operated upon or closed
```

## Prod wala trap

```java
orders.parallelStream()
      .map(o -> paymentApi.check(o))   // har order pe API call, rukne wala kaam
      .collect(...);
```

Saari `parallelStream` poori JVM me ek hi chhota thread pool share karti hain. Uska naam common ForkJoinPool hai aur size cores - 1 hota hai. Usme API ya DB jaisa rukne wala kaam daala to pool ke saare thread intezaar me baith jaate hain, aur app ki baaki har `parallelStream` bhi atak jaati hai. Ye wahi thread pool wali kahani hai jo multithreading me dekhi thi. `parallelStream` sirf tab lagao jab data bada ho, kaam CPU wala ho aur items ek-doosre se alag hon.

## Tera project

SimpleBankSystem ke `getAccountHolderNames()` me yahi belt lagi hai. `findAll()` se dabbe aaye, `map(Account::getHolderName)` beech ka station hai, aur `collect` end pe maal uthata hai.
