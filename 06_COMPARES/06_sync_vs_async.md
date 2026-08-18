# Sync vs Async

---

## 1-Line Analogy

```
Sync  = Phone call (wait until they answer)
Async = WhatsApp message (send, do other stuff, reply aata hai)
```

---

## Side by Side

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Feature         │  Sync             │  Async            │
├──────────────────┼──────────────────┼──────────────────┤
│ Caller behavior  │ WAITS / blocks │ Continues       │
├──────────────────┼──────────────────┼──────────────────┤
│ Response timing  │ Immediate          │ Later (callback,  │
│                  │ (in same call)     │ event, polling)   │
├──────────────────┼──────────────────┼──────────────────┤
│ Coupling         │ Tight              │ Loose              │
│                  │ (caller needs       │ (fire and forget) │
│                  │  callee alive)      │                    │
├──────────────────┼──────────────────┼──────────────────┤
│ Simplicity       │ Simple to code  │ Complex         │
│                  │ Linear flow         │ Callbacks/promises│
├──────────────────┼──────────────────┼──────────────────┤
│ Throughput       │ Limited (blocking)│ High (non-blocking)│
├──────────────────┼──────────────────┼──────────────────┤
│ Failure handling │ Try-catch          │ Retry/DLQ/queue   │
├──────────────────┼──────────────────┼──────────────────┤
│ Tech examples    │ REST API call,    │ Message queue,    │
│                  │ Method call,       │ Webhook, Pub-Sub  │
│                  │ DB query          │ @Async, Kafka     │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## SYNC VISUAL — Phone Call

```
CALLER                        CALLEE
   │                              │
   │ Call: "Hi, place order?"     │
   │ ────────────────────────────►│
   │                              │
   │ [WAITING... blocked]         │
   │ [WAITING... blocked]         │ Order processing
   │ [WAITING... blocked]         │
   │                              │
   │ ◄──────────────────────── "Order #123, done"
   │                              │
   │ Continue work               │

   = Caller BLOCKED until response
   = Cannot do anything else
   = Simple flow, predictable
```

---

## ASYNC VISUAL — WhatsApp

```
CALLER                        CALLEE
   │                              │
   │ Message: "Place order"       │
   │ ────────────────────────────►│
   │                              │
   │ Continue work              │ Order processing
   │ Do other tasks              │ (eventually)
   │ Drink chai                 │
   │                              │
   │ ◄────── Notification: "Order #123 placed"
   │                              │
   │ "Acha, thanks"               │

   = Caller FREE to do other work
   = Response arrives LATER
   = More complex but scalable
```

---

## IMPLEMENTATION PATTERNS

```
SYNC IMPLEMENTATION:

   // Java REST call
   String result = restTemplate.getForObject("/api/order", String.class);
   // Code waits here until response
   System.out.println(result);

   = Simple linear code
   = One thread waits during call
```

```
ASYNC IMPLEMENTATION OPTIONS:

1. CALLBACK
   sendOrder(data, response -> {
      System.out.println(response);
   });
   continueOtherWork();

2. PROMISE / CompletableFuture
   CompletableFuture<String> future = sendOrder(data);
   future.thenAccept(System.out::println);
   continueOtherWork();

3. MESSAGE QUEUE
   kafkaProducer.send("orders", orderData);
   // Order service picks up later

4. WEBHOOK
   POST /order { callbackUrl: "https://my.com/done" }
   // Server calls back to your URL when done

5. POLLING
   POST /order → returns jobId
   GET /order/{jobId}/status → check periodically
```

---

## SCENARIOS — KAB KYA USE KARE?

```
SYNC USE KARO WHEN:

   • User waiting for response (UI form submit)
   • Result needed immediately (price calculation)
   • Simple CRUD operations
   • Database queries
   • Authentication checks

   Examples:
   • Login API (sync — user waits)
   • Cart fetch (sync — page rendering)
   • Payment gateway redirect
   • DB read for user profile

ASYNC USE KARO WHEN:

   • Long-running tasks (image processing, video encode)
   • Non-blocking required (high throughput)
   • Loose coupling needed
   • Different speeds (slow consumer)
   • Retry/DLQ scenarios

   Examples:
   • Email sending (Notification System)
   • Order processing (microservices)
   • Analytics events
   • Background jobs
   • Cron-style work
   • Inter-service communication (event-driven)
```

---

## REAL WORLD — E-Commerce Order

```
User clicks "Place Order":

   SYNC parts (fast, user waits):
      1. Validate cart       → sync
      2. Check inventory      → sync
      3. Charge payment       → sync (or pending)
      4. Create order record  → sync
      5. Return order ID      → sync

      USER SEES: "Order #123 placed!" (within 2 sec)

   ASYNC parts (background, no blocking):
      6. Send confirmation email   → async (queue)
      7. Update inventory levels   → async
      8. Notify warehouse           → async
      9. Update analytics           → async
      10. Loyalty points              → async
      11. Recommendation engine      → async

      = User doesn't wait for these
      = Fail-tolerant (retry from queue)
```

---

## SPRING BOOT ANGLE

```
SYNC (default):
   @PostMapping("/users")
   public User create(@RequestBody User user) {
       return service.create(user);  // blocking
   }
   = Request held until response

ASYNC:
   @Async
   public CompletableFuture<Void> sendWelcomeEmail(User u) {
       // runs in background thread pool
       emailService.send(u);
       return CompletableFuture.completedFuture(null);
   }

   @PostMapping("/users")
   public User create(@RequestBody User user) {
       User saved = service.create(user);
       emailService.sendWelcomeEmail(saved);  // async
       return saved;  // returns immediately
   }
```

---

## ASYNC GOTCHAS

```
1. Error handling harder
   Sync: throw → caller catches
   Async: error in background → who catches?
        Solutions: error topics, DLQ, dead letter queue

2. Debugging harder
   Sync: single call stack
   Async: multiple threads, distributed traces needed

3. Ordering not guaranteed
   Sync: A → B → C in order
   Async: A, B, C might process parallel/out of order
        Solutions: ordering keys, partitions

4. Eventual consistency
   Sync: read-after-write works
   Async: data might not be there immediately
        Solutions: optimistic updates, polling

5. Resource leaks
   Async: forget to close future, thread leak
   Solutions: timeout, supervision
```

---

## INTERVIEW LINE

```
"Synchronous calls block the caller until response —
 simple to code, predictable, but limits throughput.
 Use for user-facing requests, immediate results.

 Asynchronous calls allow the caller to continue —
 higher throughput, loose coupling, but complex error
 handling and harder debugging.

 In modern systems, sync for user-facing critical paths
 (login, checkout) and async for background work,
 inter-service communication, and high-volume processing.

 Notification systems use async — fire event to Kafka,
 workers process eventually. Order placement is mixed —
 sync for confirmation, async for downstream tasks."
```

---

## Memory Hook

```
Sync  = Phone call (blocking, wait)
Async = WhatsApp (non-blocking, continue)

Sync: simple, slow, tight coupling
Async: complex, fast, loose coupling

Rule:
   USER-FACING critical → SYNC (user waits)
   BACKGROUND / BATCH → ASYNC (queue, retry)

Tools:
   Sync: REST, RPC, method calls, DB queries
   Async: Kafka, RabbitMQ, @Async, CompletableFuture,
          webhooks, message queues
```

[← HLD README](../README.md)
