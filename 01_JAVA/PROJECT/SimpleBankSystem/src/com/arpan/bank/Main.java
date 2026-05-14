package com.arpan.bank;

import com.arpan.bank.observer.EmailListener;
import java.util.List;

import com.arpan.bank.config.BankConfig;
import com.arpan.bank.exception.InsufficientFundsException;
import com.arpan.bank.factory.AccountFactory;
import com.arpan.bank.model.Account;
import com.arpan.bank.observer.EventPublisher;
import com.arpan.bank.repository.AccountRepository;
import com.arpan.bank.repository.InMemoryAccountRepository;
import com.arpan.bank.service.AccountService;
import com.arpan.bank.service.BatchTransferService;
import com.arpan.bank.service.BatchTransferService.TransferRequest;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Main.java = ENTRY POINT — JVM yahaan se shuru
//    public static void main(String[] args) = JVM entry
//    Project ko run yahin se hoga
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — MAIN DEMONSTRATES ALL CONCEPTS:
//    Main.java
//         │
//         │  Demo flow:
//         │
//         ├── 1. Singleton    → BankConfig.INSTANCE
//         │
//         ├── 2. Wiring        → Repository + Publisher + Service
//         │    (DIP — constructor injection)
//         │
//         ├── 3. Factory       → AccountFactory.create() × 3
//         │
//         ├── 4. Polymorphism  → calculateInterest() × 3 alag
//         │
//         ├── 5. Streams       → getAccountHolderNames()
//         │
//         ├── 6. Transfer      → service.transfer() (Observer fires)
//         │
//         ├── 7. Exception     → catch InsufficientFundsException
//         │
//         └── 8. Threading     → BatchTransferService (ExecutorService)
//
// WIRING SECTION (DIP IN ACTION):
//    AccountRepository repository = new InMemoryAccountRepository();
//    EventPublisher publisher = new EventPublisher();
//    publisher.subscribe(new EmailListener());
//    AccountService service = new AccountService(repository, publisher);
//
//    Yeh dependency injection MANUAL kar raha
//    Spring: @Autowired automatically injects same
//    = Same DIP principle
//
// POLYMORPHISM DEMO:
//    Account a1 = AccountFactory.create("savings", ...);
//    Account a2 = AccountFactory.create("current", ...);
//    Account a3 = AccountFactory.create("fd",      ...);
//
//    a1.calculateInterest();   // 200   (Savings 4%)
//    a2.calculateInterest();   // 0     (Current 0%)
//    a3.calculateInterest();   // 3500  (FD 7%)
//
//    Same method call → different behavior
//    = Runtime polymorphism
//
// THREAD SLEEP TRICK:
//    bs.executeBatch(tf);
//    bs.shutdown();
//    Thread.sleep(2000);   // wait for background threads
//
//    Why sleep?
//       shutdown() = naye tasks reject, current continue
//       Main thread sleep = background threads ko time
//
//    Production better way:
//       executor.awaitTermination(2, SECONDS);
//
// 🎤 INTERVIEW LINE:
//    "Main demonstrates all patterns end-to-end —
//     Singleton (BankConfig), Factory (AccountFactory),
//     Repository (DIP injection), Observer (publisher/listener),
//     Polymorphism (calculateInterest), Streams, Custom Exceptions,
//     and ExecutorService threading. Real Spring would automate
//     wiring via @Autowired, but logic identical."
// ═══════════════════════════════════════════════════════════════════════

public class Main {

    public static void main(String[] args) throws InterruptedException {

        // ─── 1. Singleton — bank-wide config ───────────────────
        System.out.println(BankConfig.INSTANCE.getBankName());
        System.out.println(BankConfig.INSTANCE.getMaxTransactionLimit());
        System.out.println(BankConfig.INSTANCE.getCurrency());

        // ─── 2. Wiring — DIP via constructor injection ─────────
        AccountRepository repository = new InMemoryAccountRepository();
        EventPublisher publisher = new EventPublisher();
        publisher.subscribe(new EmailListener());                   // Observer subscriber
        AccountService service = new AccountService(repository, publisher);

        // ─── 3. Factory — 3 different account types ────────────
        Account a1 = AccountFactory.create("savings", "S101", "Arpan", 5000);
        Account a2 = AccountFactory.create("current", "C201", "Rahul", 10000);
        Account a3 = AccountFactory.create("fd",      "F301", "Priya", 50000);

        service.createAccount(a1);
        service.createAccount(a2);
        service.createAccount(a3);

        // ─── 4. Polymorphism — same method, different behavior ─
        System.out.println(a1.calculateInterest());   // 200    (4% Savings)
        System.out.println(a2.calculateInterest());   // 0      (Current)
        System.out.println(a3.calculateInterest());   // 3500   (7% FD)

        // ─── 5. Streams — account holder names ─────────────────
        List<String> names = service.getAccountHolderNames();
        for (String s : names) {
            System.out.println(s);
        }

        // ─── 6. Transfer — Observer auto-fires email log ───────
        service.transfer(a1.getAccountId(), a2.getAccountId(), 1000);

        // ─── 7. Failed transfer — InsufficientFundsException ───
        try {
            service.transfer(a1.getAccountId(), a2.getAccountId(), 10000000);
        } catch (InsufficientFundsException e) {
            e.printStackTrace();
        }

        // ─── 8. Batch parallel transfers via ExecutorService ───
        BatchTransferService bs = new BatchTransferService(service);
        List<TransferRequest> tf = List.of(
                new TransferRequest(a3.getAccountId(), a1.getAccountId(), 100),
                new TransferRequest(a3.getAccountId(), a2.getAccountId(), 200),
                new TransferRequest(a1.getAccountId(), a2.getAccountId(), 50));

        bs.executeBatch(tf);
        bs.shutdown();
        Thread.sleep(2000);   // wait for background threads to complete
    }
}

// ═══════════════════════════════════════════════════════════════════════
// 🚀 RUN INSTRUCTIONS (terminal):
//
//   cd C:\DSA_PRACTICE\01_JAVA\07_PROJECT\SimpleBankSystem
//   javac -d out src/com/arpan/bank/**/*.java src/com/arpan/bank/Main.java
//   java -cp out com.arpan.bank.Main
//
// Expected output: bank info → account interest → 📧 transfer log →
//                  exception trace → parallel batch logs
// ═══════════════════════════════════════════════════════════════════════
