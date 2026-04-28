package com.arpan.bank.observer;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Event data carrier — "transfer hua" ki info hold karti (from, to,
//    amount, time). Listeners (Email/Audit) ye object receive karte hain.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • OOPS — simple class with private fields + constructor + getters
//    • Encapsulation — fields private final, only getters (immutable data)
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
