# Twitter feed: hand notes (Arpan, Ravi aur King Kohli)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ne tweet kiya "aaj ka mock 5/5". Ravi Arpan ko follow karta hai, aur King Kohli ko bhi (maan lo 27 crore followers). Ravi app kholta hai to home feed pe sabke naye tweet, naye pehle.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Ravi  -->  LB  -->  tweet server  -->  Kafka ("post ho gaya, nikal")  -->  workers
                                                                            |
                    har follower ka dabba (Redis, sirf last 500)  <---------+   (chhote user: likhte waqt bhar do)
                    Kohli / SRK ki alag parchi (10-15 copy)                     (celeb: padhte waqt milao)
                                |
   feed khule:  dabba + celeb parchi milao, ranking, number se poora tweet (photo CDN se)
   asli maal:   Cassandra (user id se baanto, 3 copy), purane tweet S3, follow ka track Neo4j
   video:       S3 presigned URL -> workers 4 size banayein -> CDN -> phone tukde kheenche
   region:      India branch + US branch, aapas me baat

 1   500 logon ke tweet milao, feed 6-7 sec     ->  pehle se dabbe me rakho (fan-out on write)
 1b  naya follow, purane tweet dabbe me nahi    ->  follow karte hi purane push (backfill)
 1c  unfollow, purane 50 dabbe me               ->  naye band, purane dheere khisakte (lazy)
 2   Kohli ke 27 crore dabbe, ghanton           ->  ek parchi, sab wahin se padhein (fan-out on read)
 3   doston ka dabba + 15 celeb parchi          ->  padhte waqt sort karke milao (merge at read)
 4   8,000 dabbe, Post ka button ghoome         ->  Kafka me daalo, "post ho gaya" turant (acks=all)
 5   six pe 5 crore, parchi wali machine hili   ->  10-15 copy, baant do (hot key replica)
 5b  "Inida" se "India", kuch copy purani       ->  thodi der galat, phir sab theek (eventual consistency)
 6   3 saal ke tweet dabbe me, 30 crore soye    ->  last 500 bas, 30 din nahi aaya to kuch nahi; purane S3
 7   50 crore tweet / din, likhai ki line       ->  Cassandra, user id se baanto; follow = Neo4j
 8   Ravi Delhi me, server America me           ->  India branch (region)
 9   John New York me, Arpan ka tweet nahi      ->  dono branch aapas me baat; ruki to replay
10   50,000 log 1 GB video                      ->  S3 presigned URL, server se nahi
11   John 4K TV, Ravi gaon me 3G                ->  kai size (transcoding, ABR)
12   ek machine 4 size, 20 minute               ->  kai workers, video id se baanto
13   worker 7 gira, "processing" ghanton        ->  heartbeat, timeout, checkpoint se aage
14   2:00 pe khiskaya, phone 0:00 se kheenche   ->  manifest, seedha 51va tukda; CDN pe static
15   tukde US ke S3 me, samundar paar           ->  CDN edge; private video = signed URL
16   dabbe me sirf number                       ->  number se poora tweet (hydration); delete = soft delete
17   Amit ka tweet 40 ke neeche dab gaya        ->  jisse judta wo upar (ranking)
18   scroll pe wahi 5 dobara                    ->  cursor, offset nahi
19   "mast DP" gaayab                           ->  do pe likho, do se padho (quorum)
20   naya user, 2 tweet ke baad khaali          ->  trending + suggested, engagement se apni pasand (cold start)
```

**FR / NFR kahan se:** upar ki dikkat chain dekh ke nikale; har line ke aage likha hai wo kis kadam se aayi.
Arpan: Twitter ko engagement ke hisaab se dekhna hai. Naya banda aaya, kisi ko follow nahi karta, to khaali page nahi dikhaunga; trending dikhaunga: aaj ye hua, Elon ne SpaceX me ye kiya, SpaceX aur Tesla ka merger. Instagram bhi yahi karta: main kisi ko follow nahi karta tha phir bhi bahut kuch aata; GTA 6 baar-baar search kiya to GTA 6 aane laga. Ravi 1000 logon ko follow karta, uski feed khatam nahi hoti, wahan scroll me dohrav nahi; refresh pe purana dobara aa jaaye to theek, Twitter iska hisaab nahi rakhta.
Technical me: "khaali page nahi, trending dikhao" = cold start / recommendation (follow ke alawa), "jo search kiya wahi aane laga" = interest se ranking, "scroll me dohrav nahi" = cursor (kadam 18).

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      1  tweet post karo (text, photo / video)                               -> kadam 4, 10
        2  home feed: jinko follow karta unke tweet, jisse zyada judta wo upar -> kadam 1, 2, 3, 16, 17
        3  infinite scroll: ek scroll ke beech wahi tweet dobara na aaye
           (refresh pe aa sakta, Twitter iska hisaab nahi rakhta)              -> kadam 18
        4  follow wale khatam to neeche doosron ke tweet (trending),
           feed kabhi khaali nahi                                              -> kadam 20
        5  video har net pe chale (tukde, kai quality, beech se dekhna)       -> kadam 11-15
        6  search: interviewer maange to Elasticsearch jaisi alag cheez (Yelp jaisa); wo apne aap me alag
           duniya                                                              -> News kadam 13 (wahi jawab)

NFR     1  feed ~200 ms me (40 din baad aaya to thodi der chalegi)            -> kadam 1, 6
        2  2-5 sec purani chalegi (isliye pehle se bana sakte)                -> kadam 4, 5b
        3  27 crore wale celeb ka tweet bhi sambhle                           -> kadam 2, 5
        4  duniya bhar ke user: paas ki branch se tez, door ka tweet bhi dikhe -> kadam 8, 9
Numbers ~50 crore user, 50 crore tweet / din = ~5,800 / sec · ek aadmi ~500 ko follow · shaam 8 baje 5 crore ek saath
        -> padhna : likhna ~50 : 1, isliye feed pehle se taiyaar rakho
```

&nbsp;

**1. Ravi app khole to server dhoondhe wo kis-kis ko follow karta (500 log), sabke tweet DB se nikaale, milaye, sort kare. Shaam 8 baje 5 crore log ek saath, feed 6-7 second ghoome, DB hil jaaye.**
Arpan: Baar-baar DB jaana padta. Mere 10 dost hain, wo 10 dost 3 logon ko follow kar rahe, roz din ka hisaab maang rahe. Har baar un teeno ke paas kyun jaun jab roz yahi karna hai? Main kahunga tum teeno idhar aao, mujhe apna de do, main apne paas rakhunga aur apne paas se bhej dunga. Dobara DB hit nahi. Unhone app khola, unhe turant mila. Har baar mote register me nahi jaana pada.
Technical me: mota register = DB · "apne paas rakhunga, apne paas se bhej dunga" = har user ka pehle se bana inbox (Redis), "dost apna mujhe de do" = tweet aate hi followers ke inbox me (fanout on write), "dobara DB hit nahi" = feed seedha inbox se.

**Maan lo Ravi ne aaj Amit ko follow kiya, Amit ke pichhle hafte ke 20 tweet Ravi ke dabbe me kabhi gaye hi nahi.**
Arpan: Main Twitter ki service pe baitha hoon, dekha Ravi ne abhi Amit ko follow kiya. Amit ne jo pehle daala hai wo Ravi ki feed me seedha push kar do, jaise Instagram pe follow karte hi purani reels dikhti. Aage Amit jo naya daalega wo waise hi chala jaayega.
Technical me: "follow karte hi purane push" = backfill on follow, "naya waise hi" = fanout on write (kadam 1).

**Maan lo Ravi ne Arpan ko unfollow kiya, par dabbe me Arpan ke 50 purane tweet pade hain.**
Arpan: Feed sirf follow walon se nahi banti: main khud Elon, Sachin, Kohli ko follow nahi karta, phir bhi mujhe unke tweet dikhte, kyunki Twitter / Instagram ka kaam hi engage rakhna hai; kuch follow na karo to bhi khaali page nahi dikhta. Unfollow ke baad pade hue tweet turant gayab nahi honge, par naye aane band, aur jaise-jaise feed banegi Arpan ke tweet dheere-dheere kam ho jaayenge.
Technical me: "turant gayab nahi, dheere kam" = dabba saaf nahi karte (lazy, purge nahi), naye fanout band, ranking (kadam 17) neeche karti; feed me follow ke alawa recommendation bhi.

**Maan lo bahut saare server, Ravi ki request kabhi kisi pe:**
Arpan: App stateless hoga. Bahut saare server honge, har server yaad nahi rakhega ki ye zaroori info kiski hai, to hum JWT use kar lenge.
Technical me: "har server yaad nahi rakhega" = stateless app server (session server me nahi), "JWT" = token me hi user ki pehchaan, koi bhi server padh le.

**2. Kohli ne tweet kiya "Century for India". 27 crore dabbon me daalna, ghanton lagte. Ravi ko 2 ghante baad dikha, match khatam. Tab tak baaki sabke tweet bhi line me peeche atke.**
Arpan: 27 crore ko nahi de pa raha. Aur agli baar 28 crore, 29 crore honge, dabbe badhte rahenge, dikkat nahi rukegi. 100-200, 2,000-3,000 ke liye theek hai. 27 crore bahut bada number hai, daalte-daalte system thak jaayega. Kohli ne century banayi aur likh di, to lo bhai, maine ek chhoti parchi pe likh di, yahan laga di. Na DB, na kuch. 27 crore yahin se padh lenge.
Technical me: "27 crore dabbe" = fanout on write, "100-200 ke liye theek, 27 crore ke liye nahi" = followers ki had se do raaste, "ek chhoti parchi, sab yahin se padhein" = celeb ka tweet uski apni list me, feed khulne pe wahan se (fanout on read). Dono saath = hybrid fanout.

**3. Ravi ke dabbe me doston ke tweet hain, par Kohli, Dhoni, SRK... 15 bade logon ki parchiyan alag. Ravi ko ek hi feed chahiye, sab apne time ke kram me.**
Arpan: Sabko ek saath lao. Ek parchi pe likh diya: ye Dhoni ka, ye Kohli ka, ye Arpan ka. Ab sabko sort karte hain, jaise DSA me sort karte, aur Ravi ki feed pe push. Ravi ne app khola: "le, ye tera aaj ka poora twitter ka samachar, jinko tune follow kar rakha tha, sort kar diya hai, dekh le."
Technical me: "sabko ek saath lao, ek parchi pe" = inbox + follow kiye celeb ki list, "DSA jaisa sort" = time se merge, "Ravi ki feed pe push" = merge at read.

**4. Arpan ke 8,000 followers (abhi bhi chhota user). "Post" dabaya, server ek-ek karke 8,000 dabbe bhar raha, button ghoomta rahe. 3,000 pe server gira, baaki 5,000 ko kabhi nahi mila, Arpan ko pata bhi nahi.**
Arpan: Sabse pehle ek threshold rakho, itne followers paar gaye to inbox me push nahi, Kohli wala tareeka. Bheed sambhalne ke liye ek dabba laga do, mera favourite stadium wala bhai Kafka. Maine post dabaya to "tera post ho gaya, nikal", peeche Kafka sab karta rahega. Kuch gira to outbox me rakh lo, ek parchi pe likha hai.
Technical me: dabba = Kafka (beech ki line) · "threshold, Kohli wala tareeka" = had ke upar celeb raasta, "stadium wala bhai" = Kafka (async fanout), "post ho gaya, nikal" = turant OK, "parchi pe likha" = outbox ki soch (outbox = DB ki table jo kaam ke saath usi transaction me likhi jaati); yahan tweet Cassandra me (wahan tweet + outbox ek transaction me nahi likhe jaate), isliye Kafka hi pakki parchi: Tweet Service pehle Kafka me daale (acks=all), workers / consumers Cassandra + inbox likhein.

**5. Kohli ne 99 se six maara aur tweet kiya. 1 minute me 5 crore log refresh, sabko wahi ek parchi chahiye. Wo parchi ek machine pe, wo machine hil gayi, Dhoni-SRK ki parchiyan bhi atki.**
Arpan: Us waqt wo cheez famous ho gayi to log aayenge. Use alag-alag machine pe baant do, 5 crore ek machine pe na aayein. Main dekh raha lakhon log is parchi ko padhne aa rahe. Main iski 10-15 copy bana dunga: le tu bhi ye copy le, sabko padhwa. Bheed badh rahi to baant do, copiyan bana do, clone kar do.
Technical me: parchi = Redis me us famous cheez ki key · "famous ho gayi, log aayenge" = hot key, "10-15 copy alag machine pe" = key ki replicas, "baant do" = read unme bantna.

**Maan lo Kohli ne "Inida" likh ke 10 second me "India" kar diya, kuch copies purana dikha rahi.**
Arpan: thodi der galat dikhega na. Peeche ek job chala do, ek ladka laga do, wo jaldi-jaldi sabko theek information pass kar dega. Kuch second lagenge. System rok nahi sakte, ye nahi keh sakte ki nahi dekhne denge.
Technical me: ladka = background job · "thodi der galat" = eventual consistency, "peeche ek job" = background job copies update kare.

**6. Har account ka dabba memory me. Ravi ke dabbe me 3 saal ke tweet. 30 crore log mahino se app nahi khol rahe, par unke dabbe bhi roz bhar rahe. Memory ka bill aasmaan pe.**
Arpan: Parchi chhoti karo: bas last 500. Itna koi scroll nahi karta, karega bhi to DB se query karke Redis me rakhne me koi bada teer nahi lagta. 3 saal, 4 saal purana mote register me mat rakho, alag permanent diary me, jise aam bhasha me S3 kehte, archive. Aur jo 30 crore log aa hi nahi rahe: main unki parchi taiyaar kar raha, wo saale 10 din, 20 din, 30 din se aa nahi rahe, bekaar ki mehnat. Threshold: 30 din se app nahi khola to Redis me kuch taiyaar nahi. 40 din baad aaya to "thodi der ruk, 2 second me teri parchi la raha", DB se laake Redis me daal denge, thodi loading chalegi.
Technical me: parchi = Ravi ka inbox (Redis), mota register = DB · "last 500" = inbox ki had, "DB se query karke Redis me" = cache miss pe DB, "permanent diary, S3, archive" = purana data S3, "30 din se nahi khola to kuch taiyaar nahi" = inactive user ka inbox nahi, "lautne pe DB se Redis me" = on-demand inbox banana.

**7. Saare tweet ek SQL DB me, din me 50 crore naye, likhne ki line itni lambi ki machine hile. Aur dabba banane ke liye har baar "Arpan ke naye 500" poori table me dhoondhna.**
Arpan: Likhai bahut zyada hai to alag machine, technical naam Cassandra, mera bade bhai. "Bhai Cassandra, mera system bahut likh raha, tu bada bhai hai, dekh le." Replica bhi wo kar leta. Kaun kisko follow kar raha, wo Cassandra me rakhne ka matlab nahi, uske liye ek aur bada bhai Neo4j: "ye aadmi isko follow kar raha, iska track rakhna."
Aur Arpan ke saare tweet ek jagah: naam se todo, par naam do logon ke same ho sakte (doosra Arpan), to id me baant do. Meri id har baar same, to mera saara data ek jagah.
Technical me: "likhai bahut, bada bhai" = Cassandra (write-heavy; andar ring = consistent hashing), "replica bhi wo kar leta" = Cassandra ki apni copy, "naam se nahi, id se baant do" = partition key = user id, "mera saara data ek jagah" = ek partition (author id se hissa, andar tweet id se tweet: key = author id + tweet id), "follow ka track" = graph DB Neo4j.

**Maan lo har baar "Arpan ke naye 500" poori table me dhoondhna pad raha:**
Arpan: Mujhe naye tweet chahiye, purane se koi matlab nahi. Main ek pattern note kar raha: har din ek banda aata, "Arpan ke naye 40, 50, 500" maangta; samajh gaya wo uska follower hai, use ye chahiye. To unhe thodi der ke liye apne paas rakh le, jo poori table na khojni pade. "Le bhai, Arpan ne aaj 50 kiye, rakh le"; "aaj Arpan ne kuch nahi kiya, sorry"; "aaj bas ek, le le". Har baari poori table pe mat jao, apne paas rakh lo, kyunki per day pattern hai, har din aayega.
Technical me: "naye tweet thodi der apne paas" = author ke recent tweets ka cache (Redis, TTL), "per day pattern" = recent data hi garam (time-based: naye tweet baar-baar padhe jaate), poori table scan nahi.

**Aur neeche (Cassandra hi kyun, aur key kya):**
Arpan: Twitter ke liye TB of data ho sakta hai; yahan writes bhi zyada hain aur read bhi zyada, aisa nahi ki sirf read hi zyada. Isliye Cassandra, aur Twitter har jagah hai to user ID se shard.
Technical me: write-heavy + read-heavy + TB data = Cassandra, partition key = user id.

**8. Saare server America me. Ravi Delhi me, har request samundar paar, har kaam me aadha second upar se. IPL pe India ke 10 crore America ki machine hila rahe.**
Arpan: Ravi India se kar raha to har baar America kyun bhejna, bekaar ka travel. India me hai to baatcheet India me rakh. Ye bank jaisa hai: Lajpat Nagar ke log doosri branch me aa rahe, us bank pe dabaav. Lajpat Nagar me branch nahi hai? To wahan branch khol do, wahi chale jaayenge.
Technical me: "baatcheet India me rakh" = India region, "Lajpat Nagar me branch khol do" = user ke paas wala region.

**9. Arpan ne India branch me tweet kiya. Follower John New York me, America branch se baat karta, use pata hi nahi. Ulta Kohli America se tweet kare to Ravi ko nahi dikhta.**
Arpan: Do register hain, mera is wale se baat karta, John us wale se, par dono register ek-doosre ko nahi jaante. To unhe bhi baat karni padegi. Jab ye kuch likhe to apne register me to theek, aur us register pe bhi bhej do jahan se Arpan padhta. Jaise SBI ki do branch: dost ne Mumbai wali me 500 daale, main Delhi wali me jaake poochhta "mere liye kuch aaya?", branches aapas me baat karti hain to bata deti.
Technical me: "register aapas me baat karein" = cross-region replication, "us register pe bhi bhej do" = tweet doosre region ko copy.

**Maan lo dono branch ki baat 10 minute ruki aur Arpan ne beech me 5 tweet kiye.**
Arpan: ek banda bithao jo pal-pal dekhta rahe: "America wala dost aaya?" Nahi aaya, nahi aaya, ab aa gaya. "Le, Arpan ke 5 tweet, teri feed me push kar diye." Mujhe bas dekhna tha ki tu hai ya nahi, tu aa gaya to le tera kaam.
Technical me: tweet tab tak rakhe rehna = queue me pending, "pal-pal dekhta" = retry, "aa gaya to push kar diye" = link wapas aane pe replay.

&nbsp;

**Video (Arpan stadium se King Kohli ke six ka 1 GB 4K video daalta hai)**

**10. Stadium se 50,000 log 1 GB ke video daal rahe, sab Tweet Service se guzarte, server ki memory-net bhari, text tweet bhi atke.**
Arpan: Jo video aa rahi hai use kahin aur bhej de, apne server pe na la. Bhai tu yahan apne server se nahi, alag is route pe chala ja, tujhe phir ek URL se access kar lenge.
Technical me: "kahin aur bhej, apne server pe na la" = video seedha S3 me (presigned URL), "URL se access" = tweet me sirf link.

**11. S3 me ek hi 4K file. John WiFi + TV pe mast, Ravi gaon me 3G phone pe, 1 GB kheench nahi paata, video chalta hi nahi.**
Arpan: 1 GB kyun kheench rahe, thoda-thoda kheench lo, kuch-kuch second ka. Chhote size bhi rakho. Phone bole "mera net bekaar, 240p de", net achha to 4K. Ferrari highway ke liye, Alto gaon ke liye: 4K achhe net pe, 240p-360p kharab net pe.
Technical me: "kai size" = transcoding, "thoda-thoda kuch second ka" = segments (chhote tukde), "phone net dekh ke maange" = ABR (adaptive bitrate).

**12. Ek machine 4K se 4 size banaye, 20 minute; Arpan ka tweet atka, aur 50,000 video usi ek machine ke peeche.**
Arpan: Ek machine kyun, kai machine kar de, kaam baant do. Video ki ek id rakh lo, parchi pe likh do, taaki repeat na ho. 10 log isi id pe kaam karein, aur kisi id pe nahi.
Technical me: "kai machine, kaam baanto" = video ke hisse, transcoding workers / consumers (parallel), "id ki parchi" = har hissa video id + hissa number se.

**13. 10 worker / consumer, 9 ka hissa bana, worker / consumer 7 beech me gira; video na taiyaar na koi batata, "processing" ghanton se.**
Arpan: Har worker / consumer ka status. 9 ne bata diya, ek nahi bata paaya to dekhunga, heartbeat: "tu hai kya?" Jawab nahi to ise ghar bhejo, naya banda. Uske paas parchi: "maine yahan tak kar liya", naya wahin se uthaye. Save se pehle gira to shuru se.
Technical me: "har worker / consumer ka status" = DB me hisson ki ginti, "tu hai kya" = heartbeat, "naya banda" = job dobara (retry), "yahan tak parchi" = checkpoint.

**Isi pe ek aur raasta (jo khaali hai wahi utha le):**
Arpan: Ya naya banda lao, ya jo 9 apna kaam khatam kar chuke unme se ek ko bhejo; wo parchi dekh ke wahin se uthaye jahan 7 ruka tha.
Technical me: khaali worker kaam utha le (work stealing), checkpoint se aage.

**Maan lo worker / consumer 7 zinda hai, heartbeat aa rahi, par 1 ghante se ek hissa bana raha, baaki 2 minute me khatam.**
Arpan: Uski wajah se mera kaam ruk raha. "Tu thoda rest le, ye tera replacement, bata kahan tak chhoda, ye wahin se karke dega."
Technical me: "atka to replacement" = timeout pe slow worker / consumer ka hissa doosre ko, "kahan tak chhoda" = checkpoint.

**Isi Maan lo me aur neeche ("zinda hoon" kaafi nahi, kaam kahan tak hua):**
Arpan: "zinda hoon" kaafi nahi. Chhotu tu zinda hai na? Haan. Kaam kahan tak hua? "Main zinda hoon, kaam kar raha hoon." Mujhe ye bata kaam kahan tak hua. "Abhi kuch feed reh rahi." In 10 ne kaam kar liya, tere paas aisa kya kaam? Ek aur chhotu ko bhej ke dekho: "Arpan bhai, iske paas bahut bada chunk hai, akele sambhal nahi pa raha, thak chuka." Kisi ke paas 10 newspaper the, kisi ke 20, is bande ke paas 50 aa gaye. To 2-3 chhotu extra bhej do, madad kar lein, feed taiyaar ho jaayegi aur log subah akhbaar padhne aayein to taaza mile.
Technical me: "zinda hoon != kaam ho raha" = heartbeat ke saath progress report, "bada chunk" = data skew (ek hisse pe zyada kaam), "2-3 chhotu madad" = hissa tod ke baanto (work stealing / split).

**Maan lo worker / consumer 7 ke 50 me se 30 ban chuke, 2 naye chhotu bheje; unhe kaise pata 31 se shuru karna, warna pehle ke 30 do baar.**
Arpan: Har chhotu ke paas ek parchi hogi: mera kaam yahan tak ho gaya. Sabko bol diya "apne kaam ka hamesha track rakho", kyunki kisi din bimaar pad gaye, ekdum kuch ho gaya, to main aaunga, kaise dekhunga kaam kahan tak hua. Wo track DB me, Redis me, kahin bhi; technical naam me nahi jaate, problem kaise samajh rahe wo dekhte hain. Kisi ne 5 kiye, kisi ne 7, kisi ne 10, sab likhte gaye. Worker / consumer 7 ne likh diya "30 khatam". Naye chhotu ko kaha pehle iski parchi dekho, isne kahan tak khatam kiya; 31 pe lag jaao, 10 tu sambhal, 10 tu sambhal, kaam jaldi nipta do.
Technical me: chhotu = worker / consumer · "kahan tak hua ki parchi" = checkpoint (DB / Redis me), naya worker / consumer wahin se shuru (resume), baaki bacha kaam baant ke.

**14. Ravi 2:00 pe khiskata hai six dekhne, par phone 0:00 se 2:00 tak sab kheench raha, faaltu data, six late.**
Arpan: 0 se 2 dikhane ka fayda nahi. Kisi ne copy di aur bola seedha 51wa page dikha. Main copy dekhunga, kahan 50 khatam, 51 kahan shuru, hisaab lagaunga, aur seedha 51 khol dunga.
Technical me: "copy me kaunsa page kahan" = manifest (tukdon ki list), "hisaab, seedha 51" = seek: time se tukda number, seedha wahi tukda.

**Maan lo 1 crore log dekh rahe, har ek ke ~150 tukde. Kya har tukda humare server ki API se jaayega?**
Arpan: Tukda dene me sochna kuch nahi, bas file hai, to CDN pe rakh do. Pehle socha har reader ke paas ek bachcha khada karun jo yaad rakhe kaunse page pe hai, par 1 crore log = 1 crore bachche. Book me koi bachcha nahi hota, mere paas bookmark hota: 50 pe hoon to 51 khud palat leta. Sab client side, server pe aane ki zaroorat hi nahi.
Technical me: "bas file" = tukde CDN se seedhe (static), "bookmark" = phone ka player khud yaad rakhta kahan hai, "51 khud palto" = manifest se agle tukde ka link, server sirf shuru me ek baar (manifest + link).
Arpan: Baat 10-20 crore ki nahi. AWS jaise server bahut kuch sambhal lete hain, dikkat wo nahi. Baat ye hai ki kaam kahan aasaan ho sakta hai. Yahan phone pe aasaan ho gaya to wahin kar diya. Kisi ne phone ka socha hi na hota to wahi bachche wala tareeka chal raha hota. Dikkat aayi, solution ban gaya.
Technical me: "kahan aasaan" = kaam wahan karo jahan sabse sasta + seedha (yahan client), server pe bojh + kharcha bina wajah nahi.

**15. Six viral, 1 crore log India, US, Australia se; saare tukde US ke S3 me, samundar paar, bill aasmaan pe, India me atke.**
Arpan: Samundar paar kyun? Europe wala dost bole "video mere Europe me aa jaaye". Iski copy bana do, tu apne paas rakh, tu apne paas, teeno copy same, jo jahan se dekhe paas se khule.
Technical me: "copy paas me rakho" = CDN (edge cache).

**Maan lo private video (10 dost), Ravi ne CDN link bade WhatsApp group me daal diya.**
Arpan: Taala-chaabi mere paas. Kamra mera, 10 log hi aa sakte. 11wa aaye, dost ka dost ho, access nahi.
Technical me: "taala-chaabi maalik ke paas" = har request pe access check, "10 log hi" = allow list (sirf link kaafi nahi).

**Isi pe ek aur misaal (ghar ke darwaze pe papa):**
Arpan: Ghar mera, par main US me hoon, aur CDN bhai Mumbai me darwaze pe khada hai; log mere paas aayenge hi nahi. Isliye papa (CDN) ko bol diya: ye 10 dost andar aane do, saath aaye 100 nahi.
Technical me: origin door, check edge pe (signed URL / cookie).

**Maan lo tukde CDN se seedha jaate hain, CDN ko allow list pata hi nahi, aur link leak ho gaya.**
Arpan: CDN ke paas bhi taala-chaabi ho, har request US tak nahi jaayegi. In 10 ki user id do: ye 10 aayein to link do, nahi to mat do. Aur link ki umar 5 minute, uske baad kisi ke liye nahi khulega. Leak bhi hua to 11wa nahi ghusega. Mohar kaise jaanchi jaati, wo AWS ka kaam.
Technical me: "CDN ke paas bhi taala" = CDN signature jaanchta (signed URL / cookie), "10 user id" = server allow list dekh ke hi link deta, "5 minute umar" = short expiry. Hamara faisla: kisko aur kitni der ka.

**16. Ravi ke dabbe me sirf tweet ke number (#905, #902 ...). Screen pe chahiye kisne likha, photo, kitne like.**
Arpan: Mere paas ek mota register hai, sab likha hua. 1,00,000 page me dhoondhna time lega, main baawla thodi hoon. Par mere paas ID hain (905, 902), to seedha wahi page kholunga. Register 1 crore page ka ho ya 10 crore, ID ho to kaam aasaan. Photo / video CDN se aa jaayegi, baaki mote register se. Isme koi badi dikkat nahi.
Technical me: "ID se seedha page" = hydration (ID se poori post, key se lookup; dabbe me tweet number ke saath kiska tweet bhi, "905, Arpan ka": author se hissa, number se tweet), "photo CDN se" = post me CDN link, "mota register" = tweet store (Cassandra), like count saath me.

**Maan lo Arpan ne tweet delete kar diya, par 8000 logon ke dabbe me uska number (ID) abhi bhi pada hai.**
Arpan: Tweet feed me push ho chuka tha to Ravi ko kuch second ke liye dikh sakta hai; Twitter ko baad me pata chala user ne delete kar diya, to wahan likh dega "the user has deleted this post". Ye aasaan cheez hai.
Technical me: tweet store me deleted flag (soft delete); kuch second purana dikhna = eventual consistency; "deleted" ki patti = tombstone; 8000 dabbe saaf nahi karne padte.

**Isi pe ek aur misaal (Elon):**
Arpan: Elon kabhi tweet karta hai, phir delete kar deta hai; thodi der ke liye dikh jaata hai, isi beech log screenshot le lete hain. Yahi hota hai.
Technical me: delete ke baad kuch der dikhna = eventual consistency.

**17. Ravi roz dost Amit ke tweet pe like / comment karta. Amit ka tweet 3 ghante pehle aaya, uske baad 40 aur, Amit ka neeche dab gaya; upar wo jinhe Ravi kholta bhi nahi.**
Arpan: Ye ek tarah recommendation hi hai. Peeche kaunsa algorithm / machine learning, wo design ka hissa nahi. Par main roz Amit ki photo like kar raha, baaki 40 ko bas dekh ke chhod deta. Instagram roz dekh raha ki ye ladka Amit ko like karta, to pehli photo Amit ki dikhayega. YouTube me bhi same: MrBeast ki video roz dekhun to wahi suggest karega. Unka kaam hi yahi: main jitni der rahun, unki kamaai badhe.
Technical me: "jisse zyada judta wo pehle" = ranking (kisse baat karta, post kitni nayi, likes), "algorithm design ka hissa nahi" = ranking model alag; hum list dete, ranked list lete. Netflix 6 ("tere liye") se juda.

**18. Ravi scroll kar raha, 20 dekhe, agle 20 maange. Beech me upar 5 naye aa gaye, agle page pe wahi 5 dobara.**
Arpan: 20 padhe, agle 20 maange, beech me upar 5 aa gaye, to neeche wale 5 khisak ke agle page me chale gaye; isliye wahi 5 dobara. Hal: Ravi ne kahan tak padha, thodi der ke liye yaad rakh lo. "Ravi ne yahan tak padh liya, ab iske baad wale do." Naye 5 upar hain, upar scroll karega to dikhenge. Refresh pe purani post dobara aa sakti hai, Instagram ka kaam hi engage rakhna, wo chalta hai; par interviewer poochhega to use batana padega, to main yahi karunga: Ravi ne kahan tak padha wo yaad rakh ke agle dikha dunga.
Technical me: "page number se agle 20" = offset pagination (isi se khisakta), "kahan tak padha yaad rakh" = cursor (har page ke saath aakhri dikhayi post lautao, agla uske theek baad se), "thodi der yaad" = ranked feed me ranked list thodi der save, cursor usi list me aage.

**19. Reply ki 3 copy (A, B, C) (Twitter me Cassandra, koi leader nahi; SQL shard ho to leader + copy). Tune Ravi ki DP pe reply kiya "mast DP bhai", ye A aur B me likha, C tak abhi nahi. Ravi ka "thank you bhai" C me pehle pahunch gaya. Priya C se padh rahi: sirf "thank you bhai" dikha, tera reply gaayab.**
Arpan: Sabse pehle to ye bolunga ki thodi der leader se padhwa do. Interviewer cross-question karega: leader pe bahut bojh padega, phir? To phir neeche wala (do pe likho, do se padho).
Technical me: "thodi der leader se padhwa do" = naya likhne ke baad kuch der read leader se (read-your-writes / leader read), "leader pe bojh" = trade-off: saari read leader pe = leader garam, isliye phir quorum.
"Thodi der" kitni: andaza nahi; copy khud bataye kahan tak pahunchi, uski likhai tak aa gayi to copy se, nahi to leader se (banking kadam 9).
Arpan (phir): Ye to pehle kar rakha hai: do pe likho, do se padho. 3 copy hain, update kisi me bhi ja sakta hai. Maan le DB shard karke ek leader aur neeche 3 copy rakhi. Leader ne "mast DP" likha, wo copy pe jaa raha. Jab tak leader aur kisi 2 copy ne rakh nahi liya, tab tak usko pura mano hi mat. Phir Priya ki request aayi to wo kisi 2 copy se poochhegi: C aur A se poochha, C me nahi to A me hai; B aur A se poochha, B me nahi to A me hai. Aur Twitter me to humne Cassandra lagaya hai, Cassandra ye apne aap karta hai (andar consistent hashing bhi).
Instagram pe bhi yahi hota: profile photo update ki, turant refresh kiya to purani photo dikhi, thodi der baad nayi.
Technical me: "do pe likho, do se padho" = quorum (3 copy me W = 2, R = 2, W + R > 3, isliye padhne wali do me se ek me naya reply zaroor), "leader aur 2 copy ne rakh liya tabhi pura mano" = write tabhi OK jab 2 copy ne haan bola (ack), "Cassandra apne aap karta" = Cassandra me koi leader nahi, quorum ka level khud set hota (QUORUM read / write), URL kadam 9-10 wala hi. Instagram me "photo update ki, refresh pe purani" = replication lag (nayi photo leader me, refresh pe padhna copy se jo abhi peeche).

**20. Priya ne kal hi Twitter join kiya, sirf 2 logon ko follow karti, un dono ne 2 hi tweet kiye. Priya ne dono dekh liye, ab neeche scroll kare to kya dikhe? Khaali page?**
Arpan: Twitter engagement ke hisaab se dekhna hai. Koi banda naya-naya join kiya, kisi ko follow nahi karta, to kya khaali page dikhaunga? Nahi. Use trending dikhaunga: aaj ye hua, wo hua, Elon Musk ne SpaceX ka ye kiya, SpaceX aur Tesla ka merger hua, jisse engagement bane. Instagram bhi same karta: main kisi ko follow nahi karta, phir bhi bahut cheezein aati hain; jaise-jaise engagement hua, maine follow karna shuru kiya, ab wahi dikhata. Google pe GTA 6 baar-baar search kiya to GTA 6 ka aane laga. Twitter aur Instagram almost same algorithm pe: engagement rakhna. Ek ko follow kiya, uska ek hi tweet dikha diya to Twitter ka revenue kat jaayega; us waqt Elon Musk, Bill Gates kisi ka bhi suggest karunga, beech-beech me random tweet bhi.
Ravi ka case alag: wo 1000 logon ko follow karta, unke tweet kabhi khatam nahi, wo apna dekhta rahega. Baat unki hai jo 1-2 logon ko follow karte aur jinhone 1-2 tweet kiye.
Technical me: follow wale khatam / naya user = trending + suggested (cold start), engagement se dheere-dheere apni pasand (personalized); following feed ke neeche doosron ke tweet.

Poora design = [03_twitter_feed.md](03_twitter_feed.md)
