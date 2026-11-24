# DNS — Domain Name System

> **NAV** — KYA: DNS (naam se IP, cache + TTL, Route 53 se region chunna). saath: [LB](03_load_balancing.md) · [CDN](15_cdn.md)

---

## 1. Naam se IP kaise milta hai

**Koi poochhe to aise bolo:**
```
DNS = internet ki phonebook. Hum netflix.com likhte hain, machine ko IP chahiye. Phone pehle apne cache me
dekhta hai, phir ISP ke resolver se poochta hai. Resolver ke paas na ho to wo upar se neeche poochta hai:
root ("com walon se poocho"), phir .com ("netflix ka register ye hai"), phir Netflix ka apna DNS, jo IP batata hai.
Jawab har jagah TTL tak cache rehta hai, isliye agli baar ye poora safar nahi hota.
```

**Scene:** Arpan ne pehli baar netflix.com khola. Jio ka resolver root se poochta hai, phir .com se, phir Netflix ke DNS se, aur IP mil jaata hai. Ye pehli baar ~100 ms laga. TTL 5 minute hai, to agle 5 minute tak PG ke sab log Jio resolver ke cache se turant IP paate hain.

**Do record yaad rakho, baaki naam hain:**
```
A      naam -> IP          (netflix.com -> 54.x.x.x)
CNAME  naam -> doosra naam  (www.shop.com -> shop.cloudfront.net, CDN pe bhejne ke liye yahi lagta hai)
```

**Browser cache clear kiya to site dheere kyun:** browser site ki JS, CSS, photo, font apne paas rakhta hai. Clear kiya to pehli baar sab dobara net (CDN) se aata hai, aur kai baar DNS ka sawaal aur naya TLS connection bhi dobara. Ek baar khuli to cache phir bhar gaya, agli baar tez. (Cookies bhi saaf ki to login dobara, wo session mitna hai, dheema-tez nahi.)

**Trap:** IP badla, par log purane IP pe hi ja rahe hain, kyunki unke resolver ke paas purana jawab TTL tak pada hai. Isliye bade badlaav se ek din pehle TTL ghata ke 60 sec kar do. Badlaav ke baad sab jaldi naye IP pe aa jaate hain, aur kaam ho jaaye to TTL wapas badha do.

---

## 2. DNS se region kaise chunte hain (Route 53)

**Koi poochhe to aise bolo:**
```
DNS sirf IP nahi batata, ye bhi tay kar sakta hai ki kaunsa IP de. Route 53 me chaar kaam ke tareeke:
latency = user ko sabse paas wala region, geo = desh ke hisaab se (data India me hi rahe),
weighted = 10% log naye version pe (canary), failover = primary ka health check fail to doosra region.
Par DNS ka jawab TTL tak cache rehta hai, isliye DNS se badlaav seconds me nahi, minute me pahunchta hai.
```

**Scene (do region, Mumbai aur Singapore):** Arpan Bangalore se khole to latency routing Mumbai ka IP deti hai. Naya version aaya to weighted routing se 10% logon ko naye IP pe bheja, kuch toota nahi to dheere-dheere 100%. Mumbai region poora gira, to Route 53 ka health check fail hua aur ab sabko Singapore ka IP milne laga.

**Anycast (ek line):** ek hi IP, par duniya ke kai sheharon me alag machine us IP pe jawab deti hai, aur net khud paas wali tak le jaata hai. DNS ke root server aur Cloudflare aise hi chalte hain.

**Trap:** "region gira to DNS failover turant". Nahi, jinke resolver ke paas Mumbai ka IP TTL tak pada hai, wo tab tak Mumbai pe hi jaate rahenge. Isliye failover wale record pe TTL chhota rakho (maan lo 60 sec), aur ye maan ke chalo ki failover me ek-do minute lagenge. Aur DNS ko LB mat samjho, region ke andar asli LB chahiye ([LB tukda 6](03_load_balancing.md)).
