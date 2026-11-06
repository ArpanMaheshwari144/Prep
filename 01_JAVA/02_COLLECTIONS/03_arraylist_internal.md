# ArrayList: andar se

## Kahani: chaalu survey IDs

Manager ne bola ki jo survey abhi chal rahe hain, unki IDs ki list rakho. Tune `int[100]` liya. Jaise hi 101vi ID aayi,
jagah khatam ho gayi. ArrayList isi dikkat se bana hai. Uske andar bhi array hi hota hai, par wo bharne pe khud badh
jaata hai.

```
new ArrayList<>()   -> andar {} khaali array (Java 8+ me, abhi koi memory nahi li)
pehla add           -> ab 10 ki jagah banti hai
11va add            -> 15 ka naya array banta hai (1.5x), purana usme copy hota hai
get(5)              -> seedha index pe jaata hai, isliye O(1)
add(1, 99)          -> uske baad ke sab element ek jagah khiskte hain, isliye O(n)
```

## Kis dikkat se bana

Array ka size fix hota hai, jabki ArrayList khud badh jaata hai. Iska kharcha ye hai ki jab bhi wo bharta hai, poora
array nayi jagah copy karna padta hai.

## Trade-off

Index se padhna aur end pe jodna tez hai. Par beech me daalna ya beech se hatana dheema hai, kyunki baaki sab element
khiskane padte hain.

Agar pehle se pata hai ki 5000 element aayenge, to `new ArrayList<>(5000)` likho. Isse baar-baar copy nahi hogi.

## Prod trap

```java
List<Integer> surveyIds = new ArrayList<>(List.of(101, 205, 5));
surveyIds.remove(5);   // value 5 nahi hati, INDEX 5 hatane gaya -> IndexOutOfBounds
```

`remove(int)` index hatata hai, aur `remove(Object)` value hatata hai. Integer ki list me `5` likha to Java ne use index
samjha. Isliye agar value hatani ho, to `remove(Integer.valueOf(5))` likho. Review me ye aankh se nikal jaata hai,
kyunki line dekhne me bilkul theek lagti hai.

## Thread wala hissa (file 02 se juda)

ArrayList thread-safe nahi hai. Agar list kai thread me baantni ho, to do raaste hain. Pehla `Collections.synchronizedList`,
jo har call pe ek taala lagata hai. Doosra `CopyOnWriteArrayList`, jo har likhai pe poori copy banata hai, isliye wo
tabhi theek hai jab padhna bahut ho aur likhna kam.

Vector purana tareeka hai, jisme har method synchronized hota hai. Aaj ke code me Vector nahi lete.
