# fail-fast vs fail-safe

Ek thread wala hissa (modCount kya hai, Java ne aisa kyun banaya, loop me remove pe kya hota, S102 pe chup skip)
[05_iterator_vs_listiterator.md](05_iterator_vs_listiterator.md) me hai. Yahan sirf nayi baat: jab DO THREAD ek hi list
ko chhoote hain.

## Scene: do thread, ek list

Ek thread chaalu sessions ki list pe loop chala raha hai, kyunki report bana raha hai. Usi waqt Tomcat ka doosra thread
usi list me naya session jod deta hai.

## Chala ke dekha: report thread loop chalata raha, doosra thread 2000 session jodta raha

```
ArrayList           : report loop 111 baar, CME 108 baar
CopyOnWriteArrayList: report loop 110 baar, CME 0 baar
```

- **fail-fast (ArrayList, HashMap):** badlaav dikha to turant CME. Galat data dene se achha exception (wahi register wala
  alarm, file 05).
- **fail-safe (CopyOnWriteArrayList):** CME kabhi nahi.

## Do alag fail-safe, do alag tareeke

```
COW loop ne dekha: [s1, s2, s3], list ab: [s1, s2, s3, s4]
CHM loop ne dekha: [s3, s1, s2], map ab: [s3, s4, s1, s2]
```

- **CopyOnWriteArrayList:** har likhai pe POORI nayi copy banti hai. Loop purani copy (snapshot) pe chalta hai, isliye
  loop ke beech joda s4 loop ko dikha hi nahi, par list me aa gaya.
- **ConcurrentHashMap:** copy NAHI banti. Loop asli map pe chalta hai, beech ka badlaav dikh bhi sakta hai, nahi bhi
  (weakly consistent). Is run me s4 nahi dikha, kisi aur run me dikh sakta hai.

## Trade-off

CopyOnWriteArrayList me padhna safe bhi aur tez bhi, par har `add` pe poori list ki copy. Isliye wahan theek jahan padhna
bahut aur likhna kabhi-kabhar, jaise event listeners ki list. Har second kuch judta ho to memory kha jaayega.

## Prod trap

Ye CME local pe kabhi nahi aata, kyunki local pe do request ek saath aati hi nahi. Prod pe bheed me kabhi-kabhar aata
hai, aur log me ek random stack trace dikhta hai jo dobara karne pe hota hi nahi. Upar 111 me se 108 baar aaya kyunki
doosra thread lagaataar likh raha tha; asli prod me ye kabhi-kabhar hota hai, isliye aur khatarnak.

Jab bhi koi list kai thread me baanti dikhe, use ArrayList mat rehne do.
