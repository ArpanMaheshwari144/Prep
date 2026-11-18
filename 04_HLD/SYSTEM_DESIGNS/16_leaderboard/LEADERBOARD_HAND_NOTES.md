# Leaderboard: hand notes (1 crore khiladi ka game)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Ek game hai, 1 crore khiladi. Har match ke baad kisi ka score badalta hai. Screen pe teen cheez chahiye: top 10 abhi ke abhi, "tera rank 4,52,318", aur tere aage ke 5 aur peeche ke 5 khiladi.

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
Arpan: Score update FR hai. Top 10 FR nahi ho sakta, scoreboard aise kaam nahi karta: poochho top 10, 50 ya 100. "Mera rank" aur "mere aage-peeche ke 5" FR nahi, board sabke hisaab se banta, ye kisi ek user ka hai. Aaj / hafta / all-time, UTC, world ya India ka board: ye sab FR se pehle sawaal hain, pehle requirement clear kar lo, phir uske hisaab se FR me daalo. Score barabar to pehle wala upar = NFR. Rank milli second, score kuch second, Redis gira: inka yahan matlab nahi; asli rank to contest khatam hone ke baad hi pata chalti, beech me thodi purani chalegi.
Technical me: pehle sawaal = scope / requirement clarification, FR = score update + top N, NFR = tie-break + eventual consistency (contest ke beech).

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
PEHLE SAWAAL (FR se pehle interviewer se poochho, jawab se FR badle):
        a  top kitne dikhane: 10, 50 ya 100?
        b  board kis hisaab se: aaj ka, hafte ka, all-time, ya teeno?             -> kadam 5, 5b
        c  board poori duniya ka ya ek desh (India) ka?                           -> kadam 3, 4, 6b
        d  contest hai to waqt kis time zone me (UTC)?                            -> kadam 6b
        e  kisi ek user ka rank / uske aage-peeche ke 5 chahiye?                  -> kadam 4, 6, 6c

FR      1  score update                                                     -> kadam 3, 3b
        2  top N ka board (N, board aur region upar ke sawaal se)           -> kadam 1, 4, 5

NFR     1  score barabar to pehle pahuncha wo upar (contest me: pehle khatam wala, phir milli second, phir naam)  -> kadam 2, 6b
        2  board / rank thodi der purana chalega; asli rank contest khatam hone ke baad   -> kadam 4, 6c, 6d
Numbers 1 crore khiladi · har second lakhon match khatam -> likhai bahut · board dekhne wale usse bhi zyada
        -> har baar 1 crore sort / ginti nahi chal sakti
```

&nbsp;

**1. Top 10 aur "tera rank" chahiye. SQL me har baar 1 crore row sort, aur rank ke liye "mere upar kitne" gino to 1 crore dekhne padte.**
Arpan: Ek aisa dabba jisme khiladi aur score daalte jao, wo apne aap sort karke de deta hai. Top 10 chahiye to 0 se 9 utha lo. Ye priority queue jaisa hi hai, bas wahan parent-child wala rishta hota, yahan poori line sorted hai, nikalo to wo de deta.
Technical me: "apne aap sort wala dabba" = Redis sorted set (har khiladi + score, kram me; daalna, top 10, rank sab log N).
Jaankari (reel se, 17-Nov): sorted set andar se skip list hai, highway jaisi (neeche sadak har gaon rukti, upar express lane sirf bade shehron pe), isliye 1 crore me bhi ~24 kadam.

**Isi dikkat ki doosri kahani (reel se): kisi ne kaha Redis kyun, SQL me score pe index laga do. Index laga diya, top 10 ab turant, par Arpan ka rank (45,23,118) abhi bhi 3 second leta hai.**
Arpan: 1 crore khiladi pe index kaise lagega: maan lo 5 lakh ka ek block, phir 5 lakh ka, phir 5 lakh ka. Mujhe 1 crore me se 45 lakh wala rank dhoondhna hai; 5-5 lakh alag bhi kar diye to bhi har block me jaake dekhna hi padega, dhoondhne ka time badhega. Asli dikkat ye hai ki score badalta jaata hai, har second update ho raha. Index lagaya to har likhai pe index bhi badlega, likhne aur update ka time har waqt badhta jaayega. Index ka fayda ye ki query jaldi chalti, par jahan likhai zyada hai wahan index ka utna fayda nahi; log bahut hain aur real time me score dhadadhad update ho raha. Isliye Redis sorted set: andar hi andar sort karta rehta, top 500 aur rank dono turant.
Technical me: "har block me jaake dekhna" = rank ke liye "mere upar kitne" gin-na (index se bhi count scan), "har likhai pe index badle" = write-heavy pe index ka bojh (har update pe index update), isliye sorted set (daalna, rank, top sab log N).

**2. Ravi aur Priya dono ke 9800. Line me aage kaun?**
Arpan: LeetCode jaisa: time se sort kar do. Jisne ek second, ya 0.01 milli second, pehle submit dabaya wo upar. Pehle score dekho, score same ho to time dekho, jaise streams me thenComparing padha tha.
Technical me: dabba har khiladi ke saath ek hi number leta, to "score phir time" ek number me pack: aage score, point ke baad time (jo pehle aaya uska hissa bada). Ravi 9800.95, Priya 9800.90.
Jaankari (reel se, 17-Nov): time na jodo to score barabar hone pe Redis khud naam ke alphabet se kram lagata (Aditi pehle, Zoya baad me), pehle pahunchne wala nahi; isliye time jodte hain.

**3. Har second lakhon match khatam, saare score update ek hi Redis pe.**
Arpan: Ek Redis kyun? Alag-alag kar do, region ke hisaab se baant do: India ke, Europe ke, aise 10 line. Top 10 ke liye har line ka top 10 le lo, aur ek chhotu ko de do: "ye sab mila ke sort kar aur badi screen pe dikha de". 10 line ke top 10 = 100 log, unme se top 10.
Technical me: "baant do" = shard, "har line se le ke milao" = scatter-gather, "chhotu + badi screen" = aggregator jo global top 10 bana ke cache me rakhta.

**Maan lo match ka +50 retry se do baar pahuncha.**
Arpan: Match id hi key: is match ka score pehle jud chuka to dobara mat jodo. +100 nahi, +50.
Technical me: idempotency key (wahi ID dobara aaye to chhodo).

**4. Main India wali line me 3,000 pe hoon. Screen pe poori duniya ka rank dikhana hai, aur lines 10 hain.**
Arpan: Do nazar hain. Race me bhaagne wala sirf apne aage wale dekhta, peeche wale nahi: mera rank = mere upar kitne log. Referee sabko dekhta, ek ko nahi: scoreboard sabka dikhana hai to wo kaam chhotu ka, sab mila ke.
Technical me: "mere upar kitne" = har shard se count (mere score se upar), sab jodo, +1. "Referee wala poora board" = background job jo har kuch minute me snapshot banata (thoda purana chalega).
Arpan: Khiladi alag region me hon to FR ke hisaab se poora world board (kadam 6 ke neeche) se: hum Arpan ki nazar se nahi, leaderboard ki nazar se soch rahe; sabka dekhna hai, US me Pablo ka bhi.

**5. Teen board chahiye: aaj ka, is hafte ka, all-time. Raat 12 baje aaj wala khaali ho ke naya din shuru.**
Arpan: Teeno ka kram alag hoga: aaj koi aur upar, hafte me koi aur, all-time me koi aur. Ek-ek ka score zero karne ka koi matlab nahi. Ek alag register me rakh lo, aur kal wala register side me rakh do, phekna nahi. Priya aaj nahi khelti: agar requirement hai ki naam dikhe to 0 ke saath dikha do, nahi hai to wo aaj ke register me hi nahi.
Technical me: match jeeta to score teeno dabbon me jodo. "Alag register" = har din ka alag dabba, naam me tareekh (board:5-Nov, board:week-45, board:all-time). "Kal wala side me" = live Redis se TTL se hat sakta, par board history me rehta; 80 lakh ko ek-ek chhuna nahi padta.

**Maan lo Ravi ka match 11:59:58 pe khatam, score 12:00:02 pe pahuncha, server ne 9 Nov ke register me daal diya.**
Arpan: LeetCode jaisa: Arpan ne contest diya, main parchi pe sab likh deta hoon: contest 137, Arpan ki rank, aur kis waqt khatam hua. Request server tak late aayi to koi baat nahi, parchi me sab likha hai: ye is din ka contest tha, itne baje khatam hua, to usi din ke register me daalo. Jo register kar ke aaye nahi, unka 0. Beech me LeetCode crash bhi ho jaaye to parchi hamare paas hai, wapas aate hi mote register me daal do. Purana register phekna nahi, side me rakh do; Ravi ko 7 Nov ka dekhna hai to wahi register khol ke dikha do. Agar Ravi aaj ka (10 tarikh ka) board khol ke baitha hai, to UI pe saaf likh do "ye 10 tarikh ka contest hai, kal wale ka result is link pe".
Technical me: "parchi pe contest id + khatam hone ka waqt" = event time, "usi din ke register me" = event time se bucket (pahunchne ke waqt se nahi), "purana register side me" = har din ka board history me rakho (discard nahi). "register kar ke aaye nahi" = contest ka sign-up; "mota register" = DB (kadam 7, score pehle DB me).

**6. Main 4,52,318 pe hoon, top 10 me kabhi nahi aaunga. Screen pe mere aage ke 5 aur peeche ke 5 chahiye, par wo alag-alag dabbon me baithe hain (Kenji Japan me, Sara Europe me).**
Arpan: Mera score 5200 hai, to mujhe 10,000 wale se kya lena-dena. Bheed me main apne aas-paas scan karta, poori bheed nahi dekhta. 5200 ke aas-paas wali range dekho; 4000 aur 6000 walon se matlab nahi. Jaise highway pe gaadi 300-350 km jaayegi to petrol pump usi area me dhoondhta hoon (yahi soch cache 4 me: key ring pe paas ke dabbe me).
Technical me: "apne aas-paas scan" = har shard se mere score ke thoda upar aur thoda neeche wale (score range query), sab mila ke sort, mere upar ke 5 aur neeche ke 5.

**Maan lo dabbe region ke hisaab se: India, US, Europe. Arpan world rank 50,000 pe. World me uske upar ke 5 aur neeche ke 5 kaise?**
Arpan: Ye ek sawaal nahi, pehle tod ke dekho kya maanga. Jaisa FR me tay kiya, kaam POORA world leaderboard banana hai; Arpan ke aas-paas wale to isi board ka ek hissa, Arpan jo dekhega wahi.
Arpan: Contest ek jagah hota (ICPC jaisa, US me ho ya Europe me), to sabka waqt ek time zone me, UTC. Bada bhai India, US, Europe sabke paas gaya, sabka result likh liya. Pehle gino kisne kitne sawaal kiye: contest me 5 sawaal the, 50,000 me se 20,000 ne paanchon kiye, to pehle sirf unse khelo, unki rank pehle; baaki 30,000 (4, 3, 2 wale) ko baad me alag dekh lo, unki rank waise bhi neeche. Ek jitne sawaal walon me jisne UTC me pehle khatam kiya wo upar; waqt bhi barabar to milli second (LeetCode bhi karta); wo bhi barabar to naam se. Result aate jaayein, line me judte jaayein. Rank dheere-dheere theek hoti, Arpan ko thodi der 5,000 dikhe phir 50,000. Har baar region se "aas-paas" maangna bhaari, isliye ek board banao.
Arpan: Arpan ke upar ke 5 aur neeche ke 5 seedha tay nahi ho sakte, time se tay hote. Ranking upar se dheere-dheere tay hoti aati; thodi der Arpan ki rank galat dikh sakti, farak nahi padta, LeetCode bhi time poora hone pe sab rank theek kar deta. Happy New Year film ka board: jab tak time poora nahi, Team India neeche, jaise-jaise result aate Team India upar. Jisne pehle poora kiya use upar lao, jo baad me khatam kare use neeche; board thodi der hilta, phir sab aa gaye to ek saath jud ke theek. Rassi jaisa: chhoti rassi baandhi, phir badi, phir aur, dheere-dheere apne aap lambi.
Arpan: Contest me "aage ke 5, peeche ke 5" ka alag feature ka matlab nahi. Arpan ne khatam kiya to thodi der rank upar dikhegi, abhi baaki khatam kar rahe; US wala aaya to uska waqt Arpan se milao, pehle khatam kiya to upar, nahi to neeche. Rank hilti rahegi, ye sabko pata. Contest poora hua to board bana, usme upar ke 5 aur neeche ke 5 apne aap dikh jaate, alag dikhane ki zaroorat nahi. Cheating pakadna FR ki baat, is design ka hissa nahi. (Farak: kadam 6 game hai, chalta rehta hai aur har match pe score badalta, wahan aage-peeche 5 har waqt dikhana alag cheez hai.)
Arpan (contest chalte waqt aage-peeche 10): Dabbe score ki range se baanto, naam chahe shehar ke ho: Bangalore wala dabba = 10,000 wali range, Mumbai = 50,000, Delhi = 5,000. Har desh ka ek coordinator har dabbe pe thappa lagata hai ki isme yahan se yahan tak ki range hai; US aur Europe ke liye bhi same. Arpan ki range 5200 ke aas-paas, to coordinator se poocha, seedha Delhi (India), New York (US), Stockholm (Europe) wale dabbe mile; Bangalore, Mumbai ko poochha bhi nahi. Unhi dabbon se upar ke 10 aur neeche ke 10 nikaal ke Arpan ko beech me rakh do. Contest chal raha hai to ye andaze wali rank hai, galat hogi, sabko pata; rank sirf time se nahi, galat jawab / retry bhi ginte (Arpan ne pehle khatam kiya par 3 galat, doosre ne 1 minute baad bina galti, to wo upar). Jaise-jaise sabka khatam hota, rank theek hoti rehti; asli rank contest poora hone pe.
Technical me: "dabbe score ki range se" = range partitioning (score range pe shard), "coordinator ka thappa" = shard map / router jo batata kaunsi range kis shard me, "seedha Delhi wale dabbe pe" = sirf matching shard pe range query (baaki shard skip), "andaze wali rank" = estimated rank, "galat jawab bhi ginte" = penalty (LeetCode: har galat submission pe 5 minute).
Arpan (aur tez kaise): Real time me consistency itni matter nahi, availability matter karti; sahi rank contest khatam hone se pehle ban hi nahi sakti, ye sabko pata. Contest me pehle sign-up hota (interest dikhao, sign in, tab ghuso), to LeetCode ko pehle se pata: India se 50,000, US se 30,000, Europe se 20,000. Usi hisaab se jagah baant do: India 1 se 50,000, US 1 se 30,000, Europe 1 se 20,000. Ye rank NAHI, bas bachche baantne ke liye, jaise URL me har server ko counter ki range. Desh bada hai, sab ek jagah bharoge to dikkat, isliye desh ke andar IP se Bangalore, Delhi, New York me baanto, har desh ka coordinator. Ek dabbe pe kai thappe ho sakte (Bangalore me 500 se 1,000, 10,000 se 20,000, 50,000 ke paas). Query ko 1 crore nahi chahiye, range se apne dabbe. Asli rank upar wala world board banata.
Technical me: "availability zyada, rank baad me theek" = AP + eventual consistency, "sign-up se pehle pata kitne aayenge" = capacity planning / pre-partition, "IP se desh, shehar" = jagah ke hisaab se baantna (geo partitioning; shehar IP / profile se), "ek dabbe pe kai thappe" = shard map me har shard ki kai score range, "ye rank nahi, bas baantna" = partition key, rank global board pe.
Isi dikkat ki doosri kahani (baantne ka matlab saaf): Arpan: Maan le India se 10 lakh log. Meri rank 5,000 dekhni hai to 10 lakh me kahan dhoondhega. Isliye data aisa baanto ki dhoondhna aasaan ho: map pe gole, Delhi ka gola (aas-paas ke Haryana, UP wale bhi Delhi ke dabbe me), Mumbai ka (Pune bhi), Bangalore, Chennai, Darjeeling, Gujarat ke gole. Desh kai tukdon me bat gaya. Har dabbe pe range ka thappa: Delhi me is range se is range tak. Mujhe Arpan ki 5,000 wali rank chahiye, upar se dekha kis dabbe me hai: Delhi pe likha 4,000 se 5,000, to seedha Delhi ke dabbe pe; Bangalore, Mumbai se lena-dena nahi. 10 lakh se kitna bach gaya. (Pehle "IP" shabd bola tha, matlab tha jagah ke hisaab se gola.)
Technical me: "shehar ke gole" = geo partitioning (location se shard), "dabbe pe range ka thappa" = shard map (kis shard me kaunsi range), "seedha Delhi ke dabbe pe" = query sirf us ek shard pe, poore 10 lakh pe nahi.
Bolne ki line: "FR ke hisaab se poora world board. Contest ek time zone me (UTC). Pehle sawaal gino, paanchon wale pehle; ek jitne sawaal me jo pehle khatam kare wo upar, phir milli second, phir naam. Result aate jaayein, board banta jaaye, rank thodi der me theek."
Technical me: poora world board = global leaderboard (aggregator sab region ke result mila ke ek sorted board), "pehle sawaal gino, phir waqt, ms, naam" = sort key (solved desc, finish time asc, ms, naam) = composite score (kadam 2), "paanchon wale pehle, baaki baad me" = solved count ke hisaab se bucket, "rank dheere theek" = eventual consistency.

**7. Redis gira to saare score?**
Arpan: Ekdum DB pe nahi bhaagunga. Pehle koshish ki gire hi na; gira to uska clone, wo bhi gira to ek aur clone. Teeno gir gaye tab DB pe jaao, kyunki score pehle DB me likha hai, wahan se dobara bhar do.
Technical me: "clone" = Redis replica (gira to replica primary), "teeno gaye to DB" = DB sach, Redis wahan se rebuild.

&nbsp;

Poora design = [16_leaderboard.md](16_leaderboard.md)
