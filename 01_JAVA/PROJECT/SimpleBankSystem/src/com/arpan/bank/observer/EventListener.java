package com.arpan.bank.observer;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    EventListener = SUBSCRIBER ka CONTRACT (interface)
//    "Tu listener banna chahta? Yeh method implement kar"
//    Concrete listeners (EmailListener, AuditListener, SMSListener)
//    is interface ko implement karenge
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL POSITION IN OBSERVER PATTERN:
//    ┌─────────────────────────┐
//    │  EventListener           │  ← TU YAHAN (contract)
//    │  (interface)             │
//    │                          │
//    │   void onEvent(event)    │
//    └────────────┬─────────────┘
//                 │ implements
//                 ▼
//    ┌─────────────────────────┐
//    │  EmailListener           │  ← concrete subscriber
//    └─────────────────────────┘
//
// CONCEPTS:
//    1. INTERFACE — sirf rule, koi logic nahi
//    2. FUNCTIONAL INTERFACE — single method
//       = Lambda compatible:
//          publisher.subscribe(event -> sout("Email"));
//
// PATTERN: Observer (subscriber side of pub-sub)
//
// SOLID:
//    ISP — SINGLE-method focused interface (perfect ISP)
//          No bloat, easy to implement
//    OCP — Naye listeners add ho sakte
//          Existing code UNCHANGED
//
// FUTURE EXPANSION:
//    Same contract follow karke:
//       EmailListener (already done)
//       SMSListener
//       AuditListener
//       PushNotificationListener
//       SlackListener
//    = Sab same EventListener interface implement
// ═══════════════════════════════════════════════════════════════════════

public interface EventListener {

    // Event aaye toh ye method automatically chalega
    void onEvent(TransactionEvent event);
}
