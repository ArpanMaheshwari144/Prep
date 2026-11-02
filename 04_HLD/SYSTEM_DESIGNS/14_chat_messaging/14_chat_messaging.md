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
        · photo / video · online / last seen (E2E encryption, call: scope se bahar)
NFR     message turant (1 sec se kam) · kabhi na khoye · ek chat me kram na bigde · do baar na dikhe
Numbers 50 crore user, ek waqt ~10 crore online = 10 crore khule taar -> ~100 chat server
        roz ~400 crore message = ~46,000 likhai / sec, ek message ~300 byte -> roz ~1.2 TB
```

---

## 1. Ravi ka phone har second poochh raha (beech ka dabba, khula taar)

Arpan ne Ravi ko "kahan hai?" bheja. Ravi ka phone har second server se poochhta "mere liye kuch aaya?". 10 crore log
aise poochh rahe, 99% baar jawab "kuch nahi", server poochne walon se hi dab gaya. To server ko poochne ka mauka hi na
do. Kisi ne kuch bheja to seedha bhej do, bas kaam khatam. Beech me aisa dabba rakh do jo jaise hi kuch aaya, turant
doosre bande ko bhej de.

**Asli naam:** har second poochhna = polling (band), beech ka dabba = chat server, khula taar = WebSocket (ek baar
khula, band nahi hota, server khud push karta). Server pe har khula taar memory khaata hai, isliye event loop (Netty):
kuch hi thread, sirf jis taar pe message aaya use sambhalo.

```mermaid
flowchart LR
    A["Arpan"] --> S["Beech ka dabba<br/>(Chat server)"]
    S --> R["Ravi<br/>(WebSocket, khula taar)"]
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 2. Ravi ka phone band, 2 ghante baad on (dabbe me sanjo lo)

Arpan ne "kahan hai?" bheja, par Ravi ka phone band, 2 ghante baad on hoga. Taar toota pada hai, server ke haath me
message hai, bhejne ki jagah nahi. To server se bol do, ise apne paas aisi jagah rakh le jahan baad me dekh paaye.
Ek dabba bana le, usme rakh lo: "Arpan ne Ravi ko ye bheja hai." Ravi online hoga to use bhej denge. Jaise papa ne
paise de ke kahin bheja, aur uncle ji mile nahi, to main paise apne paas hi rakhunga, kharch ya phenk thodi dunga.

**Asli naam:** dabba = message store (har message pehle DB me save, phir bhejo; pehle bhejte aur save se pehle gira
to message gaya), online hoga to bhej denge = offline delivery. DB = Cassandra, `chat_id` se baanta: likhai bahut
(46,000 / sec, roz 1.2 TB), padhai hamesha ek chat ki ("aakhri 50 do"), na join na transaction. Ek chat ke saare
message ek machine pe, kram me.

```mermaid
flowchart LR
    A["Arpan"] --> S["Beech ka dabba<br/>(Chat server)"]
    S --> D[("Sanjo ke rakhne ka dabba<br/>(Cassandra, chat_id)")]
    S --> R["Ravi<br/>(WebSocket)"]
    style D fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 3. 100 server, Ravi kis pe? (DSA wala map + chhota ladka)

10 crore log, ek server sabke taar nahi pakad sakta, 100 server lage. Arpan ka taar server 3 pe, Ravi ka server 71
pe. Server 3 ko pata hi nahi Ravi kis server pe. To ek map rakh le, DSA wala map: ye is server pe hai, ye us server
pe. Dhoondhne ki zaroorat nahi, O(1) me mil gaya. Key-value pe rakh lo.

Ab pata chal gaya Ravi server 71 pe, par message wahan pahunche kaise? Har server baaki 99 se seedha taar jode to 100
x 99 taar, kal 500 server to aur zyada. To maan le saamne 10 ladke khade, maine ek chhote ladke ko kaha: "jo address
tere paas hai, use le ke chala ja, wahan koi aadmi hoga, use ye pakda de." Wo dekhega "haan bhai, ye wo ladka hai",
aur message pakda dega.

**Asli naam:** DSA wala map = session map Redis me (`ravi -> server 71`), O(1) lookup. Chhota ladka = Redis pub/sub:
har server apne naam ka channel sunta (`server-71`), server 3 us channel pe publish karta, server 71 le ke Ravi ke
taar pe bhejta. Taar toota to server 71 map se Ravi ki entry mita deta.

```mermaid
flowchart LR
    A["Arpan"] --> S3["Server 3<br/>(Chat server)"]
    S3 --> M[("DSA wala map<br/>(Redis: ravi -> 71)")]
    S3 --> L["Chhota ladka<br/>(Redis pub/sub)"]
    L --> S71["Server 71<br/>(Chat server)"]
    S71 --> R["Ravi"]
    style M fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style L fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 4. 500 logon ka group, ek message ke 500 kaam (ek dabbe me ek baar)

Arpan ne 500 logon ke group me "kal party hai" bheja. 500 log 60 server pe bikhre. Har member ke liye alag message
likhein to ek message ke peeche 500 kaam, aur aise messages har second. To use ek dabbe me rakh lo, 500 log khud
aayenge sunne. Kahin rakh do, 500 log jab chahe apna aake sunte rahenge.

**Asli naam:** ek dabba = group message DB me sirf ek baar (`chat_id = college-gang`), members wahi ek message padhte,
500 copy nahi. Group bhi bas ek chat hai jisme 2 ki jagah 500 log; members ki list se online walon ko map + pub/sub se
bhejo, baaki wapas aayein to dabbe se.

## 5. Lift me "hi" dobara gaya, Ravi ko "hi hi" (ek key)

Arpan lift me, Ravi ko "hi" bheja. Message server pe save ho gaya, par "pahuncha" wala jawab lift me kho gaya. Phone ko
laga gaya hi nahi, "hi" dobara bheja, Ravi ko "hi hi". To message ek baar issue karwao, doosri baar discard kar do. Ek
key rakh lo: "hi" aaya, iske paas ek key hai. Doosra aaya, same person ko ja raha aur key same hai, to mana kar do:
"ye message chahiye nahi, tera pehla ho gaya." Aur jab network aaye tab bol do "haan bhai, chala gaya", tick dikha do.

**Asli naam:** key = `clientMsgId` (phone Send dabate hi banata, jaise `m-501`, dobara bhejne pe wahi; ye banking
wali idempotency key hi hai). Key same = dupe copy chhodo, DB me dobara mat likho. Network aaye to tick = "pahuncha"
ka jawab (ack) dobara bhejo.

## 6. Group me "kahan?" pehle dikha, "kal party" baad me (server lagayega number)

Group me Arpan ne "kal party" bheja, usi second Ravi ne "kahan?". Kuch members ko "kahan?" pehle dikha, aur dono phone
ki ghadiyan bhi 2 second aage-peeche. To numbering kar do: jisne pehle bheja uska pehla aayega, saaf seedhi baat. Par
number phone nahi lagayega, phone lagayega to clash ho jaayega. Phone ko thodi pata kaunsa message pehle aaya, phone
to bas bhejne wala banda hai. Server ko pata hai kaunsa pehle aaya, server lagayega number.

**Asli naam:** numbering = sequence number, server ka, har chat ka apna (`#980`, `#981`; Redis `INCR seq:college-gang`,
ek saath aaye to bhi do alag number). Phone ki ghadi pe bharosa nahi. Wapas jude Ravi ko "mere paas aakhri #979, uske
baad sab do" bhi isi number se.

## 7. "12 naye message" aur kisne padha (ek ginti)

Ravi ko unread ginti dikhani hai, "12 naye message", group me 980 message, 500 member. To ek ginti rakh lo: Ravi ne
itne padh liye. 968 tak padh liye, aakhri 980 hai, to neeche 12 aur hain, wo 12 ke 12 turant uske phone me daal do.

Ab Arpan ne #975 "kal party" bheja, Info me dekhna kis-kis ne padha. Har message x har member ki line = 980 x 500 =
4.9 lakh entry. Par Sita ka number 978, iska matlab Sita ne 975 padha, tabhi uski ginti 978 hui. Ravi ka 968, matlab
wo bahut der se offline hai, ya usne ye message padha nahi. Number ghata lo, isse pata pad jaayega kisne padha, kisne
nahi.

**Asli naam:** ginti = `last_read_seq` (kahan tak padha, har member ka har chat me ek number), unread = aakhri number
minus `last_read_seq` (980 - 968 = 12). Kisne padha = read watermark (number >= 975 to padha): 4.9 lakh ki jagah 500
number. Neeli tick tab jab sabka number >= 975.

## 8. 200 MB ka video, chat ke taar jam (alag raasta, chhota URL)

Arpan ne group me party ka 200 MB ka video bheja. Ye chat server ke raaste DB me save hota aur 500 ko isi raaste
jaata. Taar chhote text ke liye bane the, video ne sab jam kar diya. Aaj 200 MB, kal koi 1 GB bhi bhej sakta. To ise
yahan se hatao, alag raaste pe bhej do. Ek alag machine rakh lo: "tu video hai, tu idhar ja", hamari chat block na
kare. Video wahin pada rahe, uska ek chhota URL bana ke sabme bhej do, wo wahin se dekh lenge.

**Asli naam:** alag machine = S3 (chat server ke bahar; app video seedha S3 me daalti, presigned URL se), chhota URL
= message me sirf link, wahin se dekh lenge = video S3 / CDN se. 500 log dekhein to bhi video ki copy ek hi.

```mermaid
flowchart LR
    A["Arpan app"] --> V[("Video ki alag machine<br/>(S3)")]
    A --> S["Beech ka dabba<br/>(Chat server, sirf link)"]
    S --> R["Ravi"]
    R --> V
    style V fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 9. Phone band par "online" dikh raha (register + hilao)

Ravi ke naam ke neeche "online". Ravi ka phone lift me band, taar tootne ka signal server tak pahuncha hi nahi, 2
ghante se Ravi "online" dikh raha. To ek register me likh liya "Ravi online hai". Ab har 3-5 second baad Ravi ko
hilaunga: "Ravi, tu online hai?" "Haan bhai, online hoon, pareshaan mat kar." Phir haath maara, jawab nahi. Phir
maara, phir nahi. 2-3 baar hilaya aur jawab nahi diya, to register se Ravi ka naam kaat diya, ab wo offline.

**Asli naam:** register = presence entry (Redis), har 3-5 second hilana = heartbeat, 2-3 baar jawab nahi to naam
kaato = entry ki umar (TTL), heartbeat chhoote to apne aap mite. Aakhri heartbeat ka time = "last seen".

---

Notes (Arpan ke shabd) = [CHAT_HAND_NOTES.md](CHAT_HAND_NOTES.md) · [<- MASTER SHEET](../../00_MASTER_SHEET.md)
