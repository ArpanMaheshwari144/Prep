package com.example.practice.security;

/*
 * ============================================================
 * YE KYA HAI: JWT Filter — har request pe token check. Valid toh jaane do, nahi toh block.
 * KYUN CHAHIYE: Bina filter ke protected APIs OPEN ho jaayengi. Koi bhi access kar lega.
 * KYA KARTA: Request se token nikalta → verify → valid toh SecurityContext set → aage jaane do.
 * NAHI HUA TO: SecurityConfig mein .authenticated() lagaya but koi CHECK nahi → sab block hoga.
 *
 * REAL WORLD ANALOGY — YE BUILDING KA SECURITY GUARD HAI:
 *   Har visitor (request) gate pe aata → Guard (JwtFilter) bolta "PASS dikhaao" (token)
 *
 *   CASE 1: Visitor ke paas valid pass hai
 *     → Guard verify karta → naam match, expiry valid, stamp asli
 *     → "Jaao andar" → visitor register mein naam likhe (SecurityContext set)
 *     → Controller tak pahunche
 *
 *   CASE 2: Visitor ke paas pass NAHI hai (no Authorization header)
 *     → Guard kuch nahi karta → filterChain.doFilter() chalta → Spring Security dekhta
 *     → "Authenticated nahi hai" → 403 Forbidden
 *
 *   CASE 3: Visitor ke paas NAKLI pass hai (invalid/expired token)
 *     → Guard verify karta → fail → SecurityContext set NAHI karta
 *     → Spring Security → 403 Forbidden
 *
 *   CASE 4: /api/auth/register ya /api/auth/login pe aaya
 *     → SecurityConfig mein permitAll() hai → Guard check nahi karta → seedha jaane do
 *
 * FILTER CHAIN — SEQUENCE:
 *   Request → RequestLoggingFilter → JwtFilter → SecurityFilter → Controller
 *   Har filter apna kaam kare → filterChain.doFilter() se next filter ko bheje.
 * ============================================================
 */

import jakarta.servlet.*;
import jakarta.servlet.http.*;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.filter.OncePerRequestFilter;
import java.io.IOException;
import java.util.ArrayList;

/*
 * ============================================================
 * extends OncePerRequestFilter
 *
 * YE KYA HAI: Spring ka filter — guarantee ki EK request pe SIRF EK baar chalega.
 *
 * KYUN LAGAYA (OncePerRequestFilter kyun, normal Filter kyun nahi?):
 *   Normal Filter mein request FORWARD ya REDIRECT hone pe filter DOBARA chal sakta.
 *   OncePerRequestFilter = "bhai, ek baar check kiya, dobara mat kar" — guaranteed once.
 *
 * NAHI LAGAYA TO (normal Filter extend kiya toh):
 *   Token 2 baar verify hoga → unnecessary processing → potential errors
 *   SecurityContext 2 baar set hoga → confusing behavior
 *
 * REAL WORLD ANALOGY:
 *   Normal Filter = guard jo har darwaaze pe check kare (main gate + floor gate + room gate — 3 baar).
 *   OncePerRequestFilter = guard jo SIRF main gate pe check kare — andar free movement.
 *   3 baar check = slow + annoying. 1 baar check = efficient.
 *
 * ============================================================
 * @Component NAHI lagaya — KYUN?
 *
 * YE KYA HAI: Deliberately @Component NAHI use kiya. SecurityConfig mein manually register kar rahe:
 *   .addFilterBefore(new JwtFilter(), UsernamePasswordAuthenticationFilter.class)
 *
 * KYUN NAHI LAGAYA:
 *   Agar @Component lagate → Spring KHUD BHI register karta (auto scan se)
 *   + SecurityConfig BHAI register karta (addFilterBefore se)
 *   = DOUBLE REGISTER → filter 2 baar chalta har request pe!
 *   Token 2 baar verify, SecurityContext 2 baar set — bugs + performance waste.
 *
 * NAHI LAGAYA TO KYA HOGA (ye SAHI decision hai):
 *   @Component NAHI lagaya = CORRECT. Filter sirf SecurityConfig se register hoga = 1 baar chalega.
 *
 * AGAR GALTI SE @Component LAGA DIYA:
 *   Filter 2 baar chalega → double logging, double verification → mysterious bugs.
 *
 * REAL WORLD ANALOGY:
 *   Guard ko 2 jagah hire kiya — main gate + parking gate → guard confused "kahan jaau?"
 *   Ek jagah hire karo = clear. @Component nahi lagaya = ek jagah registered.
 * ============================================================
 */
public class JwtFilter extends OncePerRequestFilter {

    @Override
    protected void doFilterInternal(HttpServletRequest request,
                                    HttpServletResponse response,
                                    FilterChain filterChain) throws ServletException, IOException {
        /*
         * Ye method AUTOMATICALLY har request pe call hota (once per request).
         * request  = incoming HTTP request (URL, headers, body sab info)
         * response = outgoing HTTP response (status, headers, body set kar sakte)
         * filterChain = agle filters ki chain — doFilter() se next ko bhejo
         */

        // STEP 1: Authorization header nikalo
        String authHeader = request.getHeader("Authorization");
        /*
         * Client ne bheja:
         *   GET /api/students/5
         *   Authorization: Bearer eyJhbGciOiJIUzI1NiJ9.eyJzdWIi...
         *
         * getHeader("Authorization") → "Bearer eyJhbGciOiJIUzI1NiJ9..."
         * Nahi bheja → null → token check skip → Spring Security handle karega (403 if protected)
         */

        // STEP 2: Bearer token hai? Parse karo.
        if (authHeader != null && authHeader.startsWith("Bearer ")) {
            String token = authHeader.substring(7);  // "Bearer " (7 chars) hatao → sirf token
            /*
             * "Bearer eyJhbGci..." → substring(7) → "eyJhbGci..."
             * "Bearer " = convention. OAuth2 standard. Industry standard sab APIs aise karte.
             */

            String username = JwtUtil.extractUsername(token);
            // Token parse → signature verify → "sub" claim → "Arpan"
            // Agar token tampered/expired → Exception → catch nahi kiya toh error response

            if (username != null && JwtUtil.isTokenValid(token, username)) {
                // STEP 3: Valid token → SecurityContext mein set karo
                UsernamePasswordAuthenticationToken auth =
                    new UsernamePasswordAuthenticationToken(username, null, new ArrayList<>());
                /*
                 * Authentication object banao:
                 *   principal = "Arpan" (kaun hai — user identity)
                 *   credentials = null (password ki zaroorat nahi — token se verify ho gaya)
                 *   authorities = empty list (roles/permissions — abhi nahi lagaye)
                 *
                 *   Real project mein authorities mein ROLES daalo:
                 *   List.of(new SimpleGrantedAuthority("ROLE_ADMIN"))
                 *   Phir @PreAuthorize("hasRole('ADMIN')") se method level security laga sakte.
                 */

                SecurityContextHolder.getContext().setAuthentication(auth);
                /*
                 * ============================================================
                 * SecurityContextHolder
                 *
                 * YE KYA HAI: Current authenticated user store karta. ThreadLocal based.
                 *   Ek thread = ek request = ek user. Thread safe — dusre request ka data mix nahi hoga.
                 *
                 * KYUN LAGAYA: Spring Security ko batana hai "ye request authenticated hai."
                 *   YAHAN SET KIYA → ab Spring Security jaanta: "ye valid user hai"
                 *   → .anyRequest().authenticated() PASS ho jaayega → controller tak pahunchega
                 *
                 * NAHI SET KIYA TO (SecurityContext mein auth nahi rakha):
                 *   - Spring Security samjhega "anonymous request" → 403 Forbidden
                 *   - Token valid hai but Spring ko bataya nahi → BLOCK!
                 *   - Jaise valid ticket hai but register mein naam nahi likha → "aap kaun?"
                 *
                 * REAL WORLD ANALOGY:
                 *   SecurityContext = VISITOR REGISTER (building ka).
                 *   Guard ne ID check ki (token valid) → register mein naam LIKHA (setAuthentication).
                 *   Ab kisi bhi floor pe jaao → register mein naam hai → ALLOWED.
                 *   Naam nahi likha (set nahi kiya) → "register mein nahi ho, bahar jaao" → 403.
                 *
                 * Controller mein access:
                 *   SecurityContextHolder.getContext().getAuthentication().getName() → "Arpan"
                 * ============================================================
                 */
            }
        }

        // STEP 4: CRITICAL — Aage bhejo (next filter ya controller)
        filterChain.doFilter(request, response);
        /*
         * ============================================================
         * filterChain.doFilter() — SABSE CRITICAL LINE
         *
         * YE KYA HAI: Request ko NEXT filter ya controller ko bhejo. Chain continue karo.
         *
         * KYUN LAGAYA: Bina iske request is filter pe RUK JAAYEGI. Aage nahi jaayegi.
         *
         * NAHI CALL KIYA TO KYA HOGA:
         *   - Request YAHI RUK JAAYEGI — controller tak nahi pahunchegi
         *   - Client ko koi response NAHI milega — connection HANG
         *   - Client timeout error dekhega — "server not responding"
         *   - DEADLY BUG — app chalti dikhti but koi request complete nahi hoti
         *
         * REAL WORLD ANALOGY:
         *   filterChain.doFilter = ASSEMBLY LINE ka CONVEYOR BELT.
         *   Tera station (filter) pe kaam kiya → belt pe rakh (doFilter) → next station jaaye.
         *   Belt pe nahi rakha → product tera station pe ruk gaya → delivery NAHI.
         *
         *   Doosri analogy:
         *   Relay race mein BATON PASS karna = doFilter.
         *   Baton pass nahi kiya → race wahi ruk gayi → team LOSE.
         * ============================================================
         */
    }
}
