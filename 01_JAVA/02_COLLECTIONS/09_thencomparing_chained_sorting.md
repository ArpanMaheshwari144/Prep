# thenComparing: tie-breaker

## Leaderboard se

Do khiladi hain, aur dono ka score barabar 950 hai. Ab upar kaun aayega? Rule ye hai ki pehle jiska score bada wo upar,
aur agar score barabar ho, to jo pehle 950 pe pahuncha wo upar.

```java
players.sort(
    Comparator.comparingInt(Player::getScore).reversed()   // pehla rule: score bada pehle
              .thenComparing(Player::getReachedAt)          // barabar ho to: jo pehle pahuncha
);
```

Doosra rule tabhi chalta hai jab pehla rule 0 de, matlab jab dono score barabar hon.

## Ye if-else hi hai (Arpan ka nichod)

Andar se ye bilkul wahi kaam karta hai jo if-else karta: pehla compare 0 aaya, to doosra dekho. Koi naya jaadu nahi hai,
sirf likhne ka andaaz alag hai. Farak bas itna hai ki if-else me sign ulta likhne jaisi chhoti galti ho sakti hai, jabki
chain me wo galti nahi hoti. Jo tujhe saaf lage wahi likho, jaise stream aur for loop me se jo pasand ho.

```java
// wahi cheez if-else me
int c = Integer.compare(b.score, a.score);
if (c != 0) return c;
return a.reachedAt.compareTo(b.reachedAt);
```

## Prod trap

```java
Comparator.comparingInt(Player::getScore)
          .thenComparing(Player::getReachedAt)
          .reversed()          // POORI chain ulti: reachedAt bhi ulta -> jo BAAD me pahuncha wo upar
```

`.reversed()` jahan lagate ho, wahan tak ki poori chain ulti ho jaati hai. Yahan aakhir me lagaya, to score ke saath
reachedAt bhi ulta ho gaya, aur jo baad me pahuncha wo upar aa gaya. Isliye ulta sirf usi rule pe lagao jise chahiye.
Ye galti test me tabhi pakdi jaati hai jab test me tie wala case ho.

Ek aur baat: rule ko `&&` se jodne ki koshish karoge to compile error aayega, kyunki compare int deta hai, boolean nahi.

## Interview line

```
For a leaderboard I sort by score descending and break ties with thenComparing on the time the
score was reached; I put reversed() only on the score comparator, because reversed() at the end
flips the whole chain, including the tie-breaker.
```
