# Topic 3 — Deployment

[← Back to README](00_README.md) | [← Pod](02_pod.md) | [Service →](04_service.md)

---

## Koi poochhe to aise bolo

**Deployment kya hai:** pod ka manager. Main bata deta hoon "usercrud ke 3 pod chahiye", Deployment ginti pakad ke rakhta hai: ek pod mara to naya bana deta hai.

**Kyun:** chaar kaam ke liye. Pehla, pod mare to apne aap naya aa jaaye. Doosra, naya version aaye to ek-ek karke purane pod hatata hai aur naye lagata hai, isliye app band nahi hoti. Teesra, naya version kharab nikla to pichhle version pe turant wapas. Chautha, bheed badhe to ginti 3 se 10 kar do, bas.

---

## Kahani: building ka manager

```
Pod        = ek flat (kabhi bhi gir sakta)
Deployment = building ka manager
             "hamesha 3 flat bhare rehne chahiye"
             ek khaali hua  -> turant naya kirayedaar
             marammat karni -> ek naya flat taiyaar, phir ek purana khaali,
                               phir agla; log rehte rahe, building band nahi
```

---

## Naya version kaise aata (bina app band kiye)

```
pehle      [v1] [v1] [v1]
naya aaya  [v1] [v1] [v1] [v2]    v2 taiyaar hua tabhi ek v1 hata
           [v1] [v1] [v2] [v2]
ant me     [v2] [v2] [v2]
```

Naya version kharab nikla to wapas v1, kyunki Deployment pichhle version yaad rakhta hai.

---

## Dikkat jo kaam me aati hai

Sirf 1 copy rakhi to wahi ek pod gira to app band. Isliye asli kaam me kam se kam 2-3 copy.

Ek aur tarika hota hai jisme pehle saare purane hata do, phir naye lagao. Usme beech me app band rehti hai. Isliye wahi tarika rakhte hain jo ek-ek karke badalta hai, wahi default bhi hai.

---

## Dikkat jisse agla tukda bana

Pod marte-bante rehte hain aur har naye pod ka pata (IP) badal jaata hai. To usercrud MySQL ko dhundhe kaise? Iske liye **Service** ([04](04_service.md)).

[← Back to README](00_README.md) | [← Pod](02_pod.md) | [Service →](04_service.md)
