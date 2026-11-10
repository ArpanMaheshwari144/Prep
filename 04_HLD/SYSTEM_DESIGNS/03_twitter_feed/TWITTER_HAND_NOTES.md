# Twitter feed: hand notes (Arpan, Ravi aur King Kohli)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ne tweet kiya "aaj ka mock 5/5". Ravi Arpan ko follow karta hai, aur King Kohli ko bhi (maan lo 27 crore followers). Ravi app kholta hai to home feed pe sabke naye tweet, naye pehle.

&nbsp;

**1. Ravi app khole to server dhoondhe wo kis-kis ko follow karta (500 log), sabke tweet DB se nikaale, milaye, sort kare. Shaam 8 baje 5 crore log ek saath, feed 6-7 second ghoome, DB hil jaaye.**
Arpan: Baar-baar DB jaana padta. Mere 10 dost hain, roz apna hisaab maang rahe. Har baar unke paas kyun jaun jab roz yahi karna hai? Main kahunga tum teeno idhar aao, mujhe apna de do, main apne paas rakhunga aur apne paas se bhej dunga. Dobara DB hit nahi. Unhone app khola, unhe turant mila. Har baar mote register me nahi jaana pada.
Technical me: "apne paas rakhunga, apne paas se bhej dunga" = har user ka pehle se bana inbox (Redis), "dost apna mujhe de do" = tweet aate hi followers ke inbox me (fanout on write), "dobara DB hit nahi" = feed seedha inbox se.

**Maan lo Ravi ne aaj Amit ko follow kiya, Amit ke pichhle hafte ke 20 tweet Ravi ke dabbe me kabhi gaye hi nahi.**
Arpan: Main Twitter ki service pe baitha hoon, dekha Ravi ne abhi Amit ko follow kiya. Amit ne jo pehle daala hai wo Ravi ki feed me seedha push kar do, jaise Instagram pe follow karte hi purani reels dikhti. Aage Amit jo naya daalega wo waise hi chala jaayega.
Technical me: "follow karte hi purane push" = backfill on follow, "naya waise hi" = fanout on write (kadam 1).

**Maan lo Ravi ne Arpan ko unfollow kiya, par dabbe me Arpan ke 50 purane tweet pade hain.**
Arpan: Feed sirf follow walon se nahi banti: main khud Elon, Sachin, Kohli ko follow nahi karta, phir bhi mujhe unke tweet dikhte, kyunki Twitter / Instagram ka kaam hi engage rakhna hai; kuch follow na karo to bhi khaali page nahi dikhta. Unfollow ke baad pade hue tweet turant gayab nahi honge, par naye aane band, aur jaise-jaise feed banegi Arpan ke tweet dheere-dheere kam ho jaayenge.
Technical me: "turant gayab nahi, dheere kam" = dabba saaf nahi karte (lazy, purge nahi), naye fanout band, ranking (kadam 17) neeche karti; feed me follow ke alawa recommendation bhi.

**2. Kohli ne tweet kiya "Century for India". 27 crore dabbon me daalna, ghanton lagte. Ravi ko 2 ghante baad dikha, match khatam. Tab tak baaki sabke tweet bhi line me peeche atke.**
Arpan: 27 crore ko nahi de pa raha. Aur agli baar 28 crore, 29 crore honge, dabbe badhte rahenge, dikkat nahi rukegi. 100-200, 2,000-3,000 ke liye theek hai. 27 crore bahut bada number hai, daalte-daalte system thak jaayega. Kohli ne century banayi aur likh di, to lo bhai, maine ek chhoti parchi pe likh di, yahan laga di. Na DB, na kuch. 27 crore yahin se padh lenge.
Technical me: "27 crore dabbe" = fanout on write, "100-200 ke liye theek, 27 crore ke liye nahi" = followers ki had se do raaste, "ek chhoti parchi, sab yahin se padhein" = celeb ka tweet uski apni list me, feed khulne pe wahan se (fanout on read). Dono saath = hybrid fanout.

**3. Ravi ke dabbe me doston ke tweet hain, par Kohli, Dhoni, SRK... 15 bade logon ki parchiyan alag. Ravi ko ek hi feed chahiye, sab apne time ke kram me.**
Arpan: Sabko ek saath lao. Ek parchi pe likh diya: ye Dhoni ka, ye Kohli ka, ye Arpan ka. Ab sabko sort karte hain, jaise DSA me sort karte, aur Ravi ki feed pe push. Ravi ne app khola: "le, ye tera aaj ka poora twitter ka samachar, jinko tune follow kar rakha tha, sort kar diya hai, dekh le."
Technical me: "sabko ek saath lao, ek parchi pe" = inbox + follow kiye celeb ki list, "DSA jaisa sort" = time se merge, "Ravi ki feed pe push" = merge at read.

**4. Arpan ke 8,000 followers (abhi bhi chhota user). "Post" dabaya, server ek-ek karke 8,000 dabbe bhar raha, button ghoomta rahe. 3,000 pe server gira, baaki 5,000 ko kabhi nahi mila, Arpan ko pata bhi nahi.**
Arpan: Sabse pehle ek threshold rakho, itne followers paar gaye to inbox me push nahi, Kohli wala tareeka. Bheed sambhalne ke liye ek dabba laga do, mera favourite stadium wala bhai Kafka. Maine post dabaya to "tera post ho gaya, nikal", peeche Kafka sab karta rahega. Kuch gira to outbox me rakh lo, ek parchi pe likha hai.
Technical me: "threshold, Kohli wala tareeka" = had ke upar celeb raasta, "stadium wala bhai" = Kafka (async fanout), "post ho gaya, nikal" = turant OK, "parchi pe likha" = outbox ki soch; yahan tweet Cassandra me (wahan tweet + outbox ek transaction me nahi likhe jaate), isliye Kafka hi pakki parchi: Tweet Service pehle Kafka me daale (acks=all), workers Cassandra + inbox likhein.

**5. Kohli ne 99 se six maara aur tweet kiya. 1 minute me 5 crore log refresh, sabko wahi ek parchi chahiye. Wo parchi ek machine pe, wo machine hil gayi, Dhoni-SRK ki parchiyan bhi atki.**
Arpan: Us waqt wo cheez famous ho gayi to log aayenge. Use alag-alag machine pe baant do, 5 crore ek machine pe na aayein. Main dekh raha lakhon log is parchi ko padhne aa rahe. Main iski 10-15 copy bana dunga: le tu bhi ye copy le, sabko padhwa. Bheed badh rahi to baant do, copiyan bana do, clone kar do.
Technical me: "famous ho gayi, log aayenge" = hot key, "10-15 copy alag machine pe" = key ki replicas, "baant do" = read unme bantna.

**Maan lo Kohli ne "Inida" likh ke 10 second me "India" kar diya, kuch copies purana dikha rahi.**
Arpan: thodi der galat dikhega na. Peeche ek job chala do, ek ladka laga do, wo jaldi-jaldi sabko theek information pass kar dega. Kuch second lagenge. System rok nahi sakte, ye nahi keh sakte ki nahi dekhne denge.
Technical me: "thodi der galat" = eventual consistency, "peeche ek job" = background job copies update kare.

**6. Har account ka dabba memory me. Ravi ke dabbe me 3 saal ke tweet. 30 crore log mahino se app nahi khol rahe, par unke dabbe bhi roz bhar rahe. Memory ka bill aasmaan pe.**
Arpan: Parchi chhoti karo: bas last 500. Itna koi scroll nahi karta, karega bhi to DB se query karke Redis me rakhne me koi bada teer nahi lagta. 3 saal, 4 saal purana mote register me mat rakho, alag permanent diary me, jise aam bhasha me S3 kehte, archive. Aur jo 30 crore log aa hi nahi rahe: main unki parchi taiyaar kar raha, wo saale 10 din, 20 din, 30 din se aa nahi rahe, bekaar ki mehnat. Threshold: 30 din se app nahi khola to Redis me kuch taiyaar nahi. 40 din baad aaya to "thodi der ruk, 2 second me teri parchi la raha", DB se laake Redis me daal denge, thodi loading chalegi.
Technical me: "last 500" = inbox ki had, "DB se query karke Redis me" = cache miss pe DB, "permanent diary, S3, archive" = purana data S3, "30 din se nahi khola to kuch taiyaar nahi" = inactive user ka inbox nahi, "lautne pe DB se Redis me" = on-demand inbox banana.

**7. Saare tweet ek SQL DB me, din me 50 crore naye, likhne ki line itni lambi ki machine hile. Aur dabba banane ke liye har baar "Arpan ke naye 500" poori table me dhoondhna.**
Arpan: Likhai bahut zyada hai to alag machine, technical naam Cassandra, mera bade bhai. "Bhai Cassandra, mera system bahut likh raha, tu bada bhai hai, dekh le." Replica bhi wo kar leta. Kaun kisko follow kar raha, wo Cassandra me rakhne ka matlab nahi, uske liye ek aur bada bhai Neo4j: "ye aadmi isko follow kar raha, iska track rakhna."
Aur Arpan ke saare tweet ek jagah: naam se todo, par naam do logon ke same ho sakte (doosra Arpan), to id me baant do. Meri id har baar same, to mera saara data ek jagah.
Technical me: "likhai bahut, bada bhai" = Cassandra (write-heavy; andar ring = consistent hashing), "replica bhi wo kar leta" = Cassandra ki apni copy, "naam se nahi, id se baant do" = partition key = user id, "mera saara data ek jagah" = ek partition, "follow ka track" = graph DB Neo4j.

**8. Saare server America me. Ravi Delhi me, har request samundar paar, har kaam me aadha second upar se. IPL pe India ke 10 crore America ki machine hila rahe.**
Arpan: Ravi India se kar raha to har baar America kyun bhejna, bekaar ka travel. India me hai to baatcheet India me rakh. Ye bank jaisa hai: Lajpat Nagar ke log doosri branch me aa rahe, us bank pe dabaav. Lajpat Nagar me branch nahi hai? To wahan branch khol do, wahi chale jaayenge.
Technical me: "baatcheet India me rakh" = India region, "Lajpat Nagar me branch khol do" = user ke paas wala region.

**9. Arpan ne India branch me tweet kiya. Follower John New York me, America branch se baat karta, use pata hi nahi. Ulta Kohli America se tweet kare to Ravi ko nahi dikhta.**
Arpan: Do register hain, mera is wale se baat karta, John us wale se, par dono register ek-doosre ko nahi jaante. To unhe bhi baat karni padegi. Jab ye kuch likhe to apne register me to theek, aur us register pe bhi bhej do jahan se Arpan padhta. Jaise SBI ki do branch: dost ne Mumbai wali me 500 daale, main Delhi wali me jaake poochhta "mere liye kuch aaya?", branches aapas me baat karti hain to bata deti.
Technical me: "register aapas me baat karein" = cross-region replication, "us register pe bhi bhej do" = tweet doosre region ko copy.

**Maan lo dono branch ki baat 10 minute ruki aur Arpan ne beech me 5 tweet kiye.**
Arpan: ek banda bithao jo pal-pal dekhta rahe: "America wala dost aaya?" Nahi aaya, nahi aaya, ab aa gaya. "Le, Arpan ke 5 tweet, teri feed me push kar diye." Mujhe bas dekhna tha ki tu hai ya nahi, tu aa gaya to le tera kaam.
Technical me: "banda sambhal ke rakhe" = queue me pending, "pal-pal dekhta" = retry, "judte hi bhej" = link wapas aane pe replay.

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
Technical me: "kai machine, kaam baanto" = video ke hisse, transcoding workers (parallel), "id ki parchi" = har hissa video id + hissa number se.

**13. 10 worker, 9 ka hissa bana, worker 7 beech me gira; video na taiyaar na koi batata, "processing" ghanton se.**
Arpan: Har worker ka status. 9 ne bata diya, ek nahi bata paaya to dekhunga, heartbeat: "tu hai kya?" Jawab nahi to ise ghar bhejo, naya banda. Uske paas parchi: "maine yahan tak kar liya", naya wahin se uthaye. Save se pehle gira to shuru se.
Technical me: "har worker ka status" = DB me hisson ki ginti, "tu hai kya" = heartbeat, "naya banda" = job dobara (retry), "yahan tak parchi" = checkpoint.

**Maan lo worker 7 zinda hai, heartbeat aa rahi, par 1 ghante se ek hissa bana raha, baaki 2 minute me khatam.**
Arpan: Uski wajah se mera kaam ruk raha. "Tu thoda rest le, ye tera replacement, bata kahan tak chhoda, ye wahin se karke dega."
Technical me: "atka to replacement" = timeout pe slow worker ka hissa doosre ko, "kahan tak chhoda" = checkpoint.

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

**Maan lo tukde CDN se seedha jaate hain, CDN ko allow list pata hi nahi, aur link leak ho gaya.**
Arpan: CDN ke paas bhi taala-chaabi ho, har request US tak nahi jaayegi. In 10 ki user id do: ye 10 aayein to link do, nahi to mat do. Aur link ki umar 5 minute, uske baad kisi ke liye nahi khulega. Leak bhi hua to 11wa nahi ghusega. Mohar kaise jaanchi jaati, wo AWS ka kaam.
Technical me: "CDN ke paas bhi taala" = CDN signature jaanchta (signed URL / cookie), "10 user id" = server allow list dekh ke hi link deta, "5 minute umar" = short expiry. Hamara faisla: kisko aur kitni der ka.

**16. Ravi ke dabbe me sirf tweet ke number (#905, #902 ...). Screen pe chahiye kisne likha, photo, kitne like.**
Arpan: Mere paas ek mota register hai, sab likha hua. 1,00,000 page me dhoondhna time lega, main baawla thodi hoon. Par mere paas ID hain (905, 902), to seedha wahi page kholunga. Register 1 crore page ka ho ya 10 crore, ID ho to kaam aasaan. Photo / video CDN se aa jaayegi, baaki mote register se. Isme koi badi dikkat nahi.
Technical me: "ID se seedha page" = hydration (ID se poori post, key se lookup), "photo CDN se" = post me CDN link, "mota register" = tweet store (Cassandra), like count saath me.

**Maan lo Arpan ne tweet delete kar diya, par 8000 logon ke dabbe me uska number (ID) abhi bhi pada hai.**
Arpan: 8000 dabbe ek-ek karke saaf karna bhaari hai, zaroorat bhi nahi. Kadam 16 me screen pe dikhane se pehle hum ID se mote register ka page kholte hi hain. Page pe likha mile "ye tweet delete ho gaya", to bas use dikhao mat, chhod do. Dabbe me pada number apne aap purana hokar khisak jaayega.
Technical me: page pe deleted = tweet store me deleted flag (soft delete), dikhate waqt chhod do = hydration pe filter, 8000 dabbe saaf nahi karne padte.

**17. Ravi roz dost Amit ke tweet pe like / comment karta. Amit ka tweet 3 ghante pehle aaya, uske baad 40 aur, Amit ka neeche dab gaya; upar wo jinhe Ravi kholta bhi nahi.**
Arpan: Ye ek tarah recommendation hi hai. Peeche kaunsa algorithm / machine learning, wo design ka hissa nahi. Par main roz Amit ki photo like kar raha, baaki 40 ko bas dekh ke chhod deta. Instagram roz dekh raha ki ye ladka Amit ko like karta, to pehli photo Amit ki dikhayega. YouTube me bhi same: MrBeast ki video roz dekhun to wahi suggest karega. Unka kaam hi yahi: main jitni der rahun, unki kamaai badhe.
Technical me: "jisse zyada judta wo pehle" = ranking (kisse baat karta, post kitni nayi, likes), "algorithm design ka hissa nahi" = ranking model alag; hum list dete, ranked list lete. Netflix 6 ("tere liye") se juda.

**18. Ravi scroll kar raha, 20 dekhe, agle 20 maange. Beech me upar 5 naye aa gaye, agle page pe wahi 5 dobara.**
Arpan: 20 padhe, agle 20 maange, beech me upar 5 aa gaye, to neeche wale 5 khisak ke agle page me chale gaye; isliye wahi 5 dobara. Hal: Ravi ne kahan tak padha, thodi der ke liye yaad rakh lo. "Ravi ne yahan tak padh liya, ab iske baad wale do." Naye 5 upar hain, upar scroll karega to dikhenge. Refresh pe purani post dobara aa sakti hai, Instagram ka kaam hi engage rakhna, wo chalta hai; par interview me ye bataunga.
Technical me: "page number se agle 20" = offset pagination (isi se khisakta), "kahan tak padha yaad rakh" = cursor (har page ke saath aakhri dikhayi post lautao, agla uske theek baad se), "thodi der yaad" = ranked feed me ranked list thodi der save, cursor usi list me aage.

Poora design = [03_twitter_feed.md](03_twitter_feed.md)
