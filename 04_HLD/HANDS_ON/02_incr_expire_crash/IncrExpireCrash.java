// ============================================================
// CASE 2: INCR ke baad, EXPIRE se PEHLE app crash ho jaaye to?
// ============================================================
// Niyam wahi: user:1 ko 60 sec me max 5 request.
//
// CHALANE KA TAREEKA
//   Terminal 1:
//     docker run -d --name demo-redis -p 6390:6379 redis:7
//
//   ROUND 1 (bug dekho):  USE_LUA = false, CRASH_AFTER_INCR = true
//     java IncrExpireCrash.java      -> ENTER   (pehli request pe app crash)
//     java IncrExpireCrash.java      -> ENTER ENTER ENTER ... TTL dekho
//     1 minute ruk ke phir ENTER     -> kya user chhoota?
//
//   ROUND 2 (fix dekho):  USE_LUA = true
//     docker exec demo-redis redis-cli DEL rl:user:1     (purani key saaf)
//     java IncrExpireCrash.java      -> ENTER ... TTL dekho
//
//   Khatam:
//     docker rm -f demo-redis
// ============================================================

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Scanner;

public class IncrExpireCrash {

    static final String HOST = "localhost";
    static final int PORT = 6390;
    static final String KEY = "rl:user:1";
    static final int LIMIT = 5;
    static final int WINDOW_SEC = 60;

    static final boolean CRASH_AFTER_INCR = true;   // pehli request pe INCR ke baad app band
    static final boolean USE_LUA = false;           // true = INCR + EXPIRE ek hi command me (Lua)

    // Redis ke andar chalne wali script: dono kaam ek saath, beech me koi ghus nahi sakta
    static final String LUA =
            "local c = redis.call('INCR', KEYS[1]) " +
            "if c == 1 then redis.call('EXPIRE', KEYS[1], ARGV[1]) end " +
            "return c";

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(System.in);
        int reqNo = 0;
        System.out.println("USE_LUA = " + USE_LUA + "   CRASH_AFTER_INCR = " + CRASH_AFTER_INCR);
        System.out.println("ENTER dabao = ek request   (q + ENTER = band)");

        while (sc.hasNextLine()) {
            if (sc.nextLine().trim().equals("q")) break;
            reqNo++;
            System.out.println("\n--- request " + reqNo + " ---");

            long count;
            if (USE_LUA) {
                count = redis("EVAL", LUA, "1", KEY, String.valueOf(WINDOW_SEC));
                System.out.println("EVAL (INCR + EXPIRE ek saath) -> count = " + count);
            } else {
                count = redis("INCR", KEY);
                System.out.println("INCR -> count = " + count);

                if (count == 1) {
                    if (CRASH_AFTER_INCR) {
                        System.out.println("CRASH! app EXPIRE se pehle mar gaya");
                        System.exit(1);
                    }
                    redis("EXPIRE", KEY, String.valueOf(WINDOW_SEC));
                    System.out.println("EXPIRE " + WINDOW_SEC + " sec laga");
                }
            }

            long ttl = redis("TTL", KEY);
            System.out.println("TTL = " + ttl + (ttl == -1 ? "   <- -1 matlab KOI TIMER NAHI, key kabhi nahi mitegi" : " sec baaki"));

            if (count <= LIMIT) {
                System.out.println("count " + count + " <= " + LIMIT + " -> ALLOW");
            } else {
                System.out.println("count " + count + " > " + LIMIT + " -> BLOCK (429)");
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
