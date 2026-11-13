# YouTube: hand notes (Goa trip ka vlog)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan YouTuber hai. Goa trip ka vlog: 1 ghante ki video, 2 GB (4K me 40 GB tak). Lakhon log dekhenge, viral bhi ho sakta.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
Pehli line interviewer ko: "Main poora YouTube (sign-up se logout tak) nahi, ye cheezein solve karunga: video upload,
jaldi live, dekhna, view count, aur nayi video ki khabar. Theek lage to aage badhein." Neeche har line ke saath kadam
number, taaki FR / NFR aur design ek hi kahani rahe.

```
FR      1  video upload (badi file, 2-40 GB)                                  -> kadam 1
        2  upload ke baad har quality me bane (144p se 4K)                       -> kadam 2
        3  har phone / net pe dekhna (manifest + CDN, Netflix jaisa)             -> Netflix kadam 1-3
        4  har video ka view count                                              -> kadam 3
        5  subscribe kiya to nayi video ki khabar (MrBeast)                      -> kadam 7
        6  nakli view ginti me na aayein                                       -> kadam 4

NFR     1  upload kabhi app ko na roke (badi file app server se na guzre)       -> kadam 1
        2  video jaldi live (pehle chhoti quality)                              -> kadam 2
        3  viral pe bhi ginti DB na giraye (count thoda der se chalega)         -> kadam 3
        4  fake view na gine                                                    -> kadam 4
        5  storage sasta (purani video sasti jagah)                             -> kadam 5
        6  achanak trend pe bhi bina buffering                                  -> kadam 6

Numbers lakhon video roz, ek video 2-40 GB -> app server se nahi guzar sakti
        · viral video pe ek minute me lakhon view -> ek row pe +1 nahi chalega
        · saalon ki videos x kai quality -> sab tez storage me nahi rakh sakte
```

Trade-off jo bolne hain: view count thodi der se badhta (DB bachane ke liye) · thandi jagah sasti par laane me der
(isliye trend pe wapas garam) · pehle chhoti quality (4K wale ko thoda intezaar).

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

**4. Ek creator ne script likhi: hazaar phone / bot se har 3 second refresh, ek ghante me 10 lakh "view". Views pe paisa milta, wo trending me upar, asli creators shikayat kar rahe.**
Arpan: Ye fraud hai. Aisa view ginti me gino hi mat. API gateway pe hi rok do, request andar hi mat aane do: ek hi bande / IP se 10 lakh request? Rate limit laga do. Baar-baar kare to WAF me daal do, ya permanent ban. Peeche Kafka ka worker pattern dekhta rahe: 3-4 baar wahi pattern dikha to daal do. Rate limiter, Kafka, WAF pehle padh chuke hain.
Technical me: "gateway pe rok do" = rate limit per user / IP / video, "WAF me daal do" = WAF ban, "worker pattern dekhe" = fraud detection worker (bot / repeat view ginti se pehle filter).

**5. Roz lakhon video, har ek ki 40 GB original + 5 quality ki copies. 10 saal purani videos jo mahine me ek baar koi dekhta, wo bhi mehenge tez storage me. Bill har mahine badh raha.**
Arpan: Storage baant do: garam cheez garam jagah, thandi cheez thandi jagah. Arijit Singh ka naya gaana aaya, YouTube ke paas saalon ka pattern hai, pata hai ye chalega. Uske fan India me hain, to India ke CDN pe rakh do, aur garam storage me. 10-20 saal purana gaana jo koi nahi sunta, thande storage me. Jo ladka roz file lene aata hai, uski file main apne paas rakhta hoon; jo mahine me ek baar aata hai, uski file locker me, wo aaye tab jaake le aaunga.
Technical me: "garam / thanda storage" = hot / cold storage tiering (lifecycle policy: itne din koi nahi dekha to sasti jagah), "pehle se India ke CDN pe" = popular video ko pehle se regional CDN pe.

**6. 2010 ka gaana thandi jagah pada tha. Aaj reels pe trend ho gaya, 1 ghante me 50 lakh log. Thandi jagah se laane me har baar time, video ghoom rahi, log likh rahe "YouTube down hai?".**
Arpan: Aaj wo phat gaya to thodi der ke liye garam storage aur CDN pe rakh lo. Pattern pata hai: ek-do din phatega, roz koi nahi sunega. Jaise Saiyaara ka gaana: India ka hai, Europe wale nahi sunenge, to India ke CDN pe. Kishore Kumar ka gaana 4 log sun rahe, achanak reel se phat gaya, to use bhi 1-2 din CDN pe. Fati jeans jaisa: kabhi nahi pehenta tha, trend aaya to 4-5 din pehni, trend thanda hua to wapas almaari me. Roz ginti dekho: 50 lakh, 16 lakh, 10 lakh, 5 lakh... trend khatam, wapas thande storage me.
Technical me: "phat gaya, garam me lao" = trend detect (view spike, view count wala stream), promote to hot storage + CDN, "ginti giri, wapas almaari" = demote (CDN se hatao, phir cold storage, TTL / lifecycle).

**7. MrBeast ne nayi video daali. Uske crore subscribers ko turant pata chalna chahiye.**
Arpan: Ye wahi subscriber wala fan-out hai jo Twitter me kiya: jisne subscribe kiya hai, sabko notification push kar do, jaise Instagram / Twitter pe follow karte hain to feed me aata hai. YouTube asal me kaise karta hai, unka design public nahi, par fan-out jaisa hi kuch hoga. Ye pehle ho chuka hai.
Technical me: "sab subscribers ko push" = fan-out (Twitter design) + notification system. Ghanti ke liye pull nahi chalta (user ko pata hi nahi kuch aaya), isliye bade creator pe bhi push; crore subscribers ko queue me daal ke workers thoda-thoda bhejte (Notification design). Home feed me video dikhana alag baat: wahan Twitter wala hybrid (bade creator ka pull) chalta.

&nbsp;

Poora design = [19_youtube.md](19_youtube.md)
