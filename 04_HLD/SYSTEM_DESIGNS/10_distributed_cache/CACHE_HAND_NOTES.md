# Cache: hand notes (Arpan aur Ravi, profile page)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ki app me har bande ka profile page hai (naam, photo, bio). Ravi mashhoor hai, uska profile din me lakhon log kholte hain. Data DB me hai, aur har baar server DB se padh ke laata hai.

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
Arpan: Consistent hashing kar do, jo ring me discuss ki thi. Ek ring me machines paas-paas hain. Key ka hash do khaanon ke beech gira to sabse pehle khaane pe chala jaayega. Kho-kho dekha hai? Jab bada bhaagta hai to main sabse paas wale bande ko dhakka deta hoon: chal tu pakad jaake. Ya do padaav hain, ek 50 km pe, agla 100 km pe. Gaadi ki headlight kharab hui to main sabse paas wale padaav pe chupchaap ruk jaunga, 100 km pe nahi jaunga. Ring me bahut saare server, beech me jo gira wo sabse paas wale pe jaake baith jaayega.
Technical me: "ring me machines paas-paas" = consistent hashing (ring), "beech me gira to sabse paas wale pe" = key ghadi ki disha me agli machine pe, "kho-kho, headlight" = sabse paas wala hi sambhale.

**5. Ravi ne apna naam badal diya. DB me naya naam likh gaya, par cache me abhi bhi purana "Ravi", sab cache se padh rahe. (Sawaal: parchi kaatne aur DB likhne ke beech koi request aayi aur purana naam parchi pe likh gaya, to wo theek kaun karega?)**
Arpan: Mere paas ek diary (DB) aur ek cache ki parchi. Ravi aaya: mera naam "Ravi Kumar" kar do. Sabse pehle apni parchi se "Ravi" kaat dunga, phir DB me "Ravi Kumar". Ab koi get karne aaya, parchi pe Ravi hai nahi, to DB se uthaya "Ravi Kumar" aur parchi pe likh diya. Kaam khatam.
Beech me parchi khaali thi aur request DB pe aa gayi, to ye ek job hai: ek banda hai jo har baar check kar raha, yahan Ravi, wahan Ravi Kumar, to parchi pe Ravi Kumar kar do, kyunki DB hamara source of truth hai. Ho sakta hai kuch second Ravi dikhe, par ye koi paisa thodi hai jo ekdum consistency chahiye. Chahiye hoti to lock laga lete.
TTL alag cheez hai. Sync job tab lagaunga jab cache baar-baar update hota ho (din me 100-200 baar), jaise profile. TTL tab jab data thodi der ke liye hi rakhna ho, jaise koi form bhar raha: chhota TTL laga do, form khaali chhod diya to apne aap gaayab. Har baar TTL se kaam nahi chala sakta, TTL kabhi-kabhi dhokha de deta hai.
Technical me: "parchi se kaat do" = cache DELETE, "DB source of truth" = DB = sach, "banda har baar check kare, farak mila to theek" = background sync (reconciliation) job, "kuch second Ravi dikhe, paisa nahi" = eventual consistency chalegi, "thodi der wala data, chhota TTL" = TTL.

**6. Priya aam user thi, uski parchi cache me hai hi nahi. Raat 9 baje video viral, ek second me 10,000 log, sabko miss, sab ek saath DB pe, Priya ki ek hi query 10,000 baar.**
Arpan: 10,000 log ek saath DB jaayenge to DB maar dega, pehle unhe roko. Main chupchaap ek chhotu bhejunga: chhotu bhai, jaldi se DB me jaa, Priya ki parchi hot hai, le aa. Parchi pe likh lunga aur sabko yahin se bhaga dunga, mote register ki taraf koi nahi jaayega. Chhotu raaste me gir gaya ya dheema pad gaya, to kuch second dekh lo. Nahi aaya to unme se ek aadmi pakdunga: ae bhai tu idhar aa, mote register pe jaa, jo data tu maang raha wo utha la aur mujhe bhi de dena. Wo laya, cache me daal diya, sabko bhej diya. Ye mera backup plan.
Technical me: "10,000 ek saath DB pe" = cache stampede, "ek chhotu bhejo, baaki yahin se" = single flight (lock), "kuch second dekho, phir bheed me se ek" = lock ki umar (timeout).

**7. Priya ki parchi cache me aa gayi, par ek second me 1 crore log. Ring pe uski key sirf machine 4 pe. Baaki 9 aaram se, akeli machine 4 pe poori bheed, dheemi hote-hote girne lagi.**
Arpan: Baaki machine pe baant de. Machine 4 ka replica bana de, teen jagah ho gayi, to bat gaya. Ya ring me aur jagah bitha de: machine 8 tu bhi dekh le, machine 9 tu bhi dekh le. Jaise bheed zyada ho gayi to ek aur gaadi extra kar lo. Ek gaadi me sab nahi aa sakte, to ek-do chhoti gaadi kar lo.
Technical me: "machine 4 ka replica, 8 aur 9 tu bhi dekh" = hot key ki copies kai machines pe, "ek aur gaadi" = padhne ki bheed copies me bat-ti.

**Maan lo cache me 1 TB, crore-on key. Sync job ko kaise pata kaunsi key check karni hai? Har key roz DB se milana sambhav nahi.**
Arpan: Wahi key dekho jo update hui, baaki se lena-dena nahi. Ek saath 1 TB update nahi hota. Sync job ko time de do: pichhle ek ghante me kaunsi row update hui, sirf unki parchi DB se milao. Phir agla ghanta. Dikkat lage to 2-3 ghante ya ek din ka kar do.
Technical me: "pichhle ghante me update hui" = DB me updated_at pe query (index), sirf un key ka cache milao / theek karo; "time badha do" = window chhoti-badi.

&nbsp;

Poora design = [10_distributed_cache.md](10_distributed_cache.md)
