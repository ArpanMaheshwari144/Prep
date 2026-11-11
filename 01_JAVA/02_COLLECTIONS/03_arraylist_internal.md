# ArrayList: andar se

## Scene: chaalu survey IDs

Manager ne bola ki jo survey abhi chal rahe hain, unki IDs ki list rakho. Tune `int[100]` liya. Jaise hi 101vi ID aayi,
jagah khatam ho gayi. ArrayList isi dikkat se bana hai: andar array hi hai, par bharne pe khud badh jaata hai.

## Chala ke dekha: andar ki jagah kab badhti hai

```
new ArrayList  -> jagah 0
add #1  -> jagah 10
add #11  -> jagah 15
add #16  -> jagah 22
add #23  -> jagah 33
```

- `new ArrayList<>()` ke waqt andar khaali array hai, abhi koi memory nahi li.
- Pehla add hote hi 10 ki jagah banti hai.
- Bharne pe 1.5 guna (10 -> 15 -> 22 -> 33) ka naya array banta hai, aur purana poora usme copy hota hai.
- `get(5)` seedha index pe jaata hai, isliye O(1).
- `add(1, 99)` pe uske baad ke sab element ek jagah khiskte hain, isliye O(n).

## Trade-off

Index se padhna aur end pe jodna tez hai. Beech me daalna ya beech se hatana dheema hai, kyunki baaki sab element
khiskane padte hain. Har baar bharne pe poora array copy hota hai, ye badhne ka kharcha hai.

Pehle se pata ho ki 5000 element aayenge, to `new ArrayList<>(5000)` likho. Isse baar-baar copy nahi hogi.

## Prod trap: remove(5)

```java
List<Integer> surveyIds = new ArrayList<>(List.of(101, 205, 5));
surveyIds.remove(5);
```

```
remove(5): IndexOutOfBoundsException: Index 5 out of bounds for length 3
remove(Integer.valueOf(5)) -> [101, 205]
```

`remove(int)` index hatata hai, `remove(Object)` value hatata hai. Integer ki list me `5` likha to Java ne use index
samjha. Value hatani ho to `remove(Integer.valueOf(5))`. Review me ye aankh se nikal jaata hai, kyunki line dekhne me
bilkul theek lagti hai.

## Thread wala hissa (file 02 se juda)

ArrayList thread-safe nahi hai. List kai thread me baantni ho to do raaste:
- `Collections.synchronizedList`: har call pe ek taala.
- `CopyOnWriteArrayList`: har likhai pe poori copy, isliye tabhi theek jab padhna bahut ho aur likhna kam.

Vector purana tareeka hai (har method synchronized). Aaj ke code me Vector nahi lete.
