# Topic 6 — ConfigMap + Secret

[← Back to README](00_README.md) | [← Ingress](05_ingress.md)

---

## Koi poochhe to aise bolo

**ConfigMap / Secret kya hai:** config aur password image ke andar nahi likhte, bahar rakhte hain. Aam config (DB ka pata, kaunsa profile) ConfigMap me, aur password, key Secret me. Pod chalte waqt ye bahar se andar daal diye jaate hain, aur app inhe waise hi padhti hai jaise Compose me `environment` wali values padhti thi. Ye Spring ke `application-{profile}.properties` jaisa hi kaam hai, bas Kubernetes ke level pe.

**Kyun:** ek hi image dev pe bhi chale aur prod pe bhi, sirf bahar ki config badle. Aur password image me likha to jiske paas image, uske paas password.

---

## Kahani: packed khaana

```
Image     = band packet wala khaana (seal, koi nahi kholta)
ConfigMap = packet ke upar ki parchi (kaise garam karna, sabko dikh sakti)
Secret    = ghar ka khaas masala jo taale me rehta
            parosne ke waqt dono milte, packet wahi rehta
```

---

## Kaam se jod

Spring profiles me dekha tha: CLI se profile do, ya env se, ya file se, app wahi uthati hai. Compose me `SPRING_PROFILES_ACTIVE` aur DB ka password `environment` me diye the. Kubernetes me wahi do cheezein ConfigMap aur Secret se aati hain. Tarika naya, baat wahi.

---

## Dhyaan (yahi sabse zyada poocha jaata)

Secret apne aap me taala nahi hai. Wo sirf ulta-pulta likha hota hai (base64), jise koi bhi seedha wapas padh sakta hai. Asli bachav do cheez se aata hai: kaun padh sakta hai uski rok, aur bade kaam me alag tijori (AWS Secrets Manager, Vault), jahan se password chalte waqt aata hai.

---

## Ek trap

ConfigMap badla aur socha app turant nayi value le legi. Nahi leti. Jo value env ban ke andar gayi thi, wo pod ke shuru hone ke waqt hi padhi gayi thi. Nayi value chahiye to pods ko dobara chalao (Deployment se ek-ek karke, app band nahi hogi).

---

## Poori kahani ek saans me

Docker dabba banata hai. Kubernetes bahut saare dabbe sambhalta hai. Dabba Pod me rehta hai. Pod ka manager Deployment, jo ginti rakhta aur bina band kiye naya version laata hai. Pods ka pakka naam Service. Bahar ka darwaza Ingress. Config aur password bahar ConfigMap aur Secret me.

[← Back to README](00_README.md) | [← Ingress](05_ingress.md)
