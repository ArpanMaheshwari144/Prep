# Topic 4 — Service

[← Back to README](00_README.md) | [← Deployment](03_deployment.md) | [Ingress →](05_ingress.md)

---

## Koi poochhe to aise bolo

**Service kya hai:** pods ke aage ek pakka naam aur pata. Peeche pod marte-bante rahein, unka pata badalta rahe, Service ka naam wahi rehta hai. usercrud MySQL ko `mysql` naam se dhundh leta hai, pod ka pata jaanne ki zaroorat nahi.

**Kyun:** pod ke pate pe bharosa nahi kar sakte, wo badalta rehta hai. Aur 3 pod hon to Service request ko teeno me baant bhi deti hai, chhote load balancer jaisi. Jo pod beemar ho, use line se hata deti hai.

---

## Kahani: restaurant ka phone number

```
Pods    = waiter (roz shift badalti, naye waiter)
Service = restaurant ka ek phone number
          customer number milata, jo waiter khaali ho wo uthata
          waiter badle, number wahi
          customer ko kisi waiter ka naam yaad nahi rakhna
```

---

## Andar ya bahar

Aam Service sirf cluster ke andar dikhti hai: ek service doosri se baat kare (usercrud se MySQL, usercrud se Redis). Bahar wale ko ye seedha nahi dikhti.

Bahar ke liye do raaste hain. Ek jugaad wala, jo har machine ka ek darwaza khol deta hai; ye bas seekhne aur test ke liye, local cluster pe yahi chalaya tha. Doosra, cloud ka asli load balancer, jo prod me lagta hai. Par har service ka alag cloud load balancer mehenga padta hai, aur wahi dikkat agla tukda hal karta hai.

---

## Ek trap

Service apne pods ko naam ki parchi (label) se pehchanti hai. Service ki parchi pe "usercrud" likha ho aur pod pe kuch aur, to Service ke peeche zero pod. Request kahin nahi jaati, aur koi error bhi chilla ke nahi aata. Parchi dono taraf same honi chahiye.

---

## Dikkat jisse agla tukda bana

Service cluster ke andar ka pata hai. Bahar ki duniya (browser) ko andar aane ke liye ek darwaza chahiye, wo **Ingress** ([05](05_ingress.md)).

[← Back to README](00_README.md) | [← Deployment](03_deployment.md) | [Ingress →](05_ingress.md)
