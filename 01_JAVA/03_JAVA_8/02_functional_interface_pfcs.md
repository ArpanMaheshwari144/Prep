# Functional Interface (PFCS)

## Scene

Mart me wahi 3 dabbe hain: ghee 550, aata 320, daal 140. Mausi boli: "300 se mehnge dabbe chuno, unke naam ki parchi banao, aur ek naya dabba cheeni bhi le aao."

Yahan 4 kaam hain, aur Java 8 me har kaam ka apna khaana bana hua hai:

```java
Predicate<Item>        mehnga = i -> i.price() > 300;          // chuno: haan ya naa
Function<Item,String>  naam   = i -> i.name();                 // badlo: dabba -> naam
Consumer<String>       parchi = s -> print("parchi: " + s);    // karo, kuch lautana nahi
Supplier<Item>         naya   = () -> new Item("cheeni", 60);  // banao, lena kuch nahi

l.stream().filter(mehnga).map(naam).forEach(parchi);
```

Chala ke dekha:

```
parchi: ghee
parchi: aata
Supplier se: Item[name=cheeni, price=60]
```

## Dikkat -> hal

Lambda ka apna koi type nahi hota. Use jis khaane me rakho, wahi uska type ban jaata hai. Stream ke method batate hain ki kaunsa khaana chahiye: `filter` ko Predicate chahiye, `map` ko Function, `forEach` ko Consumer, aur `orElseGet` ko Supplier. Ye jodi (kaunsa method kaunsa khaana maangta) yaad rakhne ki cheez hai. Bhool jao to docs ya net pe dekh lo, ye normal hai.

## Prod wala trap

Teammate ne `Bill` interface me ek doosra method `undo` jod diya. Tab interface me 2 khaali khaane ho gaye.

```
@FunctionalInterface nahi tha:  G.java:2: error: Bill is not a functional interface
                                (error har us jagah jahan (p, q) -> p + q likha tha)
@FunctionalInterface tha:       F.java:1: error: Unexpected @FunctionalInterface annotation
                                (error usi ek jagah jahan undo joda)
```

Annotation ek pehredaar hai. Iske bina bhi code chalta hai, par iske saath galti usi line pe pakdi jaati hai jahan hui thi, poore codebase me bikhar ke nahi aati.

## Tera project

Insurance ke `ClaimService` me `.orElseThrow(() -> new EntityNotFoundException(...))` likha hai. Ye `() -> ...` ek Supplier hai. Ye kuch leta nahi, bas exception bana ke deta hai, aur sirf tab chalta hai jab claim na mile.
