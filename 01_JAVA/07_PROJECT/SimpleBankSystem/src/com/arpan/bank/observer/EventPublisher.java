package com.arpan.bank.observer;

import java.util.ArrayList;
import java.util.List;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Observer pattern ka HUB. Subscribers list rakhta. Event aaye toh sab
//    listeners ko notify karta. Service event publish karti — publisher
//    distribute karta.
// ═══════════════════════════════════════════════════════════════════════
//
// ╔═══════════════════════════════════════════════════════════════════╗
// ║  🎨 DESIGN PATTERN: OBSERVER (Pub-Sub)                             ║
// ╠═══════════════════════════════════════════════════════════════════╣
// ║                                                                   ║
// ║  Yeh "Hub" hai — events publish karta, multiple listeners         ║
// ║  subscribe karte. Sender (Publisher) ko receivers (Listeners) ki  ║
// ║  identity nahi pata — pure DECOUPLING.                            ║
// ║                                                                   ║
// ║  Flow:                                                            ║
// ║    1. Listeners subscribe()  → publisher list mein add            ║
// ║    2. Event happen           → publisher.publish(event)           ║
// ║    3. Publisher loops        → har listener.onEvent() call karta  ║
// ║                                                                   ║
// ║  📐 SOLID — OCP (Open/Closed):                                    ║
// ║  Naya listener add karna ho? Sirf naya class banao + subscribe.   ║
// ║  Publisher code unchanged — extension via plug-in.                ║
// ║                                                                   ║
// ║  📐 SOLID — DIP (Dependency Inversion):                           ║
// ║  Publisher `EventListener` interface pe depend karta — concrete   ║
// ║  EmailListener / SMSListener pe nahi. Pure abstraction.           ║
// ║                                                                   ║
// ║  Real-world: Spring `ApplicationEventPublisher`, RabbitMQ,        ║
// ║  Kafka — sab same Observer pattern at scale.                      ║
// ║                                                                   ║
// ║  🎤 INTERVIEW LINE:                                                ║
// ║  "Observer pattern transactions ke liye implement kiya —          ║
// ║   EventPublisher event publish karta, multiple EventListener      ║
// ║   subscribers register hote (EmailListener, future SMS, audit).   ║
// ║   Publisher koi listener specific nahi jaanta — pure decoupling.  ║
// ║   Spring ApplicationEventPublisher same concept use karta."       ║
// ║                                                                   ║
// ╚═══════════════════════════════════════════════════════════════════╝

public class EventPublisher {

    // Saare subscribers ki list
    private final List<EventListener> listeners = new ArrayList<>();

    // Subscribe — naya listener add karo
    public void subscribe(EventListener listener) {
        listeners.add(listener);
    }

    // Publish — saare listeners ko event bhejo
    public void publish(TransactionEvent event) {
        for (EventListener listener : listeners) {
            listener.onEvent(event);   // har listener react karega
        }
    }
}
