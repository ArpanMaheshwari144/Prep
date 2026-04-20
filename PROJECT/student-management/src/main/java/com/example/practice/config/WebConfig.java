package com.example.practice.config;

/*
 * ============================================================
 * YE KYA HAI: Web Config — Interceptor register karo. Kaunse URLs pe lagega define.
 * KYUN CHAHIYE: Interceptor banaya toh Spring ko batana padega KAB aur KAHAN chalana hai.
 * KYA KARTA: LoggingInterceptor ko /api/** URLs pe register karta.
 * NAHI HUA TO: Interceptor class banayi → @Component lagaya → BUT Spring use nahi karega.
 *              Interceptor DEAD padha rahega. Koi request pe nahi chalega.
 *
 * REAL WORLD ANALOGY:
 *   Interceptor = CCTV camera (hardware ready — class banayi)
 *   WebConfig   = camera ko specific floor pe INSTALL karna (wiring + placement)
 *   Camera banaya lekin install nahi kiya → kuch record nahi hoga.
 *   WebConfig = installation. Kahan lagana hai, kaunse areas cover karne hain.
 *
 * IMPORTANT DIFFERENCE:
 *   Filter mein ye step NAHI chahiye — @Component lagao, Tomcat mein auto register.
 *   Interceptor mein ye step ZAROORI — @Component + WebConfig dono chahiye.
 * ============================================================
 */

import com.example.practice.interceptor.LoggingInterceptor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.*;

/*
 * ============================================================
 * @Configuration
 *
 * YE KYA HAI: Spring configuration class — settings/beans define karo.
 *
 * KYUN LAGAYA: WebMvcConfigurer ke methods override karke Spring MVC customize karna hai.
 *   @Configuration lagaya toh Spring isko config class samjhega → methods call karega.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Spring isko normal class samjhega — config class nahi
 *   - addInterceptors() method call NAHI hoga — interceptor register nahi
 *   - Interceptor DEAD — WebConfig banayi but kaam nahi kar rahi
 *
 * REAL WORLD ANALOGY:
 *   @Configuration = ARCHITECT ka LICENSE.
 *   License hai → building plan (config) accept hoga.
 *   License nahi → "ye toh koi random aadmi ka plan hai, ignore karo."
 * ============================================================
 */
@Configuration
public class WebConfig implements WebMvcConfigurer {
    /*
     * ============================================================
     * implements WebMvcConfigurer
     *
     * YE KYA HAI: Spring MVC customize karne ka interface. Default settings badal sakte ho.
     *
     * KYUN LAGAYA: Interceptors add karne ke liye addInterceptors() method override karna hai.
     *
     * NAHI LAGAYA TO (WebMvcConfigurer implement nahi kiya):
     *   - addInterceptors() method ka koi meaning nahi — Spring call nahi karega
     *   - Interceptor register ka tarika hi nahi bacha
     *
     * KYA KYA CUSTOMIZE KAR SAKTE:
     *   addInterceptors()     — interceptors register (hum ye kar rahe)
     *   addCorsMappings()     — CORS rules (cross-origin requests allow/deny)
     *   addResourceHandlers() — static files serve (CSS, JS, images)
     *   configureViewResolvers() — view engine set (Thymeleaf, JSP)
     *
     * REAL WORLD ANALOGY:
     *   WebMvcConfigurer = building ka CUSTOMIZATION MENU.
     *   "Kaunsa guard kahan lagana?" → addInterceptors()
     *   "Kaunse bahar ke log aa sakte?" → addCorsMappings()
     *   Spring MVC ki default settings badalne ka ek HI jagah.
     * ============================================================
     */

    /*
     * @Autowired — Spring inject karega LoggingInterceptor bean (@Component wala).
     * NAHI LAGAYA TO: loggingInterceptor = null → addInterceptor mein NullPointerException.
     */
    @Autowired
    private LoggingInterceptor loggingInterceptor;

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        /*
         * ============================================================
         * addInterceptors() + addPathPatterns()
         *
         * YE KYA HAI: Interceptor register karo aur batao kaunse URLs pe chale.
         *
         * KYUN LAGAYA: LoggingInterceptor ko sirf /api/** URLs pe chalana hai.
         *   CSS, images, swagger pe nahi chalana — sirf API endpoints pe.
         *
         * NAHI LAGAYA TO (addInterceptors override nahi kiya):
         *   - Interceptor bean hai but chain mein nahi → DEAD
         *   - preHandle/afterCompletion KABHI call nahi honge
         * ============================================================
         */
        registry.addInterceptor(loggingInterceptor)
                .addPathPatterns("/api/**");
        /*
         * ============================================================
         * addPathPatterns("/api/**") — KAUNSE URLs pe interceptor chale
         *
         * YE KYA HAI: URL pattern define karo — matching URLs pe interceptor ACTIVE.
         *
         * KYUN LAGAYA: Sirf API URLs pe logging chahiye, static resources pe nahi.
         *
         * NAHI LAGAYA TO (pattern nahi diya):
         *   Default = SAB URLs pe chalega ("/**") — CSS, images, swagger sab pe logging
         *   Unnecessary logs = noise = debugging HARD
         *
         * URL MATCHING:
         *   /api/students     → INTERCEPT (match)
         *   /api/students/5   → INTERCEPT (match)
         *   /api/auth/login   → INTERCEPT (match)
         *   /swagger-ui.html  → SKIP (api nahi hai — match nahi)
         *   /css/style.css    → SKIP (match nahi)
         *
         * "/**" lagao toh SAB URLs pe chalega.
         * .excludePathPatterns("/api/auth/**") — specific URLs EXCLUDE bhi kar sakte.
         *
         * REAL WORLD ANALOGY:
         *   addPathPatterns = CCTV camera ka ANGLE set karna.
         *   "/api/**" = camera sirf lobby (API area) cover kare.
         *   "/**" = camera poora building cover kare (overkill).
         *   Exclude = "parking area mat cover karo" (auth endpoints skip).
         *
         * MULTIPLE INTERCEPTORS example:
         *   registry.addInterceptor(authInterceptor).addPathPatterns("/admin/**");
         *   registry.addInterceptor(loggingInterceptor).addPathPatterns("/api/**");
         *   Order: jis order mein add kiya, usi order mein chalenge.
         * ============================================================
         */
    }
}
