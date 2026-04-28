package com.arpan.bank.model;

import com.arpan.bank.exception.InsufficientFundsException;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Account = bank ke saare account types ka PARENT (abstract).
//    Direct object nahi ban sakta — sirf SavingsAccount/Current/FD extend karte.
//
// 📌 JAVA KYA FOLLOW HO RAHA + KYU + KAISE:
//    • OOPS — Abstraction (abstract class, direct instantiation block)
//             Encapsulation (private fields, sirf getters)
//             Inheritance (parent — child accounts isko extend karte)
//             Polymorphism (calculateInterest abstract — har child apna logic)
//    • Multithreading — deposit/withdraw synchronized (2 thread same account
//                       update kare toh race condition na ho)
//    • Custom Exception — InsufficientFundsException throw karta withdraw mein
// ═══════════════════════════════════════════════════════════════════════

/**
 * Account — Abstract Parent (Domain Model)
 *
 * OOP concepts demonstrated:
 *   • Abstraction      — abstract class (direct object nahi banta)
 *   • Encapsulation    — private fields + controlled getters
 *   • Inheritance      — Savings/Current/FixedDeposit extend karte
 *   • Polymorphism     — calculateInterest() har child mein different
 *
 * Multithreading:
 *   • synchronized deposit/withdraw — race condition se bachao
 */
public abstract class Account {

    // Encapsulation — fields private, bahar se direct access nahi
    private final String accountId;
    private final String holderName;
    protected double balance;          // protected — child use kare

    // Constructor — sab fields initialize
    public Account(String accountId, String holderName, double balance) {
        this.accountId = accountId;
        this.holderName = holderName;
        this.balance = balance;
    }

    // Getters (no setters — controlled access)
    public String getAccountId() { return accountId; }
    public String getHolderName() { return holderName; }
    public double getBalance() { return balance; }

    // Common methods — sab accounts mein same kaam
    // synchronized — thread-safe (race condition se bachao)
    public synchronized void deposit(double amount) {
        if (amount <= 0) throw new IllegalArgumentException("Amount > 0 hona chahiye");
        balance += amount;
    }

    public synchronized void withdraw(double amount) {
        if (amount > balance) {
            throw new InsufficientFundsException(
                "Account " + accountId + ": balance " + balance + ", asked " + amount
            );
        }
        balance -= amount;
    }

    // Abstract — har account type apna logic dega (Polymorphism)
    public abstract double calculateInterest();
}
