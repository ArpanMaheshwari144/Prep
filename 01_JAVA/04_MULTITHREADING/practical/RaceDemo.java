// ============================================================
// RACE CONDITION — practical (sirf jo padha: threads + synchronized/atomic)
// ============================================================
// Plan:
//   1. shared counter count = 0
//   2. Runnable task -> loop me count++ (bada loop = race dikhti)
//   3. 2 thread banao (task), dono start()  -> parallel
//   4. dono join()  (thread ke khatam hone ka WAIT -- warna main pehle print karega)
//   5. print count  -> expected se KAM aaya? = RACE (increment gum hue)
//   6. FIX: count++ ko synchronized method me, YA AtomicInteger use karo -> exact
// ============================================================

import java.util.concurrent.atomic.AtomicInteger;

public class RaceDemo {

//    static int count = 0;   // shared counter (yahin race hoti)
//
//    synchronized static void inc() {   // main ke BAHAR (class-level), taala laga ke fix
//        count++;
//    }

    static AtomicInteger count = new AtomicInteger(0);   // CAS-based, lock-free + atomic (safe)

    public static void main(String[] args) throws InterruptedException {
        // task = har thread ka kaam: counter ko loop me 10 lakh baar badhao
        Runnable task = () -> {
            for (int i = 0; i < 1000000; i++) {
                // count++;              // raw int hota to yahan RACE (ek update LOST)
                // inc();                // ya synchronized wala fix

                count.incrementAndGet();  // atomic -> increment kabhi gum nahi
            }
        };

        Thread thread1 = new Thread(task);
        Thread thread2 = new Thread(task);

        thread1.start();   // dono PARALLEL (start(), run() nahi)
        thread2.start();

        thread1.join();    // thread ke khatam hone ka WAIT
        thread2.join();

        System.out.println(count.get());   // AtomicInteger -> count.get()
    }
}
