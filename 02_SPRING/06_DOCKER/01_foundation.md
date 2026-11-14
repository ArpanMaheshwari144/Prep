# Docker 01 — Docker kya hai, kyun (image vs container)

[← Back to README](00_README.md) | [Spring + MySQL →](02_spring_mysql_host.md)

---

## Koi poochhe to aise bolo

**Docker kya hai:** Docker ek tool hai jo meri app ko uske saare saamaan (Java ka version, jar, config) ke saath ek dabbe me band kar deta hai. Is dabbe ko container kehte hain, aur jis saanche se dabba banta hai use image kehte hain.

**Kyun use karte hain:** pehle "mere laptop pe chal raha, server pe nahi" wali dikkat hoti thi, kyunki har machine pe Java ka version aur library alag hoti hai. Container me sab kuch saath band hai, isliye laptop, test server aur prod, teeno jagah app ek jaisi chalti hai. Naya banda team me aaye to ek command se usercrud aur MySQL chal jaate hain, setup me din nahi lagte. Aur ye VM se halka hai, kyunki poora OS nahi uthata, machine ka andar wala hissa (kernel) share karta hai, isliye second me chalu ho jaata hai.

---

## Kahani: recipe aur dish

```
Image     = recipe (kaagaz pe likhi, badalti nahi)
Container = us recipe se bani dish (asli, chalti hui)
            ek recipe se 100 dish, har dish alag
```

Java se jodo to image = class, container = object. Ek mysql image se teen alag MySQL chala sakte ho, har ek ka apna data.

Container band (stop) kiya to mita nahi, dobara chalu karo to data wahi. Hata diya (delete) tab jaake gaya.

---

## Image tukdon me aati hai

MySQL ki image utaari to wo ek saath nahi, kai tukdon (layer) me aayi: neeche OS, upar MySQL, upar config. Fayda: agli baar milti-julti image utaaro to jo tukde pehle se paas hain wo dobara nahi aate, sirf naye wale. Isliye doosri baar tez.

---

## Do baatein jo khud dekhi

**Port pehle se ghira tha:** MySQL ka dabba laptop ke 3306 darwaze pe chalaya to error aaya, kyunki laptop pe pehle se ek MySQL wahi darwaza le ke baitha tha. Ek darwaza ek hi ke paas reh sakta hai. Hal: laptop ka 3307 darwaza dabbe ke andar ke 3306 se jod diya. Ab Spring laptop pe `localhost:3307` se baat karta hai, andar MySQL apne 3306 pe hi hai.

**Dabba chalu, par MySQL abhi taiyaar nahi:** dabba "chal raha" dikha, par MySQL pehli baar uthne me 15-20 second leta hai (database banana, user banana). Us beech Spring ne jodne ki koshish ki to "connection refused". Isliye dabbe ke log dekho, "ready for connections" aaye tab maano taiyaar. Dabba chalna aur andar ki app taiyaar hona alag baatein hain (K8s me pod ke saath bhi yahi tha).

---

## Aage ki kahani

MySQL dabbe me chal gaya. Ab Spring laptop pe chala ke is dabbe wale MySQL se jodna hai ([02](02_spring_mysql_host.md)).

[← Back to README](00_README.md) | [Spring + MySQL →](02_spring_mysql_host.md)
