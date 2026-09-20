package concurrenthashmap;

import java.util.*;

public class C1_Takkar {

    public static void main(String[] args) throws InterruptedException {

        HashMap<Integer, Integer> hashMap = new HashMap<>();

        Thread t1 = new Thread() {
            public void run() {
                for (int i = 0; i < 1000; i++) {
                    hashMap.put(i, 1);
                }
            }
        };

        Thread t2 = new Thread() {
            public void run() {
                for (int i = 1000; i < 2000; i++) {
                    hashMap.put(i, 1);
                }
            }
        };

        t1.start();
        t2.start();
        t1.join();
        t2.join();

        System.out.println("expected 2000, mila " + hashMap.size());

    }
}
