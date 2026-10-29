# Kafka: hand notes (stadium)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Shabd (poore notes me yahi):** fan = khabar · chowkidar = Kafka broker · gate = partition · counter / kamra = andar ki service (SMS, Fraud) · ticket counter = Order Service

&nbsp;

**1. Ek saath bahut bheed**
Match ke din hazaron fan ek saath ghus rahe hain. Chowkidar ne line lagwa di, fan ek-ek karke andar jaate hain. Ticket counter ko andar walon ka intezaar nahi karna padta.
**Kafka (spike / async)**
Interview: "Kafka beech me line ka kaam karta hai. Order Service khabar daal ke aage badh jaati hai, SMS aur Fraud apni raftaar se padhte hain."

**2. Fan line se bahar gaya**
Fan thodi der ke liye bahar gaya. Uski seat booked rehti hai, wapas aaya to wahin se aage.
**offset (bookmark)**
Interview: "Har consumer group apna offset rakhta hai. Restart ke baad wahin se padhta hai, khabar padhne se mit-ti nahi."

**3. Ek gate se bheed nahi sambhli**
Kai gate khol diye, har gate ki apni line. Ticket number dekh ke fan ko uska gate milta hai, isliye ek parivaar ke saare fan ek hi gate se jaate hain.
**partition + key (hash)**
Interview: "Topic ke 10 partition, key = userId. Ek user ki khabar ek hi partition me jaati hai, isliye kram bana rehta hai."

**4. Ek fan ka ticket 3 baar punch**
3 ticket checker the, teeno har fan ka ticket punch kar rahe the. Teeno ko ek kamre me bitha diya: har fan sirf ek checker ke paas jaata hai, jisne punch kar diya baaki do use dekhte hi nahi. Security wale alag kamre me baithe hain, wo har fan ki talaashi lete hain. Dono kamre saath-saath chalte hain, ek doosre ka intezaar nahi karte.
**consumer group**
Interview: "Ek consumer group ke andar har partition sirf ek consumer ko milta hai. Alag group, jaise Fraud, ko poori copy milti hai."

**5. Chowkidar hi gir gaya**
Bagal me doosra chowkidar wahi list le ke khada tha. Usne mic uthaya aur wahin se line chala di.
**replica**
Interview: "Har partition ki 3 replica. Leader gira to in-sync replica leader ban jaati hai."

**6. Fan seat pe baith gaya, par register me likhne se pehle chowkidar gira**
Naya chowkidar wahi fan dobara bhejta hai. Counter pe pehle dekho: ye fan pehle aa chuka hai? Haan, to dobara mat baithao.
**idempotency (eventId)**
Interview: "Pehle kaam, phir offset commit. Isliye khabar kabhi dobara aa sakti hai, use eventId se pakad ke chhod deta hoon."

**7. "Likh liya" bola, aur list jal gayi**
Chowkidar ne ticket counter ko bol diya "naam likh liya", par list ki copy bani hi nahi thi. "Likh liya" tabhi bolo jab doosre chowkidar ki list me bhi naam aa jaaye.
**acks=all, min.insync = 2**
Interview: "Producer acks=all rakhta hai, min.insync.replicas 2. Kam se kam 2 copy pe likhe bina 'ho gaya' nahi aata."

**8. Ticket counter wala gate tak bhejne se pehle gira**
Ticket counter wala har ticket pehle apne register me likhta hai, phir runner gate tak le jaata hai. Runner gira, to bhi register me likha hai. Doosra runner register se utha ke le jaata hai.
**outbox**
Interview: "Order aur khabar ek hi DB transaction me likhta hoon. Relay outbox se utha ke Kafka ko bhejta hai."

**9. Galat ticket wala fan baar-baar ghus raha, line atki**
Do-teen koshish ke baad use line se hata ke alag line me khada karo, taaki baaki fan chalein. Baad me koi uska ticket dekh lega. Ya bouncer gate pe hi galat ticket rok de.
**DLQ**
Interview: "3-4 retry ke baad message DLQ me jaata hai aur alert lagta hai, taaki partition atke nahi."

**10. Andar ka SMS counter 10 minute band**
Saare fan ko alag line me mat bhejo, galti unki nahi hai. Pehle check karo ki counter sach me band hai. Band hai to fan ko rok do ya doosre counter pe bhej do, aur thodi-thodi der me dekhte raho ki khula ya nahi.
**circuit breaker, heartbeat, fallback**
Interview: "Saare message fail hone lagein to breaker khol ke padhna rokta hoon. Health check se pata chalte hi wapas chalu, ya doosri SMS company pe."

**11. Ek gate ki line 7 din peeche, list ka panna phat jaayega**
Itna peeche jaane hi mat do. Alert lagao ki line peeche ja rahi hai, aur us gate pe aur checker lagao.
**lag alert, consumer badhao**
Interview: "Consumer lag pe alert hai. Badhe to consumer badhata hoon, partition ki ginti tak. Retention 7 din hai, usse pehle pakadna hai."

&nbsp;

Poora design = [12_message_queue_kafka.md](12_message_queue_kafka.md)
