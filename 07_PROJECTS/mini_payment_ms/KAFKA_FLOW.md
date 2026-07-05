# Mini Payment MS — poora FLOW (visual)

```
        POST /order  { item, amount }
             |
             v
   ┌──────────────────┐   route (Path=/order)   ┌──────────────────┐
   │  api-gateway     │ ──────────────────────► │  order-service   │
   │  (8080)          │   single entry-point     │  (8081)          │
   └──────────────────┘                          └────────┬─────────┘
                                                          │
                                        Feign call  ★ SYNC (RUKO jab tak jawab)
                                                          │
                                                          v
                                                 ┌──────────────────┐
                                                 │ payment-service  │  ◄── PRODUCER
                                                 │ (8082)           │
                                                 │ 1. DB save (paydb)│
                                                 │ 2. kafka.send(   │
                                                 │    "payment-done",│
                                                 │     "Payment...")│
                                                 └────────┬─────────┘
                                                          │
                                       ★ ASYNC (bhej ke AAGE badh gaya, wait nahi)
                                                          │
                                                          v
                              ╔═══════════════════════════════════════════╗
                              ║        KAFKA BROKER  (docker, :9092)       ║
                              ║   TOPIC: "payment-done"                    ║
                              ║     partition 0:                           ║
                              ║       [offset 0]  "Payment DONE for order  ║
                              ║                    : 1 amount :500.0"      ║   ◄── message yahan PADA (persist, disk)
                              ╚═══════════════════════╤═══════════════════╝
                                                      │
                                @KafkaListener(topics="payment-done", groupId="notification-group")
                                                      │  (Spring khud sunta + bulaata)
                                                      v
                                            ┌──────────────────────┐
                                            │ notification-service │  ◄── CONSUMER
                                            │ (8083)               │
                                            │  -> "Email send ->   │
                                            │      Payment DONE..."│
                                            └──────────────────────┘

   ─────────────────────────────────────────────────────────────────────────────
   SYNC  (Feign)  : order ↔ payment  -> jawab TURANT chahiye -> ruko.
   ASYNC (Kafka)  : payment → notification -> "ho gaya, ab email" -> bhej ke aage, user wait nahi.
   CONNECTOR      : topic naam "payment-done" -> producer send() + consumer @KafkaListener, DONO me SAME.
   OFFSET         : message ka partition me position (0,1,2...). consumer isi se track "kahan tak padha".
   DB-per-service : order->orderdb · payment->paydb · notification->koi DB nahi (sirf sune+print).
```
