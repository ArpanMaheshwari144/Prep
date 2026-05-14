package com.arpan.bank.observer;

import java.util.ArrayList;
import java.util.List;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    EventPublisher = OBSERVER pattern ka HUB
//    Subscribers list rakhta
//    Event aaye → sab listeners ko notify karta
//    Service event publish karti — publisher distribute karta
// ═══════════════════════════════════════════════════════════════════════
//
// 🎨 PATTERN: OBSERVER (Pub-Sub) — HUB ROLE
//
// VISUAL — FULL FLOW:
//    AccountService.transfer()
//         │
//         │  publisher.publish(event)
//         ▼
//    ┌─────────────────────────────┐
//    │  EventPublisher              │  ← TU YAHAN (HUB)
//    │                                │
//    │  List<EventListener>:          │
//    │   • EmailListener              │
//    │   • AuditListener              │
//    │   • SMSListener                │
//    │                                │
//    │  publish(event):               │
//    │    for each listener:          │
//    │      listener.onEvent(event)   │
//    └────────┬─────────────────────┘
//             │
//      ┌──────┼──────┐
//      ▼      ▼      ▼
//    Email  Audit  SMS
//    (each receives event independently)
//
// FLOW STEPS:
//    1. App startup:
//       publisher.subscribe(new EmailListener());
//       publisher.subscribe(new AuditListener());
//
//    2. Transfer happens:
//       publisher.publish(new TransactionEvent(...));
//
//    3. Publisher inside:
//       for each listener: listener.onEvent(event)
//
//    4. Each listener wakes up + processes
//
// DECOUPLING POWER:
//    Publisher ko PATA NAHI:
//       • Kaun listeners hain
//       • Kya kar rahe hain
//
//    Publisher SIRF interface (EventListener) jaanta
//    = Pure abstraction
//    = Sender (publisher) ko receiver ki identity zarurat nahi
//
// CONCEPTS:
//    1. ArrayList<EventListener>
//       Dynamic list — grow karte runtime
//
//    2. subscribe / publish methods
//       Standard Observer interface
//
//    3. for-each loop
//       Saare listeners iterate, notify each
//
// 📐 SOLID:
//    OCP — Naya listener add? Just naya class + subscribe
//          Publisher code UNCHANGED
//          = Open for extension via plug-in
//
//    DIP — Publisher EventListener interface pe depend
//          Concrete EmailListener/SMSListener pe NAHI
//          = Pure abstraction
//
// REAL WORLD (Spring):
//    Spring's ApplicationEventPublisher = SAME pattern
//
//    @Service
//    class TransferService {
//        @Autowired ApplicationEventPublisher publisher;
//
//        void transfer(...) {
//            publisher.publishEvent(new TransferEvent(...));
//        }
//    }
//
//    @Component
//    class EmailListener {
//        @EventListener   ← Spring magic
//        void handle(TransferEvent e) { send email }
//    }
//
//    = Spring auto-registers listeners via reflection
//    = Manual pattern automated
//
// OTHER REAL-WORLD EVENT SYSTEMS:
//    • Kafka          (distributed pub-sub at scale)
//    • RabbitMQ       (message broker)
//    • Redis pub-sub  (in-memory)
//    • WebSocket events
//    = All same Observer pattern, scaled differently
//
// 🎤 INTERVIEW LINE:
//    "Observer pattern transactions ke liye implement kiya.
//     EventPublisher publishes events, multiple EventListener
//     subscribers register hote. Publisher koi specific listener
//     NAHI jaanta — pure decoupling via interface.
//     Spring ApplicationEventPublisher same concept use karta."
// ═══════════════════════════════════════════════════════════════════════

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
