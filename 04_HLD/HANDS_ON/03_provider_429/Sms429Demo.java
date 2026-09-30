// ============================================================
// CASE 3: SMS provider bole "429 — bahut tez bhej rahe ho". Ab kya?
// ============================================================
// Sale ka din: 1000 order ek saath -> 1000 SMS bhejne hain.
// Provider (Twilio jaisa) 1 second me sirf 100 SMS leta hai. Zyada -> 429.
// Beech me (t=3, t=4) provider aur dheema ho jaata hai: sirf 50/sec.
//
// Time NAKLI hai (t = second ki ginti), isliye turant chalta hai. Koi Docker / Redis nahi.
//
// CHALANE KA TAREEKA
//   ROUND 1:  SMART = false   (seedha sab bhej do, 429 pe turant retry, 3 baar ke baad chhod do)
//     java Sms429Demo.java
//   ROUND 2:  SMART = true    (apni taraf throttle 100/sec + 429 pe backoff 1s, 2s, 4s, kabhi drop nahi)
//     java Sms429Demo.java
// ============================================================

import java.util.*;

public class Sms429Demo {

    static final boolean SMART = false;

    static final int TOTAL_SMS = 1000;
    static final int OUR_RATE = 100;        // SMART me hum 1 sec me isse zyada nahi bhejte (token bucket)
    static final int NAIVE_MAX_TRY = 3;     // NAIVE me 3 baar 429 -> SMS chhod diya

    // provider 1 second me kitne lega
    static int providerLimit(int t) {
        return (t == 3 || t == 4) ? 50 : 100;
    }

    static class Sms {
        int id;
        int attempts = 0;
        int nextTry = 0;     // kis second pe dobara bhejna hai
        Sms(int id) { this.id = id; }
    }

    public static void main(String[] args) {
        List<Sms> queue = new ArrayList<>();
        for (int i = 1; i <= TOTAL_SMS; i++) queue.add(new Sms(i));

        int delivered = 0, dropped = 0, totalCalls = 0, total429 = 0;

        System.out.println("MODE = " + (SMART ? "SMART (throttle + backoff)" : "NAIVE (sab bhej do, turant retry)"));
        System.out.println();
        System.out.println("  t | provider | queue me | bheje | 200 OK | 429 | chhode | pahunche (kul)");
        System.out.println("----+----------+----------+-------+--------+-----+--------+---------------");

        for (int t = 0; !queue.isEmpty() && t < 40; t++) {
            int limit = providerLimit(t);
            int budget = SMART ? OUR_RATE : Integer.MAX_VALUE;   // NAIVE: jitna hai sab bhejo
            int queueAtStart = queue.size();
            int sent = 0, ok = 0, r429 = 0, drop = 0;

            Iterator<Sms> it = queue.iterator();
            while (it.hasNext()) {
                Sms s = it.next();
                if (s.nextTry > t) continue;          // abhi iska waqt nahi (backoff me ruka hai)
                if (sent == budget) break;            // is second ka apna quota khatam

                sent++;
                if (ok < limit) {                     // provider ne le liya
                    ok++;
                    delivered++;
                    it.remove();
                } else {                              // provider: 429
                    r429++;
                    s.attempts++;
                    if (!SMART) {
                        if (s.attempts >= NAIVE_MAX_TRY) { drop++; dropped++; it.remove(); }
                        else s.nextTry = t + 1;                                  // turant agle second
                    } else {
                        s.nextTry = t + (1 << Math.min(s.attempts - 1, 3));      // 1, 2, 4, 8 sec baad
                    }
                }
            }
            totalCalls += sent;
            total429 += r429;

            System.out.printf("%3d | %4d/sec | %8d | %5d | %6d | %3d | %6d | %d%n",
                    t, limit, queueAtStart, sent, ok, r429, drop, delivered);
        }

        System.out.println();
        System.out.println("PAHUNCHE      = " + delivered + " / " + TOTAL_SMS);
        System.out.println("CHHOD DIYE    = " + dropped + "   <- ye customers ko SMS kabhi nahi mila");
        System.out.println("provider call = " + totalCalls + "  (inme 429 = " + total429 + ")");
    }
}
