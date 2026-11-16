# RabbitMQ kaise bana

[<- saari kahaniyan](README.md) · farak / kab kaunsa: [Kafka ya RabbitMQ](../SYSTEM_DESIGNS/12_message_queue_kafka/kafka_ya_rabbitmq_kab_kaunsa.md)

RabbitMQ Kafka se pehle aaya tha.

```
2007  RabbitMQ   (Erlang me likha, AMQP naam ke niyam pe bana)
2011  Kafka      (LinkedIn, upar wali kahani)
```

RabbitMQ jis AMQP niyam pe bana hai, wo **JP Morgan** me shuru hua tha (2003, John O'Hara). Bank ko alag-alag
system ke beech bharose se message bhejne ka ek khula tareeka chahiye tha. Usi niyam pe baad me RabbitMQ bana.

Bank ke liye "padh liya to mita do" hi sahi tha. Arpan: bank hai, chitthi baar-baar kyun padhega; chitthi khatam, kaam khatam,
paisa jama, chitthi phaad ke phenk do. Jaise pehle cinema me gate wala ticket phaad deta tha taaki usi ticket pe koi doosra na
ghuse. LinkedIn ki dikkat alag thi (ek khabar kai jagah, dobara padhna), isliye unhone Kafka bana liya.

Baad me RabbitMQ ne bhi ye kaam jod liya: 2021 me RabbitMQ 3.9 me **Streams** aaya. Source: rabbitmq.com blog 2021-07-13
"RabbitMQ Streams Overview": "A RabbitMQ stream models an append-only log with non-destructive consuming semantics." Matlab
likhte jao, padhne se mit-ta nahi, dobara padh sakte ho, Kafka jaisa.
