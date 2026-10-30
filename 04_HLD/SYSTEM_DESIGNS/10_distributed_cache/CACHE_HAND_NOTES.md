# Cache: hand notes (Arpan aur Ravi, profile page)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Scene:** Arpan ki app me har bande ka profile page hai (naam, photo, bio). Ravi mashhoor hai, uska profile din me lakhon log kholte hain. Data DB me hai, aur har baar server DB se padh ke laata hai.
**Shabd:** DB se pehle dabba = cache (cache-aside) · purani parchi nikalo = LRU · tod do = sharding (hash) · paas wala bandaa (kho-kho) = consistent hashing · parchi kaato + check karne wala job = cache DELETE + sync job · chhotu = single flight (lock) · chhotu gira to doosra = lock ki umar · ek machine pe bheed = hot key, key ki copies

URL / rate limiter se aaya: ring, hash, Redis. Naya: LRU, cache DELETE + sync job, single flight, hot key.

&nbsp;

**1. Ravi ka profile har second hazaaron baar, har baar DB, DB ki saans phooli**
Jo data bahut padha ja raha hai aur badalta kam hai, use DB se pehle ek dabbe me rakh lo. Koi maange to dabbe se de do, baar-baar DB jaane ki zaroorat nahi.
**cache (cache-aside)**
Interview: "Pehle cache me dekho. Mila (hit) to wahin se, nahi mila (miss) to DB se laao, cache me rakho, phir do."

**2. Dabba (64 GB RAM) bhar gaya, Priya viral, uski jagah nahi**
RAM thodi khaali karo. Jo purana pada hai, jise koi use nahi kar raha, wo bekaar jagah ghere hai, use nikaal do.
**eviction: LRU**
Interview: "Dabba bhara to wo key nikalo jise sabse lambe waqt se kisi ne nahi chhua (Least Recently Used)."

**3. 10 crore users, roz wala data 1 TB, ek machine me nahi aata**
Tod do. 10 crore ko 2-2, 3-3 crore me baant ke alag dabbon me rakho, kam pade to aur todo. Kaunsi key kahan jaayegi, wo hash batata hai, same key hamesha same jagah.
**sharding (hash se)**
Interview: "Data kai cache machines me baanto, key ka hash batata hai machine. Ravi ki key hamesha ek hi machine pe."

**4. 11vi machine judi, % 10 se % 11, har key ki jagah badli, sab miss, DB baitha**
Machines ek ring pe paas-paas baitha do. Key ka hash do machine ke beech gira to sabse paas wali pe chali jaati hai, jaise kho-kho me sabse paas wale ko dhakka, ya headlight kharab ho to sabse paas wale padaav pe ruk jaao, 100 km aage nahi.
**consistent hashing (ring)**
Interview: "Ring pe key ghadi ki disha me pehli machine pe. Nayi machine aaye to sirf uske paas wali keys hilti hain, sab nahi."

**5. Ravi ne naam badla "Ravi Kumar", parchi pe abhi bhi "Ravi"**
Naam badla to apni parchi se "Ravi" kaat do, DB me "Ravi Kumar" likh do. Agli request ko parchi pe kuch nahi milega, wo DB se "Ravi Kumar" utha ke parchi pe likh degi. DB hi sach hai. Beech me koi purana utha le to ek job hai jo parchi aur DB milata rehta hai, farak mila to parchi theek. Profile ka naam paisa nahi, thodi der purana dikh gaya to chalega. TTL alag cheez hai: thodi der wala data (adhoora form) chhote TTL se apne aap hate; baar-baar badalne wala data sync job taaza rakhe.
Teeno ek hi thaali ke chatte-batte: DELETE = hum khud turant kaatte, TTL = kuch time baad apne aap kat-ti, sync job = parchi ko DB se mila ke theek karta.
**cache DELETE + sync job (DB = sach)**
Interview: "Update pe cache key delete, DB update. Beech ki purani parchi sync job DB se mila ke theek karta. Profile me thodi der purana chalega, paise jaisi kadi consistency nahi chahiye."

**6. Priya ki parchi cache me nahi, viral, ek second me 10,000 log, sab DB pe**
Sabko DB pe mat jaane do. Ek chhotu bhejo: jaa, Priya ki parchi DB se le aa. Baaki sab ruko, parchi aate hi sabko yahin se. Chhotu gir gaya ya dheema pada to kuch second dekho, phir bheed me se ek ko bhejo: tu le aa, mujhe bhi de dena.
**single flight (lock) + lock ki umar**
Interview: "Miss pe sirf ek request DB jaati, baaki uska intezaar. Lock ki umar (timeout) hoti hai, wo gira to agla jaata, lock atka nahi rehta."

**7. Priya ki key sirf machine 4 pe, 1 crore log, machine 4 gir rahi, baaki 9 aaram se**
Bheed badhi to aur gaadi lagao. Machine 4 ki copy bana do, machine 8 tu bhi dekh, machine 9 tu bhi dekh. Ek key teen jagah, bheed bat gayi.
**hot key: key ki copies kai machines pe**
Interview: "Garam key ki kai copies alag machines pe, padhne wala koi bhi ek uthata. Ek machine pe saari bheed nahi."

&nbsp;

Poora design = [10_distributed_cache.md](10_distributed_cache.md)
