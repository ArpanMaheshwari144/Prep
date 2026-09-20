package concurrenthashmap;

import java.util.concurrent.*;
import java.util.concurrent.atomic.*;

public class C4_Ilaaj {

    public static void main(String[] args) throws InterruptedException {

        ConcurrentHashMap<String, Integer> map1 = new ConcurrentHashMap<>();
        map1.put("hits", 0);
        Thread a1 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                map1.merge("hits", 1, (purani, nayi) -> purani + nayi);
            }
        });

        Thread a2 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                map1.compute("hits", (k, v) -> v == null ? 1 : v + 1);
            }
        });

        a1.start();
        a2.start();
        a1.join();
        a2.join();


        ConcurrentHashMap<String, Integer> map2 = new ConcurrentHashMap<>();
        map2.put("hits", 0);
        Thread b1 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                map2.compute("hits", (k, v) -> v == null ? 1 : v + 1);
            }
        });

        Thread b2 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                map2.compute("hits", (k, v) -> v == null ? 1 : v + 1);
            }
        });

        b1.start();
        b2.start();
        b1.join();
        b2.join();


        ConcurrentHashMap<String, AtomicInteger> map3 = new ConcurrentHashMap<>();
        map3.put("hits", new AtomicInteger(0));
        Thread c1 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                map3.get("hits").incrementAndGet();
            }
        });

        Thread c2 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                map3.get("hits").incrementAndGet();
            }
        });

        c1.start();
        c2.start();
        c1.join();
        c2.join();


        System.out.println("merge         = " + map1.get("hits"));
        System.out.println("compute       = " + map2.get("hits"));
        System.out.println("AtomicInteger = " + map3.get("hits").get());
    }
}
