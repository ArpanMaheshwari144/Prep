# News aggregator: hand notes (Arpan News, Inshorts jaisi)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ne apni news app kholi, "Arpan News". Apna reporter koi nahi, khabar bahar ki 5000 site se aati hai.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
5000 site  -->  kai chhotu (fetcher)  -->  line (Kafka)  -->  worker / consumer: dupe hatao, DB me likho
                                                                    |
                                                                    v
                                       category ki ek parchi (Redis: tech, cricket ...), garam ho to 2-3 dabbe
                                                                    |
User  -->  LB  -->  server  -->  parchi se feed (pehli baar DB se)  <+

 1   5000 site, khabar aayegi kaise                ->  chhotu, sab utha ke dabbe me (fetcher, DB)
 2   chhotu akela, ek chakkar 3 ghante             ->  bahut saare chhotu + khabar pehle se parchi me (kai fetcher, Redis feed)
 2b  chhotu 7 ki 100 site, wo mara                 ->  "main zinda hoon" band to site baaki chhotuon me (heartbeat, coordinator)
 3   ek khabar 40 baar, shabd alag                 ->  bada aadmi "dobara hai, hatao", wordings dekh lo (dedup, near-duplicate)
 4   bada aadmi akela, 1 min ek khabar             ->  chhotu Kafka me daal ke chale, peeche workers (Kafka, consumer group)
 5   Ravi cricket, Priya tech, 9 baje 1 crore      ->  category me baanto + kai server, LB
 6   Priya pehli baar aayi / site band             ->  pehli baar DB phir parchi; site band to ruk ke dobara (cache-aside, retry + backoff)
 7   9:05 ki iPhone khabar purani parchi me nahi   ->  aate hi feed me + milaan (push on write, check job)
 8   50 lakh alag parchi, 10 crore likhai          ->  saari tech khabar ek parchi (category feed)
 9   iPhone launch, 10 lakh/sec, dabba 1 lakh      ->  dabbe badhao + khaali cricket dabbon me tech ki copy (hot key copies)
10   A me khabar, refresh pe B se gayab            ->  do me likho, do se padho (quorum)
11   DB me likhi, parchi se pehle worker mara      ->  status rakh, agla aage badhaaye (state, outbox)
12   6 mahine, table mota, purani koi nahi padhta  ->  FR me kab tak, raddi jaisa nikaal ke alag (retention, archival)
13   "Kohli century" pe 8 sec gola                  ->  Elasticsearch; jad nikaalo, 5 paper ek saath; peeche reh jaaye to sync / der / alert
```

**FR / NFR kahan se:** hawa se nahi aate. Upar ki dikkat chain dekh ke nikale; har FR / NFR line ke aage likha hai wo kis kadam ki dikkat se aayi.

**Arpan:** Theek hai, badhiya hai, yahi rehne do.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      1  5000 site se khabar laao                                   -> kadam 1, 2, 2b
        2  user ko taazi feed (uski category)                         -> kadam 5, 6, 7, 8
        3  ek khabar ek hi baar (best effort, kabhi dohra chalega)    -> kadam 3, 4
        4  login nahi to aam khabar, login hai to uski pasand ki      -> kadam 5
        5  khabar search kar sake ("Kohli century")                   -> kadam 13
        (pehle sawaal: purani khabar kab tak rakhni hai)              -> kadam 12

NFR     1  feed turant (~200 ms)                                      -> kadam 2, 6
        2  khabar 5-10 min se purani nahi                             -> kadam 2, 4, 7
        3  ek site band ho to baaki na ruke                           -> kadam 4, 6
        4  aayi khabar kho na jaaye (worker / consumer gira to bhi)   -> kadam 11
        5  refresh pe khabar gayab na ho                              -> kadam 10
        6  search thoda peeche chal sakta (kitni der, FR me tay)      -> kadam 13

Numbers 1 crore user x 5 baar = 5 crore roz, subah ~6000 / sec, badi khabar (iPhone launch) pe 10 lakh / sec (kadam 9)
        nayi khabar ~1 / sec -> padhna bahut zyada, likhna kam: padhne ka raasta tez, likhna peeche chal sakta
```

&nbsp;

---

**1. NDTV, Times, Cricbuzz jaisi 5000 site kabhi bhi khabar daalti hain. Ravi 9 baje khole to 8:55 wali "India ne match jeeta" dikhe. Khabar aayegi kaise?**

**Arpan:** Ek dabba rakh lete hain jo har jagah se khinchega. Maine apne ladke se bola: chhotu, main in jagahon se news padhta hoon, saari khabar utha ke ek dabbe me daal de, main wahan se padh lunga.

*Technical me:* "chhotu" = fetcher (har site se khabar khinchne wala), "dabba" = DB.

---

**2. Chhotu akela, 5000 site, ek site pe 2 second = ek chakkar 3 ghante. 8:55 wali khabar 11 baje pahunchi.**

**Arpan:** Bahut saare chhotu rakh le. Aur khabar pehle se taiyaar rakh, ek chhoti parchi me, sab wahin se padh lein. Jaise akhbaar me saari khabar ek saath aati hai, jo achha lage padh lo.

*Technical me:* "bahut saare chhotu" = kai fetcher ek saath, "chhoti parchi" = Redis me taiyaar feed (cache).

**Maan lo koi site band hai ya bahut dheemi:**

**Arpan:** Har site ka fetch parallel hoga, timeout hoga, aur har site ka circuit breaker: koi site fail ho gayi to circuit breaker hai hamare paas. Aur peeche queue hai jo backlog aur spike sambhal sakti hai.

*Technical me:* per-source timeout + circuit breaker (ek dheemi site baaki ko na roke), queue = backlog / spike buffer.
**Aur neeche (site wapas aayi to):**

**Arpan:** Site 10 minute baad wapas aayi to ekdam poora load nahi daalna; pehle thodi-thodi request, chal gayi to dheere-dheere poora. Tab tak doosre source se khabar le aao.

*Technical me:* circuit breaker half-open, dheere-dheere load badhao.

**DB kaunsa:** Arpan (21-Nov): News me consistency honi chahiye: khabar kahan se laa rahe, title, URL, category, time, ye sab theek chahiye. Isliye shuruaat Postgres (ya MySQL) se karunga, Cassandra ekdum se nahi ghusa sakta.

Kaunsa DB, iska jawab FR se milega: interviewer bole "likhai bahut hai, 50,000 fetcher, har fetcher 100-1000 khabar laa raha, har minute DB me update", tab write-heavy hai aur Cassandra ka sense banta.

Bole "padhai zyada, likhai me itni dikkat nahi", to Cassandra lunga hi nahi, Postgres se kaam chal jaayega. Badi companiyan dono rakh sakti: ACID wala bhi aur NoSQL bhi (key-value / JSON me khabar ka title, khabar).

*Technical me:* shuruaat = Postgres (row: title, url, category, time, consistency); FR me write-heavy aaye = Cassandra; dono saath = polyglot persistence.

**Maan lo chhotu 7 ke paas 100 site thi, aur wo beech me mar gaya.**

**Arpan:** Ye Twitter ke worker / consumer 7 jaisa hai. Har chhotu thodi-thodi der me bolta rahe "main zinda hoon". Kuch der tak awaaz nahi aayi to main samajh jaata hoon chhotu 7 gaya, aur uski 100 site baaki chhotuon me baant deta hoon. Is tarah koi site bina dekhe nahi chhoot-ti.

*Technical me:* "main zinda hoon" = heartbeat, "main site baant deta" = coordinator (chhotuon ko kaam baantne wala, load balancer nahi), kaam ka reassignment (lease khatam to doosra le).

**Maan lo chhotu 7 ka net 20 second atka, wo mara nahi tha, aur uski site turant chhotu 12 ko de di. Net aaya, ab dono wahi 100 site utha rahe.**

**Arpan:** Pehli baat, ekdam se chhotu 12 ko nahi de sakte. Thodi der intezaar karenge, net aaya to wapas chala jaayega. Chhotu 7 nahi aaya, khatam, chhotu 12 wahan se aage continue karega. Phir chhotu 7 aaye to wo replication wali baat hai: leader mar gaya to doosra leader, purana leader wapas zinda ho to wo dobara leader nahi banta. Same yahan.

*Technical me:* "thodi der intezaar" = lease / heartbeat timeout, "purana wapas aaye to malik nahi" = fencing (har naye malik ka bada number, jaise epoch; purane number ki baat nahi maani jaati).

---

**3. 50 chhotu. Ravi ki feed me ek khabar 40 baar: 2 chhotu ek hi Cricbuzz se laaye, 38 alag site se alag shabdon me.**

**Arpan:** Chhotu ke paas ek bada aadmi rakh: "ye dobara hai, hatao, pehle aa chuki". Par "India jeeta, Kohli ki century" aur "Kohli ki shaandaar century, India jeeta" me shabd alag hain, hubahu wala check nahi pakdega. To wordings dekh lo: dono me Kohli, century, India, jeeta hai, to same khabar, hata do; nahi to rakh lo.

*Technical me:* chhotu = fetcher · "bada aadmi" = dedup, "hubahu same" = URL / title ka hash, "wordings dekh lo" = shabd milaan (near-duplicate check).

**Isi dikkat ki doosri baat:**

**Arpan:** Ek hi khabar baar-baar repeat ho rahi to duplicate hata sakte, dedup: paanch source pe ek hi khabar ho to ek hi laao. Jaise Bloom filter use kar sakte, usse repetition nahi hogi; aur category me baant sakte, tag: sports, politics.

*Technical me:* Bloom filter = "ye pehle dekha?" ka tez jawab (thodi jagah me; kabhi-kabhi galat "haan", "nahi" hamesha sahi), category tag.

---

**4. Bada aadmi akela, har khabar ko hafte ki 5 lakh khabron se milata, 1 minute ek khabar. Line lambi, 8:55 wali 10:30 pe.**

**Arpan:** Chhotu ko faltu khada mat kar. Khabar Kafka me daal (stadium wali kahani) aur chhotu apna kaam karta rahe. Peeche workers / consumers dupe hata ke feed / DB / Redis me rakhte rahenge. Aur har dupe nahi hatega, kabhi aa jaayega, jaise Instagram feed me bhi aata.

*Technical me:* "isme daal, chalta rahe" = Kafka (chhotu = producer), "peeche workers" = consumer group, "har dupe nahi hatega" = dedup best effort, trade-off maana.

---

**5. Ravi ko cricket, Priya ko tech, par sabki ek parchi. Aur 9 baje 1 crore log ek saath.**

**Arpan:** Khabar category ke hisaab se baant do, jise jo pasand wahi do. 1 crore ke liye bahut saare server, aur ek banda jo sabko manage kare: tere pe 10 load, tere pe 5, agli request tere paas.

*Technical me:* "category me baant" = har khabar pe category, user ki pasand DB me, "manage karne wala banda" = load balancer.

**Aur neeche (chunav ke din sab ek hi khabar padh rahe):**

**Arpan:** Server badhao, aage LB. Yahan likhna kuch nahi, sab wahi result padh rahe, to primary pe hit bekaar, replica se padhwao.

*Technical me:* read replica.

**Maan lo Priya ne teen category chuni: tech, cricket, business. Use EK feed dikhani, upar sabse nayi.**

**Arpan:** Alag-alag teen parchi rakhne ka koi matlab nahi. Ek parchi pe sabki pasand likh lunga: Priya ko tech, business, cricket; Rahul ko ye teen.

Khabar aayi, maine apna algorithm chala ke chhaant liya: ye tech ki, ye cricket ki, ye business ki. Shabdon ki list hogi (iPhone, AI), word by word dekhte hain, to kabhi non-tech bhi chali jaayegi, hota bhi hai.

Khabar to sabki ek hi hai: AI launch hua to poori duniya ke liye, Rahul aur Priya ko alag khabar thodi.

Pasand dekh ke us category ki parchi se khabar (har user ki alag feed nahi, sabki ek category parchi, kadam 8), taaki app kholte hi turant dikhe; neeche scroll karo to apne aap load.

Login hai to pasand ke hisaab se, category chunne ka option; login nahi to bas normal khabar. Ye FR me likh sakte.

*Technical me:* "pasand ki ek parchi" = user preference store, "shabdon ki list se chhaanto" = keyword-based classification (category tag), "category ki parchi" = category feed (ek key, kadam 8; har user me push nahi), "neeche scroll pe load" = pagination / infinite scroll, "login nahi to normal khabar" = logged-out default / trending feed.

**Aur neeche (chhotu kya laaye):**

**Arpan:** Chhotu ko sirf pasand wali khabar nahi bolunga, wo saari laata; chhaantna peeche, phir pasand se feed.

*Technical me:* fetcher pasand nahi dekhta, category baad me (ingest ke baad classification).

---

**6. Priya app kholti hai. Chhotu site pe jaata hai ya parchi se deta hai?**

**Arpan:** Parchi worker / consumer banata (chhotu sirf khabar Kafka me daalta, kadam 4). Naya banda aaya, pehle din pasand poochi: tech. To pehli baar DB se laao (thoda time chalega) aur parchi me rakh do. Roz tech hi dekh raha to wahi pehle se saamne rakho, pasand badle to DB se. Aur jis site se khabar la rahe wo band ho to 5 sec, 10 sec baad dobara try; na khule to chhodo, baaki site chalti rahengi.

*Technical me:* chhotu = fetcher, parchi = Redis feed · "pehli baar DB, phir parchi" = cache-aside, "5 sec, 10 sec baad dobara" = retry + backoff, ek site band = baaki pe asar nahi.

**Aur neeche (parchi khaali aur lakhon log ek saath):**

**Arpan:** Sab DB pe nahi jaayenge: ek hi request DB jaaye, saari khabar la ke parchi me rakh de, baaki ruk ke parchi se le lein.

*Technical me:* cache stampede -> single flight / lock.

---

**7. Priya ki parchi 8 baje bani. 9:05 iPhone ki khabar DB me gayi. 9:10 Priya ko parchi se purani feed, iPhone nahi.**

**Arpan:** Nayi khabar aate hi tech walon ki feed me daal do. Aur milaan: har tech wale ki parchi me aaj ki badi khabar hai na? Chhoot gayi to DB se le ke seedha feed me.

*Technical me:* parchi = Redis feed · "aate hi feed me" = likhte waqt push, "milaan" = check job.

---

**8. 50 lakh tech wale, har ek ki apni parchi, har minute 20 khabar = 10 crore likhna. Workers / consumers peeche.**

**Arpan:** Worker / consumer pe faltu load kyun? Sab tech wale hain, sabko same khabar; Tesla launch hua to sabke liye hai, aisa nahi Arpan ke liye hai Ravi ke liye nahi. 50 lakh alag parchi bekaar. Saari tech khabar ek parchi me, app khule to wahin se.

*Technical me:* "ek tech parchi" = category ka ek feed (ek key), likhna ek baar, padhte waqt user ki pasand se.

---

**9. Tech parchi ek dabbe me. 9 baje iPhone launch, 10 lakh request / sec, dabba (maan lo) 1 lakh sambhalta. Cricket wale dabbe khaali.**

**Arpan:** Do tareeke: tech ke dabbe badha do, ya kal match nahi to cricket wale dabbon me bhi tech parchi daal do, aadhi request wahan se. World Cup hota to cricket dabba chahiye tha. Teeno aapas me baant lo.

*Technical me:* parchi = tech feed ki key, dabba = Redis node · "dabbe badha do" = aur Redis node, par us pe bhi tech ki copy (ek key ek hi node pe rehti, akela node load nahi baant-ta), "cricket dabbe me tech ki copy" = garam key ki copies, load baant.

---

**10. Tech parchi 3 dabbon me. iPhone khabar sirf A me pahunchi. Priya ko A se dikhi, refresh pe B se gayi to gayab.**

**Arpan:** Garam khabar do-teen dabbon me daalo. A ke paas hai, B ke paas nahi, to B bhi rakhe. Do me likho, do se padho: A-B, A-C, B-C, koi bhi do.

*Technical me:* "do me likho, do se padho" = quorum (3 me W=2, R=2, kam se kam ek me taaza). Redis copies khud ye nahi karti; app do me likhe aur do se padhe (Cassandra jaisa DB ye khud karta).

**Isi pe ek aur misaal (aunty aur teen bande):**

**Arpan:** Teen bande A, B, C. Khabar sirf A ko di, aunty B se poochh ke chali gayi, B ke paas thi hi nahi. A-B dono ko di to aunty C se poochh gayi. Isliye do me rakho, do se poocho: aunty kisi bhi do se poochhe, ek ke paas jawab zaroor.

*Technical me:* quorum, 3 me W = 2, R = 2.

---

**11. Worker / consumer ne khabar DB me likhi, parchi me daalne se pehle mar gaya. Doosra worker / consumer DB me dekh ke dupe samjha, hata di. Khabar kisi ko nahi dikhi.**

**Arpan:** Gadbad parchi se pehle hui. DB se pehle mara to dikkat nahi, doosra daal dega, dupe check + unique constraint hai. To ek status rakh: "DB me daali", "parchi me daali". Beech me gira to agla status dekh ke kaam aage badha dega. Ek tarah ki logging.

*Technical me:* parchi = Redis feed · "status" = khabar ka state (DB me -> feed me), agla worker / consumer adhoora kaam poora karta; "logging" = outbox wali soch (outbox = DB ki table, kaam ke saath usi transaction me likhi).

---

**12. App ko 6 mahine ho gaye. Roz 5000 site se ~1 khabar / sec, photo ke saath. "Khabar" table mota, disk ka bill har mahine badh raha, feed banate waqt itne bade table me chhaantna dheema. Par Ravi aur Priya 2-3 din se purani khabar kabhi kholte hi nahi.**

**Arpan:** Sabse pehle FR me tay karo ki purani khabar kab tak rahegi. Jaise main akhbaar padhta hoon, ghar me akhbaar jama hote rehte hain.

Aaj ka padh liya, wo dheere-dheere raddi ban jaata hai, phir raddi me bech dete hain, ghar me nahi rakhte. Same yahan: purani khabar mote register me rakhne ka matlab nahi.

Shard karna sense nahi banata: shard tab karo jab dono hisse ka data kaam me aa raha ho. Ek mahine purani khabar kaun dobara padhta, koi nahi. Koi bolega partition kar do, usse bhi fayda nahi; jab tak DB me padi hai, matlab nahi.

Ek saal purani khabar kaun padhega? Use mote register se nikaalo, panna nikaal ke alag rakh do.

*Technical me:* "FR me tay kab tak" = data retention policy, "raddi, register se nikaal ke alag" = archival (purana data DB se hata ke sasti jagah, jaise S3 / cold storage); jo data padha hi nahi jaata uska shard / partition bekaar.

---

**13. Ravi ne search me "Kohli century" likha. Server mote register me 7 din ki lakhon khabron ki poori likhai me ek-ek karke "Kohli" dhoondh raha, Ravi 8 second se gola dekh raha.**

**Arpan:** Main Elasticsearch add kar dunga. Elasticsearch apne aap me alag design hai. 7 din me sach me bahut khabar, Kohli ki dhoondhne me bahut time lagega; aisa nahi ki har din ka partition karo. Elasticsearch search index bana ke khud dhoondh lega. Andar kaise kaam karta wo Elasticsearch jaane, uska design hum nahi bana rahe.

**Aur neeche (ES kya deta):**

**Arpan:** Worker khabar likhte hi ES ka index bhi update kare. Search pe ES sirf milti khabar ke number deta, poori khabar register ya parchi se.

*Technical me:* ES doc id lautata, content DB / cache se, index async.

**Maan lo headline alag-alag shabdon me:**

**Arpan:** Koi "Kohli ne century maari", koi "aaj Kohli ki shatakiya paari". Hamari company khabar banati nahi, bahar se laati hai, aur sabki headline alag. To sabki jad nikaalo: Kohli century. Mere paas 10 paper aaye, 5 me almost wahi baat, likhne ka andaaz alag. Koi aaya "Kohli ki century?", maine kaha haan maari, ye le 5 paper, isme sab likha hai kab maari.

*Technical me:* Elasticsearch = alag search index (black box, PSP jaisa) (bana-banaya: AWS OpenSearch / Elastic Cloud; hum sirf sikhate kya aur kaise dhoondhna, Foundations 12); koi pooche to: har shabd ke saamne khabar ki list, jaise kitaab ke peeche ka index ("Kohli" -> khabar 3, 7, 9), isliye poori likhai nahi padhni padti (inverted index); "sabki jad" = stemming / synonyms (analyzer), "ek baat ke 5 paper ek saath" = milti-julti khabar ka group (clustering, kadam 3 jaisa).

**Maan lo headline me ekdum "Kohli century" likha, khabar 9:00 pe register me gayi, feed me turant dikhi. Ravi ne 9:00:20 pe search kiya, kuch nahi mila; 9:02 pe mil gayi.**

**Arpan:** Feed mote register se banti, isliye turant dikhi. Elasticsearch alag dabba hai, us tak abhi baat pahunchi nahi, isliye search me nahi aayi.

Do raaste: (1) register aur Elasticsearch ko sync me rakho, khabar aaye to register me likho aur Elasticsearch me bhi bhar do, dono ek saath.

(2) feed turant, Elasticsearch peeche-peeche, search me kuch second ki der; user 20 second ruk lega, dikkat nahi. Dono raaste sahi. Search kitna ruk sakta, FR me tay.

Sync job fail ho jaaye to alert lagao, failover karo ya dobara bharo.

**Arpan (phir, wahi baat khol ke):** Pehle mujhe laga Elasticsearch bas ek API service hai; diagram dekh ke pata chala ye alag dabba hai aur feed alag dabba, tab jawab apne aap aa gaya.

Do raaste: (1) pehle dono ko sync me rakho, phir feed dikhao; 30-40 sec me dono sync, to khabar khulte hi search me bhi turant.

(2) search pe gola ghuma do (user 10-15 sec ruk leta), peeche sync chalta rahe, sync hua aur result aa gaya. Kaunsa, ye requirement: user kitne second ruk sakta.

Elasticsearch ko hum batate hain kya aur kaise dhoondhna; andar kaise karta, wo Elasticsearch jaane.

*Technical me:* mota register = DB, alag dabba = Elasticsearch · "dono me ek saath bharo" = dual write, "peeche-peeche" = async indexing (eventual consistency), "kitni der chalegi, FR me" = search lag tay, "sync fail to alert" = alert + reindex.

&nbsp;

Poora design = [08_news_aggregator.md](08_news_aggregator.md)
