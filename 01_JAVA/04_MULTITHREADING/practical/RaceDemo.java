// ============================================================
// RACE CONDITION — practical (threads + fix via AtomicInteger)
// ============================================================
// SEEKHA (live dekha):
//   - 2 thread ek shared counter ko ek saath badhaye -> RACE (increment gum hote)
//   - chhota loop (1000) -> race chhup jaati (jawab sahi aata)
//   - BADA loop (10 lakh) -> threads aapas me tikrate -> race SAAMNE (total < expected)
//   - FIX = AtomicInteger (CAS, lock-free, atomic) -> exactly expected, har baar
//
// EXPECTED: 2 thread * 1000000 = 2000000
//   count++ (raw)           -> 2000000 se KAM (race, har run alag)
//   count.incrementAndGet() -> 2000000 EXACT (race gaya)
// ============================================================

import java.util.concurrent.atomic.AtomicInteger;

public class RaceDemo {

    // ─── shared counter ─────────────────────────────────────
    // AtomicInteger = CAS-based (Compare-And-Swap), lock-free + atomic.
    // "read -> naya soch -> match to likho, warna retry" -> increment kabhi gum nahi.
    static AtomicInteger count = new AtomicInteger(0);

    // ─── ALT FIX (synchronized) -- yehi kaam, taala laga ke ─────
    // static int count = 0;
    // synchronized static void inc() { count++; }   // main ke BAHAR, class-level
    // (loop me count.incrementAndGet() ki jagah inc(); print me count)
    // synchronized = taala (ek time ek thread), CAS se thoda slow par correct.

    public static void main(String[] args) throws InterruptedException {

        // task = har thread ka kaam: counter ko 10 lakh baar badhao
        Runnable task = () -> {
            for (int i = 0; i < 1000000; i++) {
                // count++;                 // <- RAW int hota to: race (READ+ADD+WRITE, ek update LOST)
                count.incrementAndGet();    // <- ATOMIC: safe, kabhi gum nahi
            }
        };

        Thread thread1 = new Thread(task);
        Thread thread2 = new Thread(task);

        thread1.start();   // dono PARALLEL (start(), run() nahi)
        thread2.start();

        thread1.join();    // join = thread ke khatam hone ka WAIT
        thread2.join();    // (warna main pehle print kar deta, count adhoora)

        System.out.println(count.get());   // AtomicInteger -> count.get() se value
    }
}
