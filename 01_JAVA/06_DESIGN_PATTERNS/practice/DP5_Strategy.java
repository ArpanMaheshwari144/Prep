import java.util.*;

// ─── 1. PAYMENT: if-else ki jagah alag class ───
interface PaymentStrategy {                            // common shape
    String pay(double amount);
}

class CardPayment implements PaymentStrategy {
    public String pay(double amount) { return "CARD  " + amount + " (2% fee = " + amount * 0.02 + ")"; }
}

class UpiPayment implements PaymentStrategy {
    public String pay(double amount) { return "UPI   " + amount + " (fee 0)"; }
}

class PaymentService {                                 // CONTEXT — kaunsa algo hai, isko farak nahi
    private PaymentStrategy strategy;
    PaymentService(PaymentStrategy strategy) { this.strategy = strategy; }
    void setStrategy(PaymentStrategy s) { this.strategy = s; }
    void checkout(double amount) { System.out.println("   " + strategy.pay(amount)); }
}

// ─── 2. FACTORY + STRATEGY saath (asli code me aise hi milte) ───
class PaymentFactory {
    private static final Map<String, PaymentStrategy> MAP = Map.of(
            "CARD", new CardPayment(),
            "UPI", new UpiPayment());
    static PaymentStrategy get(String type) {
        PaymentStrategy s = MAP.get(type);
        if (s == null) throw new IllegalArgumentException("unknown type: " + type);
        return s;
    }
}

record User(String name, int age) {}

public class DP5_Strategy {
    public static void main(String[] args) {
        System.out.println("1) same checkout(), algo runtime pe badla:");
        PaymentService service = new PaymentService(new UpiPayment());
        service.checkout(1000);
        service.setStrategy(new CardPayment());       // SWITCH — PaymentService ka code nahi chhua
        service.checkout(1000);

        System.out.println("\n2) naya method = nayi class nahi, lambda bhi chalega (interface me ek method):");
        service.setStrategy(amount -> "WALLET " + amount + " (cashback 10)");
        service.checkout(1000);

        System.out.println("\n3) factory string se strategy de, if-else kahin nahi:");
        for (String type : List.of("UPI", "CARD")) {
            new PaymentService(PaymentFactory.get(type)).checkout(500);
        }
        try {
            PaymentFactory.get("BITCOIN");
        } catch (IllegalArgumentException e) {
            System.out.println("   BITCOIN -> " + e.getMessage());
        }

        System.out.println("\n4) Comparator = JDK ka strategy, sort() wahi, tareeka badla:");
        List<User> users = new ArrayList<>(List.of(new User("Ravi", 30), new User("Amit", 25), new User("Zoya", 28)));
        users.sort(Comparator.comparing(User::name));
        System.out.println("   naam se: " + users);
        users.sort(Comparator.comparingInt(User::age));
        System.out.println("   umar se: " + users);
    }
}
