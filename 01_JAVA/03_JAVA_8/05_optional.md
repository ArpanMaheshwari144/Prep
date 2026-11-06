# Optional

## Scene

Grahak aaya: "paneer ka daam kya hai?" Mart ke register me paneer hai hi nahi, to `get("paneer")` ne `null` diya. Tune seedha `p.price()` likha tha, to `NullPointerException` aaya aur counter band ho gaya. Har jagah `if (p != null)` lagao to code bhaari ho jaata hai, aur kahin na kahin ye check chhoot bhi jaata hai.

Java 8 ne `null` ki jagah ek dabba de diya. Dabba bhara ho sakta hai ya khaali, aur method ka return type dekh ke hi pata chal jaata hai ki khaali wala case sambhalna padega:

```java
static Optional<Item> dhundho(String n) { return Optional.ofNullable(register.get(n)); }

dhundho("ghee").map(Item::price).orElse(-1);     // dabba bhara hai to daam
dhundho("paneer").map(Item::price).orElse(-1);   // khaali hai to -1
```

Chala ke dekha:

```
ghee: 550
paneer: -1
get(): No value present
```

## Dikkat -> hal

`map` aur `filter` khaali dabbe pe kuch nahi karte, bas khaali dabba aage bhej dete hain. Isliye poori chain me ek bhi null check nahi likhna padta. Par khaali dabbe pe seedha `get()` kiya to `No value present` aata hai, matlab wahi crash, bas naye naam se. Isliye `get()` ki jagah `orElse`, `orElseThrow` ya `map` lagao. Optional return type me lagta hai, parameter ya field me nahi.

## Prod wala trap

Default nikaalne ke liye godown wale ko phone karna padta hai, aur ye mehnga kaam hai:

```java
dhundho("ghee").map(Item::price).orElse(godownSePoocho());
dhundho("ghee").map(Item::price).orElseGet(() -> godownSePoocho());
```

Chala ke dekha:

```
orElse, ghee mila tab bhi:
  (godown wale ko phone kiya)      <- ghee mil gaya tha, phir bhi phone gaya
orElseGet, ghee mila:
                                   <- phone nahi gaya
```

`orElse(x)` me Java pehle `x` banata hai aur phir method chalata hai, isliye phone har baar jaata hai. `orElseGet` ek Supplier leta hai, jo sirf khaali dabbe pe chalta hai. Default sasta ho (jaise `-1`) to `orElse` theek hai. Default mehnga ho (DB ya API call) to `orElseGet` lagao.

## Tera project

Insurance ke `ClaimService` me ye likha hai:

```java
Claim c = claimRepository.findById(id)
        .orElseThrow(() -> new EntityNotFoundException("Claim Not Found " + id));
```

`findById` Optional deta hai. Claim mila to wahi milta hai, nahi mila to saaf exception aata hai, NPE nahi.

## Interview line

```
Optional is a container that may or may not hold a value, used as a return type
to make "not found" explicit instead of returning null.
Use map, orElse or orElseThrow instead of get().
orElse always evaluates its default, orElseGet only when empty,
so use orElseGet when the default is expensive.
```
