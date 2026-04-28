package com.arpan.bank.service;

import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Multiple transfers ek saath PARALLEL chalata. Sequential 1000 transfers
//    slow honge — pool ke 5 threads se 5x faster.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • Multithreading — ExecutorService (newFixedThreadPool — production safe)
//    • Lambda — pool.submit(() -> { ... }) → Runnable inline
//    • Resource management — pool.shutdown() graceful release
//    • Exception isolation — ek transfer fail toh batch crash nahi
//    • TransferRequest = static inner class (data carrier — fromId/toId/amount)
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
    // 📋 REQUIREMENT (tu likh)
    // ───────────────────────────────────────────────────────────
    // "Bhai, ye method kya kare:
    //   • TransferRequests ki list le
    //   • Har request pe loop chala
    //   • Har request ko thread POOL mein submit kar (parallel chalegi)
    //   • Pool.submit() ke andar:
    //        accountService.transfer(fromId, toId, amount) call kar
    //        Exception aaye toh print karo, batch crash mat hone do
    //
    // Help:
    //   pool.submit(() -> {  ...  });   ← lambda Runnable
    //   try-catch around transfer()      ← ek fail = baki running
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
