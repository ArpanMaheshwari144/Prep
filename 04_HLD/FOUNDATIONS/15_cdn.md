# CDN — Content Delivery Network

> **NAV** — KYA: CDN (edge cache, kya rakhein, purana kaise hatayein, Open Connect). saath: [caching](04_caching.md) · lagta hai: [file-upload](../SYSTEM_DESIGNS/07_file_upload_validate_system/07_file_upload.md) · [twitter-feed](../SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md)

---

## 1. CDN kya hai aur kaise kaam karta hai

**Koi poochhe to aise bolo:**
```
CDN = duniya bhar me faili cache machines (edge), user ke paas. File pehli baar maangi gayi to edge ke paas
nahi hoti (miss): edge asli server (origin) se laata hai, rakh leta hai, aur user ko de deta hai. Us shehar ka
agla user wahi file edge se paata hai (hit), tez, aur origin ko pata bhi nahi chalta.
Isse teen fayde: user ke liye tez, origin pe bojh kam, aur door ka bandwidth bachta hai.
```

**Scene (Netflix poster):** origin America me hai. Arpan Bangalore PG se app kholta hai, aur "Dark" ka poster 12,000 km se aata hai, ~800 ms me. CDN laga diya: pehli baar Bangalore edge ke paas poster nahi tha, to wo America se laaya, rakh liya, aur Arpan ko de diya. Pehli baar dheema hi raha. Agle 10 lakh log Bangalore edge se ~20 ms me poster paate hain, aur America ko ye request dikhti hi nahi.

**Pull ya push:**
```
pull  edge tab laata hai jab koi maange. Pehla user der jhelta hai. Aam taur pe yahi (CloudFront default)
push  hum pehle se saari edges pe daal dete hain. Bada din pata ho tab: naye season ki release raat,
      sale ka din (Netflix notes ka "raat ko pehle se ISP ke dabbe me bhar do" yahi hai)
```

**Trap:** "origin gira to CDN sab sambhal lega". CDN sirf wahi de sakta hai jo uske paas pehle se rakha hai, aur utni der jitni TTL hai. Nayi cheez, login aur payment sab origin pe hi jaate hain. CDN backup server nahi hai, cache hai.

---

## 2. CDN pe kya rakhein, kya nahi

**Koi poochhe to aise bolo:**
```
Jo sabke liye ek jaisa hai (poster, JS, CSS, video, font), wo CDN pe lambi der tak. Jo ek user ka apna hai
(account page, cart, order) ya har second badalta hai (share ka daam), wo cache nahi hota, origin se aata hai.
Ye origin header se batata hai: public, max-age = CDN rakh le; private = sirf browser; no-store = koi nahi.
Paid / private file bhi CDN se chahiye to signed URL: file + kab tak valid + sign, galat ya purana link = 403.
```

**Signed URL (file upload kadam 6b se):** Ravi ki bank statement PDF CDN se tez aaye, par sirf Ravi ko mile. Server 10 minute ka signed link banata hai. CDN sign check karta hai, theek hai to file de deta hai. 10 minute baad wahi link 403 deta hai.

**Dynamic bhi CDN se guzar sakta hai:** cart cache nahi hoti, par request phir bhi paas wali edge se guzarti hai. TLS ka taala Bangalore me hi khul jaata hai, aur edge ka origin tak pehle se khula, garam taar hota hai. Isse raasta tez hota hai, bina kuch cache kiye.

**Trap (galat user ko galat data):** account page pe `private` lagana bhool gaye. CDN ne Arpan ka "My Account" rakh liya, aur Ravi ko Arpan ka page dikh gaya. Steam ke saath Christmas 2015 pe yahi hua tha: caching ki setting badli, aur logon ko doosron ke account page dikhe. Isliye user ke apne data pe hamesha `private` / `no-store` lagao, aur login wale jawab kabhi cache mat karo.

---

## 3. Purana maal CDN se kaise hatayein

**Koi poochhe to aise bolo:**
```
Teen raaste hain. TTL: itni der baad edge khud purana phenk de, simple hai par tab tak purana dikhega.
Versioned naam: app.3f9a.js, file badli to naam badla, to naya cache, purana kabhi galat dikhega hi nahi.
Isliye aise file pe TTL 1 saal, aur sirf index.html (jo naye naam batata hai) pe chhota TTL.
Purge: CDN ko bolo "ye file har edge se hatao", emergency ke liye, seconds se minute lagte hain.
```

**Scene (Netflix logo badla):** naya logo daala, par Bangalore edge ke paas purana rakha hai. TTL 1 ghanta hai to 1 ghante tak purana dikhega. Agar naam `logo-v2.png` kar diya to naya page naya naam maangega, aur edge ke paas wo hai hi nahi, to wo origin se naya le aayega. Galat logo pe legal ka notice aaya to purge karo.

**Trap (deploy ke baad page toota):** naya HTML aa gaya, par CDN pe purana JS pada hai, jisme naya function hai hi nahi, to page toot gaya. Ilaaj: versioned naam. Tab purge ki zaroorat lagbhag khatam ho jaati hai.

---

## 4. Bas itna yaad (CDN gira / sab miss)

```
CDN 95% bojh edge pe khatam karta hai. Sab ek saath miss hua (purge all, CDN hi gira) to origin pe achanak poora bojh.
Bolna itna hi: origin ke aage ek shield parat, aur ek file ki kai miss pe origin pe ek hi request.
Andar ki baarik baatein (cache key, error cache) design ka faisla nahi, CDN wale sambhalte hain.
```

---

## 5. Netflix Open Connect + CDN ke naam

**Koi poochhe to aise bolo:**
```
Netflix ne kisi aur ka CDN nahi liya, apna banaya: Open Connect. Apne dabbe seedha ISP (Jio, Airtel) ke
data center me rakhe. Raat ko jab net khaali hota hai, tab naya maal un dabbon me bhar diya jaata hai, aur
din me video ISP ke andar se hi chal jaata hai. Isse Netflix ka bandwidth bachta hai, ISP ka bhi, aur video jaldi shuru hota hai.
```

**Naam:**
```
CloudFront   AWS ka, AWS ke saath aasaan
Akamai       sabse purana aur bada
Cloudflare   free tier + DDoS se bachaav
Fastly       turant purge
Open Connect Netflix ka apna
```
