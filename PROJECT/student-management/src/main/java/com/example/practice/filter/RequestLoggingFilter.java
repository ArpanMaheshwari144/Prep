package com.example.practice.filter;

/*
 * ============================================================
 * YE KYA HAI: Request Logging Filter — har request log kare. Method, URL, IP, time.
 * KYUN CHAHIYE: Production mein pata hona chahiye kaun kya request bhej raha. Debugging ke liye CRITICAL.
 * KYA KARTA: Request aaye → log (method, URL, IP) → aage bhejo → response aaye → log (status, time).
 * NAHI HUA TO: Bug aaya → pata nahi kaunsi request fail hui, kab hui, kitna time laga. Blind debugging.
 *
 * REAL WORLD ANALOGY — YE BUILDING KA CCTV CAMERA HAI:
 *   Har visitor (request) aaye → camera record kare:
 *     - Kaun aaya (IP address)
 *     - Kahan ja raha (URL)
 *     - Kya karne aaya (GET/POST/PUT/DELETE)
 *     - Kitna time laga (duration)
 *     - Result kya hua (200 OK / 404 Not Found)
 *   Bina CCTV: kuch galat hua toh CCTV footage dekhte — yahan logs dekhte.
 *   CCTV nahi → "chori ho gayi, koi proof nahi" → debugging impossible.
 *
 * FILTER vs INTERCEPTOR (IMPORTANT DIFFERENCE):
 *   Filter    = SERVLET level = SABSE PEHLE chalta = SAB requests (images, CSS, API sab)
 *   Interceptor = SPRING level = BAAD mein chalta = sirf controller URLs
 *
 *   Filter ye TOMCAT ka part hai. Spring se PEHLE chalta.
 *   Interceptor ye Spring MVC ka part hai. DispatcherServlet ke BAAD chalta.
 *
 *   REQUEST PATH:
 *     Client → Tomcat → [FILTER] → DispatcherServlet → [INTERCEPTOR] → Controller
 * ============================================================
 */

import jakarta.servlet.*;
import jakarta.servlet.http.*;
import org.springframework.stereotype.Component;
import java.io.IOException;

/*
 * ============================================================
 * @Component
 *
 * YE KYA HAI: Spring bean banao — auto-register as Servlet Filter in Tomcat.
 *
 * KYUN LAGAYA: @Component lagaya → Spring detect karta → Tomcat mein filter register karta.
 *   Ek annotation se done — alag se registration ki zaroorat nahi.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Filter class banayi but register NAHI → koi request pe nahi chalega
 *   - Logging DEAD — koi request ka log nahi aayega
 *   - Class banayi, code likha — sab WASTE. Kyunki Spring ko pata hi nahi ki ye exist karta.
 *
 * REAL WORLD ANALOGY:
 *   @Component = CCTV camera ko POWER SUPPLY dena.
 *   Camera laga diya (class banayi) → but power nahi di (@Component nahi lagaya) → recording NAHI.
 *   Power di (@Component lagaya) → camera ON → sab record.
 *
 * Filter register karne ke 2 tarike:
 *   1. @Component lagao → auto register (SIMPLE — humne ye kiya)
 *   2. FilterRegistrationBean @Bean se register → order, URL pattern control (ADVANCED)
 * ============================================================
 */
@Component
public class RequestLoggingFilter implements Filter {
    /*
     * ============================================================
     * implements Filter (jakarta.servlet.Filter)
     *
     * YE KYA HAI: Servlet API ka interface. SERVLET level filter — Tomcat pe directly register.
     *   SAB requests pe chalta — API, CSS, images, JavaScript — har ek HTTP request.
     *
     * KYUN LAGAYA: Servlet level logging chahiye — sab traffic monitor karna hai.
     *
     * NAHI LAGAYA TO (Filter implement nahi kiya):
     *   - Ye class ek normal class hai — filter nahi
     *   - doFilter() method ka koi meaning nahi — Spring isko filter nahi maanega
     *   - @Component lagaya toh bean banega but filter chain mein nahi jaayega
     *
     * REAL WORLD ANALOGY:
     *   implements Filter = "main mein ek GUARD hoon" — ye identity hai.
     *   Bina identity: normal aadmi khada hai gate pe — koi nahi sunega.
     *   Identity hai (Filter interface) → system jaanta "ye gate pe khada hoga, sab check karega."
     *
     * 3 methods (Filter interface):
     *   init()    — filter start pe EK baar (setup)
     *   doFilter() — HAR request pe (main logic) — ye IMPORTANT hai
     *   destroy() — filter band hote waqt (cleanup)
     *   Yahan sirf doFilter() override kiya — init/destroy ki zaroorat nahi.
     * ============================================================
     */

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        /*
         * HAR HTTP request pe ye method AUTOMATICALLY call hota.
         *
         * ServletRequest/Response → generic (HTTP + non-HTTP).
         * HTTP specific methods ke liye HttpServletRequest/Response mein CAST karna padta:
         */

        HttpServletRequest req = (HttpServletRequest) request;
        HttpServletResponse res = (HttpServletResponse) response;

        // ===== REQUEST INFO LOG (request aaye — jaate waqt ka log) =====
        long startTime = System.currentTimeMillis();
        System.out.println("=== REQUEST ===");
        System.out.println("Method: " + req.getMethod());           // GET, POST, PUT, DELETE
        System.out.println("URL: " + req.getRequestURI());          // /api/students/5
        System.out.println("IP: " + req.getRemoteAddr());           // 192.168.1.1 (client IP)
        /*
         * Real project mein System.out.println NAHI — Logger use karo:
         *   private static final Logger log = LoggerFactory.getLogger(RequestLoggingFilter.class);
         *   log.info("Method: {}, URL: {}", req.getMethod(), req.getRequestURI());
         *   Ya Lombok: @Slf4j lagao → log.info() seedha use.
         *
         *   Logger KYUN (System.out se BETTER):
         *     - Log levels (DEBUG, INFO, WARN, ERROR) — production mein sirf ERROR dikhao
         *     - Log FILES mein save (System.out console pe hi dikhta — restart = gone)
         *     - Timestamp automatic aata
         *     - Thread info milti
         *     - Configurable — kaunse logs dikhao, kaunse nahi
         */

        // ===== CRITICAL: AAGE BHEJO — controller tak =====
        chain.doFilter(request, response);
        /*
         * ============================================================
         * chain.doFilter() — SABSE IMPORTANT LINE
         *
         * YE KYA HAI: "Mera kaam ho gaya, next filter/controller ko bhejo."
         *
         * KYUN LAGAYA: Filter chain continue karna ZAROORI hai. Ye line relay race ka baton pass hai.
         *
         * NAHI CALL KIYA TO KYA HOGA:
         *   - Request YAHI ruk jaayegi — Controller tak NAHI pahunchegi
         *   - Client ko koi response NAHI milega — connection HANG ho jaayegi
         *   - Client dekhega: "Loading..." forever → timeout → "Server not responding"
         *   - DEADLY BUG: app chalu hai but koi API kaam nahi kar rahi
         *
         * REAL WORLD ANALOGY:
         *   chain.doFilter = ASSEMBLY LINE ka conveyor belt.
         *   Tera station pe kaam kiya (logging) → belt pe rakh (doFilter) → next station jaaye.
         *   Belt pe nahi rakha → product tera station pe ruk gaya → aage kuch nahi hua → delivery ZERO.
         *
         * NOTE: chain.doFilter() ke BAAD ka code TAB chalega jab response WAPAS aaye.
         *   chain.doFilter() se pehle = REQUEST path (jaate waqt)
         *   chain.doFilter() ke baad = RESPONSE path (aate waqt — wapasi pe)
         * ============================================================
         */

        // ===== RESPONSE INFO LOG (response aa gayi — wapsi ka log) =====
        long duration = System.currentTimeMillis() - startTime;
        System.out.println("Status: " + res.getStatus());           // 200, 201, 404, 500
        System.out.println("Time: " + duration + "ms");             // kitna time laga
        System.out.println("===============\n");
        /*
         * SAMPLE LOG OUTPUT:
         *   === REQUEST ===
         *   Method: GET
         *   URL: /api/students/5
         *   IP: 127.0.0.1
         *   Status: 200
         *   Time: 15ms
         *   ===============
         */
    }
}
