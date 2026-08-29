// ============================================================
// PRODUCER-CONSUMER — practical (wait/notify + synchronized + threads + join)
// ============================================================
// Ek bounded buffer (LIMIT=5). Producer items daalta, Consumer nikaalta.
//   - buffer FULL -> producer WAIT kare (jab tak consumer jagah na banaye)
//   - buffer EMPTY -> consumer WAIT kare (jab tak producer item na daale)
//   - dono synchronized + wait/notify se coordinate karein.
//
// TU BHAR (produce + consume ka logic):
//   produce(item): while(queue.size()==LIMIT) wait();  queue.add(item);  notify();
//   consume():     while(queue.isEmpty())     wait();  int x=queue.poll();  notify();  return x;
//   ★ wait() -> WHILE loop me (if nahi -- spurious wakeup).  ★ wait/notify SIRF synchronized ke andar.
// ============================================================

import java.util.LinkedList;
import java.util.Queue;

public class ProducerConsumer {

    static class Buffer {
        private Queue<Integer> queue = new LinkedList<>();
        private final int LIMIT = 5;

        public synchronized void produce(int item) throws InterruptedException {
            while(queue.size() == LIMIT){
                wait();
            }
            queue.offer(item);
            notify();
        }

        public synchronized int consume() throws InterruptedException {
            while(queue.isEmpty()){
                wait();
            }
            int x = queue.poll();
            notify();
            return x;
        }
    }

    // ─── HARNESS (isko haath mat lagana) ───
    public static void main(String[] args) throws InterruptedException {
        Buffer buffer = new Buffer();

        Thread producer = new Thread(() -> {
            try {
                for (int i = 1; i <= 10; i++) {
                    buffer.produce(i);
                    System.out.println("Produced: " + i);
                    Thread.sleep(100);
                }
            } catch (InterruptedException e) { e.printStackTrace(); }
        });

        Thread consumer = new Thread(() -> {
            try {
                for (int i = 1; i <= 10; i++) {
                    int item = buffer.consume();
                    System.out.println("        Consumed: " + item);
                    Thread.sleep(150);   // consumer thoda slow -> buffer kabhi FULL hoga (producer wait dekhega)
                }
            } catch (InterruptedException e) { e.printStackTrace(); }
        });

        producer.start();
        consumer.start();
        producer.join();
        consumer.join();
        System.out.println("Done");
    }
}
