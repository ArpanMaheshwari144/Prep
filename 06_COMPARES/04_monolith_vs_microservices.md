# Monolith vs Microservices

---

## 1-Line Analogy

```
Monolith       = One big building (all teams inside)
Microservices  = Many small buildings (each team own)
```

---

## Visual Architecture

```
MONOLITH:
   ┌────────────────────────────────────┐
   │       SINGLE APPLICATION            │
   │                                       │
   │  ┌────────┐ ┌────────┐ ┌────────┐  │
   │  │ User    │ │ Cart    │ │ Order  │ │
   │  └────────┘ └────────┘ └────────┘  │
   │  ┌────────┐ ┌────────┐ ┌────────┐  │
   │  │Payment  │ │Invent.  │ │Notify  │ │
   │  └────────┘ └────────┘ └────────┘  │
   │                                       │
   │       ALL share ONE DB           │
   │       ┌──────────────┐               │
   │       │   DATABASE   │                │
   │       └──────────────┘               │
   └────────────────────────────────────┘

   = 1 codebase, 1 deployment, 1 DB
```

```
MICROSERVICES:
   ┌──────────┐  ┌──────────┐  ┌──────────┐
   │ User Svc │  │ Cart Svc │  │Order Svc │
   │  + DB     │  │  + DB     │  │  + DB     │
   └─────┬────┘  └─────┬────┘  └─────┬────┘
         │              │              │
         └──── REST / gRPC / Kafka ────┘

   ┌──────────┐  ┌──────────┐  ┌──────────┐
   │Payment   │  │Inventory │  │Notify    │
   │Svc + DB  │  │Svc + DB   │  │Svc + DB   │
   └──────────┘  └──────────┘  └──────────┘

   = Multiple services, each own DB
   = Communicate via APIs / events
```

---

## Side by Side

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Feature         │  Monolith         │  Microservices    │
├──────────────────┼──────────────────┼──────────────────┤
│ Codebase         │ Single             │ Many (per service)│
├──────────────────┼──────────────────┼──────────────────┤
│ Deployment       │ One unit            │ Independent per   │
│                  │                     │ service           │
├──────────────────┼──────────────────┼──────────────────┤
│ Database         │ One shared         │ DB per service     │
│                  │                     │ (polyglot OK)     │
├──────────────────┼──────────────────┼──────────────────┤
│ Communication    │ Function calls    │ HTTP/gRPC/Kafka    │
│                  │ (in-process)      │ (network)          │
├──────────────────┼──────────────────┼──────────────────┤
│ Tech stack       │ Single (Java only)│ Polyglot (Java +  │
│                  │                    │  Go + Python OK)  │
├──────────────────┼──────────────────┼──────────────────┤
│ Scaling          │ Scale ENTIRE app  │ Scale only hot    │
│                  │ (even unused parts)│ services          │
├──────────────────┼──────────────────┼──────────────────┤
│ Failure          │ One bug = whole   │ Isolated failures │
│                  │ app down           │ (circuit breaker) │
├──────────────────┼──────────────────┼──────────────────┤
│ Team             │ All work together │ Team per service   │
│                  │                    │ (autonomous)       │
├──────────────────┼──────────────────┼──────────────────┤
│ Complexity        │ Simple to start   │ Complex orchestra │
│                  │ Complex at scale   │ DevOps heavy       │
├──────────────────┼──────────────────┼──────────────────┤
│ Speed to start   │ Fast            │ Slow setup     │
│ Speed to scale   │ Slow            │ Easy            │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## MONOLITH PROS / CONS

```
PROS:
   • Simple to develop (one codebase)
   • Simple to deploy (one unit)
   • Simple to test (E2E in one place)
   • Easy debugging (single trace)
   • Less infra cost initially
   • Faster to ship MVP

CONS:
   • Single point of failure
   • Hard to scale specific features
   • Tech lock-in (one stack)
   • Team coordination bottleneck
   • Big bang releases
   • "Build the whole world to test one fix"
```

---

## MICROSERVICES PROS / CONS

```
PROS:
   • Independent deployment per service
   • Scale only what's hot (e.g., search)
   • Tech freedom (Java, Go, Python mix)
   • Team autonomy (own service, own pace)
   • Isolated failure (one service down ≠ whole app)
   • Database per service (right tool per job)

CONS:
   • Complex deployment (Kubernetes, CI/CD)
   • Network calls = latency + failures
   • Distributed tracing required (Jaeger, Zipkin)
   • Data consistency hard (no single transaction)
   • Service discovery, configuration, monitoring
   • Higher infra cost
   • "Distributed monolith" anti-pattern risk
```

---

## WHEN TO USE WHAT

```
MONOLITH START WHEN:
   • Startup / MVP phase
   • Small team (< 10 devs)
   • Domain not fully understood yet
   • Limited DevOps capacity
   • Single-language org
   • Tight deadline first launch

   Examples:
   • UserCRUD (perfect monolith)
   • New SaaS startup
   • Internal tools

MICROSERVICES WHEN:
   • Multiple teams need to deploy independently
   • Different scale requirements per feature
   • Polyglot tech needs
   • Already at monolith pain points
   • Mature DevOps (K8s, CI/CD ready)
   • Large org (50+ engineers)

   Examples:
   • Netflix, Uber, Amazon, Spotify
   • Konovo (some microservices likely)
   • Big e-commerce platforms
```

---

## REAL STORY — Netflix Migration

```
2008: Netflix had MONOLITH
        ↓ DB corruption + 3-day outage
2009: "We're moving to microservices"
        ↓ 7-year journey
2016: Fully microservices-based
        ↓
   Today: 700+ microservices
          1000+ deploys/day
          Trillions of API calls/day

= Migration is HARD
= Not a quick refactor
= "Strangler pattern" used (replace piece by piece)
```

---

## KEY INSIGHT — Don't Skip Monolith

```
COMMON MISTAKE:
   "We'll build microservices from day 1!" (startup)
   
RESULT:
   • Domain still unknown (wrong service boundaries)
   • Team too small (each service unowned)
   • DevOps overhead crushes velocity
   • "Distributed monolith" — services tightly coupled

CORRECT PATH:
   1. Start with monolith (well-modularized)
   2. Find pain points (specific scaling issues)
   3. Extract THAT service first
   4. Repeat as needed

= Martin Fowler's "Monolith First" advice
= Most successful microservices = evolved from monolith
```

---

## INTERVIEW LINE

```
"Monolith is a single deployable unit — simple to develop
 and deploy, but hard to scale specific features.

 Microservices are independent services with own data,
 communicating via APIs or events — independent scaling
 and deployment, but complex orchestration.

 Start with monolith for MVP. Move to microservices when
 you have clear scaling bottlenecks or team autonomy
 needs.

 Netflix took 7 years to migrate — it's a journey, not
 a refactor. Many startups fail by going microservices
 too early — wrong service boundaries become permanent
 debt."
```

---

## Memory Hook

```
Monolith      = One big building (all teams together)
Microservices = Many buildings (autonomous teams)

Monolith: Simple, fast start, hard scale
Microservices: Complex, slow start, easy scale

Rule: Start monolith → extract when pain hits
Anti-pattern: Day-1 microservices without need

Netflix: 700+ services, 1000+ deploys/day
```

[← HLD README](../README.md)
