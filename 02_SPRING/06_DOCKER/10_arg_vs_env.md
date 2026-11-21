# Docker 10 — ARG vs ENV (banate waqt ki value vs chalate waqt ki value)

[← Back to README](00_README.md) | [← Multi-stage](09_multistage_builds.md)

---

## Koi poochhe to aise bolo

Dockerfile me value dene ke do tareeke, fark waqt ka hai. **ARG** sirf image banate waqt (`docker build`) kaam aata:
kaunsa Java version lena, kaunsa jar copy karna. Image ban gayi to chalte dabbe ko ARG dikhta hi nahi. **ENV** chalte dabbe
ke andar rehta, app use padh sakti (Java me `System.getenv()`): kaunsi Spring profile, kaunsa time zone. ENV ki default
Dockerfile me likh do, chalate waqt `-e` ya compose ke `environment` se badal do. Image wahi, sirf config badli.

---

## Kahani: rasoi ka masala vs mez ka namak

```
ARG = rasoi ka masala: khana banate waqt dala, khana bana, masala alag se mez pe nahi aata
ENV = mez pe rakha namak-chatni: khaate waqt bhi saamne, mehmaan chahe to badal le
```

---

## Kahan dekha

06 compose me `SPRING_PROFILES_ACTIVE=compose` yahi ENV tha: compose ne chalate waqt diya, Spring ne padh ke profile
badli. Same image dev me `compose` profile se, prod me `prod` se.

---

## Dikkat se hal

Har mahaul (dev, test, prod) ke liye alag image banana bekaar. Hal: ek image, mahaul ka farak ENV se.

---

## Trap

Password / token ARG ya ENV me daala to wo image ke itihaas (`docker history`) me aur dabbe ke andar dikh jaata; koi bhi
image utha ke padh le. Raaz (secret) ke liye alag tareeka (secret store, chalate waqt bahar se), Dockerfile me kabhi nahi.

[← Back to README](00_README.md) | [← Multi-stage](09_multistage_builds.md)
