# Docker 04 — localhost ka matlab badal gaya (Spring bhi dabbe me)

[← Back to README](00_README.md) | [← Volumes](03_volumes.md) | [Networks →](05_networks.md)

---

## Koi poochhe to aise bolo

Jab Spring laptop pe tha, wo MySQL ke dabbe tak `localhost:3307` se pahunchta tha, kyunki laptop ka 3307 port dabbe ke
andar ke 3306 se juda tha. Jab Spring khud dabbe me gaya, to uske liye `localhost` ka matlab hai "main khud, mera apna
dabba", aur uske andar MySQL hai hi nahi. Isliye ab Spring MySQL ko uske **naam** se bulata hai: `mysql:3306`. Dono dabbe
ek hi network pe hain, aur Docker naam sun ke batata hai ki wo dabba kahan hai.

---

## Kahani: hostel ke kamre

```
Pehle: Arpan hostel ke bahar (laptop), MySQL kamra 3306 me.
       Gate pe board: "3307 pe aao, andar 3306 pe bhej denge"  -> localhost:3307 (port mapping)

Ab:    Arpan bhi hostel ke andar, apne kamre me.
       "localhost" = apna hi kamra, wahan MySQL nahi -> connection refused
       Hostel ka register (Docker DNS): "mysql kahan?" -> "kamra 3306"
       Andar wale seedha naam se milte, gate ka board nahi chahiye
```

---

## Chala ke dekha (usercrud me)

- Spring laptop pe, url `localhost:3307`: chal gaya.
- Spring dabbe me, url wahi `localhost:3307` rehne diya: "connection refused", kyunki Spring ke dabbe me koi MySQL nahi.
- Url `mysql:3306` kiya (compose wali profile me): chal gaya. `mysql` = compose file me MySQL service ka naam.

---

## Dikkat se hal

Dikkat: `localhost` ka matlab jagah ke hisaab se badal jaata hai. Hal: dabbe se dabbe tak naam se baat karo aur andar ka
port (3306) do. Bahar wala 3307 sirf laptop ke liye hai.

---

## Trap

Spring ko dabbe me daala aur url me `localhost` chhod diya, to "connection refused" aata hai, jabki MySQL sahi chal raha
hota hai. Log samajhte hain MySQL gira, asal me Spring apne hi kamre me dhoondh raha hota hai.

---

## Aage ki kahani

Dabbe ek doosre ko naam se kaise dhoondhte hain, wo network ka kaam hai ([05](05_networks.md)).

[← Back to README](00_README.md) | [← Volumes](03_volumes.md) | [Networks →](05_networks.md)
