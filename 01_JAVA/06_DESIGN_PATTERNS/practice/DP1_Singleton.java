// DESIGN PATTERN PRACTICE — DP1 Singleton (khud likho, phir chalao)
// Task: Config class ko SINGLETON banao — poori app me sirf EK object.
//   1. bahar se koi `new Config()` na kar sake
//   2. Config.getInstance() hamesha WAHI object de
//   3. LAZY ho (pehli getInstance() pe bane) + 100 thread ek saath maangein tab bhi EK hi bane
//   constructor ke andar `created++;` zaroor rakhna — test isi se ginta hai kitne object bane
//
// Tests (neeche main me, mat chhedna):
//   T1  100 thread ek saath PEHLI baar getInstance() -> sirf 1 object, created == 1 -> PASS
//   T2  baad me c1 == c2 (wahi object)                                       -> PASS
//   (lazy wala bina lock likhoge to T1 kabhi-kabhi FAIL hoga — kai baar chala ke dekhna)
//
// Hint (jitna chahiye): yaad "TAALA · DABBA · DARWAAZA". thread-safe + lazy ke 2 tareeke file me hain
//   (upar wali file: ../02_singleton.md). Ek likh, chala, phir doosra bhi aazma.
// compile+run:  javac DP1_Singleton.java && java DP1_Singleton

import java.util.*;
import java.util.concurrent.*;

class Config {
    static int created = 0;

    

}

public class DP1_Singleton {
    public static void main(String[] args) throws Exception {
        // T1 PEHLE: 100 thread ek saath, bilkul pehli baar getInstance() (abhi koi object nahi bana)
        ExecutorService pool = Executors.newFixedThreadPool(100);
        CountDownLatch start = new CountDownLatch(1);
        List<Future<Config>> got = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            got.add(pool.submit(() -> { start.await(); return Config.getInstance(); }));
        }
        start.countDown();
        Set<Config> distinct = Collections.newSetFromMap(new IdentityHashMap<>());
        for (Future<Config> f : got) distinct.add(f.get());
        pool.shutdown();
        System.out.println("T1 100 threads -> objects=" + distinct.size() + ", created=" + Config.created
                + " (expect 1, 1): " + (distinct.size() == 1 && Config.created == 1 ? "PASS" : "FAIL"));

        // T2: baad me maango -> wahi object
        Config c1 = Config.getInstance();
        Config c2 = Config.getInstance();
        System.out.println("T2 same object: " + (c1 == c2 && distinct.contains(c1) ? "PASS" : "FAIL"));
    }
}
