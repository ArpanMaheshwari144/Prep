package com.arpan.todoapp.security;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/security/JwtFilter.java

import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Component;
import org.springframework.web.filter.OncePerRequestFilter;

import java.io.IOException;
// ─── IMPORTS ─────────────────────────────────────────────────
// FilterChain, Servlet*, HttpServlet*  → Servlet API (Spring uses)
// UsernamePasswordAuthenticationToken  → Spring's Authentication impl
// SecurityContextHolder                → Spring's per-request user store
// @Component                            → Spring auto-register as bean
// OncePerRequestFilter                  → Spring base class (1x per req)

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    JwtFilter = HTTP request ka SECURITY GUARD
//    Har request app mein aane se PEHLE iss se guzarti
//    Token verify karta — valid? Set SecurityContext aur aage bhejo
//                          Invalid? Bhi aage bhej (controller 401 handle karega)
// ═══════════════════════════════════════════════════════════════════════
//
// HOTEL KEYCARD ANALOGY:
//
//    Guest hotel mein har room/facility access kare:
//       Door pe SCANNER (= JwtFilter)
//       Keycard scan → valid? → entry
//
//    JwtFilter "scanner":
//       Every request → check Authorization header
//       JWT extract → validate → identify user → set context
//
// OncePerRequestFilter EXTEND:
//    Spring's base class
//    GUARANTEES yeh filter EK REQUEST mein SIRF 1 BAAR chale
//    (Spring internally complex chain — sometimes filter twice trigger
//     hota — yeh class wo prevent karta)
//    Override: doFilterInternal()
//
// FLOW INSIDE doFilterInternal:
//    1. Authorization header read karo
//    2. "Bearer xxx" format check
//    3. Token nikalo (substring 7 = skip "Bearer ")
//    4. JwtService.isTokenValid(token) check
//       Invalid? → chain.doFilter (continue, SecurityContext empty,
//                                  controller 401 handle karega)
//    5. JwtService.extractEmail(token) → email
//    6. UserDetailsService.loadByUsername(email) → UserDetails
//    7. UsernamePasswordAuthenticationToken banao
//       (Spring's Authentication impl)
//    8. SecurityContextHolder.setAuthentication(...)
//       = "Current request mein yeh user authenticated hai"
//    9. chain.doFilter(...) — next filter / controller
//
// SecurityContextHolder KYA HAI:
//    Spring Security ka THREAD-LOCAL store
//    Per-request user identity
//    Controllers iss se current user nikal sakte
//    Request end pe auto-clear

@Component
public class JwtFilter extends OncePerRequestFilter {

    // ─── DEPENDENCIES (constructor injection) ────────────────
    private final JwtService jwtService;
    private final CustomUserDetailsService userDetailsService;

    public JwtFilter(JwtService jwtService, CustomUserDetailsService userDetailsService) {
        this.jwtService = jwtService;
        this.userDetailsService = userDetailsService;
    }

    // ─── doFilterInternal — Spring's per-request hook ───────
    @Override
    protected void doFilterInternal(
            HttpServletRequest request,
            HttpServletResponse response,
            FilterChain chain) throws ServletException, IOException {

        // ─── STEP 1: Extract Authorization header ───────────
        // Standard: "Authorization: Bearer eyJhbGc..."
        String authHeader = request.getHeader("Authorization");

        // ─── STEP 2: Validate Bearer format ─────────────────
        // No header OR doesn't start with "Bearer " → skip auth
        // (Controller will 401 if endpoint needs auth)
        if (authHeader == null || !authHeader.startsWith("Bearer ")) {
            chain.doFilter(request, response);
            return;
        }

        // ─── STEP 3: Extract token (skip "Bearer ") ─────────
        // "Bearer ".length() = 7
        String token = authHeader.substring(7);

        // ─── STEP 4: Validate token ─────────────────────────
        if (!jwtService.isTokenValid(token)) {
            chain.doFilter(request, response);
            return;
        }

        // ─── STEP 5: Extract email + load user ──────────────
        String email = jwtService.extractEmail(token);
        var userDetails = userDetailsService.loadUserByUsername(email);

        // ─── STEP 6: Build Spring Authentication object ─────
        // UsernamePasswordAuthenticationToken:
        //    arg1: principal (user details)
        //    arg2: credentials (null — already verified via JWT)
        //    arg3: authorities (roles — empty for now)
        var auth = new UsernamePasswordAuthenticationToken(
                userDetails,
                null,
                userDetails.getAuthorities()
        );

        // ─── STEP 7: Set SecurityContext ────────────────────
        // Per-request, thread-local store
        // Now controllers know who's logged in
        SecurityContextHolder.getContext().setAuthentication(auth);

        // ─── STEP 8: Continue filter chain ──────────────────
        // Next filter / controller runs
        chain.doFilter(request, response);
    }
}
