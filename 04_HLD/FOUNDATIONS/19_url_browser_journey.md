# "Type amazon.com -> Enter" — Browser Journey

> **NAV** — KYA: Enter dabane ke baad kya hota (concept-walkthrough, design nahi). CONCEPTS: [load-balancing](03_load_balancing.md) · [DNS](16_dns.md) · [CDN](15_cdn.md)

## Koi poochhe to aise bolo

Browser pehle URL todta hai: kaunsi site, kaunsa path. Phir DNS se pata karta hai ki amazon.com ka IP kya hai. Us IP se TCP connection banta hai, aur https hai to TLS se taala lagta hai. Phir GET request jaati hai, beech me CDN aur load balancer aate hain. Server jawab bhejta hai aur browser page bana ke dikhata hai.

## Kahani (chitthi bhejne jaisa)

```
   [TU — Browser]
        │  URL type + Enter
        ▼
   1. URL PARSE      kaunsi site, kaunsa path
        ▼
   2. DNS LOOKUP     amazon.com -> 52.94.x.x  (naam do, number lo)
        ▼
   3. TCP            darwaza khatkhatana: hello - hello - ok
        ▼
   4. TLS            taala + pehchaan patra (https)
        ▼
   5. HTTP           GET /   (photo CDN se, baaki LB se kisi server pe)
        ▼
   6. SERVER         DB se data -> jawab -> browser page banata
```

1. **URL parse:** tu chitthi pe pata likhta hai (amazon.com/products).
2. **DNS:** dakiya ko naam se ghar nahi milta, number chahiye. DNS phone book hai: naam do, IP lo. Pehle browser apni yaad (cache) dekhta hai, phir resolver se poochta hai.
3. **TCP:** darwaza khatkhatana. "Hello", "hello", "ok", ye 3 baar ki baat-cheet hoti hai.
4. **TLS:** darwaze pe taala aur pehchaan patra, taaki beech me koi chitthi padh na sake.
5. **HTTP:** ab asli chitthi jaati hai (`GET /`). Photo jaisi bhaari cheezein CDN se aati hain, baaki LB se kisi ek server pe.
6. **Server:** DB se data nikaal ke jawab bhejta hai, browser HTML se page banata hai.

## Ek trap

Interviewer poochhe "DNS hamesha har baar hota hai?" To jawab hai nahi. Browser, OS aur resolver, teeno cache rakhte hain. Isiliye doosri baar site jaldi khulti hai.

## Tasveer (ByteByteGo / Alex Xu, unki site se, copy nahi)

![What Happens When You Type a URL Into Your Browser?](https://assets.bytebytego.com/diagrams/0393-type-a-url-into-your-browser.png)

Source: [What Happens When You Type a URL Into Your Browser?](https://bytebytego.com/guides/what-happens-when-you-type-a-url-into-your-browser/) · License CC BY-NC-ND 4.0
