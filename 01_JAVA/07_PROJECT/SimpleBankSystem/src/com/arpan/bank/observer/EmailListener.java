package com.arpan.bank.observer;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Concrete subscriber — event aaye toh email "bhejta" (demo mein print).
//    Ek listener example. Aur bhi ho sakte: AuditListener, SMSListener, etc.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • OOPS — implements EventListener (interface contract)
//             @Override (compile-time safety)
// ═══════════════════════════════════════════════════════════════════════

public class EmailListener implements EventListener {

    @Override
    public void onEvent(TransactionEvent event) {
        // Real world: emailService.send(...)
        // Demo: console pe print (proof — listener actually run hua)
        System.out.println("📧 [Email] ₹" + event.getAmount() +
            " transferred from " + event.getFromId() + " to " + event.getToId());
    }
}
