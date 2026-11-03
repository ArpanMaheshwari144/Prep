# HashSet: andar se

## Tere STAR se (Email dedup, STAR 10)

10,000 panelists, kuch ne ek email do baar register kiya. ArrayList me dono chale jaate; HashSet me duplicate apne
aap ruk jaata.

## Andar ka raaz: HashSet = HashMap

Element = map ki key, value = ek dummy object.

```java
set.add("arpan@gmail.com")  =  map.put("arpan@gmail.com", PRESENT)
// dobara add -> key pehle se -> naya entry nahi -> add() = false
```

Duplicate rokne ko hashCode + equals chahiye, HashMap ye pehle se karta, to Java ne naya dabba nahi banaya.

## File 01 se juda trap

Apni class `Panelist` me `hashCode` override nahi -> same email wale do object alag khaano me -> `size() = 2`,
duplicate ghus gaya. equals + hashCode dono, email pe.

## Asli prod trap (1-Oct chala ke dekha): add != update

Set ke liye "same" = same email (equals / hashCode email pe), naam dekhta hi nahi.

1. Arpan register: email `arpan@gmail.com`, naam "Arpan" -> set me.
2. Kal naam "Arpan M" kiya. Code ne naya Panelist (wahi email, naya naam) bana ke `set.add()` kiya, socha update hoga.
3. Set: "is email wala pehle se andar" -> naya object liya hi nahi, `false`.
4. Set me ab bhi purana naam "Arpan". Naya naam kahin nahi gaya, koi error bhi nahi.

Gate pe register jaisa: Arpan andar ja chuka. Naya ID card leke dobara aaya to guard bola "tu andar hai", card wapas.
Andar purane card wala hi.

Ilaaj: `set.remove(purana)` phir `set.add(naya)`, ya `Map<email, Panelist>` (`put` sach me badalta).

## Trade-off

HashSet add / contains O(1), order nahi. Kram = LinkedHashSet, sorted = TreeSet (O(log n)).

## Interview line

```
HashSet is just a HashMap whose keys are the elements and whose values are a dummy object, so
duplicate detection comes from hashCode and equals; in an email dedup tool this means the key class
must override both, and a duplicate add keeps the first object and returns false.
```
