# Netflix: hand notes (Arpan, Ravi aur Money Heist ka naya season)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Maan lo Money Heist ka naya season Friday dopahar 12:30 baje poori duniya me ek saath. Arpan, Ravi aur India ke 2 crore log 12:30 pe ek saath play dabate hain, saath me US, Spain, Brazil bhi.

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
Arpan: "Buffering nahi" NFR galat hai. User ka net bahut mandda hai, 144p pe bhi nahi chal raha, to buffering hogi hi; quality 180p se 720p ya 360p badalti hai tab bhi hoti hai, Netflix me buffering hoti hai. Net ke hisaab se hogi. FR / NFR ki bhasha wahi jo neeche design me likhi, alag shabd ("tere liye" jaisa) nahi.
Technical me: NFR = jaldi shuru (low start-up time) + adaptive bitrate (ABR), buffering zero ka vaada nahi.

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
Numbers India me 2 crore log ek saath, saath me baaki duniya -> ek jagah se sabko video nahi bhej sakte
        · har film kai size me -> poora maal kisi ek dabbe me nahi samaata
```

&nbsp;

**1. Saari episode US ke store me. 12:30 pe karodon log wahi file maang rahe, har tukda samundar paar se, Ravi ke TV pe buffering, data pahunchane wali company ka bill aasmaan pe.**
Arpan: Main roz wahi file lene doosre shehar jaata hoon, bus me tel phoonk ke. File ghar nahi la sakta, par apne shehar me to la sakta hoon. CA doosre shehar me hai, to mera dost jo roz aata-jaata hai, wo file mere shehar le aaye. Main chupchaap jaao, sign karo, wapas aao. Bekaar me baar-baar shehar kyun.
Technical me: "file apne shehar me" = ISP ke andar Netflix ka dabba (Open Connect), "ghar nahi la sakta" = poora video phone pe nahi rakhte, "dost roz aata-jaata" = dabba US ke store se bharta rehta.

**2. Dabba lag gaya, par season 12:30 pe aaya, 12:29 tak dabba khaali. 12:30 pe 2 crore ek saath, dabba sabko "laata hoon" bolta, saari request phir samundar paar.**
Arpan: Jab pata hai log kis liye aa rahe, to wo cheez pehle se laa ke rakh lo. Pen drive me Money Heist rakh liya, ab sabko baant raha: le tu bhi le, tu bhi le, buffering hi nahi. Aur ye cheez ban gayi hai, ab badlegi nahi, isliye rakhne me koi dikkat nahi.
Technical me: "pehle se laa ke rakho" = release se pehle raat ko dabba bharna (proactive fill), "badlegi nahi" = video static hai, isliye CDN pe rakhna safe.

**3. Mumbai ke dabbe ki jagah seemit, Netflix ka saara maal (har film x kai size) nahi samaata.**
Arpan: Ek dabba nahi, kai dabbe le lo, jaise maal ek truck me na aaye to 3-4 truck. Par dabbe ki bhi seema hai, isliye dabbe me wahi daalo jiska craze hai. Garam cheez paas rakho, thandi nahi. Purana season koi aaj bhi dekhe to bade dabbe se mangwa lo, thodi der chhote dabbe me rakh lo, garmaahat khatam to wapas.
Technical me: "garam paas" = popularity se kya rakhna (region-wise), "bade dabbe se mangwa lo" = upar wala bada store / tier, "garmaahat khatam to wapas" = thanda maal dabbe se nikalna.

**4. Mumbai me 20 dabbe, kuch me Money Heist hai kuch me nahi, ek ki saans phool rahi. Ravi ke TV ko kaise pata kis dabbe se maange?**
Arpan: Akela bhaar kyun uthaun, 10 dost bulao, sabko dabba do, nikaal-nikaal ke do. Garam season ko zyada se zyada dabbon me rakho. Request hamare server pe aati hai, use pata hai kis dabbe me kya hai, wo mod dega. Hospital ka guard: wheelchair wale is line, mahila us line, buzurg us line.
Technical me: "zyada dabbon me" = hot content ki kai copy, "server mod dega" = play pe server batata kaunse dabbe se lo (steering), "guard" = load + maal dekh ke sahi dabba.

**Maan lo aadhe episode pe dabba 3 band ya dheema.**
Arpan: Saans phoolne wali baat, traffic baant do, ek dabba nahi. Money Heist ka garam season chal raha, pelam pel log dekh rahe, to use 20 dabbon me baant do. Ek dabbe me rakha aur dhadadhad log aaye ja rahe, haath badhata hoon nikaalta hoon, haath dukh gaya; to main apne 10 dost ko phone karunga: aao, ye dabbe lo, isme Money Heist hai, nikaal ke do. Ab akela bhaad nahi uthana padta. Ravi ke TV ko kaise pata 20 me se kis dabbe se maange? Hamein to pata hai Money Heist kis dabbe me hai, server use route kar dega.
Technical me (TV ki taraf se): ek dabba dheema / band to TV ke paas 2-3 dabbon ke naam, doosre se le (client failover).
Technical me: "2-3 dabbon ke naam" = server player ko kai CDN dabbon ki list deta, ek atka to player khud agle pe (client side failover).

**5. Ravi ne TV pe 27 minute dekha, band kiya. Subah metro me phone pe khola, 27:00 se chahiye.**
Arpan: Ravi ka account hai, Netflix account se yaad rakhta. Jaise kisi ke 10,000 rakhe, parchi pe uska naam likha, bus stand pe mila to pehchaan liya. TV, phone, kahin bhi khole, yaad wahi. Parchi DB me.
Technical me: "account se yaad" = watch position user ke account pe server me, device me nahi, "parchi DB me" = persistent store.

**Maan lo TV ka net gaya, TV ne "27:00" apne paas rakh liya. Subah phone pe 37 tak dekha, shaam ko TV on hote hi purana 27 pahuncha aur 37 ke upar likh gaya.**
Arpan: Netflix phone, laptop, TV sab me sync rehta, alag nahi ho sakta. Main Netflix ka server hoon: tukde main hi bhej raha, to 27 ke baad agla tukda nahi maanga matlab 25-27 ke aas-paas band kiya. Phir usne phone pe 27 se 37 dekh liya, naya point 37, parchi pe 37. Shaam ko TV khula aur purana 27 aaya: nahi, isne to 37 tak dekh liya, 27 purana hai, discard; 37 se chalao. Account ek hi hai, kahin se bhi login karo wahi dikhe; TV khulte hi net se account ka sach padhta hai, apna purana nahi thopta. Naya 37 aaya to purana 27 kaat ke 37 likh do.
Technical me: "naya point jeete, purana 27 discard" = har position ke saath uska waqt (timestamp), baad wala jeete (last-write-wins, event time se, aane ke waqt se nahi), "main Netflix hoon, tukde main hi bhej raha" = ISP wala dabba bhi Netflix ka (Open Connect), agla tukda nahi maanga to andaza 25-27 ke aas-paas.

**Maan lo karodon TV har kuch second "26:50 pe hoon" bhej rahe, DB ki saans phool gayi.**
Arpan: Race me har kadam thodi likhte, jab ruk jaaye tab likho. Abhi parchi paas rakho, 27 pe ruk gaya to register me daal do.
Technical me: "parchi paas" = position pehle fast store me, "ruk gaya to register" = DB me sirf aakhri / kabhi-kabhi likhna (write kam).

**Maan lo tukde ISP ke dabbe se aa rahe, AWS ke server ko kaise pata ki ruk gaya?**
Arpan: Do log baat kar rahe, ek mujhse baat nahi karta. Maine dost ko bola, jo baat hui sankshep me mujhe bata diya kar. Jis din kuch nahi bataya, matlab aaj aage nahi badha.
Technical me: "dost sankshep me bataye" = dabba / player playback ki report server ko bhejta.

**6. App khola, server pasand wali list banane baitha: 3 saal ka itihaas, karodon logon se milaan, 6 second khaali screen.**
Arpan: Ravi hamara 3 saal ka user hai, hamein pata lag gaya. Main roz cafe / parlour jaata hoon game khelne, pelam pel 3-4 saal se: kisi din GTA, kisi din God of War, God Hand, Valhalla, kisi din PUBG PC. Ek din nahi gaya, doosre din parlour wala bola "kal kahan reh gaye the, tumhara game intezaar karta tha, maine sab chala rakha tha ki kuch na kuch khelega; koi baat nahi, main hamesha tumhare liye taiyaar cheez rakhunga taaki tumhe wait na karna pade". Pattern hai, pattern yaad kar lo. Ravi ne 3 saal me app me kya kiya: Money Heist kitni baar search kiya, Stranger Things, Breaking Bad, kya-kya dekha; us hisaab se uska pattern hai. Ravi app kholega to hamein pata hai use kya dikhana hai, kya suggest karna hai: Money Heist dobara, Stranger Things, Breaking Bad se milte-julte shows; India me hai to Dhurandhar, aajkal latest. Netflix ko pata hai wo kya dekhega, to wo cheez uske app me hi daal do. Ye same to same Twitter wala feed hai jo hum seedha push kar dete, uske aane ka wait nahi karte; app kholega to turant dikhega, ek second ki loading nahi. Palak jhapakte content saamne hota, jaadu thodi ho raha: feed me pehle se push kar diya, app khola, content saamne.
Technical me: "pehle se laga ke rakhta" = list raat ko pehle se banti (batch), "app khole to saamne" = cache se uthao, sochna nahi.

**Maan lo list raat 2 baje bani, Ravi ne 11 baje Dhurandhar khatam ki, 11:05 pe sabse upar wahi.**
Arpan: Dekhi hui cheez turant parchi pe likh lo. List kab bhi bane, dikhate waqt parchi milao, lal patti lag jaati. Dobara dikhana galti nahi, Netflix ka dhandha tujhe app pe rokna hai.
Technical me: "turant parchi" = abhi ka dekha alag fast store me, "dikhate waqt milao" = pehle se bani list + abhi ki baat mila ke dikhana.

**Maan lo Priya ne pehli baar join kiya, koi itihaas nahi.**
Arpan: Parlour wala mere bhai ko nahi jaanta, wo kahega ye hit game hai, khelo. India me hai to India ka garam maal dikhao. Dheere-dheere lat lagegi, pattern banega.
Technical me: "garam maal" = naye user ko region ka trending (cold start), "pattern banega" = dekhne se apni list.

&nbsp;

**7. Ravi metro me Money Heist dekh raha, 4 sec ka tukda chal raha. Tunnel aayi, net 3 second poora gaya. Tukda khatam, player ne agla maanga, kuch nahi aaya, gola ghoomne laga, video ruk gayi.**
Arpan: Main doodh wala hoon. Amul har ghar aake packet nahi deta, mohalle ki dukaan ko deta, log dukaan se lete. Mere paas 10 packet, aa gaye 15 log, 5 laut gaye; 3-4 din aisa chala, nuksaan. To pehle hi Amul se bolunga 10 ki jagah 20 bhej, 18 bhi aa gaye to sabko mil jaayega, 2 bache to fridge me pade rahenge. Waise hi Ravi ke liye agla 4 sec ka tukda pehle hi apne paas rakh lo, thode tukde aage ke; peeche se data aata rahega. Amul ne mana kar diya to Mother Dairy se le aaunga: Amul jaisa nahi, par theek, kaam chal jaayega. Aur Amul ko bolunga raat ko de jaaya kar, 12-1 baje, jab bheed na ho.
Technical me: "agla tukda pehle se paas rakh lo" = player buffer (aage ke kuch tukde pehle se; sirf ek tukda aage = tunnel me ruk jaata), "Mother Dairy, kaam chal jaayega" = kam quality ka tukda (ABR, P3 ka Alto), "raat ko de jaaya kar" = raat ko bharai (kadam 2).

&nbsp;

**Padhai wala hissa (mock se pehle samjha, scene me nahi poocha gaya)**

**P1. Bahar ka CDN (CloudFront / Akamai) kyun nahi, apna dabba kyun?**
Doodh company ki kahani: courier company har litre ka paisa leti hai, aur Netflix ke liye ye "litre" roz ke hazaaron TB hain, bill aasmaan pe. Netflix ne har mohalle ki dukaan me apna fridge muft me rakh diya: Jio / Airtel jaise ISP ke andar apna dabba. Teen fayde: paisa (dabba ek baar ka kharcha, per-GB bill nahi), paas (video ISP ke andar se aata, buffering kam), ISP bhi khush (uska bahar jaane wala bandwidth bachta). Aur aam CDN tab file laata jab koi pehli baar maange, Netflix raat ko jab net khaali hota tab pehle se bhar deta. Doodh wala subah se pehle fridge bhar deta, maangne ka intezaar nahi karta.
Technical me: courier = per-GB bill wala CDN (CloudFront / Akamai), mohalle ka fridge = Open Connect (ISP ke andar, free), raat ko pehle se = proactive fill (off-peak).

**P2. Phir AWS kis kaam ka?**
AWS chhoda nahi. Login, browse, pasand wali list, aur play dabane pe kaunsa paas wala dabba video dega, ye sab AWS pe chalta. Sirf video ke bytes Open Connect se aate.
`tu play dabata -> AWS (login, kaunsa episode, kaunsa paas wala dabba) -> ISP ke andar Netflix ka dabba -> video ke tukde`
Technical me: AWS = login / browse / recommendation / steering (control plane), Open Connect = sirf video ke bytes.

**P3. Ek video ke kai size: Twitter wala Ferrari / Alto, teen twist ke saath**
Twitter me khud nikala tha: Ferrari highway ke liye, Alto gaon ke liye, phone net dekh ke size chunta, net gira to tasveer thodi dhundhli par chalti rahe. Video (maan lo) 4-4 second ke tukdon me, aage khiskao to player manifest dekh ke seedha us tukde pe (copy ka 51wa page). Netflix me teen twist:
(1) Har film ki apni seedhi. Cartoon (Doraemon) me rang saade, kam hilta, to kam bits me bhi saaf. Action film (Money Heist ka bank wala goli-baari scene) me bahut hilta, zyada bits chahiye. Ek hi niyam sab pe lagao to cartoon pe data barbaad aur action film dhundhli.
(2) Ek size ke bhi kai roop. Purana TV ek tarah ki file samajhta, naya phone doosri tarah ki jo aur chhoti hoti. Play dabate hi app batata "main ye wala samajhta hoon", wahi roop milta.
(3) Ek baar banao, karodon baar chalao. Twitter me video abhi daala abhi chalana tha, jaldi thi. Netflix ke paas studio ki film release se din-hafte pehle aa jaati. Kai machine (AWS pe) tukde baant ke aaraam se saare size aur roop banati (wahi "video ki id ki parchi, 10 log kaam baanto"), phir raat ko dabbon me.
Awaaz aur subtitle alag files: ek hi 1080p ke saath Hindi ya English kuch bhi chuno, video ki 10 copy nahi banti.
Technical me: Ferrari / Alto = transcoding (kai size) + ABR, 4 sec tukde + manifest = segments + seek, har film ki apni seedhi = per-title encode ladder, kai roop = codec, ek baar banao = AWS pe batch encoding (parallel workers / consumers), awaaz / subtitle alag = alag audio / subtitle tracks.

**P4. Pasand wali list ke baaki tukde**
Mohalle ka chaiwala tujhe roz dekhta, tere aane se pehle jaanta tu adrak wali kam cheeni maangega. Netflix tera itihaas leta (kya dekha, kitna dekha, beech me chhoda, kya search kiya) aur tere jaise logon ka bhi ("jinhone Money Heist dekha, unhone Squid Game bhi dekha"). Raat ko badi machines har user ki list bana ke rakh deti. App khola to list uthao aur thoda abhi ke hisaab se badlo: "abhi raat hai to thriller upar", "tu TV pe hai". Tasveer bhi teri: ek film ki 5-6 tasveer, tu action zyada dekhta to Money Heist ki goli wali, koi romance dekhta to usi film ki pyaar wali. Film wahi, darwaza alag.
`tera dekhna / chhodna / search -> Kafka -> raat ka batch: har user ki list; app khola -> list uthao (cache) + thoda abhi ka mod`
Technical me: chaiwala = offline / batch recommendation (tera + tere jaise logon ka itihaas), thoda abhi ka mod = online halka re-rank, apni tasveer = personalized thumbnail, dekhna -> Kafka = events ka stream batch tak.

&nbsp;

Poora design = [18_netflix.md](18_netflix.md)
