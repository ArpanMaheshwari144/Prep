# Producer-Consumer

## Scene: ladka shelf bharta, customer uthata

Ladka godown se shelf bharta hai (producer), aur customer shelf se uthata hai (consumer). Shelf me sirf 5 dabbe ki jagah hai.
Shelf bhar jaaye to ladka khada intezaar karta hai, aur khaali ho jaaye to customer. Chala ke dekha, `BlockingQueue` (5 ki
jagah) ke saath:

```
ladka ne dabba 1-5 rakha   (5 ms tak, shelf=5)     <- jhat se bhar diya
ladka ne dabba 6 rakha     (1004 ms, shelf=5)      <- put() apne aap ruka, customer ke uthane tak
ladka ne dabba 7 rakha     (1306 ms)
ladka ne dabba 8 rakha     (1607 ms)
```

Ladke ne koi `wait` ya `notify` nahi likha. `put()` bhari shelf pe khud ruk jaata hai, aur `take()` khaali shelf pe khud ruk
jaata hai. File 09 wala "galat thread jag gaya" jhanjhat bhi nahi.

```java
BlockingQueue<Order> q = new ArrayBlockingQueue<>(1000);
q.put(order);          // producer: bhara ho to ruko
Order o = q.take();    // consumer: khaali ho to ruko
```

Interview me "khud likh ke dikhao" bole, to classic jawab `synchronized` + `while` + `wait/notifyAll` hai
([ProducerConsumer](practical/ProducerConsumer.java), LIMIT 5). Production me `BlockingQueue`. Aur agar sirf sahi wale thread
jagaane hon, to `ReentrantLock` ki do `Condition` (`notFull`, `notEmpty`).

## Prod wala trap

Saada list kai thread ke beech share kar di (2 thread, 1-1 lakh item):

```
LinkedList:            147805  (hona chahiye 200000)   <- 52 hazaar dabbe gaayab
LinkedBlockingQueue:   200000
```

Saada `Queue` ya `LinkedList` sirf ek thread ke liye hai, jaise BFS me. Kai thread ke beech `BlockingQueue` chahiye.

## Ye pattern har jagah

File 10 ke pool ki line yahi hai, `@Async` yahi hai, aur bade level pe Kafka yahi hai: producer, topic (shelf), aur consumer
group. Stadium wali Kafka kahani bhi yahi thi, bas kai server pe phaili hui.
