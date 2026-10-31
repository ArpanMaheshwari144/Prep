# Chat: hand notes (Arpan, Ravi aur college-gang group)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Scene:** Arpan apne dost Ravi ko message bhejta hai, aur dono "college-gang" group me hain jisme 500 log hain.
**Shabd:** taar = WebSocket connection · dabba = DB · map = Redis · chhota ladka = pub/sub · chaabi = clientMsgId · number = server ka sequence

Banking se aaya: parchi wali chaabi (idempotency). File upload se aaya: video S3 me, presigned URL. Naya: taar, map, ladka, number.

&nbsp;

**1. Ravi ka phone har second poochh raha "kuch aaya?"**
Server ko poochne ka mauka hi mat do. Kuch aaya to beech wala dabba turant Ravi ko bhej de, kaam khatam.
**WebSocket**
Interview: "Polling me 99% jawab khaali, server dab jaata. WebSocket = phone aur server ke beech khula taar, message aate hi server khud push karta."

**2. Ravi ka phone band, taar toota**
Server message sambhaal ke rakhe, jaise papa ke diye paise uncle na mile to jeb me rakhte ho, phenkte nahi. Ravi aaye to de do.
**pehle DB, phir bhejo (offline delivery)**
Interview: "Har message pehle DB me, phir bhejo. Ravi offline to push notification. Wapas aaye to phone bataye 'mera aakhri #N', server uske baad ke sab bhej de."

**3. 200 server, server 3 ko pata nahi Ravi kahan**
Ek map rakh lo, DSA wala: kaun kis server pe. Dhoondhna nahi, O(1) me mil gaya.
**Redis session map**
Interview: "Redis me userId -> server. Taar juda to likho, toota to hatao, umar (TTL) ke saath, taaki server gire to purani entry khud mit jaaye."

**4. Server 3 se server 71 tak message kaise jaaye**
Ek chhota ladka: use address do, wo le jaake wahan wale ko pakda de. Har server ka har server se taar nahi.
**Redis pub/sub**
Interview: "Har chat server apna channel sunta (server-71). Server 3 us channel pe daalta, server 71 Ravi ke taar pe bhej deta. 200 x 199 taar nahi."

**5. 500 logon ka group, ek message = 500 kaam**
Message ek dabbe me rakh do, 500 log wahin se sunenge.
**group message ek baar likho**
Interview: "Message DB me ek baar (chat_id = college-gang, #980). Members ki list alag. Online members ko unke server pe, offline wapas aake wahi padh lein."

**6. Lift me "hi" bheja, jawab khoya, dobara bheja, Ravi ko "hi hi"**
Har message pe ek chaabi. Wahi chaabi dobara aaye to mana, pehla ho chuka.
**clientMsgId (idempotency key)**
Interview: "Phone har message pe id lagata, retry pe wahi id. Server dusri copy chhod deta aur bas 'pahuncha' dobara bhejta. Banking ki parchi wali key."

**7. Group me "kahan?" pehle dikha, "kal party" baad me**
Numbering kar do. Phone nahi lagayega, phone ko kya pata kaun pehle aaya. Server ko pata hai, server number lagaye.
**server ka sequence number (har chat ka)**
Interview: "Har chat ka counter server pe: #980, #981. Sab phone isi kram se dikhaate. Phone ki ghadi pe bharosa nahi."

**8. Ravi ko "12 naye message" dikhana**
Ravi kahan tak padh chuka, uska ek number rakh lo. Aakhri number me se ghata do.
**last_read_seq**
Interview: "Har user har chat me ek number: padha_upto = 968. Unread = 980 - 968 = 12."

**9. #975 kisne padha (Info), bina 4.9 lakh entry**
Usi number se. Jiska number 975 ya usse aage, usne padha. Sita 978 = padha, Ravi 968 = nahi.
**read watermark**
Interview: "Har member ka ek number, 4.9 lakh ki jagah 500. Padha = number >= 975. Neeli tick tab jab sabka >= 975, double tick ke liye pahuncha_upto alag number."
(Group me double tick aata hai, neeli tab jab saare padh lein. Info me har member ka dikhta.)

**10. 200 MB ka video, chat ke taar jam**
Video ko alag raaste bhejo, alag dabbe me. Video wahin pada rahe, sabko chhota link bhejo, wahin se dekhein.
**S3 + link (CDN)**
Interview: "Phone video seedha S3 me daalta (presigned URL), chat me sirf link jaata. 500 log CDN se dekhte. File upload wala design."

**11. Ravi lift me band, 2 ghante se "online" dikh raha**
Register me likh lo "online". Har kuch second hilao "zinda hai?". 2-3 baar jawab nahi to naam kaat do.
**heartbeat + umar wali entry (TTL)**
Interview: "Redis me online:ravi 60 sec ki umar ke saath, har heartbeat pe umar wapas 60. Heartbeat ruka to entry khud mite, 'last seen' dikhe."

&nbsp;

Poora design = [14_chat_messaging.md](14_chat_messaging.md)
