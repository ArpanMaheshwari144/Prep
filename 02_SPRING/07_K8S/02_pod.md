# Topic 2 — Pod

[← Back to README](00_README.md) | [← Why K8s](01_why_k8s_and_architecture.md) | [Deployment →](03_deployment.md)

---

## Koi poochhe to aise bolo

**Pod kya hai:** Kubernetes dabbe (container) ko seedha nahi pakadta, use ek kamre me rakhta hai. Wo kamra Pod hai. Pod me aam taur pe ek hi dabba hota hai, kabhi saath me ek helper bhi (jaise logs bhejne wala). Pod ko ek pata (IP) milta hai, aur ek pod ke andar wale dabbe aapas me ghar ke andar ki tarah baat kar lete hain.

**Kyun:** kabhi app ke saath ek helper ko bilkul paas chahiye, same pata aur same files ke saath. Pod ye jodi ek saath chalata aur ek saath hatata hai.

---

## Kahani: flat

```
Container = ek banda
Pod       = ek flat
            aam taur pe ek banda rehta
            kabhi saath me ek roommate (helper)
            flat ka ek hi pata, kitchen bhi ek (files share)
```

Misaal: app log likhti hai, aur saath wala helper wahi log padh ke bahar bhej deta hai. Dono ek flat me, isliye ek doosre ki file seedha padh lete hain.

---

## Dikkat jisse agla tukda bana

Dabba crash ho to Kubernetes usi flat me use dobara chala deta hai. Par flat hi gir jaaye (pod hata diya, ya machine band ho gayi) to koi naya flat nahi banata. Pod gaya to app gayi.

Isliye pod ko kabhi akela nahi chalate. Uske upar ek manager rakhte hain jo ginti pakad ke rakhe, wo hai **Deployment** ([03](03_deployment.md)).

---

## Ek baat jo khud dekhi

usercrud chalaya to pod ban gaya, par uske andar ka dabba shuru hi nahi hua, kyunki jo image chahiye thi wo cluster ko mili hi nahi. Sabak: pod ka bana hona aur app ka chalna do alag baatein hain, "kitne taiyaar" wala hissa dekho. Image ko Docker Hub pe daala, tab chala.

[← Back to README](00_README.md) | [← Why K8s](01_why_k8s_and_architecture.md) | [Deployment →](03_deployment.md)
