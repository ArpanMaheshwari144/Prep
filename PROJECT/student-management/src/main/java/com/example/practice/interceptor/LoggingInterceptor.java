package com.example.practice.interceptor;

/*
 * ============================================================
 * YE KYA HAI: Logging Interceptor — controller se pehle aur baad mein log. Sirf API requests pe.
 * KYUN CHAHIYE: Filter se zyada FINE-GRAINED control. Sirf specific URLs pe laga sakte.
 * KYA KARTA: preHandle (request aaye) → log. afterCompletion (response jaaye) → log.
 * NAHI HUA TO: Sirf Filter hai — sab requests log (images, CSS bhi). Specific API logging nahi.
 *
 * REAL WORLD ANALOGY — YE FLOOR KA RECEPTIONIST HAI:
 *   Filter = building ka main gate guard (SAB visitors log — chahe kisi bhi floor pe jaaye)
 *   Interceptor = specific floor ka receptionist (SIRF us floor ke visitors log)
 *
 *   Main gate guard: CSS file aaye → log. Image aaye → log. API aaye → log. SAB.
 *   Floor receptionist: sirf /api/** wale visitors → log. CSS/image IGNORE.
 *
 *   Isliye DONO chahiye:
 *     Filter → broad logging (sab traffic dekhna ho)
 *     Interceptor → targeted logging (sirf API traffic dekhna ho)
 *
 * FILTER vs INTERCEPTOR DIFFERENCE:
 *   Filter:       Servlet API. Tomcat level. doFilter() ek method. SAB requests pe.
 *   Interceptor:  Spring MVC. Controller level. 3 methods (pehle, baad, final). Sirf controller URLs pe.
 *     preHandle()       — Controller se PEHLE (request aa rahi)
 *     postHandle()      — Controller ke BAAD, view render se pehle (REST mein rarely use)
 *     afterCompletion() — SAB ke baad. Response bhi gaya. Cleanup/final logging.
 *
 *   Interceptor ka BONUS: Handler object milta — kaunsa controller method call hoga pata chalta.
 *
 * REQUEST PATH:
 *   Client → Tomcat → [FILTER] → DispatcherServlet → [INTERCEPTOR] → Controller
 * ============================================================
 */

import jakarta.servlet.http.*;
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.HandlerInterceptor;

/*
 * ============================================================
 * @Component
 *
 * YE KYA HAI: Spring bean banao — BUT interceptor AUTO REGISTER NAHI hota!
 *
 * KYUN LAGAYA: Spring bean chahiye toh @Component lagana padega.
 *   WebConfig mein @Autowired se inject hoga → phir manually register hoga.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - WebConfig mein @Autowired LoggingInterceptor → NULL → NullPointerException
 *   - Ya WebConfig mein manually new LoggingInterceptor() banana padega
 *
 * IMPORTANT DIFFERENCE (Filter vs Interceptor registration):
 *   Filter:       @Component lagao = AUTO register in Tomcat (done, kuch aur nahi chahiye)
 *   Interceptor:  @Component lagao = bean bana BUT register NAHI hoga!
 *                 WebConfig mein MANUALLY register karna padta:
 *                   registry.addInterceptor(loggingInterceptor).addPathPatterns("/api/**")
 *                 Bina WebConfig: @Component lagaya but interceptor DEAD — koi request pe nahi chalega.
 *
 * REAL WORLD ANALOGY:
 *   @Component = receptionist ko HIRE kiya (bean bana).
 *   BUT usko FLOOR ASSIGN nahi kiya (WebConfig mein register nahi kiya) → wo lobby mein baitha, koi kaam nahi.
 *   WebConfig = "tum 3rd floor pe baithoge, sirf /api/ wale visitors handle karo" — ab kaam shuru.
 * ============================================================
 */
@Component
public class LoggingInterceptor implements HandlerInterceptor {
    /*
     * ============================================================
     * implements HandlerInterceptor
     *
     * YE KYA HAI: Spring MVC ka interface. Controller level pe 3 hook points deta.
     *
     * KYUN LAGAYA: Controller ke aas paas (pehle + baad) custom logic lagana hai (logging).
     *
     * NAHI LAGAYA TO (HandlerInterceptor implement nahi kiya):
     *   - Ye ek normal class hai — interceptor nahi
     *   - Spring isko interceptor chain mein nahi daalega
     *   - preHandle/afterCompletion ka koi meaning nahi
     *
     * 3 HOOK POINTS:
     *   preHandle()       → Controller PEHLE (gate pe — request aa rahi)
     *   postHandle()      → Controller BAAD, view PEHLE (REST mein rarely use)
     *   afterCompletion() → SAB BAAD (response bhi gaya — final cleanup/logging)
     *
     * REAL WORLD ANALOGY:
     *   HandlerInterceptor = restaurant mein WAITER ke 3 steps:
     *   preHandle = "Welcome sir, order lein?" (customer aaye)
     *   postHandle = "Khaana ready, garnish karein?" (dish bani, serve se pehle)
     *   afterCompletion = "Bill de dein, table saaf karein" (customer ja raha)
     * ============================================================
     */

    // ==========================================
    // Controller se PEHLE — request aa rahi
    // ==========================================
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) {
        /*
         * ============================================================
         * preHandle()
         *
         * YE KYA HAI: Controller method call hone se PEHLE chalta.
         *
         * KYUN LAGAYA: Request log, auth check, rate limiting — controller se pehle karna ho.
         *
         * RETURN VALUE — CRITICAL:
         *   return true  = JAANE DO — request controller tak pahunche.
         *   return false = BLOCK — yahi rok do. Controller call NAHI hoga. Request DEAD.
         *
         * NAHI LAGAYA TO (false return kiya galti se):
         *   - SAB requests BLOCK — controller tak koi nahi pahunchega
         *   - Client ko empty response ya error milega — "server working but nothing happens"
         *
         * REAL WORLD ANALOGY:
         *   preHandle = restaurant mein waiter pehle pooche:
         *   "Sir reservation hai?" → true → table pe le jaao (controller tak jaane do)
         *   "Nahi hai" → false → "Sorry full hai, bahar jaao" (request BLOCK)
         *
         * handler = kaunsa controller method call hoga (HandlerMethod object)
         *   Real use: ((HandlerMethod) handler).getMethod().getName() → "getStudentById"
         * ============================================================
         */
        System.out.println("[PRE] " + request.getMethod() + " " + request.getRequestURI());
        request.setAttribute("startTime", System.currentTimeMillis());
        /*
         * request.setAttribute() — request ke saath data attach karo.
         * Start time yahan save → afterCompletion mein nikalo → duration calculate.
         * Ye request scope mein hai — sirf is request tak valid. Doosri request ko nahi milega.
         *
         * ANALOGY: Waiter ne notepad pe time likha "customer aaya 2:30pm" → bill pe duration likhega.
         */
        return true;   // true = aage jaane do. FALSE kiya toh request BLOCK — controller DEAD.
    }

    // ==========================================
    // Controller ke BAAD — response ja rahi
    // ==========================================
    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response,
                                Object handler, Exception ex) {
        /*
         * ============================================================
         * afterCompletion()
         *
         * YE KYA HAI: SAB ke baad chalta — controller done, response ready.
         *
         * KYUN LAGAYA: Final logging, cleanup, metrics collect. "Total kitna time laga" calculate.
         *
         * KAB CHALTA: Exception aaye ya na aaye — HAMESHA chalta (finally block jaisa).
         *
         * NAHI LAGAYA TO (method override nahi kiya):
         *   - Final logging nahi hoga — response time pata nahi chalega
         *   - Cleanup nahi hoga — resources leak ho sakte
         *
         * Exception ex parameter:
         *   ex == null → sab theek gaya — happy path.
         *   ex != null → controller mein exception aaya — log karo, alert bhejo.
         *
         * REAL WORLD ANALOGY:
         *   afterCompletion = restaurant mein BILL + CLEANUP.
         *   Customer kha ke gaya → bill generate (duration log) → table saaf (cleanup).
         *   Customer ne complaint ki (exception) → manager ko bhi batao (ex != null → log).
         * ============================================================
         */
        long startTime = (long) request.getAttribute("startTime");
        long duration = System.currentTimeMillis() - startTime;
        System.out.println("[POST] " + request.getMethod() + " " + request.getRequestURI()
                + " → " + response.getStatus() + " (" + duration + "ms)");
        /*
         * SAMPLE OUTPUT:
         *   [PRE] GET /api/students/5
         *   [POST] GET /api/students/5 → 200 (12ms)
         *
         *   [PRE] GET /api/students/999
         *   [POST] GET /api/students/999 → 404 (3ms)
         */
    }
}
