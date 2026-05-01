package com.arpan.usercrud.security;

import java.io.IOException;

import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.web.authentication.WebAuthenticationDetailsSource;
import org.springframework.stereotype.Component;
import org.springframework.web.filter.OncePerRequestFilter;

import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/* ════════════════════════════════════════════════════════════════════
 *  🛡️ JwtFilter — THE SECURITY GUARD AT BUILDING GATE
 * ════════════════════════════════════════════════════════════════════
 *
 *  Yeh class poore application ka GATE GUARD hai. Har request ko
 *  building ke andar enter karne se PEHLE rokkar check karta —
 *  "ID card dikha (token), valid hai? OK enter."
 *
 *  ─── 🏢 ANALOGY: Office Building Security ─────────────────────────
 *
 *      Tu Konovo office jata hai roz:
 *
 *         🚶 You              🛡️ Guard           🏢 Office
 *         (request)          (JwtFilter)        (controller)
 *            │                   │                  │
 *            │   "ID card?"      │                  │
 *            │ ──────────────►   │                  │
 *            │                   │ checks card      │
 *            │                   │ • photo match?   │
 *            │                   │ • expiry?        │
 *            │                   │ • valid stamp?   │
 *            │                   │                  │
 *            │   ✅ Pass          │                  │
 *            │ ◄──────────────   │                  │
 *            │                   │                  │
 *            │   Walk in →       │ ───────────────►│
 *            │                                       │
 *            │                              "Welcome Arpan"
 *
 *      Bina ID = guard reject karta — "tu kaun hai?"
 *      Galat ID = guard pakad leta — "yeh card galat lagta"
 *      Sahi ID = guard log book mein entry karta + andar bhejta
 *                "Arpan andar gaya, role: senior dev"
 *
 *  ─── 📋 5 KAAM JO YEH FILTER KARTA ────────────────────────────────
 *
 *      1️⃣  ID card mango           → header se "Bearer <token>" nikalo
 *      2️⃣  Card valid hai check     → JwtService.isValid()
 *      3️⃣  Card pe naam padho       → token se email extract
 *      4️⃣  Office HR se confirm     → CustomUserDetailsService load
 *      5️⃣  Log book mein entry      → SecurityContextHolder set
 *
 *  ─── 🔧 extends OncePerRequestFilter ──────────────────────────────
 *
 *  Spring ka abstract class. Guarantee deta — guard sirf EK BAAR
 *  check karega per visit (request). Same banda ko 5 baar gate pe
 *  rokne se bachata.
 *
 *      ❌ Plain Filter:  "ID card?... ID card?... ID card?..."
 *                        (har layer pe re-check)
 *      ✅ OncePerReq:    "ID card? ✅ aage badh"
 *                        (single check per request)
 *
 *  ─── 🔗 FILTER CHAIN MEIN PLACEMENT ───────────────────────────────
 *
 *  HTTP Request
 *      │
 *      ▼
 *   ┌──────────────┐
 *   │  CORS Filter │   ← bahar ke domain check (allowed?)
 *   ├──────────────┤
 *   │  CSRF Filter │   ← (disabled in our config, JWT stateless)
 *   ├──────────────┤
 *   │  JwtFilter ⭐│   ← yeh hum hain (THE GUARD)
 *   ├──────────────┤
 *   │  UPAuthFilter│   ← Spring's built-in (form login se related)
 *   ├──────────────┤
 *   │  Authz Check │   ← @PreAuthorize, hasRole() etc.
 *   └──────────────┘
 *      │
 *      ▼
 *   Controller (logic)
 *
 *  ─── 📜 "Bearer " prefix kyu? ─────────────────────────────────────
 *
 *  RFC 6750 standard hai — universal convention:
 *      Authorization: Bearer eyJhbGciOiJIUzI1NiI...
 *
 *  "Bearer" matlab "iska bearer (carrier) authorized hai".
 *  Spring + AWS + every modern API yahi follow karte.
 *
 *  ─── 🧠 SecurityContextHolder kya hai? ────────────────────────────
 *
 *  Imagine: Office mein ek WHITEBOARD hai jisme likha hota
 *           "Right now this person is logged in: Arpan, role:ADMIN"
 *
 *  Thread-local hai — har request ka apna whiteboard. Downstream
 *  code (controller, @PreAuthorize) iska use karke pata karta
 *  "user kaun hai abhi".
 *
 *      Without filter: whiteboard EMPTY → "anonymous user"
 *      With filter:    whiteboard SET   → "Arpan, ADMIN"
 *
 *  ════════════════════════════════════════════════════════════════════
 *  🎤 INTERVIEW TALKING POINT — JwtFilter
 *  ════════════════════════════════════════════════════════════════════
 *
 *  Q: "JWT validate karne ka flow code-level pe explain karo"
 *
 *  Tu bolega:
 *  "Maine `JwtFilter` banaya `OncePerRequestFilter` extend karke —
 *   guarantee hai ki har request pe sirf ek baar chalega.
 *
 *   Flow yeh hai:
 *   1. Request aati → `Authorization` header check —
 *      'Bearer ' prefix nahi mila? skip karke chain continue
 *      (public endpoints jaise /auth/login ke liye).
 *   2. Token nikaalta `substring(7)` se ('Bearer ' = 7 chars).
 *   3. `JwtService.isValid()` se signature + expiration verify.
 *   4. Token se email extract karta `extractEmail()` se.
 *   5. `CustomUserDetailsService.loadUserByUsername(email)` se
 *      Spring's UserDetails object load.
 *   6. `UsernamePasswordAuthenticationToken` banata aur
 *      `SecurityContextHolder.getContext().setAuthentication()`
 *      se thread-local context mein set kar deta.
 *   7. Filter chain continue karta — downstream controllers
 *      ko ab pata chal jata user kaun hai, `@PreAuthorize`
 *      bhi kaam karta.
 *
 *   `SecurityConfig` mein iss filter ko `addFilterBefore()` se
 *   `UsernamePasswordAuthenticationFilter` se PEHLE add kiya —
 *   taaki SecurityContext jaldi set ho aur Spring's built-in
 *   auth recognize kar le 'already authenticated'."
 *
 *  Q: "Token invalid hua toh kya?"
 *  → "Filter chain continue karta bina SecurityContext set kiye —
 *     baad ka authorization filter 401 throw kar dega
 *     'unauthenticated request to protected endpoint'."
 *
 *  Q: "OncePerRequestFilter kyu, plain Filter kyu nahi?"
 *  → "Spring filter chain mein same filter multiple baar add ho
 *     sakta different reasons se. OncePerRequestFilter guarantee
 *     karta — single execution per request, redundant checks bachata."
 *  ════════════════════════════════════════════════════════════════════
 *  📐 SOLID PRINCIPLES APPLIED
 *  ════════════════════════════════════════════════════════════════════
 *  ✅ SRP — Sirf JWT token extraction + validation per request.
 *  ✅ DIP — JwtService + CustomUserDetailsService inject (interfaces / abstractions).
 *  ✅ OCP — Naye auth header types (e.g., API key) ke liye doosra filter
 *           add ho sakta, JwtFilter unchanged.
 *  ════════════════════════════════════════════════════════════════════
 */
@Component
public class JwtFilter extends OncePerRequestFilter {

    /*
     *  🛠️ Guard ke 2 tools:
     *  • jwtService → "ID card scanner" (token validate karta)
     *  • userDetailsService → "HR ko phone karne ka number"
     *                         (employee details fetch karta)
     */
    private final JwtService jwtService;
    private final CustomUserDetailsService userDetailsService;

    // Constructor injection — Spring inject karega dono dependencies
    public JwtFilter(JwtService jwtService, CustomUserDetailsService userDetailsService) {
        this.jwtService = jwtService;
        this.userDetailsService = userDetailsService;
    }

    /*
     *  🎬 doFilterInternal — Yeh method har request pe call hota
     *  (OncePerRequestFilter ka contract — implement zaroor karna)
     */
    @Override
    protected void doFilterInternal(
            HttpServletRequest request,
            HttpServletResponse response,
            FilterChain chain) throws ServletException, IOException {

        // ════════════════════════════════════════════════════════
        //  1️⃣  STEP 1: ID CARD MAANGO (Authorization header)
        // ════════════════════════════════════════════════════════
        //
        //  🛡️  Guard: "Bhai, ID card dikhao."
        //
        //  Header format expected:
        //     Authorization: Bearer eyJhbGc...
        //
        //  Possible scenarios:
        //  • Header missing entirely     → public endpoint shayad (login/register)
        //  • Header hai but "Bearer " nahi → wrong format, ignore
        //  • Header + "Bearer " present  → token extract karo
        String authHeader = request.getHeader("Authorization");

        if (authHeader == null || !authHeader.startsWith("Bearer ")) {
            // 🚪 Card nahi laya? Theek hai — agar public endpoint hai
            //    (jaise /auth/login) toh chain continue karega bina auth.
            //    Agar protected endpoint hai → aage Authorization filter 401 dega.
            chain.doFilter(request, response);
            return;
        }

        // ✂️  "Bearer " prefix kaat — sirf token chahiye
        //     "Bearer eyJhbGc..."  →  "eyJhbGc..."
        //      └──7──┘
        String token = authHeader.substring(7);

        // ════════════════════════════════════════════════════════
        //  2️⃣  STEP 2: ID CARD VALID HAI? (Signature + Expiry)
        // ════════════════════════════════════════════════════════
        //
        //  🛡️  Guard scanner se card scan karta:
        //      • Stamp asli hai? (signature match with SECRET)
        //      • Card expire to nahi? (exp claim < now?)
        //      • Format theek hai? (3 parts dot-separated)
        //
        //  Koi bhi check fail = 🚫 reject
        if (!jwtService.isValid(token)) {
            // ❌ Invalid card. Andar mat jaane do.
            //    SecurityContext set NAHI karenge → downstream 401 dega.
            chain.doFilter(request, response);
            return;
        }

        // ════════════════════════════════════════════════════════
        //  3️⃣  STEP 3: CARD PE NAAM PADHO (Extract email)
        // ════════════════════════════════════════════════════════
        //
        //  🛡️  Guard: "Card pe naam Arpan likha hai. OK."
        //
        //  Token signed + verified hai already (Step 2 mein),
        //  so payload trust kar sakte — tampered nahi hai.
        String email = jwtService.extractEmail(token);

        // 🛡️  Optimization: Agar SecurityContext mein authentication
        //     pehle se set hai (kabhi-kabhi previous filter ne set kiya
        //     hota), toh re-authenticate mat karo. Save DB call.
        if (email != null && SecurityContextHolder.getContext().getAuthentication() == null) {

            // ════════════════════════════════════════════════════
            //  4️⃣  STEP 4: HR SE EMPLOYEE DETAILS LOAD
            // ════════════════════════════════════════════════════
            //
            //  🛡️  Guard HR ko phone karta:
            //      "Arpan naam ka banda hai? Permissions kya hain?"
            //
            //  HR (CustomUserDetailsService) DB mein dhundhta:
            //      "Haan, Arpan, role: ADMIN"
            //  aur Spring's UserDetails object return karta.
            UserDetails userDetails = userDetailsService.loadUserByUsername(email);

            // ════════════════════════════════════════════════════
            //  5️⃣  STEP 5: LOG BOOK MEIN ENTRY (SecurityContext)
            // ════════════════════════════════════════════════════
            //
            //  🛡️  Guard log book khol ke likhta:
            //      "Right now: Arpan andar hai, role: ADMIN"
            //
            //  Yeh log book = SecurityContextHolder
            //  Downstream code (controller, @PreAuthorize) iska use
            //  karke pata karega "user kaun hai".
            UsernamePasswordAuthenticationToken auth =
                new UsernamePasswordAuthenticationToken(
                    userDetails,                    // 👤 PRINCIPAL: kaun hai (Arpan)
                    null,                            // 🔑 CREDENTIALS: already verified, null OK
                    userDetails.getAuthorities()     // 🎫 PERMISSIONS: ROLE_USER / ROLE_ADMIN
                );

            // 📋 Extra info attach karo — IP address, session ID, etc.
            //     Useful audit logs ke liye ("Arpan ne 5:00 PM pe yeh request kiya from IP X")
            auth.setDetails(new WebAuthenticationDetailsSource().buildDetails(request));

            // 📖 Log book mein entry karo (thread-local context)
            //     Yahaan se downstream sab "logged in user" identify kar sakta:
            //         SecurityContextHolder.getContext().getAuthentication()
            SecurityContextHolder.getContext().setAuthentication(auth);
        }

        // ════════════════════════════════════════════════════════
        //  ✅ STEP 6: GATE OPEN — andar jane do!
        // ════════════════════════════════════════════════════════
        //
        //  🛡️  Guard: "OK Arpan, andar jao."
        //  Filter chain continue → next filter → eventually controller.
        chain.doFilter(request, response);
    }
}
