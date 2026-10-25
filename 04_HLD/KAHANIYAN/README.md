# Tech ki kahaniyan: ye cheez bani kyun

Ek nazar me: har tool ki ek line (kya dikkat thi -> kya hal bana). Poori kahani padhni ho to naam pe click.
Har kahani alag chhoti file, taaki koi file lambi na ho.

| Tool | Saal | Dikkat -> hal (ek line) | Design me kahan |
|---|---|---|---|
| [Redis](redis.md) | 2009 | live visits ki chhoti, hamesha badalti list MySQL (disk) pe dheemi -> data RAM me + list / sorted set jaise dabbe | feed / cache / rate limiter |
| [Kafka](kafka.md) | 2011 | LinkedIn pe 25 pipe ka jaal, purani queue padhte hi mitati -> ek register, sab apni raftaar se, dobara padh sakte | notification, twitter, news |
| [RabbitMQ](rabbitmq.md) | 2007 | bank (JP Morgan, AMQP) ko system ke beech bharose se message -> daakghar: ek chitthi ek worker | kaam baantna |

Dono ka farak aur kab kaunsa: [Kafka ya RabbitMQ](../SYSTEM_DESIGNS/12_message_queue_kafka/kafka_ya_rabbitmq_kab_kaunsa.md)
