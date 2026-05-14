package com.arpan.bank.service;

import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Multiple transfers ek saath PARALLEL chalata
//    Sequential 1000 transfers = slow
//    Pool ke 5 threads = ~5x faster
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — THREAD POOL ACTION:
//    1000 transfer requests (queue)
//               │
//               ▼
//    ┌─────────────────────────────┐
//    │   ExecutorService Pool      │
//    │   (5 threads fixed)         │
//    │                              │
//    │   T1: req 1 — transfer       │
//    │   T2: req 2 — transfer       │
//    │   T3: req 3 — transfer       │
//    │   T4: req 4 — transfer       │
//    │   T5: req 5 — transfer       │
//    └─────────────────────────────┘
//            │ when free
//            ▼
//    pick next from queue (req 6, 7, ...)
//    = Parallel processing
//
// CONCEPTS IN THIS FILE:
//    1. ExecutorService
//       newFixedThreadPool(5) — 5 worker threads
//
//    2. Lambda Runnable
//       pool.submit(() -> { ... })
//       = Inline Runnable, no boilerplate
//
//    3. Static Inner Class (TransferRequest)
//       Data carrier (fromId, toId, amount)
//       Outer instance NAHI chahiye
//
//    4. Exception Isolation
//       try-catch in lambda
//       Ek fail = baki chalu rahe
//
//    5. Graceful Shutdown
//       pool.shutdown() — naye tasks reject, current finish
//
// WHY FIXED POOL (not Cached)?
//    Fixed (5 threads):
//       ✅ Bounded resource
//       ✅ Predictable
//       ✅ Production safe
//
//    Cached:
//       ⚠️ Unlimited growth
//       ⚠️ 1000 requests = 1000 threads
//       ⚠️ OutOfMemoryError risk
//
// EXCEPTION ISOLATION:
//    Sequential:
//       for (req : requests) {
//           transfer(req);   // ek fail = loop crash
//       }
//
//    Parallel + isolated:
//       pool.submit(() -> {
//           try { transfer(req); }
//           catch (Ex e) { log error; }   // isolated
//       });
//    = 1 fail, baki 999 chalte
//
// REAL PRODUCTION:
//    Spring Boot:
//       @Async                  — same pattern automated
//       ThreadPoolTaskExecutor  — production-grade
//    = Yeh manual version hai
//
// 📐 SOLID:
//    SRP — Sirf batch processing
//    DIP — AccountService inject (constructor injection)
// ═══════════════════════════════════════════════════════════════════════

public class BatchTransferService {

    private final AccountService accountService;
    private final ExecutorService pool;

    public BatchTransferService(AccountService accountService) {
        this.accountService = accountService;
        // Fixed pool of 5 threads — production safe (cached pool = OOM risk)
        this.pool = Executors.newFixedThreadPool(5);
    }

    // ───────────────────────────────────────────────────────────
    // TransferRequest — simple data class (1 transfer ki info)
    // static inner class — outer instance ki zarurat nahi
    // ───────────────────────────────────────────────────────────
    public static class TransferRequest {
        private final String fromId;
        private final String toId;
        private final double amount;

        public TransferRequest(String fromId, String toId, double amount) {
            this.fromId = fromId;
            this.toId = toId;
            this.amount = amount;
        }

        public String getFromId() { return fromId; }
        public String getToId()   { return toId; }
        public double getAmount() { return amount; }
    }

    // ───────────────────────────────────────────────────────────
    // executeBatch — list of transfers parallel via thread pool
    // Exception isolation: ek transfer fail = batch continue
    // ───────────────────────────────────────────────────────────
    public void executeBatch(List<TransferRequest> requests) {
        for (TransferRequest req : requests) {
            pool.submit(() -> {
                try {
                    accountService.transfer(req.getFromId(), req.getToId(), req.getAmount());
                } catch (Exception e) {
                    // Ek transfer fail = batch crash nahi, baki chalein
                    System.err.println("Transfer failed: " + e.getMessage());
                }
            });
        }
    }

    // Graceful shutdown — naye tasks accept nahi, current finish karega
    public void shutdown() {
       pool.shutdown();
    }
}
