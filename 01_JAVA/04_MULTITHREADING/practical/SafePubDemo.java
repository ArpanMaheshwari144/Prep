import java.util.concurrent.atomic.AtomicInteger;

public class SafePubDemo {

    // Demo 1 ke liye — mutable, NOT thread-safe (plain int)
    static class Counter {
        int c = 0;
        void inc() { c++; }        // race: read-modify-write, atomic NAHI
    }

    public static void main(String[] args) throws InterruptedException {
        Counter shared = new Counter();
        Thread t1 = new Thread(()->{
            for(int i=0;i<100000;i++) {
                shared.inc();
            }
        });
        Thread t2 = new Thread(()->{
            for(int i=0;i<100000;i++) {
                shared.inc();
            }
        });
        t1.start();
        t2.start();
        t1.join();
        t2.join();
        System.out.println("Demo1 (mutable, expect < 200000): " + shared.c);

        AtomicInteger atomic = new AtomicInteger(0);
        Thread t3 = new Thread(()->{
            for(int i=0;i<100000;i++) {
                atomic.incrementAndGet();
            }
        });
        Thread t4 = new Thread(()->{
            for(int i=0;i<100000;i++) {
                atomic.incrementAndGet();
            }
        });
        t3.start();
        t4.start();
        t3.join();
        t4.join();
        System.out.println("Demo2 (atomic, expect 200000): " + atomic.get());

        int x = 5;
        Runnable r = () -> System.out.println("Demo3 lambda saw x = " + x);
//         x = 6;
        r.run();
    }
}
