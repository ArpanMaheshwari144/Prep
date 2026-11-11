# HashMap: andar kaise kaam karta hai

## Scene: tere project ka employee cache

```java
cache.put(new Employee(101), salary);
cache.get(new Employee(101));   // null
```

Id dono baar 101 hai, phir bhi `get` null deta hai. Isi sawaal ka jawab poora HashMap hai.

## Bina hashCode chalao (sirf `equals` likha)

```
498931366 2060468723 equals=true
get = null
set size = 2
```

`equals` true de raha hai, phir bhi `get` null aaya, aur Set me duplicate ghus gaya.

## Kyun: 16 khaane ki almari

HashMap ek almari jaisa hai jisme 16 khaane hain. Key ka `hashCode` ek number banata hai, aur `(16-1) & hash` se tay
hota hai ki key kaunse khaane me jaayegi. Khaana pehle se pata hai, isliye dhoondhna seedha O(1) hai, list ki tarah
ek-ek element nahi dekhna padta.

```
put(e101)      -> hashCode banaya -> khaana 5 mila   -> khaana [5] me rakh diya
get(naya e101) -> hashCode ALAG   -> khaana 12 dekha -> khaali -> null
```

- `hashCode` likha nahi tha, isliye Java ne har object ki memory wali pehchaan se alag number bana diya.
- Do baar `new` likha, to do alag object, do alag number, do alag khaane.
- `get` galat khaane me dhoondhne gaya, isliye baat `equals` tak pahunchi hi nahi.

Yaad rakh: **`hashCode` batata hai kaunsa khaana, `equals` batata hai khaane ke andar kaunsi key.** Isliye dono saath
me likhne padte hain.

## Tera H6 (dono likhe hue), asli output

```
132
132
true
A
```

Number same aaya, khaana same mila, `equals` true hua aur `get` ne A diya.

## Chhoti dikkat: do key ek hi khaane me

Kabhi do alag key ka khaana same nikalta hai. Tab dono us khaane me ek chain me lag jaati hain, aur chain ke andar sahi
key `equals` se pehchani jaati hai.

- Almari 75% bhar jaaye to apne aap dugni ho jaati hai (load factor 0.75). Jaldi dugni karo to memory bekaar, der se
  karo to chain lambi aur map dheema.
- Koi chain 8 se lambi ho jaaye (aur table 64 ya usse bada ho), to wo chain tree ban jaati hai. Tab bheed wale khaane
  me bhi dhoondhna O(log n) me hota hai.

## Prod trap

Cache ki key class me `hashCode` bhool gaye to cache kabhi hit nahi hota aur har baar DB call jaati hai. Koi error nahi
aata, bas system dheema ho jaata hai. Code review me `equals` likha dikh jaata hai, isliye sab theek lagta hai aur galti
pakdi nahi jaati.

Set me ye aur bura hai: `set.add(new Employee(101))` do baar kiya to `size() = 2`, matlab duplicate andar ghus gaya.

## Tera hands-on (khud likha, chala ke dekha)

[H1 index](demo/hashmap/H1_IndexKaise.java) · [H2 takkar](demo/hashmap/H2_Takkar.java) ·
[H3 spread](demo/hashmap/H3_Spread.java) · [H4 bucket](demo/hashmap/H4_Bucket.java) ·
[H5 == vs equals](demo/hashmap/H5_Get.java) · [H6 contract](demo/hashmap/H6_Contract.java)
