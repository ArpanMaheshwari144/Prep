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

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    SecurityConfig = CHIEF SECURITY OFFICER (CSO)
//    Overall security policy banata
//    3 beans define karta
//    Routes + filter chain wire karta
// ═══════════════════════════════════════════════════════════════════════
//
// CSO ROLE:
//    SecurityConfig
//         │
//         ├─► Guard hire (JwtFilter)
//         │   "Gate pe, UPAF se pehle"
//         │
//         ├─► Locker buy (BCryptPasswordEncoder)
//         │   "Saare passwords iss locker mein hash karke rakho"
//         │
//         ├─► Rule book (SecurityFilterChain)
//         │   ├─ /auth/** → public
//         │   ├─ /h2-console → public
//         │   └─ Baki → JWT mandatory
//         │
//         └─► Verification tool (AuthenticationManager)
//             "Login counter pe yeh use karo"
//
// 3 BEANS THIS CLASS DEFINES:
//    1. SecurityFilterChain   → main rule book (chain + routes)
//    2. PasswordEncoder        → BCrypt locker
//    3. AuthenticationManager  → login verification tool
//
// SecurityFilterChain SUB-CONFIGS:
//    1. csrf().disable()       → JWT stateless, no cookies, CSRF irrelevant
//    2. STATELESS sessions     → "Spring, no HttpSession" (multi-server safe)
//    3. Route rules:
//         /auth/**    → public (login/register/refresh)
//         /h2-console → public (dev only)
//         anything   → authenticated()
//    4. frameOptions disable   → H2 console iframes work
//    5. addFilterBefore        → JwtFilter UPAF se PEHLE chale
//
// WHY CSRF DISABLED?
//    CSRF = Cross-Site Request Forgery
//       Designed for cookie-based sessions
//
//    Hum cookies use NAHI kar rahe
//    JWT in Authorization header
//    Header auto-send nahi hota cross-origin
//    = CSRF attack impossible
//    = Disable safe
//
// WHY STATELESS?
//    Stateful (default):     Stateless (humara):
//       Server: { session }     Server: nothing
//       Crash = data gone       Crash = no impact
//       Multi-server hard       Multi-server easy
//
//    SessionCreationPolicy.STATELESS:
//       "Spring, koi HttpSession mat banao"
//       Multi-server scaling = essential
//
// JwtFilter ORDER (CRITICAL):
//    addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class)
//    = "Yeh filter, iss filter ke pehle chalao"
//
//    Order matter karta:
//       JwtFilter PEHLE chale
//       → SecurityContext set ho
//       → UPAF dekhke "already authenticated" recognize
//       → login form flow se hat ke aage badh jata
//
// FILTER CHAIN FINAL ORDER:
//    HTTP Request
//         │
//         ▼
//    CORS → CSRF(disabled) → JwtFilter → UPAF → Authz → Controller
//                              │
//                              └ addFilterBefore() ne yahaan rakha
//
// BCRYPT vs SHA-256:
//    SHA-256:
//       Fast (designed for speed)
//       Brute-force vulnerable (millions/sec on GPU)
//       No built-in salt
//
//    BCrypt:
//       Slow BY DESIGN (cost factor — 2^cost iterations)
//       Brute-force impractical (millions of years)
//       Built-in salt (rainbow tables blocked)
//       Industry standard for passwords
//
// PasswordEncoder Usage:
//    Register: encoder.encode("plain") → "$2a$..." (hash) → DB store
//    Login:    encoder.matches("plain", hashFromDB) → true/false
//
// AuthenticationManager Usage (AuthController will use):
//    authManager.authenticate(
//        new UsernamePasswordAuthenticationToken(email, password)
//    );
//
//    Spring internally:
//       1. loadUserByUsername(email) — CustomUserDetailsService
//       2. encoder.matches(input, stored) — PasswordEncoder
//       3. Match? → Authentication object
//          Mismatch? → BadCredentialsException
//
//    Spring Boot 3.x: explicit bean expose required
//
// ANNOTATIONS MAGIC:
//    @Configuration   → "Yeh class beans define karegi"
//    @EnableWebSecurity → "Spring Security activate karo"
//                          Filter chain processing enabled
//
// SOLID:
//    SRP — Sirf security configuration (filter chain, beans wire)
//          No business logic
//
//    DIP — JwtFilter inject (constructor)
//          Uses Spring's abstractions
//          (AuthenticationManager, PasswordEncoder interfaces)
//
//    OCP — Naye filters add ho sakte
//          .addFilterBefore() / .addFilterAfter()
//          Existing config extension friendly
//
// INTERVIEW LINE:
//    "SecurityConfig wires Spring Security with JWT:
//       • CSRF disabled, STATELESS sessions
//       • /auth/** public, rest authenticated
//       • JwtFilter added BEFORE UPAuthenticationFilter
//       • BCrypt encoder + AuthenticationManager beans
//
//     Filter ordering critical — JwtFilter sets SecurityContext early,
//     Spring's built-in filters recognize 'already authenticated'."
// ═══════════════════════════════════════════════════════════════════════

@Configuration
@EnableWebSecurity
public class SecurityConfig {

    // Guard inject kar rahe — Spring auto-wire karega
    // (JwtFilter @Component hai → bean ban gaya)
    private final JwtFilter jwtFilter;

    public SecurityConfig(JwtFilter jwtFilter) {
        this.jwtFilter = jwtFilter;
    }

    // ════════════════════════════════════════════════════════════
    //  1  SecurityFilterChain — MAIN SECURITY CONFIG
    // ════════════════════════════════════════════════════════════
    //  Yahaan rule book likhte:
    //  • CSRF disable
    //  • Sessions stateless
    //  • Routes (public/protected)
    //  • H2 iframe support (dev)
    //  • JwtFilter chain mein add
    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
            // CSRF DISABLE — JWT stateless, cookies nahi, CSRF irrelevant
            .csrf(csrf -> csrf.disable())

            // STATELESS — "Spring, koi HttpSession mat banao"
            //    Multi-server scaling ke liye essential
            .sessionManagement(session ->
                session.sessionCreationPolicy(SessionCreationPolicy.STATELESS)
            )

            // ROUTE RULES
            .authorizeHttpRequests(auth -> auth
                // Login/register/refresh — token chahiye nahi
                .requestMatchers("/auth/**").permitAll()
                // H2 console — dev only (production mein remove)
                .requestMatchers("/h2-console/**").permitAll()
                // Baki sab — JWT mandatory
                .anyRequest().authenticated()
            )

            // H2 console iframe support (dev only)
            // Spring default X-Frame-Options DENY — H2 ke liye disable
            .headers(headers ->
                headers.frameOptions(frame -> frame.disable())
            )

            // JwtFilter ko UPAF se PEHLE chain mein
            //
            // JWT app mein request aaya — kaun verify kare?
            //    JwtFilter (naya, JWT-aware)  vs  UPAF (Spring default, form-based)
            //
            // Order matters:
            //
            //    PEHLE JwtFilter chala:
            //       Header se token nikala → valid? → SecurityContext mein authentication SET
            //       → UPAF aaya, dekha "ho gaya" → skip → controller
            //
            //    PEHLE UPAF chala hota (galat):
            //       Form mein username/password chahta → JWT app mein form hota nahi
            //       → reject 401 → JwtFilter ko mauka hi nahi
            //
            // = JWT setup mein JwtFilter MUST be before UPAF.
            .addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class);

        return http.build();
    }

    // ════════════════════════════════════════════════════════════
    //  2  PasswordEncoder — BCRYPT LOCKER
    // ════════════════════════════════════════════════════════════
    //  BCrypt encoder bean — industry standard
    //
    //  Usage:
    //     Register pe:  String hash = encoder.encode("plainPassword");
    //                   user.setPassword(hash);  ← DB hash store
    //     Login pe:     boolean match = encoder.matches("plain", user.getPassword());
    //
    //  Properties:
    //  • Slow by design (cost factor 10 = 2^10 iterations)
    //  • Built-in salt (each hash unique even for same password)
    //  • Brute-force impractical (millions of years)
    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    // ════════════════════════════════════════════════════════════
    //  3  AuthenticationManager — LOGIN VERIFICATION TOOL
    // ════════════════════════════════════════════════════════════
    //  Login counter ka tool — credentials verify karta
    //
    //  AuthController use karega:
    //     authManager.authenticate(
    //         new UsernamePasswordAuthenticationToken(email, password)
    //     );
    //
    //  Internally Spring:
    //  1. Calls CustomUserDetailsService.loadUserByUsername(email)
    //  2. Compares input password with stored hash via PasswordEncoder
    //  3. Match? → Authentication object
    //     Mismatch? → BadCredentialsException
    //
    //  Spring Boot 3.x mein yeh bean explicitly expose karna padta
    @Bean
    public AuthenticationManager authenticationManager(
            AuthenticationConfiguration config) throws Exception {
        return config.getAuthenticationManager();
    }
}
