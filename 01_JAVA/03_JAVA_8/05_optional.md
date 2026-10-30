# Optional

## Kahani

Doctor ne bola: "patient 101 ka naam do." Tune `findPatient(101)` chalaya. Patient mila hi nahi, `null` aaya, aur seedha `p.getName()` likha tha. Nateeja: `NullPointerException`, prod crash. Har jagah `if (p != null)` lagao to code bhaari ho jaata hai, aur kahin na kahin chhoot bhi jaata hai.

Java 8 ne `null` ki jagah ek **dabba** de diya:

```java
Optional<Patient> p = findPatient(101);

String name = p.map(Patient::getName)    // andar hai to naam nikalo, khaali hai to aage badho
               .orElse("Unknown");        // khaali nikla to default de do
```

## Tasveer

```
Optional.of(x)          [ x ]       value pakki hai (null diya to wahin NPE)
Optional.ofNullable(x)  [ x / -- ]  null ho sakta hai (zyadatar yahi)
Optional.empty()        [ -- ]      pata hai ki khaali hai
```

## 3 khaas baat

1. **Optional bolta hai ki "yahan kuch na mile, aisa ho sakta hai".** Method ka return type `Optional<Patient>` dekhte hi call karne wala samajh jaata hai ki khaali case sambhalna padega. Isliye ye return type me lagta hai. Parameter ya field me nahi lagta.
2. **`get()` seedha mat karo.** Khaali dabbe pe `get()` karoge to `NoSuchElementException` aayega, yaani wahi crash bas naye naam se. Iski jagah `orElse`, `orElseThrow` ya `map` lagao.
3. **`map` aur `filter` khaali dabbe pe kuch nahi karte, khaali dabba aage bhej dete hain.** Isliye poori chain me ek bhi null check nahi likhna padta.

## Prod wala trap

Pichhli file ka Supplier yahan kaam aata hai:

```java
p.orElse(dbSeDefaultLao());            // dabba bhara ho tab bhi DB call chali
p.orElseGet(() -> dbSeDefaultLao());   // sirf khaali hone pe chalegi
```

`orElse(x)` me Java pehle `x` banata hai, phir method chalata hai. Isliye DB call har baar hoti hai. `orElseGet` ek Supplier leta hai, jo sirf khaali dabbe pe chalta hai. Default sasta ho (`"Unknown"`) to `orElse` theek hai. Default mehnga ho (DB / API) to `orElseGet` lagao.

## Tera project

InsuranceManagementSystem `ClaimService`:

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
