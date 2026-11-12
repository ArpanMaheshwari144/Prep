# Chat / messaging: Arpan, Ravi aur college-gang ki kahani

Arpan apne dost Ravi ko "kahan hai?" bhejta hai, aur dono "college-gang" group me hain jisme 500 log hain. Kaam:
message turant pahunche, Ravi ka phone band ho to bhi baad me mile, kram na bigde, do baar na dikhe, aur tick,
unread, video, online sab chale.

Har kadam = **kahani** (dikkat aur hal, bolne wali bhasha) -> **Asli naam** (interview me yahi shabd) -> tasveer
(kahani ka shabd, bracket me asli naam).

---

## FR / NFR / Numbers (interview me pehle 2 minute)

```
FR      1-to-1 chat + group (500 tak) · Ravi offline ho to baad me mile · tick (pahuncha / padha) + unread
        · photo / video · online / last seen (end-to-end encryption, call: scope se bahar)
NFR     message turant (1 sec se kam) · kabhi na khoye · ek chat me kram na bigde (thodi der ulta dikh sakta, phone number se theek kar leta) · do baar na dikhe
Numbers 50 crore user, ek waqt ~10 crore online = 10 crore khule taar -> ~100 chat server
        roz ~400 crore message = ~46,000 likhai / sec, ek message ~300 byte -> roz ~1.2 TB
```

---

## 1. Ravi ka phone har second poochh raha (beech ka dabba, khula taar)

Arpan ne Ravi ko "kahan hai?" bheja. Ravi ka phone har second server se poochhta "mere liye kuch aaya?". 10 crore log
aise poochh rahe, 99% baar jawab "kuch nahi", server poochne walon se hi dab gaya. To server ko poochne ka mauka hi na
do. Kisi ne kuch bheja to seedha bhej do, bas kaam khatam. Beech me aisa dabba rakh do jo jaise hi kuch aaya, turant
doosre bande ko bhej de. Phone band, net nahi, aise pench aage dekhte.

**Asli naam:** har second poochhna = polling (band), beech ka dabba = chat server, khula taar = WebSocket (ek baar
khula, band nahi hota, server khud push karta). Server pe har khula taar memory khaata hai, isliye event loop (Netty):
kuch hi thread, sirf jis taar pe message aaya use sambhalo.

```mermaid
flowchart TB
    X["10 crore phone har second:<br/>mere liye kuch aaya? (99% kuch nahi)"]:::bad
    A["Arpan: kahan hai?"]:::card
    subgraph BD["BEECH KA DABBA"]
        W(["Jaise hi aaya, turant doosre ko bhej de<br/>(khula taar, WebSocket)"]):::pill
    end
    R["Ravi ko seedha mila,<br/>poochne ka mauka hi nahi"]:::good
    X -.-> W
    A --> W
    W --> R
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style BD fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 2. Ravi ka phone band, 2 ghante baad on (dabbe me sanjo lo)

Arpan ne "kahan hai?" bheja, par Ravi ka phone band, 2 ghante baad on hoga. Taar toota pada hai, server ke haath me
message hai, bhejne ki jagah nahi. To server se bol do, ise thodi der apne paas aisi jagah rakh le jahan baad me dekh
paaye. Ek dabba bana le, usme rakh lo: "Arpan ne Ravi ko ye bheja hai." Ravi online hoga to use bhej denge, sanjo lo.
Jaise papa ne mujhe paise de ke kahin bheja, aur uncle ji mile nahi, to main paise apne paas hi rakhunga, kharch ya
phenk thodi dunga.

**Asli naam:** dabba = message store (har message pehle DB me save, phir bhejo; pehle bhejte aur save se pehle gira
to message gaya), online hoga to bhej denge = offline delivery. DB = Cassandra, `chat_id` se baanta (andar ring = consistent hashing): likhai bahut
(46,000 / sec, roz 1.2 TB), padhai hamesha ek chat ki ("aakhri 50 do"), na join na transaction. Ek chat ke saare
message ek machine pe, kram me.

```mermaid
flowchart TB
    A["Arpan: kahan hai?"]:::card
    X["Ravi ka phone band,<br/>2 ghante baad on"]:::bad
    subgraph SJ["DABBE ME SANJO LO"]
        D[("Arpan ne Ravi ko ye bheja hai<br/>(pehle DB me save)")]:::pill
    end
    R["Ravi online: tab bhej denge"]:::good
    A --> D
    A -.-> X
    D --> R
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style SJ fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 3. 100 server, Ravi kis pe? (DSA wala map)

10 crore log, ek server sabke taar nahi pakad sakta, 100 server lage. Arpan ka taar server 3 pe, Ravi ka server 71
pe. Server 3 ko pata hi nahi Ravi kis server pe. To ek map rakh le apne paas, DSA wala map: ye is server pe hai, ye us
server pe. Dhoondhne ki zaroorat nahi, O(1) time me mil gaya. Key-value pe rakh lo.

**Asli naam:** DSA wala map = session map Redis me (`ravi -> server 71`, userId -> server), O(1) me mil gaya = seedha
lookup. Taar toota to server 71 map se Ravi ki entry mita deta.

```mermaid
flowchart TB
    S3["Server 3 (Arpan):<br/>Ravi kis server pe?"]:::card
    subgraph MP["DSA WALA MAP (key-value)"]
        M[("Ravi -> server 71,<br/>Arpan -> server 3 (Redis)")]:::pill
    end
    S71["Server 71: O(1) me mil gaya"]:::good
    S3 --> M
    M --> S71
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style MP fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 4. Server 71 tak message kaise? 100 x 99 taar (chhota ladka)

Server 3 ko pata chal gaya Ravi server 71 pe. Par message 71 tak pahunche kaise? Har server baaki 99 se seedha taar
jode to 100 x 99 taar, kal 500 server to aur zyada. Ravi online hai, hume pata hai wo kis server pe. To maan le
saamne 10 ladke khade, maine ek chhote ladke ko kaha: "jo address tere paas hai, use le ke chala ja, wahan koi aadmi
hoga, use ye pakda de." Wo dekhega "haan bhai, ye wo ladka hai", aur message pakda dega.

**Asli naam:** chhota ladka = Redis pub/sub: har server apne naam ka channel sunta (`server-71`), server 3 us channel
pe publish karta. Wahan wale ko pakda de = server 71 apne channel se le ke Ravi ke taar pe bhejta.

```mermaid
flowchart TB
    S3["Server 3: Ravi server 71 pe"]:::card
    X["Har server baaki 99 se taar:<br/>100 x 99"]:::bad
    subgraph CL["CHHOTA LADKA"]
        P(["Address le ke chala ja,<br/>wahan wale ko pakda de<br/>(Redis pub/sub)"]):::pill
    end
    S71["Server 71: haan, ye wo ladka,<br/>Ravi ke taar pe"]:::good
    S3 -.-> X
    S3 --> P
    P --> S71
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style CL fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 5. 500 logon ka group, ek message ke 500 kaam (ek dabbe me ek baar)

Arpan ne 500 logon ke group me "kal party hai" bheja. 500 log 60 server pe bikhre. Har member ke liye alag message
likhein to ek message ke peeche 500 kaam, aur aise messages har second. To use ek dabbe me rakh lo, 500 log khud
aayenge sunne. Kahin rakh do, 500 log jab chahe apna aake sunte rahenge.

**Asli naam:** ek dabba = group message DB me sirf ek baar (`chat_id = college-gang`), 500 log khud aayenge sunne =
members wahi ek message padhte, 500 copy nahi. Group bhi bas ek chat hai jisme 2 ki jagah 500 log; members ki list se
online walon ko map + pub/sub se bhejo, baaki wapas aayein to dabbe se.

```mermaid
flowchart TB
    A["Arpan: kal party hai<br/>(500 logon ka group)"]:::card
    X["Har member ki alag copy:<br/>ek message, 500 kaam"]:::bad
    subgraph ED["EK DABBE ME EK BAAR"]
        D[("chat_id = college-gang<br/>(group message DB me)")]:::pill
    end
    M["500 log khud aayenge sunne"]:::good
    A --> D
    A -.-> X
    M --> D
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style ED fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 6. Lift me "hi" dobara gaya, Ravi ko "hi hi" (ek key)

Arpan lift me, Ravi ko "hi" bheja. Message server pe save ho gaya, par "pahuncha" wala jawab lift me kho gaya. Phone
ko laga gaya hi nahi, "hi" dobara bheja, Ravi ko "hi hi". Message Ravi ke paas chala gaya, par hum lift me the to jawab
hamare paas nahi aaya. To jab network aaye tab bol do "haan bhai, chala gaya", double tick dikha do. Aur message ek
baar issue karwao, doosri baar discard kar do. Ek key rakh lo: "hi" aaya, iske paas ek key hai. Doosra aaya, same
person ko ja raha aur key same hai, to mana kar do: "ye message chahiye nahi, tera pehla ho gaya."

**Asli naam:** key = `clientMsgId` (idempotency key; phone Send dabate hi banata, jaise `m-501`, dobara bhejne pe
wahi; ye banking wali idempotency key hi hai). Key same = dupe copy chhodo, DB me dobara mat likho. Network aaye to
double tick = "pahuncha" ka jawab (ack) dobara bhejo.

**Maan lo ulta: Ravi ke phone ka "mil gaya" kho gaya, server ne message dobara bheja:** Wahi key, lene wale ke phone pe: Ravi ka phone message ka number (seq / clientMsgId) dekhe, pehle aa chuka to dobara mat dikhao.
**Asli naam:** idempotency key (wahi ID dobara aaye to chhodo).

```mermaid
flowchart TB
    A["Arpan lift me: hi"]:::card
    S["Server: save ho gaya"]:::card
    X["Pahuncha wala jawab kho gaya:<br/>phone ne dobara hi bheja"]:::bad
    subgraph KY["EK KEY RAKH LO"]
        K{"Same person, same key?<br/>(clientMsgId)"}:::pill
    end
    D["Haan: discard, tera pehla ho gaya"]:::good
    T["Network aaya: double tick"]:::good
    A --> S
    S -.-> X
    X --> K
    K --> D
    S --> T
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style KY fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 7. Group me "kahan?" pehle dikha, "kal party" baad me (server lagayega number)

Group me Arpan ne "kal party" bheja, usi second Ravi ne "kahan?". Kuch members ko "kahan?" pehle dikha, aur dono phone
ki ghadiyan bhi 2 second aage-peeche. To numbering kar do: jisne pehle bheja uska pehla aayega, saaf seedhi baat. Par
number phone nahi lagayega, phone lagayega to clash ho jaayega. Phone ko thodi pata kaunsa message pehle aaya, phone
to bas message bhejne wala banda hai. Server ko pata hai kaunsa pehle aaya, kis time aaya. Server lagayega number.

**Asli naam:** numbering = sequence number, server ka, har chat ka apna (`#980`, `#981`; Redis `INCR seq:college-gang`,
ek saath aaye to bhi do alag number). Phone ko thodi pata = phone ki ghadi pe bharosa nahi. Wapas jude Ravi ko jo
nahi gaya wo sab server apne record se deta (neeche "Maan lo #980"), aur phone isi number se sort karta. Redis restart hua to ginti peeche se shuru na ho: us chat ka
aakhri number Cassandra se padh ke wahin se aage.

**Maan lo #980 ka DB save atka, #981 pehle pahunch gaya, Ravi ka phone "aakhri #981" yaad rakhe aur reconnect pe "#981 ke
baad do" bole:** #980 chhoot jaata. Par server chootiya nahi: har user ka record rakhta, ye gaya ye nahi gaya. Ravi wapas
aaya to server apne record se jo nahi gaya wo sab bhejta, phone ke aakhri number se nahi. Message milega hi; kram thoda
hile to phone number dekh ke sort kar de. Kram kitna strict chahiye, FR me pehle tay (WhatsApp me ek aage-peeche chalega).
Jaise class me teacher: roll number 6 late hai to baaki 100 ki attendance lo, register me likho "6 abhi nahi aaya"; 6 aaye
to 5 ke baad wali jagah bithao. Register server ke paas, kuch khota nahi.
**Asli naam:** har user ka delivery record (store-and-forward, pending list), reconnect pe pending sab, sequence number se
client-side ordering, ordering = requirement ka faisla.

```mermaid
flowchart TB
    A["Arpan: kal party"]:::card
    R["Ravi: kahan? (usi second)"]:::card
    X["Phone ki ghadi 2 second aage-peeche:<br/>phone number lagaye to clash"]:::bad
    subgraph SN["SERVER LAGAYEGA NUMBER"]
        N(["Jisne pehle bheja, pehla:<br/>#980, #981 (sequence)"]):::pill
    end
    P[("Har user ka record: ye gaya,<br/>ye nahi; jo nahi gaya wo sab do")]:::good
    A --> N
    R --> N
    X -.-> N
    N --> P
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style SN fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 8. "12 naye message" (ek ginti)

Ravi ko unread ginti dikhani hai, "12 naye message", group me 980 message, 500 member. To ek ginti rakh lo, Redis me ya
map me ya DB me: Ravi ne itne message padh liye. 968 tak padh liye, aakhri 980 hai, to neeche 12 aur hain, wo 12 ke 12
turant uske phone me daal do.

**Asli naam:** ginti = `last_read_seq` (kahan tak padha, har member ka har chat me ek number), 12 aur hain = unread =
aakhri number minus `last_read_seq` (980 - 968 = 12).

```mermaid
flowchart TB
    G["Group: aakhri #980"]:::card
    subgraph EG["EK GINTI RAKH LO"]
        L[("Ravi ne 968 tak padha<br/>(last_read_seq)")]:::pill
    end
    U["980 - 968 = 12 naye,<br/>12 ke 12 phone me"]:::good
    G --> U
    L --> U
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style EG fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 9. #975 kisne padha? 4.9 lakh line (number ghata lo)

Arpan ne #975 "kal party" bheja, Info me dekhna kis-kis ne padha. Har message x har member ki line = 980 x 500 = 4.9
lakh entry. Group me double tick aata hai, Info me jaake dekhte hain kisne padha. Sita ka number 978, iska matlab Sita
ne 975 padha, tabhi uski ginti 978 hui. Ravi ka 968, matlab wo bahut der se offline hai, ya pahuncha to usne ye message
read nahi kiya. Number ghata lo, isse pata pad jaayega kisne padha, kisne nahi.

**Asli naam:** ginti 978 matlab 975 padha = read watermark (number >= 975 to padha), number ghata lo = har member ka
bas ek number, 4.9 lakh ki jagah 500 number. Neeli tick tab jab sabka number >= 975.

```mermaid
flowchart TB
    X["Har message x har member:<br/>980 x 500 = 4.9 lakh line"]:::bad
    M["Arpan ka #975: kisne padha?"]:::card
    subgraph NG["NUMBER GHATA LO (har member ka ek number)"]
        S[("Sita: 978")]:::good
        R[("Ravi: 968")]:::bad
    end
    OK["978 >= 975: Sita ne padha<br/>968 < 975: Ravi ne nahi"]:::pill
    X -.-> M
    M --> S
    M --> R
    S --> OK
    R --> OK
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style NG fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 10. 200 MB ka video, chat ke taar jam (alag raasta, chhota URL)

Arpan ne group me party ka 200 MB ka video bheja. Ye chat server ke raaste DB me save hota aur 500 ko isi raaste
jaata. Taar chhote text ke liye bane the, video ne sab jam kar diya. Aaj 200 MB, kal koi 500 MB ya 1 GB bhi bhej
sakta. To ise yahan se hatao, alag raaste pe bhej do. Ek alag machine rakh lo: "tu video hai, tu idhar ja", hamari
chat block na kare. Video wahin pada rahe, uska ek chhota URL bana ke sabme bhej do, wo wahin se dekh lenge. Ek insaan
bitha do: "video aayi, mere paas aa ja, main chhota URL bana ke bhej deta hoon", chat server pe koi dikkat nahi.

**Asli naam:** alag machine = S3 (chat server ke bahar), "video aayi, mere paas aa ja" wala insaan = Media Service
(app ko S3 ka presigned upload URL deta, app video seedha S3 me daalti, phir chhota link banta), chhota URL = message
me sirf link, wahin se dekh lenge = video S3 / CDN se. 500 log dekhein to bhi video ki copy ek hi.

```mermaid
flowchart TB
    A["Arpan: 200 MB party video"]:::card
    X["Chat ke taar text ke liye:<br/>video ne sab jam kar diya"]:::bad
    subgraph AR["ALAG RAASTA, ALAG MACHINE"]
        S3[("Video wahin pada rahe<br/>(S3 / CDN)")]:::pill
    end
    C["Chat me sirf chhota URL,<br/>sab wahin se dekh lenge"]:::good
    A -.-> X
    A --> S3
    S3 --> C
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style AR fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 11. Phone band par "online" dikh raha (register + hilao)

Ravi ke naam ke neeche "online". Ravi ka phone lift me band, taar tootne ka signal server tak pahuncha hi nahi, 2
ghante se Ravi "online" dikh raha, phone band pada. To ek register me likh liya "Ravi online hai". Ab har 3-5 second
baad Ravi ko hilaunga, haath maarunga: "Ravi, tu online hai?" "Haan bhai, online hoon, pareshaan mat kar." Phir haath
maara, jawab nahi diya. Phir maara, phir nahi. 2-3 baar hilaya aur jawab nahi diya, to register se Ravi ka naam kaat
diya, ab wo offline.

**Asli naam:** register = presence entry (Redis), har 3-5 second hilana = heartbeat, 2-3 baar jawab nahi to naam
kaato = entry ki umar (TTL), heartbeat chhoote to apne aap mite. Aakhri heartbeat ka time = "last seen".

```mermaid
flowchart TB
    X["Ravi ka phone lift me band,<br/>2 ghante se online dikh raha"]:::bad
    subgraph RG["REGISTER: RAVI ONLINE HAI (Redis)"]
        H{"Har 3-5 second hilao:<br/>tu online hai? (heartbeat)"}:::pill
    end
    OK["Haan bhai: online"]:::good
    OFF["2-3 baar jawab nahi:<br/>naam kaat diya, offline (TTL)"]:::card
    X --> H
    H -- "jawab aaya" --> OK
    H -. "jawab nahi" .-> OFF
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style RG fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 11b. Neha online aayi, khabar 500 contacts ko? (sirf dukaan me aaye ko)

Neha ne app khola, wo online ho gayi. Uske phone me 500 contacts. Seedha socho to server 500 ko bata de, par lakhon log
har minute online / offline hote, har baar 500-500 khabar ka toofan, aur 500 me se zyada tar ne Neha ka naam us waqt
screen pe dekha hi nahi. WhatsApp mujhe notification bhej ke thodi batayega ki dost online hai. Jisne Neha ki chat
khol rakhi hai, bas use dikhao: 500 me se 10 ne chat kholi, to 10 ko. Jaise gaon me 500 log aur ek electronics ki
dukaan. Nayi LED aayi, to dukaan wala 500 ke ghar nahi jaata, na bahar board laga ke raah chalte ko dikhata. Jo dukaan
ke andar aaya, "aaiye sir, ye nayi LED aayi hai", usi ko dikhata hai. Aur main server se baar-baar poochta nahi ki Neha
online hai. Taar to pehle se khula hai (kadam 1). Maine chat kholi to bas server ko pata lag gaya, ab wo usi khule
taar pe mujhe khud bata dega. Dono taraf se taar khula hai.

**Asli naam:** dukaan ke andar aaya = chat kholte hi phone server ko bolta "Neha ka status batate rehna" = subscribe,
chat band = unsubscribe. Neha online hui (kadam 11 ka register badla) to server sirf subscribers ko bataye = presence
pub/sub. Isliye kisi ka online dekhna ho to uski chat kholni padti hai.

```mermaid
flowchart TB
    N["Neha online aayi<br/>(500 contacts)"]:::card
    subgraph DK["DUKAAN: JINHONE NEHA KI CHAT KHOLI (subscribe)"]
        S{"Sirf in 10 ko batao:<br/>Neha online (pub/sub)"}:::pill
    end
    Y["10 log jinki chat khuli:<br/>online dikha"]:::good
    X["Baaki 490: kuch nahi bheja<br/>(chat kholenge tab dikhega)"]:::bad
    N --> S
    S -- "chat khuli" --> Y
    S -. "chat band" .-> X
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style DK fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 12. "Nahi, parso" pehle, "kal milte hain" baad me (phone number se kram lagaye)

Ravi ne Arpan ko "kal milte hain" phir "nahi, parso" bheja. Pehla server S1 se gaya, doosra S2 se, S1 dheema tha.
Arpan ke phone pe pehle "nahi, parso" aaya, phir "kal milte hain", Arpan galat samjha. To pehle to koshish karo ki ek
chat ke dono message ek hi server se jaayein. Phir bhi alag server se gaye, to bhi hume pata hai server pe pehle kaun
aaya tha: "kal milte hain" pehla, "nahi, parso" doosra. Pehle aaya wo pehle dikhe, aisa mat rakho. Jab dono phone pe
pahunch jaayein, phone unhe sahi kram me laga ke dikhaye. Aur S1 bahut dheema ho to S2 wale ko rok ke nahi rakhenge,
message to jaana hi hai. Thodi der ulta dikh sakta hai, phir apne aap theek ho jaata, WhatsApp me bhi aisa dekha hai.
Perfect jawab nahi hai.

**Asli naam:** server pe pata kaun pehle aaya = har chat ka sequence number (kadam 7), phone sahi kram me lagaye =
client number se sort kare, aane ke kram se nahi. Dheeme ke liye doosra mat roko = deliver turant, baad me reorder.

**Maan lo interviewer kheeche: number kaun lagayega, aur ye kabhi khatam to nahi hoga:** message server pe jis time
aaya, usi time ko number bana do; time hamesha badhta hai, to number bhi, aur phone usi se kram lagata hai. Ghadi kaunsi? Har group ka apna ek
server hota hai: 500 logon ka group hamesha server 7 pe jaata hai, to number server 7 ki ghadi se lagega; ek group ke do
message alag-alag server pe nahi jaate. 10 din baad group server 20 pe gaya, to ab server 20 ki ghadi. Par "kabhi
khatam nahi hoga" jaisa daava mat karo: interviewer "maan lo 50 saal me bhar gaya to?" kheench sakta hai aur point
wapas lena padta hai. Seedha dhaancha bolo: har message ko ek kram number, phone usi se lagata hai.

**Asli naam:** time se
number = Snowflake ID (time + machine + chhota counter, FOUNDATIONS 13), ya har chat ka sequence number; ek chat hamesha ek hi server pe (chat id se server tay, kadam 12), server
badle to wahan number pichhle aakhri number ke aage se shuru. Had yaad ho
to bonus: long (64-bit) me lagbhag 9.2 x 10^18 (9 ke baad 18 zero), counter har chat ka alag; warna "padhte waqt
dekha tha, baaki net se dekh lete".

**Maan lo group me 500 log, har message pe 500 logon ka server dhoondhna padega:** 500 baar kyun bhejna. Ek taar-server
se 10 log jude hon ya 500, chat server us server pe message ek hi baar bhejta hai, aage wo server apne jude logon ko de
deta hai. Do alag dabbe: taar wala server = WebSocket gateway (jahan phone juda), group wala server = chat server (jo
number lagata).

**Asli naam:** per-server fan-out, Redis pub/sub (har gateway group ka channel sunta, chat server ek baar daalta).

```mermaid
flowchart TB
    R["Ravi: kal milte hain, phir nahi parso"]:::card
    S1["S1 (dheema): kal milte hain"]:::bad
    S2["S2: nahi, parso"]:::card
    subgraph PK["PHONE SAHI KRAM ME LAGAYE"]
        N(["Server ka number: kal = pehla,<br/>parso = doosra"]):::pill
    end
    A["Arpan: thodi der ulta,<br/>phir apne aap theek"]:::good
    R --> S1
    R --> S2
    S1 --> N
    S2 --> N
    N --> A
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style PK fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

---

Notes (Arpan ke shabd) = [CHAT_HAND_NOTES.md](CHAT_HAND_NOTES.md)
