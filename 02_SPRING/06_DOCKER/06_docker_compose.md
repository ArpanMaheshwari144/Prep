# Docker 06 — Compose (poori thali ek file se)

[← Back to README](00_README.md) | [← Networks](05_networks.md) | [Reference →](07_reference.md)

---

## Koi poochhe to aise bolo

Compose ek file hai (`docker-compose.yml`) jisme likha hota hai ki mere system me kaun-kaun se dabbe hain (MySQL,
Spring), kaun kis volume aur port se juda hai, aur kaun pehle chalega. Phir ek hi command (`docker-compose up`) se poora
system chal jaata hai: network apne aap banta hai, dabbe naam se ek doosre ko dhoondh lete hain. Haath se 5 command yaad
rakhne ki zaroorat nahi, aur same file kisi bhi machine pe same system khada kar deti hai.

---

## Kahani: thali ki recipe

```
docker run      = ek-ek dish chulhe pe alag banao (network, volume, MySQL, Spring: 5 command, kram yaad rakho)
compose file    = poori thali ki recipe ek kaagaz pe; "thali laga do" bolo, sab ek saath
depends_on      = "dal pehle, roti baad me": MySQL taiyaar ho tab Spring
```

---

## Chala ke dekha (usercrud me)

- Pehli baar `docker-compose up -d --build` fail: `openjdk:17-jdk-slim: not found`. Docker Hub pe `openjdk` wali image
  band ho chuki.
- Dockerfile me `eclipse-temurin:17-jdk-jammy` kiya (wahi Java, doosre ki packing). Dobara chalaya to chal gaya.
- Compose ne apne aap `usercrud_default` network banaya, pehle MySQL chalaya, uske "healthy" hone ka intezaar kiya, phir
  Spring.
- Spring ke log me `profile is active: "compose"`, aur Postman se login pe 200 OK + token.

---

## Dikkat se hal

Dikkat: Spring pehle jaag gaya, MySQL abhi taiyaar nahi, "connection refused". Hal: MySQL pe ek jaanch (healthcheck:
"zinda hai?") aur Spring ke liye shart "MySQL healthy ho tab chalo".

Profile kaise badli: compose ne dabbe me env var daala `SPRING_PROFILES_ACTIVE=compose`; Spring ne use padh ke khud
`application-compose.properties` uthayi (url `mysql:3306`). File Spring ne load ki, compose ne sirf env var diya.

---

## Trap

**Purana volume bhoola:** volume pe `external: true` nahi likha, to compose ne naya khaali volume bana diya, kal ka Arpan
wala data dikha hi nahi. Purani almari jodni ho to batana padta hai ki wo pehle se bani hai.

**Purani image:** Dockerfile badli par sirf `up` chalaya, to purani image hi chali. Badlaav ke baad `--build`.

---

## Aage ki kahani

Compose ek machine pe chalta hai. Bahut saari machine pe dabbe sambhalne ho to Kubernetes (K8S 01 me ho chuka).

[← Back to README](00_README.md) | [← Networks](05_networks.md) | [Reference →](07_reference.md)
