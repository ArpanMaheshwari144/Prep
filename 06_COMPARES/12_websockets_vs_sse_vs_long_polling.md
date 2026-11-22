# WebSocket vs SSE vs Long Polling — server ke paas naya aaye to phone ko kaise pata chale

---

## Koi poochhe to aise bolo

Teeno ka kaam ek: server ke paas kuch naya aaye to phone ko turant pata chale. Farak raste me.
**Long polling:** phone poochhta "kuch aaya?", server jawab rok ke rakhta jab tak kuch aaye (ya time khatam), jawab mila to phone
turant dobara poochhta. Sirf HTTP, kahin bhi chal jaata, par baar-baar naya connection. **SSE:** ek HTTP connection khula, server
us pe bhejta rehta, baat sirf server se phone; taar toote to browser khud dobara jod leta. **WebSocket:** ek taar khula, dono
taraf baat (chat, game). Keemat: har khula taar ek server se juda (stateful), isliye "Ravi kis server pe" ka naqsha Redis me.
Niyam: dono taraf baat = WebSocket; sirf server se (score, live update) = SSE; purana / roka hua network = long polling.

---

## Kahani

```
Long polling = darwaze pe khade raho "kuch aaya?"; aaya to lo, phir dobara khade ho jao
SSE          = radio: station bolta rehta, tum sirf sunte
WebSocket    = phone call: line khuli, dono bol sakte
```

---

## Tere design se

- **Chat kadam 3:** WebSocket ka khula taar; Ravi kis server pe = Redis me naqsha (session map), server aapas me pub/sub se.
- **IPL score app (31-Oct):** WebSocket + pub/sub, desk pe aakhri score ki parchi. SSE bhi chal jaata, score sirf ek taraf jaata.
- **BMS payment status:** chhotu baar-baar PSP se poochhta = polling, kyunki PSP hamare saath taar khol ke nahi baithta.

---

## Dikkat se hal

Chat me har 1 second phone se "naya message?" ka simple poll: 10 lakh user = 10 lakh request har second, zyadatar jawab "kuch
nahi". Hal: WebSocket, jab message aaye tabhi bhejo, beech me koi request nahi.

---

## Trap

WebSocket server deploy ke liye restart kiya, us server ke saare taar ek saath toote, saare phone ek hi second me dobara judne
aaye aur naya server dab gaya. Hal: dobara judne me thoda random intezaar (backoff + jitter), server ek-ek karke restart.
