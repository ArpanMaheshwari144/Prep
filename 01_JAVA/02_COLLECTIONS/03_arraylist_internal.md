# ArrayList: andar se

## Kahani: active survey IDs

Manager ne bola active survey IDs ki list rakho. `int[100]` liya, 101vi ID aayi, jagah khatam. ArrayList isi dikkat se
bana: andar array hi hai, par khud badhta hai.

```
new ArrayList<>()   -> andar {} khaali (Java 8+, koi memory nahi)
pehla add           -> 10 ki jagah
11va add            -> naya array 15 (1.5x), purana copy
get(5)              -> seedha index -> O(1)
add(1, 99)          -> baaki sab ek khiske -> O(n)
```

## Kis dikkat se bana

Array ka size fix, ArrayList khud badhta. Kharcha: bharte hi poori copy.

## Trade-off

Padhna aur end pe jodna tez, beech me daalna / hatana dheema (sab khiskana). Pehle se pata ho 5000 aayenge to
`new ArrayList<>(5000)`, baar-baar copy nahi.

## Prod trap

```java
List<Integer> surveyIds = new ArrayList<>(List.of(101, 205, 5));
surveyIds.remove(5);   // value 5 nahi hati, INDEX 5 -> IndexOutOfBounds
```

`remove(int)` = index, `remove(Object)` = value. Integer list pe value hatani ho to `remove(Integer.valueOf(5))`.
Review me aankh se nikal jaata.

## Thread wala hissa (file 02 se juda)

ArrayList thread-safe nahi. Kai thread me baantni ho: `Collections.synchronizedList` (har call pe ek taala) ya
`CopyOnWriteArrayList` (har likhai pe poori copy, tabhi jab padhna bahut, likhna kam). Vector = purana, har method
synchronized, aaj nahi.

## Interview line

```
ArrayList is a resizable array: get by index is O(1), adding at the end is amortised O(1), and
inserting in the middle is O(n) because everything shifts. When I know the size up front I pass
the capacity to avoid repeated copying.
```
