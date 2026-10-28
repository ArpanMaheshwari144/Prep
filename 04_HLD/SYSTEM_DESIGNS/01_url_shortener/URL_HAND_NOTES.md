# URL shortener: hand notes (mock se pehle ye padho)

### Shuru

Kaam do hi hain: lamba link do to 7 akshar ka chhota code milta hai, aur koi chhota link khole to use asli link pe bhej dete hain. System tez chalna chahiye, hamesha chalna chahiye, aur do logon ko kabhi ek hi code nahi milna chahiye. Har second 1,000 link bante hain aur 1 lakh khulte hain. Kholna banane se 100 guna zyada hai, isliye aage CACHE lagega. 5 saal me data 90 TB ho jaata hai, jo ek machine me nahi aata, isliye aage SHARD lagega.

&nbsp;

**1. SERVER + DB**

Sabse seedha design hai ek SERVER aur ek DB. Link aaya to code banao, dono ko DB me likho. Koi link khole to DB se lambe link dhoondho aur 302 se wahan bhej do. Memory me nahi rakhte, kyunki server restart hote hi saare link gayab ho jaate.

&nbsp;

**2. COUNTER + BASE62**

Ab sawaal ye hai ki code bane kaise aur do logon ko same na mile. Iske liye COUNTER + BASE62 lagaya: har link ko ek naya number milta hai, aur us number ko 62 akshar me likhte hain. Random ya hash me do logon ko same code mil sakta hai, aur har baar DB me jaanchna padta. Counter me number kabhi dohraata hi nahi.

&nbsp;

**3. REDIS**

Har second 1 lakh log link khol rahe hain, aur har baar DB me dhoondhna dheema hai. Isliye beech me REDIS lagaya, jo memory me rakhta hai. Redis bhar jaaye to jo link sabse der se nahi khula, use hata dete hain (LRU). Dhyaan: LRU ka matlab sabse DER se use nahi hua; "sabse kam BAAR use hua" LFU hota hai.

**Maan lo Redis ki entry expire hui aur sab ek saath DB pe toot pade:** LOCK (single flight). Sirf pehli request lock leti hai (lock ki umar 2 second) aur DB jaati hai, baaki 50 ms ruk ke Redis dekhte hain. DB pe 1,000 ki jagah 1 query.

**Maan lo lock wali pehli request beech me MAR gayi:** baaki 999 Redis dekhte rehte hain, khaali milta hai, ruke rehte hain. 2 second baad lock ki umar khatam, lock apne aap khul jaata hai. Baaki me se jo pehle pahuncha wo lock leta hai, DB jaata hai, Redis bharta hai, aur baaki sabko mil jaata hai. User ko 2 second der hui, par DB pe bheed phir bhi nahi aayi.

**Maan lo lock wali request mari nahi, par DHEEMI padi (DB 5 second le raha):** baaki 999 poore 5 second ruke rehte hain. Lock ne DB bachaya, par users ruk gaye. Ilaaj SOFT / HARD umar: key ko kabhi poora khaali hone hi mat do. SOFT 1 ghanta, HARD 1 din. 1 ghante baad key "thodi purani" maani jaati hai, par Redis me padi rehti hai. Request aayi to purana link turant de do, aur saath me ek request (lock ke saath) peeche se DB se naya laake Redis me rakh deti hai. Wo dheemi ho ya mar jaaye, kisi user ko farak nahi, kyunki link badalta nahi to purana bhi sahi hai. HARD (1 din) bas isliye ki bahut purana maal hamesha na pada rahe. Lock DB ko bachata hai, soft / hard users ko. DB call pe 500 ms ka timeout.

&nbsp;

**4. LB + COUNTER RANGE**

Ek app server itna bojh nahi jhelta, aur wo gira to poori site band. Isliye kai server lagaye aur aage LB (load balancer). Par ab har server ka apna counter ho to do server same number de denge. Isliye COUNTER RANGE: beech ka counter har server ko 1,000 number ka alag dabba deta hai.

**Maan lo duniya bhar se log aaye:** Route 53 har user ko uske paas wale region pe bhejta hai.

**Maan lo traffic 10 guna ho gaya:** app server badhao, garam link Redis me rakho, Cassandra ki machine badhao.

&nbsp;

**5. SHARD**

90 TB ek DB machine me nahi aata. Isliye data kai machine me baanta (SHARD), aur link kis machine pe jaayega ye code se tay hota hai, kyunki link kholte waqt hamare paas sirf code hota hai.

&nbsp;

**6. RING (consistent hashing)**

Machine 10 se 11 ki to % 11 se lagbhag har link ki jagah badal gayi, aur link 404 dene lage. Isliye RING (consistent hashing): nayi machine aaye to sirf ek chhota hissa hilta hai, baaki apni jagah.

&nbsp;

DB kaunsa lein? Data bas code se link hai. Join ya transaction nahi chahiye, par 90 TB baantna hai. SQL me ye sab haath se karna padta, isliye NoSQL liya: CASSANDRA (AWS pe ho to DynamoDB).

&nbsp;

**7. 3 COPY**

Ek DB machine giri to us pe pade link 404 dene lage. Isliye har link ki 3 COPY teen alag machine pe rakhte hain. Ek gire tab bhi do bachi rehti hain.

**Maan lo link bana aur dost ko turant 404 mila:** QUORUM. Har link teeno copy pe likha jaata hai, par 3 me se 2 copy se "haan" aate hi aage badh jaate hain. Padhte waqt bhi 2 copy se padhte hain. Isliye likhne wali 2 aur padhne wali 2 me kam se kam ek copy common hoti hai, aur naya link mil jaata hai.

**AWS pe:** in dabbon ko hum khud nahi sambhalte, AWS ki service lete hain: Cassandra = **Keyspaces** (ya EC2 pe khud chalao), Redis = **ElastiCache**, LB = **ALB**. Machine, copy, gira to badalna, ye AWS ka kaam hai.

&nbsp;

**8. LOCAL CACHE**

Ek link viral hua, 1 crore log ek saath khol rahe hain, aur sab Redis ki ek hi machine pe ja rahe hain. Isliye har app server apni memory me us garam link ko 5-10 second rakh leta hai (LOCAL CACHE). Yahan Redis giri nahi, sirf bheed se dheemi hai, isliye replica ko main banane se kaam nahi chalta: bojh nayi main pe chala jaata. Request Redis tak pahunchne se pehle hi rokni hai. 302 hi rakhte hain, 301 nahi, taaki har click hamare paas aaye aur bura link band kar sakein.

**302 kyun, 301 kyun nahi:** 301 = "hamesha ke liye yahin", browser link yaad kar leta aur dobara hamare paas aata hi nahi. Bojh kam, par control gaya: link spam nikla to bhi browser purane pe bhejta rahega. 302 = "abhi ke liye", har click hamare paas aata hai, band link pe chetavani page dikha sakte hain. Isliye 302, bojh cache se sambhala.

**Maan lo click ki ginti chahiye:** redirect turant karo, click ki khabar Kafka me daalo, Analytics peeche se ginta hai.

&nbsp;

**9. RATE LIMIT**

Ek bot har second hazaron link bana raha hai. Isliye link banane pe RATE LIMIT lagaya, ek user ek minute me 10 link, usse zyada pe 429. Limit user pe lagate hain, IP pe nahi, kyunki ek office ke saare log ek hi IP se aate hain. Jo bot baar-baar aaye, use WAF me block kar do. Saath me bure link ko kaali list (jaise Google Safe Browsing) se jaanch ke banne hi nahi dete.

&nbsp;

### Wrap

tez = Redis + local cache · hamesha chale = LB + 3 copy + quorum · ek code ek link = counter range


Poora design (har kadam ka diagram aur "kyun") = [01_url_shortener.md](01_url_shortener.md)
