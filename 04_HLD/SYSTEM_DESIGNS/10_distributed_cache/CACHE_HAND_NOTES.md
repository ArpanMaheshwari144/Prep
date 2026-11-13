# Cache: hand notes (Arpan aur Ravi, profile page)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ki app me har bande ka profile page hai (naam, photo, bio). Ravi mashhoor hai, uska profile din me lakhon log kholte hain. Data DB me hai, aur har baar server DB se padh ke laata hai.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      get(key), put(key, value), delete(key) · asli sach DB me, cache sirf tez copy
NFR     tez (~1 ms, DB se 50 ms) · DB pe bojh kam · thodi der purana chalega (profile hai, paisa nahi)
        · jagah bhare to purana nikle · ek hi key pe bheed ho to bhi na gire
Numbers DB ek padhai ~50 ms, kuch hazaar query / sec jhelta · Ravi ka profile hazaaron / sec, badalta din me ek baar
        10 crore user, roz khulne wale profile ~1 TB · ek machine ki RAM 64 GB -> baantna padega
```

&nbsp;

**1. DB se ek baar padhne me ~50 ms, aur DB ek second me kuch hazaar query hi jhelta. Ravi ka profile har second hazaaron log khol rahe, jabki uska data din bhar me shayad ek baar badalta. Page dheema, DB ki saans phooli.**
Arpan: Information thodi store kar lete hain. DB se pehle ek dabba rakh le jisme kuch information store kar le: kuch bhi famous, ya aisi information jo padhi bahut ja rahi ho aur update bahut kam ho rahi ho. Ab koi bhi information maange to us dabbe me se de de, baar-baar DB me jaane ki zaroorat na pade. Dikkat hi khatam.
Technical me: "DB se pehle ek dabba" = cache (Redis), "padhi bahut, update kam" = cache ke laayak data, "dabbe me se de de, baar-baar DB nahi" = cache hit (cache-aside).

**2. Dabba RAM ka hai, 64 GB. Log profile kholte gaye, sab jama hote gaye, dabba bhar gaya. Aaj Priya ka video viral, lakhon log uska profile khol rahe, par uske liye jagah nahi. Dabbe me 3 saal purane profile pade hain jinhe mahino se kisi ne nahi khola.**
Arpan: RAM ko thoda khaali kar do, usme se purana data nikaal do. Jo purana hai, jo kam use ho raha hai, wahan se nikaal do, use lena-dena hi nahi. Jo ab use nahi ho raha aur RAM me pada hai, wo befaltu ki jagah ghere hai, use hata do.
Technical me: "RAM khaali karo" = eviction, "jo purana, kam use ho raha, nikaal do" = LRU (Least Recently Used).

**3. Ab 10 crore users, roz khulne wale profile hi 1 TB. Ek machine me 64 GB, LRU kitna bhi chalao, 1 TB ek dabbe me nahi aata. Upar se ek machine ki request ki had, bheed usse kai guna.**
Arpan: Tod do. 10 crore ko tod do, 2-2 crore ya 3-3 crore ek dabbe me rakh do. Wo bhi kam pade to aur tod do, un 2 crore ko bhi tod do, todte raho. Aur jo dikhta hai wo hash hai, same pe chala jaayega. Hash hum kai baar padh chuke hain.
Technical me: "tod do, alag dabbe me" = sharding, "hash, same pe chala jaayega" = key ka hash batata machine, same key hamesha same machine.

**4. 10 machine, formula `hash(key) % 10`, Ravi machine 3 pe. 11vi machine judi, formula `% 11`, Ravi ke liye machine 7, jahan data hi nahi. Lagbhag har key ki jagah badli, saari request miss, sab DB pe, DB baith gaya.**
Arpan: Consistent hashing kar do, jo ring me discuss ki thi. Ek ring me machines paas-paas hain. Key ka hash do khaanon ke beech gira to sabse pehle khaane pe chala jaayega. Kho-kho dekha hai? Jab bada bhaagta hai to main sabse paas wale bande ko dhakka deta hoon: chal tu pakad jaake. Ya do padaav hain, ek 50 km pe, agla 100 km pe. Gaadi ki headlight kharab hui to main sabse paas wale padaav pe chupchaap ruk jaunga, 100 km pe nahi jaunga. Ring me bahut saare server, beech me jo gira wo sabse paas wale pe jaake baith jaayega. Jaise highway pe petrol 50 km ka bacha to paas ka pump dhoondhta hoon, door wala nahi (yahi soch leaderboard 6 me: score ke aas-paas).
Technical me: "ring me machines paas-paas" = consistent hashing (ring), "sabse pehle khaane pe" = key ghadi ki disha me agli machine pe (hamesha aage wali, peeche wali nahi, chahe peeche wali zyada paas ho; ek hi disha tay hai, isliye dhoondhne pe har baar wahi machine milti). "kho-kho, headlight, paas ka petrol pump" = machine giri to uski keys aage wali agli machine sambhalti, door wali nahi.

**Maan lo key 47 pe giri. Peeche machine 45 pe (2 door), aage machine 52 pe (5 door). "Sabse paas" to 45 hua, key kahan jaayegi?**
Arpan: Ise highway maan le. Petrol pump peeche paas me hai, par highway pe U-turn thodi lunga, U-turn bahut kam hote hain, ek hi disha me badhna hai. Gaadi ghuma ke nahi jaunga. Peeche wala pump nikal gaya, chahe paas ho, main aage wale pump pe hi jaunga. Highway pe aam taur pe yahi hota hai. Kho-kho me bhi same: hum usi aadmi pe dhyaan dete jo us disha me baitha ho.
Technical me: "U-turn nahi, aage wala pump" = ring pe ek hi disha (clockwise), key hamesha aage wali machine (52) pe.

**Maan lo saare dabbe bhar gaye, to unhe ring se hata ke alag kamre me kyun nahi rakhte:**
Arpan: Maine socha bank locker jaisa: dabbe bhar gaye to unme ab kuch aana hi nahi, to unhe ek alag kamre me daal do, khaali dabbe doosre kamre me. Beech me ek aadmi khada kar do jo bataye "teri key is kamre me", aur nayi key khaali kamre me.
Par ring me aise nahi chalta. Ring me dabbe bank locker ki tarah ek-ek karke nahi bharte. Hash har key ko ring pe kahin bhi giraata hai, to saare dabbe ek saath, barabar bharte hain. Koi dabba "khatam" nahi hota, nayi key purane me bhi aati rehti hai, aur dhoondhne ke liye bhi wahi ring chahiye. Sab bharne lage to naya dabba lagao, wo paas wale ka kuch hissa le leta hai, dono me jagah. (Mera do kamre wala tareeka bhi kahin chalta hai, par beech wale aadmi ko crore key ka hisaab yaad rakhna padega, aur har request ek kadam extra.)
Technical me: "bank locker, ek-ek karke bharna + beech ka aadmi" = range partitioning + lookup / directory, "saare ek saath bharte" = hash se barabar baant.

**Maan lo ring ke saare dabbe phir bhar gaye:**
Arpan: Ring me aur dabbe jodte jao. Ring bada hona dikkat nahi, ring koi kamra nahi, niyam hai; dabbe 10 ho ya lakh, dhoondhna ek hi hisaab. Consistent hashing apna kaam khud karta, interview me naam kaafi.
Technical me: "dabbe jodte jao" = scale out (nayi node ring me), "ek hi hisaab" = ring pe lookup. Cassandra bhi andar yahi (official doc: https://cassandra.apache.org/doc/latest/cassandra/architecture/dynamo.html).

**5. Ravi ne apna naam badal diya. DB me naya naam likh gaya, par cache me abhi bhi purana "Ravi", sab cache se padh rahe. (Sawaal: parchi kaatne aur DB likhne ke beech koi request aayi aur purana naam parchi pe likh gaya, to wo theek kaun karega?)**
Arpan: Mere paas ek diary (DB) aur ek cache ki parchi. Ravi aaya: mera naam "Ravi Kumar" kar do. Sabse pehle apni parchi se "Ravi" kaat dunga, phir DB me "Ravi Kumar". Ab koi get karne aaya, parchi pe Ravi hai nahi, to DB se uthaya "Ravi Kumar" aur parchi pe likh diya. Kaam khatam.
Beech me parchi khaali thi aur request DB pe aa gayi, to ye ek job hai: ek banda hai jo har baar check kar raha, yahan Ravi, wahan Ravi Kumar, to parchi pe Ravi Kumar kar do, kyunki DB hamara source of truth hai. Ho sakta hai kuch second Ravi dikhe, par ye koi paisa thodi hai jo ekdum consistency chahiye. Chahiye hoti to lock laga lete.
TTL alag cheez hai. Sync job tab lagaunga jab cache baar-baar update hota ho (din me 100-200 baar). TTL tab jab data thodi der ke liye hi rakhna ho, jaise koi form bhar raha: chhota TTL laga do, form khaali chhod diya to apne aap gaayab. Har baar TTL se kaam nahi chala sakta, TTL kabhi-kabhi dhokha de deta hai.
Technical me: "parchi se kaat do" = cache DELETE, "DB source of truth" = DB = sach, "banda har baar check kare, farak mila to theek" = background sync (reconciliation) job, "kuch second Ravi dikhe, paisa nahi" = eventual consistency chalegi, "thodi der wala data, chhota TTL" = TTL. Sync job ke saath bhi ek lamba TTL rakho, taaki job chooke to purana data hamesha ke liye na atke (safety net).

**Maan lo poochhe: pehle DB likho phir parchi kaato, ulta kyun nahi?**
Wo bhi chalta hai, beech ka chhed thoda chhota hota hai, par chhed wahan bhi hai: ek dheemi read purana naam laake parchi pe likh sakti. Isliye peeche wala banda dono kram me chahiye; kaatna fail hua to dobara kaato. Bilkul pakka chahiye to wahi lock / version check.
Technical me: "pehle DB phir kaato" = write-then-invalidate, "kaatna fail to dobara" = invalidation retry, "pakka chahiye" = version check / lock.

**Maan lo cache me 1 TB, crore-on key. Sync job ko kaise pata kaunsi key check karni hai? Har key roz DB se milana sambhav nahi.**
Arpan: Wahi key dekho jo update hui, baaki se lena-dena nahi. Ek saath 1 TB update nahi hota. Sync job ko time de do: pichhle ek ghante me kaunsi row update hui, sirf unki parchi DB se milao. Phir agla ghanta. Dikkat lage to 2-3 ghante ya ek din ka kar do.
Technical me: "pichhle ghante me update hui" = DB me updated_at pe query (index), sirf un key ka cache milao / theek karo; "time badha do" = window chhoti-badi.

**6. Priya aam user thi, uski parchi cache me hai hi nahi. Raat 9 baje video viral, ek second me 10,000 log, sabko miss, sab ek saath DB pe, Priya ki ek hi query 10,000 baar.**
Arpan: 10,000 log ek saath DB jaayenge to DB maar dega, pehle unhe roko. Main chupchaap ek chhotu bhejunga: chhotu bhai, jaldi se DB me jaa, Priya ki parchi hot hai, le aa. Parchi pe likh lunga aur sabko yahin se bhaga dunga, mote register ki taraf koi nahi jaayega. Chhotu raaste me gir gaya ya dheema pad gaya, to kuch second dekh lo. Nahi aaya to unme se ek aadmi pakdunga: ae bhai tu idhar aa, mote register pe jaa, jo data tu maang raha wo utha la aur mujhe bhi de dena. Wo laya, cache me daal diya, sabko bhej diya. Ye mera backup plan.
Technical me: "10,000 ek saath DB pe" = cache stampede, "ek chhotu bhejo, baaki yahin se" = single flight (lock), "kuch second dekho, phir bheed me se ek" = lock ki umar (timeout).

**Maan lo interviewer poochhe, aur raaste?**
Arpan: ek ki jagah do chhotu bhejo, ek gira to doosra le aayega. Hot key ko zyada der tak rakho. Ya key ki 3 copy rakho aur sabka expire ka time alag, to sab ek saath kabhi expire nahi honge. "Ek sahi jawab nahi, company kya chahti hai us pe."
Technical me: do chhotu = redundant fetch, hot key lambi umar = longer TTL, alag-alag expire = TTL jitter (staggered expiry).

**Maan lo ek bot har second 10,000 aise user id maang raha jo hain hi nahi (50 crore, 70 crore, 99 crore...). Cache me kuch nahi, DB me bhi kuch nahi, cache me rakhne ko kuch nahi, to har baar seedha DB.**
Arpan: Main khud ko scene me rakh ke dekhta hoon. Mere paas 1 se 10 tak gintee hai. Ek ladka aaya: "11 hai?" Main andar jaake dekhta hoon, nahi hai. Phir aaya: "33 hai?" Phir andar gaya, nahi hai. 2-3 baar aisa hua, har baar andar jaana pada, main pareshaan ho gaya. Aise to wo number badhata jaayega aur main har baar andar bhaagta rahunga.

To main parchi pe likh lunga "mere paas 1 se 10 tak hai", isse bada koi maange to wahin mana: "11 nahi hai, nikal", andar jaane ki zaroorat hi nahi. Fayda: sabka naam yaad rakhne ki zaroorat nahi, bas ek range. DB SQL hai, id 1, 2, 3 kram se banti, to DB me 1 se 10 crore user hain to beech wale milenge hi. 50 crore maanga to parchi dekh ke wahin mana, DB tak gaya hi nahi.

Range ke andar wala, jaise 500, nahi mila to null de do; null ka kya karna wo uska engineer if-else lagaye. Aur koi 500 baar-baar maange (range ke andar, par hai nahi), to main parchi pe likh lunga "500 = null"; ab koi bhi 500 maange, DB jaaunga hi nahi.
Technical me: "nakli id, har baar andar (DB) bhaagna" = cache penetration. "parchi pe range, bahar wala wahin wapas" = request aate hi id range check (input validation), cache aur DB se pehle; galat id na cache tak pahunchti na DB tak. (Yahan parchi = range ki parchi, cache wali key nahi.) Range ke andar par nahi mila = null ko bhi cache me rakho (negative caching, thodi der ki umar), taaki baar-baar wahi nakli id DB tak na pahunche.

**Maan lo wo ek chhotu DB pe jaake atak gaya, DB zinda hai par bojh se bahut dheema, 2 minute ho gaye. Baaki 9,999 kab tak rukenge?**
Arpan: Yahan dikkat chhotu ki nahi, DB ki hai, to ilaaj bhi DB ka. 10-15 chhotu aur bhejne ka fayda nahi, wo bhi dheeme DB pe atkenge. Padhne wale zyada hain to DB ki 2-3 copy bana do: ek primary jo likhta hai, baaki padhne ke liye. Chhotu ne dekha ye copy dheemi hai, to use chhod ke doosri pe jaaye, wahan se jaldi jawab mila, cache me rakha, aur 9,999 sab free, cache se utha ke le gaye. Copy sync karna AWS (RDS) ka kaam hai, hamara nahi.
Technical me: "DB ki copy, padhne ke liye" = read replica, "dheemi copy chhod ke doosri pe" = timeout + retry on another replica, "sync AWS ka kaam" = managed replication (RDS).

**7. Priya ki parchi cache me aa gayi, par ek second me 1 crore log. Ring pe uski key sirf machine 4 pe. Baaki 9 aaram se, akeli machine 4 pe poori bheed, dheemi hote-hote girne lagi.**
Arpan: Baaki machine pe baant de. Machine 4 ka replica bana de, teen jagah ho gayi, to bat gaya. Ya ring me aur jagah bitha de: machine 8 tu bhi dekh le, machine 9 tu bhi dekh le. Jaise bheed zyada ho gayi to ek aur gaadi extra kar lo. Ek gaadi me sab nahi aa sakte, to ek-do chhoti gaadi kar lo.
Technical me: "machine 4 ka replica, 8 aur 9 tu bhi dekh" = hot key ki copies kai machines pe, "ek aur gaadi" = padhne ki bheed copies me bat-ti.

**Maan lo Priya ki key machine 4, 8, 9 teeno pe hai (copies), usne naam badla, aur sirf machine 4 ki parchi kati.**
Arpan: Refresh pe kabhi naya naam aayega, kabhi purana, kyunki 8 aur 9 pe abhi purani parchi padi hai. Kadam 5 wala niyam "pehle parchi kaato" sirf ek machine pe nahi, jitni copies hain sab pe lagao. Aur kadam 5 wala peeche ka banda (sync job) bhi sirf ek jagah nahi, saari copies jaanche.
Technical me: copies pe bhi delete = hot key ki har replica invalidate, sync job copies bhi = reconciliation sab replicas pe.


&nbsp;

Poora design = [10_distributed_cache.md](10_distributed_cache.md)
