# HLD INTERVIEW ANCHOR — reusable toolkit + follow-ups

> Arpan ka insight: **90% HLD SAME hota** (sharding/scaling/caching/LB har design me repeat).
> Ek baar ye toolkit pakka -> koi bhi design = 8-step me isko ASSEMBLE karke bol do.
> Ye = HLD ka "gate ki chaabi" (DSA sheet jaisa). Detail = FOUNDATIONS/*, ye = interview-DRIVE layer.

---

## A. REUSABLE TOOLKIT (har design me repeat — ratta karke drop karo)

| Cheez | Interview me kaise bolo |
|---|---|
| **Scaling** | "horizontal scale — LB ke peeche stateless instances add." (vertical = hardware-limit + single point) |
| **DB choice** | SQL (ACID, money/consistency) · NoSQL (massive scale, key-value, flexible). hamesha **KYUN** bolo |
| **Sharding** | bada data -> "shard by key (userId/orderId), **consistent hashing** se hotspot avoid" |
| **Caching** | read-heavy -> "Redis cache DB ke aage, cache-aside, TTL." static -> "CDN (CloudFront)" |
| **Replication** | read-load -> "read replicas (master-slave)" |
| **Async** | slow/spike kaam -> "queue + worker (Kafka), user block na ho" |
| **Consistency** | "idempotency-key (unique constraint), retries, at-least-once" |

---

## B. INTERVIEW ANCHOR — tu DRIVE karta hai (delivery)

- Shuru me bol: *"Main 8-step flow me chalunga — requirements, scale, API, high-level, data model, deep-dive, bottleneck, wrap."*
  -> interviewer ko turant pata: tu structured hai.
- **Har choice pe TRADE-OFF:** *"X liya kyunki..., Y nahi kyunki..."* -> yahi 90% marks.
- **THINK OUT LOUD** (chup = death). Numbers bolo (QPS, storage) -> impress.
- HONEST: na aaye to *"ye use nahi kiya, par aise approach karunga."*

---

## C. COMMON FOLLOW-UPS (jo ~har design me poochte)

- *"Scale 10x?"* -> shard + replicas + cache.
- *"Ye component gir jaye?"* -> replica/failover, no single point.
- *"Consistency vs availability?"* -> CAP — kaunsa chuna + KYUN.
- *"Race condition?"* -> lock / unique-constraint / idempotency.
- *"Hot / celebrity key?"* -> cache + consistent hashing.
- *"Data lost na ho?"* -> replication + durable queue (Kafka) + ACID jahaan zaroori.

---

## D. RATE-LIMITER (example anchors — pattern samjho, baaki designs pe apply)

- *"Distributed rate-limiter kaise?"* -> **Redis central counter** (atomic `INCR`), sab app-instances share karte.
- *"Why Redis?"* -> fast + atomic + shared across instances (stateless app ke liye perfect).
- *"Algo compare?"* ->
  - fixed-window : simple, par boundary pe BURST problem.
  - sliding-window : smooth, thoda mehnga.
  - **token-bucket** : bursts allow + smooth refill -> "main ye lunga, burst-friendly."

---

> CORE: naya design aaye -> 8-step me is toolkit ko ASSEMBLE -> done. 90% pehle se pata hai.
> "Problem-solver" HLD me = ye toolkit + trade-off bolna, na ki har baar scratch se sochna.
