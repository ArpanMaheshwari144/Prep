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
// ═══════════════════════════════════════════════════════════════════════

public class Main {

    public static void main(String[] args) throws InterruptedException {

        // ─────────────────────────────────────────────────────
        // 📋 REQUIREMENT 1
        // ─────────────────────────────────────────────────────
        // Singleton se bank ka data print kar (BankConfig).
        //
        // ↓↓↓ tu likh ↓↓↓
        // BankConfig enum singleton — INSTANCE same object har baar
        System.out.println(BankConfig.INSTANCE.getBankName());           // bank ka naam
        System.out.println(BankConfig.INSTANCE.getMaxTransactionLimit()); // max limit
        System.out.println(BankConfig.INSTANCE.getCurrency());            // currency

        // ─────────────────────────────────────────────────────
        // 📋 REQUIREMENT 2
        // ─────────────────────────────────────────────────────
        // App ke saare layers wire kar:
        // Repository, EventPublisher, listener subscribe, Service.
        //
        // ↓↓↓ tu likh ↓↓↓
        // Repository — interface type, concrete impl assign (DIP)
        AccountRepository repository = new InMemoryAccountRepository();

        // Publisher — Observer pattern hub
        EventPublisher publisher = new EventPublisher();

        // Subscribe — Email listener register; transfer pe automatic notify
        publisher.subscribe(new EmailListener());

        // Service — repository + publisher inject (Constructor Injection — DIP)
        AccountService service = new AccountService(repository, publisher);

        // ─────────────────────────────────────────────────────
        // 📋 REQUIREMENT 3
        // ─────────────────────────────────────────────────────
        // Factory se 3 accounts banao — alag-alag types
        // (savings, current, fd). Service ke through register kar.
        //
        // ↓↓↓ tu likh ↓↓↓
        // Factory ko bas type bola — andar concrete class decide karta (Factory Pattern)
        Account a1 = AccountFactory.create("savings", "S101", "Arpan", 5000);   // SavingsAccount banegi
        Account a2 = AccountFactory.create("current", "C201", "Rahul", 10000);  // CurrentAccount
        Account a3 = AccountFactory.create("fd",      "F301", "Priya", 50000);  // FixedDeposit

        // Service ke through register — duplicate ID validation karega
        service.createAccount(a1);
        service.createAccount(a2);
        service.createAccount(a3);

        // ─────────────────────────────────────────────────────
        // 📋 REQUIREMENT 4
        // ─────────────────────────────────────────────────────
        // Teeno accounts ka interest print kar.
        // (Polymorphism dikhega — ek hi method, alag output)
        //
        // ↓↓↓ tu likh ↓↓↓
        // Same method calculateInterest() — har class ka apna logic (Polymorphism)
        System.out.println(a1.calculateInterest());   // Savings: 5000 × 4%  = 200
        System.out.println(a2.calculateInterest());   // Current: hardcoded  = 0
        System.out.println(a3.calculateInterest());   // FD:      50000 × 7% = 3500

        // ─────────────────────────────────────────────────────
        // 📋 REQUIREMENT 5
        // ─────────────────────────────────────────────────────
        // Service ka stream-wala method use kar — saare account
        // holders ke naam print kar.
        //
        // ↓↓↓ tu likh ↓↓↓
        // Streams: source(findAll) → stream → map(name) → collect → list
        List<String> list = service.getAccountHolderNames();
        for (String s : list) {                              // har naam print
            System.out.println(s);
        }

        // ─────────────────────────────────────────────────────
        // 📋 REQUIREMENT 6
        // ─────────────────────────────────────────────────────
        // Ek normal transfer kar — service.transfer(...).
        // Observer (EmailListener) automatic react karega.
        //
        // ↓↓↓ tu likh ↓↓↓
        // Transfer — service.transfer() call karte hi:
        //   1. paisa A→B move (synchronized)
        //   2. transfer success → publisher event fire (try-catch ke bahar)
        //   3. EmailListener ka onEvent() automatic chalega
        service.transfer(a1.getAccountId(), a2.getAccountId(), 1000);

        // ─────────────────────────────────────────────────────
        // 📋 REQUIREMENT 7
        // ─────────────────────────────────────────────────────
        // Ek aisa transfer kar jo FAIL ho (bahut zyada amount).
        // Try-catch lagao, exception pakad ke message print kar.
        //
        // ↓↓↓ tu likh ↓↓↓
        // Bahut zyada amount — Account.withdraw() InsufficientFundsException throw karega
        // Service ke try-catch + manual rollback bhi handle karega
        try {
            service.transfer(a1.getAccountId(), a2.getAccountId(), 10000000);
        } catch (InsufficientFundsException e) {
            // Specific exception caught — program crash nahi hota
            e.printStackTrace();
        }

        // ─────────────────────────────────────────────────────
        // 📋 REQUIREMENT 8 (LAST)
        // ─────────────────────────────────────────────────────
        // BatchTransferService use karke 3-5 transfers PARALLEL chala.
        // Steps:
        // 1. BatchTransferService instance bana
        // 2. TransferRequest list bana
        // 3. executeBatch call kar
        // 4. shutdown call kar
        // 5. Thread.sleep(2000) — background threads complete ho
        //
        // ↓↓↓ tu likh ↓↓↓
        // BatchTransferService — andar 5-thread pool hai (ExecutorService)
        BatchTransferService bs = new BatchTransferService(service);

        // 3 transfer requests — ek list mein
        List<TransferRequest> tf = List.of(
                new TransferRequest(a3.getAccountId(), a1.getAccountId(), 100),
                new TransferRequest(a3.getAccountId(), a2.getAccountId(), 200),
                new TransferRequest(a1.getAccountId(), a2.getAccountId(), 50));

        bs.executeBatch(tf);     // saari requests parallel chalengi (lambda → Runnable)
        bs.shutdown();           // graceful — naya kaam nahi, current finish
        Thread.sleep(2000);      // 2 sec wait — background threads complete

    }
}

// ═══════════════════════════════════════════════════════════════════════
// 🚀 RUN KARNE KE LIYE (terminal mein):
//
// 1. Project root mein jao:
// cd C:\DSA_PRACTICE\01_JAVA\07_PROJECT\SimpleBankSystem
//
// 2. Compile kar (saari .java files):
// javac -d out src/com/arpan/bank/**/*.java src/com/arpan/bank/Main.java
// (ya VS Code mein right-click Main.java → Run Java)
//
// 3. Run kar:
// java -cp out com.arpan.bank.Main
//
// 4. Output dekh:
// • Bank info, account names, interest values
// • 📧 [Email] log automatic transfer pe
// • Failed transfer ka error message
// • Multiple parallel transfer logs
// ═══════════════════════════════════════════════════════════════════════
