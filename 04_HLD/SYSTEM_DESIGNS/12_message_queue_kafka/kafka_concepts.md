# Kafka concepts: ek-ek karke

Kahani: Arpan ne Swiggy pe order kiya. Order Svc ka kaam bas order banana hai. Is order ki khabar do aur
service ko chahiye: SMS Svc (Arpan ko SMS) aur Fraud Svc (order shaq wala to nahi).

---

## 1. Topic

```
                                          +--> [ SMS Svc ]
 [ Order Svc ] --khabar--> [ order-events ]
                              (topic)     +--> [ Fraud Svc ]
```

- Order Svc khabar ek naam wali line me daal deta hai. Line ka naam "order-events". Kafka me aisi naam wali
  line ko **topic** kehte hain.
- Order Svc ko pata hi nahi aage kaun padh raha. Khabar daal ke apne kaam pe.
- SMS aur Fraud dono isi topic se padhte hain. Padhne se khabar mitti nahi, isliye dono ko poori khabar.
- Kal teesri service aaye (Analytics), to bas isi topic se padhe. Order Svc me ek line nahi badalti.

---

## 2. Partition (topic ke tukde)

**Peeche se:** order-events ek topic, ek line, Kafka ki ek machine pe.
**Ab:** Diwali sale, har second lakhon order. Ek machine itna bojh nahi utha paati.

```
 topic: order-events
   [ Tukda P0 ]  machine A   <- Ravi ki khabar
   [ Tukda P1 ]  machine B   <- Arpan ki khabar
   [ Tukda P2 ]  machine C   <- Sita ki khabar
```

- Topic wahi ek, order-events. Andar 3 tukde. Ek tukde ko **partition** kehte hain.
- Har tukda alag machine pe, to bojh teen machine me bat gaya.
- Kaunsi khabar kis tukde me, ye Order Svc ek **key** se tay karta hai. Key = user ki id. Same user ki khabar
  hamesha same tukde me, isliye Arpan ki saari khabar (order, pay, cancel) P1 me.
- Kram (order, phir pay, phir cancel) sirf ek tukde ke andar pakka. Isliye key = userId, taaki ek user ki
  khabar ek tukde me rahe aur kram me rahe.
