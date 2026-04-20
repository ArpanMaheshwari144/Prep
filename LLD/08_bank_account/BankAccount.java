// =========================================================
// 🧪 SOLO LLD TEST #1 — Bank Account System
// Date: 2026-04-20
// Goal: Method implementation solo, no Claude help during coding
// =========================================================

// ==========================
// 📋 PROBLEM STATEMENT
// ==========================
// Design a simple BankAccount class.
// Create methods to deposit, withdraw, check balance, and transfer money
// between two accounts.

// ==========================
// 🏗️ CLASS SIGNATURE
// ==========================
// class BankAccount
//   - Fields: owner (String), balance (double)
//   - Constructor: BankAccount(String owner, double initialBalance)
//
// Methods to implement:
//   1. boolean deposit(double amount)
//   2. boolean withdraw(double amount)
//   3. double getBalance()
//   4. boolean transfer(BankAccount other, double amount)

// ==========================
// 📊 METHOD SPECS
// ==========================
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ deposit(double amount)                                            │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If amount <= 0         → return false (invalid amount)          │
// │ - Otherwise              → balance += amount, return true         │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ withdraw(double amount)                                           │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If amount <= 0         → return false (invalid amount)          │
// │ - If amount > balance    → return false (insufficient funds)      │
// │ - Otherwise              → balance -= amount, return true         │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ getBalance()                                                      │
// ├──────────────────────────────────────────────────────────────────┤
// │ - Return current balance                                          │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ transfer(BankAccount other, double amount)                        │
// ├──────────────────────────────────────────────────────────────────┤
// │ - Withdraw from THIS account                                      │
// │ - If withdraw fails     → return false (no change to either)     │
// │ - If withdraw succeeds  → deposit to OTHER account, return true   │
// └──────────────────────────────────────────────────────────────────┘

// ==========================
// 🎯 EXAMPLES (expected I/O)
// ==========================
//
// BankAccount arpan = new BankAccount("Arpan", 1000);
// arpan.deposit(500)      → true  (balance: 1500)
// arpan.deposit(-100)     → false (balance: 1500, unchanged)
// arpan.withdraw(200)     → true  (balance: 1300)
// arpan.withdraw(5000)    → false (balance: 1300, insufficient)
// arpan.withdraw(-50)     → false (balance: 1300, invalid)
// arpan.getBalance()      → 1300.0
//
// BankAccount claude = new BankAccount("Claude", 500);
// arpan.transfer(claude, 300)    → true  (arpan: 1000, claude: 800)
// arpan.transfer(claude, 5000)   → false (no change — arpan: 1000, claude: 800)

// ==========================
// ⚙️ CONSTRAINTS
// ==========================
// - amount and balance are double
// - Owner name is String
// - Negative or zero amounts always rejected

// =========================================================
// 👇 YAHAN APNA CODE LIKH
// =========================================================

class BankAccount {
    // TODO: fields
    String owner;
    double balance;

    public BankAccount(String owner, double balance) {
        this.owner = owner;
        this.balance = balance;
    }

    public String getOwner() {
        return owner;
    }

    public void setOwner(String owner) {
        this.owner = owner;
    }

    public double getBalance() {
        return balance;
    }

    public void setBalance(double balance) {
        this.balance = balance;
    }

    // TODO: deposit
    public boolean deposit(double amount) {
        if (amount <= 0) {
            return false;
        }
        balance += amount;
        return true;

    }

    // TODO: withdraw
    public boolean withdraw(double amount) {
        if (amount <= 0) {
            return false;
        }
        if (amount > balance) {
            return false;
        }
        balance -= amount;
        return true;

    }

    // TODO: getBalance

    // TODO: transfer
    public boolean transfer(BankAccount other, double amount) {
        return this.withdraw(amount) ? other.deposit(amount) : false;
    }
}

// =========================================================
// 🧪 TEST RUNNER (already ready — tu bas classes fill kar)
// =========================================================

class Main {
    public static void main(String[] args) {
        // Test 1: Create account
        BankAccount arpan = new BankAccount("Arpan", 1000);
        System.out.println("Test 1 — initial balance: " + arpan.getBalance());
        System.out.println("  Expected: 1000.0");

        // Test 2: Deposit positive
        System.out.println("\nTest 2 — deposit 500: " + arpan.deposit(500));
        System.out.println("  Expected: true | balance: " + arpan.getBalance() + " (expected 1500.0)");

        // Test 3: Deposit negative
        System.out.println("\nTest 3 — deposit -100: " + arpan.deposit(-100));
        System.out.println("  Expected: false | balance: " + arpan.getBalance() + " (expected 1500.0)");

        // Test 4: Deposit zero
        System.out.println("\nTest 4 — deposit 0: " + arpan.deposit(0));
        System.out.println("  Expected: false | balance: " + arpan.getBalance() + " (expected 1500.0)");

        // Test 5: Withdraw valid
        System.out.println("\nTest 5 — withdraw 200: " + arpan.withdraw(200));
        System.out.println("  Expected: true | balance: " + arpan.getBalance() + " (expected 1300.0)");

        // Test 6: Withdraw insufficient
        System.out.println("\nTest 6 — withdraw 5000: " + arpan.withdraw(5000));
        System.out.println("  Expected: false | balance: " + arpan.getBalance() + " (expected 1300.0)");

        // Test 7: Withdraw negative
        System.out.println("\nTest 7 — withdraw -50: " + arpan.withdraw(-50));
        System.out.println("  Expected: false | balance: " + arpan.getBalance() + " (expected 1300.0)");

        // Test 8: Transfer success
        BankAccount claude = new BankAccount("Claude", 500);
        System.out.println("\nTest 8 — transfer 300 to claude: " + arpan.transfer(claude, 300));
        System.out.println("  Expected: true");
        System.out.println("  arpan balance: " + arpan.getBalance() + " (expected 1000.0)");
        System.out.println("  claude balance: " + claude.getBalance() + " (expected 800.0)");

        // Test 9: Transfer insufficient (atomic — no change)
        System.out.println("\nTest 9 — transfer 5000 (insufficient): " + arpan.transfer(claude, 5000));
        System.out.println("  Expected: false");
        System.out.println("  arpan balance: " + arpan.getBalance() + " (expected 1000.0)");
        System.out.println("  claude balance: " + claude.getBalance() + " (expected 800.0)");

        // Test 10: Transfer to self (edge case — should still work)
        System.out.println("\nTest 10 — transfer 100 to self: " + arpan.transfer(arpan, 100));
        System.out.println(
                "  arpan balance: " + arpan.getBalance() + " (expected 1000.0 — withdraw + deposit cancel out)");

        System.out.println("\n=== DONE ===");
    }
}
