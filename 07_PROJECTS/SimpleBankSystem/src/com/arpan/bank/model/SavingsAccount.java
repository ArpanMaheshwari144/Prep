package com.arpan.bank.model;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    SavingsAccount = Account ka CONCRETE CHILD
//    "Bank ka savings account" — 4% interest deta
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL POSITION IN HIERARCHY:
//    Account (abstract — parent)
//       │
//       │ extends
//       ▼
//    SavingsAccount  ← TU YAHAN (4% interest)
//
// CONCEPTS IN THIS FILE:
//    1. INHERITANCE
//       extends Account → parent ke saare fields + methods milte
//
//    2. SUPER CONSTRUCTOR
//       super(id, name, balance) — parent ko initialize
//
//    3. POLYMORPHISM
//       @Override calculateInterest() — apna 4% logic
//
//    4. STATIC FINAL CONSTANT
//       INTEREST_RATE = 0.04 — class-level, immutable
//
// THEORY ↔ CODE:
//    Account abstract method  →  calculateInterest() ABSTRACT
//    SavingsAccount extends   →  MUST implement (compiler force)
//
// STATIC FINAL DEEP — `INTEREST_RATE`:
//    private static final double INTEREST_RATE = 0.04;
//            ↑       ↑       ↑
//            │       │       constant (cannot change)
//            │       class-level (no instance copy)
//            encapsulation (sirf is class mein)
//
//    Why STATIC?
//       Without: har Savings object mein INTEREST_RATE copy
//                10 objects = 10 copies (waste, value SAME)
//       With:    CLASS-level, ek hi copy, sab share
//                = 1 copy in memory
//
//    Why FINAL?
//       INTEREST_RATE = 0.05;   // compile error
//       Once assigned, never changes = immutability
//
//    Why PRIVATE?
//       Sirf SavingsAccount andar access
//       Bahar se touch nahi = encapsulation
//
//    COMPILE-TIME CONSTANT magic:
//       Compile time pe value INLINE ho jati
//       Bytecode mein 0.04 directly likha
//       Runtime: balance * INTEREST_RATE ≡ balance * 0.04
//       = FASTEST possible (no field lookup)
//
//    Memory layout:
//       ┌────────────────────────────┐
//       │  CLASS LOADER (Method Area) │
//       │  INTEREST_RATE = 0.04        │  ← yahaan (one copy)
//       └────────────────────────────┘
//       ┌────────────────────────────┐
//       │  HEAP                        │
//       │  s1, s2, s3 → instance fields│  ← per object
//       └────────────────────────────┘
//
//    Naming convention: SCREAMING_SNAKE_CASE for constants
//
//    Real examples:
//       Math.PI                = 3.14159
//       Integer.MAX_VALUE      = 2147483647
//       HttpURLConnection.HTTP_OK = 200
//
// PATTERN: Template Method (concrete step implementation)
//
// SOLID — LSP (Liskov Substitution):
//    Account a = new SavingsAccount(...);
//    a.deposit(500);   ← inherited works
//    a.withdraw(200);  ← inherited works
//    a.calculateInterest();  ← Savings's 4% version runs
//    = Account ki place pe perfectly substitute
// ═══════════════════════════════════════════════════════════════════════

public class SavingsAccount extends Account {

    private static final double INTEREST_RATE = 0.04;   // 4% — class-level constant

    public SavingsAccount(String id, String name, double balance) {
        super(id, name, balance);    // parent constructor call
    }

    @Override
    public double calculateInterest() {
        return balance * INTEREST_RATE;   // SavingsAccount ka apna logic
    }
}
