# HashMap vs LinkedHashMap vs TreeMap

## Scene: teeno tere kaam me aa chuke hain

- **HashMap:** userId se naam nikaalna. Teeno me sabse tez, par kram (order) ki koi guarantee nahi.
- **LinkedHashMap:** tera LRU cache. HashMap ke saath ek doubly linked list rakhta hai jo kram yaad rakhti hai.
- **TreeMap:** leaderboard. Keys hamesha sorted, kyunki andar Red-Black tree hai (wahi jo file 01 me chain ke tree banne
  wali baat me aaya). Asli fayda range wale sawaal.

## Chala ke dekha: kram

```
HashMap int key   : {101=Arpan, 102=Rahul, 103=Priya}
HashMap String key: [Rahul, Priya, Arpan, Zoya, Kabir]
LinkedHashMap     : [Priya, Arpan, Rahul, Zoya, Kabir]
```

- Daala tha 103, 101, 102 ke kram me. HashMap ne int key sorted print ki, par ye SANYOG hai (agla trap).
- String key daali Priya, Arpan, Rahul, Zoya, Kabir. HashMap ne bikher di.
- LinkedHashMap ne wahi kram rakha jisme daala tha.

## LRU 5 line me (LinkedHashMap)

```java
new LinkedHashMap<>(16, 0.75f, true) {          // true = kram access ke hisaab se
    protected boolean removeEldestEntry(Map.Entry e) { return size() > 3; }
};
```

```
LRU (3 ki jagah, u1 padha, u4 aaya): [u3, u1, u4]
```

u1, u2, u3 daale, u1 padha (to wo aage aa gaya), u4 aaya to sabse purana u2 apne aap bahar. DSA wala LRU Java me 5 line.

## TreeMap: range wale sawaal

```
floorKey(90)=85 ceilingKey(90)=92 subMap(85,95)={85=Rahul, 92=Priya}
```

- `floorKey(90)` = 90 se chhota ya barabar sabse bada.
- `ceilingKey(90)` = 90 se bada ya barabar sabse chhota.
- `subMap(85, 95)` = 85 se 95 ke beech (95 shaamil nahi).

TreeMap KEY pe sort karta hai, value pe nahi.

## Dikkat: leaderboard me score ko key banaya

```
92 pe Zoya bhi aayi: {85=Rahul, 92=Zoya, 100=Arpan}
```

Priya gayab ho gayi. Ek key pe ek hi value rehti hai, to naya `put` purane ko kaat deta hai. Hal wahi jo HLD leaderboard
me tha: composite score, matlab score ke saath kuch aur jod ke key alag bana do.

## Trade-off

- HashMap O(1), koi kram nahi.
- LinkedHashMap bhi O(1) aur kram rakhta hai, par list ke liye thodi zyada memory.
- TreeMap O(log n), sorted rakhne ki keemat.

95% jagah HashMap hi kaafi hai.

## Prod trap: "HashMap sorted deta hai"

Upar int key sorted aayi kyunki chhote Integer ka hashCode wahi number hota hai, to keys kram se khaano me gir gayi. Kisi
ne ye dekh ke maan liya "HashMap sorted deta hai", test bhi pass. Par key String hui (upar dekha) ya map bada hua, kram
bikhar gaya.

Report ya API me kram chahiye to LinkedHashMap ya TreeMap lo, HashMap pe bharosa mat karo.

## Null

```
TreeMap null key -> NPE
HashMap null key  -> {null=1}
```

HashMap aur LinkedHashMap me ek null key chal jaati hai. TreeMap ko sort karne ke liye compare karna padta hai, aur null ko
compare nahi kar sakte, isliye NPE. `Comparator.nullsFirst` do to chal jaata hai.
