# Kafka vs RabbitMQ

> Dono message-passing, par DO alag philosophy: Kafka = LOG (rakho + replay) · RabbitMQ = QUEUE (route + deliver-once).

---

## 1-Line

```
Kafka    = distributed LOG / event-stream -> messages RUKTE hain (retained), consumer PULL + offset, replay ho sakta. High-throughput.
RabbitMQ = traditional QUEUE / broker    -> message deliver ho ke KHATAM (consume+delete), smart broker ROUTING karta, PUSH karta.
```

---

## ANALOGY

```
Kafka = TAPE-RECORDER / newspaper archive
   sab kuch RECORD hota rehta. koi bhi consumer baad me aa ke apni jagah (offset) se padhe,
   dobara REPLAY kare. message delete nahi hota (retention tak). "stream + replay".

RabbitMQ = POST-OFFICE / courier
   message queue me daalo -> jise dena hai use DELIVER -> deliver ho gaya to GAYAB (ack).
   smart routing (kaunsa exchange, kis address). "route + deliver-once".
```

---

## FARAK (side by side)

```
┌──────────────────┬────────────────────────────┬────────────────────────────┐
│  Feature         │  KAFKA                      │  RABBITMQ                  │
├──────────────────┼────────────────────────────┼────────────────────────────┤
│ Model            │ append-only LOG (retained)  │ QUEUE (consume -> delete)  │
│ Message baad me  │ REHTA (replay ho sakta)     │ ack ke baad HAT jaata      │
│ Consumer         │ PULL + offset (khud track)  │ PUSH (broker bhejta)       │
│ Routing          │ simple (topic + partition)  │ SMART (exchange: direct/    │
│                  │                             │ topic/fanout/headers)      │
│ Throughput       │ BAHUT high (millions/sec)   │ medium (routing-flexible)  │
│ Ordering         │ per-PARTITION               │ per-queue                  │
│ Broker           │ "dumb broker, smart consumer"│ "smart broker, dumb consumer"│
└──────────────────┴────────────────────────────┴────────────────────────────┘
```

---

## KAB KAUNSA

```
KAFKA    -> event-streaming, log-aggregation, analytics, REPLAY chahiye, bahut zyada volume.
            (mini_payment_ms: payment event bheja -> notification-service consume -> event log me raha)
RABBITMQ -> task-queue, RPC, COMPLEX routing, per-message low-latency, transactional workflows.

RULE: "stream / replay / high-volume"   -> Kafka
      "route / deliver-once / task-queue" -> RabbitMQ
```

---

## INTERVIEW LINE

```
"Kafka ek distributed LOG hai -- messages retain hote, consumer PULL + offset se replay kar sakta,
 high-throughput event-streaming ke liye. RabbitMQ traditional broker/QUEUE hai -- smart routing
 (exchanges), message ack ke baad delete, task-queue/complex-routing ke liye.
 Kafka = 'stream + replay', RabbitMQ = 'route + deliver-once'."
```

---

## Memory Hook

```
Kafka    = LOG  -> rakho + REPLAY (pull, offset, high-throughput)   -> "tape recorder"
RabbitMQ = QUEUE -> route + DELIVER-ONCE (push, ack-delete, smart)  -> "post office"
```

[← COMPARES README](../README.md)
