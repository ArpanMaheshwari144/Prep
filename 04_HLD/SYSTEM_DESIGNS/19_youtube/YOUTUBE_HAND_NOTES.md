# YouTube: hand notes (Goa trip ka vlog)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan YouTuber hai. Goa trip ka vlog: 1 ghante ki video, 2 GB (4K me 40 GB tak). Lakhon log dekhenge, viral bhi ho sakta.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
creator  --presigned URL-->  S3 (file seedha locker me, server se nahi)  -->  20-25 worker tukde baant ke har quality
                                                                              (chhoti quality pehle = jaldi live)
dekhne wala  -->  manifest + CDN (Netflix jaisa) · garam video garam jagah, purani sasti jagah, trend pe wapas CDN
view +1  -->  Kafka parchi, chhotu batch me jode · bot ke view gate pe hi rok
nayi video  -->  Twitter wala fan-out, subscribers ko khabar · reels: agle kuch shuru ke tukde pehle se

 1   10 hazaar creator, video server me phansi   ->  seedha locker (S3, presigned URL)
 2   ek machine har quality, 3 ghante            ->  tukde baanto, chhoti quality pehle (parallel encoding)
 3   1 lakh view, ek row ke lock pe line         ->  +1 parchi pe, batch me jodo (Kafka, batch count)
 4   bot / phone se 10 lakh "view"               ->  gate pe rok, baar-baar to ban (rate limit, WAF)
 5   10 saal purani video bhi mehngi jagah       ->  garam garam jagah, thandi sasti (hot / cold storage)
 6   2010 ka gaana aaj trend pe                  ->  thodi der garam + CDN, phir wapas
 7   MrBeast ki video, crore subscriber          ->  fan-out + notification
 8   reels me har swipe pe spinner               ->  agle pehle se (pre-fetch)
 8b  mobile data, reels skip                     ->  net ke hisaab se quality (ABR)
 8c  har reel poori pehle se, 29 sec bekaar      ->  sirf shuru ke tukde pehle se
```

**Arpan (FR / NFR pe, 18-Nov):** nakli view FR nahi, NFR hai (FR 6 hata, NFR 4 me). "Bina buffering" jhootha vaada, Netflix jaisa: net ke hisaab se buffering hogi.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
Pehli line interviewer ko: "Main poora YouTube (sign-up se logout tak) nahi, ye cheezein solve karunga: video upload,
jaldi live, dekhna, view count, nayi video ki khabar, aur reels / shorts ka swipe. Theek lage to aage badhein." Neeche har line ke saath kadam
number, taaki FR / NFR aur design ek hi kahani rahe.

```
FR      1  video upload (badi file, 2-40 GB)                                  -> kadam 1
        2  upload ke baad har quality me bane (144p se 4K)                       -> kadam 2
        3  har phone / net pe dekhna (manifest + CDN, Netflix jaisa)             -> Netflix kadam 1-4 + P3
        4  har video ka view count                                              -> kadam 3
        5  subscribe kiya to nayi video ki khabar (MrBeast)                      -> kadam 7
        6  reels / shorts: swipe karte hi agli chale                            -> kadam 8
        7  roko, wahi se chalao                                                 -> Netflix kadam 5 (yaad account pe)
        8  search + recommendation (pasand ka music / video): recommendation ki algorithm / ML design ka
           hissa nahi (Arpan 8-Nov); pattern kaise banta = kadam 8b; search maange to Elasticsearch   -> kadam 8b

NFR     1  upload kabhi app ko na roke (badi file app server se na guzre)       -> kadam 1
        2  video jaldi live (pehle chhoti quality)                              -> kadam 2
        3  viral pe bhi ginti DB na giraye (count thoda der se chalega)         -> kadam 3
        4  nakli / bot ke view ginti me na aayein                               -> kadam 4
        5  storage sasta (purani video sasti jagah)                             -> kadam 5
        6  purani video achanak trend pe aaye to bhi aaram se chale (garam jagah + CDN);
           net ke hisaab se quality, kamzor net pe thodi buffering hogi hi      -> kadam 6 + Netflix P3
        7  swipe pe spinner nahi, par skip hui reel pe data / battery bekaar na jaaye -> kadam 8, 8c

Numbers lakhon video roz, ek video 2-40 GB -> app server se nahi guzar sakti
        · viral video pe ek minute me lakhon view -> ek row pe +1 nahi chalega
        · saalon ki videos x kai quality -> sab tez storage me nahi rakh sakte
```

Trade-off jo bolne hain: view count thodi der se badhta (DB bachane ke liye) · thandi jagah sasti par laane me der
(isliye trend pe wapas garam) · pehle chhoti quality (4K wale ko thoda intezaar) · reels pehle se zyada laaye to data / battery, kam laaye to spinner.

&nbsp;

**1. Upload dabaya, video hamare app server se hokar storage ja rahi. Usi waqt 10 hazaar creator aur, server ki bandwidth / memory videos me phans gayi, jo sirf search / comment kar rahe the unka app bhi atka.**
Arpan: Badi file apne paas mat rakho, seedha storage me bhejo. Jaise bank me koi bahut moti file le aaya ki locker me rakh lo. Hum badi file locker me kyun rakhenge? Watchman ko bulao: iska bas ek chhota sa hissa (parchi) apne paas rakho, aur file seedha bade storage me bhijwa do. Khatam kahani.
Technical me: "file seedha bade storage me" = pre-signed URL, phone seedha object storage (S3 / GCS) me upload karta, "chhota hissa apne paas" = DB me sirf video ki parchi (metadata), app server pe video nahi.

**2. Video storage me aa gayi. Ab 360p, 720p, 1080p, 4K banana hai. Ek machine poori video ek-ek quality me bana rahi, 3 ghante lag rahe, subscribers pooch rahe "video kab?".**
Arpan: Badi file hai to 20-25 log lagao. File tod do, chhote-chhote hisse, sab apna hissa fatafat banao, jab sab ho jaaye to poori file ek jagah jod do. Upload ho jaaye to storage mujhe ek khabar bhej de ki ho gaya. Phir pehle sabse chhoti, theek-thaak quality bana do. Jaise dulha-dulhan ko apni shaadi ki video jaldi dekhni hai: pehle chhoti quality de do, ek baar dekh ke santusht ho jaayenge, aur peeche hamare log 720p, 4K banate rahenge.
Technical me: "upload ho gaya, khabar bhejo" = storage event, queue, "file tod do, 20-25 log" = segment me kaat ke parallel encoding (har tukda, har quality), "phir jod do" = merge, "pehle chhoti quality" = 360p pehle publish, baaki baad me.

**3. Vlog viral: 1 minute me 1 lakh view. Har view pe DB me ek hi row me views + 1. Ek lakh request ek row ke lock pe line me, DB atka, baaki videos ke like / comment bhi ruke.**
Arpan: Har +1 pe mote register tak bhaagna pagalpan hai. +1 abhi parchi pe likh do. Peeche chhotu rakh lo: wo parchi dekhta rahe aur batch me mote register me jodta rahe: "is batch me itne +1 aaye". Ek chhotu gira to 2-3 aur hain. Ye Kafka jaisa hai: ek stream me aate rahein, chhotu batch me DB me daalein. Aur YouTube pe likhai bahut zyada hai, to Cassandra jaisa DB soch sakte.
Technical me: "parchi pe +1" = view event stream me (Kafka), "chhotu batch me jode" = batch aggregation (memory me gino, kuch second me ek baar likho), count thodi der se badhta = trade-off, "likhai zyada" = Cassandra.
Doosri kahani (17-Nov grill): Har baar DB me update karna pagalpan. Bheed aa rahi hai to Kafka bhi laga sakte, Redis bhi: sabko pehle ek dabbe me rakh lo. Jaise koi har ghante mere paas ek rupaya leke aa raha, to main har ek rupaye ke liye bank nahi bhaagunga; apne paas rakh lunga, dheere-dheere badhta rahe. Din poora hua, to jitni gaddi jama hui, poora batch chupchaap jaake update kar do.
Technical me: "pehle dabbe me" = Redis counter (INCR) / Kafka, "gaddi jama hone pe ek saath" = batch flush to DB.

**4. Ek creator ne script likhi: hazaar phone / bot se har 3 second refresh, ek ghante me 10 lakh "view". Views pe paisa milta, wo trending me upar, asli creators shikayat kar rahe.**
Arpan: Ye fraud hai. Aisa view ginti me gino hi mat. API gateway pe hi rok do, request andar hi mat aane do: ek hi bande / IP se 10 lakh request? Rate limit laga do. Baar-baar kare to WAF me daal do, ya permanent ban. Peeche Kafka ka worker / consumer pattern dekhta rahe: 3-4 baar wahi pattern dikha to daal do. Rate limiter, Kafka, WAF pehle padh chuke hain.
Technical me: "gateway pe rok do" = rate limit per user / IP / video, "WAF me daal do" = WAF ban, "worker / consumer pattern dekhe" = fraud detection worker / consumer (bot / repeat view ginti se pehle filter).

**5. Roz lakhon video, har ek ki 40 GB original + 5 quality ki copies. 10 saal purani videos jo mahine me ek baar koi dekhta, wo bhi mehenge tez storage me. Bill har mahine badh raha.**
Arpan: Storage baant do: garam cheez garam jagah, thandi cheez thandi jagah. Arijit Singh ka naya gaana aaya, YouTube ke paas saalon ka pattern hai, pata hai ye chalega. Uske fan India me hain, to India ke CDN pe rakh do, aur garam storage me. 10-20 saal purana gaana jo koi nahi sunta, thande storage me. Jo ladka roz file lene aata hai, uski file main apne paas rakhta hoon; jo mahine me ek baar aata hai, uski file locker me, wo aaye tab jaake le aaunga.
Technical me: "garam / thanda storage" = hot / cold storage tiering (lifecycle policy: itne din koi nahi dekha to sasti jagah), "pehle se India ke CDN pe" = popular video ko pehle se regional CDN pe.

**6. 2010 ka gaana thandi jagah pada tha. Aaj reels pe trend ho gaya, 1 ghante me 50 lakh log. Thandi jagah se laane me har baar time, video ghoom rahi, log likh rahe "YouTube down hai?".**
Arpan: Aaj wo phat gaya to thodi der ke liye garam storage aur CDN pe rakh lo. Pattern pata hai: ek-do din phatega, roz koi nahi sunega. Jaise Saiyaara ka gaana: India ka hai, Europe wale nahi sunenge, to India ke CDN pe. Kishore Kumar ka gaana 4 log sun rahe, achanak reel se phat gaya, to use bhi 1-2 din CDN pe. Fati jeans jaisa: kabhi nahi pehenta tha, trend aaya to 4-5 din pehni, trend thanda hua to wapas almaari me. Roz ginti dekho: 50 lakh, 16 lakh, 10 lakh, 5 lakh... trend khatam, wapas thande storage me. Purana gaana US ke server pe pada tha, India ka traffic roz US tak ja raha aur badhta hi ja raha: ek copy Mumbai ke server pe rakh do. 10 din peak, phir 11, 12, 15, 18 din ginti girti dikhe to wapas thandi jagah.
Technical me: "phat gaya, garam me lao" = trend detect (view spike, view count wala stream), promote to hot storage + CDN, "ginti giri, wapas almaari" = demote (CDN se hatao, phir cold storage, TTL / lifecycle).

**7. Arpan ka vlog chal gaya, uske subscriber nayi video ka intezaar karte. Par asli bojh tab, jab MrBeast jaisa bada YouTuber nayi video daale. MrBeast ne nayi video daali. Uske crore subscribers ko turant pata chalna chahiye.**
Arpan: Ye wahi subscriber wala fan-out hai jo Twitter me kiya: jisne subscribe kiya hai, sabko notification push kar do, jaise Instagram / Twitter pe follow karte hain to feed me aata hai. YouTube asal me kaise karta hai, unka design public nahi, par fan-out jaisa hi kuch hoga. Ye pehle ho chuka hai.
Technical me: "sab subscribers ko push" = fan-out (Twitter design) + notification system. Ghanti ke liye pull nahi chalta (user ko pata hi nahi kuch aaya), isliye bade creator pe bhi push; crore subscribers ko queue me daal ke workers / consumers thoda-thoda bhejte (Notification design). Home feed me video dikhana alag baat: wahan Twitter wala hybrid (bade creator ka pull) chalta.

**8. Arpan ne Goa vlog se 30 second ke shorts bhi kaat ke daale. Chhoti video ka swipe (YouTube Shorts ho ya Instagram Reels, ek hi baat). Arpan ka dost Ravi Instagram pe reels dekh raha. Har swipe pe app server se agli reel maangta: phone se server, server se CDN, phir chale. Beech me 1-2 second ka spinner, aur reels me 1 second bhi bahut; spinner dikha aur Ravi aage swipe kar gaya.**
Arpan: Instagram ke servers yahan hain nahi, to har reel pe bahar tak nahi jaayega; reel ke saare tukde CDN me laake rakh leta hai. Reel upload hui to 720p, 480p, jitne bhi p hote honge utne me todta hoga, transcoding yahan bhi lagti hogi: reel chhoti hoti hai to pehle chhoti quality bani, phir usse badi, phir usse badi, aur sab CDN ke server pe store. Ravi ki Instagram ki algorithm set hogi ki wo kis-kis ki reel dekhta hai, wahi reels aayengi. Ab koi dekh raha to 5 reel apne paas rakh lo. Ek scene lete hain: ek banda mere paas aaya, use page padhne hain, mere paas 20 page hain. Wo dhadadhad padhta raha. To kya main 20ve page ke khatam hone ka intezaar karunga? Nahi, chhotu ko bolunga "turant agle 20 page utha la". Wo 20 page padh le to agle 20 de diye, phir agle 20. Wo intezaar nahi kar raha, mere paas pehle se saamaan hai use dene ke liye. Bas yahi Instagram karta hai.
Technical me: "agle 20 page pehle se" = pre-fetching (aage ki reels ki list ek saath aati, aur unke tukde pehle se phone pe, agli reel ka player bhi taiyaar, swipe kiya aur chali), "tukde paas me" = CDN, "kai quality" = transcoding.

**Maan lo Ravi metro me mobile data pe hai, 1.5 GB ka pack. Zyadatar reels 1 second me skip karta, par app har baar aage ki 5 reels poori, achhi quality me utaar raha. Shaam 4 baje pack khatam.**
Arpan: Dekho uska net kaisa hai. Net itna badiya nahi chal raha to reels kam quality ki laao, faltu me zyada quality kyun; 240p bahut hai (Instagram me 480p bhi phone pe bahut HD lagti). Net slow to pehle thodi blur dikhegi; net theek chala to saaf. Net ke hisaab se 240p, 480p ya 1080p. Battery ke hisaab se Instagram kya karta hai, wo unki policy hai, hamein nahi pata, us pe faltu tippani nahi kar sakte. Aur Instagram ka main funda engagement hai: kisi reel pe meri engagement 1 second hai to wo reel ab dikhane ka nahi; kisi me 30 second gujaar diye to samajh jaayega ye dekhna chah raha. Library jaisa: 10 log roz aa rahe, unka pattern samajh gaya ki ye log ye padhne aate hain, to wahi kitaabein zyada aur alag-alag variety rakh di.
Technical me: "net ke hisaab se quality" = ABR (adaptive bitrate, pehle blur phir saaf), "1 second wali reel ab nahi" = recommendation ka engagement signal (kitni der dekhi), battery wala hissa public nahi.

**Maan lo Ravi ka net achha, reels 1080p me aa rahi, par 5 me se 4 reels 1 second me skip. App ne har reel poori 30 second ki pehle hi utaar li thi, skip hui reel ke 29 second ka data bekaar.**
Arpan: 10 reel poori laana thoda bewakoofi hai: 10 ki 10 skip kar di to aage 10 reel ke liye phir mehnat. To shuru ke kuch chunks le lo; use achha laga to theek, hamari mehnat bhi bekaar nahi gayi. Copy wala scene: chhotu ko 20 page laane bheja, dekha banda 1-2 page padh ke chala gaya; kal phir aaya, phir 1-2 page. Agle din kaha "chhotu, sirf 3-4 page la, bahut hai". Usne poora padh liya to chhotu ko chupchaap bhej dunga, wo poore le aayega. Par 1-2 page dekh ke kitaab nahi hataunga, kya pata use teesra-chautha page achha lage. Shuru me Instagram ke paas hamara kuch nahi hota, kuch bhi dikhata; 1-2 din pattern dekhega ki kya padhta, kitna padhta, phir laana kam karega. Pre-fetch bina soche nahi, dekh ke laaya hua.
Technical me: "shuru ke kuch chunks" = pre-fetch me sirf pehle kuch second, baaki tab jab reel sach me chale; "kitna laana" = user ke pattern se (kitna skip karta). Trade-off: zyada laaye to data aur battery, kam laaye to spinner. Data saver on ho to pehle se laana band.
**Maan lo pattern kaise banta:** Arpan: Shuru me Instagram ke paas hamara kuch nahi hota, kuch bhi reel dikhata. Roz jaate hain to use pata lag jaata ye Arpan ye zyada dekhta, to feed me wahi. Algorithm set hota: is region (India) ke log ye zyada dekhte, phir India me north ke log ye, south ke ye. Main bhi library me aisa karunga: is region ke log comic zyada padhte, is region ke horror fantasy.
**Maan lo 1-2 reel achhi nahi lagi:** Arpan: 1-2 page pe poori kitaab nahi phaadunga; kya pata teesre-chauthe page pe interesting lage. Thoda-thoda dikhao, pattern samjho. Haan, 7 din nikal gaye aur kisi ne nahi padhi, to us kitaab ko zaroor hata dunga.
Technical me: region / sub-region pe trending (cold start), user signal se personal feed; ek-do skip pe nahi hatao, lambe waqt tak koi engagement nahi to hatao.

&nbsp;

Poora design = [19_youtube.md](19_youtube.md)
