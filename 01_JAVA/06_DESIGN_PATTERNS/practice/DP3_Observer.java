import java.util.*;
import java.util.concurrent.*;

// ─── 1. EVENT: kya hua ───
class TransferEvent {
    final String from, to;
    final double amount;
    TransferEvent(String from, String to, double amount) { this.from = from; this.to = to; this.amount = amount; }
}

// ─── 2. LISTENER: sunne ka contract ───
interface TransferListener {
    void onEvent(TransferEvent e);
}

// ─── 3. PUBLISHER: list rakhta, sabko batata ───
class TransferPublisher {
    private final List<TransferListener> listeners;

    TransferPublisher(List<TransferListener> listStore) { this.listeners = listStore; }

    void subscribe(TransferListener l)   { listeners.add(l); }
    void unsubscribe(TransferListener l) { listeners.remove(l); }

    void publish(TransferEvent e) {
        for (TransferListener l : listeners) {
            try {
                l.onEvent(e);                                  // har ek ko batao
            } catch (RuntimeException ex) {
                System.out.println("   [publisher] ek listener fail hua (" + ex.getMessage() + "), baaki chalte rahenge");
            }
        }
    }
}

// ─── concrete listeners ───
class EmailListener implements TransferListener {
    public void onEvent(TransferEvent e) { System.out.println("   Email : Rs " + e.amount + " " + e.from + " -> " + e.to); }
}
class SmsListener implements TransferListener {
    public void onEvent(TransferEvent e) { System.out.println("   SMS   : Rs " + e.amount + " transfer hua"); }
}
class AuditListener implements TransferListener {
    public void onEvent(TransferEvent e) { System.out.println("   Audit : log likha"); }
}
class BrokenListener implements TransferListener {
    public void onEvent(TransferEvent e) { throw new RuntimeException("SMS gateway down"); }
}

public class DP3_Observer {
    public static void main(String[] args) {
        TransferEvent ev = new TransferEvent("A1", "A2", 1000);

        System.out.println("1) Email + SMS subscribe, phir publish:");
        TransferPublisher p = new TransferPublisher(new ArrayList<>());
        p.subscribe(new EmailListener());
        p.subscribe(new SmsListener());
        p.publish(ev);

        System.out.println("\n2) Audit naya joda (publisher ka code NAHI chhua) + ek toota listener:");
        p.subscribe(new AuditListener());
        p.subscribe(new BrokenListener());
        p.publish(ev);

        System.out.println("\n3) publish ke BEECH ek listener khud ko hata de:");
        System.out.println("   (a) ArrayList ke saath:");
        TransferPublisher pa = new TransferPublisher(new ArrayList<>());
        addSelfRemover(pa);
        pa.subscribe(new EmailListener());
        pa.subscribe(new SmsListener());
        // (sirf 2 listener hote to CRASH nahi, Email CHUP-CHAAP chhoot jaata — Collections 05 wala trap)
        try {
            pa.publish(ev);
        } catch (ConcurrentModificationException ex) {
            System.out.println("   CRASH -> ConcurrentModificationException");
        }

        System.out.println("   (b) CopyOnWriteArrayList ke saath:");
        TransferPublisher pc = new TransferPublisher(new CopyOnWriteArrayList<>());
        addSelfRemover(pc);
        pc.subscribe(new EmailListener());
        pc.subscribe(new SmsListener());
        pc.publish(ev);
        System.out.println("   theek chala (list ki copy pe ghoomta hai)");
    }

    // ek listener jo pehli khabar milte hi apne aap ko hata deta hai
    static void addSelfRemover(TransferPublisher p) {
        TransferListener[] self = new TransferListener[1];
        self[0] = e -> { System.out.println("   OneTime: mila, ab main hat raha"); p.unsubscribe(self[0]); };
        p.subscribe(self[0]);
    }
}
