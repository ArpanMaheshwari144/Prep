# News aggregator: hand notes (Arpan News, Inshorts jaisi)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ne apni news app kholi, "Arpan News". Apna reporter koi nahi, khabar bahar ki 5000 site se aati hai.

&nbsp;

**1. NDTV, Times, Cricbuzz jaisi 5000 site kabhi bhi khabar daalti hain. Ravi 9 baje khole to 8:55 wali "India ne match jeeta" dikhe. Khabar aayegi kaise?**
Arpan: Ek dabba rakh lete hain jo har jagah se khinchega. Maine apne ladke se bola: chhotu, main in jagahon se news padhta hoon, saari khabar utha ke ek dabbe me daal de, main wahan se padh lunga.
Technical me: "chhotu" = fetcher (har site se khabar khinchne wala), "dabba" = DB.

**2. Chhotu akela, 5000 site, ek site pe 2 second = ek chakkar 3 ghante. 8:55 wali khabar 11 baje pahunchi.**
Arpan: Bahut saare chhotu rakh le. Aur khabar pehle se taiyaar rakh, ek chhoti parchi me, sab wahin se padh lein. Jaise akhbaar me saari khabar ek saath aati hai, jo achha lage padh lo.
Technical me: "bahut saare chhotu" = kai fetcher ek saath, "chhoti parchi" = Redis me taiyaar feed (cache).

**Maan lo chhotu 7 ke paas 100 site thi, aur wo beech me mar gaya.**
Arpan: Ye Twitter ke worker 7 jaisa hai. Har chhotu thodi-thodi der me bolta rahe "main zinda hoon". Kuch der tak awaaz nahi aayi to main samajh jaata hoon chhotu 7 gaya, aur uski 100 site baaki chhotuon me baant deta hoon. Is tarah koi site bina dekhe nahi chhoot-ti.
Technical me: "main zinda hoon" = heartbeat, "main site baant deta" = coordinator (chhotuon ko kaam baantne wala, load balancer nahi), kaam ka reassignment (lease khatam to doosra le).

**3. 50 chhotu. Ravi ki feed me ek khabar 40 baar: 2 chhotu ek hi Cricbuzz se laaye, 38 alag site se alag shabdon me.**
Arpan: Chhotu ke paas ek bada aadmi rakh: "ye dobara hai, hatao, pehle aa chuki". Par "India jeeta, Kohli ki century" aur "Kohli ki shaandaar century, India jeeta" me shabd alag hain, hubahu wala check nahi pakdega. To wordings dekh lo: dono me Kohli, century, India, jeeta hai, to same khabar, hata do; nahi to rakh lo.
Technical me: "bada aadmi" = dedup, "hubahu same" = URL / title ka hash, "wordings dekh lo" = shabd milaan (near-duplicate check).

**4. Bada aadmi akela, har khabar ko hafte ki 5 lakh khabron se milata, 1 minute ek khabar. Line lambi, 8:55 wali 10:30 pe.**
Arpan: Chhotu ko faltu khada mat kar. Khabar Kafka me daal (stadium wali kahani) aur chhotu apna kaam karta rahe. Peeche workers dupe hata ke feed / DB / Redis me rakhte rahenge. Aur har dupe nahi hatega, kabhi aa jaayega, jaise Instagram feed me bhi aata.
Technical me: "isme daal, chalta rahe" = Kafka (chhotu = producer), "peeche workers" = consumer group, "har dupe nahi hatega" = dedup best effort, trade-off maana.

**5. Ravi ko cricket, Priya ko tech, par sabki ek parchi. Aur 9 baje 1 crore log ek saath.**
Arpan: Khabar category ke hisaab se baant do, jise jo pasand wahi do. 1 crore ke liye bahut saare server, aur ek banda jo sabko manage kare: tere pe 10 load, tere pe 5, agli request tere paas.
Technical me: "category me baant" = har khabar pe category, user ki pasand DB me, "manage karne wala banda" = load balancer.

**6. Priya app kholti hai. Chhotu site pe jaata hai ya parchi se deta hai?**
Arpan: Parchi bhi chhotu hi banata. Naya banda aaya, pehle din pasand poochi: tech. To pehli baar DB se laao (thoda time chalega) aur parchi me rakh do. Roz tech hi dekh raha to wahi pehle se saamne rakho, pasand badle to DB se. Aur jis site se khabar la rahe wo band ho to 5 sec, 10 sec baad dobara try; na khule to chhodo, baaki site chalti rahengi.
Technical me: "pehli baar DB, phir parchi" = cache-aside, "5 sec, 10 sec baad dobara" = retry + backoff, ek site band = baaki pe asar nahi.

**7. Priya ki parchi 8 baje bani. 9:05 iPhone ki khabar DB me gayi. 9:10 Priya ko parchi se purani feed, iPhone nahi.**
Arpan: Nayi khabar aate hi tech walon ki feed me daal do. Aur milaan: har tech wale ki parchi me aaj ki badi khabar hai na? Chhoot gayi to DB se le ke seedha feed me.
Technical me: "aate hi feed me" = likhte waqt push, "milaan" = check job.

**8. 50 lakh tech wale, har ek ki apni parchi, har minute 20 khabar = 10 crore likhna. Workers peeche.**
Arpan: Worker pe faltu load kyun? Sab tech wale hain, sabko same khabar; Tesla launch hua to sabke liye hai, aisa nahi Arpan ke liye hai Ravi ke liye nahi. 50 lakh alag parchi bekaar. Saari tech khabar ek parchi me, app khule to wahin se.
Technical me: "ek tech parchi" = category ka ek feed (ek key), likhna ek baar, padhte waqt user ki pasand se.

**9. Tech parchi ek dabbe me. 9 baje iPhone launch, 10 lakh request / sec, dabba (maan lo) 1 lakh sambhalta. Cricket wale dabbe khaali.**
Arpan: Do tareeke: tech ke dabbe badha do, ya kal match nahi to cricket wale dabbon me bhi tech parchi daal do, aadhi request wahan se. World Cup hota to cricket dabba chahiye tha. Teeno aapas me baant lo.
Technical me: "dabbe badha do" = aur Redis node, par us pe bhi tech ki copy (ek key ek hi node pe rehti, akela node load nahi baant-ta), "cricket dabbe me tech ki copy" = garam key ki copies, load baant.

**10. Tech parchi 3 dabbon me. iPhone khabar sirf A me pahunchi. Priya ko A se dikhi, refresh pe B se gayi to gayab.**
Arpan: Garam khabar do-teen dabbon me daalo. A ke paas hai, B ke paas nahi, to B bhi rakhe. Do me likho, do se padho: A-B, A-C, B-C, koi bhi do.
Technical me: "do me likho, do se padho" = quorum (3 me W=2, R=2, kam se kam ek me taaza). Redis copies khud ye nahi karti; app do me likhe aur do se padhe (Cassandra jaisa DB ye khud karta).

**11. Worker ne khabar DB me likhi, parchi me daalne se pehle mar gaya. Doosra worker DB me dekh ke dupe samjha, hata di. Khabar kisi ko nahi dikhi.**
Arpan: Gadbad parchi se pehle hui. DB se pehle mara to dikkat nahi, doosra daal dega, dupe check + unique constraint hai. To ek status rakh: "DB me daali", "parchi me daali". Beech me gira to agla status dekh ke kaam aage badha dega. Ek tarah ki logging.
Technical me: "status" = khabar ka state (DB me -> feed me), agla worker adhoora kaam poora karta; "logging" = outbox / WAL wali soch.

&nbsp;

Poora design = [08_news_aggregator.md](08_news_aggregator.md)
