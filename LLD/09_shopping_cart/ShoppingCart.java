// =========================================================
// 🧪 SOLO LLD TEST #2 — Shopping Cart
// Date: 2026-04-20
// Goal: Method implementation solo, no Claude help during coding
// =========================================================

import java.util.*;


// ==========================
// 📋 PROBLEM STATEMENT
// ==========================
// Design a simple ShoppingCart system.
// Users can add items, remove items, get total price, check count,
// and clear the cart.


// ==========================
// 🏗️ CLASS SIGNATURES
// ==========================
//
// class Item
//   - Fields: name (String), price (double)
//   - Constructor: Item(String name, double price)
//   - Provided as-is (complete, no work)
//
// class Cart
//   - Field: List<Item> items
//   - Constructor: Cart()
//
//   Methods to implement:
//   1. boolean addItem(Item item)
//   2. boolean removeItem(String name)
//   3. double getTotalPrice()
//   4. int size()
//   5. void clear()


// ==========================
// 📊 METHOD SPECS
// ==========================
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ addItem(Item item)                                                │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If item == null       → return false                            │
// │ - If item.price <= 0    → return false                            │
// │ - Otherwise             → add to list, return true                │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ removeItem(String name)                                           │
// ├──────────────────────────────────────────────────────────────────┤
// │ - Find FIRST occurrence of item with matching name                │
// │ - If found             → remove it, return true                   │
// │ - If NOT found         → return false                             │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ getTotalPrice()                                                   │
// ├──────────────────────────────────────────────────────────────────┤
// │ - Return sum of price across all items                            │
// │ - Empty cart           → return 0.0                               │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ size()                                                            │
// ├──────────────────────────────────────────────────────────────────┤
// │ - Return current item count in cart                               │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ clear()                                                           │
// ├──────────────────────────────────────────────────────────────────┤
// │ - Remove all items from cart                                      │
// │ - No return (void)                                                │
// └──────────────────────────────────────────────────────────────────┘


// ==========================
// 🎯 EXAMPLES (expected I/O)
// ==========================
//
// Cart cart = new Cart();
// cart.addItem(new Item("Apple", 50))    → true
// cart.addItem(new Item("Bread", 30))    → true
// cart.addItem(new Item("Free", -10))    → false (negative price)
// cart.addItem(null)                     → false
// cart.size()                            → 2
// cart.getTotalPrice()                   → 80.0
// cart.removeItem("Apple")               → true
// cart.removeItem("Apple")               → false (already removed)
// cart.size()                            → 1
// cart.clear()                           → (no return)
// cart.size()                            → 0


// ==========================
// ⚙️ CONSTRAINTS
// ==========================
// - Price is double; must be > 0
// - Name is String; case-sensitive match for removal
// - Multiple items with same name allowed in cart (grocery style)


// =========================================================
// 👇 ITEM CLASS — PROVIDED AS-IS (koi kaam nahi)
// =========================================================

class Item {
    String name;
    double price;

    public Item(String name, double price) {
        this.name = name;
        this.price = price;
    }

    public String getName() {
        return name;
    }

    public double getPrice() {
        return price;
    }
}


// =========================================================
// 👇 CART CLASS — YAHAN APNA CODE LIKH
// =========================================================

class Cart {
    // Field
    List<Item> items;
    double price;

    // Constructor
    public Cart() {
        this.items = new ArrayList<>();
        this.price = 0.0;
    }

    // TODO: addItem — tu likh body
    public boolean addItem(Item item) {
        if(item == null){
            return false;
        }
        if(item.price <= 0){
            return false;
        }
        items.add(item);
        price += item.price;
        return true;
    }

    // TODO: removeItem — tu likh body
    public boolean removeItem(String name) {
        for(Item i : items){
            if(i.name.equals(name)){
                items.remove(i);
                price -= i.price;
                return true;
            }
        }
        return false;
    }

    // TODO: getTotalPrice — tu likh body
    public double getTotalPrice() {
        return price;
    }

    // TODO: size — tu likh body
    public int size() {
        return items.size();
    }

    // TODO: clear — tu likh body (void — return nahi)
    public void clear() {
        items.clear();
        this.price = 0.0;
    }
}


// =========================================================
// 🧪 TEST RUNNER (already ready)
// =========================================================

class Main {
    public static void main(String[] args) {
        Cart cart = new Cart();

        // Test 1: Empty cart
        System.out.println("Test 1 — empty cart size: " + cart.size() + " (expected 0)");
        System.out.println("  total: " + cart.getTotalPrice() + " (expected 0.0)");

        // Test 2: Add valid items
        System.out.println("\nTest 2 — add Apple (50): " + cart.addItem(new Item("Apple", 50)));
        System.out.println("  Expected: true | size: " + cart.size() + " (expected 1)");

        // Test 3: Add another item
        System.out.println("\nTest 3 — add Bread (30): " + cart.addItem(new Item("Bread", 30)));
        System.out.println("  Expected: true | size: " + cart.size() + " (expected 2)");

        // Test 4: Add negative price
        System.out.println("\nTest 4 — add Free (-10): " + cart.addItem(new Item("Free", -10)));
        System.out.println("  Expected: false | size: " + cart.size() + " (expected 2, unchanged)");

        // Test 5: Add zero price
        System.out.println("\nTest 5 — add Zero (0): " + cart.addItem(new Item("Zero", 0)));
        System.out.println("  Expected: false | size: " + cart.size() + " (expected 2)");

        // Test 6: Add null
        System.out.println("\nTest 6 — add null: " + cart.addItem(null));
        System.out.println("  Expected: false | size: " + cart.size() + " (expected 2)");

        // Test 7: Total price
        System.out.println("\nTest 7 — total price: " + cart.getTotalPrice());
        System.out.println("  Expected: 80.0");

        // Test 8: Remove existing
        System.out.println("\nTest 8 — remove Apple: " + cart.removeItem("Apple"));
        System.out.println("  Expected: true | size: " + cart.size() + " (expected 1)");

        // Test 9: Remove already-removed
        System.out.println("\nTest 9 — remove Apple again: " + cart.removeItem("Apple"));
        System.out.println("  Expected: false | size: " + cart.size() + " (expected 1)");

        // Test 10: Remove non-existent
        System.out.println("\nTest 10 — remove Nothing: " + cart.removeItem("Nothing"));
        System.out.println("  Expected: false");

        // Test 11: Multiple items same name
        cart.addItem(new Item("Milk", 25));
        cart.addItem(new Item("Milk", 25));
        System.out.println("\nTest 11 — 2 Milks added | size: " + cart.size() + " (expected 3 = 1 bread + 2 milk)");
        System.out.println("  total: " + cart.getTotalPrice() + " (expected 80.0 = 30 + 25 + 25)");

        // Test 12: Remove first Milk (should only remove ONE)
        System.out.println("\nTest 12 — remove Milk once: " + cart.removeItem("Milk"));
        System.out.println("  Expected: true | size: " + cart.size() + " (expected 2)");

        // Test 13: Clear
        cart.clear();
        System.out.println("\nTest 13 — clear | size: " + cart.size() + " (expected 0)");
        System.out.println("  total: " + cart.getTotalPrice() + " (expected 0.0)");

        System.out.println("\n=== DONE ===");
    }
}
