# Monolith vs Microservices — ek badi building ya kai chhoti

---

## Koi poochhe to aise bolo

**Monolith:** poora app ek codebase, ek deploy, ek DB. Banana, chalana, debug aasaan; par ek chhote badlaav pe bhi poora app
dobara deploy, aur sirf search pe bheed ho to bhi poora app badhana padta. **Microservices:** user, order, payment alag-alag
service, har ek ka apna DB aur apna deploy, aapas me REST / Kafka se baat. Har service alag se badhti, ek giri to baaki
chalti. Keemat: network call me der aur fail, kai DB me ek saath transaction nahi (saga lagta), tracing / monitoring /
deploy bhaari. Sahi raasta: shuru me monolith (saaf hisson me), jis hisse me sach me dard aaye sirf use alag nikaalo.

---

## Kahani: ek bada dafter vs alag-alag chhote office

```
Monolith      = ek bada dafter: sab ek chhat ke neeche, baat seedhe mez se mez (function call)
                ek pipe phata to poora dafter band; ek department badhana ho to poori building badhao
Microservices = har department ka apna chhota office aur apni almari (DB); baat phone / chitthi se (REST / Kafka)
                ek office band to baaki chalte; par phone kat sakta, aur do almari ek saath badalna mushkil
```

---

## Tere project se

- **usercrud** = monolith: ek app, ek MySQL, seekhne ke liye bilkul sahi.
- **mini_payment_ms** = microservices: order aur payment alag service, Feign / Kafka se baat. Do DB me ek transaction nahi,
  isliye **SAGA**; service giri to **circuit breaker**. Yahi microservices ki asli keemat, haath se jheli.
- HLD: payment design me "alag jagah = saga" wala FR 6 yahi baat.

---

## Asli misaal

Netflix 2008 me monolith pe tha; DB kharab hua aur 3 din service ruki. Uske baad dheere-dheere service alag nikaali, cloud
pe poora jaane me kai saal (2016 tak). Ek jhatke me nahi, hissa-hissa.

---

## Trap

Pehle din se microservices bana di: domain samjha nahi tha, to service galat jagah se kati. Har kaam me 4 service ek saath
badalni padti, sab ek saath deploy = **distributed monolith**: dono ki burai, kisi ki achhai nahi.
