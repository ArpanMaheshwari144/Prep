package com.arpan.usercrud.security;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.annotation.authentication.configuration.AuthenticationConfiguration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;

/* ════════════════════════════════════════════════════════════════════
 *  👮 SecurityConfig — THE CHIEF SECURITY OFFICER (CSO)
 * ════════════════════════════════════════════════════════════════════
 *
 *  Yeh class chief security officer hai — building ki overall
 *  security policy banata. Guards ko hire karta, rules set karta,
 *  password lockers buy karta, kaunse rooms public/private decide karta.
 *
 *  ─── 🏢 ANALOGY: Office Security Headquarters ─────────────────────
 *
 *      Imagine office building security:
 *
 *      🎯 Chief Security Officer (THIS CLASS)
 *           │
 *           ├─► 🛡️ Hires guard (JwtFilter)
 *           │   └─ Tells where to stand: "Gate pe, but UPAF se pehle"
 *           │
 *           ├─► 🔐 Buys password locker (BCryptPasswordEncoder)
 *           │   └─ "Saare passwords iss locker mein hash karke rakho"
 *           │
 *           ├─► 📋 Makes the rule book (SecurityFilterChain)
 *           │   ├─ Cafeteria → public (anyone enter)
 *           │   ├─ Lab/server room → only with valid card
 *           │   └─ Server admin room → only ADMIN role
 *           │
 *           └─► 🔑 Provides AuthenticationManager
 *               └─ "Login counter pe yeh tool use karo
 *                  credentials verify karne ke liye"
 *
 *  ─── 📋 3 BEANS YEH DEFINE KARTA ──────────────────────────────────
 *
 *      1️⃣  SecurityFilterChain
 *          → main config — filter chain + route rules
 *
 *      2️⃣  PasswordEncoder
 *          → BCrypt for hashing (locker)
 *
 *      3️⃣  AuthenticationManager
 *          → login flow ka tool (AuthController use karega)
 *
 *  ─── 🚦 ROUTE RULES (kaun kahaan ja sakta) ────────────────────────
 *
 *      🟢 PUBLIC (no token needed):
 *         • /auth/login    — naya banda login karne aaye
 *         • /auth/register — naya banda signup
 *         • /auth/refresh  — token expire hua, naya lo
 *         • /h2-console/** — dev-only DB UI
 *
 *      🔴 PROTECTED (valid JWT mandatory):
 *         • /users/**      — apna data dekho/edit
 *         • Anything else  — sab JWT ke saath hi
 *
 *  ─── 🔗 FILTER CHAIN ORDER (CRITICAL) ─────────────────────────────
 *
 *      HTTP Request
 *           │
 *           ▼
 *      ┌──────────────────┐
 *      │  CORS Filter     │  ← bahar ke domain check
 *      ├──────────────────┤
 *      │  CSRF Filter     │  ← (DISABLED — JWT stateless)
 *      ├──────────────────┤
 *      │  JwtFilter ⭐    │  ← hum yahaan add kiya (THE GUARD)
 *      ├──────────────────┤
 *      │  UPAuthFilter    │  ← Spring's built-in
 *      ├──────────────────┤
 *      │  Authz Check     │  ← @PreAuthorize, hasRole()
 *      └──────────────────┘
 *           │
 *           ▼
 *       Controller
 *
 *      JwtFilter UPAF se PEHLE — taaki SecurityContext jaldi set ho,
 *      Spring's built-in auth recognize kar le 'already authenticated'.
 *
 *  ─── 🔒 BCRYPT vs SHA — Password hashing ──────────────────────────
 *
 *      ❌ SHA-256:
 *          • Fast (designed for speed)
 *          • Brute-force vulnerable (1 GHz GPU = millions/sec)
 *          • No built-in salt
 *
 *      ✅ BCrypt:
 *          • Slow BY DESIGN (cost factor — 2^cost iterations)
 *          • Brute-force impractical (millions of years)
 *          • Built-in salt (rainbow table attacks blocked)
 *          • Industry standard for passwords
 *
 *      Iron rule: passwords ke liye BCrypt always.
 *
 *  ─── 🎯 STATELESS — kya matlab? ───────────────────────────────────
 *
 *      "Spring, koi HttpSession mat banao server pe."
 *
 *      Stateful (default):                Stateless (humara):
 *      ────────────────                  ──────────────────
 *      Server memory mein:                Server memory mein:
 *      { sessionId → user }               (kuch nahi)
 *      ↓                                  ↓
 *      Crash = sessions gone              Crash = no impact
 *      Multi-server hard                  Multi-server trivial
 *                                        Token har request mein
 *                                        khud apni identity laata
 *
 *  ════════════════════════════════════════════════════════════════════
 *  🎤 INTERVIEW TALKING POINT — SecurityConfig
 *  ════════════════════════════════════════════════════════════════════
 *
 *  Q: "Spring Security mein JWT integration kaise wire karte ho?"
 *
 *  Tu bolega:
 *  "Maine `SecurityConfig` class banayi `@Configuration` aur
 *   `@EnableWebSecurity` lagayi. 3 main beans define kiye:
 *
 *   1. **SecurityFilterChain** — main config:
 *      • CSRF disable (stateless API mein zaroorat nahi)
 *      • SessionCreationPolicy.STATELESS (no HttpSession)
 *      • Public routes: /auth/** (login/register/refresh) +
 *        /h2-console/** (dev tool) → permitAll()
 *      • Baaki sab → authenticated()
 *      • addFilterBefore(jwtFilter, UsernamePasswordAuthFilter.class)
 *
 *   2. **PasswordEncoder** — BCryptPasswordEncoder bean.
 *      Industry standard — slow + salted hash, brute-force
 *      resistant.
 *
 *   3. **AuthenticationManager** — login flow ke liye exposed
 *      bean, AuthController use karta credentials verify karne mein.
 *
 *   Filter ordering critical hai — JwtFilter `UsernamePasswordAuth-
 *   enticationFilter` se PEHLE chalna chahiye, taaki SecurityContext
 *   jaldi set ho jaye."
 *
 *  Q: "CSRF disable kyu kiya?"
 *  → "Stateless JWT API hai — CSRF tokens session-based cookie
 *     attacks pe lagte. Hum cookies use nahi kar rahe, JWT use
 *     kar rahe Authorization header mein. Toh CSRF irrelevant."
 *
 *  Q: "STATELESS ka matlab?"
 *  → "Spring server side koi HttpSession nahi banayega. Har
 *     request fresh treat hoti — JWT se hi pata chalta user
 *     kaun hai. Multi-server scaling ke liye ideal."
 *
 *  Q: "BCrypt vs SHA-256 — password ke liye?"
 *  → "BCrypt always — slow + salted by design. SHA fast hai,
 *     brute-force vulnerable. Plus BCrypt mein salt built-in,
 *     SHA mein manually karna padta. Industry standard BCrypt."
 *
 *  Q: "addFilterBefore vs addFilterAfter — kyu before?"
 *  → "JwtFilter ko Spring ka built-in
 *     UsernamePasswordAuthenticationFilter chalne se PEHLE chahiye —
 *     taaki SecurityContext jaldi set ho jaye. Phir UPAF dekh ke
 *     'already authenticated' recognize kar leta, login form
 *     ke flow se hat ke aage badh jata."
 *  ════════════════════════════════════════════════════════════════════
 */
@Configuration
@EnableWebSecurity
public class SecurityConfig {

    /*
     *  🛡️ Guard inject kar rahe — Spring auto-wire karega
     *  (JwtFilter @Component hai → bean ban gaya)
     */
    private final JwtFilter jwtFilter;

    public SecurityConfig(JwtFilter jwtFilter) {
        this.jwtFilter = jwtFilter;
    }

    // ════════════════════════════════════════════════════════════
    //  1️⃣  SecurityFilterChain — MAIN SECURITY CONFIG
    // ════════════════════════════════════════════════════════════
    /*
     *  📋 Yahaan rule book likhte:
     *  • CSRF disable
     *  • Sessions stateless
     *  • Routes (public/protected)
     *  • H2 iframe support (dev)
     *  • JwtFilter chain mein add
     */
    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
            // ═══════════════════════════════════════════════════
            //  🚫 CSRF DISABLE
            // ═══════════════════════════════════════════════════
            //
            //  CSRF kya hota:
            //     "Cross-Site Request Forgery" — attacker tera browser cookie
            //     use karke malicious request bhej de tere bina pata.
            //
            //  Yeh attack TAB possible hai jab:
            //     ✓ Browser auto-send cookies
            //     ✓ Server cookies se session identify
            //
            //  Hum cookies use NAHI kar rahe — JWT use kar rahe Authorization
            //  header mein. Header auto-send nahi hota cross-origin → CSRF
            //  attack possible nahi → CSRF protection redundant.
            //
            //  Toh disable kar dete (otherwise har POST/PUT request mein
            //  CSRF token bhejna padega — useless overhead).
            .csrf(csrf -> csrf.disable())

            // ═══════════════════════════════════════════════════
            //  🌊 STATELESS SESSION POLICY
            // ═══════════════════════════════════════════════════
            //
            //  "Spring, koi HttpSession mat banao."
            //
            //  Default: Spring login pe session create karta memory mein.
            //  Hum: "Bhai, JWT use kar rahe — session ki zaroorat hi nahi.
            //        Har request token-based authenticate hogi fresh."
            //
            //  Multi-server scaling ke liye essential.
            .sessionManagement(session ->
                session.sessionCreationPolicy(SessionCreationPolicy.STATELESS)
            )

            // ═══════════════════════════════════════════════════
            //  🚦 ROUTE RULES — kaun kahaan ja sakta
            // ═══════════════════════════════════════════════════
            //
            //  🟢 Public:  /auth/** + /h2-console/**
            //  🔴 Protected: anything else (JWT mandatory)
            .authorizeHttpRequests(auth -> auth
                // 🟢 Login/register/refresh — token chahiye nahi
                //    (kyunki abhi user ke paas token hai hi nahi)
                .requestMatchers("/auth/**").permitAll()

                // 🟢 H2 console — dev only (production mein remove karenge)
                .requestMatchers("/h2-console/**").permitAll()

                // 🔴 Baaki sab — JWT mandatory
                //    (no auth → 401 Unauthorized)
                .anyRequest().authenticated()
            )

            // ═══════════════════════════════════════════════════
            //  🖼️ H2 CONSOLE IFRAME SUPPORT
            // ═══════════════════════════════════════════════════
            //
            //  H2 web console iframes use karta apne UI mein.
            //  Spring Security default X-Frame-Options DENY set karta
            //  (clickjacking se bachane ke liye).
            //
            //  Dev mein H2 console chahiye → frameOptions disable.
            //  Production mein H2 console hi nahi chahiye, so non-issue.
            .headers(headers ->
                headers.frameOptions(frame -> frame.disable())
            )

            // ═══════════════════════════════════════════════════
            //  🛡️ JWT FILTER — chain mein add karo
            // ═══════════════════════════════════════════════════
            //
            //  addFilterBefore() = "Yeh filter, iss filter ke pehle chalao"
            //
            //  Hum bol rahe: "JwtFilter, UPAF (UsernamePasswordAuthenticationFilter)
            //                 ke pehle chalao."
            //
            //  Order matter karta — JwtFilter pehle SecurityContext set kar deta,
            //  UPAF dekh ke "already authenticated" recognize kar leta.
            .addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class);

        return http.build();
    }

    // ════════════════════════════════════════════════════════════
    //  2️⃣  PasswordEncoder — BCRYPT LOCKER
    // ════════════════════════════════════════════════════════════
    /*
     *  🔐 Password locker — BCrypt encoder.
     *
     *  Usage:
     *     Register pe:  String hash = encoder.encode("plainPassword");
     *                   user.setPassword(hash);  ← DB mein hash store
     *
     *     Login pe:     boolean match = encoder.matches("plainPassword", user.getPassword());
     *
     *  Properties:
     *  • Slow by design (cost factor 10 = 2^10 iterations)
     *  • Built-in salt (each hash unique even for same password)
     *  • Brute-force impractical (millions of years)
     *
     *  Industry standard. Production code mein bas yahi use karte.
     */
    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    // ════════════════════════════════════════════════════════════
    //  3️⃣  AuthenticationManager — LOGIN VERIFICATION TOOL
    // ════════════════════════════════════════════════════════════
    /*
     *  🔑 Login counter ka tool — credentials verify karta.
     *
     *  AuthController use karega:
     *     authManager.authenticate(
     *         new UsernamePasswordAuthenticationToken(email, password)
     *     );
     *
     *  Internally Spring:
     *  1. Calls CustomUserDetailsService.loadUserByUsername(email)
     *  2. Compares input password with stored hash via PasswordEncoder
     *  3. Match? → Authentication object return
     *     Mismatch? → BadCredentialsException
     *
     *  Spring Boot 3.x mein yeh bean explicitly expose karna padta —
     *  isliye yeh `@Bean` method.
     */
    @Bean
    public AuthenticationManager authenticationManager(
            AuthenticationConfiguration config) throws Exception {
        return config.getAuthenticationManager();
    }
}
