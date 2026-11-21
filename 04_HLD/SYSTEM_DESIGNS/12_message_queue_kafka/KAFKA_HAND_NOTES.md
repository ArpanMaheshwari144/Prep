# Kafka: hand notes (stadium)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Match ka din, stadium ke bahar bahut bheed. Gate pe admin / chowkidar line sambhalta hai (Kafka), andar checker ka kamra aur counter (SMS, Fraud), bahar ticket counter (Order Service). Scene Arpan ne khud chuna.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Order Service  -->  DB + vasiyat (outbox)  -->  Kafka (stadium: gate = partition, line = topic)
                                                   |  admin "likh di" tabhi jab copy bhi likhi (acks=all)
                                                   |  order id se gate (partition key) -> ek order ka kram pakka
                         +-------------------------+-------------------------+
                    SMS room (consumer group)  Fraud room            Analytics room
                    parchi / bookmark (offset) · pakki id se dohra phenko · kharab ticket DLQ · counter band = circuit breaker

 1   stadium ke bahar bheed, sab ghus rahe       ->  admin, line banao (Kafka)
 2   line me ek aadmi nikla, sab atke            ->  seat booked, wahi baithega (offset)
 3   ek admin se nahi sambhal rahi               ->  aur gate, har gate ki apni line (partition)
 3b  #A7 ka paid aur shipped alag gate pe        ->  order id se ek gate (partition key)
 4   har ticket 3 baar punch                     ->  ek kamre me bitha do (consumer group)
 4b  security wale bhi usi kamre me              ->  alag room (alag consumer group)
 4c  scanner hang, talaashi bhi ruki             ->  nayi line ticket_ok; zyada consumer
 5   admin ne "aa gaya" bola, phir gira          ->  copy bhi likhe, tab "likh di" (acks=all)
 6   punch ke baad, register se pehle gira       ->  pehle aaya hai? check (idempotency)
 6b  SMS gaya, bookmark se pehle gira            ->  pakki id, dobara aaye to phenko (idempotent consumer)
 7   Order Service gate tak bhejne se pehle gira ->  pehle vasiyat likh de (outbox)
 8   galat ticket baar-baar, line atki           ->  line se hata (DLQ); gate pe hi rok (validation)
 8b  pay DLQ me, cancel chal gayi, kram toota    ->  A2 ruka to A3 bhi ruka (same key parking)
 9   SMS counter 10 min band, sab DLQ me         ->  pehle check, waqai band? (circuit breaker, fallback)
10   7 din peeche, purana panna phat jaata       ->  aur consumer daalo, phir partition
10b  aaj 1 din, kal 2 din peeche                 ->  alert (consumer lag alert)
```

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      Order Service ki khabar SMS, Fraud, Analytics tak pahunche · Order Service kisi ka intezaar na kare
        · ek user ki khabar kram me (order, pay, cancel)
NFR     khabar kabhi na khoye · do baar aaye to bhi kaam ek baar
        · ek service dheemi ho to baaki na rukein · ek kharab khabar line na roke
Numbers roz 10 crore khabar = ~1,200 / sec, sale me ~5,000 / sec · ek checker ~500 / sec -> 10 checker, gate 30 (aage checker badhane ki jagah)
        ek khabar ~1 KB -> roz ~100 GB, 7 din x 3 copy = ~2 TB disk
```

**FR / NFR -> kadam (Arpan: "sale ki bheed hatao, yahan matlab nahi; baaki sab theek"):** khabar SMS, Fraud, Analytics tak =
1, 4, 4b · Order Service intezaar na kare = 1, 7 · ek user ki khabar kram me = 3b, 8b · NFR khabar kabhi na khoye = 5, 7 ·
do baar aaye to kaam ek baar = 6, 6b · ek service dheemi to baaki na rukein = 2, 4b, 4c, 9, 10 · ek kharab khabar line na roke = 8, 8b.

&nbsp;

**1. Stadium ke bahar bahut bheed, sab ek saath andar ghusna chahte hain.**
Arpan: Kafka mere liye ek aisa banda hai jo bheed ko kaabu karta hai. Stadium ke bahar bahut bheed, sab ghus rahe. Ek admin ne bola: line banao, ek-ek karke jao, seat mili to baith jao. Bas ye hai Kafka mere liye: bheed ko rokne wala. Spike aaya, usne rok diya.
Technical me: "bheed ko kaabu karne wala" = Kafka (beech ki queue), "line banao, ek-ek karke jao" = khabar line me, padhne wale apni raftaar se (async), "spike aaya, rok diya" = spike / load sambhalna.
**Isi ki doosri kahani (tube):** Arpan: Ise tunnel samajh, ek tube samajh: tube me main idhar se kuch daal raha, wo producer. Wo jagah jahan line lag rahi, wo Kafka. Jo main udhar se nikaal lun, wo consumer, worker / consumer jo bhi kehna chaho.
**Maan lo pooche app ke andar hi async thread kyun nahi:** Arpan: Kaam disk pe rehna chahiye. Thread hai aur kuch gir gaya to thread sab kho dega, wo hum afford nahi kar sakte; email kisko gaya, kya gaya, kuch pata nahi lagega. Kafka apni cheez disk pe rakhta, consumer kahan tak hai, offset hai.
**Maan lo pooche RabbitMQ jaisi aam queue kyun nahi:** Arpan: Server ki memory me list rakhi to restart pe saari khabar gaayab. RabbitMQ jaisi aam queue me ek ne khabar padhi aur wo line se hata degi, jaise pehle cinema me ticket le ke line me lagte, gate wala ticket phaad deta taaki us ticket pe koi doosra na ghuse. Hamein aisa chahiye jisme khabar hate na, isliye Kafka: khabar disk pe, mitegi nahi; har service apna bookmark rakhti.
**Maan lo pooche Kafka kab nahi chahiye:** Arpan: Jab jawab tumhe turant chahiye, jaise REST API.
**AWS pe:** Arpan: Kafka AWS me hai to broker, copy sab AWS manage karega, hum nahi.
Technical me: producer -> topic -> consumer (tube), in-process async thread = crash pe kaam gaya (durable nahi), RabbitMQ = padha to hata (ticket phaad), Kafka = log disk pe + offset, turant jawab chahiye = sync REST, managed Kafka (MSK) me broker / replica AWS ka kaam.

**2. Line me ek aadmi ruk gaya, peeche sab atak gaye. Wo line se nikal ke bahar chala gaya. (Ye dikkat Arpan ne khud uthayi.)**
Arpan: Maan le stadium me line hai, ek aadmi ruk gaya, wahin se peeche sab disturb ho gaye. Wo line se nikla, bahar chala gaya. Uski seat booked hai, wahi baithega, uski seat pe koi aur nahi baith sakta. To line dobara shuru kari, koi gum nahi hua, kuch dikkat nahi.
Technical me: "seat booked, wahi baithega" = offset (bookmark), "line dobara shuru, koi gum nahi" = restart ke baad wahin se padhna, khabar padhne se mit-ti nahi.

**3. Bheed itni ki ek admin se nahi sambhal rahi.**
Arpan: Bheed bahut hai, wo akela admin nahi sambhal pa raha, to inke liye aur gate khade karo. Har ek ke andar apni line. Yahi Kafka karta hai: broker, partition, aur unke andar line. Kaun kis gate pe jaaye, wahan hash aa gaya.
Technical me: "aur gate" = partition, "har gate ke andar apni line" = kram sirf ek partition ke andar, "kaun kis gate pe, hash" = key ka hash se partition.

**Maan lo bahar ka ticket counter (Order Service) har order ki do khabar bhejta hai, pehle "paid" phir "shipped". Order #A7 ke dono wo koi bhi khaali partition me daal deta: paid partition 2 me, shipped partition 5 me. Partition 5 ka consumer tez, shipped pehle chal gaya.**
Arpan: Alag-alag nahi bhej sakte. Key ke zariye ek hi partition me laayenge: A7 ki saari cheez ek partition me, A8 ki ek me. Main Kafka hoon: paid is line me ghusa, shipped us line me, shipped aage nikal gaya, paid peeche reh gaya, dikkat. Isliye ek key, ek tukda: A7, phir A8, phir A9. Tukde ke andar Kafka line follow karta hi hai: pehle paid phir shipped.
Technical me: "key ke zariye ek partition" = partition key (message key = order id), "tukde ke andar line" = kram sirf ek partition ke andar.
**Isi ki doosri kahani (exam hall):** Arpan: Class me bachche kaise bhi baithe hon, par exam hall me aaye to fix seat: tera roll number yahan, tera wahan. Cinema ke bahar bheed kaise bhi, andar ek ke baad ek. Kafka ka matlab hi line lagana; partition ke andar kram wo follow karega hi.
**Maan lo ek user ki khabar bahut garam:** Arpan: Key me number jod deta hoon, jaise user 9 ya 4, to wo alag tukde pe chala jaata. Keemat ye ki us user ka poora kram nahi bachta, bas har tukde ke andar bachta.
Technical me: "fix seat" = partition ke andar order guaranteed, "key me number jodo" = key salting (hot key ko kai partition me baanto), keemat = us key ka global order toota.

**4. Gate ke andar 3 ticket checker, teeno har fan ka ticket punch kar rahe. Har ticket 3 baar punch, line teen guna dheemi.**
Arpan: 3 punch ho gaya hai. To aise kar de, un saalon ko mila de, ek bana de, ek kamre me hi baitha de. Is kamre me 3 baithe hain, koi bhi aaye, bas in 3 me se ek pe bhej. Jisne punch kar diya, baaki 2 usko dekhe hi nahi.
Technical me: "ek kamre me baitha de, ek bana de" = consumer group, "3 me se ek pe bhej" = ek partition group me ek hi consumer ko, "baaki 2 dekhe hi nahi" = group me ek khabar ek hi checker ke paas (par commit se pehle gira to dobara aa sakti, [kadam 6](12_message_queue_kafka.md#6-checker-register-me-likhne-se-pehle-gira-pehle-aaya-dekh-lo)).

**Maan lo har fan ki talaashi bhi honi hai, 2 security wale aaye. Unhe bhi checker ke kamre me bitha diya to "sirf ek ke paas" wala niyam unpe bhi lagega, talaashi chhoot jaayegi.**
Arpan: To alag baitha do. Matlab pehle ticket checker ka room, phir security ka room. Ek ko paar karo, phir doosre room me jao.
Technical me: "alag room" = alag consumer group. "pehle ticket checker ka room, phir security ka room" = checker jo paas kare use nayi line me daale (naya topic: ticket_ok), security group usi nayi line se padhe, kyunki talaashi ticket ke baad hi. Agar talaashi ka ticket se lena-dena na hota, to dono group seedha ek hi line se, apni-apni copy aur apni raftaar se padhte, ek dheema to doosra nahi rukta.

**Maan lo checker ke room me scanner hang, 10 minute sab atke.**
Arpan: Ticket check zaroori hai, bina ticket talaashi kyun lun, wahin se nikaal dunga. Order maintain karna hai, jaise pehle order phir pay. Scanner atka to ek mat rakho, copy bana ke rakho: 2-3 scanner, 2-3 room. Ek kharab to doosre se traffic chalta rahega.
Technical me: "2-3 scanner / room" = group me zyada consumer (ek gira to uske partition baaki me bat jaate, rebalance); "pehle ticket phir talaashi" = do topic ki chain (ticket_ok).

**5. Admin ne ticket counter ko "aa gaya" bol diya, phir wo gira, aur uski jagah lene wale ke paas aakhri kuch logon ki list hi nahi thi. Arpan ki vasiyat me: baap ne vasiyat ek hi copy me likhi, ghar walon ko bol diya "likh di", phir wo copy jal gayi. "Likh di" kab bolna tha?**
Arpan: Bhai "likh di" aise bolna ki uski copy aur bana, aur wo likh de, tab "likh di". Matlab replica hai, replica ne bol diya "likh liya", sab bolein, tab. Ye raha connection.
Technical me: "copy bhi likh de, tab likh di" = acks=all, "replica ne bol diya likh liya" = in-sync replica ki haan (min.insync.replicas = 2).

**6. Checker ne fan ko seat pe baitha diya (ticket punch), par register me likhne se pehle gir gaya. Naya checker wahi fan dobara bhejega.**
Arpan: Idempotency check kar le ki wo pehle aaya hai. Bas, samajh ja.
Technical me: "check kar le ki pehle aaya hai" = idempotency (eventId se dobara wali khabar pakad ke chhodna).

**Maan lo worker / consumer ne Arpan ke order #501 ka SMS bhej diya, par bookmark (commit) aage khiskaane se pehle gir gaya. Naya worker / consumer wahi message dobara uthata hai, Arpan ko do SMS nahi jaane chahiye.**
Arpan (pehli soch): Parchi pe likh lo "Ravi ko 10,000 dene hain", de diye to parchi pe DONE, aur paise dena + DONE likhna ek hi kaam bana do, ya dono ho ya koi nahi.
Pench: paise dena (SMS company) aur parchi (hamara DB) do alag jagah hain, ek jhatke me nahi ho sakte. SMS chala gaya aur DONE likhne se pehle gira, ye beech ka pal kabhi nahi hatega. Is pal ko rok nahi sakte, isliye dobara aana maan ke chalo.
Arpan (aakhri): Har message ki ek pakki id bana do. Is id se ek SMS chala gaya; wahi id dobara aaye to discard kar do, chahe naya worker / consumer laaye ya doosri SMS company se jaaye.
Technical me: "rok nahi sakte, dobara aayega hi" = Kafka at-least-once, "pakki id, dobara aaye to discard" = idempotent consumer (idempotency key, eventId / order id se), wahi id SMS company ko bhi bhejo taaki wo bhi dobara na bheje.
**Maan lo pooche exactly once kyun nahi:** Arpan: Exactly once main scope ke bahar rakhta hoon; Kafka exactly once sirf Kafka se Kafka tak deta, SMS bahar ki cheez hai.
Technical me: Kafka EOS = sirf Kafka ke andar (read-process-write), bahar ka SMS = at-least-once + pakki id se dohra phenko.

**7. Ticket dene wala (Order Service) gate tak khabar bhejne se pehle hi gir gaya.**
Arpan: Yahan pe outbox. Matlab wo pehle kahin likh de ki haan, main ye daal dunga. Kabhi koi mar bhi jaaye, to wo cheez ek jagah mil jaaye. Jaise real life me hota hai: baap marne se pehle apni sampatti likh ke chala jaata hai.
Technical me: "pehle kahin likh de" = outbox (khabar usi DB transaction me), "baap ki vasiyat, baad me mil jaaye" = relay outbox se utha ke Kafka ko bhejta hai.
**Maan lo wo dabba (outbox) hi gir jaaye:** Arpan: Kafka aur bhejne ke beech kuch gira to use dabbe me kahin store kar lo, dikkat khatam; wo dabba hi gir jaaye to uska replica bana do. Yahi kahani hai.
Technical me: outbox table DB me hi (DB ki apni replica), relay gire to dobara uthata.

**8. Ek fan ka ticket hi galat hai. Wo baar-baar gate pe ghusne ki koshish karta, har baar mana, aur peeche poori line atki.**
Arpan: Use line se hata, usko ghusne hi mat de. Tu bahar, tu aayega hi nahi. Matlab mere paas ek aisi cheez ho jo uske gate pe hi rok de, andar hi na aane de. Matlab bouncer, hahaha.
Technical me: "line se hata" = DLQ (alag line, baaki line chale), "bouncer gate pe hi rok de" = galat khabar ko pehle hi rokna (validation).

**Maan lo Arpan ki "pay" khabar DLQ me gayi, peeche ki "cancel" chal gayi, kram toota.**
Arpan: Cinema hall ka ticket checker: 3 logon ka group aaya (order, pay, cancel). Ek gadbad nikla to use jaane nahi dunga. Ya to teeno mat jao, ya pehla (order) chala gaya to baaki do (pay, cancel) abhi mat jao; jab wo theek ho jaaye to pay aur cancel ek saath dobara jaayenge. Isse kram nahi tootega. Jaise ticket pe likha A1, A2, A3: A1 andar chala gaya, A2 ruk gaya, to A3 ko A2 ki seat pe nahi bhejunga. A2 jaayega to A3 jaayega, A2 cancel to A3 bhi ruka. Stadium me do saathi aaye, ek ki jeb me hathiyar nikla: dono bahar, ek nahi. Pehle PAY, phir CANCEL; PAY nahi hua to dono hi nahi, dono baad me line me.
Technical me: "3 logon ka group" = ek order ki khabar (key = order id, wahi 3b wali, ek partition), "gadbad wala atka to baaki do bhi ruko" = us key ki aage ki khabar bhi park karo (DLQ + same key ki parking), theek hone pe kram se dobara; baaki users chalte rahein. Kafka kram ek partition ke andar pakka deta hai; yahan kram humare consumer ne toda, jab pay ko DLQ bhej ke cancel pe aage badh gaya.

**9. Andar ka SMS counter 10 minute ke liye band. Jo fan wahan pahunchta hai sab fail, sab alag line (DLQ) me ja rahe.**
Arpan: Nahi, maan lo 10 minute kuch dikkat hai, wo 11ve minute khul sakta hai. Sab DLQ me nahi bhejunga. Pehle dekhunga ki kya hai, kya waqai wo band hai, main check karunga. Aur ek service nahi, bahut saari services hain. Agar wo waqai band hai to uska traffic kahin aur bhej dunga. Jaise aam road pe: mujhe kahin jaana hai, wo road band hai, to traffic divert kar do.
Technical me: "sab DLQ me nahi, pehle check" = circuit breaker, "check karunga kya waqai band" = heartbeat / health check, "traffic divert" = fallback (doosri SMS company).

**10. Koi line me 7 din se zyada peeche reh gaya, aur gate ki 7 din purani list ka panna phaad diya jaata hai.**
Arpan: Wahan bheed bahut zyada hai, aur admin daalo wahan. Use itna peeche rehne hi mat do na, wahan aisa karo taaki wo bheed 7 din tak peeche na rahe.
Technical me: "aur admin daalo" = pehle checker (consumer) badhao, gate (30) tak; gate bhar gaye tab gate (partition) bhi badhao, isliye shuru se zyada gate rakhe the, "itna peeche rehne hi mat do" = retention khatam hone se pehle lag pakadna.

**Maan lo wo dheere-dheere peeche ja raha: aaj 1 din, kal 2 din. 7 din hone se pehle kaise pata chalega?**
Arpan: Alert laga do. Wo peeche ja raha, alert aa gaya. Wo aise bolta: "abe ye bahut peeche hai, admin jaldi-jaldi kaam karo, itna peeche kyun ja raha hai."
Technical me: "alert laga do" = consumer lag alert.
**Isi dikkat ki doosri baat (29-Oct):** Arpan: Fraud service 7 din peeche chal rahi, kal 8, parson 9. Hum aisa thodi karenge ki bookmark aage badha do, "ab tu na padh"; khabar to use padhwani hi hai, Kafka ka matlab wahi hai. To retention se pehle us khabar ko kahin aur daal do, wo wahan se padh legi apne hisaab se, baaki service pe asar nahi; ya retention 7 din se badha do.
Technical me: "bookmark aage mat khiskao" = offset skip nahi (data loss), "kahin aur daal do" = archive / alag topic (S3 sink), "retention badha do" = retention.ms.

&nbsp;

Poora design = [12_message_queue_kafka.md](12_message_queue_kafka.md)
