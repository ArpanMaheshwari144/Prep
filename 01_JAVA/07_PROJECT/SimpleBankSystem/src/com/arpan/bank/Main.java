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
//    Main.java = ENTRY POINT. Project ko run yahin se hoga.
//    JVM `public static void main` se shuru karta.
//
//    Demo flow:
//      1. Singleton config print
//      2. Wire layers (Repository, EventPublisher, Service)
//      3. Factory se 3 accounts banao
//      4. Polymorphism — har account ka interest print
//      5. Streams — getAccountHolderNames()
//      6. Transfer (Observer auto-notify)
//      7. Failed transfer (custom exception)
//      8. Parallel batch transfers (ExecutorService)
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
