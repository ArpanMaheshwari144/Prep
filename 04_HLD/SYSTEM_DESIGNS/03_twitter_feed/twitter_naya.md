# Twitter feed: shuru se, ek kahani

Kram wahi jo interview me hota hai: **FR, phir NFR, phir numbers, phir design**. Design sabse seedhe dabbe
se shuru hota hai, aur har dikkat aane pe ek naya dabba judta hai. Diagram me peela dabba = abhi juda. Poori
file me ek hi kahani hai: Arpan 200 logon ko follow karta hai (Virat Kohli bhi), subah app khol ke feed dekhta
hai, kabhi khud tweet karta hai.

Zyadatar tukde pehle se padhe hue: Kafka worker ([notification](../04_notification_system/04_notification_system.md)),
Redis ([URL](../01_url_shortener/01_url_shortener.md)), Cassandra + S3 ([chat](../14_chat_messaging/14_chat_messaging.md)).
Iska apna: ek tweet laakhon logon ki feed me kaise pahunche (fanout), aur celebrity pe wo kaise mudta hai.

---

## 1. FR: kya karna hai

```
1. tweet post karna (text, photo / video)
2. home feed: jinko follow karta, unke naye tweet, naye upar
scope ke bahar: search, trending, DM, notification
```

## 2. NFR: kaisa chalna chahiye

```
- feed 200 ms me khule
- feed 2-5 sec purani dikhe to chalega (isliye pehle se bana sakte hain)
- celebrity (10 crore follower) ka tweet bhi sambhale
```

## 3. Numbers

```
- ~50 crore user, 50 crore tweet / din -> ~5,000 tweet / sec
- ek aadmi ~200 ko follow karta, celebrity ke 10 crore follower
- padhna : likhna ~50 : 1 (feed dekhne wale bahut zyada)
```

---

## 4. Design

### Kadam 1: sabse seedha (do table, har feed pe jod)

Do table: `tweets` (kisne kya likha, kab) aur `follows` (kaun kisko follow karta). Arpan feed kholta hai, to
har baar:

```
feed = follows se Arpan ke 200 log nikaalo
       -> tweets me se un 200 ke tweet nikaalo
       -> time se sort -> upar ke 50 dikhao
```

```mermaid
flowchart LR
    U["Arpan: feed kholi"] --> F["Feed Svc"]
    F -- "1. Arpan ke 200 log" --> FO[("SQL: follows")]
    F -- "2. un 200 ke tweet, sort" --> T[("SQL: tweets")]
    style F fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par har feed kholne pe 200 logon ke tweet DB se nikaal ke sort karna. Feed dekhne wale likhne walon se 50
guna zyada, to DB har second laakhon baar yahi bhaari jod. Feed 3 sec me khulti, DB thak ke baith gaya.
Ek line: feed padhte waqt banti hai, aur padhne wale bahut zyada hain.

### Kadam 2: har user ka pehle se bana inbox (fanout on write)

Har user ka Redis me ek **inbox** = uski taiyaar feed (sorted set, naya sabse upar). Koi tweet kare to wo
tweet usi waqt uske saare followers ke inbox me daal do. Isi ko **fanout** kehte hain: ek tweet, kai inbox me
bikhra.

```
Ravi ne tweet kiya (T-901) --> Ravi ke 200 follower kaun? --> har ek ke inbox me T-901 daalo
                                                                inbox:Arpan = [T-901, T-880, T-850 ...]
                                                                inbox:Mohan = [T-901, ...]

Arpan ne feed kholi --> Redis: inbox:Arpan se upar ke 50 --> turant (koi jod nahi, koi sort nahi)
```

```mermaid
flowchart LR
    R["Ravi: tweet T-901"] --> TS["Tweet Svc"]
    TS -- "200 follower ke inbox me" --> IN[("Redis: inbox per user")]
    U["Arpan: feed kholi"] --> F["Feed Svc"]
    F -- "inbox:Arpan, upar ke 50" --> IN
    style IN fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Ab kya theek hua: feed kholna sirf ek Redis read, 200 ms se bahut kam. Bhaari kaam (kis-kis ke tweet nikaalo)
ab tweet karte waqt ek baar, har feed pe nahi. Dekhne wale 50 guna zyada, to kaam likhne wali taraf khiskana
sahi.

Inbox me poora tweet nahi, sirf tweet id (`T-901`). Text ek baar tweets table me; 200 jagah poora text rakhte
to jagah 200 guna. Feed pe un 50 id ka poora tweet DB se ek saath aata.

Read replica laga dete? Copy pe bhi har feed wahi 200 logon ka jod hota, bas bant jaata. Jod hi dikkat hai,
fanout hi jawab.

Par Arpan ka dost Mohan chhota influencer hai, 5,000 follower. Mohan ne tweet dabaya, aur Tweet Svc 5,000
inbox me likh raha, uske baad hi "tweet ho gaya" bolega. Mohan ka button 5 second ghoomta raha.

### Kadam 3: tweet turant, fanout peeche se (Kafka + fanout worker)

Dikkat: tweet karne wala ruke nahi, 5,000 inbox me likhna peeche se ho, aur beech me kuch khoye nahi. Yahi
notification kadam 2 wali dikkat hai, to wahi ilaaj.

```
Mohan: tweet --> Tweet Svc --1. tweets DB me save--> "tweet ho gaya" (turant)
                           --2. khabar "T-950 by Mohan"--> [Kafka: tweets]
                                                              |
                                          fanout worker (kai machine) uthata
                                          -> Mohan ke 5,000 follower nikaalo
                                          -> har inbox me T-950 daalo
```

```mermaid
flowchart LR
    M["Mohan: tweet"] --> TS["Tweet Svc"]
    TS -- "save + outbox" --> DB[("tweets DB")]
    DB -- "relay" --> K[("Kafka: tweets")]
    K --> FW["Fanout worker<br/>(kai machine)"]
    FW -- "5,000 inbox me" --> IN[("Redis: inbox")]
    style K fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style FW fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Kafka yahan isliye, ki dikkat khud maang rahi: likhne wala ruke nahi, peeche ka kaam khoye nahi, aur bheed
aaye to worker badha sakein.

Ab kya theek hua:
- Mohan ka button turant ruk jaata hai.
- 5,000 inbox me likhna worker peeche se karte. 2-3 sec lagein to chalega (NFR: feed 2-5 sec purani chalegi).
- worker gira to khabar Kafka me padi, wapas aake wahin se uthata.
- 5,000 tweet / sec ki bheed: worker badha do.

Ek chhed: Tweet Svc ne DB me tweet save kiya, aur Kafka ko bhejne se pehle gira. Tweet DB me hai, par kisi ke
inbox me nahi. Ilaaj wahi **outbox** ([Kafka file kadam 7](../12_message_queue_kafka/12_message_queue_kafka.md#kadam-7-bhejne-wala-gira-outbox)): khabar bhi usi transaction me outbox table me,
relay use Kafka me daale.

Par Virat Kohli ne match ke baad tweet kiya, 10 crore follower. Fanout worker 10 crore inbox me likhne baitha,
ghanton lag gaye. Uske peeche Arpan, Mohan, sabke tweet line me atke.

### Kadam 4: celebrity ka alag raasta (hybrid fanout, twitter ka apna)

Do tarah ke log, do raaste:

```
aam log (jaise Mohan, 5,000 follower)  -> fanout on WRITE: tweet sabke inbox me (Kadam 2-3)
celebrity (jaise Virat, 10 crore)      -> fanout NAHI. Tweet bas tweets DB me (jaise har tweet jaata)

Arpan ne feed kholi:
  1. inbox:Arpan se aam logon ke tweet                         (pehle se bane, Redis)
  2. Arpan jin 5 celebrity ko follow karta, tweets DB se unke aakhri 20 (abhi padhe;
     tweets table me author_id + time ka index, to ye saada query)
  3. dono milao -> time se sort -> upar ke 50
```

```mermaid
flowchart LR
    V["Virat: tweet"] --> TS["Tweet Svc"]
    TS -- "sirf save, fanout nahi" --> T[("tweets DB<br/>index: author_id + time")]
    U["Arpan: feed"] --> F["Feed Svc"]
    F -- "1. aam logon ke" --> IN[("Redis: inbox:Arpan")]
    F -- "2. 5 celeb ke aakhri 20" --> T
    style F fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Ab kya theek hua: Virat ka tweet ek hi jagah likha, 10 crore jagah nahi, to fanout line nahi atakti. Arpan ki
feed me bas 5 celebrity ke tweet padhte waqt jodne, 200 logon ke nahi, to feed tez rehti.

Doosra kyun nahi (asli choice):
- sabke liye fanout on write: Virat ka ek tweet = 10 crore likhai, line ghanton atakti (Kadam 3 wali).
- sabke liye fanout on read: har feed pe 200 logon ka jod, Kadam 1 wapas, feed dheemi.
- dono ka mel (hybrid): kam follower wale ka likhte waqt baanto, bahut wale ka padhte waqt jodo.

To kaun kahan se? (Arpan ka sawaal)

```
aam logon ke tweet  -> Redis (inbox:Arpan, pehle se bana)
celebrity ke tweet  -> tweets DB (padhte waqt), unke liye koi Redis nahi
```

Celebrity kaun? Ek had tay hoti (jaise 1 lakh se zyada follower). Ye faisla hai, pakka niyam nahi.

Par (Arpan ka sawaal: "crore log seedha DB pe?") match khatam, Virat ke tweet pe 1 crore log ek saath feed
khol rahe. Sabki feed tweets DB se wahi ek query chala rahi: "Virat ke aakhri 20". DB pe ek hi jagah crore
padhai, DB garam.

### Kadam 5: crore log, sab DB se "Virat ke aakhri 20" (har server ka apna cache)

Ilaaj: har Feed Svc machine ye jawab **apni memory me** 2 second rakhti hai (local cache). Matlab har server
ka apna chhota cache, uske andar hi; koi alag Redis nahi.

```
pehle: 1 crore feed --> sab tweets DB pe "Virat ke aakhri 20"          --> DB garam
ab:    1 crore feed --> 100 Feed Svc machine
                        har machine ki memory me "Virat ke 20" (umar 2 sec)
                        --> DB tak har machine 2 sec me sirf 1 baar = 100 sawaal / 2 sec, crore nahi
```

```mermaid
flowchart LR
    U["1 crore feed"] --> F["100 Feed Svc machine<br/>(har ek ka apna cache: Virat ke 20, 2 sec)"]
    F -. "2 sec me ek baar" .-> T[("tweets DB")]
    style F fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Crore sawaal machine ki memory se nipat jaate, DB tak sirf 100 machine x 2 sec me ek baar.

2 sec purana chalega? Haan, NFR: feed 2-5 sec purani chalegi. Virat ka naya tweet kisi ko 2 sec der se dikhe,
to chalega.

Beech me Redis kyun nahi? Local cache akela hi DB ko bacha leta. Celebrity gine-chune (kuch hazaar), to unke
aakhri 20 tweet har machine ki memory me aaraam se aa jaate. Redis is design me sirf inbox ke liye (Kadam 2).

Ye [URL kadam 8](../01_url_shortener/01_url_shortener.md#kadam-8-ek-link-viral-local-cache) wala viral link hi hai: wahan ek link, yahan ek celebrity.

Par Redis me 50 crore logon ka inbox, har inbox me hazaron tweet id. Redis ki RAM bhar rahi, aur Redis RAM
mehngi hai.

### Kadam 6: inbox chhota rakho (3 chhoti baatein)

**1. Inbox me sirf tweet id** (ye Kadam 2 me ho chuka). Ek id lagbhag 8 byte ki, poora tweet nahi.

**2. Har inbox me sirf aakhri 800 tweet.** Koi 800 se neeche scroll nahi karta. Naya tweet aaya to sabse
purana kaat do (sorted set trim).

**3. Sirf active logon ka inbox.** Jo 30 din se app nahi khola, uska inbox Redis me rakhte hi nahi (umar / TTL
se mit jaata). Wo wapas aaya to pehli baar uski feed Kadam 1 wale tareeke se ek baar banao (200 logon ka jod),
aur inbox phir se bhar do. Bas pehli feed thodi dheemi khulti.

```
pehle:  50 crore inbox x hazaron id                      -> RAM bhar gayi
ab:     ~10 crore active inbox x 800 id x 8 byte = ~640 GB -> kai Redis machine me baant do (user_id se)
        purana / inactive -> inbox nahi, wapas aaye to ek baar banao
```

```mermaid
flowchart LR
    FW["Fanout worker"] -- "sirf active ke inbox,<br/>aakhri 800 id" --> R1[("Redis 1")]
    FW --> R2[("Redis 2")]
    FW --> R3[("Redis ... (user_id se baante)")]
    style R1 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style R2 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style R3 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

(10 crore active aur 8 byte, dono mera andaza. Asli number Redis ke apne extra kharche ke saath thoda zyada
aayega.)

**Ab kya theek hua:** Redis me sirf wahi jo sach me dekha jaata hai. 640 GB kai Redis machine me baant dete,
jaise [URL me shard](../01_url_shortener/01_url_shortener.md#kadam-5-data-bahut-bada-db-ke-tukde--shard) kiya tha.

Par roz 50 crore naye tweet tweets DB me jaate: 5,000 likhai / sec, saal bhar me sau TB. Ek SQL DB machine ye
likhai aur jagah nahi jhel paayegi.
