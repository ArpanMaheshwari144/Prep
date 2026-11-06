# HashMap vs LinkedHashMap vs TreeMap

## Teeno tere kaam me aa chuke hain

- **HashMap:** userId se naam nikaalne ke liye. Ye teeno me sabse tez hai, par kram (order) ki koi guarantee nahi deta.
- **LinkedHashMap:** tera LRU cache isi se banta hai. Ye HashMap ke saath ek doubly linked list rakhta hai, jo kram yaad
  rakhti hai. `new LinkedHashMap<>(16, 0.75f, true)` likhne se kram access ke hisaab se chalta hai, aur `removeEldestEntry`
  override karne se sabse purani entry apne aap bahar ho jaati hai. Matlab DSA wala LRU Java me 5 line me ban jaata hai.
- **TreeMap:** leaderboard ke liye. Isme keys hamesha sorted rehti hain, kyunki andar Red-Black tree hota hai (wahi jo
  file 01 me chain ke tree banne wali baat me aaya tha). Iska asli fayda range wale sawaal hain:

```java
scores = {85=Rahul, 92=Priya, 100=Arpan}
scores.floorKey(90)    // 85  (90 se chhota ya barabar sabse bada)
scores.ceilingKey(90)  // 92
scores.subMap(85, 95)  // {85=Rahul, 92=Priya}
```

## Trade-off

HashMap O(1) me kaam karta hai, par koi kram nahi rakhta. LinkedHashMap bhi O(1) hai aur kram rakhta hai, par uske liye
thodi zyada memory leta hai. TreeMap O(log n) leta hai, kyunki sorted rakhne ki keemat chukani padti hai.

95% jagah HashMap hi kaafi hota hai.

## Prod trap (1-Oct chala ke dekha)

```java
map.put(103,"Priya"); map.put(101,"Arpan"); map.put(102,"Rahul");
// HashMap print: {101, 102, 103}  <- SANYOG se sorted
```

Yahan HashMap ne sorted print kiya, par ye sirf sanyog hai. Chhote Integer ka hashCode wahi number hota hai, isliye
keys kram se khaano me gir gayi. Ab kisi ne ye dekh ke maan liya ki "HashMap sorted deta hai", aur test bhi pass ho gaya.
Par jaise hi key String hui, ya map bada hua, kram bikhar jaata hai.

Isliye agar report ya API me kram chahiye, to LinkedHashMap ya TreeMap lo, HashMap pe bharosa mat karo.

## Null

HashMap aur LinkedHashMap me ek null key rakh sakte ho. TreeMap me null key daalo to NPE aata hai, kyunki sort karne ke
liye use compare karna padta hai, aur null ko compare nahi kar sakte. Haan, agar `Comparator.nullsFirst` de do, to chal
jaata hai.

Ek aur baat yaad rakh: TreeMap KEY pe sort karta hai, value pe nahi.
