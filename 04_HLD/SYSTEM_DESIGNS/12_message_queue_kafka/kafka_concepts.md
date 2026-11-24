# Kafka concepts: stadium ki kahani

Match ka din hai, stadium ke bahar bahut bheed hai. Arpan ticket counter se ticket leta hai.
Har hissa pichhle hisse se aage badhta hai. Jo hissa Arpan ne chat me samjha, wahi yahan likha hai.

---

## 1. Topic

Ticket counter (Order Service) ka kaam bas ticket kaatna hai. Ticket ki khabar andar do kamron ko chahiye: checker
(SMS Service, Arpan ko "seat pakki" ka SMS bhejega) aur security (Fraud Service, dekhega ki ticket nakli to nahi).

Counter kisi kamre ka intezaar nahi karta. Khabar gate ki line me daal deta hai aur agle fan ka ticket kaatne lagta hai.
Stadium me jo line lagti hai, wahi **topic** hai (naam: order-events).

Dono kamre isi line se padhte hain. Padhne se khabar mitti nahi. Kal koi naya kamra khule, to wo bhi isi line se padh
lega, counter me kuch nahi badalta.

---

## 2. Partition

India-Pakistan ka match aaya, har second hazaaron ticket. Ek gate pe itni bheed nahi sambhli.

Topic wahi rakha, bas usi line ke liye 3 gate khol diye. Har gate = **partition**, alag machine pe. Jaise 1000 ki ek
line thi, use 300, 300 aur 400 ki teen alag line me baant diya. Line (topic) wahi rahi, bas gate badh gaye.

Par Arpan ne ticket liya, phir pay kiya, phir cancel kiya. Teeno alag gate pe gaye to andar kaun pehle pahunche, iska
koi bharosa nahi. Kram sirf ek gate ke andar pakka hota hai.

Isliye counter har khabar ke saath **key** deta hai (user id). Same key hamesha same gate pe jaati hai, to Arpan ki
teeno khabar P1 pe, aur kram se.

Jaise teen dost: alag-alag line me ghuse to andar kaun pehle pahunchega, koi kram nahi. Ek hi line me saath ghuse to
pehle main, phir wo, phir wo.

---

## 3. Consumer group

Checker ke kamre (SMS Service) me 3 checker baithe hain: banda-A, banda-B, banda-C. Agar teeno har gate ke saare fan
dekhein, to Arpan ka ticket teeno punch karenge aur Arpan ko 3 SMS chale jaayenge.

Isliye teeno ko ek **team** bana diya (naam "sms"). Is team ko **consumer group** kehte hain. Team ke andar har gate
sirf ek checker ko milta hai:

```
P0 -> banda-A     P1 (Arpan) -> banda-B     P2 -> banda-C
```

Kyun ek gate pe ek hi checker? P1 ki line me Arpan ki teen khabar kram se khadi hain: order, pay, cancel. P1 pe do
checker hote to ek "order" uthata, doosra "cancel". Cancel wala tez nikla to cancel pehle ho jaata, order baad me. Jo
kram hissa 2 me key laga ke bachaya tha, wo toot jaata. Isliye ek gate pe ek hi checker.

Arpan ka ticket sirf banda-B punch karega, to Arpan ko ek hi SMS jaayega. banda-B ne dekh liya, to banda-A aur banda-C
use nahi dekhte.

Security (Fraud) alag kamra hai aur uski alag team hai. Use apni poori line alag se milti hai. Har team apne register me
likhti hai ki line me kahan tak dekh liya (ye **offset** hai). Isliye security dheemi chale to bhi checker nahi rukta,
dono ka hisaab alag hai.

Aur agar banda-B gir gaya, to Kafka uska gate P1 banda-A ya banda-C ko de deta hai. Ise **rebalance** kehte hain.

**Maan lo Diwali sale me tune SMS team me 2 checker aur bitha diye, ab 5 checker aur 3 gate:** ek gate ek hi checker
ko milta hai, to naye do khaali baithenge. Isliye gate bhi badha do: P0, P1, P2, P3, P4. Ab 5 checker 5 gate pe, line
aur bat gayi, kaam tez. (Technical me: pehle partition badhao, phir consumer; consumer partition se zyada ho to khaali.)

---

## 4. Commit

Checker banda-B ka kaam teen kadam ka hai:

```
1. P1 ki line se agli khabar uthao ("2 cancel")
2. Arpan ko SMS bhejo
3. register me likho "agli baar 3 se bhejna"
```

Teesra kadam, yaani register me bookmark aage karna, **commit** kehlata hai.

Ek din banda-B ne SMS bheja aur register me likhne se pehle gir gaya. Bookmark abhi 2 pe hi tha. P1 banda-A ko mila,
usne register dekha aur "2 cancel" dobara uthaya. Arpan ko 2 SMS chale gaye.

Ulta karte (pehle register, phir SMS) to aur bura hota: bookmark 3 pe chala jaata, aur "2 cancel" ka SMS kabhi jaata hi
nahi.

Isliye **pehle kaam, phir commit**. Khabar kabhi khoti nahi, bas kabhi dobara aa jaati hai.

Dobara aayi khabar pakadne ke liye har khabar ka apna number hota hai (eventId), jaise har ticket ka. SMS bhejne se pehle
checker dekhta hai ki ye number pehle aa chuka ya nahi (Redis ya DB me likha rehta hai). Aa chuka to chhod do, nahi aaya
to SMS bhejo aur number likh do. Ye pehchaanna checker (SMS Service) ka kaam hai, gate (Kafka) ka nahi.
