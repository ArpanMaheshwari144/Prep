# Docker 05 — Network (dabbe naam se ek doosre ko kaise dhoondhte)

[← Back to README](00_README.md) | [← Transition](04_transition.md) | [Compose →](06_docker_compose.md)

---

## Koi poochhe to aise bolo

Network dabbon ki ek gali hai: jo dabbe usme hain, wo ek doosre ko naam se bula sakte hain. Docker ka apna bana-banaya
network (default bridge) hota hai, par usme naam se dhoondhna kaam nahi karta, sirf IP se. Isliye hum apna network banate
hain (`app-net`). Usme Docker ka apna register (DNS) chalta hai: "mysql-userdb kahan?" poochho to wo uska IP bata deta.
Compose ye network apne aap bana deta hai, isliye 04 me `mysql:3306` bina kuch kiye chal gaya.

---

## Kahani: apartment ka intercom

```
Docker = apartment building, har dabba ek flat (Spring 101, MySQL 102, Redis 103)

Default gali (bridge):  intercom hai, par sirf flat ka number (IP) dabao, naam se nahi
Apni gali (app-net):    intercom pe naam bolo "MySQL", wo khud 102 mila deta (DNS)
None:                   flat pe taala, kisi se baat nahi (akela batch kaam)
```

---

## Chala ke dekha

- `docker network create app-net`, phir MySQL ka dabba (`mysql-userdb`) `--network app-net` se usme joda. Docker ne IP
  diya `172.18.0.2`.
- Ek chhota MySQL client dabba **usi network** pe chalaya, host me IP ki jagah naam: `-h mysql-userdb`.
- Andar `SELECT * FROM users` chalaya to Arpan wali row aa gayi. Naam se pahunch gaya, IP likhna hi nahi pada.

---

## Dikkat se hal

IP dabba dobara banne pe badal sakta hai, to IP likh ke rakhna toot jaata hai. Hal: apna network banao aur naam se bulao,
naam nahi badalta.

---

## Trap

Do dabbe alag-alag network pe hon, to naam se bulane pe "host nahi mila" aata hai; dono ek hi network pe hone chahiye.
Default bridge pe bhi naam kaam nahi karta, to log sochte hain DNS toota, jabki asal me network galat hota hai.

---

## Aage ki kahani

Ye sab (network banana, dabbe jodna, volume, kaun pehle chale) ek file me likh ke ek command se chalana = docker compose
([06](06_docker_compose.md)).

[← Back to README](00_README.md) | [← Transition](04_transition.md) | [Compose →](06_docker_compose.md)
