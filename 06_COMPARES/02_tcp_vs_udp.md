# TCP vs UDP — pakka pahuncha vs jaldi pahuncha

---

## Koi poochhe to aise bolo

Dono data bhejne ke tareeke. **TCP** pehle haath milata ("baat karein?" "haan" "chalo"), har tukde ki raseed leta,
khoya to dobara bhejta, kram bhi sahi rakhta: pakka, par thoda dheema. Web page, API, DB connection, email: jahan ek bhi
byte galat nahi chalega, wahan TCP. **UDP** bina haath milaaye bas phenkta jaata, na raseed na dobara bhejna: tez, par
khoya to khoya. Video call, online game, DNS: jahan der se pahuncha data kisi kaam ka nahi, wahan UDP.

---

## Kahani: phone call vs postcard

```
TCP = phone call: "hello?" "haan" "chalo" -> har baat ka jawab, kuch chhoota to "phir se bolo"
UDP = postcard: daak me daal diya, pahunche na pahunche; par ek ke baad ek turant bhej sakte
```

---

## HLD se jod

- **PUBG (20):** goli / chaal UDP pe; 120 ms purani jagah ka koi kaam nahi, nayi jagah hi chahiye, ek packet khoya to
  agla aa hi raha.
- **Chat (14):** WebSocket TCP pe; message khona nahi aur kram sahi chahiye. "Kram number" wala hal TCP ke upar app ka hisaab.
- **Netflix (18):** video pehle se bana, aage tak buffer hota, to TCP theek; thodi der chalegi, har frame sahi chahiye.
- **Video call (Zoom):** UDP; 5 sec der se aaya chehra bekaar, ek frame tootna chalega.

---

## Dikkat se hal

TCP ka haath milana aur raseed real-time me der laata. Hal: jahan der khatarnaak wahan UDP, aur jo thodi pakkayi chahiye
wo app khud upar se jod le (jaise game me "aakhri jagah hi maano").

---

## Trap

"UDP = unreliable to kabhi mat lo" galat soch. HTTP/3 (QUIC) UDP ke upar hi chalta aur pakkayi upar se jodta, kyunki
naya connection jaldi banta aur mobile pe network badalne pe bhi tikta.
