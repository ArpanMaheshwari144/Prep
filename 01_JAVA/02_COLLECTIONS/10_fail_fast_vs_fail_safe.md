# fail-fast vs fail-safe

> Ek thread ka hissa (modCount, loop me remove, S102 chup-chaap skip) = [05_iterator_vs_listiterator.md](05_iterator_vs_listiterator.md).
> Yahan sirf naya: do thread, ek list.

## Kahani: do thread, ek list

Ek thread active sessions ki list pe loop (report bana raha), usi waqt Tomcat ka doosra thread usi list me naya
session jodta -> pehle thread ko CME.

```
fail-fast (ArrayList, HashMap)  -> badlaav dikha to turant CME: "galat data se achha exception"
fail-safe:
  CopyOnWriteArrayList   -> har likhai pe POORI nayi copy; loop purani copy (snapshot) pe, CME kabhi nahi
  ConcurrentHashMap      -> copy NAHI; asli map pe chalta, naya badlaav dikh bhi sakta, nahi bhi (weakly consistent)
```

## Trade-off

CopyOnWriteArrayList: padhna safe + tez, par har `add` pe poori copy. Theek wahan jahan padhna bahut, likhna
kabhi-kabhar (event listeners). Har second judne wali list pe memory kha jaayega.

## Prod trap

Ye CME local pe kabhi nahi aata (do request ek saath aati hi nahi). Prod pe bheed me kabhi-kabhar, log me random
stack trace jo dobara karne pe hota hi nahi. List kai thread me baanti dikhe = ArrayList mat rehne do.

## Interview line

```
ArrayList and HashMap iterators are fail-fast: if the collection changes underneath, the next call
throws ConcurrentModificationException instead of returning wrong data. For a list shared across
threads that is read far more than written, like listeners, I use CopyOnWriteArrayList, which
iterates over a snapshot; ConcurrentHashMap walks the live map and is only weakly consistent.
```
