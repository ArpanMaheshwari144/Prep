# HashMap vs LinkedHashMap vs TreeMap

## Teeno tere kaam me aa chuke

- **HashMap:** userId se naam. Sabse tez, order ki koi guarantee nahi.
- **LinkedHashMap:** tera LRU cache. HashMap + doubly linked list. `new LinkedHashMap<>(16, 0.75f, true)` = access ke
  kram me, `removeEldestEntry` se sabse purana apne aap bahar. DSA wala LRU Java me 5 line.
- **TreeMap:** leaderboard. Keys hamesha sorted (andar Red-Black tree, file 01 wala). Asli fayda = range wale sawaal:

```java
scores = {85=Rahul, 92=Priya, 100=Arpan}
scores.floorKey(90)    // 85  (90 se chhota ya barabar sabse bada)
scores.ceilingKey(90)  // 92
scores.subMap(85, 95)  // {85=Rahul, 92=Priya}
```

## Trade-off

HashMap O(1) bina order · LinkedHashMap O(1) + thodi memory, kram rakhta · TreeMap O(log n), sort ki keemat.
95% jagah HashMap.

## Prod trap (1-Oct chala ke dekha)

```java
map.put(103,"Priya"); map.put(101,"Arpan"); map.put(102,"Rahul");
// HashMap print: {101, 102, 103}  <- SANYOG se sorted
```

Chhote Integer ka hashCode = wahi number, to kram se khaano me gire. Kisi ne maan liya "HashMap sorted deta", test
pass. Key String hui ya map bada hua, order bikhra. Report / API ka order chahiye = LinkedHashMap ya TreeMap.

## Null

HashMap, LinkedHashMap = ek null key. TreeMap = null key pe NPE (compare karna padta), jab tak `Comparator.nullsFirst`
na do. TreeMap sort KEY pe karta, value pe nahi.

## Interview line

```
I pick the map by the order I need: HashMap for plain O(1) lookups, LinkedHashMap when insertion or
access order matters, like an LRU cache with accessOrder true, and TreeMap for sorted keys and range
queries like floorKey on a leaderboard, at O(log n).
```
