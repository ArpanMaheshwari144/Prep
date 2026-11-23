# Google Docs: Arpan aur Ravi ke "Trip plan" doc ki kahani

Arpan aur Ravi ek hi doc "Trip plan" pe saath likh rahe hain. Doc me hai `Goa jaayenge`. Kaam: dono ek saath likhein
to kisi ka likha gayab na ho, dono ki screen pe ek hi doc dikhe, aur "Saved" dikha to likha hua ude nahi (bas dabba aur
uski copy dono ek saath gire to aakhri kuch second, kadam 5).

Har kadam = **kahani** (dikkat aur hal, bolne wali bhasha) -> **Asli naam** (interview me yahi shabd) -> tasveer
(kahani ka shabd, bracket me asli naam).

---

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Arpan / Ravi  -->  WebSocket  -->  doc ka owner server (ek doc = ek maalik, ek version)
                                       |
                    har badlaav = op (insert / delete), OT se khiska ke sabka doc SAME
                    op Redis buffer (+ copy) me, kuch der baad batch me DB · har kuch hazaar op pe photo (snapshot)
                    permission badli to sab server ko awaaz (pub/sub), har op pe haq dekho

 1   Save pe poora text, Arpan ki line gayab     ->  poora text nahi, op bhejo (insert / delete)
 1b  "budget 20k" ka op dobara gaya              ->  op id dekh ke chhodo (idempotency)
 2   laakhon save, har badlaav DB pe             ->  chhota register, baad me batch me DB (Redis buffer)
 2b  2 saal purana doc, 50 lakh op               ->  doc ki photo + baad ke op (snapshot)
 3   "Hum" aur "kal", kal galat jagah            ->  shabdon ko khiska do (OT)
 3b  Ravi ne B hataya, Arpan ne E joda           ->  dono ka doc same: A E C D (OT)
 3c  "jo pehle aaya taala laga de"               ->  phir sharing ka matlab nahi (locking nahi)
 4   lakhon doc, do server, do shakal            ->  har doc ka ek maalik (owner server)
 5   "Saved" dikhaya, DB se pehle server gira    ->  dabbe ki copy (Redis copy); owner gira to naya owner wahin se
 5b  dabba + copy dono gire                      ->  disk pe bhi; phir bhi gire to kuch second chhoot
 5c  poore data center ki bijli gayi             ->  copy doosre shehar (multi-region)
 6   Ravi hataya, server pe purani permission    ->  sabko awaaz (pub/sub), har op pe haq dekho
 6b  Ravi offline likhta raha                    ->  popup "hata diya", purane edits phenko
 7   server 7 ne awaaz suni hi nahi              ->  wapas aake note dekh (reconnect)
```

## FR / NFR / Numbers (interview me pehle 2 minute)

```
FR      kai log ek doc ek saath edit karein · doosre ka likha turant dikhe, kisi ka likha mite nahi
        · kaun edit kar sakta, hataya to turant band
        · scope se bahar: comments, version history, rich text formatting; ek doc pe aam taur pe 2-3 likhne wale,
          max ~100                                                           -> kadam 4 (owner server kaafi)
NFR     typing turant, ruke nahi · aakhir me sabka doc SAME · "Saved" dikha to ude nahi (sab copy ek saath gire to aakhri kuch second)
        · likhna = AP (net gaya to bhi typing chale, baad me sync); permission = CP (jiska haq hata, wo kabhi na likh
          paaye; net aate hi server check, haq nahi to operation reject)                      -> kadam 6, 6b
Numbers ~10 crore user, roz ~3-4 crore · har akshar ek badlaav
        -> likhna bahut zyada: har akshar pe DB nahi chal sakta
```

**FR / NFR -> kadam (Arpan: "achha lag raha, aise hi rehne do"):** kai log ek saath edit = 1, 3, 3b, 4 · doosre ka likha
turant dikhe, kisi ka mite nahi = 1, 1b, 3, 3c · kaun edit kar sakta, hataya to turant band = 6, 6b, 7 · NFR typing turant =
2 · aakhir me sabka doc SAME = 3, 3b, 4 · "Saved" dikha to ude nahi = 5, 5b, 5c.

---

## 1. Ravi ke Save ne Arpan ki line gayab ki (poora text nahi, operation bhejo)

Jo "Save" dabata hai, uska poora text server pe jaake purane ko hata deta hai. Maine upar "Goa jaayenge" likh ke Save
kiya, Ravi ne neeche "budget 20k" likh ke Save kiya, par Ravi ki screen pe purana text tha, to meri line gayab. Dono ka
text dikhao, kisi ko gayab kyun karna. Ye to last write wins wali situation ho gayi, aur Google Docs me last write wins
nahi chalne ka, warna Google Docs ka matlab hi khatam. Poora text bhejenge to overwrite hoga hi, to poora text mat
bhejo, operation bhejo: insert, delete, add. Ravi ne kuch kiya to uska operation aayega "is letter ke baad add kar do",
aur main use "Goa jaayenge" ke baad, ya neeche, ya beech me kahin, wahin laga dunga.

**Asli naam:** last write wins nahi = dono ka badlaav bache, poora text nahi, operation bhej do = operation (op:
insert / delete), is letter ke baad add kar do = op me jagah (position) + kis version pe dekh ke likha (baseVersion). Server se dono ki screen tak op
turant = WebSocket (dono taraf khula taar).

**Maan lo Ravi ka "budget 20k" wala op dobara chala gaya ("mil gaya" kho gaya):** Har op ki apni id: server pehle id dekhe, ye op lag chuka to dobara mat lagao. Doc me "budget 20k budget 20k" nahi.
**Asli naam:** idempotency key (wahi ID dobara aaye to chhodo).

**Misaal:** Jira comment / Atlassian me "abhi kuch change hua" bhi version number se. **Maan lo Ravi ko dikh hi nahi raha:**
HTTP ek tarafa, har baar refresh; WebSocket se turant push; toota to judte hi saare ek saath (WhatsApp jaisa).
**Asli naam:** version check, WebSocket push, reconnect pe missed ops.

```mermaid
flowchart TB
    X["Poora text bheja: Arpan ki line gayab<br/>(last write wins, nahi chalega)"]:::bad
    A["Arpan: Goa jaayenge"]:::card
    R["Ravi: budget 20k"]:::card
    subgraph OP["POORA TEXT NAHI, OPERATION BHEJO"]
        S(["insert / delete / add:<br/>is letter ke baad add kar do"]):::pill
    end
    D["Dono ka text dikhe"]:::good
    A --> S
    R --> S
    S --> D
    X -.-> S
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style OP fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 2. Har chhota badlaav DB pe (chhota register, kuch der baad DB)

Hazaron-laakhon log dhadadhad save kar rahe, aur har chhota badlaav DB pe jaaye to kitna DB write karega, Cassandra ki
bhi haalat ho jaayegi. Main hi Google Docs hoon: log baar-baar hata rahe, badal rahe, check kar rahe. To main apna ek
chhota register bana lunga, unki saari baatein usme likhta rahunga, phir kuch der baad DB me save kar dunga. Har write
pe DB query nahi maarunga, warna DB baith jaayega.

**Asli naam:** chhota register, saari baatein likhta rahunga = buffer (server ki apni memory, Redis kadam 5 me), kuch der baad DB me save = batch write
(Cassandra), har write pe DB query nahi = har akshar pe DB nahi. Cassandra kyun: op log me sirf aage jodna, padhna
hamesha "is doc ke op, version ke kram se"; key `(docId, version)`; likhai bahut tez, machine jodo to data khud bat-ta;
join / transaction ki zarurat nahi, isliye Postgres nahi.
**DB kaunsa (Arpan):** Docs me padhna kam, likhna pelam pel, har banda har akshar pe likh raha. Cassandra me leader
nahi, machine jodo to data apne aap bat jaata. Copy sirf padhai badhati, likhai ke liye tod ke baanto.

**Maan lo trip plan doc 2 saal purana, op log me 50 lakh op; Ravi ne aaj khola to kya server saare op shuru se lagayega:** Ye Banking ke master register wali baat hai: May wale ko 1 May ka balance chahiye to shuru se nahi jodte, master register se uthate hain. Waise hi har kuch hazaar op pe poore doc ki ek photo save kar lo. Ravi doc khole to sabse nayi photo uthao aur sirf uske baad ke thode op lagao. Doc turant khulega.
**Asli naam:** poore doc ki photo = snapshot, photo + baad ke op = snapshot + op log replay.

```mermaid
flowchart TB
    U["Lakhon log dhadadhad save"]:::card
    S["Main hi Google Docs hoon"]:::card
    subgraph CR["APNA CHHOTA REGISTER<br/>(buffer)"]
        B[("Saari baatein likhta rahunga<br/>(Redis buffer)")]:::pill
    end
    D[("Kuch der baad DB me save<br/>(Cassandra, batch)")]:::good
    X["Har write pe DB query:<br/>DB baith jaayega"]:::bad
    U --> S
    S --> B
    B -- "kuch der baad" --> D
    S -.-> X
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style CR fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 3. Ek hi second me dono ne likha, Ravi ka "kal" galat jagah (shabd khiska do)

Doc me `Goa jaayenge`. Ek hi second me maine shuru me "Hum " joda, aur Ravi ne "Goa" ke baad (jagah 3) " kal" joda.
Server pe mera pehle pahuncha, phir Ravi ka "jagah 3" lagaya to ban gaya `Hum kal Goa jaayenge`, Ravi ka kal galat
jagah. Maine likha "Hum Goa jaayenge", usne likha "Goa kal jaayenge", to poora ban jaayega "Hum Goa kal jaayenge".
Uska kal galat jagah nahi aur hamara Hum bhi galat jagah nahi, humne shabdon ko khiska diya. Main yaad rakhun ki Arpan
ne Goa se pehle Hum likha tha, to wahan Hum likh dun; Ravi ne Goa ke baad kal likha tha, to "Hum Goa kal jaayenge".
Koi dikkat hi nahi.

**Asli naam:** shabdon ko khiska diya = OT (Operational Transformation), yaad rakhun kisne kahan likha = op ki jagah
pichhle op ke hisaab se aage (op jis version pe bana, uske baad lage op ke hisaab se position aage-peeche). Kram
server tay karta, jo pehle pahuncha use agla version.

**Aur neeche (ek ne hataya, doosre ne usi pal joda, A B C D):** dono ke paas A B C D. Ravi ne B hataya, Arpan ne C se pehle E joda. Dono ke
document bilkul same laga do: B hat jaayega, B ki jagah E aake lag jaayega, dono ke paas A E C D, na kisi ka kuch khoya.
**Asli naam:** wahi OT (Ravi ka delete pehle laga, to Arpan ke E ki jagah ek peeche khiski).
```
             ARPAN (US)                          RAVI (India)

shuru        0  A                                0  A
             1  B                                1  B
             2  C                                2  C
             3  D                                3  D

apna kaam    insert(2, E) = C se pehle E         delete(1) = B hatao

             0  A                                0  A
             1  B                                1  C
             2  E                                2  D
             3  C
             4  D

doosre ka    Ravi ka delete(1) aaya:             Arpan ka insert(2, E) aaya, bina OT:
aaya         0  A                                0  A
             1  E                                1  C
             2  C                                2  E   <- GALAT, E ko C se pehle aana tha
             3  D                                3  D

OT ke saath                                      delete jagah 1 pe hua, insert jagah 2 pe (usse aage),
                                                 to insert ek peeche khiska do = insert(1, E)
                                                 0  A
                                                 1  E
                                                 2  C
                                                 3  D   <- dono ke paas A E C D
```

**Maan lo dono ne 0 pe likha:** tie-break user id + timestamp, dono ko same. **OT ke alawa:** CRDT, har akshar ki id,
merge commutative, central transform nahi. **Asli naam:** deterministic tie-break, CRDT.

```mermaid
flowchart TB
    A["Arpan: shuru me Hum"]:::card
    R["Ravi: Goa ke baad kal"]:::card
    subgraph KH["SHABDON KO KHISKA DIYA"]
        S(["Yaad rakhun kisne kahan likha<br/>(OT)"]):::pill
    end
    F["Hum Goa kal jaayenge:<br/>na kal galat jagah, na Hum"]:::good
    A --> S
    R --> S
    S --> F
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style KH fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

```mermaid
flowchart TB
    S["Shuru, dono ke paas:<br/>A<br/>B<br/>C<br/>D"]:::card
    RD["Ravi: delete(1) = B hatao"]:::card
    AI["Arpan: insert(2, E) = C se pehle E"]:::card
    AS["Arpan ki screen, delete(1) ke baad:<br/>A<br/>E<br/>C<br/>D"]:::good
    RW["Ravi ki screen bina OT, insert(2) ke baad:<br/>A<br/>C<br/>E  galat jagah<br/>D"]:::bad
    subgraph OT["SHABDON KO KHISKA DIYA (OT)"]
        T(["delete jagah 1 pe, insert 2 pe:<br/>insert ek peeche, insert(1, E)"]):::pill
    end
    RR["Ravi ki screen OT ke saath:<br/>A<br/>E<br/>C<br/>D<br/>(dono same)"]:::good
    S --> RD
    S --> AI
    AI --> AS
    RD -.-> RW
    RD --> T
    T --> RR
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style OT fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

**Maan lo koi bole: jo pehle aaya wo taala laga de, doosra ruke:** phir Google Docs ka matlab hi nahi, wo sharing
document hai. Taala lagaya to doosra bas dekhta rahega, jaise YouTube upload pe hi mana kar de. Tum likho, Docs apne
hisaab se aage-peeche karta.
**Asli naam:** locking (ek likhe, baaki ruke) mil ke likhne me nahi chalta; OT me sab ek saath, kram server ka.


## 4. Do server, ek doc ki do shakal (har doc ka ek centralized maalik)

Lakhon doc, 100 server. Main server A se, Ravi server B se. Dono server apne-apne kram me OT lagaate, to ek hi doc ki
do alag shakal. To ek centralized system bana do. Do ladke apne-apne server pe likh rahe, main Google Docs hoon: dono
ka likha mere tak aaya, main dono ka sentence padh ke sahi se likh dunga, "Hum Goa kal jaayenge", aur dono ki screen
pe wahi, ek hi version. Par sabka ek nahi hoga, har doc ke liye alag. Main aur Ravi ek doc pe to hum dono ka ek
centralized system; 100 log ek doc pe to un 100 ka ek; 1 lakh log to unka ek. Us centralized system ka koi na koi
maalik hoga, sab uske hisaab se chalega.

**Asli naam:** centralized system, ek hi version = ek jagah kram aur OT, har doc ka alag centralized system, uska
maalik = har doc ka ek owner server (LB `docId` dekh ke ring pe owner chunta = consistent hashing, server jude / gire to sirf paas ke doc hilein, ek doc ke saare editor wahin).

Aur neeche (Arpan Mumbai server pe, Ravi Delhi server pe, centralized kaise): pehle saare op apne paas jama karo (Redis,
abhi DB nahi). Arpan ne jagah 0 pe "ARPAN" likha, Ravi ne jagah 2 pe "RAVI". Dono op ek jagah aaye: pehle ARPAN, phir
Ravi jagah 2 pe, jahan "P" hai. Arpan ka kaat ke Ravi ka likhna, ya "AR" + "RAVI" likh ke "PAN" kho dena, dono galat. To
"P" ko khiska do, beech me poora "RAVI": "AR RAVI PAN". Jaise main Madhya Pradesh me beech me baitha, Ravi Delhi se, Arpan
Mumbai se; dono ka mere paas aaya, merge karke dono ko ek jaisa dikha diya. Upar se har likhne wale ki rang wali line
(cursor) bhi dikhti.
**Asli naam:** owner server pe OT (position khiskana), live cursor (presence).

**Maan lo 1000 log ek doc pe:** OT sirf editors ka, 900 viewer ho to bhi dikkat nahi. **Asli naam:** editors pe transform,
viewers ko fan-out.

**Aur neeche (badlaav doosre server tak kaise):** 50 lakh connection ek server nahi jhelta, LB ke peeche kai server. Main server 1 pe likh raha, Ravi server 2 pe padh raha, to server 1 badlaav Redis pub/sub pe daale, wo saare server pe bhej de.
**Asli naam:** WebSocket servers + Redis pub/sub fan-out.

```mermaid
flowchart TB
    A["Arpan (server A)"]:::card
    R["Ravi (server B)"]:::card
    subgraph CS["HAR DOC KA EK CENTRALIZED SYSTEM"]
        O(["Maalik: dono ka padh ke<br/>sahi se likhe (owner server)"]):::pill
    end
    V["Dono ki screen pe ek hi version"]:::good
    A --> O
    R --> O
    O --> V
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style CS fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 5. "Saved" dikha, server gira, para uda (dabba aur uski copy)

Owner server ne 5 second ke badlaav register (apni memory) me jama kiye, mujhe "Saved" dikha diya. DB me likhne se
pehle server gira, aur mera poora para uda. To use ek chhoti memory me likh do. Chal maan le mere paas ek dabba hai, us
dabbe me saari information store kar raha. Kal wo dabba mar jaaye ya gir jaaye, to us dabbe ki ek aur copy bana dunga.
Ya main dabbe ko bolunga: ye meri information tu bhi store kar le. Kal mujhe kuch ho jaaye to tu hai, tujhe kuch ho
jaaye to main hoon.

**Aur neeche (maalik server gira, Arpan):** Maalik server gira to kaam rukega, isliye uski copy saath: main chhutti pe
jaun ya tabiyat kharab ho to thode din tu sambhal lena, tu meri copy hai. Aur doc DB me hai, naya server wahan se utha lega.
**Asli naam:** ek dabba, saari information store = Redis buffer, us dabbe ki ek aur copy = Redis ki copy (replica),
"Saved" tab jab copy ne bhi likh liya (Redis copy apne aap thodi der baad likhti; "copy ne bhi likh liya" pakka karne ke liye WAIT); mujhe kuch ho to tu hai = owner gira to naya owner usi copy se uthaaye. DB
(Cassandra) me batch waise hi.

**Maan lo dabba aur copy dono gir gaye, batch abhi DB me nahi gaya:** dabba memory ke saath apni disk pe bhi likhe, gire
to disk se wapas (Kafka jaisa, line disk pe). Phir bhi sab gira to kuch second ki lines chhoot sakti hain, user dobara
likh lega. Aisa sochenge to design hi nahi banega: poori consistency chahiye to 2 kyun, 500 copy rakho, aur 500 ka
paisa bhi do. Interviewer dekhna chahta hai main sochta kaise hoon, har copy girana design nahi.
**Asli naam:** disk pe bhi = Redis AOF (persistence), kuch second chhoot = chhota data loss window, durability vs cost trade-off.

**Aur neeche ("Saved" se pehle Redis gira):** Saved se pehle Redis gira to bhi kho nahi jaata, bhejne wale ke paas op id hai, Redis aaye to wahi dobara bhej dega; dobara aaya to 1b wala niyam.
**Asli naam:** bina ack ke op dobara bhejo, op id se idempotency.
**Isi pe ek aur raasta (thodi der local me):** Ya Redis gira to thodi der server ke local cache me rakh lo, phir Redis me daal do.
**Asli naam:** temporary local buffer.

```mermaid
flowchart TB
    O["Maalik gira: Saved dikha tha"]:::bad
    subgraph DC["EK DABBA + USKI COPY<br/>(Redis buffer + replica)"]
        B[("Dabba: saari information<br/>(Redis buffer)")]:::card
        C[("Copy: tu bhi store kar le,<br/>Saved iske baad (replica)")]:::pill
    end
    D[("DB (Cassandra, batch)")]:::card
    N["Mujhe kuch ho to tu hai:<br/>naya maalik copy se"]:::good
    O --> B
    B --> C
    B -- "batch" --> D
    N -.-> C
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style DC fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

**Maan lo poore data center ki bijli gayi, dabba aur copy dono ek hi jagah the:** ek jagah mat rakho. Arpan ka data
Mumbai me, copy Delhi ke data center me; Mumbai gaya to Delhi se. (Infra ka sawaal, kam hi hota.)
**Asli naam:** multi-region replica (alag data center me copy).

## 6. Ravi hata, phir bhi 2 minute likhta raha (memory me note + tez bhaagne wala bachcha)

Main doc ka maalik, maine Ravi ko hata diya. Par Ravi ke server ke paas permission ki purani copy, "Ravi = editor", to
hatne ke baad bhi Ravi 2 minute likhta raha. To main apni memory me chhota note daal dunga: ye banda editor nahi, iska
koi bhi operation aaye, accept nahi karunga. Par Ravi alag server pe hai, to main ek parchi rakhunga "Ravi ab editor
nahi", aur wo parchi ek tez bhaagne wale bachche ko dunga: jahan bhi Ravi ki permission hai wahan sab ja aur bata de
"Ravi ab editor nahi". Ek centralized authority hai jo sab likh rahi. Usne mana kiya to chhota banda building ke bahar
khada hoke chilla de "suno, Ravi editor nahi", to Ravi jis bhi server pe ho, wahan se uska editor note hat chuka hoga.
Bahut se answer ho sakte, ek sahi answer nahi: Redis pub/sub bolo ya building ke bahar koi chilla de.

**Asli naam:** centralized authority = permission ka asli sach (SQL: kaun, kis doc pe, kya kar sakta), memory me note =
server pe permission cache + har op pe check (har akshar pe SQL nahi), iska operation accept nahi = op reject, tez bhaagne wala bachcha / building ke bahar chilla de = pub/sub invalidation (Redis pub/sub).

**Maan lo Ravi hatne ke waqt offline tha:** laptop pe likhta raha, aadhe ghante baad net aaya aur saare edits ek saath aaye. Net aate hi
Ravi ko popup "tumhe owner ne hata diya". Server pe edit seedha nahi lagta, har edit pe dekho bhejne wale ke paas abhi edit ka haq hai ya nahi;
tha to merge, nahi to saare edits phenk do. Alag rakhna ho (Ravi baad me wapas mile) to wo FR ki baat, interviewer se poochh ke.
**Asli naam:** popup = permission revoked event, har edit pe haq = har op pe server-side permission check, phenk do = op reject, alag rakhna = rejected ops ka log (FR pe).


```mermaid
flowchart TB
    M["Arpan: Ravi ko hatao"]:::card
    subgraph CA["CENTRALIZED AUTHORITY"]
        Q[("Sab likh rahi: Ravi editor nahi<br/>(SQL)")]:::pill
    end
    P(["Tez bhaagne wala bachcha:<br/>building ke bahar chilla de<br/>(pub/sub)"]):::pill
    S1["Ravi ka server: note hata,<br/>op accept nahi"]:::bad
    S2["Baaki server: note hata"]:::good
    M --> Q
    Q --> P
    P --> S1
    P --> S2
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style CA fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 7. Server 7 ne awaaz suni hi nahi (server ki copy note kar leti)

Ladka chillaya "Ravi editor nahi", par server 7 us pal restart / net se bahar tha, awaaz suni hi nahi. Pub/sub dobara
nahi chillata. Server 7 ki memory me abhi bhi "Ravi = editor", aur Ravi wahin se likh raha. Par server akela nahi,
uski copy saath khadi hai. Server bola tha: "main thodi der bahar ja raha, kuch bhi ho to note karke likh lena, wapas
aake tujhse dekh lunga, tu meri copy hai." Ladka chillaya "Ravi editor nahi", copy ne likh liya. Server wapas aaya:
"kya hua?" "Bas ek awaaz thi, Ravi editor nahi." Achha, to Ravi ki permission kaat do, khatam kahani.

**Asli naam:** server ki copy jo peeche note kare = standby / replica server bhi pub/sub sunta (pub/sub ek hi baar bolta, chhoote to khoya), wapas aake copy se
dekh lunga = reconnect pe chhoote hue badlaav copy (ya SQL) se le ke note theek, phir hi Ravi ka op maano.

```mermaid
flowchart TB
    P["Bachcha chillaya: Ravi editor nahi"]:::card
    S7["Server 7 bahar tha,<br/>awaaz nahi suni"]:::bad
    subgraph CP["SERVER KI COPY SAATH KHADI<br/>(standby / replica server)"]
        C(["Note karke likh liya:<br/>Ravi editor nahi (standby)"]):::pill
    end
    W["Wapas aaya: kya hua?<br/>Ravi ki permission kaat do"]:::good
    P --> C
    S7 --> W
    C --> W
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style CP fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

---

## Poora dhaancha ek nazar me

```mermaid
flowchart TB
    U(["Arpan / Ravi"]):::pill -- "WebSocket" --> O["Doc ka owner server<br/>ek doc = ek maalik, ek version"]:::pill
    O --> OT["Har badlaav = op,<br/>OT se sabka doc SAME"]:::card
    OT --> R[("Redis buffer + copy")]:::card
    R -- "batch" --> DB[("DB + snapshot")]:::card
    P["Permission badli"]:::card -- "pub/sub" --> O
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
```

---

Notes (Arpan ke shabd) = [DOCS_HAND_NOTES.md](DOCS_HAND_NOTES.md)
