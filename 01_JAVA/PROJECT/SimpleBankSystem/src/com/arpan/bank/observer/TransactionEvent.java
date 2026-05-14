package com.arpan.bank.observer;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    TransactionEvent = EVENT DATA CARRIER
//    "Transfer hua" ki info hold karta (from, to, amount, time)
//    Listeners (Email/Audit/SMS) is object ko receive karte
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL POSITION IN OBSERVER PATTERN:
//    AccountService.transfer()
//         │
//         │  creates event
//         ▼
//    ┌─────────────────────────┐
//    │  TransactionEvent        │  ← TU YAHAN (data)
//    │  - fromId, toId          │
//    │  - amount, timestamp     │
//    └────────────┬─────────────┘
//                 │ passed to
//                 ▼
//    EventPublisher → all listeners
//
// CONCEPTS:
//    1. IMMUTABLE CLASS
//       private final fields (cannot change)
//       No setters
//       Once created, NEVER modified
//
//    2. DATA-ONLY CLASS
//       No business logic
//       Just carrier — like DTO
//
//    3. WHY IMMUTABLE?
//       Multiple listeners receive SAME event
//       Agar 1 listener change kare → others ko galat data
//       = Thread-safe by design
//       = Defensive — corruption se safe
//
//    4. GETTERS ONLY
//       Sirf padhne ke liye, modify nahi
//
// 🎨 PATTERN: Observer (event payload — passed to subscribers)
//
// 📐 SOLID:
//    SRP — Sirf event data carry karta, koi behavior nahi
//
// REAL WORLD COMPARISON:
//    Spring's ApplicationEvent — same pattern
//    Kafka message — same concept
//    All event-driven systems: immutable event payload
// ═══════════════════════════════════════════════════════════════════════

public class TransactionEvent {

    // private final = immutable, change nahi ho sakti
    private final String fromId;
    private final String toId;
    private final double amount;
    private final long timestamp;

    // Constructor — sab fields set
    public TransactionEvent(String fromId, String toId, double amount, long timestamp) {
        this.fromId = fromId;
        this.toId = toId;
        this.amount = amount;
        this.timestamp = timestamp;
    }

    // Getters — sirf padhne ke liye, change nahi
    public String getFromId() { return fromId; }
    public String getToId() { return toId; }
    public double getAmount() { return amount; }
    public long getTimestamp() { return timestamp; }
}
