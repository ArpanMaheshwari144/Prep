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

//    static AtomicInteger count = new AtomicInteger(0);   // CAS-based, lock-free + atomic (safe)

    public static void main(String[] args) throws InterruptedException {
        // count ab main ke ANDAR (local). lambda ise capture kar leta kyunki 'count' reference
        // kabhi reassign nahi hota = EFFECTIVELY FINAL (andar ka object mutate hota, reference nahi).
        AtomicInteger count = new AtomicInteger(0);

        // task = har thread ka kaam: counter ko loop me 10 lakh baar badhao
        // ★ LAMBDA vs CLASS -- dono SAME (Thread ko ek Runnable [jisme run()] chahiye):
        //   CLASS way:  class MyTask implements Runnable { public void run(){ ...loop... } }
        //               new Thread(new MyTask());          // alag class + run() likhna padta
        //   LAMBDA way (neeche): () -> { ...loop... }       // ye {} block hi run() ka BODY
        //   kyun chalta: Runnable = FUNCTIONAL INTERFACE (sirf 1 method = run()) -> lambda 1-line me.
        //   lambda = class+run() ka SHORTCUT (kaam same, likhna kam).
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
