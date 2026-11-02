# Notification system: Arpan ke order ki kahani

Arpan ek shopping app se order karta hai. Order hote hi app use teen jagah khabar bhejti hai: SMS ("order placed"),
email (bill) aur phone pe push ("shipped"). Ye khabar bhejne wala system main bana raha hoon. Kaam: order turant ho,
khabar khoye nahi, do baar na jaaye, aur OTP kabhi bheed me na phanse.

Har kadam = **kahani** (dikkat aur hal, bolne wali bhasha) -> **Asli naam** (interview me yahi shabd) -> tasveer
(kahani ka shabd, bracket me asli naam).

---

## FR / NFR / Numbers (interview me pehle 2 minute)

```
FR      ek ghatna (order hua) -> SMS / email / push · user ki pasand maano · message ka saancha ("Hi {naam}")
NFR     Order Service ruke nahi · khabar khoye nahi, do baar na jaaye · OTP turant, offer ruk sakta
Numbers 10 crore user x 5 khabar = 50 crore roz, ~5,800 / sec · sale pe 10 guna
        SMS company ki had 100 / sec · Diwali campaign = 1 crore log ek saath
```

---

## 1. "Place Order" 8 second ghooma (beech ka dabba)

Order Service khud DB me order likhta, phir khud SMS, email, push company ko call karta. SMS company dheemi, har call
8 second, to button 8 second ghoomta. Aur SMS company band ho to order hi fail dikhta, jabki DB me likha ja chuka. To
beech me ek dabba rakh lo. Wo dabba kahe: aao bhai aao, sab mere paas aa jao, koi dikkat nahi. Main sabko store kar
lunga aur dheere-dheere aaram se sabko bhejta rahunga. Order Service khabar dabbe me daal ke turant "order ho gaya"
bol deta.

**Asli naam:** beech ka dabba = queue (Kafka, khabar disk pe, worker gira tab bhi khoti nahi), aaram se bhejna =
async, order turant.

```mermaid
flowchart LR
    U["Arpan"] --> O["Order Service"]
    O --> K[("Beech ka dabba<br/>(Kafka)")]
    K --> W["Worker"]
    W --> C["SMS / email / push company"]
    style K fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 2. SMS ke peeche bill atka (sabki alag line)

Dabbe me ek hi line thi, SMS, email, push sab ek ke peeche ek, aur ek worker. SMS company 8 second le rahi, to email
aur push company theek hote hue bhi bill aadhe ghante baad mila. To line alag-alag kar do, sabki apni line. Jaise
ladies ki alag line, gents ki alag, bachchon ki alag; airport security, mandiron me bhi aise hi hota. Har line apne
log khud sambhal le. Ab SMS dheema hua to sirf SMS line rukti, bill apni line se nikal jaata.

**Asli naam:** sabki alag line = SMS, email, push ka alag topic, apni line sambhalna = har topic ka apna consumer
group (worker alag-alag badha sakte: sale me push ke 100, SMS ke 20).

## 3. Ek "order placed" do baar (parchi)

SMS worker ne khabar uthayi, SMS Arpan ke phone pe pahunch gaya, par Kafka ko commit karne se theek pehle crash.
Dobara chala, Kafka ne wahi khabar phir di, SMS do baar. OTP ya "5000 debited" pe hua to banda ghabra jaayega. To
parchi rakh lo apne paas. Kaam ho gaya to likh liya aur kahin rakh liya. 10 din baad wahi khabar dobara aayi, parchi
dekhi: ye to ho chuka, chhod do. Kabhi bhi kuch ho jaaye, parchi hai.

**Asli naam:** parchi = idempotency. Khabar ki id (jaise 999:sms) ka "ho gaya" record DB me (id UNIQUE), bhejne se
pehle check, mila to skip.

## 4. "Abhi bheed hai, baad me aana" (thodi der baad dobara)

Company ne OTP pe error lautaya: "abhi bheed hai, baad me try karo". Chhod do to OTP kabhi nahi milega, aur turant
baar-baar bhejo to company pe bojh aur peeche ki saari SMS line ruki. To use alag rakh do, thodi der baad try karenge.
Jaise main papa se 100 rupaye maangne gaya aur papa busy hain, to thodi der ruk jaata hoon, baad me jaata hoon. Pattern
yahi: koi bhi dikkat aayi to likh lo, kahin daal do, line mat roko. Par galat number pe 3 ghante baad bhi kuch nahi
hoga, use dobara ghumaana bekaar. Wo kaise pehchanein, agle kadam me.

**Asli naam:** alag rakh ke thodi der baad = retry queue (ruk ke dobara, wait badhta jaata), main line nahi rukti.

## 5. SMS company poori band (teen-chaar company + VIP line)

Har call 30 second latki phir fail, saare worker atke, retry line phoolti ja rahi, Arpan ka OTP nahi aaya. To ek
company nahi, teen-chaar company rakh lo, ek band to doosri. Aur OTP important hai: bank OTP, transfer OTP, Uber ka
OTP. Unki priority alag kar do, jaise mandir me VIP darshan, VIP ko jaldi.

Band hai kaise samjhun: SMS bheja, fail, apni parchi me note kiya. Doosra fail, teesra fail. Ab jo fail hue unhe backup
se bhejo. Backup se chal gaye aur main pe 5-6 baar fail, matlab main company band hai: ab traffic backup se guzaaro.
Beech-beech me main ko try karo: "tu theek hai? chal raha?" Nahi chala to backup chalta rahega, chal gaya to wapas
main pe. Aur backup bhi wahi fail bole, to dikkat company me nahi, phone number me hai: use seedha alag dabbe me daal
do, retry me nahi, koi dekhe, number galat hai to hatao.

**Asli naam:** teen-chaar company = failover (backup provider), VIP darshan = OTP ki alag priority line (topic),
5-6 baar fail to backup = circuit breaker OPEN, "tu theek hai?" = HALF-OPEN, chal gaya = CLOSED, backup bhi fail =
DLQ (dead letter queue).

```mermaid
flowchart LR
    N["Notification Service"] --> T[("VIP line<br/>(OTP topic)")]
    N --> S[("SMS line")]
    T --> W["Worker"]
    S --> W
    W --> CB{"Band hai?<br/>(circuit breaker)"}
    CB --> M["Main company"]
    CB --> B["Backup company<br/>(failover)"]
    W --> D[("Alag dabba<br/>(DLQ)")]
    style T fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style CB fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style B fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style D fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 6. Company ki had 100 / sec (saanjha dabba)

20 worker apni raftaar se, sab milake 300 / sec, company 429 de rahi aur block ki dhamki. Har worker ko sirf apni
ginti pata. Main company ko nahi bol sakta ki had badha do, unhe baaki bhi dekhna hai, aur apni SMS company banana
impossible. To ruk-ruk ke bhejo, peeche lambi kataar lage to koi dikkat nahi, burst ek saath na jaaye. Aur ginti
saanjha kar lo: 10 log apna-apna kaam kar rahe, tumne kitna kara? +2, tumne +3. Ek dabba jisme ye ginti likhi ho, sab
usi se guzrein. Main beech me baitha hoon: tune bhej diya, tu bhi bhej raha? Nahi, tu ruk.

**Asli naam:** had apni taraf se = rate limit, saanjha dabba = shared token bucket (Redis, 100 token / sec, sab worker
wahi se token lete), token nahi mila = khabar line me wait, khoti nahi.

## 7. Company ne "le liya" bola, SMS pahuncha hi nahi (le liya ke baad)

Company ne turant OK bola, maine record me SENT likha. Par Arpan ka phone network se bahar, SMS pahuncha hi nahi, aur
support bolta "humne to bhej diya tha". Asal me company ka OK matlab: tera SMS le liya, abhi bheja nahi. Phir company
dheere-dheere aage badhegi aur kuch der baad khud batayegi: bhej diya ya nahi bheja. Do hi raaste, teesra nahi. Sent
hua to apni parchi me "sent", fail hua to "fail".

**Asli naam:** le liya = ACCEPTED (SENT nahi), company baad me batati = webhook (signature jaanch ke), sent ya fail =
DELIVERED / FAILED, apni parchi = Tracking DB (Cassandra: har message ek row, bahut likhna, JOIN nahi).

```mermaid
flowchart LR
    W["SMS worker"] --> C["SMS company"]
    W --> T[("Parchi<br/>(Tracking DB: ACCEPTED)")]
    C --> H["Company batati<br/>(Webhook)"]
    H --> T
    style T fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style H fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 8. Diwali: 1 crore ko offer (tukde + moti diary)

Ek khabar "1 crore users ko offer bhejo". Ek worker ne list nikaali, loop me bhejne laga, 40 lakh pe crash. Dobara
chala to loop shuru se, 40 lakh ko do baar, aur kaam ek worker pe atka, baaki khaali. To thoda todo. 20, 30, 50 worker
me baant do, sabko alag range: tu yahan se yahan tak. Jo bheje gaye unhe turant DB me mark karo. 1 crore Redis me
nahi rakhunga, Redis ko befaltu nahi bharna, apni moti diary hai, usme karte jao. Jaise 10 logon ko kaam diya: tu iske
ghar ja, tu iske ghar, parchi pe likhte raho ye ghar ho gaya. Kal koi bimaar pada to parchi dekho: 10 me se 7 ho gaye,
backup tu aa ja, baaki 3 tu bhej de.

**Asli naam:** tod ke range = campaign tukde (chunks), turant DB me mark = checkpoint (DB), baaki 3 backup = crash pe
wahin se. Mark ke baad aur marne ke beech wale kuch log dobara aa jaayein, to kadam 3 wali parchi (idempotency)
rokti: isko bhej diya tha? Haan, to dobara nahi.

## 9. Har SMS pe DB, OTP dheema (ek saancha apne paas)

Arpan ne settings me chuna "offer wale SMS mat bhejo". Har SMS se pehle worker DB se user ki pasand aur message ka
saancha padhta. 1 crore SMS = 1 crore query, DB ki saans phooli. Wo baar-baar wahi cheez padh raha hai, use apne paas
rakh lo. Diwali pe sabko ek hi offer, "Diwali 50% off". Mote register pe baar-baar kyun jaun? Ek saancha apne paas,
bas naam badalna hai, bhejta ja. Par user ki pasand uski settings hai, wo DB me hai, DB se hi uthayenge; crore users
ki setting cache me kaise rakhenge. OTP itna late nahi hoga, kyunki OTP ki apni line, apne worker aur rate limit
pehle se hain. Baad me naap ke lage ki faltu queries bahut badh gayi, tab sochunga pasand bhi cache me rakhun.

**Asli naam:** ek saancha apne paas = template cache (saancha badla to key DELETE), pasand = user preference DB se
(taazi, cache nahi), queries badhi tab = naap ke hi preference cache.

```mermaid
flowchart LR
    K[("Kafka")] --> N["Notification Service"]
    N --> TC[("Ek saancha<br/>(Template cache)")]
    N --> L[("SMS / email / push line")]
    L --> W["Worker"]
    W --> PD[("Mota register<br/>(Preference DB)")]
    style TC fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

---

Notes (Arpan ke shabd) = [NOTIFICATION_HAND_NOTES.md](NOTIFICATION_HAND_NOTES.md) · [<- MASTER SHEET](../../00_MASTER_SHEET.md)
