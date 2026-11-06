# HashSet: andar se

## Tere STAR se (Email dedup, STAR 10)

10,000 panelists the, aur kuch ne ek hi email do baar register kar diya tha. Agar ArrayList lete, to dono entry andar
chali jaati. HashSet me duplicate apne aap ruk jaata hai.

## Andar ka raaz: HashSet asal me HashMap hi hai

HashSet me jo element tu daalta hai, wo andar ek HashMap ki key ban jaata hai, aur value me ek dummy object rakh diya
jaata hai.

```java
set.add("arpan@gmail.com")  =  map.put("arpan@gmail.com", PRESENT)
// dobara add kiya -> key pehle se hai -> nayi entry nahi bani -> add() = false
```

Duplicate rokne ke liye hashCode aur equals chahiye. HashMap ye kaam pehle se karta hai, isliye Java ne HashSet ke liye
naya dabba banaya hi nahi, HashMap ko hi andar use kar liya.

## File 01 se juda trap

Maan lo teri apni class `Panelist` me `hashCode` override nahi kiya. To same email wale do object alag-alag khaano me
chale jaate hain, `size() = 2` aata hai, aur duplicate andar ghus jaata hai. Isliye equals aur hashCode dono override
karo, aur dono email pe hi bane hone chahiye.

## Asli prod trap (1-Oct chala ke dekha): add karna update nahi hota

Set ke liye do Panelist tab "same" hain jab unka email same hai, kyunki equals aur hashCode email pe bane hain. Naam ko
set dekhta hi nahi.

1. Arpan ne register kiya: email `arpan@gmail.com`, naam "Arpan". Wo set me chala gaya.
2. Kal usne naam badal ke "Arpan M" kar diya. Code ne naya Panelist banaya (wahi email, naya naam) aur `set.add()` kar
   diya, ye soch ke ki update ho jaayega.
3. Set ne dekha ki is email wala pehle se andar hai, isliye naya object liya hi nahi aur `false` lauta diya.
4. Set me ab bhi purana naam "Arpan" hi hai. Naya naam kahin nahi gaya, aur koi error bhi nahi aaya.

Isko gate ke register jaisa samjho. Arpan andar ja chuka hai. Ab wo naya ID card leke dobara aaya, to guard ne bola "tu
to andar hai" aur card wapas kar diya. Andar ab bhi purane card wala Arpan hi hai.

Ilaaj: pehle `set.remove(purana)` karo, phir `set.add(naya)`. Ya phir Set ki jagah `Map<email, Panelist>` lo, kyunki
map me `put` sach me purani value badal deta hai.

## Trade-off

HashSet me add aur contains O(1) hain, par koi kram nahi milta. Agar daalne ka kram chahiye to LinkedHashSet lo, aur
sorted chahiye to TreeSet lo, jo O(log n) leta hai.
