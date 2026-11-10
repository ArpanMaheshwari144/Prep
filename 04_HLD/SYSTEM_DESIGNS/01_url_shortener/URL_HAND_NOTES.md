# URL shortener: hand notes (token counter)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Token counter. Log aate hain aur bahut lamba pata likhi chit dete hain ("amazon.in/products/xyz/..."). Counter wala unhe chhota token deta hai, jaise aB3x9. Jo bhi token dikhaye, use wo lamba pata mil jaata hai.

&nbsp;

**1. Counter wala token apne man se bana raha, kuch bhi chhota likh deta. Ek din do alag logon ko same token de diya. Chhota bhi rahe aur do logon ko kabhi same na mile, kaise?**
Arpan: Uska token ek line wala kar do. Jaise abhi number hai 9, to agla 10. Matlab ek line me wo token banayega. Counter kar do, counter: ye ban gaya to aage badhao, phir ye ban gaya to aage badhao. Aise wo number kabhi repeat nahi hoga. Counter service.
Technical me: "ek line me, aage badhao" = counter (har link ko agla number), "kabhi repeat nahi" = takrav nahi.

**2. Saal bhar me counter 10 arab pe. Token "10000000000", 11 akshar, aur lamba hota jaayega. Number wahi rakhte hue token chhota kaise? (Sawaal: gaadi ki number plate "DL 3C AB 1234" me sirf number nahi, akshar bhi.)**
Arpan: Uske peeche kuch laga do, kuch aisi algorithm set kar do ki wo number apna bhi chhota kar de. Technical terms me kuch hota hoga. Jaise DL 3C, phir agla aa jaayega 4C, 5C, phir 5D, phir A, AB ke baad CB, ye aata rahega. 1 2 3 4 ki jagah aisa kuch, hum combination bana sakte hain. 7 character hain to kaafi combination ho sakte hain.
Technical me: "number plate jaise akshar bhi, combination" = base62 (0-9, a-z, A-Z), "7 character, kaafi combination" = 62^7, ~3.5 lakh crore code.

**3. Token lene kam aate, token dikha ke pata lene wale bahut: har second 1 lakh. Counter wala har baar mota register palat ke dhoondhta, line ruk jaati. Kuch token din me hazaar baar aate.**
Arpan: Unhe chhote register me rakh lo. Jo bhi log dekhne aa rahe, bas usi register se wapas kar do. Mota register to rakh lo, koi dikkat nahi, ek chhota pehla register rakh lo, usme wahi wale token rakh lo. Jo log bahut baar poochhne aa rahe, wo wahin se chale jaayenge.
Technical me: "chhota pehla register" = cache (Redis), "mota register" = DB, "bahut baar poochhe jaane wale" = garam / baar-baar padhe link.

**4. Chhota register bhar gaya. Naya token likhna hai, kisi purane ko hatana padega. Kise?**
Arpan: Purana hata denge, hamare paas kuch tareeke hain. Time laga lo na, ki ye token itni der tak rahega register me, uske baad hata do. Ya jo sabse kam use hua, jo ab poochha hi nahi ja raha, wo token hata do. Wo likha hi kyun ja raha hai, hatao use.
Technical me: "itni der tak rahega, phir hata do" = TTL, "jo poochha hi nahi ja raha, hatao" = LRU.

**5. Ek mashhoor token ka waqt theek 7 baje khatam, chhote register se hat gaya. Usi pal 1,000 log wahi token le ke aaye, saare ek saath mote register pe toot pade.**
Arpan: Mere paas ek ladka hai, chhota ladka. Main kahunga: bhai chupchap bhaag ke ja, aur us mote register se wo information nikaal ke la, aur isme la kar rakh de, to sab yahin ruk jaayenge. Tu ja, patapat, jaldi se jaldi nikaal ke la aur turant chhote register me rakh de. Khatam kahani, ho gaya kaam.
Technical me: "ek ladka jaaye, sab yahin ruken" = lock (single flight), "la ke chhote register me rakh de" = DB se laake Redis bharna.

**6. Ladka bhaaga aur raste me gir gaya, kabhi wapas nahi aayega. Baaki 999 "ladka laayega" soch ke ruke khade.**
Arpan: Yahan thoda decision lena padega. Yahan hum purani information de sakte hain, unko baad me update kar denge. Main intezaar kab tak karunga? 999 me se ek aadmi ko bolunga: bhai le idhar aa, tu ek kaam kar, mote register me se information la ke mujhe de de, mera wo ladka pata nahi kahan chala gaya, uska koi naamo-nishaan nahi. Matlab main us ladke ki timeline rakhunga: agar itni der tak nahi aaya, to 999 me se jo bhi hai use bulaunga.
Technical me: "ladke ki timeline, nahi aaya to agla" = lock TTL (umar khatam to lock khulta), "purani information do, baad me update" = soft / hard TTL.

**7. Ek counter se bheed nahi sambhli, 5 counter khol diye. Har counter apni ginti 1, 2, 3 chala raha. Counter 1 aur 2 dono ne apne 10ve aadmi ko token "10" de diya.**
Arpan: Counter ko kahunga ki tum apni-apni range le lo. Un paanch counter ko tod dunga: tu 1 se 100 karega, tu 101 se 200, tu 201 se 300, tu 301 se 400. Aise sabko range me baant dunga.
Technical me: "apni-apni range" = counter range (beech ka counter har app server ko number ka dabba deta).

**8. 5 saal me mote register me itne token (90 TB) ki wo ek almari me aata hi nahi.**
Arpan: Shard kar do, chhote-chhote tukde bana do. 90 TB kab tak sambhaal loge, wo bharta jaayega. 10 TB ke 9 tukde kar do, aur sab tukde alag-alag rakh do: ye yahan, ye yahan. Aur use hash se baant do, ki ab kabhi aayega to wo is tukde pe chala jaayega, phir same aayega to phir usi tukde pe.
Technical me: "chhote tukde, alag almari" = shard, "hash se baant do, same aaye to usi tukde pe" = code ke hash se machine tay.

**Maan lo almari phir kam padi, 10vi lagani hai:** Cassandra khud sambhalta: token ke hash se tukda wahi tay karta, nayi machine lagao to apne aap thoda data baant deta. Uska andar ka kaam, hamara faisla nahi.
Technical me: Cassandra andar token ring (consistent hashing) se baantta, official doc: https://cassandra.apache.org/doc/latest/cassandra/architecture/dynamo.html

**9. Almari number 3 me aag lagi, uske saare token jal gaye.**
Arpan: Ek almari mat rakho, unhi ki copy bana do. 3 number ki almari gir gayi to uski ek aur copy hai. Ab isme dikkat ye aa sakti hai ki copy thodi si peeche ho sakti hai, to wahi acknowledge wala kar lo: jab tak acknowledge nahi hota wo wahin rahega, "haan bhai maine likh liya, tu acknowledge bhej". Same dikkat ka same solution ho gaya.
Technical me: "almari ki copy" = 3 copy (replica), "acknowledge aaye tab pakka" = quorum write (W=2).

**10. Teen copy A, B, C. Arpan ne naya token banaya, A aur B ne haan bola, C peeche. Turant dost ne token dikhaya, counter wale ne C se padha, "nahi mila".**
Arpan: Humne A, B pe likha na, to token ko do se hi padhwao. Agar do likh rahe hain to do hi padhenge, ek nahi padhega. Mere dost ne token maanga, to use B aur C se padhwao, ya A aur C se, single-single se nahi. Maan lo ek copy peeche bhi reh gayi, to atleast ek ke paas to hai na. C nahi hai to B ke paas to hai.
Technical me: "do se hi padhwao" = R=2, "ek ke paas to hai" = W + R > 3, ek copy common.

**11. Cricket match ka token viral, 1 crore log ek saath. Sab counter wale us token ke liye ek hi chhote register ki taraf bhaag rahe, wo itna dheema ki uske baaki token bhi atke.**
Arpan: Sabko wahi token chahiye. To us chhote register ko bahut saaron me baant do. Ek hi kyun hai wo register, 10 register bana de, uski photocopy bana de aur divide kar de. Ek hi chhote register pe ek saath itne log nahi aa rahe, wo bat gaya. 10 kar do, 100 kar do.
Technical me: "chhote register ki photocopy, divide" = Redis replica (padhai copies me bantna).

**Maan lo copy 10 ho ya 100, har baar counter wale ko kursi se uth ke register tak jaana padta. 1 crore log, 1 crore chakkar.**
Arpan: Us counter wale ko bol: ye information tu rakh le, baar-baar uth ke kyun ja raha hai. Ek chhoti parchi le, uska token aur information apne paas rakh. Maan le roz 100 log aaye, ek hi information maang rahe, aur wo mere se pachaas kadam door hai, mujhe har baar wahin jaana padta. Main wo information utha ke apne paas ek page pe likh lunga, aur apni kursi pe baith gaya. Ab koi poochhe to "le bhai, teri information ye rahi". Mujhe dobara kahin jaana nahi padta.
Technical me: "counter wala apne paas parchi rakhe" = local cache (app server ki memory me garam link kuch second).

**12. Kisi ka idea: har aadmi pata apne phone me hi likh le, counter pe aaye hi nahi. Hafte baad pata chala ek token ka pata dhokhe wali site ka hai, use band karna hai, par log seedha phone wale pate pe ja rahe.**
Arpan: Token radd kar do, ki ab wo galat site pe nahi jaayega. Doosri cheez, aisi kisi cheez ko hum pehle se hi dekh lenge ki wo theek bhi hai: user ko dene se pehle khud check kar lo ki jo wo bana raha hai wo theek bana raha hai ya nahi. Galat bana raha hai to mana kar do banane se, aur us token ko radd kar do.
Technical me: "token radd" = code band, "dene se pehle khud check" = kaali list (Google Safe Browsing).

**Maan lo token radd kiya, par log counter pe aa hi nahi rahe. Unhe kaise pata chale? Shuru me phone me likhne dena tha ya nahi?**
Arpan: Unke phone me wo cheez update kar do na. Wo cookies aur cache se aa rahe honge, to unka cache radd kar do, hata do, to agli request seedha yahan aayegi. Phir (jab poocha ki phone uske haath me hai, tu kaise mitayega): To phir phone pe likho mat na bhai. Phone pe likhega nahi to wo hamare counter pe dobara aayega, dobara hum use theek token de denge. Agar aisi phone wali dikkat hai to uske phone me likhna hi mat do, saaf baat hai.
Baad me Arpan: aam taur pe phone ke cache ko TTL laga ke invalidate kar sakte hain, lekin hum technical nahi, aam dikkat ka solution bol rahe the, isliye thodi der me samjha.
Technical me: "phone me likhna hi mat do, har baar counter pe aaye" = 302 + `Cache-Control: no-store` (save hi mat karna; 301 nahi), "phone me likh le" = 301 (browser cache), "phone cache pe TTL" = browser cache ki umar.

**13. Ek aadmi ne machine laga di, har second hazaar chit, sab nakli ya dhokhe wale pate. Counter ka number faltu khatam, register bhar raha, asli log atke.**
Arpan: Jaanch padtaal karenge ki script laga di. Ek-do baar to aisa hoga hi, teen baar, chaar baar, phir pattern pata lag jaayega ki ye banda to har baar aa raha hai, har baar ye drama karke chala jaata hai. To us bande ko rok do, andar ghusne hi mat do, wo kamre me hi nahi aayega, use bahar gate pe hi bhaga do.
Technical me: "pattern pata lag jaayega, gate pe bhaga do" = WAF block.

**Maan lo pakadne me 3-4 chakkar lagte, tab tak wo laakhon chit de chuka. Shuru se har aadmi ke liye kya niyam?**
Arpan: Yahan thoda pench hai. Had ek user, ek aadmi pe rakhenge. Kyunki maan lo ek office hai, wahan wo log unke WiFi se aa rahe hain, to dikkat ho jaayegi. Ek group ek saath aa gaya, lekin wo sab legit users hain, unko to token dena padega. To per user pe lagao, group pe nahi: ye ek banda hai, ek baar me 5 hi lagayega, usse zyada nahi.
Technical me: "per user had, group pe nahi" = rate limit user pe (IP pe nahi), "office ka WiFi" = ek IP ke peeche kai asli user, "5 se zyada nahi" = had paar pe 429.

**Maan lo bina login ke bhi link banta, script har second hazaar link bana rahi, userId hai hi nahi.**
Arpan: Sabse pehle login laga do: bina login link nahi. Isse dashboard / tracking bhi chalega. Login nahi rakhna to tracking bhool ja, aur IP pe had: IP ki had thodi zyada, userId ki thodi kam. Script hazaar link nahi bana paayegi. Baar-baar aaye to IP ban. Naye-naye IP se phir bhi aaye to peeche investigate karo, aur ek hi desh se aa raha to us desh ko thodi der WAF ki block list me daal do.
Technical me: "login laga do" = auth zaroori (tab userId pe had + tracking), "bina login IP pe dheeli had" = anonymous rate limit per IP, "IP ban, desh block" = WAF block list / geo-block.

&nbsp;

Poora design = [01_url_shortener.md](01_url_shortener.md)
