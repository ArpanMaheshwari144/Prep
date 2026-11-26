# Leaderboard: hand notes (1 crore khiladi ka game)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Ek game hai, 1 crore khiladi. Har match ke baad kisi ka score badalta hai. Roz ke match ke alawa game me LeetCode jaisa contest bhi hota hai (sab ek saath shuru, 5 level / sawaal, ant me contest ki apni rank). Screen pe teen cheez chahiye: top 10 abhi ke abhi, "tera rank 4,52,318", aur tere aage ke 5 aur peeche ke 5 khiladi.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
match khatam (+50, match id)  -->  server  -->  Redis sorted set (apne aap sort)  --clone-->  replica, DB sach
                                                   |
                         region ke dabbe (India / US / Europe), chhotu mila ke world board (scatter-gather)
                         aaj / hafta / all-time = teen alag dabbe (key me tareekh) · referee board = snapshot job

 1   1 crore sort kiye bina top 10 + rank        ->  apne aap sort wala dabba (Redis sorted set)
 1b  score pe index laga do                      ->  rank = ginna, har likhai pe index badle; write-heavy
 2   Ravi, Priya dono 9800                       ->  score phir time, ek number me (composite score)
 3   lakhon update ek Redis pe                   ->  region se baanto, chhotu mila ke (shard, scatter-gather)
 3b  match ka +50 do baar                        ->  match id se ek baar (idempotency)
 4   India me 3,000, duniya ka rank              ->  har line me mere upar kitne, jodo; poora board = snapshot job
 5   raat 12 baje aaj wala khaali                ->  key me tareekh, kal wala side me; teeno dabbe alag
 5b  11:59:58 ka match, score 12:00:02 pe        ->  khatam hone ka waqt, usi din ka register (event time)
 6   mere aage-peeche ke 5, alag dabbon me       ->  aas-paas scan, merge (score range)
 6b  world ke aas-paas 5 + 5                     ->  poora world board; UTC, sawaal, waqt, ms, naam
 6c  chalte contest me aage-peeche 10            ->  dabbe score range se, coordinator seedha sahi dabba
 6d  1 crore contest aur tez                     ->  availability > consistency; pehle pata kitne, jagah baanto
 7   Redis gira                                  ->  clone, clone, teeno gaye to DB se (replica, DB sach)
```

**FR / NFR kahan se:** upar ki dikkat chain dekh ke nikale; har line ke aage likha hai wo kis kadam se aayi.

**Arpan:** Score update FR hai. Top 10 FR nahi ho sakta, scoreboard aise kaam nahi karta: poochho top 10, 50 ya 100. "Mere aage-peeche ke 5" FR nahi, board sabke hisaab se banta, ye kisi ek user ka hai.

Aaj / hafta / all-time, UTC, world ya India ka board: ye sab FR se pehle sawaal hain, pehle requirement clear kar lo, phir uske hisaab se FR me daalo. Score barabar to pehle wala upar = NFR.

Rank milli second, score kuch second, Redis gira: inka yahan matlab nahi; asli rank to contest khatam hone ke baad hi pata chalti, beech me thodi purani chalegi.

*Technical me:* pehle sawaal = scope / requirement clarification, FR = score update + scoreboard (har user usi board me apni rank), NFR = tie-break + eventual consistency (contest ke beech).

**Arpan (FR pakka):** Top 10 FR nahi ho sakta; LeetCode me top 10 / top 15 jaisa koi feature nahi, wo rank pe chalta, top 10 pe koi inaam nahi. Top 10 feature to top 11, top 100 bhi feature, iska matlab nahi. Feature = scoreboard: sab usi board pe dikhte, apni rank bhi usi me.

*Technical me:* FR = score update + scoreboard (har user usi board me apni rank dekhe); "top kitne dikhane" sirf pehle sawaal (a), FR nahi.

**FR / NFR ki doosri kahani + dhaancha (round me) (Arpan):**
```
Pehla sawaal: world stage ya India?
Koi India nahi bolega, world hi hoga, to time zone UTC.
"Arpan ke upar ke 5, neeche ke 5" sawaal nahi banta; hum Arpan ya kisi ek ki nazar se nahi dekh rahe,
  LeetCode bhi aise nahi dekhta, board sabke score se banta, sab upar dekhte, apna bhi usi me aa jaata,
  design bhi us hisaab se nahi banta.

FR       contest aaya, Arpan ne contest diya, uski rank dikhani; weekly, biweekly contest, aur mahine / hafte me
         top pe kaun.

NFR      score barabar to time se, low latency, consistency (sabka score dikhe), crash na ho.

Dhaancha User app pe aaya, contest diya, LB, phir bahut saare server.
         Duniya bhar ka hai to paas me CDN (HTML / static file paas se).
         Local cache, phir Redis, phir DB AWS RDS, uski replica.
         DB me chahiye user id, naam, score, kaunsa contest: consistent data, JSON jaisa kuch nahi, isliye SQL /
         Postgres.
```

*Technical me:* dhaancha = CDN + LB + kai app server + local cache + Redis + RDS Postgres (primary + replica); "consistent, JSON nahi" = row wala data, SQL.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
PEHLE SAWAAL (FR se pehle interviewer se poochho, jawab se FR badle):
        a  top kitne dikhane: 10, 50 ya 100?
        b  board kis hisaab se: har contest (weekly, biweekly), mahina, all-time?  -> kadam 5, 5b
        c  board poori duniya ka ya ek desh (India) ka?                           -> kadam 3, 4, 6b
        d  contest hai to waqt kis time zone me (UTC)?                            -> kadam 5, 6b

FR      1  score update                                                     -> kadam 3
        2  scoreboard: sab ek board pe, har user usme apni rank dekhe     -> kadam 1, 4, 5

NFR     1  score barabar to jisne pehle banaya wo upar (contest me: pehle khatam wala, phir milli second, phir naam)  -> kadam 2, 6b
        2  board / rank thodi der purana chalega; asli rank contest khatam hone ke baad   -> kadam 4, 6c, 6d
        3  ek match ka score ek hi baar jude (match id)                    -> kadam 3b
Numbers 1 crore khiladi · har second lakhon match khatam -> likhai bahut · board dekhne wale usse bhi zyada
        -> har baar 1 crore sort / ginti nahi chal sakti
API     (Arpan: "API ki aisi koi zaroori baat nahi")
        GET  contests                 -> kaunse contest hain (weekly, biweekly)
        POST submit                   -> ek sawaal submit, uska score                  (FR score update)
        POST contest submit           -> poora contest submit
        GET  world board / world rank -> board aur apni rank                          (FR board)
```

&nbsp;

---

**1. Top 10 aur "tera rank" chahiye. SQL me har baar 1 crore row sort, aur rank ke liye "mere upar kitne" gino to 1 crore dekhne padte.**

**Arpan:** Ek aisa dabba jisme khiladi aur score daalte jao, wo apne aap sort karke de deta hai. Top 10 chahiye to 0 se 9 utha lo. Ye priority queue jaisa hi hai, bas wahan parent-child wala rishta hota, yahan poori line sorted hai, nikalo to wo de deta.

*Technical me:* "apne aap sort wala dabba" = Redis sorted set (har khiladi + score, kram me; daalna, top 10, rank sab log N). (Redis khud chhote se bade ke kram me rakhta; leaderboard me ulta (reverse) padhte, bade se chhote, taaki sabse zyada score upar.)
Jaankari (reel se): sorted set andar se skip list hai, highway jaisi (neeche sadak har gaon rukti, upar express lane sirf bade shehron pe), isliye 1 crore me bhi ~24 kadam.

**Maan lo (reel se) kisi ne kaha Redis kyun, SQL me score pe index laga do. Index laga diya, top 10 ab turant, par Arpan ka rank (45,23,118) abhi bhi 3 second leta hai.**

**Arpan:** 1 crore khiladi pe index kaise lagega: maan lo 5 lakh ka ek block, phir 5 lakh ka, phir 5 lakh ka.

Mujhe 1 crore me se 45 lakh wala rank dhoondhna hai; 5-5 lakh alag bhi kar diye to bhi har block me jaake dekhna hi padega, dhoondhne ka time badhega. Asli dikkat ye hai ki score badalta jaata hai, har second update ho raha.

Index lagaya to har likhai pe index bhi badlega, likhne aur update ka time har waqt badhta jaayega.

Index ka fayda ye ki query jaldi chalti, par jahan likhai zyada hai wahan index ka utna fayda nahi; log bahut hain aur real time me score dhadadhad update ho raha.

Isliye Redis sorted set: andar hi andar sort karta rehta, top 500 aur rank dono turant.

*Technical me:* "har block me jaake dekhna" = rank ke liye "mere upar kitne" gin-na (index se bhi count scan), "har likhai pe index badle" = write-heavy pe index ka bojh (har update pe index update), isliye sorted set (daalna, rank, top sab log N).

**Isi Maan lo ka doosra jawab:**

**Arpan:** Ek dabba rakh do jo apne aap sort kar de, sabko us dabbe se guzaar do, baar-baar query na chalani pade. Contest me ek saath 1 crore nahi aate, dheere-dheere: 3, 10, 20, 100, 1000. Postgres ko har baar bolo "10 sort kar, ab 100, ab 2000" to wo thak jaata. Isliye ek aisa dabba rakh lo jo tez sort karta hai, sabko usi dabbe se guzaar do.

*Technical me:* "apne aap sort wala dabba" = Redis sorted set (ek dabba jisme score daalte hi line apne aap kram me rehti).

---

**2. Ravi aur Priya dono ke 9800. Line me aage kaun?**

**Arpan:** LeetCode jaisa: pehle score dekho, score same ho to time dekho, jaise streams me thenComparing padha tha. Jisne ek second, ya 0.01 milli second, pehle submit dabaya wo upar: Ravi ne pehle khatam kiya to Ravi upar, Priya ek second bhi late to neeche. LeetCode andar kaunsa hisaab lagata public nahi; number jodo, ghatao ya baad me ulta kar do, wo andar ki baat.

*Technical me:* dabba har khiladi ke saath ek hi number leta, to "score phir time" ek number me pack: aage score, point ke baad time (jo pehle aaya uska hissa bada). Ravi 9800.95, Priya 9800.90.
Jaankari (reel se): time na jodo to score barabar hone pe Redis khud naam ke alphabet se kram lagata (Aditi pehle, Zoya baad me), pehle pahunchne wala nahi; isliye time jodte hain.

**Maan lo (Claude ka nitpick) pooche: time ko number bana ke score me jodo, to baad wale ka number bada; Ravi 10:00 = 9800 + 5 = 9805, Priya 10:05 = 9800 + 5.15 = 9805.15. Board pe sabse zyada score upar dikhana hai, to upar Priya aa jaayegi?**

**Arpan:** Ravi upar hi aayega, Priya kaise aa jaayegi. Wo number sirf misaal tha. Redis sorted rakhta hai 1, 2, 3, matlab sabse chhota upar. Hume jo upar chahiye us hisaab se ulta (reverse) kar do: sabse zyada score upar chahiye to reverse karke padho. Number jodo, ghatao ya reverse karo, dikkat kya hai; LeetCode andar kaunsa hisaab lagata wo public nahi. Baat itni: pehle score, barabar to time, pehle khatam wala upar.

*Technical me:* jo pehle aaya uska hissa bada rakho (ya time ko ulta karke jodo), tabhi bade-se-chhote padhne pe pehle wala upar = composite score; misaal ka number sirf misaal.

---

**3. Har second lakhon match khatam, saare score update ek hi Redis pe.**

**Arpan:** Ek Redis kyun? Alag-alag kar do, region ke hisaab se baant do: India ke, Europe ke, aise 10 line. Top 10 ke liye har line ka top 10 le lo, aur ek chhotu ko de do: "ye sab mila ke sort kar aur badi screen pe dikha de". 10 line ke top 10 = 100 log, unme se top 10.

*Technical me:* "baant do" = shard, "har line se le ke milao" = scatter-gather, "chhotu + badi screen" = aggregator jo global top 10 bana ke cache me rakhta.

**Aur neeche (kitne Redis chahiye, kaunse region, top 10 kaise jude):**

**Arpan:** 10 lakh register karte, aate maan lo 1 lakh, uske liye 1-2 Redis kaafi. World level pe region se baanto: EU alag, USA + Canada alag, Russia + Belarus alag, India alag, Australia + NZ alag; India me log zyada to India ka aur baant do. Top 10: saare dabbe sorted hain, har dabbe se top 10 lo, 5 dabbe = 50, jaise 5 priority queue me bikhre number, unme se duniya ke top 10.

*Technical me:* region se baanto = shard, har dabbe se lo = scatter-gather, 5 sorted list milana = merge k sorted (heap).

**Maan lo match ka +50 retry se do baar pahuncha.**

**Arpan:** ("ID jodna bas, sab me ID jodna; same ID to idempotency, server reject kar dega; bas chhota connect rakh.") Match id hi key: is match ka score pehle jud chuka to dobara mat jodo. +100 nahi, +50.

*Technical me:* idempotency key (wahi ID dobara aaye to chhodo).

**Aur neeche (parchi pe kya likha, dobara aayi to):**

**Arpan:** Parchi pe likho "Ravi, contest 003, +40". Parchi raste me kho gayi, server ne dobara bheji, phir purani bhi ghoom ke pahunchi, to dekho Ravi ka contest 003 wala score lag chuka, us parchi ko phaad ke phenk do.

*Technical me:* idempotency key = user id + contest id (UNIQUE).

**Isi Maan lo me aur neeche (server atka to? parchi pe likh lo):**

**Arpan:** Sawaal ki ID saath bhejo. Server atka to parchi pe likh le, baad me bhej de. Retry me wahi ID aayi to dobara nahi judega; score hamesha ek hi baar.

*Technical me:* sawaal ki ID = idempotency key, parchi pe likh ke baad me bhejo = outbox (hamare DB ki table, usi transaction me likho; chhotu / relay baad me bheje) + retry.

**Maan lo server parchi pe likhne se pehle hi gir gaya, likh hi nahi paaya.**

**Arpan:** Score aur parchi ek hi transaction me likh do, ek hi jagah. Dono likhe gaye ya koi nahi; aadha kabhi nahi hoga.

*Technical me:* score + outbox (hamare DB ki table) ki line ek hi DB transaction me (dono ya koi nahi), isliye "score likha par parchi nahi" ho hi nahi sakta.

---

**4. Main India wali line me 3,000 pe hoon. Screen pe poori duniya ka rank dikhana hai, aur lines 10 hain.**

**Arpan:** Do nazar hain. Race me bhaagne wala sirf apne aage wale dekhta, peeche wale nahi: mera rank = mere upar kitne log. Referee sabko dekhta, ek ko nahi: scoreboard sabka dikhana hai to wo kaam chhotu ka, sab mila ke.

*Technical me:* chhotu = background job (aggregator) · "mere upar kitne" = har shard se count (mere score se upar), sab jodo, +1. "Referee wala poora board" = background job jo har kuch minute me snapshot banata (thoda purana chalega).

**Arpan:** FR pe depend karta hai. Hum Arpan ke point of view se soch rahe the; Arpan ko shuru ke 5 aur neeche ke 5 nahi dikhane, hum leaderboard ki tarah soch rahe.

Leaderboard me world rank banana, to sabka dekhna, sirf Arpan ka nahi: US me Pablo ka bhi, yahan iska, wahan uska. Sabka kaise dikhega? Time se.

Upar se ranking dheere-dheere tay hoti aayegi; ho sakta Arpan ki rank thodi der galat lage, farak nahi padta. Leaderboard ka matlab yahi: thodi der galat, time poora hote-hote sab rank theek.

**Aur neeche (world rank turant kyun nahi niklegi):**

**Arpan:** World rank ekdum nahi niklegi, LeetCode bhi khatam hone ke baad thoda time leta. Pehle Arpan ko koi andaze wali rank dikhegi (maan lo 5000); jaise-jaise US, EU wale khatam karte, board pe judte, rank upar-neeche hoti, sab khatam hone ke 5-10 minute baad theek. Mujhe Arpan se lena-dena nahi, board sabka banta. LeetCode andar wo number kaise nikalta, mujhe nahi pata, aur galat nahi bolunga.

*Technical me:* andaze wali rank = estimated rank, sab region mila ke board = aggregator / global board, rank dheere theek = eventual consistency.

---

**5. Teen board chahiye: aaj ka, is hafte ka, all-time. Raat 12 baje aaj wala khaali ho ke naya din shuru.**

**Arpan:** Teeno ka kram alag hoga: aaj koi aur upar, hafte me koi aur, all-time me koi aur. Ek-ek ka score zero karne ka koi matlab nahi. Ek alag register me rakh lo, aur kal wala register side me rakh do, phekna nahi. Priya aaj nahi khelti: agar requirement hai ki naam dikhe to 0 ke saath dikha do, nahi hai to wo aaj ke register me hi nahi.

*Technical me:* match jeeta to score teeno dabbon me jodo. "Alag register" = har din ka alag dabba, naam me tareekh (board:5-Nov, board:week-45, board:all-time). "Kal wala side me" = live Redis se TTL se hat sakta, par board history me rehta; 80 lakh ko ek-ek chhuna nahi padta.

**Isi pe ek aur misaal (bank ki nayi kitaab):**

**Arpan:** Jaise bank me roz ka hisaab nayi kitaab me likhte aur bhari kitaab phenkte nahi, waise kal ka register side me rakh do, aaj naye pe likho.

*Technical me:* har din ki alag key, purani itihaas ke liye.

**Isi pe ek aur raasta (alag register ki jagah seedha overwrite):**

**Arpan:** Mita mat, overwrite kar de. Aaj Arpan ke 10, Priya ke 10. Kal Arpan aaya +10, Priya nahi aayi to uska 0. Ya to ek alag register me rakh ke purana poora register discard kar de, ya overwrite kar de; ek-ek ko zero karne nahi baithna.

*Technical me:* reset = naya key (din ki tareekh se) ya agla score aate hi overwrite; 80 lakh ko ek-ek zero karna nahi.

**Isi pe ek aur misaal (class ke 10 bachche):**

**Arpan:** Class me 10 bachche, kal sabke 10 the. Aaj test me kisi ke 10 aaye to usi waqt pichhla 10 minus karo, aaj ka 10 plus. Aisa nahi ki pehle saare bachchon ko line me laga ke zero karun, phir class me bithaun.

*Technical me:* lazy reset, sabko ek saath zero nahi.

**Aur neeche (game nahi, contest ho to board kaise):**

**Arpan:** Contest ka naam 437, sabke score DB me. Agla Sunday 438; 437 phenkna nahi, DB me pada. UI pe link: "437 ki rank yahan dekho, 438 me register yahan"; thodi der baad link 437 ke poore board pe le jaayega. Mahine ka board: Arpan ke har contest (Saturday, Sunday, Wednesday) ka score jod ke ek formula se rank; jo contest nahi diya uska +0, rank dheere-dheere theek.

*Technical me:* contest id se alag board (key me contest id), purana = DB me history, mahine wala = sab contest ka jod (aggregate).

**Maan lo Ravi ka match 11:59:58 pe khatam, score 12:00:02 pe pahuncha, server ne 9 Nov ke register me daal diya.**

**Arpan:** LeetCode jaisa: Arpan ne contest diya, main parchi pe sab likh deta hoon: contest 137, Arpan ki rank, aur kis waqt khatam hua.

Request server tak late aayi to koi baat nahi, parchi me sab likha hai: ye is din ka contest tha, itne baje khatam hua, to usi din ke register me daalo. Jo register kar ke aaye nahi, unka 0.

Beech me LeetCode crash bhi ho jaaye to parchi hamare paas hai, wapas aate hi mote register me daal do. Purana register phekna nahi, side me rakh do; Ravi ko 7 Nov ka dekhna hai to wahi register khol ke dikha do.

Agar Ravi aaj ka (10 tarikh ka) board khol ke baitha hai, to UI pe saaf likh do "ye 10 tarikh ka contest hai, kal wale ka result is link pe".

*Technical me:* "parchi pe contest id + khatam hone ka waqt" = event time, "usi din ke register me" = event time se bucket (pahunchne ke waqt se nahi), "purana register side me" = har din ka board history me rakho (discard nahi). "register kar ke aaye nahi" = contest ka sign-up; "mota register" = DB (kadam 7, score pehle DB me).

**Isi Maan lo me aur neeche (contest ka aakhri sawaal 12:59:59 pe, server tak 1:00:03, server gira to):**

**Arpan:** Parchi pe likh lo Arpan ne kis waqt khatam kiya. Server dheema / gira, wapas aaye to parchi se bhejo; ya doosre server pe bhejo. LeetCode crash hota tab bhi score kho nahi jaate, matlab kahin store karta. Bahut saare log ek saath to beech me Kafka, worker ek-ek karke uthaye (Kafka disk pe rakhta); itna data na ho to Kafka ka matlab nahi.

*Technical me:* parchi pe submit ka waqt = event time, gira to parchi se baad me bhejo = outbox (DB ki table), bheed me beech ka Kafka = buffer / queue (disk pe, worker = consumer).

**Maan lo contest raat 11 baje shuru, 1-3 baje khatam:**

**Arpan:** 9 tareekh raat 11 baje shuru hua, to hamare leaderboard pe wo contest 9 tareekh ka. Ye leaderboard duniya bhar ke online contest (LeetCode jaisa) ka hai, to ek hi time zone rakho, UTC jo aajkal standard hai; aisa nahi ki India ka IST, Europe ka alag. Ek room me bulaya elite contest (Gennady jaise top coder) alag cheez hai, wo yahan ki baat nahi.

*Technical me:* contest ka din = shuru hone ki tareekh (contest id ke saath); duniya bhar ka online contest = ek hi time zone UTC.

---

**6. Main 4,52,318 pe hoon, top 10 me kabhi nahi aaunga. Screen pe mere aage ke 5 aur peeche ke 5 chahiye, par wo alag-alag dabbon me baithe hain (Kenji Japan me, Sara Europe me).**

**Arpan:** Mera score 5200 hai, to mujhe 10,000 wale se kya lena-dena. Bheed me main apne aas-paas scan karta, poori bheed nahi dekhta. 5200 ke aas-paas wali range dekho; 4000 aur 6000 walon se matlab nahi. Jaise highway pe gaadi 300-350 km jaayegi to petrol pump usi area me dhoondhta hoon (yahi soch cache 4 me: key ring pe paas ke dabbe me).

*Technical me:* dabba = shard · "apne aas-paas scan" = har shard se mere score ke thoda upar aur thoda neeche wale (score range query), sab mila ke sort, mere upar ke 5 aur neeche ke 5.

**Isi pe ek aur raasta (shehar ke dabbe, pehle poochho kis dabbe me):**

**Arpan:** 10 crore ko ek saath rank karna mushkil hai, to shard me baant do: New York, California, Bangalore, Mumbai, Delhi. Mujhe 5200 ke aas-paas wale chahiye to pehle India wale se poochho 5000 wale kis dabbe me hain. Wo bolega Bangalore aur Mumbai, to sirf un dono me dekho.

*Technical me:* shard map / router, score range sirf milte shard pe.

**Maan lo dabbe region ke hisaab se: India, US, Europe. Is Sunday ke world contest me Arpan ki rank 50,000 (game wali 4,52,318 alag hai). World me uske upar ke 5 aur neeche ke 5 kaise?**

**Arpan:** Ye ek sawaal nahi, pehle tod ke dekho kya maanga. Jaisa FR me tay kiya, kaam POORA world leaderboard banana hai; Arpan ke aas-paas wale to isi board ka ek hissa, Arpan jo dekhega wahi.

**Arpan:** Contest duniya bhar me online hota (India, US, Europe se log), to sabka waqt ek time zone me, UTC. Bada bhai India, US, Europe sabke paas gaya, sabka result likh liya.

Pehle gino kisne kitne sawaal kiye: contest me 5 sawaal the, 50,000 me se 20,000 ne paanchon kiye, to pehle sirf unse khelo, unki rank pehle; baaki 30,000 (4, 3, 2 wale) ko baad me alag dekh lo, unki rank waise bhi neeche.

Ek jitne sawaal walon me jisne UTC me pehle khatam kiya wo upar; waqt bhi barabar to milli second (LeetCode bhi karta); wo bhi barabar to naam se. Result aate jaayein, line me judte jaayein.

Rank dheere-dheere theek hoti, Arpan ko thodi der 5,000 dikhe phir 50,000. Har baar region se "aas-paas" maangna bhaari, isliye ek board banao.

**Arpan:** Arpan ke upar ke 5 aur neeche ke 5 seedha tay nahi ho sakte, time se tay hote.

Ranking upar se dheere-dheere tay hoti aati; thodi der Arpan ki rank galat dikh sakti, farak nahi padta, LeetCode bhi time poora hone pe sab rank theek kar deta.

Happy New Year film ka board: jab tak time poora nahi, Team India neeche, jaise-jaise result aate Team India upar.

Jisne pehle poora kiya use upar lao, jo baad me khatam kare use neeche; board thodi der hilta, phir sab aa gaye to ek saath jud ke theek. Rassi jaisa: chhoti rassi baandhi, phir badi, phir aur, dheere-dheere apne aap lambi.

**Arpan:** Contest me "aage ke 5, peeche ke 5" ka alag feature ka matlab nahi.

Arpan ne khatam kiya to thodi der rank upar dikhegi, abhi baaki khatam kar rahe; US wala aaya to uska waqt Arpan se milao, pehle khatam kiya to upar, nahi to neeche. Rank hilti rahegi, ye sabko pata.

Contest poora hua to board bana, usme upar ke 5 aur neeche ke 5 apne aap dikh jaate, alag dikhane ki zaroorat nahi. Cheating pakadna FR ki baat, is design ka hissa nahi.

(Farak: kadam 6 game hai, chalta rehta hai aur har match pe score badalta, wahan aage-peeche 5 har waqt dikhana alag cheez hai.)

**Arpan (contest chalte waqt aage-peeche 10):** Dabbe score ki range se baanto, naam chahe shehar ke ho: Bangalore wala dabba = 10,000 wali range, Mumbai = 50,000, Delhi = 5,000.

Har desh ka ek coordinator har dabbe pe thappa lagata hai ki isme yahan se yahan tak ki range hai; US aur Europe ke liye bhi same.

Arpan ki range 5200 ke aas-paas, to coordinator se poocha, seedha Delhi (India), New York (US), Stockholm (Europe) wale dabbe mile; Bangalore, Mumbai ko poochha bhi nahi.

Unhi dabbon se upar ke 10 aur neeche ke 10 nikaal ke Arpan ko beech me rakh do.

Contest chal raha hai to ye andaze wali rank hai, galat hogi, sabko pata; rank sirf time se nahi, galat jawab / retry bhi ginte (Arpan ne pehle khatam kiya par 3 galat, doosre ne 1 minute baad bina galti, to wo upar).

Jaise-jaise sabka khatam hota, rank theek hoti rehti; asli rank contest poora hone pe.

*Technical me:* "dabbe score ki range se" = range partitioning (score range pe shard), "coordinator ka thappa" = shard map / router jo batata kaunsi range kis shard me, "seedha Delhi wale dabbe pe" = sirf matching shard pe range query (baaki shard skip), "andaze wali rank" = estimated rank, "galat jawab bhi ginte" = penalty (LeetCode: har galat submission pe 5 minute).

**Arpan (aur tez kaise):** Real time me consistency itni matter nahi, availability matter karti; sahi rank contest khatam hone se pehle ban hi nahi sakti, ye sabko pata.

Contest me pehle sign-up hota (interest dikhao, sign in, tab ghuso), to LeetCode ko pehle se pata: India se 50,000, US se 30,000, Europe se 20,000.

Usi hisaab se jagah baant do: India 1 se 50,000, US 1 se 30,000, Europe 1 se 20,000. Ye rank NAHI, bas bachche baantne ke liye, jaise URL me har server ko counter ki range.

Desh bada hai, sab ek jagah bharoge to dikkat, isliye desh ke andar IP se Bangalore, Delhi, New York me baanto, har desh ka coordinator.

Ek dabbe pe kai thappe ho sakte (Bangalore me 500 se 1,000, 10,000 se 20,000, 50,000 ke paas). Query ko 1 crore nahi chahiye, range se apne dabbe. Asli rank upar wala world board banata.

*Technical me:* "availability zyada, rank baad me theek" = AP + eventual consistency, "sign-up se pehle pata kitne aayenge" = capacity planning / pre-partition, "IP se desh, shehar" = jagah ke hisaab se baantna (geo partitioning; shehar IP / profile se), "ek dabbe pe kai thappe" = shard map me har shard ki kai score range, "ye rank nahi, bas baantna" = partition key, rank global board pe.

**Isi pe ek aur raasta (har region se range maango):**

**Arpan:** Ek raasta: bada bhai har region (India, US, Europe) se poochhe, "Arpan 50,000 pe hai, tere paas 49,500 se 50,500 me kaun hai?" Jiske paas upar wale hain wo upar wale de, jiske paas neeche wale wo neeche wale, sab mila ke Arpan ki screen pe. Par har user ke liye har baar har region se maangna bhaari, isliye doosra raasta behtar: ek world board, rank dheere-dheere theek ho.

*Technical me:* har shard pe range query + merge (scatter-gather); har request pe sab shard = bhaari, isliye global aggregator behtar.

**Aur neeche (range se baantne ka matlab saaf: 10 lakh me meri 5,000 kaise mile):**

**Arpan:** Maan le India se 10 lakh log. Meri rank 5,000 dekhni hai to 10 lakh me kahan dhoondhega.

Isliye data aisa baanto ki dhoondhna aasaan ho: map pe gole, Delhi ka gola (aas-paas ke Haryana, UP wale bhi Delhi ke dabbe me), Mumbai ka (Pune bhi), Bangalore, Chennai, Darjeeling, Gujarat ke gole. Desh kai tukdon me bat gaya.

Har dabbe pe range ka thappa: Delhi me is range se is range tak.

Mujhe Arpan ki 5,000 wali rank chahiye, upar se dekha kis dabbe me hai: Delhi pe likha 4,000 se 5,000, to seedha Delhi ke dabbe pe; Bangalore, Mumbai se lena-dena nahi. 10 lakh se kitna bach gaya.

(Pehle "IP" shabd bola tha, matlab tha jagah ke hisaab se gola.)

*Technical me:* "shehar ke gole" = geo partitioning (location se shard), "dabbe pe range ka thappa" = shard map (kis shard me kaunsi range), "seedha Delhi ke dabbe pe" = query sirf us ek shard pe, poore 10 lakh pe nahi.
Bolne ki line: "FR ke hisaab se poora world board. Contest ek time zone me (UTC). Pehle sawaal gino, paanchon wale pehle; ek jitne sawaal me jo pehle khatam kare wo upar, phir milli second, phir naam. Result aate jaayein, board banta jaaye, rank thodi der me theek."

*Technical me:* poora world board = global leaderboard (aggregator sab region ke result mila ke ek sorted board), "pehle sawaal gino, phir waqt, ms, naam" = sort key (solved desc, finish time asc, ms, naam) = composite score (kadam 2), "paanchon wale pehle, baaki baad me" = solved count ke hisaab se bucket, "rank dheere theek" = eventual consistency.

---

**7. Redis gira to saare score?**

**Arpan:** Ekdum DB pe nahi bhaagunga. Pehle koshish ki gire hi na; gira to uska clone, wo bhi gira to ek aur clone. Teeno gir gaye tab DB pe jaao, kyunki score pehle DB me likha hai, wahan se dobara bhar do.

*Technical me:* "clone" = Redis replica (gira to replica primary), "teeno gaye to DB" = DB sach, Redis wahan se rebuild.

**Aur neeche (kitni consistency chahiye, uske hisaab se kitni copy):**

**Arpan:** Ek Redis nahi, uski copy bhi. Peeche ek job Redis se DB me likhti rahe, contest ka data nahi khona. Kitni consistency chahiye us hisaab se: bilkul pakka to pehle parchi, phir Redis, phir 2-3 copy; thoda chhoot sakta to ek Redis + ek copy, gira to server copy se baat kar lega.

*Technical me:* copy = Redis replica, peeche DB me likhna = DB sach (Redis wahan se dobara), pehle parchi = log / outbox (DB ki table), kitni copy = consistency ka faisla.

&nbsp;

Poora design = [16_leaderboard.md](16_leaderboard.md)
