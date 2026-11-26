# PUBG: hand notes (Arpan aur Claude ka squad, Pochinki drop)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan aur Claude ek squad me. Plane se Pochinki me utre, Claude ke haath me shotgun. Raat 9 baje India me lakhon log khel rahe, ek match me 100 log, 8 km x 8 km ka Erangel. Uber se aaya: map pe kaun kahan, aas-paas ka gola, gudda, sab wahi.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Phone  -->  khula taar (WebSocket / UDP)  -->  LB  -->  India ka game server (ek match, 100 log)
                                                   |
             match me jodna:                       |   India wala India server pe, squad fill
             khel ke beech:                        |   sirf aas-paas ki khabar
                                                   |   apni chaal phone pe turant, faisla server ka
                                                   |   goli + chaal server jaanche (rewind karke)
             match khatam:                         |   parchi (game server / Redis) -> line (Kafka) -> Cassandra
                                                   v
             user ki profile:  Postgres (seedha SQL)

 1   America ka ladka Mumbai match me, ping 300    ->  America wala America server pe; akela 3 wale squad me (region matchmaking, squad fill)
 2   100 logon ko 99-99 ki khabar, phone garam     ->  khula taar + sirf aas-paas walon ko (WebSocket / UDP, area of interest)
 2b  har kadam pe 120 ms, kichad me chalna         ->  player turant apni screen pe chale, server peeche hisaab kare (client-side prediction)
 3   hacker ka phone bole "goli lagi"              ->  server line kheenche, deewar, gudda (hit validation, hitbox)
 3b  phone pe 3 kadam aage, server bole ruk gaya   ->  server ki maano, 3 kadam peeche; goli ke pal tak rewind (reconciliation, lag compensation)
 4   1 second me 500 meter                         ->  Koramangala se Whitefield namumkin, phenk do, ban (movement validation)
 5   chicken dinner pe lakhon likhai, DB ghuta     ->  match me parchi, end pe line + copy, dheere DB (batch, Kafka, Cassandra)
```

**FR / NFR kahan se:** hawa se nahi aate. Upar ki dikkat chain dekh ke nikale; har FR / NFR line ke aage likha hai wo kis kadam ki dikkat se aayi.

**Arpan:** FR badhiya. Start dabane pe match me jodna, PUBG karta bhi hai. Match me aas-paas ke log, goli, gaadi chalegi, noise aayegi. Goli lagi to mara hua dikhao; sar pe lagi to headshot, kahin bhi lagi to dead dikhao.

NFR me ping kam aayega. Phone garam na ho, ye NFR me nahi aayega: ye physical hardware ki dikkat hai, hatao. Hacker na jeete theek, match khatam hone pe DB theek.

Har kadam pe 120 ms ka kichad: client-side prediction NFR me daal sakte. Phone pe 3 kadam aage, server bole ruk gaya, goli ke pal tak peeche: ye bhi NFR me aa sakta. Chicken dinner NFR me nahi, wo PUBG ka khaas hai. 1 second me 500 meter namumkin, wo hacker wale me aa gaya.

*Technical me:* "button dabate hi chale" = low input latency (client-side prediction), "goli ke pal tak peeche" = fair hit registration (lag compensation), "faisla server ka" = server authoritative.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
Pehli line interviewer ko: "Main poora PUBG (skins, shop, login) nahi, ye cheezein solve karunga: match me jodna,
match ke andar khel, goli aur chaal ka faisla, aur match ke baad ka hisaab. Design India ke servers tak." Neeche har
line ke saath kadam number.

```
FR      1  "Start" dabao to 100 logon ke match me jodo, 4-4 (ya 2-3) ke squad   -> kadam 1
        2  match me aas-paas ke log, goli, gaadi, awaaz dikhe / sunai de        -> kadam 2
        3  goli lagi ya nahi; sar pe lagi to headshot, kahin bhi lagi to dead   -> kadam 3
        4  match khatam: kills, damage, rank points save                        -> kadam 5

NFR     1  ping kam (India wala India ke server pe)                             -> kadam 1
        2  button dabate hi player chale, kichad jaisa na lage                  -> kadam 2b
        3  hacker na jeete: faisla server ka                                    -> kadam 3, 4
        4  goli ka faisla insaaf se: goli ke pal jo saamne tha use lagi         -> kadam 3b
        5  match khatam hone pe DB na ghute, agla match turant                  -> kadam 5

Numbers raat 9 baje lakhon log · ek match 100 log, 8 km x 8 km
        · server second me (maan lo) 20 baar sabki jagah -> 100 x 99 khabar har baar, sabko sab nahi bhej sakte
        · raat 10 baje hazaron match ek saath khatam -> lakhon likhai ek pal me
```

Trade-off jo bolne hain: door ki khabar nahi aati (par door ka dikhna hi nahi chahiye) · server har goli check karta
(server pe kaam zyada, par hacker ki nahi chalti) · rank points thodi der se dikhte (DB bachane ke liye).

**Isi pe ek aur raasta (round me: FR / NFR, numbers, dhaancha):**
```
FR       1  4-4 ka squad bane, log pelam pel khelein (2-3 ka squad, ya random squad me bhi)   -> kadam 1
         2  India ke log India ke server pe, China ke China pe, Australia ke Australia pe     -> kadam 1
         3  "Start" dabate hi match chale                                                     -> kadam 1
         4  gaadi me baithe to gaadi ki awaaz, goli chali to goli ki, par sirf aas-paas ki    -> kadam 2
         5  goli maari to wo mare; headshot maara to dikhe "you killed this enemy by headshot" -> kadam 3

NFR      1  ping kam                                                                          -> kadam 1
         2  guddi chalao to guddi chalta dikhe, gaadi chalao to gaadi; lag kam                -> kadam 2b
         NFR nahi:  "server ye kare, wo kare"     (peeche ka scene hai)
                    "phone kam MB / GB me chale"  (phone ki jitni memory, utni lega hi)

Numbers  kul 1 crore user, roz ~5 million khelte (5 million sweet spot; roz 1 crore bahut zyada)

Dhaancha phone --khula taar (WebSocket)--> LB --> kaafi saare game server
         client main hai: guddi chalaya, gaadi chalayi, goli maari; server decide kare kisne kya kiya
         Cassandra  = likhai bahut (har goli, har kadam ke coordinate, duniya bhar se)      -> kadam 5
         Postgres   = user ki profile (seedha SQL)
         Redis      = kuch rakhna pade to (match ke coordinate ki parchi)                  -> kadam 5
         CDN nahi   = design me koi aapatti nahi kar raha
```

*Technical me:* "dono taraf khula taar" = WebSocket, "likhai bahut" = write-heavy DB (Cassandra), "profile" = relational DB (Postgres), "kuch rakhna pade" = cache (Redis).

**API (round me):**

**Arpan (API):** Zyadatar POST hongi, kyunki client se server ko jaata hai: match shuru, guddi chala, goli chali, headshot laga, kaun mara, sab coordinate ke saath; server unse line banata. Server se jo aata (ping), wo server khud bhejta. Match khatam (winner winner chicken dinner): GET match ke result.
```
API     POST /matchmaking          -> "Start" dabaya, 100 ke match me jodo, squad bhar do       (FR 1, kadam 1)
        match ke andar REST nahi, khule taar (WebSocket / UDP) pe chhote message:
             phone -> server  chaal {kahan, kis disha}  -> server line banaye, jagah tay      (FR 2, kadam 2b, 4)
             phone -> server  goli  {kahan se, disha}   -> server line kheench ke jaanche     (FR 3, kadam 3, 3b)
             server -> phone  aas-paas ki khabar, kaun mara, headshot (server khud push)      (FR 2, 3, kadam 2)
        GET  /matches/{id}/result  -> chicken dinner: kills + aakhri bachi team               (FR 4, kadam 5)
```

&nbsp;

---

**1. Raat 9 baje 1 lakh log ek saath "Start" dabate hain. Ek match me 100 chahiye, Arpan ke match me abhi 37. Tabhi America se ek ladka aaya, Mumbai server tak ping 300 ms, wo bhi isi match me jud gaya.**

**Arpan:** America wala ladka America ke server pe jaayega, uska yahan matlab nahi. PUBG me aisa hota bhi nahi ki bahar se log aayein; online bhi kholo to India ke log hi judenge. Design India tak, India ke servers pe.

PUBG me log hamesha 4-4 ke squad me baithte hain: tere 4 dost hain to tere 4 jud jaayenge. Akela hai aur match chahiye to kisi random squad me daal do jahan 3 log pehle se hain.

4 zaroori bhi nahi, 2 ya 3 ka squad bhi match shuru kar sakta hai. (Raat 3 baje 37 pe atka match: ye matchmaking ki baat hai, main design ye nahi, aage badho.)

*Technical me:* "America wala America ke server pe" = region-based matchmaking (har desh ka apna server, ping kam), "akele ko 3 wale squad me" = squad fill, "2-3 ka squad bhi chalega" = flexible squad size.

---

**2. Match shuru. Server har second (maan lo) 20 baar sabki jagah sabko bhej raha: 100 logon ko 99-99 ki khabar. Arpan Pochinki ke ghar me chhupa, 6 km door Military Base me 30 log lad rahe, unki har goli, har kadam Arpan ke phone pe. Phone garam, data jal raha.**

**Arpan:** Ek khula taar rakh lo, phone mangwa nahi raha, khabar apne aap aa rahi. Par 6 km door ki awaaz PUBG me aati hi nahi, mere aas-paas ki aayegi.

Goli chali to ek event bana; server dekhe kaun-kaun us jagah ke aas-paas hai, sirf unko bhejo. Saamne wali building me goli chali to mujhe sunai de, Military Base wali nahi.

Ye Uber jaisa hai: hamein aas-paas ka dikhana hai, Arpan ke aas-paas kya ho raha, wahi Arpan ke kaano me jaana chahiye. Map pe bhi aas-paas ke log dikhenge, door ke nahi.

*Technical me:* "khula taar" = WebSocket / UDP connection (push), "sirf aas-paas walon ko event" = area of interest / interest management (map ko dabbon me baanto, Uber ke geohash jaisa, har player ko sirf aas-paas ke dabbon ki khabar).

**Maan lo pooche aas-paas kitna:**

**Arpan:** Ghar baithe 40-50 km door kya ho raha, jaanna zaroori nahi. PUBG me 5 km bhi bahut doori hai. Aas-paas goli chali, grenade phata, gaadi ja rahi, loot, bande aaye, wahi dikhao: mushkil se 500 se 800 meter, bahut hua to 1 km, isse zyada nahi.

*Technical me:* area of interest radius ~500 m - 1 km.

**Isi pe ek aur raasta (round me: 150-200 meter):** Poore map se awaaz nahi aa sakti, awaazon ka bombardment ho jaayega, phone 2 minute me garam. Arpan ko uske aas-paas 150 se 200 meter ke beech ki awaaz aani chahiye; 1 km bahut bada area ho jaayega.

---

**2b. Arpan ka phone Mumbai server se 60 ms door, jaana-aana 120 ms. Arpan "aage chal" dabata, phone server ko batata, server "haan" bole tab player ek kadam chalta. Har kadam pe 120 ms ki der, jaise kichad me chal raha ho.**

**Arpan:** Hum is cheez ke liye server pe rely kar rahe, aur server door bhi ho sakta hai. To client side hi kar lo: player ko chala do, apni screen pe dikha do gudda chal raha, jaise Zomato me dikhate hain.

Server peeche-peeche andaza lagata rahega ki wo kahan se kahan ja raha. Seedhi line me koi dikkat nahi.

Player idhar se udhar ja raha to client peeche ek chhoti line bana de; server thoda lag me chal raha ho to bhi us line se pata lag jaayega ki ye yahan ghoom raha, iski aakhri jagah ye hogi.

Exact nahi keh sakte, kabhi lag ho jaata, par server aaram se andaza laga leta. Player ka button smooth, server apni location nikaal lega; na server ki dikkat, na uski.

*Technical me:* "player ko turant apni screen pe chala do, server ka intezaar nahi" = client-side prediction, "peeche chhoti line, server usse jagah pakde" = phone apni chaal (input) server ko bhejta rehta, server apna hisaab chalata (server authoritative).

**Isi pe ek aur misaal (round me: server ko guddi nahi dikhta):**
- Screen pe lagta guddi bhaag raha, par server ko guddi-puddi nahi dikhta. Wo lines aur ping dekhta: kahan se ping aaya, uske hisaab se react karta.
- Jaise Zomato me bike chalti dikhti hai, asal me bike thodi chal rahi, animation hai; server number se, ping se baat karta.
- Client guddi chalata hai to peeche ek chhoti line chalti, jo na client ko dikhti na kisi aur ko, sirf server dekhta.
- Ping me 150-200 ms lagte, to server ke hisaab se tu yahan khada, asal me thoda aage. Mudne pe Zomato ke driver jaisa: server andaza lagata, kabhi seedha nikal ke wapas aata.

---

**3. Ek hacker ka phone server ko bolta "meri goli Arpan ko lagi", jabki goli deewar pe lagi ya Arpan se 5-6 meter door. Server ne phone ki baat maan li, Arpan mar gaya.**

**Arpan:** Aisa to wo sabko maar dega, wahi king ban jaayega. Faisla main (server) karunga ki goli kisko, kab, kahan lagi. Goli kahan se chali, beech me koi deewar to nahi aayi?

Arpan 5 minute se ek hi jagah khada tha, saanp ban ke chhupa. Goli chalne se pehle, chalne ke baad aur lagne tak Arpan wahin tha, aur goli uske 5-6 meter door lagi; thoda idhar-udhar bhi karun to bhi nahi lagti.

To Arpan nahi marega. Aur maan lo maine (baagi motu) Claude ko goli maar di: server ek seedhi line kheenchega, kya Claude us line pe tha?

Claude ka gudda bana ke dekha goli kahan lagi: upar lagi to headshot, neeche lagi to "Arpan killed Claude".

*Technical me:* "server line kheenche, deewar dekhe, gudda bana ke dekhe" = server-side hit validation (raycast: goli ki line, beech me deewar, player ka hitbox), "upar lagi to headshot" = hitbox (sar / body / pair alag), phone sirf "maine is disha me goli chalayi" bhejta, faisla server ka.

**Isi pe ek aur misaal (round me: hamari goli bas animation):** Arpan ne enemy ko dekh ke goli chalayi. Server scene recreate karta: jahan se goli chali wahan se seedhi line us guddi tak; beech me deewar to nahi, player chhupa to nahi. Goli seedhi uske sar pe lagi = turant headshot. Hamari goli kuch nahi karti, bas animation hai; jo chala wo sab server ne kiya. Guddi wahan khada nahi hota to goli miss, koi dikkat nahi.

---

**3b. Phone ne Arpan ko turant 3 kadam aage dikha diya. 120 ms baad server ka jawab: us pal se pehle hi Claude ki goli lag chuki thi, Arpan ruk gaya tha, 3 kadam chala hi nahi. Phone pe ek jagah, server ke hisaab se doosri.**

**Arpan:** Server ki hi maanenge, server hi sach hai. 3 kadam aage chale to 3 kadam peeche. Maan lo Claude ne mujhe dekha, main uske dekhne ke baad 3 kadam aage badh gaya, aur Claude ne wahan goli chala di.

Server is scene ko recreate karega: goli kahan se chali, Arpan us waqt kahan tha, sab calculate karega.

2-3 millisecond ka delay hoga, server replay karega; agar main us waqt goli ke bilkul saamne khada tha to goli mujhe lagti hui nazar aayegi, bhale main 3 kadam aage chala gaya.

Ek aur scene: saamne se goliyan chal rahi, main bhaag ke ekdum deewar ke peeche gaya, phir bhi mar gaya. Bahut baar hua, PUBG kya, GTA jaise bahut game me hota. Server ki aankhen thodi hain, wo API se, ping se baat kar raha.

Goli seedhi line me chali; deewar pe lagti to server mana karta, wo deewar wala scene pata hai. Par server mujhe (delay jitna) peeche karke dekhega aur goli aayi; agar main wahan tha to goli seedhi mujhe lagi, khatam kahani.

*Technical me:* "server ki maano, 3 kadam peeche" = server reconciliation, jhatke se peeche aana = rubber banding, "server scene recreate / replay kare, mujhe peeche karke dekhe" = lag compensation (server rewind: goli wale ke ping jitna duniya peeche, Valve ne apne games me likha), "deewar ke peeche phir bhi mara" = lag compensation ka asar, corner se nikal ke maarne wale ko isi se fayda = peeker's advantage (Riot ne kam karne ko Valorant me 128 tick server).

**Isi pe ek aur misaal (round me: dost aage nikal gaya phir bhi mara):** Dost chal ke ja raha tha, saamne se goli aayi, wo 1-2 second aage nikal chuka tha phir bhi mar gaya. Kyunki goli chali to server ne scene dobara banaya aur dekha dost aakhri baar kahan khada tha; wahin goli lagi. Server dobara chala ke dekhta hai, isliye aage nikal ke bhi mar jaate.

---

**4. Kadam 3 wala wahi hacker agle match me phir aaya. Is baar uski goli imaandaar thi. Dikkat ye ki wo 1 second me 500 meter daud ke Arpan ke peeche pahuncha, phone ne bas bola "main ab Arpan ke peeche hoon".**

**Arpan:** Ye wahi Uber aur Zomato wala design hai. 500 meter to PUBG me ek poori jagah hai, poora School paar. Server ke paas pichhli jagah hai: 1 second baad player ya to wahin hoga ya aas-paas 1 meter me.

Jaise Koramangala me khadi gaadi 1 second baad Whitefield nahi pahunch sakti, namumkin. To gadbad hai: ye update phenk do (discard), ye bot hai ya hacker.

Is baar ghus gaya to ghus gaya, uski koi baat mat maano, aur agli baar match me ghusne hi mat do.

*Technical me:* "pichhli jagah se 1 second me itna hi" = server-side movement validation (speed ki had, namumkin chaal reject), "update phenk do" = discard / server ki jagah sach, "agli baar ghusne mat do" = anti-cheat ban.

**Isi pe ek aur raasta (round me: ping chhoot gaya to):** Pochinki se School 5-6 minute ka raasta, ye 1 second me pahunch gaya.
- Iske liye pattern nahi chahiye, server khud pakad lega: pehla ping Pochinki se, doosra School se = namumkin, hacker.
- Server ne poora raasta dekha: wo maanega hi nahi. Hum hote to Arpan ko marne nahi dete, kill nullify, count nahi hoga.
- Beech me kuch millisecond server andha ho gaya, ping aaye hi nahi: chhalaang dikhi hi nahi. Hacker School me aaram se chal ke 10 meter andar khade Arpan ke peeche gaya aur maar diya, to server ko ye theek khela lagega.
- Par sab logs me likha ja raha: baad me dekh ke uske kills radd, agle din phir aaya to phir radd, kuch din baad match me ghusne hi mat do.
- PUBG asal me kya karta (nullify ya ban), wo unke docs me hoga, mujhe nahi pata.

*Technical me:* "server khud pakde, maane hi nahi" = server-side movement validation (server authoritative), "ping chhoota to chhalaang dikhi hi nahi" = packet loss se beech ka gap, "baad me logs dekh ke radd + ban" = offline cheat review + ban.

---

**5. "Winner winner chicken dinner." Raat 10 baje 5,000 match lagbhag ek saath khatam, 5 lakh players ke kills, damage, rank points ek pal me DB pe. DB ghut gaya, Arpan ki screen pe "Rank points updating..." 2 minute se.**

**Arpan:** Har cheez turant DB pe? Arpan ne itne kill kiye, Shubham ne itne, Claude ne itne (Claude bechara mar gaya).

Mujhe aisa DB chahiye jo likhai dhadadhad pel de: Cassandra, kyunki yahan paise jaisi pakki consistency nahi chahiye, wo kaam server dekh raha. Aur 5,000 to kam bole, lakhon match hote hain.

Docs wala scene: har click pe DB pe mat jao; match ke beech parchi pe likho "Arpan ne itna kiya", match khatam ho to ek saath DB me. Lakh match = lakh parchi, parchi kho jaaye to uski copy rakho.

Phir sab parchiyan le jao aur DB me bharo, DB pe achanak itna bojh nahi.

*Technical me:* "likhai dhadadhad" = write-heavy DB (Cassandra), "match ke beech parchi, end me ek saath" = game server memory me jama, match end pe batch, "parchi line me + copy, phir DB" = queue (Kafka) + async write, copy = durable queue (Arpan ko DB ka intezaar nahi, agla match turant).

**Isi pe ek aur raasta (round me: coordinate kyun likhein):** Cassandra me aise hi kuch bhi nahi likh dete.
- Likhai isliye bahut, kyunki hacker ek din me nahi pakda jaata, pattern chahiye: ek second me Koramangala, agle second Whitefield, namumkin. Sirf aakhri coordinate likha to ye pata hi nahi chalega.
- Har user ko seedha DB pe nahi likhwaana: pehle parchi (Redis) me "iska aakhri coordinate ye" (jaldi milta), phir Kafka me batch, line lagwa ke Cassandra.
- Match khatam pe har kisi ke stats nahi chahiye, damage / rank points kuch nahi: sirf kills (Arpan ne 4, isne 8, usne 9) aur aakhir tak kaunsi team bachi, wahi winner.
- Coordinate sirf pattern ke liye, taaki hacker dikhe aur game sudhaar sakein.

*Technical me:* "parchi -> line -> Cassandra" = Redis + Kafka batch writes, "kai din ka pattern dekh ke hacker" = offline cheat detection (poora raasta log, baad me analytics).

&nbsp;

Poora design = [20_pubg.md](20_pubg.md)
