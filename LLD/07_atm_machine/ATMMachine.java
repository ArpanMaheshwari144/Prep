import java.util.*;

// --- ENUMS ---
enum TransactionType{
    WITHDRAW, DEPOSIT, BALANCE_CHECK;
}


// --- CLASSES ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Card sirf apna data rakhta (cardNumber, pin, balance)
//
// Encapsulation = data private, access controlled via getter/setter
// Yahan kyu: cardNumber, pin, balance sab private — bahar se direct modify nahi
class Card{
    private String cardNumber;
    private int pin;
    private double balance;

    public Card(String cardNumber, int pin, double balance) {
        this.cardNumber = cardNumber;
        this.pin = pin;
        this.balance = balance;
    }

    public String getCardNumber() {
        return cardNumber;
    }

    public int getPin() {
        return pin;
    }

    public double getBalance() {
        return balance;
    }

    public void setBalance(double balance) {
        this.balance = balance;
    }
}

// SRP = har class ka ek hi kaam
// Yahan kyu: User sirf identity info (name + associated card) rakhta
//
// Composition = "has-a" relationship
// Yahan kyu: User HAS-A Card (ownership clear, User gaya to Card bhi context gone)
class User{
    private String name;
    private Card card;

    public User(String name, Card card) {
        this.name = name;
        this.card = card;
    }

    public String getName() {
        return name;
    }

    public Card getCard() {
        return card;
    }
}

// SRP = har class ka ek hi kaam
// Yahan kyu: Transaction sirf ek record hold karta (kya hua, kitna, kab)
class Transaction{
    TransactionType type;
    int amount;
    String time;

    public Transaction(TransactionType type, int amount, String time) {
        this.type = type;
        this.amount = amount;
        this.time = time;
    }

    public TransactionType getType() {
        return type;
    }

    public int getAmount() {
        return amount;
    }

    public String getTime() {
        return time;
    }
}

// ATM: atmId, cashAvailable
// Methods: checkPin(), withdraw(), deposit(), checkBalance()
//
// SRP = har class ka ek hi kaam
// Yahan kyu: ATM sirf transactions handle karta (pin check, withdraw, deposit, balance)
//
// Facade Pattern = complex internals ko simple API ke piche chhupa
// Yahan kyu: Client sirf withdraw(card, amount) call kare, andar balance check + cash deduct + log hidden
//
// 2-gate validation = safety layering (multiple checks)
// Yahan kyu: Withdraw mein PEHLE ATM cash check, PHIR card balance check — dono pass tab hi paisa
class ATM{
    private String atmId;
    private double cashAvailable;
    private List<Transaction> transactions;

    public ATM(String atmId, double cashAvailable) {
        this.atmId = atmId;
        this.cashAvailable = cashAvailable;
        this.transactions = new ArrayList<>();
    }

    public String getAtmId() {
        return atmId;
    }

    public double getCashAvailable() {
        return cashAvailable;
    }

    public List<Transaction> getTransactions() {
        return transactions;
    }

    boolean checkPin(int pin, int enteredPin){
        return pin == enteredPin;
    }

    void withdraw(Card card, int amount){
        if(amount > cashAvailable){
            return;
        }
        else if(card.getBalance() < amount){
            return;
        }
        else if(cashAvailable >= amount && card.getBalance() >= amount){
            card.setBalance(card.getBalance() - amount);
            cashAvailable = cashAvailable - amount;
            transactions.add(new Transaction(TransactionType.WITHDRAW, amount, "now"));

        }
        return;
    }

    void deposit(Card card, int amount){
        card.setBalance(card.getBalance() + amount);
        cashAvailable = cashAvailable + amount;
        transactions.add(new Transaction(TransactionType.DEPOSIT, amount, "now"));
        return;
    }

    double checkBalance(Card card){
        return card.getBalance();
    }

}


// --- MAIN ---
class Main {
    public static void main(String[] args) {
        Card card = new Card("1234-5678-9012", 1234, 10000);
        User user = new User("Arpan", card);
        ATM atm = new ATM("ATM-001", 50000);

        // PIN check — sahi
        System.out.println("PIN correct? " + atm.checkPin(card.getPin(), 1234));

        // PIN check — galat
        System.out.println("PIN correct? " + atm.checkPin(card.getPin(), 9999));

        // Balance check
        System.out.println("Balance: " + atm.checkBalance(card));

        // Withdraw 3000 — success
        atm.withdraw(card, 3000);
        System.out.println("After withdraw 3000 → Balance: " + card.getBalance() + ", ATM cash: " + atm.getCashAvailable());

        // Withdraw 20000 — fail (balance kam)
        atm.withdraw(card, 20000);
        System.out.println("After withdraw 20000 → Balance: " + card.getBalance() + " (should be same — failed)");

        // Deposit 5000
        atm.deposit(card, 5000);
        System.out.println("After deposit 5000 → Balance: " + card.getBalance() + ", ATM cash: " + atm.getCashAvailable());

        // Final balance
        System.out.println("Final balance: " + atm.checkBalance(card));

        // Transactions
        System.out.println("Total transactions: " + atm.getTransactions().size());
        for (Transaction t : atm.getTransactions()) {
            System.out.println("  " + t.getType() + " → " + t.getAmount());
        }

        System.out.println("ATM Machine Done!");
    }
}
