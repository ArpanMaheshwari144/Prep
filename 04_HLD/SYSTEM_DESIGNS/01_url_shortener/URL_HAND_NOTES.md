# URL shortener: hand notes (token counter)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Shabd (poore notes me yahi):** lamba pata = lamba link · token = chhota code · mota register = DB · chhota register = Redis · counter wala = app server · almari = DB machine · manager = beech ka counter

&nbsp;

**1. Counter wale ne do logon ko same token de diya**
Token ek line me banao: agla number, phir agla. Number kabhi dohraata nahi.
**counter**
Interview: "Har link ko naya number, isliye takrav nahi. Random ya hash me takrav aur har baar DB jaanch."

**2. 10 arab pe token 11 akshar ka ho gaya**
Number plate jaisa: har khaane me sirf 0-9 nahi, akshar bhi. Kam khaane, bahut token.
**base62**
Interview: "Number ko base62 me likhta hoon (0-9, a-z, A-Z). 7 akshar = ~3.5 lakh crore code."

**3. Har second 1 lakh log, mota register palatna dheema**
Saamne chhota register, baar-baar poochhe jaane wale token wahan. Nahi mila to mote se laao, chhote me likho.
**cache (Redis)**
Interview: "Pehle Redis, nahi mila to DB se laake Redis me. Padhai likhai se 100 guna zyada hai."

**4. Chhota register bhar gaya**
Waqt ki had laga do, aur jo token poochha hi nahi ja raha wo hatao.
**TTL, LRU**
Interview: "Har key pe TTL, bhara to LRU: jo sabse der se nahi padha gaya wo hatao."

**5. Token ka waqt khatam, 1,000 log ek saath mote register pe**
Ek ladka mote register tak jaaye, baaki 999 yahin ruken. Ladka laaye to sabko mil jaaye.
**lock (single flight)**
Interview: "Stampede pe lock: ek request DB jaati, baaki thoda ruk ke Redis dekhti hain. DB pe 1 query."

**6. Ladka raste me gir gaya**
Ladke ki timeline rakho, itni der me nahi aaya to 999 me se agla jaaye. Aur tab tak purana pata de do, baad me taaza kar denge.
**lock TTL, soft / hard TTL**
Interview: "Lock pe umar, mara to khud khulta. Soft TTL pe purana do aur peeche se taaza laao, link badalta nahi."

**7. 5 counter, dono ne token "10" de diya**
Har counter ko apna dabba: 1-100, 101-200, aur aage.
**counter range**
Interview: "Kai app server, aage LB. Beech ka counter har server ko 1,000 number ki range deta hai."

**8. Dabba dene wala manager chhutti pe**
Uska duplicate rakho, jaise company me doosra manager sambhaal leta hai.
**replica**
Interview: "Counter ki replica aage aati hai. Tab tak har server ke paas apni range ke bache number hain."

**9. Register ek almari me nahi aata (90 TB)**
Register ke tukde, alag almari. Hash se token ki almari tay, wahi token hamesha wahi almari.
**shard**
Interview: "Data code ke hash se 10 machine me. Kholte waqt sirf code hota hai, isliye code se baanta."

**10. 10vi almari aayi, sab token galat almari me dhoondhe gaye**
Almaariyan gol chakkar pe doori par. Token jahan gire, paas wali almari me, jaise toofan me paas wala shelter. Nayi almari se sirf aas-paas hilte hain.
**ring (consistent hashing)**
Interview: "% N badla to sab hilte. Ring pe nayi machine aaye to sirf ek tukda hilta hai."

**11. Almari jal gayi**
Har almari ki copy alag jagah. "Likh liya" tabhi jab copies bhi haan bolein.
**3 copy + quorum (W=2)**
Interview: "Har link ki 3 copy. Teeno pe likho, 2 ki haan pe aage."

**12. Dost ne peeche wali copy se padha, "nahi mila"**
Ek se mat padho, 2 se padho. 2 pe likha, 2 se padha, to ek copy common.
**R=2**
Interview: "W=2, R=2, W + R > 3, isliye naya link hamesha mil jaata."

**13. Viral token, sab ek chhote register pe**
Chhote register ki copies bana do aur bheed baant do. Aur counter wala garam token ki parchi apne paas rakhe, baar-baar uth ke kyun jaaye.
**Redis replica, local cache**
Interview: "Garam key har app server ki memory me 5-10 second. Redis tak jaata hi nahi."

**14. Log pata phone me likh ke aana band, dhokhe wala pata band nahi ho raha**
Phone me likhne hi mat do. Har baar counter pe aao, to band token pe chetavani de sakte hain.
**302, 301 nahi**
Interview: "301 pe browser cache kar leta, control gaya. 302 se har click hamare paas, bojh local cache sambhale."

**15. Machine se har second hazaar nakli chit**
Bura pata pehle hi jaancho. Baar-baar wala gate pe roko. Aur har aadmi pe had, group pe nahi, kyunki office ka WiFi ek hai.
**kaali list, WAF, rate limit (user pe, 429)**
Interview: "Link banane pe rate limit user pe, IP pe nahi. Safe Browsing se bura link banne hi nahi. Repeat bot WAF me."

&nbsp;

Poora design = [01_url_shortener.md](01_url_shortener.md)
