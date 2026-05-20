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

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    JwtFilter = SECURITY GUARD at building gate
//    Every request gate pe roke → token check → valid? andar bhej
//    Invalid? chain continue without auth (401 later)
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — OFFICE GUARD ANALOGY:
//    You              Guard           Office
//    (request)          (JwtFilter)        (controller)
//       │                   │                  │
//       │   "ID card?"      │                  │
//       │ ──────────────►   │                  │
//       │                   │ checks card      │
//       │                   │ • signature?     │
//       │                   │ • expired?       │
//       │                   │                  │
//       │   Valid         │ log book entry   │
//       │ ──────────────►   │ ───────────────►│
//       │                                       │
//       │                              "Welcome Arpan"
//
// 5-STEP FLOW:
//    Request arrives
//         │
//         ▼
//    1. ID card mango          → Authorization header check
//         │ "Bearer ..."
//         ▼
//    2. Card valid?            → jwtService.isValid(token)
//         │ yes
//         ▼
//    3. Card pe naam padho     → jwtService.extractEmail(token)
//         │
//         ▼
//    4. HR se confirm           → userDetailsService.loadUserByUsername(email)
//         │
//         ▼
//    5. Log book mein entry    → SecurityContextHolder.setAuthentication(auth)
//         │
//         ▼
//    Controller reaches → knows "current user is Arpan"
//
// extends OncePerRequestFilter WHY?
//    Spring filter chain mein same filter multiple baar add ho sakta
//       = Same banda 5 baar gate pe rokte (redundant)
//
//    OncePerRequestFilter:
//       Guarantee — sirf EK BAAR chalega per request
//       = Single check, no redundant work
//
// FILTER CHAIN PLACEMENT:
//    HTTP Request
//         │
//         ▼
//    CORS Filter
//         ↓
//    CSRF (disabled — JWT stateless)
//         ↓
//    JwtFilter  ← TU YAHAN
//         ↓
//    UPAuthFilter (Spring built-in)
//         ↓
//    Authz Check (@PreAuthorize, hasRole)
//         ↓
//    Controller
//
// "Bearer " PREFIX:
//    RFC 6750 standard:
//       Authorization: Bearer eyJhbGc...
//
//    "Bearer" = "iska bearer authorized hai"
//    Universal convention (Spring, AWS, every modern API)
//
//    substring(7) = "Bearer " (7 chars) hata ke pure token
//
// SecurityContextHolder — Thread-local "Log Book":
//    Imagine office WHITEBOARD:
//       "Right now logged in: Arpan, role: ADMIN"
//
//    Thread-local = har request ka apna whiteboard
//
//    Downstream code uses:
//       SecurityContextHolder.getContext().getAuthentication()
//       → returns logged-in user info
//
//    Without filter: EMPTY (anonymous)
//    With filter:    FULL (authenticated)
//
// UsernamePasswordAuthenticationToken — 3 PIECES:
//    new UsernamePasswordAuthenticationToken(
//       PRINCIPAL,      ← WHO (UserDetails object)
//       CREDENTIALS,    ← password (null after JWT verify — already trusted)
//       AUTHORITIES     ← permissions (ROLE_USER, ROLE_ADMIN)
//    )
//
//    2 constructors in Spring:
//       1-arg (no credentials check): used by us (post-verify)
//       3-arg with password: used during initial login (compare)
//
// auth.setDetails() — EXTRA INFO:
//    Attaches:
//       • IP address
//       • Session ID
//       • User agent
//
//    Useful for audit logs:
//       "Arpan ne 5:00 PM pe POST /api/users kiya from IP 1.2.3.4"
//
// PUBLIC ENDPOINTS STILL WORK:
//    /auth/login, /auth/register = public
//       No token expected
//
//    Flow:
//       Request → Authorization header NULL
//            ↓
//       if (header == null) chain.doFilter → return
//            ↓
//       No auth set in SecurityContext
//            ↓
//       AuthController endpoint hits
//            ↓
//       SecurityConfig allows /auth/** without auth
//
// SOLID:
//    SRP — Sirf token extraction + validation per request
//          No business logic
//    DIP — JwtService + CustomUserDetailsService injected
//    OCP — Naya auth type (API key)? Add another filter
//          JwtFilter unchanged
//
// INTERVIEW LINE:
//    "JwtFilter extends OncePerRequestFilter — guarantees single
//     execution per request.
//
//     Flow:
//       1. Extract token from 'Bearer ' header
//       2. Validate via JwtService.isValid()
//       3. Extract email, load UserDetails
//       4. Create UsernamePasswordAuthenticationToken
//       5. Set in SecurityContextHolder (thread-local)
//
//     Downstream sees authenticated user, @PreAuthorize works.
//     Configured before UsernamePasswordAuthenticationFilter via
//     addFilterBefore() in SecurityConfig."
// ═══════════════════════════════════════════════════════════════════════

@Component
public class JwtFilter extends OncePerRequestFilter {

    // Guard ke 2 tools:
    //   jwtService → "ID card scanner" (token validate karta)
    //   userDetailsService → "HR ko phone karne ka number"
    //                        (employee details fetch karta)
    private final JwtService jwtService;
    private final CustomUserDetailsService userDetailsService;

    // Constructor injection — Spring inject karega dono dependencies
    public JwtFilter(JwtService jwtService, CustomUserDetailsService userDetailsService) {
        this.jwtService = jwtService;
        this.userDetailsService = userDetailsService;
    }

    // doFilterInternal — har request pe call hota
    // (OncePerRequestFilter ka contract — implement zaroor karna)
    @Override
    protected void doFilterInternal(
            HttpServletRequest request,
            HttpServletResponse response,
            FilterChain chain) throws ServletException, IOException {

        // ════════════════════════════════════════════════════════
        //  1️⃣  STEP 1: ID CARD MAANGO (Authorization header)
        // ════════════════════════════════════════════════════════
        //  Header format expected:
        //     Authorization: Bearer eyJhbGc...
        //
        //  Possible scenarios:
        //  • Header missing entirely     → public endpoint shayad (login/register)
        //  • Header hai but "Bearer " nahi → wrong format, ignore
        //  • Header + "Bearer " present  → token extract karo
        String authHeader = request.getHeader("Authorization");

        if (authHeader == null || !authHeader.startsWith("Bearer ")) {
            // Card nahi laya? Theek hai — chain continue karega bina auth.
            //    Agar protected endpoint hai → aage Authorization filter 401 dega.
            chain.doFilter(request, response);
            return;
        }

        //  "Bearer " prefix kaat — sirf token chahiye
        //     "Bearer eyJhbGc..."  →  "eyJhbGc..."
        String token = authHeader.substring(7);

        // ════════════════════════════════════════════════════════
        //  2️⃣  STEP 2: ID CARD VALID HAI? (Signature + Expiry)
        // ════════════════════════════════════════════════════════
        //  Guard scanner se card scan karta:
        //      • Stamp asli hai? (signature match with SECRET)
        //      • Card expire to nahi? (exp claim < now?)
        //      • Format theek hai? (3 parts dot-separated)
        //
        //  Koi bhi check fail = reject
        if (!jwtService.isValid(token)) {
            // Invalid card. SecurityContext set NAHI → downstream 401.
            chain.doFilter(request, response);
            return;
        }

        // ════════════════════════════════════════════════════════
        //  3️⃣  STEP 3: CARD PE NAAM PADHO (Extract email)
        // ════════════════════════════════════════════════════════
        //  Token signed + verified already (Step 2 mein), payload trust kar sakte.
        String email = jwtService.extractEmail(token);

        // Optimization: Agar SecurityContext mein authentication pehle se set hai
        // (kabhi-kabhi previous filter ne set kiya hota), toh re-authenticate mat karo.
        // Save DB call.
        if (email != null && SecurityContextHolder.getContext().getAuthentication() == null) {

            // ════════════════════════════════════════════════════
            //  4️⃣  STEP 4: HR SE EMPLOYEE DETAILS LOAD
            // ════════════════════════════════════════════════════
            //  CustomUserDetailsService DB mein dhundhta:
            //      "Haan, Arpan, role: ADMIN"
            //  aur Spring's UserDetails object return karta.
            UserDetails userDetails = userDetailsService.loadUserByUsername(email);

            // ════════════════════════════════════════════════════
            //  5️⃣  STEP 5: LOG BOOK MEIN ENTRY (SecurityContext)
            // ════════════════════════════════════════════════════
            //  Yeh log book = SecurityContextHolder
            //  Downstream code (controller, @PreAuthorize) iska use karta
            UsernamePasswordAuthenticationToken auth =
                new UsernamePasswordAuthenticationToken(
                    userDetails,                    // PRINCIPAL: kaun hai
                    null,                            // CREDENTIALS: already verified
                    userDetails.getAuthorities()     // PERMISSIONS: ROLE_USER / ROLE_ADMIN
                );

            // Extra info attach (IP, session, etc.) — useful audit logs ke liye
            auth.setDetails(new WebAuthenticationDetailsSource().buildDetails(request));

            // Log book mein entry karo (thread-local context)
            SecurityContextHolder.getContext().setAuthentication(auth);
        }

        // ════════════════════════════════════════════════════════
        //  STEP 6: GATE OPEN — andar jane do!
        // ════════════════════════════════════════════════════════
        //  Filter chain continue → next filter → eventually controller.
        chain.doFilter(request, response);
    }
}
