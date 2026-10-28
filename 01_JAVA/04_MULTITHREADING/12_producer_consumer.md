# Producer-Consumer

**Kahani:** Amazon pe ek second me 1000 order aate hain, par processing (DB + email + inventory) ek second me sirf 50 ho paati hai. Beech me ek fixed size ka bench rakh do. Bench bhar jaaye to order lene wala rukta hai, bench khaali ho to process karne wala rukta hai. File 08 aur 09 ka `wait/notify` isi ke liye tha.

```
PRODUCER ──> [ ][ ][ ][ ][ ] (fixed jagah) ──> CONSUMER
   bhara -> ruko                                  khaali -> ruko
```

**Tera practical:** [ProducerConsumer](practical/ProducerConsumer.java) (synchronized + wait/notify, LIMIT 5): 10 produce, 10 consume, "Done". Kabhi 2 item ek saath bane, kabhi consumer peeche, par buffer 5 se aage nahi gaya.

**Khaas baat:**
1. Teen tareeke hain:
   - `synchronized + wait/notifyAll`: interview ka classic. Isme `while` aur `notifyAll` zaroori hain (file 08 aur 09).
   - `BlockingQueue`: production me yahi. `put()` bhari queue pe rukta hai, `take()` khaali queue pe rukta hai. Koi taala ya `wait` khud nahi likhna padta.
     ```java
     BlockingQueue<Order> q = new ArrayBlockingQueue<>(1000);
     q.put(order);          // producer: bhara ho to ruko
     Order o = q.take();    // consumer: khaali ho to ruko
     ```
   - `ReentrantLock + Condition`: do alag bench-line, `notFull` aur `notEmpty`. Isse sirf sahi waale thread jaagte hain, file 09 wali galat-thread-jagne ki dikkat khatam.

   ```
                     ┌────────────────────────┐
                     │  Producer-Consumer ka  │
                     │  implementation chahiye│
                     └────────────┬───────────┘
                    ┌─────────────┴─────────────┐
             Production code?           Interview / Learning?
                    │                           │
                    ▼                           ▼
           ┌──────────────────┐       ┌──────────────────────┐
           │  BlockingQueue   │       │ synchronized +       │
           │  (default)       │       │ wait/notify          │
           └────────┬─────────┘       │ (classic answer)     │
                    │                 └──────────────────────┘
           Custom signaling chahiye?
                    ▼
           ┌──────────────────────┐
           │ ReentrantLock +      │
           │ Condition (notFull,  │
           │ notEmpty)            │
           └──────────────────────┘
   ```
2. Saada `Queue` ek thread ke liye hai (BFS jaisa). Kai thread share karein to `BlockingQueue`. Chala ke dekha tha: 2 thread x 1 lakh add, LinkedList pe 197868 aaye, BlockingQueue pe 200000.
3. Ye pattern har jagah hai:
   - `ExecutorService` ka bench file 10 wala hi hai.
   - Async logging.
   - `@Async`.
   - Bade level pe Kafka: Producer, topic, Consumer group. Notification aur Kafka design yahi hai, bas kai server pe phaila hua.

**BlockingQueue vs Kafka (idea same, 4 farak):**
```
                    BlockingQueue                Kafka
kahan rehta         ek JVM ki RAM                disk pe, 3 broker pe copy
app crash           saara maal gaya              maal bacha rehta
padhne ke baad      take() = nikal gaya          message rehta, sirf offset aage badhta
kaun padhe          ek item ek hi consumer       har consumer group apni copy padhe
```
Ek JVM ke andar threads ke beech BlockingQueue, services / machines ke beech Kafka. Maal kaise bachta = [kafka_concepts hissa 5](../../04_HLD/SYSTEM_DESIGNS/12_message_queue_kafka/kafka_concepts.md).

**Prod wala trap:**
```java
q.offer(order);    // bhari queue pe false lautata -> check nahi kiya to order chup-chaap gaya
q.put(order);      // jagah banne tak rukta, order kabhi nahi khota
new LinkedBlockingQueue<>();   // bina size = koi had nahi = file 10 wala OOM, naya naam
```

```
Interview: "Producer-consumer decouples a fast producer from a slow consumer with a bounded buffer -
producer waits when full, consumer waits when empty. Classic version is synchronized with
wait/notifyAll in a while loop. In production I use a bounded BlockingQueue - put and take
block for me - or ReentrantLock with notFull / notEmpty conditions for targeted wakeups.
At system scale the same idea is Kafka."
```
