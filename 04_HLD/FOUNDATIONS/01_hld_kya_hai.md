# HLD kya hai — code se pehle ka naksha

> **NAV** — KYA: HLD hota kya hai (entry). agla: [02 capacity-estimation](02_capacity_estimation.md) · juda: [11 SPOF](11_reliability_spof_cloud.md) · [03 load balancing](03_load_balancing.md)

## Koi poochhe to aise bolo

HLD system ka naksha hai jo code likhne se pehle banta hai. Isme tay hota hai ki kaun se dabbe honge (LB, server, cache, DB, queue), wo aapas mein kaise judenge, aur bheed badhe ya koi dabba gire to kya hoga. LLD ek dabbe ke andar ki class aur method dekhta hai, HLD poore shehar ka naksha.

## Kahani (ghar)

Ghar banane wala seedha eent nahi rakhta. Pehle naksha banata hai ki kamre kahan honge, paani aur bijli kahan se aayegi, kitni manzil banegi aur kal family badhi to kamra kaise judega. HLD yahi naksha hai. LLD baad ka kaam hai: kamre ke andar almari kahan lagegi.

Bheed badhe to do raaste hain:

```
VERTICAL   ek dukandaar ko aur tez banao (bada CPU / RAM)
           -> ek had ke baad nahi badhta, mehnga, aur wo gira to dukaan band (SPOF)
HORIZONTAL aur dukandaar rakho, kaam baant do (LB ke peeche 3-4 server)
           -> jitne chahiye badhao, ek gira to baaki chalte
```

Bade system horizontal chalte hain, aur LB, replica aur shard isi ke auzaar hain.

Ek aam dhaancha aisa dikhta hai:

```
   USER (browser / app)
        │
        ▼
   ┌───────────────┐
   │ Load Balancer │  ← traffic baantta
   └───────┬───────┘
           │
   ┌───────┼─────────┐
   ▼       ▼         ▼
┌──────┐┌──────┐┌──────┐
│App 1 ││App 2 ││App 3 │  ← kai server
└──┬───┘└──┬───┘└──┬───┘
   │       │       │
   └───┬───┴───┬───┘
       ▼       ▼
   ┌────────┐ ┌─────────┐
   │ Cache  │ │ Database│
   │ (Redis)│ │ (MySQL) │
   └────────┘ └────┬────┘
                   │ copy
                   ▼
                ┌─────────┐
                │ Replica │
                └─────────┘
```

## Ek trap

Interviewer pooche "ek bada server le lo, kaafi nahi?" To jawab hai: shuru mein chal jaayega, par uski had hai aur wo SPOF hai. Isliye bheed wale system mein server badhao aur LB lagao.
