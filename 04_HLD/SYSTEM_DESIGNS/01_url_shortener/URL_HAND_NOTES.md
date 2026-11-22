# URL shortener: hand notes (token counter)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Token counter. Log aate hain aur bahut lamba pata likhi chit dete hain ("amazon.in/products/xyz/..."). Counter wala unhe chhota token deta hai, jaise aB3x9. Jo bhi token dikhaye, use wo lamba pata mil jaata hai.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
User  -->  LB  -->  kai counter (app server)
                        |
        naya token:     |   ginti ki line (counter, har counter ki apni range)
                        |   number -> chhota token (akshar + number, base62)
                        |   mota register me likho (Cassandra, tukdon me, 3 copy)
                        |
        token se pata:  |   pehle chhota register (Redis cache), na mile to mota register
                        v
                  lamba pata -> wahan bhej do (302)

 1  do logon ko same token              ->  ek line ka counter (ginti)
 2  10 arab pe token lamba              ->  number plate jaise akshar bhi (base62)
 3  har baar mota register palatna      ->  chhota register pehle (cache)
 4  chhota register bhar gaya           ->  jo kab se poochha nahi gaya use hatao (LRU)
 5  7 baje 1,000 log mote register pe   ->  ek chhota ladka jaaye, baaki ruke (single flight)
 6  ladka raste me gir gaya             ->  ladke ki timeline (lock ki umar)
 7  5 counter, dono ne "10" diya        ->  har counter ki apni range
 8  90 TB, ek almari me nahi            ->  tukde, hash se baanto (sharding)
 9  almari 3 me aag                     ->  copy + "haan" ka intezaar (replica, ack)
10  dost ne C se padha, "nahi mila"     ->  do me likho, do se padho (quorum)
11  cricket token viral                 ->  register ki photocopy + counter ki parchi (hot key copies, local cache)
12  dhokhe wala pata, log phone se      ->  token radd + phone me likhne mat do (302, 301 nahi)
13  machine se har second hazaar chit   ->  gate pe bhaga do + har aadmi pe had (rate limit)
```

**FR / NFR kahan se:** hawa se nahi aate. Upar ki dikkat chain dekh ke nikale; har FR / NFR line ke aage likha hai wo kis kadam ki dikkat se aayi.
Arpan: Hum hamesha URL ki baat kar rahe, to FR me wahi jaayega ki lamba URL le ke chhota banana. Jo dikkat design me dekh rahe, wahi FR / NFR banti: do logon ko same token na mile, lambe ka chhota hamesha theek ho, garam (hot) URL sambhale, sab mote register pe na jaaye, machine se hazaar chit na aaye. NFR me: consistency kaise, jaldi kaise, copies, local kaise rakhein. Dikkat se hi FR bante, unse hi NFR bhi; ye aaj pakka ho gaya. Numbers tab jab sach me zaroorat ho; 1 million ho ya 1 billion, shard waise bhi lagana padega.
Technical me: "do logon ko same token na mile" = uniqueness (counter / range), "lambe ka chhota hamesha theek" = consistency (quorum read / write), "garam URL sambhale" = hot key (cache, copies), "sab mote register pe na jaaye" = cache hit ratio, "machine se hazaar chit nahi" = rate limit, "jaldi" = low latency, "copies" = replication / availability.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      1  lamba URL do, chhota token lo                                  -> kadam 1, 2
        2  chhota token kholo to lambe pe bhejo                             -> kadam 3, 12
        3  dhokhe wala link pata chale to wo token band kar sako             -> kadam 12

NFR     1  kholna turant, har baar mote register pe na jaaye                -> kadam 3, 4
        2  garam (viral) token pe bhi na atke                               -> kadam 5, 6, 11
        3  lambe ka chhota hamesha sahi, abhi likha turant mile             -> kadam 9, 10
        4  jo save ho gaya wo na khoye (almari jale to bhi copy);
           save hi fail ho to user "dobara banao"                           -> kadam 9
        5  do logon ko kabhi same token na mile                             -> kadam 1, 7
        6  machine se hazaar chit na ban sake (har aadmi pe had)            -> kadam 13

Numbers sirf jahan faisla badle: saalon me itna data (~90 TB) ki ek almari me nahi aata -> tukde (kadam 8)
```

&nbsp;

**1. Counter wala token apne man se bana raha, kuch bhi chhota likh deta. Ek din do alag logon ko same token de diya. Chhota bhi rahe aur do logon ko kabhi same na mile, kaise?**
Arpan: Uska token ek line wala kar do. Jaise abhi number hai 9, to agla 10. Matlab ek line me wo token banayega. Counter kar do, counter: ye ban gaya to aage badhao, phir ye ban gaya to aage badhao. Aise wo number kabhi repeat nahi hoga. Counter service. Random ya hash me dono logon ko same code mil sakta hai aur har baar DB me jaanchna padta hai; counter me number kabhi same hoga nahi, to ye dikkat khatam.
Technical me: "ek line me, aage badhao" = counter (har link ko agla number), "kabhi repeat nahi" = takrav nahi, "random / hash me same code" = collision, har baar DB check; counter me check ki zaroorat nahi.

**Maan lo Arpan ne Delhi me YouTube ka ek gaana chhota kiya, Mumbai me kisi aur ne wahi link chhota kiya:** Arpan: URL shortener banane dega, uska link alag hoga. Duplicate link nahi banenge, par ek ke do link ho sakte hain, dikkat nahi, ye to design ka hissa hai; counter aage chalega hi chalega. (21-Nov Arpan ne bit.ly pe khud jaancha: ek hi YouTube link do baar chhota kiya, dono baar alag link mila, bit.ly/4AGUMOh aur bit.ly/4AKx7N1.)
Technical me: same lamba link = har baar naya code (lambe link pe "pehle se hai?" ki jaanch nahi), ek lambe ke kai chhote chalte hain.

**Maan lo do log bilkul ek hi pal me aayein:** Arpan: Ek aur pehra laga do: DB me code pe unique constraint, "insert if not exist". Ye DB me apne aap atomic kaam hai, do baar same code likha hi nahi jaayega.
Technical me: "insert if not exist" = unique index / constraint on code (counter ke upar aakhri pehra).

**2. Saal bhar me counter 10 arab pe. Token "10000000000", 11 akshar, aur lamba hota jaayega. Number wahi rakhte hue token chhota kaise? (Sawaal: gaadi ki number plate "DL 3C AB 1234" me sirf number nahi, akshar bhi.)**
Arpan: Uske peeche kuch laga do, kuch aisi algorithm set kar do ki wo number apna bhi chhota kar de. Technical terms me kuch hota hoga. Jaise DL 3C, phir agla aa jaayega 4C, 5C, phir 5D, phir A, AB ke baad CB, ye aata rahega. 1 2 3 4 ki jagah aisa kuch, hum combination bana sakte hain. 7 character hain to kaafi combination ho sakte hain.
Technical me: "number plate jaise akshar bhi, combination" = base62 (0-9, a-z, A-Z), "7 character, kaafi combination" = 62^7, ~3.5 lakh crore code.

**3. Token lene kam aate, token dikha ke pata lene wale bahut: har second 1 lakh. Counter wala har baar mota register palat ke dhoondhta, line ruk jaati. Kuch token din me hazaar baar aate.**
Arpan: Unhe chhote register me rakh lo. Jo bhi log dekhne aa rahe, bas usi register se wapas kar do. Mota register to rakh lo, koi dikkat nahi, ek chhota pehla register rakh lo, usme wahi wale token rakh lo. Jo log bahut baar poochhne aa rahe, wo wahin se chale jaayenge.
Technical me: "chhota pehla register" = cache (Redis), "mota register" = DB, "bahut baar poochhe jaane wale" = garam / baar-baar padhe link.

**Maan lo pooche server ki memory me hi kyun nahi rakh lete, baar-baar DB kyun:** Arpan: Sabse seedha design ek server aur ek DB: link aaya to code banao, DB me likh do; kisi ne khola to DB se bada link de do, 302 redirect. Memory me rakhne ka sawaal aa sakta hai, par server restart ho gaya to link saare gaayab ho jaayenge, isliye memory me nahi rakh sakte.
Technical me: "restart pe gaayab" = server ki memory volatile, link ko pakki jagah (DB) chahiye; tez padhne ke liye alag chhota register (Redis).

**Maan lo chhota register (Redis) gir gaya:** Arpan: Redis gira to copy (replica) rakh lo. Copy bhi giri to aakhri kaam fail open kar do: sab dobara DB se aa jaayega. DB to zinda hai, wahan se aa jaayega, itne me Redis dobara khada ho jaayega aur waise hi chal padega.
Technical me: Redis replica; dono gire to fail open (cache chhod ke seedha DB se), Redis wapas aate hi phir cache se.

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

**Maan lo ek counter apni range ke beech, 400 pe, crash ho gaya:** Arpan: Dikkat ho sakti hai, us range ke baaki number bekaar gaye. Lekin hamare paas 62^7, 3.5 trillion aur bhi hain; kuch bekaar ho jaayenge, koi farak nahi padta, app chalta hi rahega.
Technical me: range ke bache number kho jaate (gap), koi dikkat nahi; counter number wapas nahi maangta.

**8. 5 saal me mote register me itne token (90 TB) ki wo ek almari me aata hi nahi.**
Arpan: Shard kar do, chhote-chhote tukde bana do. 90 TB kab tak sambhaal loge, wo bharta jaayega. 10 TB ke 9 tukde kar do, aur sab tukde alag-alag rakh do: ye yahan, ye yahan. Aur use hash se baant do, ki ab kabhi aayega to wo is tukde pe chala jaayega, phir same aayega to phir usi tukde pe.
Technical me: "chhote tukde, alag almari" = shard, "hash se baant do, same aaye to usi tukde pe" = code ke hash se machine tay.

**Maan lo almari phir kam padi, 10vi lagani hai:** Cassandra khud sambhalta: token ke hash se tukda wahi tay karta, nayi machine lagao to apne aap thoda data baant deta. Uska andar ka kaam, hamara faisla nahi.
Technical me: Cassandra andar token ring (consistent hashing) se baantta, official doc: https://cassandra.apache.org/doc/latest/cassandra/architecture/dynamo.html
Arpan (jab 9 almari bhar gayi, 10vi aayi): Hum mod karte the na, to mod se ab har cheez badal jaayegi. To aisa karo ki in sabko gol chakkar me bitha do. Token ka hash aaya, maan lo 5vi aur 6vi ke beech gira, to jo sabse paas wali almari hai wahan chala jaayega.
Technical me: "mod se har cheez badal jaayegi" = hash % N me N badla to lagbhag saare token nayi almari pe, "gol chakkar, sabse paas wali" = consistent hashing (ring pe agli machine).

**Isi dikkat ki doosri kahani: mota register state ke hisaab se kaato.**
Arpan: Register 100 page, phir 200, phir 10,000, phir 20,000; kitna mota karenge, ek limit to hogi. To mote register ko tod do: 50,000 page ka hai to 15,000 alag, 15,000 alag, 20,000 alag. Par ab 3 register hain, kisi ko data kahan milega, teeno me dhoondhne me pagal ho jaayenge. To baantte waqt har register ke upar thappa laga do ki isme ye pattern hai. Register baantna achhi baat, par trade-off: dhoondhne me samay lagega, isliye tabhi jab sach me bahut zaroorat ho; aur shard karna AWS ka kaam, hum utna gehra nahi padhte. Scene: mote register pe Delhi, Ambala, Chandigarh, Punjab sab ke log aa rahe. To Delhi wali information kaat ke Delhi ke office me rakh do, Punjab wali Punjab me, UP ki East UP aur West UP me. Ab Punjab ke log Punjab ke register pe, Delhi ke Delhi me, UP ke UP me; dhoondhne ki zaroorat nahi kyunki pata hai kaunsa kis ka.
Technical me: "region ke hisaab se kaato" = sharding by region (geo-partition), "register pe thappa" = shard ka record (kaunsa data kahan).

**Maan lo Ravi ne Delhi me link banaya, Delhi ke register me gaya. Mumbai wala dost kholta hai; counter ke paas bas token aB3x9. Use kaise pata Delhi ke register me jaana hai?**
Arpan: Ek code bana lo: Delhi ka 01, Punjab ka 03, UP ka koi aur; state 25-30 hain to 30 number kaafi. Ye code link me sabko nahi dikhega, server chupchaap aage ya peeche jod dega. Ek bada bhai beech me baitha hai, uske paas list hai: 01 wali, 02 wali. Use bol diya: tere paas link aayega, uske shuru ke 2 digit uthaana, list se milana kis state ka hai, aur request wahin bhej dena. Mumbai se aaya, shuru me 01 likha, list me 01 = Delhi, to Delhi bhejo. Kahani khatam.
Technical me: "token me state ka code" = shard key ID ke andar (shard id prefix), "bada bhai list dekh ke bheje" = router / lookup (shard map).

**DB kaunsa:** Arpan: Cassandra, kyunki likhai bahut hai; link pelam pel ban rahe, aur yahan poori consistency chahiye bhi nahi, nature of data hi aisa hai: key-value pair (token -> lamba link). MongoDB, DynamoDB, Cassandra jaisa; Cassandra 90 TB ke liye bhi aaram se scale ho jaayega, saalon tak dikkat nahi. Aur Cassandra apne andar copy (replica) aur node wali consistency khud sambhalta, hamein zyada lena-dena nahi.
Technical me: key-value + bahut likhai + saalon ka data = Cassandra (wide-column, apne aap replica + scale). Cassandra kyun: data key-value, likhai bahut, aur data me bank jaisi consistency (paisa nikaalna, transaction) chahiye hi nahi.

**9. Almari number 3 me aag lagi, uske saare token jal gaye.**
Arpan: Ek almari mat rakho, unhi ki copy bana do. 3 number ki almari gir gayi to uski ek aur copy hai. Ab isme dikkat ye aa sakti hai ki copy thodi si peeche ho sakti hai, to wahi acknowledge wala kar lo: jab tak acknowledge nahi hota wo wahin rahega, "haan bhai maine likh liya, tu acknowledge bhej". Same dikkat ka same solution ho gaya.
Technical me: "almari ki copy" = 3 copy (replica), "acknowledge aaye tab pakka" = quorum write (W=2).

**Isi dikkat ki doosri kahani:** Arpan: Delhi me aag lage to Delhi ka register jal jaaye, pagal thodi hain. Copy alag-alag rakh do: building me Delhi ka register hai, mujhe pata wo bahut important hai, to socho is floor pe ya building me aag lag gayi to dikkat; isliye copy Lajpat Nagar, ISBT, Kashmiri Gate rakh do. Trade-off: copy peeche chal sakti hai, ek link copy tak pahunchte-pahunchte beech me gir gaya; koi baat nahi. Ek link bana, par kaise na kaise DB me save nahi hua: server gira, sab crash, rollback karna pada, link ka kuch ata-pata nahi. To user ko dikha do "tera link bana nahi, dobara banana padega"; aisa nahi ki "save" bol diya aur link khatam. Hamare paas arabon number hain, ek-do number aise bekaar ho bhi jaayein to dikkat nahi, doosra number le ke naya bana lo. Beech me ek central authority rakh dunga: jo bhi aaye pehle uske paas, wo apne paas likhe aur phir ISBT, Kashmiri Gate ko pass on kare. Wo bhi kabhi gir jaaye to uski jagah uski ek aur copy rakh do. Peeche kaam chalta rahega, copiyan update hoti rahengi.
Technical me: "copy alag-alag jagah" = replica (alag zone), "central authority pehle likhe phir bheje" = coordinator (jo request leta, ring me aas-paas ki copies ko aage bhejta), "copy peeche chale, baad me update" = async replication lag (save fail to user ko "dobara banao", number bekaar gaya to naya number, arabon hain), "authority ki bhi copy" = wo gire to koi aur coordinator ban jaaye. Cassandra andar se kaise karta, wo uska kaam, design ka hissa nahi.

**10. Teen copy A, B, C. Arpan ne naya token banaya, A aur B ne haan bola, C peeche. Turant dost ne token dikhaya, counter wale ne C se padha, "nahi mila".**
Arpan: Humne A, B pe likha na, to token ko do se hi padhwao. Agar do likh rahe hain to do hi padhenge, ek nahi padhega. Mere dost ne token maanga, to use B aur C se padhwao, ya A aur C se, single-single se nahi. Maan lo ek copy peeche bhi reh gayi, to atleast ek ke paas to hai na. C nahi hai to B ke paas to hai.
Technical me: "do se hi padhwao" = R=2, "ek ke paas to hai" = W + R > 3, ek copy common.

**11. Cricket match ka token viral, 1 crore log ek saath. Sab counter wale us token ke liye ek hi chhote register ki taraf bhaag rahe, wo itna dheema ki uske baaki token bhi atke.**
Arpan: Sabko wahi token chahiye. To us chhote register ko bahut saaron me baant do. Ek hi kyun hai wo register, 10 register bana de, uski photocopy bana de aur divide kar de. Ek hi chhote register pe ek saath itne log nahi aa rahe, wo bat gaya. 10 kar do, 100 kar do.
Technical me: "chhote register ki photocopy, divide" = Redis replica (padhai copies me bantna).

**Maan lo copy 10 ho ya 100, har baar counter wale ko kursi se uth ke register tak jaana padta. 1 crore log, 1 crore chakkar.**
Arpan: Us counter wale ko bol: ye information tu rakh le, baar-baar uth ke kyun ja raha hai. Ek chhoti parchi le, uska token aur information apne paas rakh. Maan le roz 100 log aaye, ek hi information maang rahe, aur wo mere se pachaas kadam door hai, mujhe har baar wahin jaana padta. Main wo information utha ke apne paas ek page pe likh lunga, aur apni kursi pe baith gaya. Ab koi poochhe to "le bhai, teri information ye rahi". Mujhe dobara kahin jaana nahi padta.
Technical me: "counter wala apne paas parchi rakhe" = local cache (app server ki memory me garam link kuch second).

**Maan lo wahi cricket wala token roz ka garam hai, 10 photocopy pe pelam pel log. Theek 7 baje photocopy 7 pe uski umar khatam, wahan ek lakh log ek saath, ladke ko mote register se aane me 2 second. Aur purani information? Wo to umar khatam hote hi mit gayi.**
Arpan: Sabse pehle to aisi parchi ko ek hi baar me khatam hone hi nahi dunga. Wo parchi bahut garam hai, us pe pel ke users aa rahe, 1000 nahi, 10 lakh, 20 lakh, pelam pel. To main us pe nazar rakhunga: chhotu dekh, sun idhar aa, iski umar abhi 20 second ki hai, badha de, 60 kar de, phir +60, phir +60. Aur 10 lakh log aa rahe to ek counter pe nahi, 10 pe baant do, aur sabki umar ek saath khatam nahi hogi, kisi me thoda jitter, kisi ki 20 second me, kisi ki 30 me, kisi ki 50 me. Maan lo kuch der ke liye meri nazar hat gayi aur counter 7 ki umar khatam ho gayi, wahan ek saath ek lakh request aa gayi. Kya wo ek lakh DB pe jaayengi? Nahi. Mote register pe bheed lag jaayegi, aur bhi log wahan apna kaam kar rahe, unke liye dikkat hogi. To main bolunga: sab ruko, chupchaap, ekdam shaanti. Chhotu bhai ja, yahan parchi ud gayi hai, mote register me ja, jo ye log maang rahe uska pata la, aur yahan likh de. Chhotu parchi le ke aaya, maine dekha kya likha hai, likh liya: aa jao sab, dobara aa jao. Ek parchi nahi hai. Mujhe pata hai 60 second me ye khatam ho gayi to sab log ek saath aa jaayenge, mujhe dikkat ho jaayegi. To main chupchaap ise ek aur parchi pe likh ke apne paas rakh leta hoon, jisme information pehle se hai. Asli wali khatam ho gayi to bolunga: bhai abhi ke liye ye le lo, thodi der purana dekh lo. Phir wahi chhotu jaayega, la ke parchi dega: le bhai, updated information, ab sab yahan se padh lo. Apni parchi maine phaad ke phenk di, khatam kahani. Purani information app ke cache se, browser ke cache se bhi de sakte; un pe thodi umar rakho, 60 second purana dekh lenge, koi baat nahi, phir wo udega aur log Redis pe aayenge, wahan naya mil jaayega. Chhotu gir gaya to ek aur chhotu laga do: yaar ye to 2 second ka kaam hai, use 10 second ho gaye, chhodo use, tu chala ja. Backoff tab, jab maine 10 chhotu bheje aur ek bhi nahi aaya; tab maanunga ki DB gira. Ek chhotu gira to DB nahi gira, baaki counter to chal rahe hain.
Technical me: (jawab me "counter 7" = photocopy 7, "10 counter pe baant do" = 10 photocopy, "chhotu" = kadam 5 wala ladka.) "umar badhate raho" = hot key TTL extend (refresh-ahead), "photocopy ki umar alag" = TTL jitter, "chupchaap doosri parchi" = stale copy (soft / hard TTL, stale-while-revalidate; kadam 6 wali purani information yahi), "counter ki parchi chhoti umar" = local cache short TTL, "10 na aayein tab backoff" = backoff: ladka bheja, nahi aaya, thoda ruko, doosra bheja, nahi aaya, ab thoda zyada ruko, teesra bheja; har baar intezaar badhta (1, 2, 4 sec), mote register pe bombard nahi. 10 me se ek bhi na aaye tab maano mota register (DB) gira. Jitter iska hissa nahi: jitter upar wala "photocopy ki umar alag" hai, photocopy ki umar me thoda +1, +2 jod do taaki sab ek saath na mitein.

**Maan lo 50 counter hain, log kabhi kisi counter pe, kabhi kisi pe (LB). Photocopy 7 pe token nahi mila, to har counter wale ne apna-apna ladka bhej diya. Mote register pe ek ki jagah 50 ladke.**
Arpan: Sahi baat hai, LB to round robin se bhej raha, kabhi kisi pe, kabhi kisi pe: pehla aaya counter 1 pe gaya, phir counter 10 pe, phir 17 pe, phir 20 pe, aur har counter ka apna chhotu mote register ki taraf bhaaga. To main bahar saamne khada hoon, sab counter ko dekh raha hoon. Ruko sab, hold on, chup, ekdam shaanti banaye rakho. Saare 50 counter ko roko, bas 1 second, kuch nahi. Chhotu log chup ho jao, shor nahi, sabke chhotu apne counter pe jaake chup khade ho jao. Ek counter ka chhotu bulaya: bhai chhotu sun, ye sab pagal hain, inki baat na sun, faltu ki baat kar rahe tere se, idhar aa, mote register pe ja, usme information padi hogi, padh ke sabko de de, khatam kahani. 50 chhotu ko jaane ki zaroorat hi nahi, 50 ki jagah 1 chhotu bhi kaam kar lega. Ek information hi to laani hai yaar, 50 chhotu kyun bhejne? Ek chhotu jaayega, 50 ka apne aap update ho jaayega.
Technical me: (yahan "counter" = counter wala, yaani hamara server; "chhotu" = us counter ka ladka.) "sabke saamne ek banda, sirf ek ladka" = distributed lock (saajha lock, Redis me ek key: SET NX + umar). Kadam 5 ka ladka sirf ek counter ka ho to 50 counter ke 50 ladke jaate, isliye lock saajha.
Isi ki doosri kahani: Arpan: 3-4 counter phans gaye to unke chhotu ko bol do: ek chhotu jaayega, information laake teeno chhotuon ke kaan me daal dega, wo apne register me khud update kar lenge; 3 chhotu ek saath nahi jaayenge. Waise 5-10 chhotu ek saath mote register pe jaayein to utna wo sambhal lega, AWS ke server itna sambhalte hain; dikkat ho tabhi.
Technical me: "ek chhotu laake sabke kaan me" = request coalescing (ek fetch, baaki ko wahi jawab), saajha lock ke saath.

**Isi dikkat ki doosri kahani:** Arpan: Sabse pehle, link badal nahi raha, to use local cache aur server ke cache me daal dunga, Redis tak aane hi nahi dunga. Redis dheema hua to copiyan jod do. Aur us link ke naam ka ek alag dabba, alag Redis rakh do: bheed wo jhelta rahe, apna baaki kaam chalta rahe. Saath rakha to kuch log apna important mail ya kaam dekh rahe, IPL ki request ke peeche unki dikkat ho jaayegi. 1 crore log aa rahe to ek alag dabba, wo yahin aayenge. Uski 1-2 copy aur rakh do, kharcha badhega, par DB ya poora app girne se bacha lenge. Ye consistent hashing nahi hai, isliye uska yahan koi matlab nahi: link update nahi ho raha, bas match dekhne ke liye. Main bas Redis ki copiyan ek ring me rakhta hoon: ek copy yahan, ek yahan, ek yahan; aas-paas koi gira to agli copy pe aake information pass on kar dega.
Technical me: "server ke paas" = local cache, "us link ka alag dabba" = hot key isolation (alag Redis / alag pool, baaki link na atkein), "1-2 copy" = hot key replicas, "copiyan ring me, gira to agli copy" = replicas ring pe (failover agli copy pe), consistent hashing nahi.

**12. Kisi ka idea: har aadmi pata apne phone me hi likh le, counter pe aaye hi nahi. Hafte baad pata chala ek token ka pata dhokhe wali site ka hai, use band karna hai, par log seedha phone wale pate pe ja rahe.**
Arpan: Token radd kar do, ki ab wo galat site pe nahi jaayega. Doosri cheez, aisi kisi cheez ko hum pehle se hi dekh lenge ki wo theek bhi hai: user ko dene se pehle khud check kar lo ki jo wo bana raha hai wo theek bana raha hai ya nahi. Galat bana raha hai to mana kar do banane se, aur us token ko radd kar do.
Technical me: "token radd" = code band, "dene se pehle khud check" = kaali list (Google Safe Browsing).

**Maan lo token radd kiya, par log counter pe aa hi nahi rahe. Unhe kaise pata chale? Shuru me phone me likhne dena tha ya nahi?**
Arpan: Unke phone me wo cheez update kar do na. Wo cookies aur cache se aa rahe honge, to unka cache radd kar do, hata do, to agli request seedha yahan aayegi. Phir (jab poocha ki phone uske haath me hai, tu kaise mitayega): To phir phone pe likho mat na bhai. Phone pe likhega nahi to wo hamare counter pe dobara aayega, dobara hum use theek token de denge. Agar aisi phone wali dikkat hai to uske phone me likhna hi mat do, saaf baat hai.
Baad me Arpan: aam taur pe phone ke cache ko TTL laga ke invalidate kar sakte hain, lekin hum technical nahi, aam dikkat ka solution bol rahe the, isliye thodi der me samjha.
Technical me: "phone me likhna hi mat do, har baar counter pe aaye" = 302 + `Cache-Control: no-store` (save hi mat karna; 301 nahi), "phone me likh le" = 301 (browser cache), "phone cache pe TTL" = browser cache ki umar.
**302 ka doosra fayda:** Arpan: Link ko track bhi karna ho to main 302 redirect kar sakta hoon, aur peeche apna dashboard bhi bana sakta hoon, koi dikkat wali baat nahi.

**Maan lo dashboard banana hai, har click ginna hai, par user click karke khada na rahe:** Arpan: User ko link do, use free karo, wo chala jaayega. Peeche Kafka, worker, queue rakh do, ye sab peeche apna dashboard banate rahenge. Kuch fail ho jaaye to dead letter queue hai hi, wahan daal do, baad me investigate karke theek kar sakte hain.
Technical me: "302 se har click hamare paas" = click tracking / analytics, "peeche queue, worker" = async click events (Kafka -> consumer -> dashboard), redirect dheema nahi hota, "fail to alag dabba" = DLQ.

**Isi dikkat ki doosri kahani:** Arpan: Link banate hue chupke se khol ke dekh lo ki theek hai ya nahi. Koi mere paas kitaab rakhne aaya, cover chadha rakha; main chupke se dekh lunga kitaab sach me theek hai ya nahi, tab library me rakhunga, warna kal log aake kahenge "kaisi kitaab rakh rakhi, police case kar raha". Kitaab gandi nikli to "nikal beta, chal nikal". Site ka koi score-vagairah peeche hota hoga, wo check kar sakte. Theek hai to user ko do, nahi to mat do.
Technical me: "banate waqt chupke se dekho" = create time pe URL safety check (reputation / kaali list), "gandi to mat do" = reject.

**13. Ek aadmi ne machine laga di, har second hazaar chit, sab nakli ya dhokhe wale pate. Counter ka number faltu khatam, register bhar raha, asli log atke.**
Arpan: Jaanch padtaal karenge ki script laga di. Ek-do baar to aisa hoga hi, teen baar, chaar baar, phir pattern pata lag jaayega ki ye banda to har baar aa raha hai, har baar ye drama karke chala jaata hai. To us bande ko rok do, andar ghusne hi mat do, wo kamre me hi nahi aayega, use bahar gate pe hi bhaga do.
Technical me: "pattern pata lag jaayega, gate pe bhaga do" = WAF block.

**Maan lo pakadne me 3-4 chakkar lagte, tab tak wo laakhon chit de chuka. Shuru se har aadmi ke liye kya niyam?**
Arpan: Yahan thoda pench hai. Had ek user, ek aadmi pe rakhenge. Kyunki maan lo ek office hai, wahan wo log unke WiFi se aa rahe hain, to dikkat ho jaayegi. Ek group ek saath aa gaya, lekin wo sab legit users hain, unko to token dena padega. To per user pe lagao, group pe nahi: ye ek banda hai, ek baar me 5 hi lagayega, usse zyada nahi.
Technical me: "per user had, group pe nahi" = rate limit user pe (IP pe nahi), "office ka WiFi" = ek IP ke peeche kai asli user, "5 se zyada nahi" = had paar pe 429.

**Maan lo bina login ke bhi link banta, script har second hazaar link bana rahi, userId hai hi nahi.**
Arpan: Sabse pehle login laga do: bina login link nahi. Isse dashboard / tracking bhi chalega. Login nahi rakhna to tracking bhool ja, aur IP pe had: IP ki had thodi zyada, userId ki thodi kam. Script hazaar link nahi bana paayegi. Baar-baar aaye to IP ban. Naye-naye IP se phir bhi aaye to peeche investigate karo, aur ek hi desh se aa raha to us desh ko thodi der WAF ki block list me daal do.
Technical me: "login laga do" = auth zaroori (tab userId pe had + tracking), "bina login IP pe dheeli had" = anonymous rate limit per IP, "IP ban, desh block" = WAF block list / geo-block.

**Isi dikkat ki doosri kahani:** Arpan: Ek aadmi ne script chala di aur lagbhag 1 lakh link le gaya, pata hi nahi laga kaise aaya kaise gaya. To ek bodyguard khada karo: kuch bhi suspicious lage to rok do. Suspicious matlab abnormal: koi script le ke aa raha, ek hi aadmi laakhon request, ek hi IP se 1000 request ek saath. Baar-baar aaye to gala pakad ke jail me daal do, dobara nahi aayega. AWS ke gateway pe hi rok denge, phir block list me.
Technical me: "bodyguard" = API gateway / WAF pe rate limit + anomaly check, "jail" = IP / user ban.

&nbsp;

Poora design = [01_url_shortener.md](01_url_shortener.md)
