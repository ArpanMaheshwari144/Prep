# HashSet: andar se

## Scene: tera STAR (Email dedup, STAR 10)

10,000 panelists the, aur kuch ne ek hi email do baar register kar diya tha. ArrayList lete to dono entry andar chali
jaati. HashSet me duplicate apne aap ruk jaata hai.

## Andar ka raaz: HashSet asal me HashMap hi hai

Jo element tu daalta hai, wo andar ek HashMap ki KEY ban jaata hai, aur value me ek dummy object (`PRESENT`) rakh diya
jaata hai.

```java
set.add("arpan@gmail.com")  =  map.put("arpan@gmail.com", PRESENT)
```

```
add arpan@gmail.com  -> true
dobara add           -> false, size 1
```

Dobara add kiya to key pehle se thi, nayi entry nahi bani, `add()` ne false lautaya. Duplicate rokne ke liye hashCode aur
equals chahiye, aur HashMap ye kaam pehle se karta hai. Isliye Java ne HashSet ke liye naya dabba banaya hi nahi.

## File 01 se juda trap

Apni class `Panelist` me `hashCode` override nahi kiya to same email wale do object alag khaano me jaate hain, `size() = 2`,
aur duplicate andar ghus jaata hai. equals aur hashCode dono override karo, aur dono email pe hi bane hon.

## Asli prod trap: add karna update nahi hota

Set ke liye do Panelist tab "same" hain jab email same hai (equals / hashCode email pe bane hain). Naam ko set dekhta hi
nahi.

1. Arpan ne register kiya: `arpan@gmail.com`, naam "Arpan". Set me chala gaya.
2. Kal naam badal ke "Arpan M" kiya. Code ne naya Panelist banaya (wahi email, naya naam) aur `set.add()` kar diya, ye
   soch ke ki update ho jaayega.
3. Set ne dekha ki is email wala pehle se andar hai, to naya object liya hi nahi.

```
naya naam 'Arpan M' add -> false, set me: [Arpan]
Map put se            -> [Arpan M]
```

Set me ab bhi purana naam "Arpan" hai, aur koi error nahi aaya.

Gate ke register jaisa samjho: Arpan andar ja chuka hai. Naya ID card leke dobara aaya, guard ne bola "tu to andar hai"
aur card wapas kar diya. Andar ab bhi purane card wala Arpan hai.

Ilaaj:
- Pehle `set.remove(purana)`, phir `set.add(naya)`.
- Ya Set ki jagah `Map<email, Panelist>`. Map me `put` sach me purani value badal deta hai (upar output).

## Trade-off

HashSet me add aur contains O(1), par koi kram nahi. Daalne ka kram chahiye to LinkedHashSet, sorted chahiye to TreeSet
(O(log n)):

```
TreeSet               -> [arpan@x, kabir@x, zoya@x]
```
