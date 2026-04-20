package com.example.practice.security;

/*
 * ============================================================
 * YE KYA HAI: Security Config — kaunse URLs open, kaunse protected. JWT filter register. Rules define.
 * KYUN CHAHIYE: Spring Security ko batana padta ki kaun access kar sakta. Ye RULEBOOK hai.
 * KYA KARTA: URL rules define, CSRF off, session off (stateless), JWT filter register.
 * NAHI HUA TO: Spring Security ADD karte hi SAB URLs blocked. Login page aata. Kuch access nahi.
 *
 * REAL WORLD ANALOGY — YE BUILDING KA SECURITY POLICY HAI:
 *   Building (app) mein floors (APIs) hain:
 *     Ground floor (login/register) → OPEN. Koi bhi aa sakta. No pass needed.
 *     Swagger floor (API docs)      → OPEN. Documentation dekhne ke liye.
 *     Baaki sab floors (APIs)       → RESTRICTED. Valid PASS (JWT) chahiye.
 *
 *   Ye config wo POLICY hai jo guard (JwtFilter) follow karta.
 *   Policy nahi toh guard ko pata nahi kisko rokna, kisko jaane dena.
 *
 * SPRING SECURITY DEFAULT (bina config):
 *   - SAB URLs protected
 *   - Auto login page generate (HTML form)
 *   - Username: "user", Password: console mein print hota
 *   - CSRF enabled
 *   - Session based authentication
 *   → Ye REST API ke liye GALAT hai. Isliye customize karna padta.
 * ============================================================
 */

import com.example.practice.security.JwtFilter;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;

/*
 * ============================================================
 * @Configuration
 *
 * YE KYA HAI: Spring ko bolo "is class mein @Bean methods hain — objects create karo aur manage karo."
 *
 * KYUN LAGAYA: SecurityFilterChain bean banana hai → @Bean method chahiye → @Configuration class chahiye.
 *   @Bean sirf @Configuration class ke andar kaam karta (properly).
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - @Bean method Spring DETECT nahi karega → security rules apply NAHI honge
 *   - SecurityFilterChain bean nahi banega → Spring default security use karega
 *   - Default = sab blocked + login page → REST API ke liye USELESS
 *
 * REAL WORLD ANALOGY:
 *   @Configuration = ARCHITECT ka BLUEPRINT (building ka plan).
 *   Blueprint mein likha: "Ground floor open, baaki restricted" (security rules).
 *   Blueprint nahi diya → builder (Spring) default plan se banega → sab floors LOCKED.
 * ============================================================
 */
@Configuration

/*
 * ============================================================
 * @EnableWebSecurity
 *
 * YE KYA HAI: Spring Security ACTIVATE karo. Security features ON karo.
 *
 * KYUN LAGAYA: Bina iske security features OFF hain — filter chain nahi chalega, rules dead.
 *   Ye annotation Spring Security ke filters, authentication manager, CSRF setup — sab activate karta.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - SecurityFilterChain define kiya → but activate NAHI — rules DEAD
 *   - Jaise rules banaye but police deploy nahi ki → koi follow nahi karega
 *   - Security config likhi but kaam nahi kar rahi → "kyun nahi lag rahi?" — @EnableWebSecurity bhool gaye
 *
 * REAL WORLD ANALOGY:
 *   @EnableWebSecurity = SECURITY SYSTEM ka POWER BUTTON.
 *   CCTV laga diya, guards hire kiye, rules banaye → but power ON nahi kiya → sab OFF.
 *   Power ON karo (@EnableWebSecurity) → system ACTIVE → rules enforce honge.
 * ============================================================
 */
@EnableWebSecurity
public class SecurityConfig {

    /*
     * ============================================================
     * @Bean
     *
     * YE KYA HAI: Is method ka return object = Spring managed bean. Spring isko ApplicationContext mein rakhega.
     *
     * KYUN LAGAYA: SecurityFilterChain object Spring ko dena hai — wo apply karega sab requests pe.
     *
     * NAHI LAGAYA TO KYA HOGA:
     *   - Security rules define kiye but Spring ko PATA NAHI — kuch apply nahi hoga
     *   - Method sirf ek normal method rahega — Spring usse bean nahi banayega
     *   - Default security chalegi = sab blocked
     *
     * @Bean vs @Component:
     *   @Component = CLASS pe lagao → Spring us class ka object banaye
     *   @Bean = METHOD pe lagao → method ka return value = bean
     *   Third-party class ka bean banana ho (class modify nahi kar sakte) → @Bean use karo
     *   Apni class hai → @Component lagao. Kisi aur ki class hai → @Bean se wrap karo.
     * ============================================================
     */
    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        /*
         * HttpSecurity = security rules configure karne ka builder.
         * Spring inject karta — tu new nahi karta. Builder pattern se rules chain karo.
         */
        http
            /*
             * ============================================================
             * csrf.disable() — CSRF Protection OFF
             *
             * YE KYA HAI: Cross-Site Request Forgery protection OFF karo.
             *
             * KYUN OFF KIYA (REST API ke liye):
             *   CSRF attack TAB hota jab browser COOKIE auto-send karta.
             *   Attacker fake website banata → user us pe click kare → browser automatically
             *   teri site ka cookie bhejta → attacker ke request mein tera session jaata → ATTACK!
             *
             *   REST API mein: JWT header mein jaata (Authorization header).
             *   Browser Authorization header auto-send NAHI karta (cookies karta hai, header nahi).
             *   → CSRF attack POSSIBLE HI NAHI → protection ki zaroorat NAHI → OFF karo.
             *
             * NAHI OFF KIYA TO KYA HOGA:
             *   - POST/PUT/DELETE requests pe 403 Forbidden error — "CSRF token missing"
             *   - Postman se request bhejoge → 403 → "kya hua?"
             *   - Client ko har request mein CSRF token bhejni padegi — REST API ke liye OVERKILL
             *
             * REAL WORLD ANALOGY:
             *   CSRF protection = umbrella (chhaata).
             *   Baarish ho rahi (cookie-based auth) → chhaata CHAHIYE (CSRF ON).
             *   Dhoop hai (JWT-based auth, no cookies) → chhaata band karo (CSRF OFF) — unnecessary burden.
             * ============================================================
             */
            .csrf(csrf -> csrf.disable())

            /*
             * ============================================================
             * SessionCreationPolicy.STATELESS — Session OFF
             *
             * YE KYA HAI: Server pe SESSION store mat karo. Har request independent hai.
             *
             * KYUN LAGAYA (JWT based app ke liye):
             *   JWT mein sab info hai (username, expiry, signature).
             *   Server pe STATE rakhne ki zaroorat NAHI — token verify karo, DONE.
             *   = STATELESS. No server memory waste. Horizontally scalable.
             *
             * NAHI LAGAYA TO KYA HOGA:
             *   - Spring session CREATE karega har user ke liye — server memory waste
             *   - Multiple servers hain → user server-1 pe login → request server-2 pe gaya →
             *     server-2 pe session NAHI → "kaun ho tum?" → FAIL
             *   - JWT ka poora point WASTE — token bhi hai, session bhi hai → confusion
             *
             * REAL WORLD ANALOGY:
             *   SESSION BASED (purana) = HOTEL mein room booking.
             *     Check-in → room assign (session create) → room number yaad rakh (JSESSIONID cookie)
             *     PROBLEM: 10 hotels mein room booking → kisi hotel mein jaao → "aapka room nahi hai!"
             *     SOLUTION: Ek master list (session replication) → EXPENSIVE + COMPLEX
             *
             *   JWT BASED (naya, STATELESS) = MOVIE TICKET.
             *     Ticket liya → kisi bhi screen pe dikhaao → ticket pe sab info → VERIFY → entry.
             *     Server pe kuch yaad rakhna nahi → 10 screens (servers) mein kahi bhi jaao → WORKS.
             * ============================================================
             */
            .sessionManagement(session ->
                session.sessionCreationPolicy(SessionCreationPolicy.STATELESS))

            .authorizeHttpRequests(auth -> auth
                /*
                 * ============================================================
                 * permitAll() — Open endpoints (bina JWT access)
                 *
                 * /api/auth/** = login/register → OPEN.
                 * KYUN: Login karne ke liye JWT chahiye, JWT ke liye login chahiye — DEADLOCK!
                 *   Isliye auth endpoints OPEN rakhte — token lene ke liye.
                 * NAHI LAGAYA TO: Login bhi protected → login ke liye token chahiye →
                 *   token ke liye login chahiye → INFINITE LOOP → koi login NAHI kar payega!
                 *
                 * ANALOGY: Gate ke andar pass milta hai, but gate pe bhi pass chahiye → andar kaise jaaye?
                 *   Solution: Pass counter BAHAR rakho (permitAll) → pehle pass lo → phir andar jaao.
                 * ============================================================
                 */
                .requestMatchers("/api/auth/**").permitAll()

                /*
                 * Swagger OPEN — API documentation bina JWT dekh sakte.
                 * /swagger-ui/** = Swagger UI page (visual interface)
                 * /v3/api-docs/** = OpenAPI spec JSON (raw API specification)
                 * NAHI LAGAYA TO: API docs dekhne ke liye bhi JWT chahiye → developer friendly nahi.
                 */
                .requestMatchers("/swagger-ui/**", "/v3/api-docs/**").permitAll()

                /*
                 * ============================================================
                 * .anyRequest().authenticated() — BAAKI SAB PROTECTED
                 *
                 * YE KYA HAI: Jo URLs upar permitAll mein nahi hain → sab ke liye authentication ZAROORI.
                 *
                 * KYUN LAGAYA: Protected APIs pe sirf logged-in users access karein.
                 *   /api/students/** → authenticated() → JwtFilter mein SecurityContext set hona chahiye.
                 *
                 * NAHI LAGAYA TO KYA HOGA:
                 *   - Sab URLs OPEN → koi bhi bina login ke student create/delete kar le
                 *   - Security ka koi matlab nahi → JWT filter laga ke bhi kya fayda
                 *
                 * ANALOGY: Building mein sirf ground floor open, baaki sab floors pe PASS chahiye.
                 *   .authenticated() = wo PASS CHECK hai. Bina pass → 403 Forbidden.
                 * ============================================================
                 */
                .anyRequest().authenticated()
            )

            /*
             * ============================================================
             * addFilterBefore — JWT Filter Register
             *
             * YE KYA HAI: Hamara JwtFilter, Spring ke default filter se PEHLE lagao.
             *
             * KYUN LAGAYA: Request aaye → PEHLE JWT check (hamara filter) → phir Spring security check.
             *   JwtFilter SecurityContext set karega → Spring dekhega "authenticated hai" → OK pass.
             *   Agar Spring ka filter PEHLE chale → SecurityContext empty → "not authenticated" → BLOCK.
             *
             * NAHI LAGAYA TO KYA HOGA:
             *   - JwtFilter register NAHI hua → token koi CHECK nahi karega
             *   - .authenticated() pe SAB requests FAIL → 403 sab pe (valid token bhi)
             *   - JWT system poora USELESS — token banaya, bheja, but koi verify nahi kar raha
             *
             * REAL WORLD ANALOGY:
             *   addFilterBefore = guard ko gate pe PEHLE khada karo.
             *   Guard PEHLE check kare → sahi hai → aage jaane do → receptionist (Spring filter) confirm kare.
             *   Guard ko peeche rakha → receptionist pehle check kare → "pass dikhaao" → "guard ne abhi check nahi kiya" → BLOCK.
             *
             * FILTER ORDER:
             *   Request → ... → JwtFilter (hamara, PEHLE) → UsernamePasswordAuthFilter (Spring) → Controller
             *   "Before" = "isse PEHLE lagao"
             * ============================================================
             */
            .addFilterBefore(new JwtFilter(), UsernamePasswordAuthenticationFilter.class);

        return http.build();
        // Builder pattern — rules set kiye → build() → SecurityFilterChain object return → Spring apply karega
    }
}
