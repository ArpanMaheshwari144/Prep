# Google Docs: hand notes (Arpan aur Ravi, "Trip plan" doc)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan aur Ravi ek hi doc "Trip plan" pe saath likh rahe hain. Doc me hai `Goa jaayenge`.

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

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      kai log ek doc ek saath edit karein · doosre ka likha turant dikhe, kisi ka likha mite nahi
        · kaun edit kar sakta, hataya to turant band
        · scope se bahar: comments, version history, rich text formatting; ek doc pe aam taur pe 2-3 likhne wale,
          max ~100 (Arpan 7-Oct)                                             -> kadam 4 (owner server kaafi)
NFR     typing turant, ruke nahi · aakhir me sabka doc SAME · "Saved" dikha to ude nahi (sab copy ek saath gire to aakhri kuch second)
        · likhna = AP (net gaya to bhi typing chale, baad me sync); permission = CP (jiska haq hata, wo kabhi na likh
          paaye; net aate hi server check, haq nahi to operation reject) (Arpan 22-Oct)       -> kadam 6, 6b
Numbers ~10 crore user, roz ~3-4 crore · har akshar ek badlaav
        -> likhna bahut zyada: har akshar pe DB nahi chal sakta
```

**FR / NFR -> kadam (Arpan: "achha lag raha, aise hi rehne do"):** kai log ek saath edit = 1, 3, 3b, 4 · doosre ka likha
turant dikhe, kisi ka mite nahi = 1, 1b, 3, 3c · kaun edit kar sakta, hataya to turant band = 6, 6b, 7 · NFR typing turant =
2 · aakhir me sabka doc SAME = 3, 3b, 4 · "Saved" dikha to ude nahi = 5, 5b, 5c.

&nbsp;

**1. Jo "Save" dabata hai, uska poora text server pe jaake purane ko hata deta hai. Arpan ne upar "Goa jaayenge" likh ke Save kiya, Ravi ne neeche "budget 20k" likh ke Save kiya. Ravi ki screen pe purana text tha, to Arpan ki line gayab.**
Arpan: Dono ka text dikhao, kisi ko gayab kyun karna. Ye to last write wins wali situation ho rahi, Google Docs me last write wins nahi chalne ka, warna Google Docs ka matlab hi khatam. Poora text nahi bhej sakte, poora bhejenge to overwrite ho jaayega. Operation bhej do: insert, delete, add. Maine likha "Goa jaayenge", Ravi ne kuch kiya to uska operation bhejenge: is letter ke baad add kar do. Main Ravi ka operation laaunga: isne ye likha to "Goa jaayenge" ke baad budget likh dunga, ya neeche, ya beech me kahin. Google andar se kaise karta, wahan jaana bhi nahi chahta, main khud kaise karunga wo dekh raha.
Technical me: "last write wins nahi" = dono ka badlaav bache, "poora text nahi, operation bhej do: insert, delete" = operation (op) bhejna, "is letter ke baad add kar do" = op me jagah (position).

**Maan lo Ravi ka "budget 20k" wala op dobara chala gaya ("mil gaya" kho gaya).**
Arpan: Har op ki apni id: server pehle id dekhe, ye op lag chuka to dobara mat lagao. Doc me "budget 20k budget 20k" nahi.
Technical me: idempotency key (wahi ID dobara aaye to chhodo).
Arpan (misaal, 22-Oct): Har operation version number ke saath jaata, jaise Atlassian me: save karne jaate to mana kar deta "abhi yahan kuch change hua hai"; Jira comment me bhi kai baar comment save nahi hota, wo bhi version number se hota hai.
**Maan lo Ravi ko Arpan ka likha dikh hi nahi raha, refresh pe bhi nahi:** Arpan: Hum HTTP use kar rahe the, HTTP ek tarafa hai, server Ravi ko khud nahi bhejega, har baar refresh karna padega. Peeche WebSocket laga dunga: change hua, turant push, two-way. WebSocket toota to jaise hi connection judega wo apne aap dekh lega aur turant push kar dega, same as WhatsApp: offline hue, online aaye to saare message ek saath.
Technical me: version number = optimistic check (Jira / Confluence wala "kisi ne badla"), "HTTP ek tarafa" = request-response, "WebSocket two-way push", "judte hi chhoote ek saath" = reconnect pe missed ops replay (last version se aage).

**2. (Ye dikkat Arpan ne khud uthayi) Hazaron-laakhon log dhadadhad save kar rahe, har chhota badlaav DB pe jaaye to?**
Arpan: Sab save DB pe jaayenge kya, kitna DB write karega, Cassandra ki bhi haalat ho jaayegi. Main hi Google Docs hoon: wo baar-baar hata rahe, badal rahe, check kar rahe. To main apna ek chhota register bana lunga, unki saari baatein likhta rahunga, phir kuch der baad DB me save kar dunga. Har write pe DB query nahi maarunga, warna DB baith jaayega.
Technical me: "chhota register, saari baatein likhta rahunga" = buffer (server ki apni memory; gira to udega, isliye kadam 5 me Redis + copy), "kuch der baad DB me save" = batch write (Cassandra), "har write pe DB query nahi" = har akshar pe DB nahi.

**Maan lo trip plan doc 2 saal purana, op log me 50 lakh op; Ravi ne aaj khola to kya server saare op shuru se lagayega.**
Arpan: Ye Banking ke master register wali baat hai: May wale ko 1 May ka balance chahiye to shuru se nahi jodte, master register se uthate hain. Waise hi har kuch hazaar op pe poore doc ki ek photo save kar lo. Ravi doc khole to sabse nayi photo uthao aur sirf uske baad ke thode op lagao. Doc turant khulega.
Technical me: poore doc ki photo = snapshot, photo + baad ke op = snapshot + op log replay.

**3. Doc me `Goa jaayenge`. Ek hi second me Arpan ne shuru me "Hum " joda, Ravi ne "Goa" ke baad (jagah 3) " kal" joda. Server pe Arpan ka pehle pahuncha, Ravi ka "jagah 3" lagaya to ban gaya `Hum kal Goa jaayenge`, Ravi ka kal galat jagah.**
Arpan: Maine likha "Hum Goa jaayenge", usne likha "Goa kal jaayenge", to poora ban jaayega "Hum Goa kal jaayenge". Uska kal galat jagah nahi aur hamara Hum bhi galat jagah nahi. Humne shabdon ko khiska diya. Main yaad rakhun ki Arpan ne Goa se pehle Hum likha tha, to wahan Hum likh dun; Ravi ne Goa ke baad kal likha tha, to "Hum Goa kal jaayenge". Koi dikkat hi nahi.
Technical me: "shabdon ko khiska diya" = OT (Operational Transformation), "yaad rakhun kisne kahan likha" = op ki jagah pichhle op ke hisaab se aage.

**Isi dikkat ki doosri kahani: Arpan ke paas A B C D, Ravi ke paas bhi A B C D. Usi pal Ravi ne B hataya aur Arpan ne C se pehle naya hissa E joda.**
Arpan: Ravi ne B hataya, iska matlab Arpan ke paas bhi B hatega; Arpan ne E joda, to Ravi ke paas bhi aayega. Main hota to: achha, dono same document pe; ek ne B hataya, ek ne C ke upar E joda. To dono ke document bilkul same laga do: A E C D. Na kisi ka kuch khoya, na kisi ka. B to hat jaayega, aur B ki jagah E aake lag jaayega. Ravi ke document me A E C D, Arpan ke document me bhi A E C D, dono me same, koi dikkat wali baat nahi, aasaan hai.
Technical me: wahi OT: Ravi ka "B hatao" pehle laga, to Arpan ke E ki jagah ek peeche khisak gayi, dono jagah A E C D.
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

**Maan lo server ka ek banda bole: seedha hal hai, jo pehle likhne aaya wo doc pe taala laga de, doosra tab tak ruka rahe.**
Arpan: Phir Google Docs ka matlab hi nahi rahega; Google Docs ka matlab hi sharing document hai. Ek pe taala laga doge to saara matlab mitti paleet. Wahi baat hogi ki YouTube pe video upload karne gaya aur YouTube ne pehle hi mana kar diya. Taala laga ke baith jaoge to doosra banda bas dekhta rahega, jo galat hai. Google Docs me taala jaisa koi feature ho sakta hai, par usme hum nahi jaate, wo design ka part nahi. Tum likho, Google Docs apne hisaab se use aage-peeche karta hai.
Technical me: taala = locking (ek likhe, baaki ruke), mil ke likhne wale doc me nahi chalta; OT me sab ek saath likhte, kram server tay karta.
**Maan lo dono ne bilkul ek hi jagah (0 pe) likha:** Arpan: Dono ko ek hi dikhega, "XY Hello" ya "YX Hello", deterministic tie-break: user id aur timestamp pe, dono ko same.
**Maan lo pooche OT ke alawa raasta:** Arpan: CRDT doosra raasta: har character ki unique id aur position, merge kisi bhi kram me same (commutative), beech me central transform nahi. Implement nahi karna, bas samajhna aur bolna: CRDT bhi hai, OT bhi; humne OT explain kiya, CRDT aisa hi hai par thoda alag.
Technical me: same jagah = tie-break (userId / timestamp), CRDT = har akshar ki id, merge commutative, server pe transform nahi (offline ke liye achha).

**4. Lakhon doc, 100 server. Arpan server A se, Ravi server B se. Dono server apne kram me OT lagaate, ek hi doc ki do alag shakal.**
Arpan: Ek centralized system bana do. Do ladke apne-apne server pe likh rahe, main Google Docs hoon: dono ka mere tak aaya, main dono ka sentence padh ke sahi se likh dunga, "Hum Goa kal jaayenge", aur dono ki screen pe wahi, ek hi version.
Par sabka ek nahi hoga: har doc ke liye alag. Main aur Ravi ek doc pe to hum dono ka ek centralized system. 100 log ek doc pe to un 100 ka ek, 1 lakh log to unka ek. Us centralized system ka koi na koi maalik hoga, uske hisaab se chalega.
Arpan (doosri kahani, 16-Nov grill, Arpan Mumbai server se, Ravi Delhi server se): Pehle saare operation apne paas jama kar lunga, Redis me ya kahin bhi, abhi DB me nahi (DB baith jaayega). Arpan ne index 0 pe "ARPAN" likha, Ravi ne index 2 pe "RAVI". Dono operation Google Docs ke andar ek jagah aayenge, wahan dekha jaayega: pehle ARPAN, phir Ravi index 2 pe, wahan "P" hai. Noob sochega Arpan ka kaat ke Ravi ka overwrite kar do, ya "AR" + "RAVI" likh ke "PAN" kho do; dono galat, phir sharing doc ka matlab hi khatam. To "P" ko khiska do, beech me poora "RAVI" daal do: "AR RAVI PAN". Jaise main Madhya Pradesh me beech me baitha hoon, Ravi Delhi se, Arpan Mumbai se likh rahe: dono ka mere paas aaya, merge karke poora sentence dono ko ek jaisa dikha dunga. Upar se aajkal Docs me har likhne wale ki rang wali vertical line (cursor) dikhti hai, Jira me bhi.
Technical me: "centralized system, ek hi version" = ek jagah kram aur OT, "har doc ka alag centralized system, uska maalik" = har doc ka ek owner server.
**Maan lo ek doc pe 1000 log aa gaye:** Arpan: Editor owner pe hai, OT sirf likhne walon ka hai; viewer koi bhi ho, 900 viewer hain to bhi koi dikkat nahi.
Technical me: OT / transform sirf editors ke ops pe, viewers sirf broadcast sunte (fan-out), owner pe bojh likhne walon se.

**5. Owner server ne 5 second ke badlaav register (apni memory) me jama kiye, Arpan ko "Saved" dikha diya. DB me likhne se pehle server gira, Arpan ka poora para uda.**
Arpan: Use ek chhoti memory me likh de. Chal maan le mere paas ek dabba hai, us dabbe me saari information store kar raha. Kal wo dabba mar jaaye ya gir jaaye, to us dabbe ki ek aur copy bana dunga. Ya main dabbe ko bolunga: ye meri information tu bhi store kar le. Kal mujhe kuch ho jaaye to tu hai, tujhe kuch ho jaaye to main hoon.
Technical me: "ek dabba, saari information store" = Redis buffer, "us dabbe ki ek aur copy" = Redis ki copy (replica), "Saved" tab jab copy ne bhi likh liya (Redis copy apne aap thodi der baad likhti; "copy ne bhi likh liya" pakka karne ke liye WAIT); "mujhe kuch ho to tu hai" = owner gira to naya owner usi copy se uthaaye. DB (Cassandra) me batch waise hi.

**Maan lo dabba aur uski copy dono gir gaye, system restart, batch abhi DB me nahi gaya tha?**
Arpan: Dabba apni disk pe bhi likh de, memory me bhi aur disk pe bhi; gire to disk se wapas aa jaayega, jaise Kafka me line disk pe rehti. Phir bhi sab gira diya to haan, kuch second ki lines chhoot sakti hain, user dobara likh lega. Aisa sochenge to design hi nahi banega: poori consistency chahiye to 2 kyun, 500 copy rakho, aur 500 ka paisa bhi do. Interviewer dekhna chahta hai main sochta kaise hoon, har copy girana design nahi.
Technical me: "disk pe bhi likh de" = Redis AOF (persistence), "kuch second chhoot sakti" = chhota data loss window, trade-off: zyada durability = zyada copy / zyada paisa.

**Maan lo poore data center ki bijli gayi, Redis aur uski copy dono ek hi jagah the, dono gaye.**
Arpan: Ek jagah mat rakho. Main Google ka CEO hota to sochta: Mumbai ke data center me kabhi bijli ja sakti, to Arpan ka data Mumbai me, aur uski ek copy Delhi ke data center me. Mumbai gaya to copy Delhi se aa jaayegi, data safe. (Ye infra ka sawaal hai, aisa kam hi hota.)
Technical me: "copy doosre shehar me" = multi-region replica (alag data center me copy).

**6. Arpan doc ka maalik, usne Ravi ko hata diya. Par Ravi ke server ke paas permission ki purani copy, "Ravi = editor". Hatne ke baad bhi Ravi 2 minute likhta raha.**
Arpan: Main apni memory me chhota note daal dunga: ye banda editor nahi, iska koi bhi operation aaye, accept nahi karunga. Ravi alag server pe hai, to main ek parchi rakhunga "Ravi ab editor nahi", aur wo parchi ek tez bhaagne wale bachche ko dunga: jahan bhi Ravi ki permission hai wahan sab ja aur bata de "Ravi ab editor nahi". Ek centralized authority hai jo sab likh rahi. Usne mana kiya to chhota banda building ke bahar khada hoke chilla de "suno, Ravi editor nahi", to Ravi jis bhi server pe ho wahan se uska editor note hat chuka hoga. Bahut se answer ho sakte, ek sahi answer nahi: Redis pub/sub bolo ya building ke bahar koi chilla de.
Technical me: "centralized authority" = permission ka asli sach (SQL), "iska operation accept nahi" = server pe permission check, "tez bhaagne wala bachcha, building ke bahar chilla de" = pub/sub invalidation.

**Maan lo Ravi hatne ke waqt offline tha, laptop pe likhta raha, aadhe ghante baad net aaya aur saare edits ek saath server pe aaye.**
Arpan: Net aate hi Ravi ko popup dikha do "tumhe owner ne hata diya, ab ye doc edit nahi kar sakte". Server pe edit seedha nahi lagta, har edit pe pehle dekho ye kaun bhej raha aur kya abhi uske paas edit ka haq hai. Haq tha to merge, nahi tha to uske saare edits phenk do. Chahe to kahin alag rakh lo (baad me Ravi wapas mile to), par wo FR ki baat hai, interviewer se poochh ke tay karo; yahan phenk rahe.
Technical me: popup = client ko permission revoked ka event, "har edit pe haq dekho" = har op pe server-side permission check (aaj ka haq, purana nahi), phenk do = op reject; alag rakhna = FR pe (rejected ops ka log / draft copy).


**7. Ladka chillaya "Ravi editor nahi", par server 7 us pal restart / net se bahar tha, awaaz suni hi nahi. Pub/sub dobara nahi chillata. Server 7 ki memory me abhi bhi "Ravi = editor", aur Ravi wahin se likh raha.**
Arpan: Server akela nahi, uski copy saath khadi hai. Main thodi der bahar ja raha: "kuch bhi ho to note karke likh lena, wapas aake tujhse dekh lunga, tu meri copy hai." Ladka chillaya "Ravi editor nahi", copy ne likh liya. Main wapas aaya: "kya hua?" "Bas ek awaaz thi, Ravi editor nahi." Achha, to Ravi ki permission kaat do, khatam kahani.
Technical me: "server ki copy jo peeche note kare" = standby / replica server bhi pub/sub sunta, "wapas aake copy se dekh lunga" = reconnect pe chhoote hue badlaav copy (ya SQL) se le ke note theek, phir hi Ravi ka op maano.

&nbsp;

Poora design = [11_google_docs_collab.md](11_google_docs_collab.md)
