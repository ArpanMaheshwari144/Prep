import com.sun.net.httpserver.*;
import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;

public class ChatServer {

    static final int THREADS = 50;

    static ConcurrentHashMap<String, PrintWriter> register = new ConcurrentHashMap<>();

    public static void main(String[] args) throws IOException {

        int port = (args.length > 0) ? Integer.parseInt(args[0]) : 8080;

        HttpServer server = HttpServer.create(new InetSocketAddress(port), 0);
        server.setExecutor(Executors.newFixedThreadPool(THREADS));

        // ---------- page dena (isme kuch karna nahi) ----------
        server.createContext("/", ex -> {
            byte[] page = Files.readAllBytes(Paths.get("index.html"));
            ex.getResponseHeaders().set("Content-Type", "text/html; charset=utf-8");
            ex.sendResponseHeaders(200, page.length);
            ex.getResponseBody().write(page);
            ex.close();
        });

        // ---------- CONNECT — connection kholo aur PAKDE RAHO ----------
        server.createContext("/connect", ex -> {
            String user = param(ex, "user");

            // ye teen header browser ko kehte hain: "connection khuli rakho, main bolta rahunga"
            ex.getResponseHeaders().set("Content-Type", "text/event-stream");
            ex.getResponseHeaders().set("Cache-Control", "no-cache");
            ex.sendResponseHeaders(200, 0);

            PrintWriter pen = new PrintWriter(ex.getResponseBody(), true);


            register.put(user, pen);
            System.out.println("[JUDA] " + user + "   register ab = " + register.keySet());


            try {
                // ye loop connection ko ZINDA rakhta hai.
                // har 15 sec me ek khaali line -- "main abhi bhi hoon"
                while (true) {
                    Thread.sleep(15000);
                    pen.print(": ping\n\n");
                    pen.flush();
                    if (pen.checkError()) throw new IOException("connection tooti");
                }
            } catch (Exception e) {
                register.remove(user, pen);
                System.out.println("[TOOTI] " + user + "   register ab = " + register.keySet());
            }
        });

        // ---------- SEND — register me dhoondo aur THOOS do ----------
        server.createContext("/send", ex -> {
            String from = param(ex, "from");
            String to   = param(ex, "to");
            String text = param(ex, "text");

            System.out.println("\n[SEND] " + from + " -> " + to + " : " + text);

            String jawab;

            PrintWriter pen = register.get(to);
            if (pen != null) {
                pen.print("data: " + from + ": " + text + "\n\n");
                pen.flush();
                jawab = "bhej diya";
            } else {
                jawab = to + " OFFLINE hai";
            }


            byte[] b = jawab.getBytes("UTF-8");
            ex.sendResponseHeaders(200, b.length);
            ex.getResponseBody().write(b);
            ex.close();
        });

        server.start();
        System.out.println("chal pada -> http://localhost:" + port + "/?me=A");
        System.out.println("            http://localhost:" + port + "/?me=B");
        System.out.println("thread pool = " + THREADS + "  (itni hi connection ek saath judegi)");
    }

    // ---------- URL se value nikalne wala chhota helper (isme kuch karna nahi) ----------
    static String param(HttpExchange ex, String key) {
        String q = ex.getRequestURI().getQuery();
        if (q == null) return null;
        for (String pair : q.split("&")) {
            String[] kv = pair.split("=", 2);
            if (kv[0].equals(key)) {
                return kv.length > 1 ? URLDecoder.decode(kv[1], java.nio.charset.StandardCharsets.UTF_8) : "";
            }
        }
        return null;
    }
}
