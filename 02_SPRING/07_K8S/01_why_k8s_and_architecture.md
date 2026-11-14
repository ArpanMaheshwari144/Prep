# Topic 1 — Kubernetes kya hai, kyun

[← Back to README](00_README.md) | [Pod →](02_pod.md)

---

## Koi poochhe to aise bolo

**Kubernetes kya hai:** Docker se app ek dabbe (container) me band hoti hai, par Docker ek machine tak hi kaam aata hai. Jab 50 machine pe 100 dabbe chalane hon, koi dabba gir jaaye to use dobara uthana ho, bheed badhe to aur copy chahiye, aur naya version bina app band kiye daalna ho, tab ye sab Kubernetes sambhalta hai. Isliye wo Docker ka bhagwan hai: Docker dabba banata hai, Kubernetes bahut saare dabbe chalata aur sambhalta hai.

**Kyun use karte hain:** main use bas bata deta hoon "mujhe usercrud ki 3 copy chahiye". Wo baar-baar dekhta rehta hai ki kitni chal rahi hain, aur 2 bachi to ek nayi bana deta hai. Raat 3 baje crash hua to koi insaan uthane nahi baithta, Kubernetes khud uthata hai. Compose ye nahi karta, wo ek machine ka hai aur khud se kuch nahi sambhalta.

---

## Kahani: restaurant ki chain

```
Docker      = ek kitchen, ek dish banata hai
Compose     = ek restaurant, kai kitchen ek jagah
Kubernetes  = 50 shehar me poori chain
              rush aaya      -> aur kitchen khul jaati
              kitchen tooti  -> nayi lag jaati
              menu badla     -> har jagah dheere-dheere badal jaata
```

---

## Andar kya hai (bas itna)

Ek **dimaag** hota hai aur kai **kaam karne wali machines**.

Dimaag ke teen kaam hain. Pehla, likh ke rakhna ki kya chahiye (3 copy). Doosra, tay karna ki naya dabba kis machine pe jaaye. Teesra, baar-baar milana ki jo chahiye aur jo chal raha hai wo barabar hai ya nahi; barabar nahi to theek karna.

Kaam wali machines pe dabbe asal me chalte hain. Har machine pe ek chhota agent hota hai jo dimaag ki baat sunta hai aur batata rehta hai ki uske dabbe theek hain ya nahi.

Ye wahi "jo hona chahiye vs jo hai, milao" wala kaam hai jo banking design me sync job karta tha. Bas yahan dabbon ke liye.

---

## Kisne banaya

Google ne banaya. Google apne andar saalon se karodon dabbe ek system se chalata tha, jiska naam Borg tha; usi ke tajurbe se Kubernetes bana. 2014 me Google ne ise open source kiya, aur ab ise CNCF (Linux Foundation ke neeche) sambhalti hai, kisi ek company ka nahi. AWS pe EKS, Google pe GKE, Azure pe AKS, sab ise taiyaar chala ke dete hain. Naam Greek ka hai, matlab "jahaz chalane wala"; K8s isliye kyunki K aur s ke beech 8 akshar.

---

## Koi poochhe "Kubernetes ne Docker hata diya?"

Kubernetes ab dabbe chalane ke liye Docker ka poora engine beech me nahi rakhta, seedha ek chhote runtime se chalata hai. Par Docker se banayi image waise hi chalti hai, kyunki image ka format sabka ek hai. Matlab image banane me Docker ab bhi kaam aata hai.

---

## Aage ki kahani

Kubernetes dabbe ko seedha nahi pakadta, use **Pod** me rakhta hai (02). Pod ka manager **Deployment** hai (03). Pods ko pakka pata **Service** deti hai (04). Bahar se andar aane ka darwaza **Ingress** hai (05). Config aur password bahar **ConfigMap aur Secret** me rehte hain (06).

[← Back to README](00_README.md) | [Pod →](02_pod.md)
