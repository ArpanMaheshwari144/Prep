# Stock trading: hand notes (Arpan aur Ravi)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Scene:** Arpan TCS ke 10 share 3500 pe kharidna chahta hai, Ravi apne 10 TCS 3500 pe bechna chahta hai. Dono ek-doosre ko jaante nahi.
**Shabd:** beech ka aadmi = matching engine · do list = order book · chaabi = client_order_id · paisa hold = BLOCK · ek line = ek share ek thread · parchi = order log · 10 ladkon ki fauj = Kafka

Banking se aaya: chaabi (idempotency). BookMyShow se: hold. Kafka se: fauj. Chat se: taar + chhota ladka. Naya: matching, ek line, parchi se dobara jamana.

&nbsp;

**1. Arpan aur Ravi ek-doosre ko jaante nahi, lakhon log**
Beech me ek aadmi baitha do. Wo dekhe Arpan ko kis bhaav me chahiye, Ravi kis bhaav me de raha, bhaav mila to dono ka sauda kara de.
**matching engine + order book**
Interview: "Har share ki do list: kharidne wale (ooncha bhaav upar), bechne wale (sasta upar). Upar wale mile to sauda. Ek bhaav pe pehle aaya pehle (price-time)."

**2. Net gaya, app ne "Buy 10" dobara bheja, 20 kharid liye**
Order ke saath ek chaabi. Wahi chaabi dobara aaye to "nikal, ye ho chuka".
**idempotency key, (user_id, client_order_id) UNIQUE**
Interview: "App har order pe id lagati, retry pe wahi. DB me ye jodi UNIQUE, dusra order ghus hi nahi paata, pehle wale ka jawab wapas."

**3. 35,000 se do order, dono match ho gaye**
Paisa hold kar lo, jaise EMI ke liye bank hold karta. Kahin aur kharch hi na ho.
**funds BLOCK**
Interview: "Order pe ek query: available -35,000, blocked +35,000, shart available >= 35,000. Doosra order wahin reject. Sauda pe blocked se kato, cancel pe wapas."

**4. Do thread ne Ravi ke wahi 10 share do logon ko bech diye**
Sab ek line me, ek jagah. Baanto mat.
**ek share ki book, ek thread**
Interview: "TCS ki book ko sirf ek thread chhoota, order line me ek-ek. Lock nahi chahiye. Speed ke liye alag share alag thread pe."

**5. Machine mari, book RAM ke saath udi, Arpan ka adhoora order bhi**
Subah se ek parchi me sab likho, jaise bag ki list. Bag bigde to khaali karo aur parchi dekh ke dobara jamao. Parchi wahi ki wahi rehti hai.
**order log + replay (+ standby)**
Interview: "Order aate hi pehle log me line, phir engine. Log me sirf neeche judta. Machine mari to log dobara chala ke wahi book. Standby saath-saath log padhta, turant aage aata. Pehle numbers poochho: kam bheed me DB pe bhi chal jaata, NSE jaisi bheed me RAM + log."

**6. Sauda hua, sabko batana hai, engine ruke nahi**
Engine khud nahi jaata. 10 ladkon ki fauj ko de deta hai, har ladka apna kaam kare.
**Kafka (trades topic)**
Interview: "Engine sauda trades topic me ek baar daal ke aage. Portfolio, ledger, SMS, price, sab apne consumer group se. SMS dheema to bas wahi peeche."

**7. Portfolio DB band, paisa kata, share dikh nahi rahe**
Sauda ulta nahi, match ho chuka, warna consumer court. Arpan ko bolo "thodi der me dikhega". Parchi apne paas, DB aaye to de do.
**ulta nahi: PENDING + retry + alert**
Interview: "Message Kafka me pada, consumer DB aate hi dobara uthata, trade id se likhta to share do baar nahi judte. User ko PENDING, ops ko alert."

**8. TCS ka bhaav 10 lakh screen pe, har second**
Wahi chat wala: taar khula rakho, beech ka chhota ladka bhaav pahunchaye.
**WebSocket + Redis pub/sub**
Interview: "Price Service har badlaav Redis pub/sub pe, har WebSocket server apne jude logon ko push. App poochhti nahi."

&nbsp;

Poora design = [05_stock_broker_trading.md](05_stock_broker_trading.md)
