package com.arpan.bank.observer;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    EmailListener = CONCRETE subscriber (event aaye → email bhejo)
//    Demo mein print karta, real world mein SMTP/SES integration
//    Ek listener example — aur bhi ho sakte (SMS, Audit, Push)
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL POSITION IN OBSERVER PATTERN:
//    EventListener (interface — contract)
//         │
//         │ implements
//         ▼
//    ┌─────────────────────────┐
//    │  EmailListener           │  ← TU YAHAN (concrete)
//    │                          │
//    │   onEvent(event) {       │
//    │     send email...        │
//    │   }                       │
//    └─────────────────────────┘
//
// CONCEPTS:
//    1. INTERFACE IMPLEMENTATION
//       implements EventListener → contract follow
//
//    2. @Override
//       Compile-time safety — interface method correctly implemented
//       Spelling galat? Compiler error
//
//    3. CONCRETE BEHAVIOR
//       Apna kaam: email bhejna (or print in demo)
//
// REAL WORLD:
//    Real production:
//       @Component
//       class EmailListener {
//           @Autowired EmailService emailService;
//
//           @EventListener           ← Spring annotation
//           void handle(TransactionEvent e) {
//               emailService.send(
//                   user.getEmail(),
//                   "Transfer of " + e.getAmount()
//               );
//           }
//       }
//
// 🎨 PATTERN: Observer (concrete subscriber)
//
// 📐 SOLID:
//    SRP — Sirf email notification handle karta, kuch nahi
//          Not business logic, not DB save, not audit
//
// MULTIPLE LISTENERS POSSIBLE:
//    EmailListener  (this file)   → send email
//    AuditListener  (future)      → write audit log
//    SMSListener    (future)      → send SMS
//    SlackListener  (future)      → Slack message
//    = Each implements EventListener
//    = Independent, decoupled
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
