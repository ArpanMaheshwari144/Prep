package concurrenthashmap;

import java.util.concurrent.*;

public class C3_CheckThenAct {

    public static void main(String[] args) throws InterruptedException {

        ConcurrentHashMap<String, Integer>  map = new ConcurrentHashMap<>();
        map.put("hits", 0);


        Thread t1 = new Thread(()->{
            for (int i=0;i<10000;i++) {
                int hits = map.get("hits");
                int val = hits + 1;
                map.put("hits",val);
            }
        });

        Thread t2 = new Thread(()->{
            for (int i=0;i<10000;i++) {                 
                int hits = map.get("hits");             
                int val = hits + 1;
                map.put("hits",val);
            }
        });

        t1.start();
        t2.start();
        t1.join();
        t2.join();

        System.out.println("expected 20000, mila " + map.get("hits")   );
    }
}
