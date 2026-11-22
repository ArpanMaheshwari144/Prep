# Concurrency vs Parallelism — ek chef kai dish, ya kai chef

---

## Koi poochhe to aise bolo

**Concurrency** = kai kaam ek saath SAMBHALNA, chahe CPU ek hi ho: CPU baari-baari thoda-thoda har kaam karta; ek kaam DB ke
jawab ka intezaar kar raha to tab tak doosra chal jaata. **Parallelism** = kai kaam sach me ek hi pal me CHALNA, alag-alag core
pe. Concurrency = dhaancha (kaam kaise baanta), parallelism = hardware (kitne core sach me saath chal rahe). Tera Spring app dono
karta hai: 200 thread = concurrency, aur machine me 8 core hon to unme se 8 ek pal me sach me chal rahe.

---

## Kahani: ek chef vs teen chef

```
Concurrency = ek chef, teen dish: daal ubaal pe rakhi, tab tak sabzi kaati, phir roti; teeno chalti dikhti, haath ek hi
Parallelism = teen chef, teen dish: sach me teeno ek pal me ban rahi
```

---

## Tere project se

- **Tomcat ke 200 thread:** zyadatar request DB / doosri service ka intezaar karti; intezaar me CPU khaali, isliye 8 core pe
  bhi 200 request sambhal lete. Ye concurrency.
- **parallelStream / bhaari ginti** (1 crore number ka jod): intezaar nahi, sirf CPU ka kaam, 8 core pe baant ke tez.
  Ye parallelism.
- **Kafka partitions:** 6 partition, 6 consumer sach me ek saath padhte. Ye parallelism.

---

## Dikkat se hal

CPU wala bhaari kaam (image resize) chalana tha, to 500 thread bana diye ki tez hoga. Core 8 hi the: 500 thread aapas me
baari-baari karte rahe aur ulta dheema. Hal: CPU wale kaam me thread lagbhag core jitne; intezaar wale kaam (DB, network) me
thread zyada rakh sakte.

---

## Trap

`parallelStream()` laga diya ki tez hoga, par list chhoti thi, ya andar DB call thi, ya `ArrayList` me add kar rahe the.
Nateeja: kabhi dheema, kabhi galat ginti (`ArrayList` thread-safe nahi). parallelStream sirf bade, saaf CPU wale kaam me.
