// =========================================================
// 🧪 SOLO LLD TEST #5 — Vending Machine
// Date: 2026-04-20
// Mock interview — discussion-based
// =========================================================

import java.util.*;

// ==========================
// 📋 SCOPE (from discussion)
// ==========================
// - Products have id, name, price, quantity
// - Users have id, name, insertedAmount (paisa jo abhi dala hai)
// - Accepted notes: ONLY 10, 20, 50, 100 (baaki reject)
// - User ko pehle paisa dalna hoga, phir product select
// - Stock 0 → product nahi milega
// - Paisa kam → product nahi milega
// - Cancel → poora dala hua paisa wapas

// ==========================
// 🏗️ CLASS DESIGN
// ==========================
//
// class Product
//   - productId (String), name, price, quantity
//   - Provided as-is
//
// class User
//   - userId (String), name, insertedAmount (int, rupees)
//   - Provided as-is
//
// class VendingMachine
//   - products: Map<String, Product>
//   - users: Map<String, User>
//
//   Methods to implement:
//   1. boolean addUser(User user)
//   2. boolean addProduct(Product product)
//   3. boolean refillProduct(String productId, int qty)
//   4. boolean insertMoney(String userId, int noteValue)
//   5. int selectProduct(String userId, String productId)   // change or -1
//   6. int cancel(String userId)                            // refund or -1

// ==========================
// 📊 METHOD SPECS
// ==========================
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ addUser(User user)                                               │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If user == null          → return false                        │
// │ - If userId already exists → return false                        │
// │ - Otherwise                → add to map, return true             │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ addProduct(Product product)                                      │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If product == null         → return false                      │
// │ - If productId already exists → return false                     │
// │ - Otherwise                  → add to map, return true           │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ refillProduct(String productId, int qty)                         │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If productId doesn't exist → return false                      │
// │ - If qty <= 0                → return false                      │
// │ - Otherwise                  → product.quantity += qty, true     │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ insertMoney(String userId, int noteValue)                        │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If userId doesn't exist                → return false          │
// │ - If noteValue not in {10, 20, 50, 100}  → return false          │
// │ - Otherwise → user.insertedAmount += noteValue, return true      │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ selectProduct(String userId, String productId)                   │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If userId doesn't exist                → return -1             │
// │ - If productId doesn't exist             → return -1             │
// │ - If product.quantity == 0               → return -1             │
// │ - If user.insertedAmount < product.price → return -1             │
// │ - Otherwise:                                                      │
// │   - product.quantity--                                            │
// │   - change = user.insertedAmount - product.price                  │
// │   - user.insertedAmount = 0                                       │
// │   - return change                                                 │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ cancel(String userId)                                            │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If userId doesn't exist       → return -1                      │
// │ - If user.insertedAmount == 0   → return -1                      │
// │ - Otherwise:                                                      │
// │   - refund = user.insertedAmount                                  │
// │   - user.insertedAmount = 0                                       │
// │   - return refund                                                 │
// └──────────────────────────────────────────────────────────────────┘

// ==========================
// 🎯 EXAMPLES
// ==========================
//
// VendingMachine vm = new VendingMachine();
// vm.addProduct(new Product("P1", "Chips", 30, 3))     → true
// vm.addProduct(new Product("P1", "Dup", 10, 1))       → false (dup id)
// vm.addUser(new User("U1", "Arpan"))                  → true
//
// vm.insertMoney("U1", 5)                              → false (invalid note)
// vm.insertMoney("U1", 20)                             → true  (balance 20)
// vm.selectProduct("U1", "P1")                         → -1 (20 < 30, insufficient)
// vm.insertMoney("U1", 20)                             → true  (balance 40)
// vm.selectProduct("U1", "P1")                         → 10 (40-30 change, stock 2)
// vm.cancel("U1")                                      → -1 (balance 0, nothing to refund)
//
// vm.insertMoney("U1", 50)                             → true  (balance 50)
// vm.cancel("U1")                                      → 50 (refunded)
//
// vm.refillProduct("P1", 5)                            → true (stock 2+5=7)

// =========================================================
// 👇 PRODUCT CLASS — PROVIDED AS-IS
// =========================================================

class Product {
    String productId;
    String name;
    int price;
    int quantity;

    public Product(String productId, String name, int price, int quantity) {
        this.productId = productId;
        this.name = name;
        this.price = price;
        this.quantity = quantity;
    }

    public String getProductId() {
        return productId;
    }

    public String getName() {
        return name;
    }

    public int getPrice() {
        return price;
    }

    public int getQuantity() {
        return quantity;
    }
}

// =========================================================
// 👇 USER CLASS — PROVIDED AS-IS
// =========================================================

class User {
    String userId;
    String name;
    int insertedAmount;

    public User(String userId, String name) {
        this.userId = userId;
        this.name = name;
        this.insertedAmount = 0;
    }

    public String getUserId() {
        return userId;
    }

    public String getName() {
        return name;
    }

    public int getInsertedAmount() {
        return insertedAmount;
    }
}

// =========================================================
// 👇 VENDING MACHINE CLASS — YAHAN APNA CODE LIKH
// =========================================================

class VendingMachine {
    // Fields
    Map<String, Product> products;
    Map<String, User> users;

    // Constructor
    public VendingMachine() {
        this.products = new HashMap<>();
        this.users = new HashMap<>();
    }

    // TODO: addUser
    public boolean addUser(User user) {
        // │ addUser(User user) │
        // ├──────────────────────────────────────────────────────────────────┤
        // │ - If user == null → return false │
        // │ - If userId already exists → return false │
        // │ - Otherwise → add to map, return true

        if (user == null) {
            return false;
        } else if (users.containsKey(user.getUserId())) {
            return false;
        }
        users.putIfAbsent(user.getUserId(), user);
        return true;
    }

    // TODO: addProduct
    public boolean addProduct(Product product) {
        // │ addProduct(Product product) │
        // ├──────────────────────────────────────────────────────────────────┤
        // │ - If product == null → return false │
        // │ - If productId already exists → return false │
        // │ - Otherwise → add to map, return true

        if (product == null) {
            return false;
        } else if (products.containsKey(product.getProductId())) {
            return false;
        }
        products.putIfAbsent(product.getProductId(), product);
        return true;
    }

    // TODO: refillProduct
    public boolean refillProduct(String productId, int qty) {
        // │ refillProduct(String productId, int qty) │
        // ├──────────────────────────────────────────────────────────────────┤
        // │ - If productId doesn't exist → return false │
        // │ - If qty <= 0 → return false │
        // │ - Otherwise → product.quantity += qty, true

        if (productId == null) {
            return false;
        } else if (qty <= 0) {
            return false;
        } else if (!products.containsKey(productId)) {
            return false;
        }
        products.get(productId).quantity += qty;
        return true;

    }

    // TODO: insertMoney
    public boolean insertMoney(String userId, int noteValue) {
        // │ insertMoney(String userId, int noteValue) │
        // ├──────────────────────────────────────────────────────────────────┤
        // │ - If userId doesn't exist → return false │
        // │ - If noteValue not in {10, 20, 50, 100} → return false │
        // │ - Otherwise → user.insertedAmount += noteValue, return true

        if (userId == null) {
            return false;
        } else if (!Set.of(10, 20, 50, 100).contains(noteValue)) {
            return false;
        } else if (!users.containsKey(userId)) {
            return false;
        }
        users.get(userId).insertedAmount += noteValue;
        return true;
    }

    // TODO: selectProduct (returns change amount or -1)
    public int selectProduct(String userId, String productId) {
        // │ selectProduct(String userId, String productId) │
        // ├──────────────────────────────────────────────────────────────────┤
        // │ - If userId doesn't exist → return -1 │
        // │ - If productId doesn't exist → return -1 │
        // │ - If product.quantity == 0 → return -1 │
        // │ - If user.insertedAmount < product.price → return -1 │
        // │ - Otherwise: │
        // │ - product.quantity-- │
        // │ - change = user.insertedAmount - product.price │
        // │ - user.insertedAmount = 0 │
        // │ - return change

        if (userId == null) {
            return -1;
        } else if (productId == null) {
            return -1;
        } else if (!users.containsKey(userId)) {
            return -1;
        } else if (!products.containsKey(productId)) {
            return -1;
        } else if (products.get(productId).quantity == 0) {
            return -1;
        } else if (users.get(userId).insertedAmount < products.get(productId).price) {
            return -1;
        }
        products.get(productId).quantity--;
        int change = users.get(userId).insertedAmount - products.get(productId).price;
        users.get(userId).insertedAmount = 0;
        return change;
    }

    // TODO: cancel (returns refund amount or -1)
    public int cancel(String userId) {
        // │ cancel(String userId) │
        // ├──────────────────────────────────────────────────────────────────┤
        // │ - If userId doesn't exist → return -1 │
        // │ - If user.insertedAmount == 0 → return -1 │
        // │ - Otherwise: │
        // │ - refund = user.insertedAmount │
        // │ - user.insertedAmount = 0 │
        // │ - return refund

        if (userId == null) {
            return -1;
        } else if (!users.containsKey(userId)) {
            return -1;
        } else if (users.get(userId).insertedAmount == 0) {
            return -1;
        }
        int refund = users.get(userId).insertedAmount;
        users.get(userId).insertedAmount = 0;
        return refund;
    }
}

// =========================================================
// 🧪 TEST RUNNER
// =========================================================

class Main {
    public static void main(String[] args) {
        VendingMachine vm = new VendingMachine();

        // === SETUP — ADD PRODUCTS ===
        System.out.println("=== SETUP PRODUCTS ===");
        System.out.println("Add Chips (P1, 30, qty 3): " + vm.addProduct(new Product("P1", "Chips", 30, 3)));
        System.out.println("Add Cola (P2, 50, qty 2): " + vm.addProduct(new Product("P2", "Cola", 50, 2)));
        System.out.println("Add duplicate P1: " + vm.addProduct(new Product("P1", "Dup", 10, 1)) + " (expected false)");
        System.out.println("Add null product: " + vm.addProduct(null) + " (expected false)");

        // === SETUP — ADD USERS ===
        System.out.println("\n=== SETUP USERS ===");
        System.out.println("Add Arpan (U1): " + vm.addUser(new User("U1", "Arpan")));
        System.out.println("Add Claude (U2): " + vm.addUser(new User("U2", "Claude")));
        System.out.println("Add duplicate U1: " + vm.addUser(new User("U1", "Dup")) + " (expected false)");
        System.out.println("Add null user: " + vm.addUser(null) + " (expected false)");

        // === INSERT MONEY — VALIDATION ===
        System.out.println("\n=== INSERT MONEY ===");
        System.out.println("Arpan inserts 5 (invalid): " + vm.insertMoney("U1", 5) + " (expected false)");
        System.out.println("Arpan inserts 25 (invalid): " + vm.insertMoney("U1", 25) + " (expected false)");
        System.out.println("Invalid user inserts 10: " + vm.insertMoney("U99", 10) + " (expected false)");

        System.out.println("\nArpan inserts 20: " + vm.insertMoney("U1", 20) + " (expected true)");
        System.out.println("  Arpan balance: " + vm.users.get("U1").getInsertedAmount() + " (expected 20)");

        // === SELECT PRODUCT — INSUFFICIENT ===
        System.out.println("\n=== SELECT PRODUCT ===");
        System.out
                .println("Arpan selects Chips (need 30, have 20): " + vm.selectProduct("U1", "P1") + " (expected -1)");
        System.out.println("  Arpan balance unchanged: " + vm.users.get("U1").getInsertedAmount() + " (expected 20)");
        System.out.println("  Chips stock unchanged: " + vm.products.get("P1").getQuantity() + " (expected 3)");

        // === SELECT — SUCCESS ===
        System.out.println("\nArpan inserts 20 more: " + vm.insertMoney("U1", 20) + " (expected true, balance 40)");
        System.out.println("Arpan selects Chips (40-30): " + vm.selectProduct("U1", "P1") + " (expected 10 change)");
        System.out.println("  Arpan balance reset: " + vm.users.get("U1").getInsertedAmount() + " (expected 0)");
        System.out.println("  Chips stock: " + vm.products.get("P1").getQuantity() + " (expected 2)");

        // === CANCEL — NO MONEY ===
        System.out.println("\n=== CANCEL ===");
        System.out.println("Arpan cancels (no money): " + vm.cancel("U1") + " (expected -1)");

        // === CANCEL — WITH MONEY ===
        System.out.println("\nArpan inserts 50: " + vm.insertMoney("U1", 50) + " (expected true)");
        System.out.println("Arpan cancels: " + vm.cancel("U1") + " (expected 50)");
        System.out.println("  Arpan balance after cancel: " + vm.users.get("U1").getInsertedAmount() + " (expected 0)");

        // === STOCK DEPLETION ===
        System.out.println("\n=== STOCK DEPLETION ===");
        System.out.println("Claude inserts 100: " + vm.insertMoney("U2", 100) + " (expected true)");
        System.out.println("Claude buys Cola: " + vm.selectProduct("U2", "P2") + " (expected 50 change, stock 1)");
        System.out.println("Claude inserts 50: " + vm.insertMoney("U2", 50));
        System.out.println("Claude buys Cola: " + vm.selectProduct("U2", "P2") + " (expected 0 change, stock 0)");
        System.out.println("Claude inserts 50: " + vm.insertMoney("U2", 50));
        System.out.println("Claude buys Cola (stock 0): " + vm.selectProduct("U2", "P2") + " (expected -1)");

        // === REFILL ===
        System.out.println("\n=== REFILL ===");
        System.out.println("Refill Cola with 5: " + vm.refillProduct("P2", 5) + " (expected true)");
        System.out.println("  Cola stock: " + vm.products.get("P2").getQuantity() + " (expected 5)");
        System.out.println("Refill invalid product: " + vm.refillProduct("P99", 5) + " (expected false)");
        System.out.println("Refill with 0: " + vm.refillProduct("P2", 0) + " (expected false)");
        System.out.println("Refill with -3: " + vm.refillProduct("P2", -3) + " (expected false)");

        // === INVALID IDS ===
        System.out.println("\n=== INVALID IDS ===");
        System.out.println("Invalid user select: " + vm.selectProduct("U99", "P1") + " (expected -1)");
        System.out.println("Invalid product select: " + vm.selectProduct("U1", "P99") + " (expected -1)");
        System.out.println("Invalid user cancel: " + vm.cancel("U99") + " (expected -1)");

        System.out.println("\n=== DONE ===");
    }
}
