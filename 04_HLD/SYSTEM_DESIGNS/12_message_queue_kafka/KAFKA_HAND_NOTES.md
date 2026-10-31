# Kafka: hand notes (stadium)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Match ka din, stadium ke bahar bahut bheed. Gate pe admin / chowkidar line sambhalta hai (Kafka), andar checker ka kamra aur counter (SMS, Fraud), bahar ticket counter (Order Service). Scene Arpan ne khud chuna.

&nbsp;

**1. Stadium ke bahar bahut bheed, sab ek saath andar ghusna chahte hain.**
Arpan: Kafka mere liye ek aisa banda hai jo bheed ko kaabu karta hai. Stadium ke bahar bahut bheed, sab ghus rahe. Ek admin ne bola: line banao, ek-ek karke jao, seat mili to baith jao. Bas ye hai Kafka mere liye: bheed ko rokne wala. Spike aaya, usne rok diya.
Technical me: "bheed ko kaabu karne wala" = Kafka (beech ki queue), "line banao, ek-ek karke jao" = khabar line me, padhne wale apni raftaar se (async), "spike aaya, rok diya" = spike / load sambhalna.

**2. Line me ek aadmi ruk gaya, peeche sab atak gaye. Wo line se nikal ke bahar chala gaya. (Ye dikkat Arpan ne khud uthayi.)**
Arpan: Maan le stadium me line hai, ek aadmi ruk gaya, wahin se peeche sab disturb ho gaye. Wo line se nikla, bahar chala gaya. Uski seat booked hai, wahi baithega, uski seat pe koi aur nahi baith sakta. To line dobara shuru kari, koi gum nahi hua, kuch dikkat nahi.
Technical me: "seat booked, wahi baithega" = offset (bookmark), "line dobara shuru, koi gum nahi" = restart ke baad wahin se padhna, khabar padhne se mit-ti nahi.

**3. Bheed itni ki ek admin se nahi sambhal rahi.**
Arpan: Bheed bahut hai, wo akela admin nahi sambhal pa raha, to inke liye aur gate khade karo. Har ek ke andar apni line. Yahi Kafka karta hai: broker, partition, aur unke andar line. Kaun kis gate pe jaaye, wahan hash aa gaya.
Technical me: "aur gate" = partition, "har gate ke andar apni line" = kram sirf ek partition ke andar, "kaun kis gate pe, hash" = key ka hash se partition.

**4. Gate ke andar 3 ticket checker, teeno har fan ka ticket punch kar rahe. Har ticket 3 baar punch, line teen guna dheemi.**
Arpan: 3 punch ho gaya hai. To aise kar de, un saalon ko mila de, ek bana de, ek kamre me hi baitha de. Is kamre me 3 baithe hain, koi bhi aaye, bas in 3 me se ek pe bhej. Jisne punch kar diya, baaki 2 usko dekhe hi nahi.
Technical me: "ek kamre me baitha de, ek bana de" = consumer group, "3 me se ek pe bhej" = ek partition group me ek hi consumer ko, "baaki 2 dekhe hi nahi" = group me ek khabar ek hi baar.
**Maan lo har fan ki talaashi bhi honi hai, 2 security wale aaye. Unhe bhi checker ke kamre me bitha diya to "sirf ek ke paas" wala niyam unpe bhi lagega, talaashi chhoot jaayegi.**
Arpan: To alag baitha do. Matlab pehle ticket checker ka room, phir security ka room. Ek ko paar karo, phir doosre room me jao.
Technical me: "alag room" = alag consumer group, har group ko saari khabar ki poori copy.

**5. Line chalane wala admin (chowkidar) hi gir gaya / bahar chala gaya.**
Arpan: Wo admin line se nikla, bahar chala gaya, lekin uski jagah koi aur admin le sakta hai. Koi fail ho jaaye to replica, aur wahin se shuru, bookmark se. Wo to pata hai na.
Technical me: "uski jagah koi aur admin" = replica leader ban jaati hai, "wahin se shuru, bookmark" = offset se aage padhna.

**6. Checker ne fan ko seat pe baitha diya (ticket punch), par register me likhne se pehle gir gaya. Naya checker wahi fan dobara bhejega.**
Arpan: Idempotency check kar le ki wo pehle aaya hai. Bas, samajh ja.
Technical me: "check kar le ki pehle aaya hai" = idempotency (eventId se dobara wali khabar pakad ke chhodna).

**7. Admin gira, aur uski jagah lene wale ke paas aakhri kuch logon ki list hi nahi thi, jabki ticket counter ko "aa gaya" bol diya tha. Arpan ki vasiyat me: baap ne vasiyat ek hi copy me likhi, ghar walon ko bol diya "likh di", phir wo copy jal gayi. "Likh di" kab bolna tha?**
Arpan: Bhai "likh di" aise bolna ki uski copy aur bana, aur wo likh de, tab "likh di". Matlab replica hai, replica ne bol diya "likh liya", sab bolein, tab. Ye raha connection.
Technical me: "copy bhi likh de, tab likh di" = acks=all, "replica ne bol diya likh liya" = in-sync replica ki haan (min.insync = 2).

**8. Ticket dene wala (Order Service) gate tak khabar bhejne se pehle hi gir gaya.**
Arpan: Yahan pe outbox. Matlab wo pehle kahin likh de ki haan, main ye daal dunga. Kabhi koi mar bhi jaaye, to wo cheez ek jagah mil jaaye. Jaise real life me hota hai: baap marne se pehle apni sampatti likh ke chala jaata hai.
Technical me: "pehle kahin likh de" = outbox (khabar usi DB transaction me), "baap ki vasiyat, baad me mil jaaye" = relay outbox se utha ke Kafka ko bhejta hai.

**9. Ek fan ka ticket hi galat hai. Wo baar-baar gate pe ghusne ki koshish karta, har baar mana, aur peeche poori line atki.**
Arpan: Use line se hata, usko ghusne hi mat de. Tu bahar, tu aayega hi nahi. Matlab mere paas ek aisi cheez ho jo uske gate pe hi rok de, andar hi na aane de. Matlab bouncer, hahaha.
Technical me: "line se hata" = DLQ (alag line, baaki line chale), "bouncer gate pe hi rok de" = galat khabar ko pehle hi rokna (validation).

**10. Andar ka SMS counter 10 minute ke liye band. Jo fan wahan pahunchta hai sab fail, sab alag line (DLQ) me ja rahe.**
Arpan: Nahi, maan lo 10 minute kuch dikkat hai, wo 11ve minute khul sakta hai. Sab DLQ me nahi bhejunga. Pehle dekhunga ki kya hai, kya waqai wo band hai, main check karunga. Aur ek service nahi, bahut saari services hain. Agar wo waqai band hai to uska traffic kahin aur bhej dunga. Jaise aam road pe: mujhe kahin jaana hai, wo road band hai, to traffic divert kar do.
Technical me: "sab DLQ me nahi, pehle check" = circuit breaker, "check karunga kya waqai band" = heartbeat / health check, "traffic divert" = fallback (doosri SMS company).

**11. Koi line me 7 din se zyada peeche reh gaya, aur gate ki 7 din purani list ka panna phaad diya jaata hai.**
Arpan: Wahan bheed bahut zyada hai, aur admin daalo wahan. Use itna peeche rehne hi mat do na, wahan aisa karo taaki wo bheed 7 din tak peeche na rahe.
Technical me: "aur admin daalo" = consumer badhao (partition ki ginti tak), "itna peeche rehne hi mat do" = retention khatam hone se pehle lag pakadna.
**Maan lo wo dheere-dheere peeche ja raha: aaj 1 din, kal 2 din. 7 din hone se pehle kaise pata chalega?**
Arpan: Alert laga do. Wo peeche ja raha, alert aa gaya. Wo aise bolta: "abe ye bahut peeche hai, admin jaldi-jaldi kaam karo, itna peeche kyun ja raha hai."
Technical me: "alert laga do" = consumer lag alert.

&nbsp;

Poora design = [12_message_queue_kafka.md](12_message_queue_kafka.md)
