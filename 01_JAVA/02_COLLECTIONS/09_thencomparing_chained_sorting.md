# thenComparing: tie-breaker

## Scene: leaderboard

Arpan aur Priya dono ka score 950 hai. Upar kaun? Rule: score bada wo upar, aur score barabar ho to jo PEHLE 950 pe
pahuncha wo upar.

```java
players.sort(
    Comparator.comparingInt(Player::getScore).reversed()   // pehla rule: score bada pehle
              .thenComparing(Player::getReachedAt)          // barabar ho to: jo pehle pahuncha
);
```

```
sahi (reversed sirf score pe): [Arpan(950, 3 baje), Priya(950, 5 baje), Rahul(900, 1 baje)]
```

Doosra rule tabhi chalta hai jab pehla rule 0 de, matlab jab dono score barabar hon. Arpan 3 baje pahuncha, Priya 5 baje,
to Arpan upar.

## Ye if-else hi hai (Arpan ka nichod)

Andar se bilkul wahi jo if-else karta: pehla compare 0 aaya to doosra dekho. Koi naya jaadu nahi, sirf likhne ka andaaz
alag. Farak itna ki if-else me sign ulta likhne jaisi chhoti galti ho sakti hai, chain me nahi. Jo saaf lage wahi likho.

```java
int c = Integer.compare(b.score, a.score);
if (c != 0) return c;
return a.reachedAt.compareTo(b.reachedAt);
```

```
if-else wahi             : [Arpan(950, 3 baje), Priya(950, 5 baje), Rahul(900, 1 baje)]
```

## Prod trap: `.reversed()` aakhir me

```java
Comparator.comparingInt(Player::getScore)
          .thenComparing(Player::getReachedAt)
          .reversed()          // POORI chain ulti
```

```
trap (aakhir me reversed): [Priya(950, 5 baje), Arpan(950, 3 baje), Rahul(900, 1 baje)]
```

- `.reversed()` jahan lagao, wahan tak ki POORI chain ulti ho jaati hai.
- Aakhir me lagaya to score ke saath reachedAt bhi ulta: jo BAAD me (5 baje) pahuncha wo upar.
- Ulta sirf usi rule pe lagao jise chahiye.
- Test me ye tabhi pakdi jaati hai jab test me tie wala case ho. Bina tie ke dono output same dikhte.

Ek aur baat: rules ko `&&` se jodoge to compile error, kyunki compare int deta hai, boolean nahi.
