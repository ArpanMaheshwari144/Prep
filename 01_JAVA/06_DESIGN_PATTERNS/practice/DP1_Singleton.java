import java.util.*;
import java.util.concurrent.*;

class Config {
    static int created = 0;

    private Config() {
        created++;
    }

    private static volatile Config instance;
    public static Config getInstance() {
        if (instance == null) {
            synchronized (Config.class) {
                if (instance == null) {
                    instance = new Config();
                }
            }
        }
        return instance;
    }

}

// ─── Version 2: HOLDER idiom (Bill Pugh) — lazy + thread-safe, bina lock ───
class ConfigHolder {
    static int created = 0;

    private ConfigHolder() {
        created++;
    }

    private static class Holder {                 // ye class tabhi load hogi jab pehli baar Holder.INSTANCE chhua
        private static final ConfigHolder INSTANCE = new ConfigHolder();   // JVM class-init = thread-safe
    }

    public static ConfigHolder getInstance() {
        return Holder.INSTANCE;
    }
}

// ─── Version 3: ENUM — sabse safe (reflection / serialization / clone proof) ───
enum ConfigEnum {
    INSTANCE;                                      // bas yahi. JVM ek hi banata

    private final String appName = "PrepApp";      // config jaisa koi data
    public String getAppName() { return appName; }
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

        // T3: HOLDER — 100 thread, ek hi object
        ExecutorService pool2 = Executors.newFixedThreadPool(100);
        CountDownLatch start2 = new CountDownLatch(1);
        List<Future<ConfigHolder>> got2 = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            got2.add(pool2.submit(() -> { start2.await(); return ConfigHolder.getInstance(); }));
        }
        start2.countDown();
        Set<ConfigHolder> d2 = Collections.newSetFromMap(new IdentityHashMap<>());
        for (Future<ConfigHolder> f : got2) d2.add(f.get());
        pool2.shutdown();
        System.out.println("T3 holder -> objects=" + d2.size() + ", created=" + ConfigHolder.created
                + ": " + (d2.size() == 1 && ConfigHolder.created == 1 ? "PASS" : "FAIL"));

        // T4: ENUM — wahi ek object, aur reflection se naya banana NAHI hota
        System.out.println("T4 enum same: " + (ConfigEnum.INSTANCE == ConfigEnum.INSTANCE ? "PASS" : "FAIL")
                + " · appName=" + ConfigEnum.INSTANCE.getAppName());
        try {
            java.lang.reflect.Constructor<?> k = ConfigEnum.class.getDeclaredConstructors()[0];
            k.setAccessible(true);
            k.newInstance("X", 1);
            System.out.println("T5 enum reflection: FAIL (naya ban gaya)");
        } catch (Exception e) {
            System.out.println("T5 enum reflection blocked: PASS (" + e.getClass().getSimpleName() + ")");
        }
    }
}
