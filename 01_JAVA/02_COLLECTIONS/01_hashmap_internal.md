# HashMap: andar kaise kaam karta hai

## Tere code se shuru (H6)

```java
cache.put(new Employee(101), salary);
cache.get(new Employee(101));   // null
```

Same id 101, phir bhi "mila hi nahi". Kyun? Isi sawaal ka jawab poora HashMap hai.

## Kahani: 16 khaane ki almari

Key aati hai -> `hashCode` ek number banata hai -> `(16-1) & hash` se khaana tay. Har key ka khaana pehle se pata,
isliye dhoondhna seedha O(1), list ki tarah ek-ek dekhna nahi.

```
put(e101)      -> hashCode -> khaana 5  -> [5]: e101
get(naya e101) -> hashCode ALAG -> khaana 12 -> khaali -> null
```

Employee me `hashCode` likha hi nahi tha, to Java ne object ki pehchaan se number banaya. Do `new` = do number = do
khaane. `get` galat khaane me gaya, `equals` tak baat pahunchi hi nahi.

## Dikkat: do key ek khaane me

Dono ek chain me lagti hain, chain ke andar sahi key `equals` se pehchani jaati hai.
`hashCode` = kaunsa khaana, `equals` = khaane ke andar kaunsi. Dono saath override.

## Trade-off

75% bharte hi almari dugni (load factor 0.75). Jaldi dugni = memory bekaar, der se = chain lambi, map dheema. Koi
chain 8 se lambi (aur table 64+) to wo tree ban jaati, bheed me bhi O(log n).

## Prod trap

Cache kabhi hit nahi hota, har baar DB call. Koi error nahi, bas sab dheema. Code review me `equals` dikh jaata, sab
theek lagta. Set me aur bura: `set.add(new Employee(101))` do baar -> `size() = 2`.

## Tera hands-on (khud likha, chala ke dekha)

[H1 index](demo/hashmap/H1_IndexKaise.java) · [H2 takkar](demo/hashmap/H2_Takkar.java) ·
[H3 spread](demo/hashmap/H3_Spread.java) · [H4 bucket](demo/hashmap/H4_Bucket.java) ·
[H5 == vs equals](demo/hashmap/H5_Get.java) · [H6 contract](demo/hashmap/H6_Contract.java)

## Interview line

```
HashMap picks a bucket from the key's hashCode and finds the exact key inside the bucket with
equals, so both must be overridden together; I have seen a cache that never hit because a key class
had equals but no hashCode.
```
