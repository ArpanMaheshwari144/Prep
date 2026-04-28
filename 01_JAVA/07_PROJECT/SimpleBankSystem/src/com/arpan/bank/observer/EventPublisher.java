package com.arpan.bank.observer;

import java.util.ArrayList;
import java.util.List;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Observer pattern ka HUB. Subscribers list rakhta. Event aaye toh sab
//    listeners ko notify karta. Service event publish karti — publisher
//    distribute karta.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • Collections — List<EventListener> (saare subscribers)
//    • Polymorphism — list mein interface type, concrete classes (Email,
//      Audit, SMS) sab fit
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
