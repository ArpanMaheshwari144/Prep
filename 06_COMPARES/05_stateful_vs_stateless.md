# Stateful vs Stateless — server yaad rakhe ya har baar naya

---

## Koi poochhe to aise bolo

**Stateful** server user ki baat apni memory me yaad rakhta (login session), to agli request usi server pe jaani chahiye
(sticky); wo server gira to user logout. **Stateless** server kuch yaad nahi rakhta: har request apne saath poori pehchaan
laati (JWT token), isliye koi bhi server kisi bhi request ko sambhal leta; LB jahan chahe bheje, server badhao-ghatao, gire
to doosra le le. Asal me "stateless" ka matlab state hi nahi, aisa nahi: state server ki memory se nikaal ke bahar (token,
Redis, DB) rakh di jaati.

---

## Kahani: waiter vs vending machine

```
Waiter (stateful):           "mera usual lao" -> yaad hai biryani, table 5; doosra waiter aaya to "aap kaun?"
Vending machine (stateless): paisa daalo + button dabao -> coke; koi bhi machine, kuch yaad rakhne ki zaroorat nahi
```

---

## Tere design aur project se

- **Twitter kadam 1:** stateless JWT, har request token ke saath, koi bhi server sambhale.
- **todoapp:** JWT login, server me session nahi.
- **Chat kadam 3:** WebSocket ka khula taar ek server se juda = stateful. Isliye "Ravi kis server pe" ka naqsha Redis me
  (session map): state server se nikaal ke bahar.

---

## Dikkat se hal

Session server ki memory me tha, restart hua, saare user logout; upar se LB sticky, bheed barabar nahi bati. Hal: state bahar
(JWT, ya Redis me session); server khaali, badhana-ghatana aasaan.

---

## Trap

JWT stateless hai to logout / "is user ko turant band karo" mushkil: token apni umar tak chalta rahega. Isliye umar chhoti
(15 min) + refresh token; turant band karna ho to Redis me "block list", yaani thoda state wapas.
