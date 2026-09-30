// ============================================================
// CASE 4: bank ka "paisa kata" event — kahan kho sakta hai, kaise bachao?
// ============================================================
// 5 debit hote hain: e1..e5. Har debit ka event Kafka se SMS service tak jaana chahiye.
// Event 3 jagah kho sakta hai — teeno jagah jaan-boojh ke crash karwaya hai:
//
//   [Bank DB] --1--> [Kafka] --2--> (replica) --3--> [SMS service]
//
//   1  e3: paisa kata (DB me likha), event bhejne se PEHLE app crash
//   2  e4: Kafka leader ne le liya, replica tak pahunchne se PEHLE leader gira
//   3  e2: SMS service ne offset commit kiya, SMS bhejne se PEHLE crash
//
// Sab nakli hai (DB = list, Kafka = list). Koi Docker nahi, turant chalta.
//
// CHALANE KA TAREEKA
//   ROUND 1:  FIX = false   ->  java EventLossDemo.java   (kaunse event khoye?)
//   ROUND 2:  FIX = true    ->  java EventLossDemo.java   (outbox + acks=all + offset baad me + idempotent)
// ============================================================

import java.util.*;

public class EventLossDemo {

    static final boolean FIX = false;

    static List<String> bankDb = new ArrayList<>();      // debit table
    static List<String> outbox = new ArrayList<>();      // FIX: event yahan, debit ke SAATH
    static List<String> kafkaLog = new ArrayList<>();    // jo Kafka me PAKKA bacha (replica tak)
    static List<String> smsSent = new ArrayList<>();     // customer ko gaye SMS

    public static void main(String[] args) {
        System.out.println("FIX = " + FIX + "\n");

        // ---------------- 1. PRODUCER ----------------
        System.out.println("=== 1. PRODUCER: paisa kaato + event bhejo ===");
        for (int i = 1; i <= 5; i++) {
            String e = "e" + i;
            if (!FIX) {
                bankDb.add(e);
                System.out.println(e + ": DB me debit likha");
                if (e.equals("e3")) {
                    System.out.println(e + ": CRASH! event bhejne se pehle app mar gaya -> event kabhi nahi gaya");
                    continue;
                }
                sendToKafka(e);
            } else {
                bankDb.add(e);
                outbox.add(e);                                      // dono EK transaction me
                System.out.println(e + ": DB debit + OUTBOX row, dono ek transaction me");
                if (e.equals("e3")) {
                    System.out.println(e + ": CRASH! bhejne se pehle app mar gaya -> koi baat nahi, outbox me pada hai");
                }
            }
        }
        if (FIX) {
            System.out.println("-- relay process (restart ke baad bhi) outbox padh ke Kafka bhejta hai --");
            for (String e : outbox) sendToKafka(e);
        }

        // ---------------- 3. CONSUMER ----------------
        System.out.println("\n=== 3. CONSUMER: SMS service Kafka se padhti hai ===");
        int offset = 0;                     // "yahan tak padh liya" (commit)
        boolean crashedOnce = false;
        Set<String> alreadyDone = new HashSet<>();   // FIX: idempotency (eventId)

        while (offset < kafkaLog.size()) {
            String e = kafkaLog.get(offset);
            if (!FIX) {
                offset++;                                           // PEHLE commit
                System.out.println(e + ": offset commit (" + offset + ")");
                if (e.equals("e2") && !crashedOnce) {
                    crashedOnce = true;
                    System.out.println(e + ": CRASH! SMS bhejne se pehle -> restart pe offset " + offset + " se aage padhega -> " + e + " CHHOOT gaya");
                    continue;
                }
                smsSent.add(e);
                System.out.println(e + ": SMS bheja");
            } else {
                if (alreadyDone.contains(e)) {
                    System.out.println(e + ": ye pehle ho chuka (eventId dekha) -> DUPLICATE chhoda");
                    offset++;
                    continue;
                }
                smsSent.add(e);
                alreadyDone.add(e);
                System.out.println(e + ": SMS bheja");
                if (e.equals("e2") && !crashedOnce) {
                    crashedOnce = true;
                    System.out.println(e + ": CRASH! offset commit se pehle -> restart pe " + e + " PHIR aayega");
                    continue;                                       // offset nahi badha
                }
                offset++;                                           // kaam ke BAAD commit
                System.out.println(e + ": offset commit (" + offset + ")");
            }
        }

        // ---------------- NATEEJA ----------------
        List<String> lost = new ArrayList<>(bankDb);
        lost.removeAll(smsSent);
        System.out.println("\n=== NATEEJA ===");
        System.out.println("paisa kata (DB)   = " + bankDb);
        System.out.println("Kafka me bacha    = " + kafkaLog);
        System.out.println("SMS mila          = " + smsSent);
        System.out.println("KHO GAYE          = " + lost + (lost.isEmpty() ? "" : "   <- paisa kata, customer ko pata nahi"));
    }

    // ---------------- 2. KAFKA ----------------
    static void sendToKafka(String e) {
        if (e.equals("e4")) {
            if (!FIX) {
                System.out.println(e + ": Kafka (acks=1) leader ne memory me rakha, turant 'OK' bol diya");
                System.out.println(e + ": CRASH! leader gira, replica tak pahuncha hi nahi -> " + e + " GAYA (producer ko lagta hai bhej diya)");
                return;
            } else {
                System.out.println(e + ": Kafka (acks=all) leader + replica dono pe likha, TAB 'OK'");
                System.out.println(e + ": CRASH! leader gira -> replica naya leader, " + e + " uske paas hai");
            }
        } else {
            System.out.println(e + ": Kafka me pahuncha");
        }
        kafkaLog.add(e);
    }
}
