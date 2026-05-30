# Observer Pattern — Pub-Sub (Event Notifications)

> **Design Patterns: Topic 4 — Behavioral Pattern (Common Interview)**

---

## STORY — YouTube Channel Subscription

> Imagine **YouTube channel pe video upload hua**:
>
> Tu (creator) bolta — *"Naya video upload kiya!"*
>
> **YouTube automatically** notify karta saare subscribers ko —
> Email, app notification, bell icon, home feed.
>
> Tu (creator) **kisi specific subscriber ko personally call nahi karta** —
> bas "publish" button dabate, baaki YouTube handle karta.
>
> **Observer pattern same kaam karta:**
> - **Publisher** (YouTube channel) — event publish karta
> - **Subscribers** (viewers) — event receive karte automatically
> - **Pub-Sub** — pure decoupling, sender ko receivers ki identity nahi pata.

---

## The Problem — Tight Coupling

```java
// Without Observer — tightly coupled
public class TransactionService {
    private EmailService email;
    private SMSService sms;
    private AuditService audit;
    private SlackService slack;

    public void transfer(Account from, Account to, double amount) {
        // ... transfer logic ...

        email.send("Transfer of " + amount);     // hardcoded
        sms.send("Transfer happened");            // hardcoded
        audit.log("Transfer event");              // hardcoded
        slack.notify("New transfer");             // hardcoded

        // Naya notification add karna ho? service code modify
        // pushNotification.send(...);   ← every time code change
    }
}
```

**Problems:**
1. **Service knows ALL notification types** — tight coupling
2. **Naya notification add** = service code modify (OCP violation)
3. **Hard to test** — multiple dependencies hardcoded
4. **Single Responsibility violated** — service does business + notifications

---

## Observer Solution

```java
public class TransactionService {
    private EventPublisher publisher;

    public void transfer(Account from, Account to, double amount) {
        // ... transfer logic ...

        publisher.publish(new TransferEvent(...));   // ONE call
        // Publisher automatically notifies all subscribers
    }
}

// Subscribers register independently:
publisher.subscribe(new EmailListener());
publisher.subscribe(new SMSListener());
publisher.subscribe(new AuditListener());
publisher.subscribe(new SlackListener());
```

**Benefits:**
- **Service decoupled** from listeners
- **Naye listeners add** without modifying publisher
- **Single Responsibility** — service does business, listeners do notifications
- **Easy to test** — mock publisher

---

## VISUAL — Pub-Sub Flow

```
                    ┌──────────────────┐
                    │  PUBLISHER       │
                    │  (EventPublisher)│
                    │                  │
                    │  subscribers:    │
                    │  ┌────────────┐  │
                    │  │ email      │──┼──┐
                    │  │ sms        │──┼──┤
                    │  │ audit      │──┼──┤
                    │  │ slack      │──┼──┤
                    │  └────────────┘  │  │
                    └────┬─────────────┘  │
                         │                │
              event.publish(e)            │
                         │                │
                         ▼                │ broadcast
                  ┌──────────────┐         │
                  │ for each sub:│         │
                  │   sub.onEvent│─────────┤
                  └──────────────┘         │
                                           │
   ┌──────────┬──────────┬──────────┬──────┘
   ▼          ▼          ▼          ▼
EmailListener SMSListener Audit  SlackListener
  .onEvent()  .onEvent() .onEvent() .onEvent()

```

**Key insight:** Publisher loops through subscribers, calls `onEvent()` on each. **One-to-many** broadcast.

---

## PART 1: Manual Implementation (3 components)

### 1 Event class — what happened
```java
public class TransactionEvent {
    private final String fromId;
    private final String toId;
    private final double amount;
    private final long timestamp;

    public TransactionEvent(String fromId, String toId, double amount, long timestamp) {
        this.fromId = fromId;
        this.toId = toId;
        this.amount = amount;
        this.timestamp = timestamp;
    }

    // getters...
}
```

### 2 Listener interface — who can subscribe
```java
public interface EventListener {
    void onEvent(TransactionEvent event);
}
```

### 3 Publisher — manages subscribers + broadcasts
```java
public class EventPublisher {
    private final List<EventListener> listeners = new ArrayList<>();

    public void subscribe(EventListener listener) {
        listeners.add(listener);
    }

    public void unsubscribe(EventListener listener) {
        listeners.remove(listener);
    }

    public void publish(TransactionEvent event) {
        for (EventListener listener : listeners) {
            listener.onEvent(event);   // notify each
        }
    }
}
```

### 4 Concrete listeners
```java
public class EmailListener implements EventListener {
    @Override
    public void onEvent(TransactionEvent event) {
        System.out.println("Email: ₹" + event.getAmount() + " transferred");
    }
}

public class SMSListener implements EventListener {
    @Override
    public void onEvent(TransactionEvent event) {
        System.out.println("SMS: Transfer of ₹" + event.getAmount());
    }
}
```

### Usage
```java
EventPublisher publisher = new EventPublisher();
publisher.subscribe(new EmailListener());
publisher.subscribe(new SMSListener());

publisher.publish(new TransactionEvent("A1", "A2", 1000, System.currentTimeMillis()));
// Output:
//   Email: ₹1000.0 transferred
//   SMS: Transfer of ₹1000.0
```

---

## PROJECT USAGE — `EventPublisher.java` (SimpleBankSystem)

**Files:** `01_JAVA/07_PROJECT/SimpleBankSystem/src/com/arpan/bank/observer/`

```java
// 1. EventPublisher.java
public class EventPublisher {
    private final List<EventListener> listeners = new ArrayList<>();

    public void subscribe(EventListener listener) {
        listeners.add(listener);
    }

    public void publish(TransactionEvent event) {
        for (EventListener listener : listeners) {
            listener.onEvent(event);
        }
    }
}

// 2. EventListener.java
public interface EventListener {
    void onEvent(TransactionEvent event);
}

// 3. EmailListener.java (concrete)
public class EmailListener implements EventListener {
    @Override
    public void onEvent(TransactionEvent event) {
        System.out.println("[Email] ₹" + event.getAmount() +
            " transferred from " + event.getFromId() + " to " + event.getToId());
    }
}

// 4. AccountService — integrates observer
public class AccountService {
    private final EventPublisher publisher;

    public void transfer(String fromId, String toId, double amount) {
        // ... transfer logic ...

        // Publish event — listeners auto-notified
        publisher.publish(new TransactionEvent(fromId, toId, amount, System.currentTimeMillis()));
    }
}
```

**Caller (Main.java):**
```java
EventPublisher publisher = new EventPublisher();
publisher.subscribe(new EmailListener());          // Email notifications
// publisher.subscribe(new SMSListener());          // future addition
// publisher.subscribe(new AuditListener());        // future addition

AccountService service = new AccountService(repository, publisher);
service.transfer("A1", "A2", 1000);
// → AccountService internally publishes event
// → EmailListener.onEvent() runs automatically
// → "[Email] ₹1000 transferred from A1 to A2"
```

**Interview line:**
> *"Observer pattern transactions ke liye implement kiya — `EventPublisher` event publish karta, multiple `EventListener` subscribers register hote (`EmailListener`, future SMS/audit). Publisher koi specific listener nahi jaanta — pure decoupling. Naye listener add karne mein publisher unchanged. Spring's `ApplicationEventPublisher` same concept use karta — RabbitMQ, Kafka at scale."*

---

## Real-World Examples

### 1. **Spring `ApplicationEventPublisher`** (built-in observer)
```java
@Service
public class OrderService {
    @Autowired
    private ApplicationEventPublisher publisher;

    public void placeOrder(Order order) {
        // ... save order ...
        publisher.publishEvent(new OrderPlacedEvent(order));   // broadcast
    }
}

@Component
public class EmailListener {
    @EventListener
    public void onOrderPlaced(OrderPlacedEvent event) {
        // send email
    }
}

@Component
public class InventoryListener {
    @EventListener
    public void onOrderPlaced(OrderPlacedEvent event) {
        // decrease stock
    }
}
```

### 2. **Java Swing/JavaFX action listeners**
```java
button.addActionListener(e -> System.out.println("Clicked!"));
// Multiple listeners can subscribe to same button
```

### 3. **`PropertyChangeListener` (JavaBeans)**
```java
bean.addPropertyChangeListener("name", evt -> {
    System.out.println("Name changed from " + evt.getOldValue() + " to " + evt.getNewValue());
});
```

### 4. **Reactive Streams (Project Reactor / RxJava)**
```java
Flux.just(1, 2, 3)
    .subscribe(value -> System.out.println(value));   // observer
```

### 5. **Message brokers (Kafka, RabbitMQ)** — Observer at scale
- Publisher = producer
- Subscribers = consumer groups
- Broker = decoupling middleware

---

## Push vs Pull Observer

### Push (humara approach)
```java
// Publisher PUSHES full data to listener
public void publish(TransactionEvent event) {
    for (EventListener l : listeners) {
        l.onEvent(event);   // entire event object passed
    }
}
```

### Pull
```java
// Publisher notifies, listener pulls what it needs
public void publish() {
    for (EventListener l : listeners) {
        l.onEvent(this);   // pass publisher reference
                           // listener calls publisher.getX(), getY()
    }
}
```

**Trade-offs:**
| | Push | Pull |
|---|---|---|
| Network/memory | More (full data) | Less (selective) |
| Coupling | More (data structure shared) | Less (interface-based) |
| Common in | Most modern code | Some legacy / large objects |

**Industry default:** Push (simpler, modern code).

---

## When to Use Observer?

| Use Observer when... | Don't use when... |
|---|---|
| One-to-many notifications | Single listener (just call directly) |
| Loose coupling between sender/receiver | Tight coupling acceptable |
| Dynamic listener add/remove | Fixed listener set |
| Event-driven architecture | Procedural flow OK |
| Decouple cross-cutting concerns (logging, auditing) | Single concern |

---

## Key Design Principles

1. **Publisher doesn't know listeners** — only interface contract
2. **Listeners can be added/removed** at runtime
3. **Order not guaranteed** — listeners may execute in any order (parallel possible)
4. **Async option** — publishers can run listeners on separate threads
5. **Failure isolation** — one listener fail shouldn't break others (try-catch around onEvent)

```java
// Production-grade publisher with isolation
public void publish(TransactionEvent event) {
    for (EventListener listener : listeners) {
        try {
            listener.onEvent(event);
        } catch (Exception e) {
            log.error("Listener failed", e);   // log, but continue
        }
    }
}
```

---

## INTERVIEW TALKING POINT

**Q: "Observer pattern kya hai aur kab use karte ho?"**

> *"Observer = pub-sub pattern jo **one-to-many** notifications enable karta. Publisher event publish karta, multiple subscribers automatically notified. Pure decoupling — publisher ko receivers ki identity nahi pata.
>
> Mere SimpleBankSystem mein implement kiya — `EventPublisher` transactions pe events fire karta, `EmailListener` subscribe karta. Naye listeners (SMS, audit) add karne mein publisher untouched. **OCP follow** karta — extension via plug-in.
>
> Real-world: Spring's `ApplicationEventPublisher`, Java Swing listeners, Kafka/RabbitMQ at scale, Project Reactor/RxJava streams."*

**Q: "Coupling reduce kaise karta?"**

> *"3 levels mein:
> 1. **Publisher → Listener**: publisher list mein interface store, concrete classes pata nahi
> 2. **Listener → Event**: listener event interface pe depend, publisher class pe nahi
> 3. **Add/remove dynamic**: runtime pe subscribe/unsubscribe — compile-time wiring nahi
>
> Spring's `@EventListener` zyada elegant — annotation-based subscription, hum manual `subscribe()` call nahi karte."*

**Q: "Push vs Pull observer?"**

> *"**Push** = publisher full event data send karta listener ko (humara approach). Modern preferred — simple, fast.
> **Pull** = publisher notify karta, listener publisher se data pull karta. Less coupling for large objects, but more network calls.
>
> 90% production code Push use karta — clear, fast, debugging easy."*

**Q: "Failure isolation kaise handle?"**

> *"Production publisher mein har listener call ko try-catch wrap karte. Ek listener crash → log karte but continue with others. Parallel execution possible bhi — ExecutorService use karke async event delivery. Critical concern: order matter karta ho toh sequential, performance critical ho toh async."*

---

## POWER PHRASE

> **"Observer pattern one-to-many notifications enable karta — publisher event publish karta, multiple subscribers automatic notified. Pure decoupling, OCP-friendly (naye listeners add bina publisher modify). Spring's `ApplicationEventPublisher`, Kafka, Java Swing — sab same concept at different scales."**

---

## MEMORY HOOK

```
Observer = "YouTube channel"
   • Creator publishes once
   • Subscribers automatic notified
   • Subscribers come/go dynamically

3 components:
   1. Event       → what happened (data carrier)
   2. Listener    → interface (subscribe contract)
   3. Publisher   → broadcasts to all listeners

Real-world:
   • Spring ApplicationEventPublisher
   • Java Swing addActionListener
   • Kafka producers/consumers
   • EventPublisher in SimpleBankSystem

Use when:
   One-to-many notifications
   Cross-cutting concerns (audit, logging)
   Loose coupling needed
   Dynamic listener management

Push vs Pull:
   Push (default) → publisher sends full data
   Pull (rare)    → listener fetches from publisher
```

---

## TRAP BOX

```
Trap 1: "Observer = Spring @EventListener"
         Spring is one implementation
         Pattern is concept — Spring/Kafka/Swing all use it

Trap 2: "Listeners run in parallel"
         Default sequential — order of subscribe()
         Need ExecutorService for parallel

Trap 3: "Publisher must know about listeners"
         Anti-pattern — defeats purpose
         Publisher knows ONLY interface contract

Trap 4: "One listener fail = all fail"
         Without isolation, yes
         Wrap each onEvent() in try-catch

Trap 5: "Memory leak with listeners"
         Forgotten listeners stay in publisher's list
         Always provide unsubscribe() — call in cleanup
```

---

## Quick Recall Summary

```
WHAT     → One-to-many event notifications
WHY      → Decouple publisher from listeners + OCP
HOW      → Publisher list + listener interface + broadcast loop
PROJECT  → EventPublisher in SimpleBankSystem
SPRING   → ApplicationEventPublisher + @EventListener
SCALE    → Kafka, RabbitMQ, message brokers
TRAP     → Memory leaks (forgotten listeners) + failure isolation
```
