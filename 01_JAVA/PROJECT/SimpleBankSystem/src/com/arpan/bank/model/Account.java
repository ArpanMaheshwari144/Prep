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
//
// ╔═══════════════════════════════════════════════════════════════════╗
// ║  🎨 DESIGN PATTERN: TEMPLATE METHOD (via abstract class)           ║
// ╠═══════════════════════════════════════════════════════════════════╣
// ║                                                                   ║
// ║  Abstract class COMMON workflow define karti, subclasses          ║
// ║  SPECIFIC steps implement karte.                                  ║
// ║                                                                   ║
// ║  Common (concrete methods here):                                  ║
// ║    • deposit()  — saare accounts mein same                        ║
// ║    • withdraw() — saare accounts mein same                        ║
// ║                                                                   ║
// ║  Subclass-specific (abstract):                                    ║
// ║    • calculateInterest() — Savings 4%, FD 7%, Current 0%          ║
// ║                                                                   ║
// ║  📐 SOLID — OCP (Open/Closed):                                    ║
// ║  Naye account types add karne ke liye `Account` extend karo —     ║
// ║  parent class modify NAHI karna. Open for extension.              ║
// ║                                                                   ║
// ║  📐 SOLID — LSP (Liskov Substitution):                            ║
// ║  Sab subclasses (SavingsAccount, FixedDeposit, CurrentAccount)    ║
// ║  Account ki place pe substitute ho sakte —                        ║
// ║      Account a = new SavingsAccount(...);  ✅                     ║
// ║      Account a = new FixedDeposit(...);    ✅                     ║
// ║  Behavior consistent, no surprising exceptions.                   ║
// ║                                                                   ║
// ║  🎤 INTERVIEW LINE:                                                ║
// ║  "Account abstract class Template Method-style — deposit() aur    ║
// ║   withdraw() concrete (saare accounts mein same),                 ║
// ║   calculateInterest() abstract (har subclass apna formula).       ║
// ║   Common framework + specific behavior. OCP + LSP follow karta."  ║
// ║                                                                   ║
// ╚═══════════════════════════════════════════════════════════════════╝

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
