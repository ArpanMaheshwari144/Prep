# Chat (WhatsApp jaisa): hand notes (mock se pehle ye padho)

### Shuru

**Kaam:** 1-to-1 chat aur 500 tak ke group, history server pe, tick (bheja / pahuncha / padha), Ravi offline ho to baad me mile, photo / video, online / last seen. Bahar: E2E encryption, call, lakhon wale broadcast. E2E zaroor poochho, kyunki haan bola to server message padh hi nahi sakta: poora design badal jaata hai.

**Sabse upar:** turant pahunche (1 sec se kam), kabhi na khoye, chat ke andar kram na bigde, do baar na dikhe.

**Numbers:** 50 crore user, roz ~400 crore message = ~46,000 likhai har second, roz ~1.2 TB. Par asli paimana ye nahi: asli hai ek saath ~2 crore khuli connection, kyunki har khuli connection memory khaati hai.

**Maan lo E2E chahiye:** server pe search khatam, naya member purane message nahi dekh sakta, chaabiyon ka poora system.

&nbsp;

**1. WEBSOCKET**

Ek chat server + DB. Arpan ne "hi" bheja, DB me gaya. Par Ravi ne to kuch maanga hi nahi, use kaise mile? HTTP me server sirf jawab deta hai, khud nahi bolta. Polling = har 2 sec "kuch aaya?", bekaar bheed aur late. Long polling = behtar, par har message pe nayi request. Isliye **WEBSOCKET**: Ravi ek baar connection khole (`GET /connect`), wo khuli rahe, server jab chahe bhej de.

Par ye connection khuli rehti hai, aur subah 2 crore log ek saath app khol ke baith gaye.

&nbsp;

**2. 200 SERVER + EVENT LOOP + BACKOFF**

Ek server ki memory 2 crore khuli connection nahi jhel paayi. Ek machine ~1 lakh connection, to ~**200 CHAT SERVER** + LB. Aur har connection ko ek thread nahi, kyunki zyadatar khaali baithe rehte hain. **EVENT LOOP** (Netty / Node): kuch hi thread, jis connection pe message aaya sirf use sambhalo.

**Maan lo chat server 9 gir gaya:** chat server khud kuch yaad nahi rakhta (sab DB aur Redis me hai), isliye iski copy (replica) nahi hoti. Server 9 gira to uske 1 lakh log ek hi second me dobara judne aaye, aur us dhakke se baaki server bhi hil gaye. Isliye app ruk-ruk ke jude, har baar thoda zyada ruke (1, 2, 4 sec = **BACKOFF**) + thoda random (**JITTER**), taaki 1 lakh log kai second me phail ke aayein aur LB unhe baaki ~199 server pe baant de.

**Maan lo deploy karna hai:** isi wajah se server dheere-dheere khaali karo, sab ek saath restart nahi.

Par ab 200 server hain. Arpan server 5 pe, Ravi server 9 pe.

&nbsp;

**3. REDIS MAP (KAUN KAHAN)**

Server 5 ko pata hi nahi Ravi kahan hai, aur 200 server se poochhna dheema. Isliye **REDIS MAP**: ravi -> server 9 (hotel ka main register). Server gira to entry galat reh jaati, isliye **UMAR (TTL 60 sec)** + app ka har 30 sec **HEARTBEAT**. Kai device = list: ravi -> [server 9 phone, server 14 laptop].

**AWS pe:** Redis hum khud nahi chalate, AWS ka **ElastiCache** lete hain (Redis jise AWS chalata hai). Machine lagana, uski copy rakhna, gira to copy ko aage karna, ye sab AWS ka kaam hai. Hum bas use karte hain.

Par Ravi ka phone band hai, Redis me uski entry hi nahi. "hi" kahan jaaye?

&nbsp;

**4. PEHLE DB, PHIR BHEJO + ACK + PUSH**

Isliye **PEHLE DB ME LIKHO, PHIR BHEJO**: pehle bhejte aur DB se pehle girte to message hamesha ke liye gaya. DB me likh ke **ACK** = ek grey tick. Ack se pehle gira to app khud dobara bhejti hai. Offline ko **PUSH** (FCM / APNs).

**Maan lo push fail hua:** Google / Apple bahar ki service hai, to apna retry + queue.

Wapas juda to app bole "mere paas 41 tak hai" (`GET /messages?chatId=&after=41`), server 42 se aage bheje.

**Maan lo Ravi ka server gira:** message DB me pakka hai, Ravi naye server se judta hai aur "41 ke baad sab do".

Par ab har message DB me jaata hai: roz 1.2 TB, 46,000 likhai har second.

&nbsp;

**5. CASSANDRA (CHAT_ID)**

Ek DB nahi jhel paaya. Message ek baar likha, kabhi badalta nahi, padhai hamesha "is chat ke aakhri 50" ya "41 ke baad". Join / transaction chahiye hi nahi. Isliye **CASSANDRA**, **CHAT_ID** se baanto, taaki ek chat ke saare message ek machine pe number ke kram me rahein. User id se baant-te to ek chat do machine me batt jaati. Paisa / join / search = Cassandra nahi.

**AWS pe:** Cassandra AWS ka **Keyspaces** (machine, copy AWS sambhalta), ya EC2 pe khud chalao (tab sab hamara sar-dard).

Par server ne DB me likha, ack se pehle gira, app ne dobara bheja.

&nbsp;

**6. CLIENTMSGID (DO BAAR NAHI)**

Ravi ko "hi" do baar dikha. Isliye **CLIENTMSGID**: phone Send pe id banata hai (m-501), dobara pe bhi wahi (`POST /messages { chatId, text, clientMsgId }`). Kram: pehle Redis me "m-501 = #42", phir DB me #42. Dobara aaya to wahi #42 dobara likho, aur Cassandra same key pe upar likh deta hai, naya row nahi. Id phone kyun banaye? Dobara phone hi bhejta hai. Ye banking ki idempotency key hai.

Par Arpan ki ghadi 5 min aage thi.

&nbsp;

**7. SERVER KA NUMBER (KRAM)**

Arpan ka sawaal Ravi ke jawab ke neeche chala gaya. Phone ki ghadi pe bharosa nahi. Isliye **SERVER KA NUMBER**: pahunchte hi `INCR seq:arpan-ravi` (44, 45). Jo server pe pehle aaya use agla number. Redis ek baar me ek kaam karta hai, to do ko same number kabhi nahi. Dikhne wala time server ka (NTP), par kram number se.

Par College Gang me 500 log hain.

&nbsp;

**8. GROUP: EK BAAR LIKHO**

Har member ki copy = 500 likhai. Isliye message **EK BAAR LIKHO** (college-gang, #980). Members ki list group_members table me + Redis me. Online ko unke server pe, offline ko push. Lakhon wala broadcast scope ke bahar.

Par neeli tick aur "50 unread" kaise dikhe?

&nbsp;

**9. "KAHAN TAK" NUMBER (TICK + UNREAD)**

Har message pe padha / nahi padha = 980 x 500 nishaan, bahut bhaari. Isliye har user ka har chat me **EK NUMBER**: pahuncha_upto = 45 (do grey tick), padha_upto = 45 (neeli), app `POST /chats/{chatId}/read { padha_upto }` bhejti hai. Unread = aakhri 980 - padha 930 = 50, bas ek ghatana.

Par Arpan ne 200 MB ka video bheja.

&nbsp;

**10. VIDEO S3**

Video chat server aur Cassandra se gaya, baaki sab atak gaye. Isliye app video seedha **S3** me (presigned URL, 10 min ki chaabi), message me sirf link. Ravi tap kare to video **CDN** se aata hai, S3 ya server se nahi.

**AWS pe:** video **S3** me (AWS rakhta, copy bhi), aur log tak **CloudFront** (AWS ka CDN) se paas wali jagah se.

Par "Arpan online" kaise dikhe? 2 crore log ka online / offline badalta rehta hai.

&nbsp;

**11. ONLINE = UMAR WALI ENTRY**

Wahi heartbeat: `online:arpan` 60 sec ki umar ke saath, last_seen har heartbeat pe. 60 sec heartbeat nahi aaya to entry mit gayi = offline. Ravi chat khole tab hi poochho. Har badlaav 500 contacts ko bhejna = bheed.

&nbsp;

### Wrap

turant = WebSocket + Redis map · kabhi na khoye = pehle DB, ack, app ka dobara bhejna, push + "aakhri ke baad sab" · server gira = backoff + jitter, LB baaki pe baante · kram = server ka number · do baar nahi = clientMsgId · group = ek baar likho · tick = kahan tak number · video = S3

**Chal raha kaise pata:** message pahunchne ka p99, har server pe khuli connection, dobara judne ki dar, push fail, alert.


Poora design (har kadam ka diagram aur "kyun") = [14_chat_messaging.md](14_chat_messaging.md)
