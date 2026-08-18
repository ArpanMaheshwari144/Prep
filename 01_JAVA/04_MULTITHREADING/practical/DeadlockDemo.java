// ============================================================
// DEADLOCK DEMO — chalta hua deadlock dekho (tu banayega, main guide)
// ============================================================
// GOAL: 2 thread, 2 lock. dono ULTA order me lock lein -> program HANG (deadlock).
//
// Kahani:
//   Thread-1: pehle lockA lo -> phir lockB chahiye.
//   Thread-2: pehle lockB lo -> phir lockA chahiye.
//   -> dono ek-doosre ka wait -> koi release nahi -> HANG (na khatam hoga, na aage).
// ============================================================

public class DeadlockDemo {

    // 2 lock objects (shared) -- ye main ne de diye, inhi pe lock lena
    static final Object lockA = new Object();
    static final Object lockB = new Object();

    public static void main(String[] args) {

        // ---- THREAD 1: lockA pehle, phir lockB ----
        Thread t1 = new Thread(() -> {
            synchronized (lockA) {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                synchronized (lockB) {
                    System.out.println("T1 got both");
                }
            }
        });

        // ---- THREAD 2: lockB pehle, phir lockA (ULTA order) ----
        Thread t2 = new Thread(() -> {
            synchronized (lockB) {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                synchronized (lockA) {
                    System.out.println("T2 got both");
                }
            }
        });

        t1.start();
        t2.start();
        System.out.println("dono thread start ho gaye... (agar hang -> deadlock)");
    }
}
