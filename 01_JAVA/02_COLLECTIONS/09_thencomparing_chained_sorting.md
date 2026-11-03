# thenComparing: tie-breaker

## Leaderboard se

Do khiladi, score barabar 950. Upar kaun? Rule: pehle score bada, barabar ho to jo pehle 950 pe pahuncha.

```java
players.sort(
    Comparator.comparingInt(Player::getScore).reversed()   // pehla rule: score bada pehle
              .thenComparing(Player::getReachedAt)          // barabar ho to: jo pehle pahuncha
);
```

Doosra rule tabhi chalta jab pehla 0 de (barabar).

## Ye if-else hi hai (Arpan ka nichod)

Andar se bilkul wahi: pehla compare 0 aaya to doosra dekho. Koi naya jaadu nahi, sirf likhne ka andaaz alag. Farak
bas itna ki if-else me sign ulta likhne jaisi chhoti galti ho sakti, chain me nahi. Jo saaf lage wahi likho (stream
vs for loop jaisa).

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

`.reversed()` jahan lagao, wahan tak ki poori chain ulti. Ulta sirf usi rule pe jise chahiye. Test me tabhi pakda
jaata jab tie wala case ho. `&&` se rule jodna = compile error (compare int deta, boolean nahi).

## Interview line

```
For a leaderboard I sort by score descending and break ties with thenComparing on the time the
score was reached; I put reversed() only on the score comparator, because reversed() at the end
flips the whole chain, including the tie-breaker.
```
