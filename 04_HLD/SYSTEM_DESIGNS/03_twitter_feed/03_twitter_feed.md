# Twitter feed: Arpan, Ravi aur King Kohli ki kahani

Arpan ne tweet kiya "aaj ka mock 5/5". Ravi Arpan ko follow karta hai, aur King Kohli ko bhi (maan lo 27 crore followers).
Kaam: Ravi app khole to home feed pe sabke naye tweet, naye pehle, turant; Kohli ka tweet bhi, aur stadium se aaya
video bhi har net pe chale.

Har kadam = **kahani** (dikkat aur hal, bolne wali bhasha) -> **Asli naam** (interview me yahi shabd) -> tasveer
(kahani ka shabd, bracket me asli naam).

---

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
```

**FR / NFR kahan se:** upar ki dikkat chain dekh ke nikale; har line ke aage likha hai wo kis kadam se aayi.
Arpan: Twitter ko engagement ke hisaab se dekhna hai. Naya banda aaya, kisi ko follow nahi karta, to khaali page nahi dikhaunga; trending dikhaunga: aaj ye hua, Elon ne SpaceX me ye kiya, SpaceX aur Tesla ka merger. Instagram bhi yahi karta: main kisi ko follow nahi karta tha phir bhi bahut kuch aata; GTA 6 baar-baar search kiya to GTA 6 aane laga. Ravi 1000 logon ko follow karta, uski feed khatam nahi hoti, wahan scroll me dohrav nahi; refresh pe purana dobara aa jaaye to theek, Twitter iska hisaab nahi rakhta.
Technical me: "khaali page nahi, trending dikhao" = cold start / recommendation (follow ke alawa), "jo search kiya wahi aane laga" = interest se ranking, "scroll me dohrav nahi" = cursor (kadam 18).

## FR / NFR / Numbers (interview me pehle 2 minute)

```
FR      1  tweet post karo (text, photo / video)                               -> kadam 4, 10
        2  home feed: jinko follow karta unke tweet, jisse zyada judta wo upar -> kadam 1, 2, 3, 16, 17
        3  infinite scroll: ek scroll ke beech wahi tweet dobara na aaye
           (refresh pe aa sakta, Twitter iska hisaab nahi rakhta)              -> kadam 18
        4  follow wale khatam to neeche doosron ke tweet (trending),
           feed kabhi khaali nahi                                              -> (abhi kadam nahi)
        5  video har net pe chale (tukde, kai quality, beech se dekhna)       -> kadam 11-15
        6  search: interviewer maange to Elasticsearch jaisi alag cheez (Yelp jaisa); wo apne aap me alag
           duniya (Arpan 21-Nov)                                               -> (abhi kadam nahi)

NFR     1  feed ~200 ms me (40 din baad aaya to thodi der chalegi)            -> kadam 1, 6
        2  2-5 sec purani chalegi (isliye pehle se bana sakte)                -> kadam 4, 5b
        3  27 crore wale celeb ka tweet bhi sambhle                           -> kadam 2, 5
        4  duniya bhar ke user: paas ki branch se tez, door ka tweet bhi dikhe -> kadam 8, 9
Numbers ~50 crore user, 50 crore tweet / din = ~5,800 / sec · ek aadmi ~500 ko follow · shaam 8 baje 5 crore ek saath
        -> padhna : likhna ~50 : 1, isliye feed pehle se taiyaar rakho
```

---

## 1. Feed 6-7 second ghoome (apne paas rakhunga, apne paas se bhejunga)

Ravi app khole to server dhoondhe wo kis-kis ko follow karta (500 log), sabke tweet DB se nikaale, milaye, sort kare.
Shaam 8 baje 5 crore log ek saath, feed 6-7 second ghoome aur DB hil jaaye, kyunki baar-baar DB jaana padta. Mere 10
dost hain, roz apna hisaab maang rahe, to har baar unke paas kyun jaun jab roz yahi karna hai? Main kahunga: tum idhar
aao, apna mujhe de do, main apne paas rakhunga aur apne paas se bhej dunga. Unhone app khola, unhe turant mila, mote
register me nahi jaana pada, dobara DB hit nahi.

**Asli naam:** apne paas rakhunga, apne paas se bhej dunga = har user ka pehle se bana inbox (Redis sorted set, naya
upar), dost apna mujhe de do = tweet aate hi followers ke inbox me (fanout on write), dobara DB hit nahi = feed seedha
inbox se.

**Maan lo Ravi ne aaj Amit ko follow kiya, Amit ke pichhle hafte ke 20 tweet Ravi ke dabbe me kabhi gaye hi nahi:** Main Twitter ki service pe baitha hoon, dekha Ravi ne abhi Amit ko follow kiya. Amit ne jo pehle daala hai wo Ravi ki feed me seedha push kar do, jaise Instagram pe follow karte hi purani reels dikhti. Aage Amit jo naya daalega wo waise hi chala jaayega.
**Asli naam:** "follow karte hi purane push" = backfill on follow, "naya waise hi" = fanout on write (kadam 1).

**Maan lo Ravi ne Arpan ko unfollow kiya, par dabbe me Arpan ke 50 purane tweet pade hain:** Feed sirf follow walon se nahi banti: main khud Elon, Sachin, Kohli ko follow nahi karta, phir bhi mujhe unke tweet dikhte, kyunki Twitter / Instagram ka kaam hi engage rakhna hai; kuch follow na karo to bhi khaali page nahi dikhta. Unfollow ke baad pade hue tweet turant gayab nahi honge, par naye aane band, aur jaise-jaise feed banegi Arpan ke tweet dheere-dheere kam ho jaayenge.
**Asli naam:** "turant gayab nahi, dheere kam" = dabba saaf nahi karte (lazy, purge nahi), naye fanout band, ranking (kadam 17) neeche karti; feed me follow ke alawa recommendation bhi.

**Maan lo kai server, request kabhi kisi pe:** app stateless, har server yaad nahi rakhega, pehchaan JWT se.
**Asli naam:** stateless app server + JWT.

```mermaid
flowchart TB
    X["Har baar 500 logon ke tweet<br/>DB se: feed 6-7 second"]:::bad
    A["Dost: apna mujhe de do<br/>(tweet aate hi)"]:::card
    subgraph AP["APNE PAAS RAKHUNGA"]
        IN[("Ravi ka dabba<br/>(Redis inbox)")]:::pill
    end
    R["Ravi ne app khola:<br/>turant, dobara DB nahi"]:::good
    X -. "pehle" .-> IN
    A --> IN
    IN -- "apne paas se bhej dunga" --> R
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style AP fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 2. Kohli ke 27 crore dabbe (Kohli ki ek chhoti parchi)

Kohli ne tweet kiya "Century for India". 27 crore dabbon me daalna, ghanton lagte, Ravi ko 2 ghante baad dikha, match
khatam, aur tab tak baaki sabke tweet bhi line me peeche atke. Agli baar 28 crore, 29 crore honge, dabbe badhte
rahenge, dikkat nahi rukegi. 100-200, 2,000-3,000 ke liye dabba theek hai, par 27 crore bahut bada number hai,
daalte-daalte system thak jaayega. To Kohli ne century banayi aur likh di, maine ek chhoti parchi pe likh di aur yahan laga
di. Na DB, na kuch, 27 crore yahin se padh lenge.

**Asli naam:** 27 crore dabbe = fanout on write (celeb pe nahi chalta), 100-200 ke liye theek, 27 crore ke liye nahi =
followers ki had se do raaste, ek chhoti parchi, sab yahin se padhein = celeb ka tweet uski apni list me, feed khulne
pe wahan se (fanout on read). Dono saath = hybrid fanout.

```mermaid
flowchart TB
    K["Kohli: Century for India"]:::card
    X["27 crore dabbon me daalna:<br/>ghanton, system thak jaayega"]:::bad
    subgraph CP["KOHLI KI EK CHHOTI PARCHI"]
        P[("Na DB, na kuch:<br/>27 crore yahin se padh lenge")]:::pill
    end
    N["100-200, 2,000-3,000 wale"]:::card
    D["Seedha unke followers ke dabbe me:<br/>theek (fanout on write)"]:::good
    K -. "nahi" .-> X
    K --> P
    N --> D
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style CP fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 3. Dabba alag, 15 bade logon ki parchiyan alag (sab ek saath lao, sort karo)

Ravi ke dabbe me doston ke tweet hain, par Kohli, Dhoni, SRK, aise 15 bade logon ki parchiyan alag hain, aur Ravi ko
ek hi feed chahiye, sab apne time ke kram me. To sabko ek saath lao. Ek parchi pe likh diya: ye Dhoni ka, ye Kohli
ka, ye Arpan ka. Ab sabko sort karte hain, jaise DSA me sort karte, aur Ravi ki feed pe push. Ravi ne app khola: "le,
ye tera aaj ka poora twitter ka samachar, jinko tune follow kar rakha tha, sort kar diya hai, dekh le."

**Asli naam:** sabko ek saath lao, ek parchi pe = inbox + follow kiye celeb ki list, DSA jaisa sort = time se merge,
Ravi ki feed pe push = merge at read.

```mermaid
flowchart TB
    IN[("Doston ka dabba")]:::card
    P[("15 bade logon ki parchiyan<br/>(Kohli, Dhoni, SRK)")]:::card
    subgraph EK["SABKO EK SAATH LAO"]
        M(["Ek parchi pe sab,<br/>DSA jaisa sort<br/>(merge at read)"]):::pill
    end
    R["Le, ye tera aaj ka poora<br/>twitter ka samachar"]:::good
    IN --> M
    P --> M
    M --> R
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style EK fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 4. Arpan ke 8,000 followers, "Post" ghoomta rahe (threshold + stadium wala bhai)

Arpan ke 8,000 followers, abhi bhi chhota user. "Post" dabaya, server ek-ek karke 8,000 dabbe bhar raha, button
ghoomta rahe. 3,000 pe server gira, baaki 5,000 ko kabhi nahi mila, aur Arpan ko pata bhi nahi. Sabse pehle ek
threshold rakho: itne followers paar gaye to inbox me push nahi, Kohli wala tareeka. Aur bheed sambhalne ke liye ek
dabba laga do, mera favourite stadium wala bhai Kafka. Maine post dabaya to "tera post ho gaya, nikal", peeche Kafka
sab karta rahega. Kuch gira to outbox me rakh lo, ek parchi pe likha hai.

**Asli naam:** threshold, Kohli wala tareeka = had ke upar celeb raasta, stadium wala bhai = Kafka (async fanout, peeche fanout workers / consumers),
post ho gaya, nikal = turant OK, parchi pe likha = outbox ki soch. Yahan tweet Cassandra me jaata, aur wahan tweet +
outbox ek transaction me nahi likhe jaate, isliye Kafka hi pakki parchi: Tweet Service pehle Kafka me daale
(acks=all), peeche workers / consumers tweet Cassandra me aur inbox me likhein (tweet_id se idempotent, dobara aaye to do baar nahi).

```mermaid
flowchart TB
    A["Arpan: Post<br/>(8,000 followers)"]:::card
    T{"Threshold paar?<br/>Kohli wala tareeka"}:::card
    subgraph SB["STADIUM WALA BHAI"]
        K[("Kafka")]:::pill
    end
    OK["Tera post ho gaya, nikal"]:::good
    W["Peeche dabbe bharte rahe"]:::card
    C["Kohli wala tareeka:<br/>apni parchi, feed khulne pe padho"]:::card
    A --> T
    T -- "nahi" --> K
    T -- "haan" --> C
    K --> OK
    K --> W
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style SB fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 5. Six ke baad 5 crore ek parchi pe (parchi ki 10-15 copy)

Kohli ne 99 se six maara aur tweet kiya. 1 minute me 5 crore log refresh, sabko wahi ek parchi chahiye. Wo parchi ek
machine pe, wo machine hil gayi, aur Dhoni-SRK ki parchiyan bhi atki. Us waqt wo cheez famous ho gayi to log aayenge
hi. Main dekh raha lakhon log is parchi ko padhne aa rahe, to iski 10-15 copy bana dunga, alag-alag machine pe: le tu
bhi ye copy le, sabko padhwa. Bheed badh rahi to baant do, copiyan bana do, clone kar do.

**Asli naam:** famous ho gayi, log aayenge = hot key, 10-15 copy alag machine pe = key ki replicas, baant do = read
unme bant-ta.

**Maan lo Kohli ne "Inida" likh ke 10 second me "India" kar diya, aur kuch copies purana dikha rahi:** thodi der galat
dikhega na. Peeche ek job chala do, ek ladka laga do, wo jaldi-jaldi sabko theek information pass kar dega, kuch second
lagenge. System rok nahi sakte, ye nahi keh sakte ki nahi dekhne denge.
**Asli naam:** thodi der galat = eventual consistency, peeche ek job = background job copies update kare.

```mermaid
flowchart TB
    U["Six ke baad 5 crore refresh,<br/>sabko wahi ek parchi"]:::bad
    subgraph CL["10-15 COPY, ALAG MACHINE (clone kar do)"]
        C1[("Copy 1")]:::good
        C2[("Copy 2")]:::good
        CN[("... Copy 15")]:::good
    end
    L(["Inida se India: peeche ek ladka<br/>sab copy theek kare (kuch second)"]):::pill
    U --> C1
    U --> C2
    U --> CN
    L -.-> CL
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style CL fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 6. Memory ka bill aasmaan pe (aakhri 500 + 30 din wala)

Har account ka dabba memory me, Ravi ke dabbe me 3 saal ke tweet, aur 30 crore log mahino se app nahi khol rahe, par
unke dabbe bhi roz bhar rahe. To parchi chhoti karo: bas last 500. Itna koi scroll nahi karta, karega bhi to DB se
query karke Redis me rakhne me koi bada teer nahi lagta. 3-4 saal purana mote register me mat rakho, alag permanent
diary me, jise aam bhasha me S3 kehte, archive. Aur jo 30 crore log aa hi nahi rahe, unki parchi taiyaar karna bekaar
ki mehnat, wo 10 din, 20 din, 30 din se aa nahi rahe. Threshold: 30 din se app nahi khola to Redis me kuch taiyaar
nahi. 40 din baad aaya to "thodi der ruk, 2 second me teri parchi la raha", DB se laake Redis me daal denge, thodi
loading chalegi.

**Asli naam:** last 500 = inbox ki had (trim), DB se query karke Redis me = cache miss pe DB, permanent diary, S3,
archive = purana data S3, 30 din se nahi khola to kuch taiyaar nahi = inactive user ka inbox nahi, lautne pe DB se
Redis me = on-demand inbox banana.

```mermaid
flowchart TB
    subgraph PC["PARCHI CHHOTI KARO"]
        IN[("Bas last 500<br/>(Redis)")]:::pill
        DB[("Mota register<br/>(DB)")]:::card
        S3[("3-4 saal purana: permanent diary<br/>(S3, archive)")]:::card
    end
    X["30 din se app nahi khola:<br/>kuch taiyaar nahi, aaye to<br/>2 second, DB se la ke"]:::bad
    IN -- "500 se purane" --> DB
    DB -- "3-4 saal purane" --> S3
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style PC fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 7. Din me 50 crore tweet, SQL hil gaya (bada bhai Cassandra + follow ka Neo4j)

Saare tweet ek SQL DB me, din me 50 crore naye, likhne ki line itni lambi ki machine hile. Aur dabba banane ke liye har
baar "Arpan ke naye 500" poori table me dhoondhna. Likhai bahut zyada hai to alag machine, mera bada bhai Cassandra:
"bhai Cassandra, mera system bahut likh raha, tu bada bhai hai, dekh le." Replica bhi wo khud kar leta. Kaun kisko
follow kar raha, wo Cassandra me rakhne ka matlab nahi, uske liye ek aur bada bhai Neo4j: "ye aadmi isko follow kar
raha, iska track rakhna." Aur Arpan ke saare tweet ek jagah chahiye: naam se todo to doosra Arpan bhi ho sakta, isliye
id se baanto. Meri id har baar same, to mera saara data ek jagah.

**Asli naam:** likhai bahut, bada bhai = Cassandra (write-heavy, na join na transaction chahiye, khud kai machine me baant-ta, andar ring = consistent hashing: nayi machine jude to sirf paas ka hissa hile), replica bhi wo kar leta = Cassandra ki apni copy,
naam se nahi, id se baant do = partition key = user id, mera saara data ek jagah = ek partition (author id se hissa, andar tweet id se tweet, time ke kram me),
follow ka track = graph DB Neo4j.

**Cassandra kyun (doosri baat):** writes bhi zyada, reads bhi zyada, TB data; shard user ID se. **Maan lo har baar "Arpan
ke naye 500" poori table me:** roz follower wahi maangta (per day pattern), to author ke naye tweet thodi der apne paas rakh lo.
**Asli naam:** write + read heavy = Cassandra (partition = user id); recent tweets cache (Redis, TTL).

```mermaid
flowchart TB
    X["Din me 50 crore tweet,<br/>SQL ki likhne ki line lambi"]:::bad
    subgraph BB["DO BADE BHAI"]
        C[("Cassandra: likhai dekh le,<br/>id se baanto, replica khud")]:::pill
        G[("Neo4j: kaun kisko follow<br/>kar raha, iska track")]:::pill
    end
    X --> C
    X --> G
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style BB fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 8. Server America me, Ravi Delhi me (Lajpat Nagar me branch)

Saare server America me, Ravi Delhi me, har request samundar paar, har kaam me aadha second upar se, aur IPL pe India
ke 10 crore America ki machine hila rahe. Ravi India se kar raha to har baar America kyun bhejna, bekaar ka travel.
India me hai to baatcheet India me rakh. Ye bank jaisa hai: Lajpat Nagar ke log doosri branch me aa rahe, us bank pe
dabaav. Lajpat Nagar me branch nahi hai? To wahan branch khol do, wo wahi chale jaayenge.

**Asli naam:** baatcheet India me rakh = India region, Lajpat Nagar me branch khol do = user ke paas wala region.

```mermaid
flowchart TB
    R["Ravi Delhi me,<br/>har request samundar paar"]:::bad
    subgraph LN["LAJPAT NAGAR ME BRANCH KHOL DO"]
        IN(["India branch<br/>(baatcheet India me)"]):::pill
    end
    US["America branch"]:::card
    R --> IN
    R -. "bekaar ka travel" .-> US
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style LN fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 9. John ko Arpan ka tweet nahi dikhta (branches aapas me baat karein)

Arpan ne India branch me tweet kiya. Follower John New York me America branch se baat karta, use pata hi nahi. Ulta
Kohli America se tweet kare to Ravi ko nahi dikhta. Do register hain, mera is wale se baat karta, John us wale se, par
dono register ek-doosre ko nahi jaante. To unhe bhi baat karni padegi: jab ye kuch likhe to apne register me to theek,
aur us register pe bhi bhej do jahan se Arpan padhta. Jaise SBI ki do branch: dost ne Mumbai wali me 500 daale, main
Delhi wali me jaake poochhta "mere liye kuch aaya?", branches aapas me baat karti hain to bata deti.

**Asli naam:** register aapas me baat karein = cross-region replication, us register pe bhi bhej do = tweet doosre
region ko copy.

**Maan lo dono branch ki baat 10 minute ruki aur Arpan ne beech me 5 tweet kiye:** ek banda bithao jo pal-pal dekhta
rahe: "America wala dost aaya?" Nahi aaya, nahi aaya, ab aa gaya. "Le, Arpan ke 5 tweet, teri feed me push kar diye."
Mujhe bas dekhna tha ki tu hai ya nahi, tu aa gaya to le tera kaam.
**Asli naam:** tweet tab tak rakhe rehna = queue me pending, pal-pal dekhta = retry, aa gaya to push = link wapas aane pe
replay.

```mermaid
flowchart TB
    A["Arpan ka tweet<br/>(India branch)"]:::card
    subgraph BB["BRANCHES AAPAS ME BAAT"]
        B(["Us register pe bhi bhej do<br/>jahan se John padhta"]):::pill
        P(["Baat ruki? Pal-pal dekhne wala<br/>banda: aa gaya to 5 tweet push"]):::pill
    end
    US["America branch:<br/>John ko dikha"]:::good
    A --> B
    B --> US
    P -.-> US
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style BB fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

---

**Video: Arpan stadium se King Kohli ke six ka 1 GB 4K video daalta hai.**

## 10. 50,000 log 1 GB ke video daal rahe (alag raasta, URL se access)

Stadium se 50,000 log 1 GB ke video daal rahe, sab Tweet Service se guzarte, server ki memory aur net bhari, aur text
tweet bhi atke. To jo video aa rahi hai use kahin aur bhej do, apne server pe mat lao: bhai tu yahan apne server se
nahi, alag is route pe chala ja, tujhe phir ek URL se access kar lenge.

**Asli naam:** kahin aur bhej, apne server pe na la = video seedha S3 me (presigned URL), URL se access = tweet me sirf
link.

```mermaid
flowchart TB
    A["Stadium se 50,000 log<br/>1 GB video"]:::card
    TS["Tweet Service<br/>(sirf URL)"]:::card
    subgraph AR["ALAG ROUTE"]
        S3[("Apne server pe na la:<br/>seedha S3")]:::pill
    end
    A -- "URL do" --> TS
    A -- "video" --> S3
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style AR fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 11. Ravi gaon me 3G pe, 1 GB nahi kheench paata (Ferrari aur Alto)

S3 me ek hi 4K file hai. John WiFi aur TV pe mast, par Ravi gaon me 3G phone pe, 1 GB kheench nahi paata, video chalta
hi nahi. 1 GB ek saath kyun kheench rahe, thoda-thoda kheench lo, kuch-kuch second ka. Aur chhote size bhi rakho.
Phone bole "mera net bekaar, 240p de", net achha to 4K. Ferrari highway ke liye, Alto gaon ke liye: 4K achhe net pe,
240p-360p kharab net pe.

**Asli naam:** kai size = transcoding, thoda-thoda kuch second ka = segments (chhote tukde), phone net dekh ke maange =
ABR (adaptive bitrate).

```mermaid
flowchart TB
    V[("Ek 4K file, 1 GB")]:::card
    subgraph TT["THODA-THODA KHEENCHO, CHHOTE SIZE BHI"]
        F["Ferrari: 4K<br/>(highway, achha net)"]:::good
        AL(["Alto: 240p-360p<br/>(gaon, 3G)"]):::pill
    end
    V --> F
    V --> AL
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style TT fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 12. Ek machine, 20 minute, 50,000 video peeche (kai machine + id ki parchi)

Ek machine 4K se 4 size banaye, 20 minute lagte, Arpan ka tweet atka, aur 50,000 video usi ek machine ke peeche. Ek
machine kyun, kai machine kar do, kaam baant do. Video ki ek id rakh lo, parchi pe likh do, taaki repeat na ho. 10 log
isi id pe kaam karein, aur kisi id pe nahi.

**Asli naam:** kai machine, kaam baanto = video ke hisse, transcoding workers / consumers (parallel), id ki parchi = har hissa
video id + hissa number se.

```mermaid
flowchart TB
    X["Ek machine, 20 minute,<br/>50,000 video peeche"]:::bad
    P(["Video ki id, parchi pe:<br/>taaki repeat na ho"]):::pill
    subgraph KM["KAI MACHINE, KAAM BAANTO"]
        W1["Machine 1: isi id ka hissa 1"]:::card
        W2["Machine 2: hissa 2"]:::card
        WN["... Machine 10: hissa 10"]:::card
    end
    X --> P
    P --> W1
    P --> W2
    P --> WN
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style KM fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 13. Worker / consumer 7 beech me gira, "processing" ghanton se (tu hai kya + yahan tak parchi)

10 worker / consumer, 9 ka hissa bana, worker / consumer 7 beech me gira, aur video na taiyaar na koi batata, "processing" ghanton se. To har
worker / consumer ka status rakho. 9 ne bata diya, ek nahi bata paaya to dekhunga, heartbeat: "tu hai kya?" Jawab nahi to ise
ghar bhejo, naya banda lao. Uske paas parchi: "maine yahan tak kar liya", naya wahin se uthaye. Save se pehle gira to
shuru se.

**Asli naam:** har worker / consumer ka status = DB me hisson ki ginti, tu hai kya = heartbeat, naya banda = job dobara (retry),
yahan tak parchi = checkpoint.

**Maan lo worker / consumer 7 zinda hai, heartbeat aa rahi, par 1 ghante se ek hissa bana raha, baaki 2 minute me khatam:** uski
wajah se mera kaam ruk raha. "Tu thoda rest le, ye tera replacement, bata kahan tak chhoda, ye wahin se karke dega."
**Asli naam:** atka to replacement = timeout pe slow worker / consumer ka hissa doosre ko, kahan tak chhoda = checkpoint.
Doosri kahani: "zinda hoon" kaafi nahi, poochho kaam kahan tak hua. Ek chhotu ke paas 50 newspaper aa gaye, baaki ke
10-20; 2-3 chhotu extra bhejo, madad karein. Naye chhotu pehle uski parchi dekhein ("30 khatam") aur 31 se lagein.
**Asli naam:** heartbeat + progress report, bada chunk = data skew, madad = work stealing / split, parchi = checkpoint (DB / Redis).

```mermaid
flowchart TB
    W7["Worker / consumer 7 beech me gira"]:::bad
    subgraph ST["HAR WORKER / CONSUMER KA STATUS"]
        H{"Tu hai kya?<br/>(heartbeat)"}:::pill
        C[("Parchi: maine yahan<br/>tak kar liya")]:::card
    end
    N["Naya banda:<br/>wahin se uthaye"]:::good
    W7 -.-> H
    H -- "jawab nahi: ghar bhejo" --> N
    C --> N
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style ST fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 14. Ravi 2:00 pe khiskata, phone 0 se kheench raha (copy dekh ke seedha 51wa page)

Ravi 2:00 pe khiskata hai six dekhne, par phone 0:00 se 2:00 tak sab kheench raha, faaltu data, aur six late. 0 se 2
dikhane ka fayda nahi. Kisi ne copy di aur bola seedha 51wa page dikha. Main copy dekhunga, kahan 50 khatam, 51 kahan
shuru, hisaab lagaunga, aur seedha 51 khol dunga.

**Asli naam:** copy me kaunsa page kahan = manifest (tukdon ki list), hisaab, seedha 51 = seek: time se tukda number,
seedha wahi tukda.

**Maan lo 1 crore log dekh rahe, har ek ke ~150 tukde, to kya har tukda hamare server ki API se jaayega:** tukda dene me
sochna kuch nahi, bas file hai, to CDN pe rakh do. Pehle socha har reader ke paas ek bachcha khada karun jo yaad rakhe
kaunse page pe hai, par 1 crore log = 1 crore bachche. Book me koi bachcha nahi hota, mere paas bookmark hota: 50 pe
hoon to 51 khud palat leta. Sab client side, server pe aane ki zaroorat hi nahi.
**Asli naam:** bas file = tukde CDN se seedhe (static), bookmark = phone ka player khud yaad rakhta kahan hai, 51 khud
palto = manifest se agle tukde ka link, server sirf shuru me ek baar (manifest + link).

Baat 10-20 crore ki nahi, AWS jaise server bahut kuch sambhal lete hain, dikkat wo nahi. Baat ye hai ki kaam kahan
aasaan ho sakta hai. Yahan phone pe aasaan ho gaya to wahin kar diya. Kisi ne phone ka socha hi na hota to wahi bachche
wala tareeka chal raha hota. Dikkat aayi, solution ban gaya.
**Asli naam:** kahan aasaan = kaam wahan karo jahan sabse sasta aur seedha (yahan client), server pe bojh aur kharcha
bina wajah nahi.

```mermaid
flowchart TB
    R["Ravi 2:00 pe khiskata,<br/>phone 0 se kheench raha"]:::bad
    subgraph CD["COPY DEKHO"]
        M(["Kahan 50 khatam, 51 kahan<br/>shuru (manifest)"]):::pill
    end
    B["Phone ka bookmark:<br/>50 pe hoon, 51 khud palto"]:::card
    T[("Seedha 51wa tukda<br/>(CDN, bas file)")]:::good
    R --> M
    M --> T
    B -.-> T
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style CD fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 15. Six viral, saare tukde US me (copy paas me + taala-chaabi)

Six viral, 1 crore log India, US, Australia se, aur saare tukde US ke S3 me, samundar paar, bill aasmaan pe, India me
atke. Samundar paar kyun? Europe wala dost bole "video mere Europe me aa jaaye". To iski copy bana do, tu apne paas
rakh, tu apne paas, teeno copy same, jo jahan se dekhe paas se khule.

**Asli naam:** copy paas me rakho = CDN (edge cache), S3 tak sirf pehli baar.

**Maan lo private video (10 dost), Ravi ne CDN link bade WhatsApp group me daal diya:** taala-chaabi mere paas. Kamra
mera, 10 log hi aa sakte. 11wa aaye, dost ka dost ho, to bhi access nahi.
**Asli naam:** taala-chaabi maalik ke paas = har request pe access check, 10 log hi = allow list (sirf link kaafi
nahi).

**Maan lo tukde CDN se seedha jaate hain, CDN ko allow list pata hi nahi, aur link leak ho gaya:** CDN ke paas bhi
taala-chaabi ho, har request US tak nahi jaayegi. In 10 ki user id do: ye 10 aayein to link do, nahi to mat do. Aur
link ki umar 5 minute, uske baad kisi ke liye nahi khulega. Leak bhi hua to 11wa nahi ghusega. Mohar kaise jaanchi
jaati, wo AWS ka kaam.
**Asli naam:** CDN ke paas bhi taala = CDN signature jaanchta (signed URL / cookie, AWS ka kaam), 10 user id = server allow list dekh
ke hi link deta, 5 minute umar = short expiry. Hamara faisla: kisko aur kitni der ka.

```mermaid
flowchart TB
    X["Saare tukde US ke S3 me,<br/>samundar paar, India me atke"]:::bad
    subgraph CP["COPY PAAS ME (CDN)"]
        I[("India copy")]:::good
        U[("US copy")]:::good
        E[("Europe copy")]:::good
    end
    T(["Taala-chaabi mere paas:<br/>10 dost hi, link 5 minute"]):::pill
    X --> CP
    T -.-> CP
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style CP fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 16. Dabbe me sirf number, screen pe poora tweet (ID se seedha page)

Ravi ke dabbe me sirf tweet ke number pade hain: #905, #902, #899. Par screen pe number nahi chahiye, chahiye kisne
likha, photo, kitne like. Mere paas ek mota register hai, sab likha hua. 1,00,000 page me dhoondhna time lega, main
baawla thodi hoon. Par mere paas ID hain, to seedha wahi page kholunga. Register 1 crore page ka ho ya 10 crore, ID ho
to kaam aasaan. Photo / video CDN se aa jaayegi, baaki mote register se.

**Asli naam:** ID se seedha page = hydration (ID se poori post, key se lookup; dabbe me "905, Arpan ka", author se hissa,
number se tweet), photo CDN se = post me CDN link, mota
register = tweet store (Cassandra), like count saath me. Dabbe me sirf ID isliye, taaki dabba chhota rahe (kadam 6).

**Maan lo Arpan ne tweet delete kar diya, par 8000 logon ke dabbe me uska number (ID) abhi bhi pada hai:** 8000 dabbe ek-ek karke saaf karna bhaari hai, zaroorat bhi nahi. Kadam 16 me screen pe dikhane se pehle hum ID se mote register ka page kholte hi hain. Page pe likha mile "ye tweet delete ho gaya", to bas use dikhao mat, chhod do. Dabbe me pada number apne aap purana hokar khisak jaayega. Doosri kahani (Arpan): feed me push ho chuka tha to kuch second dikh sakta; phir "The user has deleted this post" ki patti (eventual consistency, tombstone).
**Asli naam:** page pe deleted = tweet store me deleted flag (soft delete), dikhate waqt chhod do = hydration pe filter, 8000 dabbe saaf nahi karne padte.

```mermaid
flowchart TB
    subgraph DB["RAVI KA DABBA: SIRF NUMBER"]
        direction LR
        a(["#905"]):::pill
        b(["#902"]):::pill
        c(["#899"]):::pill
        a ~~~ b ~~~ c
    end
    R[("Mota register:<br/>ID se seedha page")]:::card
    C[("CDN: photo / video")]:::card
    S["Ravi ki screen: kisne likha,<br/>photo, 2,481 like"]:::good
    DB --> R
    R --> S
    C --> S
    linkStyle 0,1 stroke-width:0px
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style DB fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 17. Amit ka tweet 40 ke neeche dab gaya (jisse judta wo pehle)

Ravi roz dost Amit ke tweet pe like / comment karta. Amit ka tweet 3 ghante pehle aaya, uske baad 40 aur, Amit ka
neeche dab gaya; upar wo jinhe Ravi kholta bhi nahi. Ye ek tarah recommendation hi hai. Peeche kaunsa algorithm /
machine learning, wo design ka hissa nahi. Par main roz Amit ki photo like kar raha, baaki 40 ko bas dekh ke chhod
deta. Instagram roz dekh raha ki ye ladka Amit ko like karta, to pehli photo Amit ki dikhayega. YouTube me bhi same:
MrBeast ki video roz dekhun to wahi suggest karega. Unka kaam hi yahi: main jitni der rahun, unki kamaai badhe.

**Asli naam:** jisse zyada judta wo pehle = ranking (kisse baat karta, post kitni nayi, likes), algorithm design ka
hissa nahi = ranking model alag; hum dabbe ki list dete, ranked list lete, phir kadam 16 se poori post. Netflix kadam
6 ("tere liye") se juda.

```mermaid
flowchart TB
    subgraph TL["TIME KE KRAM ME"]
        direction LR
        t1(["#905"]):::card
        t2(["#902"]):::card
        t3(["... 40 aur"]):::card
        t4(["#860 Amit"]):::bad
        t1 ~~~ t2 ~~~ t3 ~~~ t4
    end
    M(["Jisse judta wo pehle:<br/>kisse baat, kitni nayi, likes<br/>(ranking)"]):::pill
    subgraph RK["RAVI KA KRAM"]
        direction LR
        r1(["#860 Amit"]):::good
        r2(["#905"]):::card
        r3(["#899"]):::card
        r1 ~~~ r2 ~~~ r3
    end
    TL --> M
    M --> RK
    linkStyle 0,1,2,3,4 stroke-width:0px
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style TL fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
    style RK fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 18. Scroll ke beech upar 5 naye, wahi 5 dobara (kahan tak padha yaad rakh)

Ravi scroll kar raha, 20 dekhe, agle 20 maange. Beech me upar 5 naye aa gaye, to neeche wale 5 khisak ke agle page me
chale gaye; isliye wahi 5 dobara. Hal: Ravi ne kahan tak padha, thodi der ke liye yaad rakh lo. "Ravi ne yahan tak
padh liya, ab iske baad wale do." Naye 5 upar hain, upar scroll karega to dikhenge. Refresh pe purani post dobara aa
sakti hai, Instagram ka kaam hi engage rakhna, wo chalta hai.

**Asli naam:** page number se agle 20 = offset pagination (isi se khisakta), kahan tak padha yaad rakh = cursor (har
page ke saath aakhri dikhayi post lautao, agla uske theek baad se), thodi der yaad = ranked feed (kadam 17) me ranked
list thodi der save, cursor usi list me aage.

```mermaid
flowchart TB
    X["Page 1 dekha: #45 se #26,<br/>beech me upar 5 naye aaye"]:::bad
    O["Page number se agle 20:<br/>5 khiske, wahi 5 dobara"]:::bad
    CR(["Kahan tak padha, yaad rakh (cursor):<br/>Ravi ne #26 tak padha"]):::pill
    N["Agla page #25 se,<br/>dobara kuch nahi"]:::good
    U["Naye 5 upar:<br/>upar scroll pe dikhenge"]:::card
    X -.-> O
    X --> CR
    CR --> N
    CR -.-> U
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
```

## 19. "Thank you" dikha, "mast DP" gaayab (do pe likho, do se padho)

Ravi ne nayi DP daali, Arpan ne reply kiya "mast DP bhai", Ravi ne uspe "thank you bhai". Reply ki 3 copy hain (A, B,
C). Arpan ka reply A aur B me likha gaya, C tak abhi nahi; Ravi ka "thank you" C me pehle pahunch gaya. Priya C se padh
rahi, use sirf "thank you" dikha.

Pehla seedha raasta: thodi der leader (jahan pehle likha) se hi padhwa do. Par saari read leader pe = leader pe bojh.
Isliye phir: do pe likho, do se padho. Reply tabhi pura mano jab kam se kam 2 copy ne rakh liya, aur Priya bhi kisi 2 copy
se poochhe: C me nahi to A me hai. DB shard karke leader + 3 copy rakhi hon tab bhi yahi. Twitter me Cassandra hai (koi
leader nahi), wo ye khud karta hai.
Instagram pe bhi yahi hota: profile photo update ki, turant refresh kiya to purani photo dikhi, thodi der baad nayi.

**Asli naam:** do pe likho, do se padho = quorum (N = 3, W = 2, R = 2, W + R > N, isliye padhi gayi do copy me se ek me
naya likha zaroor), thodi der leader se padho = leader read / read-your-writes (trade-off: leader pe bojh), 2 ne rakha tabhi pura = write ack, Cassandra = leaderless, QUORUM read / write (URL kadam 9-10).
Instagram me "photo update ki, refresh pe purani" = replication lag (nayi photo leader me, refresh pe padhna copy se jo abhi peeche).

```mermaid
flowchart TB
    W["Arpan ka reply 'mast DP':<br/>A aur B me likha (2 ne haan bola)"]:::card
    C[("Copy C: abhi sirf<br/>'thank you'")]:::bad
    Q(["Priya kisi 2 copy se padhe<br/>(quorum: W 2, R 2)"]):::pill
    OK["C me nahi to A me hai:<br/>dono reply sahi kram me"]:::good
    W -.-> C
    W --> Q
    Q --> OK
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
```

---

## Poora dhaancha ek nazar me

```mermaid
flowchart TB
    U(["Ravi: tweet"]):::pill --> LB["LB"]:::card --> T["Tweet server"]:::card
    T --> K["Kafka: post ho gaya"]:::card --> W["Workers"]:::card
    W --> F[("Har follower ka dabba<br/>Redis, last 500")]:::pill
    CL[("Kohli / SRK ki alag parchi<br/>padhte waqt milao")]:::card
    F --> FE(["Feed: dabba + celeb parchi,<br/>ranking"]):::good
    CL --> FE
    T --> DB[("Cassandra: user id se baanto, 3 copy<br/>purane S3, follow Neo4j")]:::card
    V["Video: presigned URL -> S3<br/>workers 4 size -> CDN"]:::card --> FE
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
```

---

Notes (Arpan ke shabd) = [TWITTER_HAND_NOTES.md](TWITTER_HAND_NOTES.md)
