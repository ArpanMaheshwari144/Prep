# Notification: company ne 429 diya, chala ke dekha

Design file: [04 notification](../../SYSTEM_DESIGNS/04_notification_system/04_notification_system.md)

Galti jo hui thi: 429 pe "fail-open" bola. Fail-open HUMARE limiter ka faisla hai (Redis gira to). Yahan had
COMPANY ki hai, uska darwaza hum nahi khol sakte, dheema HUMEIN hona padega.

CODE: `04_HLD/HANDS_ON/03_provider_429/Sms429Demo.java` (Docker nahi, time nakli)
· line 21 `SMART = false / true`
-> `java Sms429Demo.java`

```
SETUP:  1000 SMS ek saath · company 100 / sec · t=3,4 pe 50 / sec
NAIVE:  sab ek saath, 429 pe agle second turant dobara, 3 fail -> PHENK DO
SMART:  apni taraf 100 / sec (token bucket) · 429 pe 1s, 2s, 4s · kabhi phenko mat

NAIVE   PAHUNCHE 300 / 1000 · CHHODE 700 · call 2700 (429 = 2400) · 3 sec me khatam
SMART   PAHUNCHE 1000 / 1000 · CHHODE 0 · call 1100 (429 = 100) · 11 sec, dheere par sab pahunche
```

Jawab: chala ke dekha, company ki had ka dhyaan na rakha aur turant dobara bheja to 1000 me se 700 gaye hi
nahi. Had me bheja aur ruk-ruk ke dobara koshish ki to poore 1000 pahunche.
