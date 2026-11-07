# Notification: hand notes (Arpan ka order)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ek shopping app se order karta hai. Order hone pe app use teen jagah khabar bhejti hai: SMS ("order placed"), email (bill) aur phone pe push ("shipped"). Ye khabar bhejne ka system Arpan bana raha hai.

&nbsp;

**1. Order Service khud DB me order likhta, phir khud SMS, email, push company ko call karta. SMS company dheemi, har call 8 second, "Place Order" ka button 8 second ghoome. SMS company band ho to order hi fail dikhe, jabki DB me likha ja chuka.**
Arpan: Beech me ek dabba rakh le. Wo dabba yun kahe: aao bhai aao, sab mere paas aa jao, koi dikkat nahi. Main sabko store kar lunga aur dheere-dheere aaram-aaram se sabko bhejta rahunga.
Technical me: "beech me ek dabba" = queue (Kafka), "sabko store kar lunga, aaram se bhejta rahunga" = async bhejna, order turant ho jaata.
**Maan lo Order Service ne DB me order likh diya, par Kafka me khabar daalne se pehle server gir gaya. Order ho gaya, SMS kabhi nahi aaya.**
Arpan: Yahan outbox chalega, Kafka wala hi. Pehle kahin likh de ki haan, main ye khabar daalunga, order ke saath hi. Kabhi beech me mar bhi jaaye to wo cheez ek jagah mil jaaye, jaise baap marne se pehle vasiyat likh ke jaata hai. Baad me koi utha ke Kafka me daal dega.
Technical me: "order ke saath likh de" = outbox (order + khabar ek hi DB transaction me), "baad me koi utha ke daale" = relay (runner) outbox se Kafka ko bhejta.

**2. Dabbe me ek hi line, SMS, email, push sab ek ke peeche ek. Ek worker. SMS company dheemi, 8 second har SMS. Email aur push company theek, phir bhi bill aadhe ghante baad mila.**
Arpan: Line alag-alag kar de, sabki line alag. Jaise ladies ki alag line, gents ki alag, bachchon ki alag. Hum aamtaur pe aise hi karte hain, airport security, mandiron me sabki alag line hoti hai. Iska matlab consumer group: sab apni alag line sambhal lenge.
Technical me: "sabki line alag" = SMS, email, push ka alag topic, "apni line sambhal lenge" = har topic ka apna consumer group.

**3. SMS worker ne khabar uthayi, SMS Arpan ke phone pe pahunch gaya, par Kafka ko commit karne se theek pehle crash. Dobara chala, Kafka ne wahi khabar phir di, "order placed" do baar. OTP ya "₹5000 debited" pe hua to banda ghabra jaayega.**
Arpan: Parchi rakh le apne paas. Ye kaam ho gaya, to maine likh liya aur likh ke kahin rakh liya. Ab 10 din baad dobara aaya, dekha Arpan ke 500 rupaye (ya 5000, 10000) to chale gaye hain, kaam ho gaya hai. To apni parchi pe likh lo, parchi rakho. Kabhi bhi kuch ho jaaye, parchi hai. Parchi zindabaad.
Technical me: "kaam ho gaya, parchi pe likh liya" = idempotency (khabar ki id + "ho gaya" record), "dobara aaya, dekha ho gaya" = bhejne se pehle check, mila to skip (SMS gaya aur record likhne se pehle crash hua to ek baar dohra ab bhi ho sakta; parchi khidki chhoti karti, band nahi).

**4. Company ne OTP pe error lautaya: "abhi bheed hai, baad me try karo". Chhod de to OTP kabhi nahi milega, turant baar-baar bheje to company pe bojh aur peeche ki saari SMS line ruki. Kuch khabar kabhi ja hi nahi sakti, jaise galat number.**
Arpan: Alag cheez me bhej de use, thodi der baad try kar lenge, kahin rakh de. Jaise main papa ke paas 100 rupaye maangne ja raha aur papa kisi kaam me busy hain, to thodi der ruk jaate hain, baad me chale jaayenge. Important kaam hai to rakh lunga, thodi der baad try karenge, 3 ghante baad. Pattern dikh raha hai: koi bhi dikkat aayi to likh lo, kuch bhi aaya to kahin daal do.
Technical me: "alag cheez me, thodi der baad try" = retry queue (ruk ke dobara), "dikkat aayi to likh lo, kahin daal do" = queue / parchi / retry ka ek hi pattern. Galat number ka hal block 5 me.

**5. SMS company poori band. Har call 30 second latke phir fail, saare worker atke, retry line phoolti ja rahi, Arpan ka OTP nahi aaya. (Sawaal: kab tay karega ki main company ko call band karo, seedha backup, aur kaise pata chalega ki wo wapas theek ho gayi?)**
Arpan: Ek company nahi, teen-chaar company extra rakh lo apne paas, ek band to doosri mil gayi. Aur OTP important hote hain: bank OTP, transfer OTP, Uber ka OTP. Unki priority set kar do. Mandiron me VIP hote hain, VIP ko jaldi darshan karwane hain, Ambani sahab India ke VIP hain.
Band hai kaise samjhun: SMS bheja, fail. Jo fail hua uska note down karo, DB me, apni parchi me. Doosra bheja fail, teesra fail. Ab jo fail hua use backup se bhejta hoon. Backup ne bhej diya, 2-3, 5-6 baar try kara, matlab main company band hai. Pehle jinke nahi gaye unhe bhejo, ab traffic backup se guzaaro. Beech-beech me main company ko try karo: "tu theek hai? chal raha?" Nahi chal raha to backup chalta rahega, thodi der baad phir try, chal gaya to theek. Aur backup bhi same bole to phone number me dikkat hai: dead letter queue me daal do, investigate karo, number galat hai to hatao.
Technical me: "teen-chaar company" = failover (backup provider), "OTP VIP darshan" = OTP ki alag priority line (topic), "2-3 baar fail to traffic backup se" = circuit breaker OPEN, "beech-beech me main ko 'tu theek hai?'" = HALF-OPEN, "chal gaya" = wapas CLOSED, "backup bhi fail to number galat" = DLQ.
**Maan lo SMS company ne 30 second tak haan / na kuch nahi bola (timeout), humne backup se bhej diya, par pehli ne bhi bhej diya tha. Arpan ke paas 2 SMS.**
Arpan: Company se pehle hi baat karke rakhte: 30 second me haan ya na chahiye, aur tera uptime kitna, 99% hai to doosri se baat karo, ek backup bhi rakho. Phir bhi timeout aaya to faisla lena padega, chup nahi baith sakte. OTP hai (bank transfer) to OTP chahiye hi, turant doosri company se bhejo; dono ne bhej diya to 2 OTP chale jaayenge, kabhi-kabhi hamare paas bhi aate, koi dikkat nahi, bas dono me ek hi OTP ho. OTP nahi hai, festive sale ka message hai, to thoda ruk jao, 15-30 second late gaya to kiska kya chhoota.
Technical me: "30 second me haan / na, uptime kitna" = SLA + timeout threshold, "timeout = pata nahi gaya ya nahi" (error nahi), "OTP turant doosri company" = failover, "2 OTP chalega, ek hi code" = idempotent OTP (same code dono SMS me), "sale wala ruk sakta" = priority ke hisaab se retry.
Aur agar company (business) ko pakka ek hi OTP chahiye? Onboarding pe SMS company se terms me hi tay kar lo, jaise AWS ka SLA hota (uptime kam hua to paisa credit): "30 second me jawab chahiye; 30 second me nahi bheja to tum apni taraf fail maan lo, mat bhejna, aur hume fail wapas bata do, taaki main turant doosri company pe bhej sakun." Yahan cheez hamare haath me kam hai, baat SMS company ke server pe ho rahi, to unse pehle hi sab maalum kar lo.
Technical me: "30 second me nahi bheja to mat bhejna" = message validity period (TTL / expiry), "terms me tay, paisa credit" = SLA, "fail wapas bata do" = failure callback.
Par perfect system nahi banta. Company ki line me pada SMS khud fail ho jaayega, par agar wo aage Jio / Airtel tak chala gaya, to wahan hum kuch nahi kar sakte, wo hamara area nahi, out of scope. Ya to Google jaisa sab apne paas rakho, ya third party rakho to kahin na kahin kuch tootega, isliye "2 OTP chalega, ek hi code" bhi saath rakho.
Technical me: Twilio me ye setting `validity_period` (1 se 36,000 sec): itni der line me pada raha to bhejta hi nahi; carrier tak gaya to wapas nahi.

**6. Company ki had: ek second me 100 SMS. 20 worker apni raftaar se, sab milake 300, company 429 de rahi aur block ki dhamki. Har worker ko sirf apni ginti pata. (Sawaal: rate limiter me 10 server ki alag ginti thi, tab ginti kahan rakhi thi?)**
Arpan: Main SMS company ko nahi bol sakta ki had badha do, unhe baaki bhi dekhna hai, aur apni SMS company banana impossible hai. Ruk-ruk ke bhejo, peeche lambi kataar lag rahi to koi dikkat nahi, burst ek saath nahi jaa raha.
Saanjha kar lo, ek jagah se sabko guzaaro. Mere paas 10 log hain, apna-apna kaam kar rahe: tumne kitna kara? +2. Tumne? +2. Tumne +3, +4. Ek saanjha rakh lo jise sab dekhenge. Main beech me baitha hoon: tune ek bhej diya, tu bhi bhej raha? Nahi, tu nahi bhejega. Ek aisa dabba hona chahiye jisme ye baat likhi ho.
Technical me: "company ko had badhane nahi bol sakte" = had apni taraf se, "ruk-ruk ke, lambi kataar" = bachi khabar line me wait, "saanjha dabba, sab usi se guzrein, ginti likhi" = shared token bucket (Redis).

**7. Company ne turant "OK, le liya" bola, record me SENT likha. Par Arpan ka phone network se bahar, SMS pahuncha hi nahi. Support record dekh ke bolta "humne to bhej diya tha".**
Arpan: Company bolegi "tera SMS le liya, abhi bheja nahi". Phir company dheere-dheere aage badhegi aur ek time ke baad humein batayegi: ya to bhej diya ya nahi bheja. Do hi raaste, teesra nahi, aur ye "le liya" ke baad hai: sent ya fail. Sent kara to apni parchi me likh lo "sent", fail hua to apni parchi me "fail" likh do.
Technical me: "le liya, abhi bheja nahi" = ACCEPTED, "ek time ke baad company batayegi" = webhook, "sent ya fail" = DELIVERED / FAILED, "apni parchi me likh lo" = Tracking DB.

**8. Diwali: ek khabar "1 crore users ko offer bhejo". Ek worker ne list nikaali, loop me bhejne laga, 40 lakh pe crash. Dobara chala to loop shuru se, 40 lakh ko do baar, kaam ek worker pe atka, baaki khaali.**
Arpan: Thoda todte hain. 1 crore ek worker nahi sambhalega. 20, 30, 50 worker me baant do, alag-alag range de do: tu yahan se yahan bhejega. Jo bheje gaye unhe turant DB me mark kar do. 1 crore Redis me nahi rakhunga, Redis ko befaltu nahi bharna, apni ek moti diary hai usme karte jao. Bai chance worker fail hone ke baad dobara bhej de, to idempotency hamare paas hai hi: isko bhej diya tha? Haan, to dobara nahi jaayega.
Jaise mere paas 10 log hain, maine kaam diya: tu iske ghar jaa, tu iske ghar. Parchi pe likhte raho ye ghar ho gaya. Kal koi bimaar pad gaya: apni parchi dikhana, achha 10 me se 7 ko bhej diya tha. Backup tu aa ja, baaki 3 ko tu bhej de.
Technical me: "tod do, alag range" = campaign tukde (chunks), "turant DB me mark, moti diary" = checkpoint (DB), "10 me se 7, baaki 3 backup bheje" = crash pe wahin se, "bhej diya tha? dobara nahi" = idempotency.

**9. Arpan ne settings me chuna "offer wale SMS mat bhejo". Har SMS se pehle worker DB se user ki pasand aur message ka saancha ("Hi {naam}, Diwali pe 50% off") padhta. 1 crore SMS = 1 crore query, DB ki saans phooli, OTP line bhi isi DB se, OTP dheema. (Sawaal: pasand kitni baar badalti, aur aisi cheez ke saath cache me kya kiya tha?)**
Arpan: Wo baar-baar wahi cheez padh raha hai, use apne paas rakh lo. Diwali pe sabko ek hi offer jaata hai, "Diwali 50% off", sabko wahi message. Mote register pe baar-baar kyun jaun? Main apne paas rakh lunga: le, ye ek hi tera hai, isme bas naam badalna hai, bhejta ja.
User ki pasand uski settings hai, wo DB me hai, DB se hi uthayenge. Hamari company bhi aise hi karti hai, email template banate waqt code DB se khud padh leta hai. Crore users hain, sabki setting cache me kaise rakhenge. OTP itna late nahi hoga, kyunki OTP ki priority pehle se set hai, worker bhi hain, rate limit bhi hai. Baad me kabhi lage ki faltu queries bahut badh gayi, tab sochunga cache me rakhun, aur tab har user ke liye.
Technical me: "ek hi saancha apne paas, bas naam badlo" = template cache, "settings DB se hi" = user preference DB se, "OTP ki priority pehle se" = OTP ki alag line, "queries badhi tab cache" = naap ke hi preference cache.

**Maan lo Diwali pe 1 crore offer, har ek se pehle DB me pasand, DB ki saans phooli. OTP worker bhi pasand padhne usi DB pe gaya, OTP 40 second late.**
Arpan: OTP ka worker DB me pasand padhne jaayega kyun? Diwali ka offer thodi bhej rahe, OTP bhej rahe. Seedha OTP bhejo, DB se lena-dena hi nahi. Offer bhejte to pasand padhna banta.
Technical me: "OTP seedha bhejo" = OTP pe preference check skip (OTP band nahi kiya ja sakta), to OTP path pe DB hi nahi; pasand sirf offer / marketing pe.

&nbsp;

Poora design = [04_notification_system.md](04_notification_system.md)
