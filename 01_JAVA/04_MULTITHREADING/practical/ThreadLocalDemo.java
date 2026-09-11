import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class ThreadLocalDemo {

    private static final ThreadLocal<String> threadLocal = new ThreadLocal<String>();
    static void handleRequest(String user, boolean cleanup) {
        threadLocal.set(user);
        try {
            Thread.sleep(100);
            System.out.println("[" + Thread.currentThread().getName() + "]  sees user = " + threadLocal.get());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            if (cleanup) {
                threadLocal.remove();
            }
        }
    }

    static void readOnly() {
        System.out.println("[" + Thread.currentThread().getName() + "]  readOnly sees = " + threadLocal.get());
    }

    public static void main(String[] args) throws InterruptedException {

        // ===== DEMO 1: ISOLATION — 3 alag thread, har ek apni value =====
        System.out.println("--- DEMO 1: per-thread isolation (har thread sirf apni value) ---");
        Thread tA = new Thread(() -> handleRequest("Arpan", true), "T-A");
        Thread tB = new Thread(() -> handleRequest("Rahul", true), "T-B");
        Thread tC = new Thread(() -> handleRequest("Sita",  true), "T-C");
        tA.start(); tB.start(); tC.start();
        tA.join();  tB.join();  tC.join();
        // EXPECT: T-A sees Arpan · T-B sees Rahul · T-C sees Sita  (clash NAHI)

        // ===== DEMO 2a: POOL REUSE, NO remove() -> LEAK =====
        System.out.println("\n--- DEMO 2a: pool reuse, NO remove() -> LEAK ---");
        ExecutorService pool = Executors.newFixedThreadPool(1);   // 1 thread -> reuse pakka
        pool.submit(() -> handleRequest("Arpan", false));         // cleanup=false -> locker ganda chhodta
        pool.submit(() -> readOnly());                            // pichhli "Arpan" dekhega = LEAK
        pool.shutdown();
        pool.awaitTermination(2, TimeUnit.SECONDS);
        // EXPECT (leak): readOnly() "Arpan" dekhega (galat -- ye nayi req thi)

        // ===== DEMO 2b: same, WITH remove() -> clean =====
        System.out.println("\n--- DEMO 2b: pool reuse, WITH remove() -> clean ---");
        ExecutorService pool2 = Executors.newFixedThreadPool(1);
        pool2.submit(() -> handleRequest("Arpan", true));         // cleanup=true -> remove()
        pool2.submit(() -> readOnly());                           // ab stale nahi -> null
        pool2.shutdown();
        pool2.awaitTermination(2, TimeUnit.SECONDS);
        // EXPECT (clean): readOnly() null dekhega (pichhli req ne remove() kar diya)
    }
}
