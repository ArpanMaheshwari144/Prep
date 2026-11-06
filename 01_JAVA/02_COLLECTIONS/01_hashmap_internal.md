# HashMap: andar kaise kaam karta hai

## Tere code se shuru (H6)

```java
cache.put(new Employee(101), salary);
cache.get(new Employee(101));   // null
```

Id dono baar same hai, 101. Phir bhi `get` bolta hai ki "mila hi nahi" aur null de deta hai. Aisa kyun hua, isi sawaal ka
jawab poora HashMap hai.

## Kahani: 16 khaane ki almari

HashMap ko ek almari samjho jisme 16 khaane hain. Jab koi key aati hai, to pehle uska `hashCode` ek number banata hai.
Phir `(16-1) & hash` se tay hota hai ki ye key kaunse khaane me jaayegi. Matlab har key ka khaana pehle se pata hota hai,
isliye dhoondhna seedha O(1) hai. List ki tarah ek-ek element dekhna nahi padta.

```
put(e101)      -> hashCode banaya -> khaana 5 mila  -> khaana [5] me rakh diya e101
get(naya e101) -> hashCode ALAG bana -> khaana 12 dekha -> wo khaali hai -> null
```

Tere `Employee` me `hashCode` likha hi nahi tha, to Java ne object ki apni pehchaan (memory wali) se number bana diya.
Tune do baar `new` likha, to do alag object bane, do alag number bane, aur dono alag khaano me gaye. `get` galat khaane
me dhoondhne gaya, isliye baat `equals` tak pahunchi hi nahi.

## Dikkat: do key ek hi khaane me aa gayi

Kabhi-kabhi do alag key ka khaana same nikalta hai. Tab dono us khaane me ek chain me lag jaati hain, aur chain ke andar
sahi key `equals` se pehchani jaati hai.

Isko aise yaad rakh: `hashCode` batata hai ki kaunsa khaana, aur `equals` batata hai ki khaane ke andar kaunsi key.
Isliye dono ko hamesha saath me override karna hota hai.

## Trade-off

Jaise hi almari 75% bhar jaati hai, wo apne aap dugni ho jaati hai. Isi ko load factor 0.75 kehte hain. Agar jaldi dugni
karein to memory bekaar jaati hai, aur agar der se karein to chain lambi ho jaati hai aur map dheema pad jaata hai.

Agar koi chain 8 se lambi ho jaaye (aur table 64 ya usse bada ho), to wo chain tree ban jaati hai. Isse bheed wale khaane
me bhi dhoondhna O(log n) me ho jaata hai.

## Prod trap

Maan lo cache ki key class me `hashCode` nahi likha. To cache kabhi hit hi nahi hota, aur har baar DB call jaati hai.
Koi error nahi aata, bas sab dheema chalta hai. Code review me `equals` likha hua dikh jaata hai, isliye sab theek lagta
hai aur galti pakdi nahi jaati.

Set me ye aur bura hai. Agar tune `set.add(new Employee(101))` do baar kiya, to `size() = 2` aata hai, matlab duplicate
andar ghus gaya.

## Tera hands-on (khud likha, chala ke dekha)

[H1 index](demo/hashmap/H1_IndexKaise.java) · [H2 takkar](demo/hashmap/H2_Takkar.java) ·
[H3 spread](demo/hashmap/H3_Spread.java) · [H4 bucket](demo/hashmap/H4_Bucket.java) ·
[H5 == vs equals](demo/hashmap/H5_Get.java) · [H6 contract](demo/hashmap/H6_Contract.java)
