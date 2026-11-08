# Chat: hand notes (Arpan, Ravi aur college-gang group)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan apne dost Ravi ko message bhejta hai, aur dono "college-gang" group me hain jisme 500 log hain.

&nbsp;

**1. Arpan ne Ravi ko "kahan hai?" bheja. Ravi ka phone har second server se poochta "mere liye kuch aaya?". 10 crore log aise poochh rahe, 99% baar jawab "kuch nahi". Server poochne walon se hi dab gaya.**
Arpan: Server ko poochne ka mauka hi na do. Ravi ne kuch bheja to seedha bhej do, bas kaam khatam. Naam kya hai, technology kya hai, wo hogi koi. Bas beech me aisa dabba rakh do jo jaise hi kuch aaya, turant doosre bande ko bhej de. Kuch scene honge jahan pench phans sakta, jaise phone band hai, net nahi hai, wo aage dekhte.
Technical me: "poochne ka mauka hi na do" = polling band, "beech ka dabba jo aate hi turant bhej de" = WebSocket (khula taar, server khud push kare).

**2. Arpan ne "kahan hai?" bheja, par Ravi ka phone band, 2 ghante baad on hoga. Taar toota pada hai. Server ke haath me message hai, bhejne ki jagah nahi.**
Arpan: Server se bol do, ise thodi der apne paas rakh le, aisi jagah jahan baad me dekh paaye. Ek dabba bana le, usme rakh lo: "Arpan ne Ravi ko ye bheja hai." Ravi online hoga to use bhej denge, sanjo lo. Jaise papa ne mujhe paise de ke kahin bheja, aur uncle ji mile nahi, to main paise apne paas hi rakhunga, kharch ya phenk thodi dunga.
Technical me: "ek dabbe me rakh lo" = message store (pehle DB me save), "online hoga to bhej denge" = offline delivery.

**3. 10 crore log, ek server sabke taar nahi pakad sakta, 100 server lage. Arpan ka taar server 3 pe, Ravi ka server 71 pe. Server 3 ko pata hi nahi Ravi kis server pe.**
Arpan: Ek map rakh le apne paas, DSA wala map: ye is server pe hai, ye us server pe. Dhoondhne ki zaroorat nahi, O(1) time me mil gaya. Key-value pe rakh lo.
Technical me: "DSA wala map, key-value" = session map in Redis (userId -> server), "O(1) me mil gaya" = seedha lookup.

**4. Server 3 ko pata chal gaya Ravi server 71 pe. Message 71 tak kaise pahunche? Har server baaki 99 se seedha taar jode? 100 x 99 taar, kal 500 server to aur zyada.**
Arpan: Ravi online hai, hume pata hai wo kis server pe. Maan le saamne 10 ladke khade, maine ek chhote ladke ko kaha: "jo address tere paas hai, use le ke chala ja, wahan koi aadmi hoga, use ye pakda de." Wo dekhega "haan bhai, ye wo ladka hai", aur message pakda dega.
Technical me: "chhota ladka address le ke jaaye" = Redis pub/sub (har server apne channel ko sunta), "wahan wale ko pakda de" = server 71 apne channel se le ke Ravi ke taar pe bheje.

**5. Arpan ne 500 logon ke group me "kal party hai" bheja. 500 log 60 server pe bikhre. Har member ke liye alag message, to ek message ke peeche 500 kaam, aur aise messages har second.**
Arpan: Use ek dabbe me rakh lo, 500 log khud aayenge sunne. Kahin rakh do, 500 log jab chahe apna aake sunte rahenge.
Technical me: "ek dabbe me rakh lo" = group message DB me ek baar (chat_id = college-gang), "500 log khud aayenge sunne" = members wahi ek message padhein, 500 copy nahi.

**6. Arpan lift me, Ravi ko "hi" bheja. Message server pe save ho gaya, par "pahuncha" wala jawab lift me kho gaya. Phone ko laga gaya hi nahi, "hi" dobara bheja. Ravi ko "hi hi".**
Arpan: Message Ravi ke paas chala gaya, par hum lift me the to jawab hamare paas nahi aaya, humein laga nahi gaya. To jab network aaye tab bol do "haan bhai, chala gaya", double tick dikha do. Network nahi aaya to wo dobara bhej deta, do "hi" pahunch gaye. To message ek baar issue karwao, doosri baar discard kar do. Ek key rakh lo: "hi" aaya, iske paas ek key hai. Doosra aaya, same person ko ja raha aur key same hai, to mana kar do: "ye message chahiye nahi, tera pehla ho gaya."
Technical me: "ek key rakh lo, key same to mana" = clientMsgId (idempotency key), "doosri baar discard" = dupe copy chhodo, "network aaye to double tick" = "pahuncha" ka jawab dobara bhejo.

**7. Group me Arpan ne "kal party" bheja, usi second Ravi ne "kahan?". Kuch members ko "kahan?" pehle dikha. Dono phone ki ghadiyan bhi 2 second aage-peeche. (Claude ne poochha: number lagayega kaun?)**
Arpan: Numbering kar do: jisne pehle bheja uska pehla aayega, saaf seedhi baat. Phone nahi lagayega number, phone lagayega to clash ho jaayega. Phone ko thodi pata kaunsa message pehle aaya, phone to bas message bhejne wala banda hai. Server ko pata hai kaunsa pehle aaya, kis time aaya. Server lagayega number.
Technical me: "numbering kar do" = sequence number, "server lagayega number" = server ka sequence, har chat ka apna (#980, #981), "phone ko thodi pata" = phone ki ghadi pe bharosa nahi.
**Maan lo "kal party" ko #980 mila par DB me save hone me atka, Ravi ka "kahan?" #981 pehle pahunch gaya. Ravi ka phone yaad rakhe "aakhri #981", net gaya, wapas aaya, "#981 ke baad do" bola. #980 chhoot gaya?**
Arpan: Server chootiya thodi hai. Server har cheez bhej raha to record bhi rakh raha: ye bhej diya, ye bhej diya, teesra nahi gaya, chautha gaya, phir teesra bhi gaya. To Ravi ko message milega hi milega, kabhi nahi jaayega aisa nahi. Haan, group me kram thoda aage-peeche ho sakta hai. Server ko pata hai kaunsa pehle aaya, to sort karke daal do: thodi der ulta dikha, phir sahi. Kram kitna zaroori hai, ye FR me pehle hi tay kar lo; WhatsApp social app hai, ek message aage-peeche gaya to badi baat nahi.
Technical me: "server ka record, ye gaya ye nahi" = har user ka delivery record (store-and-forward, pending list), reconnect pe "jo nahi gaya wo sab do", phone ke aakhri number se nahi; "sort karke daal do" = sequence number se phone pe ordering; "FR me tay" = ordering kitni strict, requirement ka faisla.

**8. Ravi ko unread ginti dikhani hai: "12 naye message". Group me 980 message, 500 member.**
Arpan: Ek ginti rakh lo, Redis me ya map me ya DB me: Ravi ne itne message padh liye. 50 padh liye, ab neeche 12 aur hain, to 12 ke 12 turant uske phone me daal do.
Technical me: "ek ginti rakh lo, Ravi ne itne padh liye" = last_read_seq (kahan tak padha), "12 aur hain" = unread = aakhri number - last_read_seq (980 - 968 = 12).

**9. Arpan ne #975 "kal party" bheja, Info me dekhna kis-kis ne padha. Har message x har member ki line = 980 x 500 = 4.9 lakh entry. Ravi ka number 968, Sita ka 978. Kisne padha?**
Arpan: Group me double tick aata hai, Info me jaake dekhte hain kisne padha. Ravi ne nahi padha, Sita ne padh liya. Maine 975 bheja, Sita ka 978, iska matlab Sita ne 975 padha, tabhi uski ginti 978 hui. Ravi ne 968 padha, matlab wo bahut der se offline hai, ya pahuncha to usne ye message read nahi kiya. Number ghata lo, isse pata pad jaayega kisne padha, kisne nahi.
Technical me: "ginti 978, matlab 975 padha" = read watermark (number >= 975 to padha), "number ghata lo" = har member ka bas ek number, 4.9 lakh ki jagah 500.

**10. Arpan ne group me party ka 200 MB ka video bheja. Chat server ke raaste DB me save hoga aur 500 ko isi raaste. Taar chhote text ke liye bane the, video ne sab jam kar diya.**
Arpan: Video ke liye alag taar jod do. Aaj 200 MB, kal koi 500 MB ya 1 GB bhi bhej sakta. Ise yahan se hatao, alag raaste pe bhej do. Ek alag machine rakh lo: "tu video hai, tu idhar ja", hamari chat block na kare. Video ko wahin pada rehne do, uska ek chhota URL bana do aur sabme bhej do, wo wahin se dekh lenge. Ek insaan bitha do: "video aayi, mere paas aa ja, main chhota URL bana ke bhej deta hoon", chat server pe koi dikkat nahi.
Technical me: "alag machine, alag raasta" = S3 (chat server ke bahar), "chhota URL sabme bhejo" = message me sirf link, "wahin se dekh lenge" = video S3 / CDN se.

**11. Ravi ke naam ke neeche "online". Ravi ka phone lift me band, taar tootne ka signal server tak pahuncha hi nahi. 2 ghante se Ravi "online" dikh raha, phone band pada.**
Arpan: Ek register me likh liya "Ravi online hai". Ab har 3-5 second baad Ravi ko hilaunga, haath maarunga: "Ravi, tu online hai?" "Haan bhai, online hoon, pareshaan mat kar." Phir haath maara, jawab nahi diya. Phir maara, phir jawab nahi. 2-3 baar hilaya aur jawab nahi diya, to register se Ravi ka naam kaat diya, ab wo offline.
Technical me: "register me Ravi online" = presence entry (Redis), "har 3-5 second hilaunga" = heartbeat, "2-3 baar jawab nahi to naam kaat diya" = heartbeat chhoote to entry mite (TTL), "last seen" dikhe.

**12. Ravi ne Arpan ko "kal milte hain" phir "nahi, parso" bheja. Pehla server S1 se gaya, doosra S2 se, S1 dheema tha. Arpan ke phone pe pehle "nahi, parso" aaya, phir "kal milte hain". Arpan galat samjha.**
Arpan: Pehle to koshish karo ki ek chat ke dono message ek hi server se jaayein. Phir bhi alag server se gaye, to bhi hume pata hai server pe pehle kaun aaya tha: "kal milte hain" pehla, "nahi, parso" doosra. Pehle aaya wo pehle dikhe, aisa mat rakho. Jab dono phone pe pahunch jaayein, phone unhe sahi kram me laga ke dikhaye. Aur S1 bahut dheema ho to S2 wale ko rok ke nahi rakhenge, message to jaana hi hai. Thodi der ulta dikh sakta hai, phir apne aap theek ho jaata, WhatsApp me bhi aisa dekha hai. Perfect jawab nahi hai.
Technical me: "server pe pata kaun pehle aaya" = har chat ka sequence number (block 7), "phone sahi kram me lagaye" = client number se sort kare, aane ke kram se nahi, "dheeme ke liye doosra mat roko" = deliver turant, baad me reorder.
**Maan lo interviewer kheeche: number kaun lagayega, aur ye number kabhi khatam to nahi hoga?**
Arpan: Har ladke se poochho "tu kab aaya", aur us time ko hi number bana do. Time hamesha badhta hai, to number bhi badhta jaata hai, aur phone usi number se kram laga ke dikhata hai. Zyada gehra pooche to ek ID rakh lo. Par "ye number kabhi khatam nahi hoga" jaisa daava nahi karunga. Daava kiya to interviewer kahega "maan lo crore log dhadadhad message kar rahe, 50 saal me bhar gaya to?", aur mujhe apna point wapas lena padega. Interviewer bhi insaan hai, wahi poochega jo use pata hai; main bahas nahi karunga. Isliye seedha dhaancha bolunga: har message ko ek kram number, phone usi se lagata hai.
Technical me: "time ko hi number" = Snowflake ID (time + machine + chhota counter, FOUNDATIONS 13), "ek ID" = har chat ka sequence number. Kamzor jawab = number / had ka daava ("long ki had itni, kabhi nahi bharega"), kyunki interviewer use kheench sakta. Mazboot jawab = sirf dhaancha. Had yaad aa gayi to bonus line: long (64-bit) me lagbhag 92 lakh crore crore, aur counter har chat ka alag. Range jaisa kuch aur pooche to: "padhte waqt dekha tha isliye yaad hai, baaki net se dekh lete".

&nbsp;

Poora design = [14_chat_messaging.md](14_chat_messaging.md)
