# fail-fast vs fail-safe

Ek thread wala hissa (modCount kya hai, loop me remove karne pe kya hota, aur S102 hatane pe loop chup-chaap kaise skip
karta) [05_iterator_vs_listiterator.md](05_iterator_vs_listiterator.md) me hai. Yahan sirf nayi baat hai: jab do thread
ek hi list ko chhoote hain.

## Kahani: do thread, ek list

Ek thread chaalu sessions ki list pe loop chala raha hai, kyunki wo report bana raha hai. Usi waqt Tomcat ka doosra
thread usi list me ek naya session jod deta hai. Nateeja ye hai ki pehle thread ko CME aa jaata hai.

```
fail-fast (ArrayList, HashMap)
  -> badlaav dikha to turant CME, kyunki galat data dene se achha exception dena hai

fail-safe:
  CopyOnWriteArrayList   -> har likhai pe POORI nayi copy banti hai; loop purani copy (snapshot) pe chalta, CME kabhi nahi
  ConcurrentHashMap      -> copy NAHI banti; loop asli map pe chalta, naya badlaav dikh bhi sakta, nahi bhi (weakly consistent)
```

## Trade-off

CopyOnWriteArrayList me padhna safe bhi hai aur tez bhi, par har `add` pe poori list ki copy banti hai. Isliye ye wahan
theek hai jahan padhna bahut hota hai aur likhna kabhi-kabhar, jaise event listeners ki list. Agar list me har second
kuch judta hai, to ye memory kha jaayega.

## Prod trap

Ye CME local pe kabhi nahi aata, kyunki local pe do request ek saath aati hi nahi. Prod pe bheed me ye kabhi-kabhar aata
hai, aur log me ek random stack trace dikhta hai jo dobara karne pe hota hi nahi. Isliye jab bhi koi list kai thread me
baanti hui dikhe, to use ArrayList mat rehne do.
