// =========================================================
// 🧪 SOLO LLD TEST #4 — Library Management System
// Date: 2026-04-20
// Mock interview — discussion-based
// =========================================================

import java.util.*;

// ==========================
// 📋 SCOPE (from discussion)
// ==========================
// - Max 5 copies per book
// - Max 3 books per member at once
// - Due date: 15 days from borrow
// - Late return fine: ₹10 per day late

// ==========================
// 🏗️ CLASS DESIGN (from discussion)
// ==========================
//
// class Book
//   - bookId (String), title, author, totalCopies, availableCount
//   - Provided as-is
//
// class Member
//   - memberId (String), name, borrowedBookIds (List<String>)
//   - Provided as-is
//
// class Library
//   - books: Map<String, Book>
//   - members: Map<String, Member>
//
//   Methods to implement:
//   1. boolean addBook(Book book)
//   2. boolean addMember(Member member)
//   3. boolean borrowBook(String memberId, String bookId)
//   4. int returnBook(String memberId, String bookId, int daysLate)

// ==========================
// 📊 METHOD SPECS
// ==========================
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ addBook(Book book)                                               │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If book is null            → return false                      │
// │ - If bookId already exists   → return false (duplicate)          │
// │ - Otherwise                  → add to map, return true           │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ addMember(Member member)                                         │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If member is null          → return false                      │
// │ - If memberId already exists → return false                      │
// │ - Otherwise                  → add to map, return true           │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ borrowBook(String memberId, String bookId)                       │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If member doesn't exist           → return false                │
// │ - If book doesn't exist             → return false                │
// │ - If book availableCount == 0       → return false (all out)      │
// │ - If member already has 3 books     → return false (max limit)    │
// │ - Otherwise:                                                       │
// │   - book.availableCount--                                          │
// │   - member.borrowedBookIds.add(bookId)                             │
// │   - return true                                                    │
// └──────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────┐
// │ returnBook(String memberId, String bookId, int daysLate)          │
// ├──────────────────────────────────────────────────────────────────┤
// │ - If member doesn't exist                       → return -1       │
// │ - If book doesn't exist                         → return -1       │
// │ - If member didn't borrow this book             → return -1       │
// │ - Otherwise:                                                       │
// │   - book.availableCount++                                          │
// │   - member.borrowedBookIds.remove(bookId)                          │
// │   - fine = daysLate > 0 ? daysLate * 10 : 0                       │
// │   - return fine (amount in rupees)                                │
// └──────────────────────────────────────────────────────────────────┘

// ==========================
// 🎯 EXAMPLES
// ==========================
//
// Library lib = new Library();
// lib.addBook(new Book("B1", "Harry Potter", "JK", 5))       → true
// lib.addBook(new Book("B1", "Duplicate", "X", 3))           → false (dup id)
// lib.addMember(new Member("M1", "Arpan"))                   → true
// lib.borrowBook("M1", "B1")                                 → true (availableCount: 4)
// lib.borrowBook("M99", "B1")                                → false (member not found)
// lib.borrowBook("M1", "B99")                                → false (book not found)
// lib.returnBook("M1", "B1", 0)                              → 0 (on time, no fine)
// lib.returnBook("M1", "B1", 3)                              → 30 (3 days × ₹10)
// lib.returnBook("M1", "B1", -5)                             → 0 (early return, no fine)

// =========================================================
// 👇 BOOK CLASS — PROVIDED AS-IS
// =========================================================

class Book {
    String bookId;
    String title;
    String author;
    int totalCopies;
    int availableCount;

    public Book(String bookId, String title, String author, int totalCopies) {
        this.bookId = bookId;
        this.title = title;
        this.author = author;
        this.totalCopies = totalCopies;
        this.availableCount = totalCopies; // starts full
    }

    public String getBookId() {
        return bookId;
    }

    public String getTitle() {
        return title;
    }

    public int getAvailableCount() {
        return availableCount;
    }
}

// =========================================================
// 👇 MEMBER CLASS — PROVIDED AS-IS
// =========================================================

class Member {
    String memberId;
    String name;
    List<String> borrowedBookIds;

    public Member(String memberId, String name) {
        this.memberId = memberId;
        this.name = name;
        this.borrowedBookIds = new ArrayList<>();
    }

    public String getMemberId() {
        return memberId;
    }

    public String getName() {
        return name;
    }

    public List<String> getBorrowedBookIds() {
        return borrowedBookIds;
    }
}

// =========================================================
// 👇 LIBRARY CLASS — YAHAN APNA CODE LIKH
// =========================================================

class Library {
    // Fields
    Map<String, Book> books;
    Map<String, Member> members;

    // Constructor
    public Library() {
        this.books = new HashMap<>();
        this.members = new HashMap<>();
    }

    // TODO: addBook
    public boolean addBook(Book book) {
        if (book == null) {
            return false;
        }
        if (books.containsKey(book.getBookId())) {
            return false;
        }
        books.putIfAbsent(book.getBookId(), book);
        return true;

    }

    // TODO: addMember
    public boolean addMember(Member member) {
        if(member == null){
            return false;
        }
        if(members.containsKey(member.getMemberId())){
            return false;
        }
        members.putIfAbsent(member.getMemberId(), member);
        return true;
    }

    // TODO: borrowBook
    public boolean borrowBook(String memberId, String bookId) {
        if (!members.containsKey(memberId)) {
            return false;
        } else if (!books.containsKey(bookId)) {
            return false;
        } else if (books.get(bookId).availableCount == 0) {
            return false;
        } else if (members.get(memberId).getBorrowedBookIds().size() == 3) {
            return false;
        }
        books.get(bookId).availableCount--;
        members.get(memberId).borrowedBookIds.add(bookId);
        return true;
    }

    // TODO: returnBook (returns fine amount or -1 on error)
    public int returnBook(String memberId, String bookId, int daysLate) {
        if (!members.containsKey(memberId)) {
            return -1;
        } else if (!books.containsKey(bookId)) {
            return -1;
        } else if (!members.get(memberId).getBorrowedBookIds().contains(bookId)) {
            return -1;
        }
        books.get(bookId).availableCount++;
        members.get(memberId).borrowedBookIds.remove(bookId);
        int fine = daysLate > 0 ? daysLate * 10 : 0;
        return fine;
    }
}

// =========================================================
// 🧪 TEST RUNNER
// =========================================================

class Main {
    public static void main(String[] args) {
        Library lib = new Library();

        // === SETUP ===
        System.out.println("=== SETUP ===");
        System.out.println("Add Harry Potter (5 copies): " + lib.addBook(new Book("B1", "Harry Potter", "JK", 5)));
        System.out.println("Add LOTR (3 copies): " + lib.addBook(new Book("B2", "LOTR", "Tolkien", 3)));
        System.out.println("Add duplicate B1: " + lib.addBook(new Book("B1", "Dup", "X", 2)) + " (expected false)");
        System.out.println("Add null book: " + lib.addBook(null) + " (expected false)");

        System.out.println("\nAdd member Arpan: " + lib.addMember(new Member("M1", "Arpan")));
        System.out.println("Add member Claude: " + lib.addMember(new Member("M2", "Claude")));
        System.out.println("Add duplicate M1: " + lib.addMember(new Member("M1", "Dup")) + " (expected false)");
        System.out.println("Add null member: " + lib.addMember(null) + " (expected false)");

        // === BORROW ===
        System.out.println("\n=== BORROW ===");
        System.out.println("Arpan borrows B1: " + lib.borrowBook("M1", "B1") + " (expected true)");
        System.out.println("  B1 avail: " + lib.books.get("B1").getAvailableCount() + " (expected 4)");

        System.out.println("\nArpan borrows B2: " + lib.borrowBook("M1", "B2") + " (expected true)");
        System.out.println("Arpan borrows B1 again: " + lib.borrowBook("M1", "B1") + " (expected true, 2nd copy)");
        System.out.println("  Arpan has " + lib.members.get("M1").getBorrowedBookIds().size() + " books (expected 3)");

        System.out.println("\nArpan tries 4th borrow: " + lib.borrowBook("M1", "B2") + " (expected false, max 3)");
        System.out.println("Invalid member: " + lib.borrowBook("M99", "B1") + " (expected false)");
        System.out.println("Invalid book: " + lib.borrowBook("M1", "B99") + " (expected false)");

        // === RETURN ===
        System.out.println("\n=== RETURN ===");
        System.out.println("Return B1 on time (daysLate=0): " + lib.returnBook("M1", "B1", 0) + " (expected 0)");
        System.out.println("  B1 avail: " + lib.books.get("B1").getAvailableCount() + " (expected 4)");

        System.out.println("\nReturn B2 late 3 days: " + lib.returnBook("M1", "B2", 3) + " (expected 30)");
        System.out.println(
                "Return B1 early (-5): " + lib.returnBook("M1", "B1", -5) + " (expected 0, no fine for early)");

        System.out.println("\nReturn book Arpan didn't borrow: " + lib.returnBook("M1", "B2", 0)
                + " (expected -1, already returned)");
        System.out.println("Invalid member return: " + lib.returnBook("M99", "B1", 0) + " (expected -1)");
        System.out.println("Invalid book return: " + lib.returnBook("M1", "B99", 0) + " (expected -1)");

        // === BORROW AGAIN AFTER RETURN ===
        System.out.println("\n=== BORROW AFTER RETURN ===");
        System.out.println("Claude borrows B1: " + lib.borrowBook("M2", "B1") + " (expected true)");

        System.out.println("\n=== DONE ===");
    }
}
