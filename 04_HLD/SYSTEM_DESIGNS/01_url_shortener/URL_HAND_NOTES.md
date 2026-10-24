# URL shortener: hand notes (mock se pehle ye padho)

### Shuru

Kaam do hi hain: lamba link do to 7 akshar ka chhota code milta hai, aur koi chhota link khole to use asli link pe bhej dete hain. System tez chalna chahiye, hamesha chalna chahiye, aur do logon ko kabhi ek hi code nahi milna chahiye. Har second 1,000 link bante hain aur 1 lakh khulte hain. Kholna banane se 100 guna zyada hai, isliye aage CACHE lagega. 5 saal me data 90 TB ho jaata hai, jo ek machine me nahi aata, isliye aage SHARD lagega.

1. Sabse seedha design hai ek SERVER aur ek DB. Link aaya to code banao, dono ko DB me likho. Koi link khole to DB se lambe link dhoondho aur 302 se wahan bhej do. Memory me nahi rakhte, kyunki server restart hote hi saare link gayab ho jaate.

2. Ab sawaal ye hai ki code bane kaise aur do logon ko same na mile. Iske liye COUNTER + BASE62 lagaya: har link ko ek naya number milta hai, aur us number ko 62 akshar me likhte hain. Random ya hash me do logon ko same code mil sakta hai, aur har baar DB me jaanchna padta. Counter me number kabhi dohraata hi nahi.

3. Har second 1 lakh log link khol rahe hain, aur har baar DB me dhoondhna dheema hai. Isliye beech me REDIS lagaya, jo memory me rakhta hai. Redis bhar jaaye to jo link sabse der se nahi khula, use hata dete hain (LRU).

4. Ek app server itna bojh nahi jhelta, aur wo gira to poori site band. Isliye kai server lagaye aur aage LB (load balancer). Par ab har server ka apna counter ho to do server same number de denge. Isliye COUNTER RANGE: beech ka counter har server ko 1,000 number ka alag dabba deta hai.

5. 90 TB ek DB machine me nahi aata. Isliye data kai machine me baanta (SHARD), aur link kis machine pe jaayega ye code se tay hota hai, kyunki link kholte waqt hamare paas sirf code hota hai.

6. Machine 10 se 11 ki to % 11 se lagbhag har link ki jagah badal gayi, aur link 404 dene lage. Isliye RING (consistent hashing): nayi machine aaye to sirf ek chhota hissa hilta hai, baaki apni jagah.

DB kaunsa lein? Data bas code se link hai. Join ya transaction nahi chahiye, par 90 TB baantna hai. SQL me ye sab haath se karna padta, isliye NoSQL liya: CASSANDRA (AWS pe ho to DynamoDB).

7. Ek DB machine giri to us pe pade link 404 dene lage. Isliye har link ki 3 COPY teen alag machine pe rakhte hain. Ek gire tab bhi do bachi rehti hain.

8. Ek link viral hua, 1 crore log ek saath khol rahe hain, aur sab Redis ki ek hi machine pe ja rahe hain. Isliye har app server apni memory me us garam link ko 5-10 second rakh leta hai (LOCAL CACHE). 302 hi rakhte hain, 301 nahi, taaki har click hamare paas aaye aur bura link band kar sakein.

9. Ek bot har second hazaron link bana raha hai. Isliye link banane pe RATE LIMIT lagaya, ek user ek minute me 10 link, usse zyada pe 429. Saath me bure link ko kaali list se jaanch ke banne hi nahi dete.

### Agar Pooche

- Link bana aur dost ko turant 404 mila: QUORUM. 3 me se 2 copy pe likho aur 2 se padho, to naya link kam se kam ek copy pe zaroor milega.

- Redis ki entry expire hui aur sab ek saath DB pe toot pade: LOCK (single flight). Sirf pehli request DB jaati hai, baaki thoda ruk ke Redis se lete hain.

### Wrap

tez = Redis + local cache · hamesha chale = LB + 3 copy + quorum · ek code ek link = counter range


Poora design (har kadam ka diagram aur "kyun") = [01_url_shortener.md](01_url_shortener.md)
