// ============================================================
// RATE LIMITER + REDIS — khud chala ke dekho (koi library nahi, sirf Java)
// ============================================================
// Niyam: ek user (user:1) ko 60 sec me max 5 request. 6th se BLOCK.
//
// CHALANE KA TAREEKA
//   Terminal 1 (Redis chalao, persistence BAND):
//     docker run -d --name demo-redis -p 6390:6379 redis:7 redis-server --save "" --appendonly no
//
//   Terminal 2 (ye program):
//     java RateLimiterDemo.java
//     -> har ENTER = ek request
//
//   Beech me Terminal 1 se khud maaro / wapas lao:
//     docker stop demo-redis
//     docker start demo-redis
//
//   Khatam hone pe:
//     docker rm -f demo-redis
// ============================================================

import java.io.*;
import java.net.Socket;
import java.util.Scanner;

public class RateLimiterDemo {

    static final String HOST = "localhost";
    static final int PORT = 6390;
    static final String KEY = "rl:user:1";
    static final int LIMIT = 5;
    static final int WINDOW_SEC = 60;

    // Redis na mile to kya karein?  true = ALLOW (fail-open)  |  false = BLOCK (fail-closed)
    static final boolean FAIL_OPEN = true;

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int reqNo = 0;
        System.out.println("ENTER dabao = ek request   (q + ENTER = band)");

        while (sc.hasNextLine()) {
            if (sc.nextLine().trim().equals("q")) break;
            reqNo++;
            System.out.println("\n--- request " + reqNo + " ---");

            try {
                long count = redis("INCR", KEY);
                System.out.println("INCR " + KEY + " -> count = " + count);

                if (count == 1) {
                    redis("EXPIRE", KEY, String.valueOf(WINDOW_SEC));
                    System.out.println("pehli request -> EXPIRE " + WINDOW_SEC + " sec laga");
                }

                long ttl = redis("TTL", KEY);
                System.out.println("TTL baaki = " + ttl + " sec");

                if (count <= LIMIT) {
                    System.out.println("count " + count + " <= " + LIMIT + " -> ALLOW");
                } else {
                    System.out.println("count " + count + " > " + LIMIT + " -> BLOCK (429)");
                }

            } catch (IOException e) {
                System.out.println("REDIS SE BAAT NAHI HUI: " + e.getMessage());
                if (FAIL_OPEN) {
                    System.out.println("FAIL_OPEN = true -> ALLOW (bina limit ke)");
                } else {
                    System.out.println("FAIL_OPEN = false -> BLOCK (429)");
                }
            }
        }
        System.out.println("band.");
    }

    // Ek Redis command bhejo, integer jawab lautao. (Redis ka protocol = RESP, plain text)
    static long redis(String... parts) throws IOException {
        try (Socket s = new Socket(HOST, PORT)) {
            s.setSoTimeout(1000);
            StringBuilder cmd = new StringBuilder("*" + parts.length + "\r\n");
            for (String p : parts) {
                cmd.append("$").append(p.length()).append("\r\n").append(p).append("\r\n");
            }
            OutputStream out = s.getOutputStream();
            out.write(cmd.toString().getBytes());
            out.flush();

            BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
            String reply = in.readLine();          // jaise ":6"
            if (reply == null || reply.charAt(0) != ':') {
                throw new IOException("ajeeb jawab: " + reply);
            }
            return Long.parseLong(reply.substring(1));
        }
    }
}
