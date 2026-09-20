package concurrenthashmap;

import java.util.*;
import java.util.concurrent.*;

public class C2_Concurrent {

    public static int chalao(Map<Integer, Integer> map) throws InterruptedException{

        Thread t1 = new Thread() {
            public void run() {
                for (int i = 0; i < 1000; i++) {
                    map.put(i, 1);
                }
            }
        };

        Thread t2 = new Thread() {
            public void run() {
                for (int i = 1000; i < 2000; i++) {
                    map.put(i, 1);
                }
            }
        };

        t1.start();
        t2.start();
        t1.join();
        t2.join();

        return map.size();
    }


    public static void main(String[] args) throws InterruptedException {
        int ans1 = 0;
        for (int i = 0; i < 10; i++) {
            Map<Integer, Integer> hashMap = new HashMap<>();
            ans1 = chalao(hashMap);
        }

        int ans2 = 0;
        for (int i = 0; i < 10; i++) {
            Map<Integer, Integer> concurrentHashMaphashMap = new ConcurrentHashMap<>();
            ans2 = chalao(concurrentHashMaphashMap);
        }

        System.out.println(ans1);
        System.out.println(ans2);
    }
}
