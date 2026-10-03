import java.util.*;

// ─── 1. LOGGER ───
class LegacyLogger {                                   // purani class — chhoo nahi sakte
    void logMessage(String msg) { System.out.println("   [LEGACY] " + msg); }
}

interface AppLogger {                                  // tera naya code ye shape maangta
    void info(String message);
}

class LoggerAdapter implements AppLogger {             // ADAPTER
    private final LegacyLogger legacy;
    LoggerAdapter(LegacyLogger legacy) { this.legacy = legacy; }
    public void info(String message) { legacy.logMessage(message); }   // naya call -> purana call
}

// ─── 2. PAYMENT (bahar ki library) ───
class ThirdPartyPayApi {                               // vendor ki class — unka shape
    String processPayment(double amount, String currency) {
        return amount > 0 ? "OK-" + currency : "DECLINED";
    }
}

interface PaymentProcessor {                           // tera app sirf ye jaanta
    boolean charge(double amount);
}

class ThirdPartyPayAdapter implements PaymentProcessor {
    private final ThirdPartyPayApi api = new ThirdPartyPayApi();
    public boolean charge(double amount) {
        String res = api.processPayment(amount, "INR");   // unka shape: currency bhi do, String wapas
        return res.startsWith("OK");                       // tera shape: sirf true/false
    }
}

public class DP4_Adapter {
    public static void main(String[] args) {
        System.out.println("1) naya code AppLogger maangta, andar purana LegacyLogger chala:");
        AppLogger log = new LoggerAdapter(new LegacyLogger());
        log.info("user logged in");

        System.out.println("\n2) app sirf PaymentProcessor.charge() jaanta, vendor ka shape chhupa:");
        PaymentProcessor pay = new ThirdPartyPayAdapter();
        System.out.println("   charge(500) -> " + pay.charge(500));
        System.out.println("   charge(0)   -> " + pay.charge(0));
        // kal vendor badla -> naya adapter likho, app ka code wahi rahega

        System.out.println("\n3) Arrays.asList = array ke upar adapter (view), asli ArrayList nahi:");
        String[] arr = {"a", "b", "c"};
        List<String> view = Arrays.asList(arr);
        view.set(0, "z");
        System.out.println("   view.set(0,\"z\") -> array bhi badla: " + Arrays.toString(arr));
        try {
            view.add("d");
        } catch (UnsupportedOperationException e) {
            System.out.println("   view.add(\"d\")   -> UnsupportedOperationException (array ki size fixed)");
        }
        List<String> real = new ArrayList<>(Arrays.asList(arr));
        real.add("d");
        System.out.println("   new ArrayList<>(...) me add chala: " + real);
    }
}
