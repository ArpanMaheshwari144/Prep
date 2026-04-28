package com.arpan.bank.observer;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Subscriber ka contract — koi bhi class jo events sunna chahti, ye
//    interface implement kare aur onEvent() method mein apna kaam likhe.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • OOPS — Interface (sirf rule, koi logic nahi)
//    • Concrete classes (EmailListener, AuditListener etc.) implement karenge
// ═══════════════════════════════════════════════════════════════════════

public interface EventListener {

    // Event aaye toh ye method automatically chalega
    void onEvent(TransactionEvent event);
}
