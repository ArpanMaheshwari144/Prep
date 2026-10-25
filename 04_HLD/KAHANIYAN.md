# Tech ki kahaniyan: ye cheez bani kyun

Halki padhai. Har kahani me ek hi baat: kya dikkat thi, aur uske hal se ye tool bana. Design me ye tool kyun lagate
hain, uska jawab yahi kahani hai.

- Kafka (LinkedIn) + RabbitMQ farak: [kafka_vs_rabbitmq.md](SYSTEM_DESIGNS/12_message_queue_kafka/kafka_vs_rabbitmq.md)

---

## Redis kaise bana

2009 me Italy me ek developer tha, Salvatore Sanfilippo. Log use "antirez" naam se jaante hain. Wo ek chhota sa tool
bana raha tha, jo websites pe aane wale logon ko live dikhata tha: abhi kaun aaya, kis page pe, kahan se. Har website
ke liye wo aakhri kuch hazaar visits ki ek list rakhta tha.

**Dikkat:** ye list wo MySQL me rakh raha tha. Har visit pe ek likhai, aur puraani entries kaatni bhi padti thi.
Jaise-jaise websites badhi, MySQL ghutne laga. Disk pe har baar likhna aur padhna bahut dheema tha, jabki use bas
ek chhoti si, hamesha badalti list chahiye thi.

**Uska idea:** data ko disk ki jagah RAM me rakho. RAM disk se hazaaron guna tez hai. Aur sirf "key = value" mat do,
seedhe kaam ke dabbe do: list, set, sorted set. Taaki "aakhri 1000 visit rakho, purani kaat do" ek hi command me ho
jaaye.

```
pehle:  har visit -> MySQL (disk) me likho -> purani mitao -> dheema, DB ghut-ta
baad:   har visit -> RAM wali list me daalo -> list khud chhoti rakho -> ~1 ms
```

Usne ye chhota sa server C me likha aur naam rakha **Redis**, yaani REmote DIctionary Server, door rakhi hui ek
dictionary. Phir open-source kar diya. Kuch hi saal me GitHub, Twitter aur Instagram jaisi companies use karne lagi.

**Designs se jod:** news ki feed ki "taazi 500" Redis sorted set me rakhi, aur purani kaat di. Antirez ki asli
dikkat bhi bilkul yahi thi: ek chhoti, hamesha badalti list, jo DB ke liye bhaari thi aur RAM ke liye halki.
