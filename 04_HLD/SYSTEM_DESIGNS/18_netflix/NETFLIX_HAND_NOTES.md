# Netflix: hand notes (Arpan, Ravi aur Money Heist ka naya season)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Maan lo Money Heist ka naya season Friday dopahar 12:30 baje poori duniya me ek saath. Arpan, Ravi aur India ke 20 lakh log 12:30 pe ek saath play dabate hain, saath me US, Spain, Brazil bhi.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Ravi ka TV  -->  AWS (login, list, pasand wali list, kaunsa dabba = steering)   video bytes AWS se nahi
                       |                                                        |
              watch position: pehle cache, DB kabhi-kabhi                 ISP ke andar Netflix ke dabbe (Open Connect)
              pasand wali list: raat ka batch + abhi ki parchi            raat ko pehle se bhare, garam paas, thanda bade dabbe se
                                                                          dabba band = TV agla dabba; player buffer

 1   har tukda samundar paar se                  ->  file apne shehar me (Open Connect, ISP ke andar)
 2   12:29 tak dabba khaali                      ->  pehle se bhar do, video badalta nahi (proactive fill)
 3   dabbe me poora maal nahi                    ->  garam paas, thanda bade dabbe se (popularity, tier)
 4   20 dabbe, kis se maange                     ->  server batata (steering, AWS pe)
 4b  aadhe episode pe dabba 3 band               ->  TV ke paas 2-3 naam, agla (client failover)
 5   TV pe 27 min, phone pe wahi se              ->  account pe parchi (watch position)
 5b  TV ka purana 27, phone ke 37 ke upar        ->  baad wala jeete (timestamp, last-write-wins)
 5c  har kuch second update, DB ki saans         ->  pehle cache, DB kabhi-kabhi
 5d  tukde ISP ke dabbe se, AWS ko kaise pata    ->  TV sankshep me bataye
 6   pasand wali list me 6 second                ->  raat ka batch, cache se
 6b  11 baje Dhurandhar dekhi                    ->  turant parchi, dikhate waqt milao
 6c  Priya naya user                             ->  India ka garam maal (cold start)
 7   tunnel me 3 sec net gaya                    ->  player buffer, nahi to doosri quality
```

**FR / NFR kahan se:** upar ki dikkat chain dekh ke nikale; har line ke aage likha hai wo kis kadam se aayi.

**Arpan:** "Buffering nahi" NFR galat hai. User ka net bahut mandda hai, 144p pe bhi nahi chal raha, to buffering hogi hi; quality 180p se 720p ya 360p badalti hai tab bhi hoti hai, Netflix me buffering hoti hai. Net ke hisaab se hogi. FR / NFR ki bhasha wahi jo neeche design me likhi, alag shabd ("tere liye" jaisa) nahi.

*Technical me:* NFR = jaldi shuru (low start-up time) + adaptive bitrate (ABR), buffering zero ka vaada nahi.

**Isi pe ek aur raasta (round me: FR / NFR, numbers, dhaancha) (Arpan):**
```
Pehle scope saaf karo: recommendation chahiye ya nahi, kitna chahiye.

FR       user ko video dikhe, wo video khole, play / pause kare, search kare.

NFR      video net ke hisaab se ho; net achha chal raha to badhiya quality, achha nahi to theek-thaak quality.
         "Video buffer na ho" koi requirement nahi: net band ya slow hua to buffer hoga hi, Netflix bhi buffer
         karta; quality 144p tak gir jaaye to dekhne ka fayda nahi.

Numbers  poori duniya me 1 billion user, 10-20 million roz (DAU), matlab bahut scalable system chahiye.

Dhaancha user app pe aaya, bahut saare server, beech me LB; LB ke baad local cache, phir Redis, phir CDN, CDN ke
         peeche S3.
         Video kahan rakhta wo Netflix ka andar ka system hai, hamara lena-dena nahi.
         Video koi DB me nahi rakhta, wo S3 jaisi cheez me; CDN Netflix ne apne bana rakhe (Open Connect), paas
         ke server pe video taaki kam se kam time me dikhe.

DB       Netflix andar bahut saare DB use kar sakta, Cassandra / NoSQL type shayad mushkil, par kisi use case ke
         liye wo bhi kar le; mere hisaab se mere hisaab se Postgres theek, kyunki writes bahut nahi: Netflix pe
         hum user video upload nahi karte, Netflix rights khareed ke khud daalta.
         Postgres me user ki jaankari, uski recommendations, wo roz kya dekhta.
         Ya recommendation ke liye Neo4j bhi le sakta hoon.
         Netflix ka design public nahi, andar kya use karta pata nahi.
```

*Technical me:* FR = browse, play / pause, search · NFR = adaptive bitrate (ABR), buffering zero ka vaada nahi · dhaancha = app ka raasta LB -> app servers -> local cache -> Redis; video ka raasta alag: TV seedha CDN (Open Connect dabba) se, uske peeche S3 · Postgres (RDS) = user, list, dekhne ka itihaas (dekhne ki likhai bahut ho to kadam 5c: pehle cache, DB me batch) · Neo4j = graph DB (kaun kis jaisa dekhta).

**API (round me):**

**Arpan (API):** POST wala video upload nahi chahiye, kyunki video Netflix ke andar hi daali jaati, hum upload nahi karte. GET filmein / series: kitni filmein, kitni series, Netflix ke server se jo data aa raha. GET recommendation: poori pasand wali list. GET dekhi hui: 3 saal purane user ne jo dekh rakhi (Stranger Things, Money Heist). POST search. POST payment wali. Ye 4-5 main API, jin pe system tika hai.
```
API     GET  /titles             -> filmein + series ki list
        GET  /recommendations    -> pasand wali list (pehle se bani, kadam 6)
        GET  /continue-watching  -> dekhi hui + kahan tak (kadam 5)
        POST /search             -> dhoondho
        POST /payment            -> subscription
        video upload ki API nahi (Netflix khud daalta)
        video ke tukde API server se nahi, paas ke dabbe (Open Connect) se
```

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      1  video chalana (play)                                             -> kadam 1, 4, 7
        2  video beech me roko, kisi bhi device pe wahi se aage chale       -> kadam 5, 5b, 5c, 5d
        3  app kholte hi user ki pasand ke hisaab se videos ki list         -> kadam 6, 6b, 6c

NFR     1  play dabate hi video jaldi shuru ho; net ke hisaab se quality ghate-badhe
           (net bahut kamzor ho ya quality badle to thodi buffering hogi hi) -> kadam 1, 2, 4b, 7
        2  kamzor net aur purane TV pe bhi chale                            -> kadam 7, P3
        3  pasand wali list jaldi khule (thodi purani chalegi)              -> kadam 6, 6b
        4  data pahunchane ka kharcha kam                                   -> kadam 1, 3
Numbers India me 20 lakh log ek saath, saath me baaki duniya -> ek jagah se sabko video nahi bhej sakte
        · har film kai size me -> poora maal kisi ek dabbe me nahi samaata
```

&nbsp;

---

**1. Saari episode US ke store me. 12:30 pe karodon log wahi file maang rahe, har tukda samundar paar se, Ravi ke TV pe buffering, data pahunchane wali company ka bill aasmaan pe.**

**Arpan:** Main roz wahi file lene doosre shehar jaata hoon, bus me tel phoonk ke. File ghar nahi la sakta, par apne shehar me to la sakta hoon. CA doosre shehar me rehta hai, roz sign karne bulata hai, to mera dost jo roz aata-jaata hai, wo file mere shehar le aaye. Main chupchaap jaaun, sign karun, wapas aa jaaun. Bekaar me baar-baar shehar kyun.

*Technical me:* "file apne shehar me" = ISP ke andar Netflix ka dabba (Open Connect), "ghar nahi la sakta" = poora video phone pe nahi rakhte, "dost roz aata-jaata" = dabba US ke store se bharta rehta.

**Maan lo pooche video cache (Redis) me kyun nahi:**

**Arpan:** Cache me video thodi rakhenge, cache us kaam ka nahi. Video ek dabbe me rakh lo (CDN). Is design ki sabse badhiya baat: video ban gayi, ab update nahi hogi, isliye hum use apne paas rakh sakte. Jaise maine pen drive me Money Heist rakh liya aur sabko baant raha, baar-baar laptop leke nahi ja raha.

*Technical me:* badi, kabhi na badalne wali file = CDN (edge), Redis = chhota garam data (KB), video nahi.

---

**2. Dabba lag gaya, par season 12:30 pe aaya, 12:29 tak dabba khaali. 12:30 pe 20 lakh ek saath, dabba sabko "laata hoon" bolta, saari request phir samundar paar.**

**Arpan:** Jab pata hai log kis liye aa rahe, to wo cheez pehle se laa ke rakh lo. Pen drive me Money Heist rakh liya, ab sabko baant raha: le tu bhi le, tu bhi le, buffering hi nahi. Aur ye cheez ban gayi hai, ab badlegi nahi, isliye rakhne me koi dikkat nahi.

*Technical me:* "pehle se laa ke rakho" = release se pehle raat ko dabba bharna (proactive fill), "badlegi nahi" = video static hai, isliye CDN pe rakhna safe.

**Isi pe ek aur raasta (round me: kahan maang, Netflix ko pata):**

**Arpan:** Netflix ko pata hai bada season aa raha aur kahan-kahan maang zyada hogi: Spain jaise desh me (Money Heist wahi bani), India, Australia, New Zealand.

Har baar maangne pe laana Netflix ke liye bahut dikkat: Spain me lakhon dekhenge tab laao, India dekhe tab laao, Australia dekhe tab.

Isliye Mumbai ke server pe pehle se laga ke rakh do; ek baar laao, rakh lo, phir sabko serve karte raho. Main roz CA se sign karwane doosre shehar kyun jaaun, dost roz aata-jaata hai, wahi le aayega.

Video ek baar bani to badalne ki nahi, isliye rakhne me dikkat nahi.

*Technical me:* region ke hisaab se maang pata = kis desh ke dabbe me kya pehle se bharna (proactive fill per region).

---

**3. Mumbai ke dabbe ki jagah seemit, Netflix ka saara maal (har film x kai size) nahi samaata.**

**Arpan:** Ek dabba nahi, kai dabbe le lo, jaise maal ek truck me na aaye to 3-4 truck. Par dabbe ki bhi seema hai, isliye dabbe me wahi daalo jiska craze hai. Garam cheez paas rakho, thandi nahi. Purana season koi aaj bhi dekhe to bade dabbe se mangwa lo, thodi der chhote dabbe me rakh lo, garmaahat khatam to wapas.

*Technical me:* dabba = CDN box (ISP ke paas) · "garam paas" = popularity se kya rakhna (region-wise), "bade dabbe se mangwa lo" = upar wala bada store / tier, "garmaahat khatam to wapas" = thanda maal dabbe se nikalna.

**Isi pe ek aur raasta (round me: mahina bhar garam, thande ka chhota tukda):**

**Arpan:** Saara maal main server pe banta (US me), bahar se to laana hi padega. Par jo garam cheez baar-baar maangi ja rahi, use baar-baar bahar se laane ka fayda nahi.

Money Heist ka season aaya, public jaanti ye mahina bhar chalega, karodon India me dekh rahe: paas wale dabbe pe rakh do, wahin se serve.

Stranger Things abhi garam nahi, 1-2 request aa rahi to koi dikkat nahi: uska chhota tukda rakh do, koi dobara maange to bade se le aao. Garam maal paas, thanda door; thande ki thodi request aaye to thodi der ke liye paas.

*Technical me:* popularity se tier: garam = edge dabba, thanda = maang pe bade store se (cache miss -> upar se bharo), thodi der rakh ke nikaalo (evict).

---

**4. Mumbai me 20 dabbe, kuch me Money Heist hai kuch me nahi, ek ki saans phool rahi. Ravi ke TV ko kaise pata kis dabbe se maange?**

**Arpan:** Akela bhaar kyun uthaun, 10 dost bulao, sabko dabba do, nikaal-nikaal ke do. Garam season ko zyada se zyada dabbon me rakho. Request hamare server pe aati hai, use pata hai kis dabbe me kya hai, wo mod dega. Hospital ka guard: wheelchair wale is line, mahila us line, buzurg us line.

*Technical me:* "zyada dabbon me" = hot content ki kai copy, "server mod dega" = play pe server batata kaunse dabbe se lo (steering), "guard" = load + maal dekh ke sahi dabba.

**Isi pe ek aur misaal (banner pe click):**

**Arpan:** Ravi ne Money Heist ke banner pe click kiya to Netflix ko pata lag gaya ye Money Heist dekhne aaya; jaise bank me koi AC ki hawa khaane nahi aata.

*Technical me:* play request me content id aata, server usi se dabba chunta (steering).

**Maan lo aadhe episode pe dabba 3 band ya dheema.**

**Arpan:** Saans phoolne wali baat, traffic baant do, ek dabba nahi. Money Heist ka garam season chal raha, pelam pel log dekh rahe, to use 20 dabbon me baant do.

Ek dabbe me rakha aur dhadadhad log aaye ja rahe, haath badhata hoon nikaalta hoon, haath dukh gaya; to main apne 10 dost ko phone karunga: aao, ye dabbe lo, isme Money Heist hai, nikaal ke do. Ab akela bhaad nahi uthana padta.

Ravi ke TV ko kaise pata 20 me se kis dabbe se maange? Hamein to pata hai Money Heist kis dabbe me hai, server use route kar dega.
Technical me (TV ki taraf se): ek dabba dheema / band to TV ke paas 2-3 dabbon ke naam, doosre se le (client failover).

*Technical me:* "2-3 dabbon ke naam" = server player ko kai CDN dabbon ki list deta, ek atka to player khud agle pe (client side failover).

**Isi Maan lo me aur neeche (dabba slow ho to doosri copy se):**

**Arpan:** Money Heist famous hai to use ek dabbe me mat rakho, aur dabbon me bhi rakh lo, paise ki kami thodi hai. Maan lo ek dabbe me bahut important paper hai jo sabko dikhana hai; bheed itni ki dikha nahi pa raha, to us dabbe ki bahut saari copy bana ke line se laga dunga, bheed baant di. Ab ek dabba slow hai to koi baat nahi, doosre dabbe se le le, intezaar kyun kar raha.

*Technical me:* garam cheez kai CDN dabbon pe, player ke paas list, ek slow to agle se (client side failover).

**Isi pe ek aur misaal (round me: 100 DVD):**

**Arpan:** Netflix ek dabbe pe nahi rakhta; Money Heist garam hai, isliye 10 dabbon pe, taaki ek gire ya dheema ho to kaam na ruke. Maan lo maine kahin se Money Heist ki 100 DVD ka jugaad kar liya, logon ko pata chala to ekdum bheed. Main akela itni bheed nahi sambhalunga, 10 dost bulaunga: lo 10-10 pakdo, sab jagah baant do, paise le lo. Ek dheema hai to koi dikkat nahi, doosre chal rahe.

*Technical me:* garam maal ki kai copy (replication), ek dabba atka to doosre se (client failover).

---

**5. Ravi ne TV pe 27 minute dekha, band kiya. Subah metro me phone pe khola, 27:00 se chahiye.**

**Arpan:** Ravi ka account hai, Netflix account se yaad rakhta. Jaise kisi ke 10,000 rakhe, parchi pe uska naam likha, bus stand pe mila to pehchaan liya. TV, phone, kahin bhi khole, yaad wahi. Parchi DB me.

*Technical me:* "account se yaad" = watch position user ke account pe server me, device me nahi, "parchi DB me" = persistent store.

**Aur neeche (parchi cache me ya DB me):**

**Arpan:** Parchi cache me nahi, DB me rakhunga. Netflix andar kya use karta mujhe nahi pata, par main Netflix hota to yahi karta.

*Technical me:* dekhne ki jagah = DB (pakka), cache sirf tez padhne ke liye.

**Isi pe ek aur raasta (round me: apna cache uda do tab bhi 27):**

**Arpan:** Netflix account banwata hai, sign in karwata hai, matlab account me bahut kuch rakhta: is ladke ne kya-kya kahan tak dekha, kya dekh raha, kya recommendation.

Arpan ne phone pe 27 minute dekha to server ko pata laga, aur wo bahar jaake chilla deta: Arpan ne 27 tak dekh liya, jahan-jahan iska account hai sab sync kar lo.

Ab account kahin bhi khule, TV, laptop, phone, London ho ya India, 27 se chalega. Parchi account pe hai. Apne phone ka cache uda diya to bhi Netflix ke paas 27 hi rahega: wo tera cache hai, Netflix ka apna alag.

Netflix andar kaise sync karta wo public nahi, hum bas itna bolenge: account pe sync.

*Technical me:* watch position server pe account ke saath (asli sach), device ka cache sirf apni copy.

**Maan lo TV ka net gaya, TV ne "27:00" apne paas rakh liya. Subah phone pe 37 tak dekha, shaam ko TV on hote hi purana 27 pahuncha aur 37 ke upar likh gaya.**

**Arpan:** Netflix phone, laptop, TV sab me sync rehta, alag nahi ho sakta. Main Netflix ka server hoon: tukde main hi bhej raha, to 27 ke baad agla tukda nahi maanga matlab 25-27 ke aas-paas band kiya.

Phir usne phone pe 27 se 37 dekh liya, naya point 37, parchi pe 37. Shaam ko TV khula aur purana 27 aaya: nahi, isne to 37 tak dekh liya, 27 purana hai, discard; 37 se chalao.

Account ek hi hai, kahin se bhi login karo wahi dikhe; TV khulte hi net se account ka sach padhta hai, apna purana nahi thopta. Naya 37 aaya to purana 27 kaat ke 37 likh do.

*Technical me:* "naya point jeete, purana 27 discard" = har position ke saath uska waqt (timestamp), baad wala jeete (last-write-wins, event time se, aane ke waqt se nahi), "main Netflix hoon, tukde main hi bhej raha" = ISP wala dabba bhi Netflix ka (Open Connect), agla tukda nahi maanga to andaza 25-27 ke aas-paas.

**Isi pe ek aur misaal (round me: librarian):**

**Arpan:** TV ka net gaya to server ko itna pata ki aakhri tukda 25 se 27 ke beech kahin (maan lo 26:30 tak) diya, parchi pe wahi. Phir mobile pe 26:30 se 37 tak dekha, parchi kaat ke 37. TV khulega to parchi padhega: 37. Jaise librarian: koi kitab le gaya aur 8 din se nahi aaya, main bhoolunga nahi, sab likha hai kisne kaunsi kitab kab li.

*Technical me:* server ka register hi sach (server jaanta kaunsa tukda diya), TV khulte hi account se padhta.

**Maan lo karodon TV har kuch second "26:50 pe hoon" bhej rahe, DB ki saans phool gayi.**

**Arpan:** Race me har kadam thodi likhte, jab ruk jaaye tab likho. Abhi parchi paas rakho, 27 pe ruk gaya to register me daal do.

*Technical me:* "parchi paas" = position pehle fast store me, "ruk gaya to register" = DB me sirf aakhri / kabhi-kabhi likhna (write kam).

**Isi pe ek aur raasta (round me: chhotu ki sync job):**

**Arpan:** Har baar Postgres pe likhne ka matlab nahi, server ki parchi (cache) pe rakh lo.

Register sab padh rahe: Arpan 27 page tak, Ravi itne page tak, sab mere paas likha; har baar peeche wale mote register (DB) me update karne nahi jaunga.

Thodi der baad sync job: chhotu idhar aa, ye sab likha hai, peeche DB me update karta ja. Chhotu gira to pata nahi kahan tak hua: DB se poochho "tu kahan tak update hua", DB bata dega. Parchi ki 3-4 copy apne paas.

Peeche DB ki replica hai to wo bhi theek. Chhotu gira to doosra chhotu. DB ki saans nahi phoolegi, kyunki hum baar-baar DB pe nahi jaate, batch me kaam karte.

*Technical me:* pehle cache me (copy ke saath), phir sync job batch me DB me (write-behind / batch write); job gira to DB me jahan tak likha wahan se aage.

**Maan lo tukde ISP ke dabbe se aa rahe, AWS ke server ko kaise pata ki ruk gaya?**

**Arpan:** Do log baat kar rahe, ek mujhse baat nahi karta. Maine dost ko bola, jo baat hui sankshep me mujhe bata diya kar. Jis din kuch nahi bataya, matlab aaj aage nahi badha.

*Technical me:* "dost sankshep me bataye" = dabba / player playback ki report server ko bhejta.

**Isi pe ek aur misaal (round me: game parlour ka maalik, saar wahi: dabba AWS ko bata de):**
Pehle tasveer:
```
                 Ravi ka TV
                /          \
   (1) login, list,        (2) video ke tukde
       parchi (27:00)          (asli video bytes)
              |                      |
     AWS wala server          Mumbai ka paas wala dabba
     (Netflix ka dimaag)      (ISP ke andar, Open Connect)
     parchi / DB yahan        sirf video rakhta aur bhejta
```

**Arpan:** Aisa ho hi nahi sakta ki do dabbe aapas me baat na karein; microservice me bhi sabko pata hota.

Jaise HDFC: mera account Khatauli me khula, main Delhi gaya, to wo kahenge saari jaankari home branch me milegi, par itni yahan bhi de sakte.

Game parlour ka maalik saare computer apne main computer se jod leta: koi bhi site khole to use pata, kyunki kal ko phansega wahi. Waise hi dabba AWS ko event bhej de: is bande ne yahan tak dekha.

Ya ISP pe ek parchi / chhotu job rakh lo jo sab AWS tak sync kar de; ye kabhi bhi, raat ko jab bheed kam ho, ya subah 5 baje.

AWS ke DB me sab update ho gaya (Ravi 27, Arpan 25), to sab jagah sync: Mumbai, Delhi, Jio / Airtel ke dabbe.

*Technical me:* dabba / player playback report AWS ko bhejta (event), wahan parchi update; dabbe pe sirf video.

---

**6. App khola, server pasand wali list banane baitha: 3 saal ka itihaas, karodon logon se milaan, 6 second khaali screen.**

**Arpan:** Ravi hamara 3 saal ka user hai, hamein pata lag gaya. Main roz cafe / parlour jaata hoon game khelne, pelam pel 3-4 saal se: kisi din GTA, kisi din God of War, God Hand, Valhalla, kisi din PUBG PC.

Ek din nahi gaya, doosre din parlour wala bola "kal kahan reh gaye the, tumhara game intezaar karta tha, maine sab chala rakha tha ki kuch na kuch khelega; koi baat nahi, main hamesha tumhare liye taiyaar cheez rakhunga taaki tumhe wait na karna pade".

Pattern hai, pattern yaad kar lo. Ravi ne 3 saal me app me kya kiya: Money Heist kitni baar search kiya, Stranger Things, Breaking Bad, kya-kya dekha; us hisaab se uska pattern hai.

Ravi app kholega to hamein pata hai use kya dikhana hai, kya suggest karna hai: Money Heist dobara, Stranger Things, Breaking Bad se milte-julte shows; India me hai to Dhurandhar, aajkal latest.

Netflix ko pata hai wo kya dekhega, to wo cheez uske app me hi daal do. Ye same to same Twitter wala feed hai jo hum seedha push kar dete, uske aane ka wait nahi karte; app kholega to turant dikhega, ek second ki loading nahi.

Palak jhapakte content saamne hota, jaadu thodi ho raha: feed me pehle se push kar diya, app khola, content saamne.

*Technical me:* "pehle se laga ke rakhta" = list raat ko pehle se banti (batch), "app khole to saamne" = cache se uthao, sochna nahi.

**Maan lo list raat 2 baje bani, Ravi ne 11 baje Dhurandhar khatam ki, 11:05 pe sabse upar wahi.**

**Arpan:** Dekhi hui cheez turant parchi pe likh lo. List kab bhi bane, dikhate waqt parchi milao, lal patti lag jaati. Dobara dikhana galti nahi, Netflix ka dhandha tujhe app pe rokna hai.

*Technical me:* "turant parchi" = abhi ka dekha alag fast store me, "dikhate waqt milao" = pehle se bani list + abhi ki baat mila ke dikhana.

**Maan lo Priya ne pehli baar join kiya, koi itihaas nahi.**

**Arpan:** Parlour wala mere bhai ko nahi jaanta, wo kahega ye hit game hai, khelo. India me hai to India ka garam maal dikhao. Dheere-dheere lat lagegi, pattern banega.

*Technical me:* "garam maal" = naye user ko region ka trending (cold start), "pattern banega" = dekhne se apni list.

**Maan lo Priya Ravi ki behen hai:**

**Arpan:** Netflix ko nahi pata Priya Ravi ki behen hai, wo Ravi ke account se nahi aayi, uska apna alag account hai. Family account hota to alag baat. To Priya ko India ke hot topic dikhao (jaise Dhurandhar).

*Technical me:* pasand account / profile ke hisaab se, rishta nahi pata; naye account pe region ke trending (cold start).

**Isi pe ek aur misaal (round me: library):**

**Arpan:** Ravi 3 saal se hamare paas, uska sab dekha hua pata: pehle Stranger Things, phir Money Heist, phir Breaking Bad. Pattern: chori wali series, sci-fi drama.

Use unhi jaisi suggest karo (Narcos), taaki zyada der Netflix pe rahe.

Library: 100 tarah ki kitab, 50 log roz aate jo 5 tarah ki padhte (drama, thodi horror); main unke liye usi jaisi aur kitab laaunga taaki wo zyada der library me rahein, unhe dekh ke aur log aayein, library famous ho.

Naya user, koi itihaas nahi: us waqt jo famous chal raha wo dikhao, "most watched", "most loved". List pehle se bana ke rakho, Twitter wala hi.

*Technical me:* pasand = itihaas se milti cheez, naya user = trending (cold start), list pehle se (precompute, kadam 6).

&nbsp;

---

**7. Ravi metro me Money Heist dekh raha, 4 sec ka tukda chal raha. Tunnel aayi, net 3 second poora gaya. Tukda khatam, player ne agla maanga, kuch nahi aaya, gola ghoomne laga, video ruk gayi.**

**Arpan:** Main doodh wala hoon. Amul har ghar aake packet nahi deta, mohalle ki dukaan ko deta, log dukaan se lete. Mere paas 10 packet, aa gaye 15 log, 5 laut gaye; 3-4 din aisa chala, nuksaan.

To pehle hi Amul se bolunga 10 ki jagah 20 bhej, 18 bhi aa gaye to sabko mil jaayega, 2 bache to fridge me pade rahenge.

Waise hi Ravi ke liye agla 4 sec ka tukda pehle hi apne paas rakh lo, thode tukde aage ke; peeche se data aata rahega. Amul ne mana kar diya to Mother Dairy se le aaunga: Amul jaisa nahi, par theek, kaam chal jaayega.

Aur Amul ko bolunga raat ko de jaaya kar, 12-1 baje, jab bheed na ho.

*Technical me:* "agla tukda pehle se paas rakh lo" = player buffer (aage ke kuch tukde pehle se; sirf ek tukda aage = tunnel me ruk jaata), "Mother Dairy, kaam chal jaayega" = kam quality ka tukda (ABR, P3 ka Alto), "raat ko de jaaya kar" = raat ko bharai (kadam 2).

**Isi pe ek aur misaal (round me: moti file):**

**Arpan:** Arpan 12ve minute pe dekh raha to Netflix har minute laane nahi jaata; paas ke server pe 30 minute tak rakh lo, phir 40, phir 50, ek saath bada tukda laake serve. Moti file hai, roz uske 2-3 page khol rahe, to file apne paas le aaunga, baar-baar wahan nahi jaunga.

*Technical me:* aage ke tukde pehle se (prefetch + player buffer).

&nbsp;

**Padhai wala hissa (mock se pehle samjha, scene me nahi poocha gaya)**

**P1. Bahar ka CDN (CloudFront / Akamai) kyun nahi, apna dabba kyun?**
Doodh company ki kahani: courier company har litre ka paisa leti hai, aur Netflix ke liye ye "litre" roz ke hazaaron TB hain, bill aasmaan pe.

Netflix ne har mohalle ki dukaan me apna fridge muft me rakh diya: Jio / Airtel jaise ISP ke andar apna dabba.

Teen fayde: paisa (dabba ek baar ka kharcha, per-GB bill nahi), paas (video ISP ke andar se aata, buffering kam), ISP bhi khush (uska bahar jaane wala bandwidth bachta).

Aur aam CDN tab file laata jab koi pehli baar maange, Netflix raat ko jab net khaali hota tab pehle se bhar deta. Doodh wala subah se pehle fridge bhar deta, maangne ka intezaar nahi karta.

*Technical me:* courier = per-GB bill wala CDN (CloudFront / Akamai), mohalle ka fridge = Open Connect (ISP ke andar, free), raat ko pehle se = proactive fill (off-peak).

**P2. Phir AWS kis kaam ka?**
AWS chhoda nahi. Login, browse, pasand wali list, aur play dabane pe kaunsa paas wala dabba video dega, ye sab AWS pe chalta. Sirf video ke bytes Open Connect se aate.
`tu play dabata -> AWS (login, kaunsa episode, kaunsa paas wala dabba) -> ISP ke andar Netflix ka dabba -> video ke tukde`

*Technical me:* AWS = login / browse / recommendation / steering (control plane), Open Connect = sirf video ke bytes.

**P3. Ek video ke kai size: Twitter wala Ferrari / Alto, teen twist ke saath**
Twitter me khud nikala tha: Ferrari highway ke liye, Alto gaon ke liye, phone net dekh ke size chunta, net gira to tasveer thodi dhundhli par chalti rahe. Video (maan lo) 4-4 second ke tukdon me, aage khiskao to player manifest dekh ke seedha us tukde pe (copy ka 51wa page). Netflix me teen twist:
(1) Har film ki apni seedhi. Cartoon (Doraemon) me rang saade, kam hilta, to kam bits me bhi saaf. Action film (Money Heist ka bank wala goli-baari scene) me bahut hilta, zyada bits chahiye. Ek hi niyam sab pe lagao to cartoon pe data barbaad aur action film dhundhli.
(2) Ek size ke bhi kai roop. Purana TV ek tarah ki file samajhta, naya phone doosri tarah ki jo aur chhoti hoti. Play dabate hi app batata "main ye wala samajhta hoon", wahi roop milta.
(3) Ek baar banao, karodon baar chalao. Twitter me video abhi daala abhi chalana tha, jaldi thi. Netflix ke paas studio ki film release se din-hafte pehle aa jaati. Kai machine (AWS pe) tukde baant ke aaraam se saare size aur roop banati (wahi "video ki id ki parchi, 10 log kaam baanto"), phir raat ko dabbon me.
Awaaz aur subtitle alag files: ek hi 1080p ke saath Hindi ya English kuch bhi chuno, video ki 10 copy nahi banti.

*Technical me:* Ferrari / Alto = transcoding (kai size) + ABR, 4 sec tukde + manifest = segments + seek, har film ki apni seedhi = per-title encode ladder, kai roop = codec, ek baar banao = AWS pe batch encoding (parallel workers / consumers), awaaz / subtitle alag = alag audio / subtitle tracks.

**P4. Pasand wali list ke baaki tukde**
Mohalle ka chaiwala tujhe roz dekhta, tere aane se pehle jaanta tu adrak wali kam cheeni maangega.

Netflix tera itihaas leta (kya dekha, kitna dekha, beech me chhoda, kya search kiya) aur tere jaise logon ka bhi ("jinhone Money Heist dekha, unhone Squid Game bhi dekha"). Raat ko badi machines har user ki list bana ke rakh deti.

App khola to list uthao aur thoda abhi ke hisaab se badlo: "abhi raat hai to thriller upar", "tu TV pe hai".

Tasveer bhi teri: ek film ki 5-6 tasveer, tu action zyada dekhta to Money Heist ki goli wali, koi romance dekhta to usi film ki pyaar wali. Film wahi, darwaza alag.
`tera dekhna / chhodna / search -> Kafka -> raat ka batch: har user ki list; app khola -> list uthao (cache) + thoda abhi ka mod`

*Technical me:* chaiwala = offline / batch recommendation (tera + tere jaise logon ka itihaas), thoda abhi ka mod = online halka re-rank, apni tasveer = personalized thumbnail, dekhna -> Kafka = events ka stream batch tak.

&nbsp;

Poora design = [18_netflix.md](18_netflix.md)
