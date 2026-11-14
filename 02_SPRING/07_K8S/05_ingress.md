# Topic 5 — Ingress

[← Back to README](00_README.md) | [← Service](04_service.md) | [ConfigMap+Secret →](06_configmap_secret.md)

---

## Koi poochhe to aise bolo

**Ingress kya hai:** cluster ka main gate. Bahar se (browser se) aayi har request yahin aati hai, aur Ingress batata hai ki `/users` wali request usercrud ki Service pe jaaye aur `/todo` wali todo ki Service pe. Ye Kubernetes ke andar ka NGINX hai, wahi kaam: aage khada hoke raasta dekh ke request aage bhejna.

**Kyun:** 5 service hon aur har ek ka alag cloud load balancer kholo to 5 guna kharcha, 5 alag pata, har ek pe alag HTTPS certificate. Ingress me ek gate, ek domain, raste ke hisaab se aage bhejo, aur HTTPS bhi yahin ek jagah.

---

## Kahani: office ka reception

```
bina reception: har department ka alag darwaza, kaun kahan ja raha koi nahi dekhta
reception ke saath (Ingress):
   sab ek main gate se aate
   "users? teesri manzil"  "orders? paanchvi"  "payment? saatvi"
   security check ek jagah (HTTPS)
   register me sab likha: kaun aaya, kahan gaya, kya hua (NGINX jaise access log)
```

---

## Kaam se jod

Office me AWS pe NGINX aage khada hota hai aur request ko raaste ke hisaab se sahi server pe bhejta hai, aur uske access log me IP, URL, status sab aata hai. Ingress wahi cheez hai, bas Kubernetes ke andar. Local cluster pe bahar se pahunchne ke liye jugaad wala darwaza khola tha; prod me cloud load balancer + Ingress lagta hai.

---

## Ek trap

Ingress me sirf niyam likhe hote hain ("ye raasta us service pe"). Un niyamon pe chalne wala asli gatekeeper (jaise NGINX) cluster me alag se lagana padta hai. Niyam likh diye, gatekeeper nahi lagaya, to kuch nahi hota. Jaise reception ka register rakh diya par reception pe koi baitha hi nahi.

---

## Aage ki kahani

App chal rahi hai aur bahar se pahunch rahi hai. Bacha: DB ka pata aur password kahan rakhein. Wo **ConfigMap aur Secret** ([06](06_configmap_secret.md)).

[← Back to README](00_README.md) | [← Service](04_service.md) | [ConfigMap+Secret →](06_configmap_secret.md)
