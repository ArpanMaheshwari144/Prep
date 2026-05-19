package com.arpan.todoapp.security;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/security/SecurityConfig.java

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
// ─── IMPORTS ─────────────────────────────────────────────────
// @Configuration, @Bean         → Spring config class + beans
// AuthenticationManager*        → login auth chain (Bean 3)
// HttpSecurity                  → builder for filter chain (Bean 1)
// @EnableWebSecurity            → enable Spring Security
// SessionCreationPolicy         → stateless mode
// BCryptPasswordEncoder         → password hashing (Bean 2)
// SecurityFilterChain           → filter chain bean
// UsernamePasswordAuthenticationFilter → marker for filter ordering

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    SecurityConfig = JWT layer ka CONTROL CENTER
//
//    Spring Security ko batao:
//       • Endpoint rules (/auth/* open, baaki protected)
//       • JwtFilter chain mein kahaan lagega
//       • Session policy (stateless — JWT app)
//       • CSRF disable (JWT mein nahi chahiye)
//       • PasswordEncoder bean (BCrypt)
//       • AuthenticationManager bean (login flow)
// ═══════════════════════════════════════════════════════════════════════
//
// 🏨 SECURITY GUARD ANALOGY:
//
//    AuthenticationManager = RECEPTION DESK (login guard)
//       • Guest 1st time aaya
//       • ID + password verify
//       • Genuine? → keycard de
//       • EK BAAR per login
//
//    JwtFilter = ROOM/FACILITY DOOR SCANNER (ongoing guard)
//       • Har request pe scan
//       • Keycard (JWT) valid? → entry
//       • EVERY request
//
//    SecurityConfig = HOTEL POLICY (rules + setup)
//       • Kaun se rooms open (lobby = /auth/*)
//       • Kaun se locked (rooms = /todos, /users)
//       • Sab guards ka setup
//
// 🔑 3 BEANS DEFINED:
//
//    1. SecurityFilterChain
//       • HTTP rules (which URLs open vs protected)
//       • Add JwtFilter to filter chain
//       • CSRF off + Stateless session
//
//    2. PasswordEncoder (BCrypt)
//       • UserService uses for register (hash password)
//       • Spring uses for login (matches raw vs hash)
//
//    3. AuthenticationManager
//       • Login engine (Bean 3)
//       • AuthController uses for credential verification
//       • Spring Boot 3+ requires explicit bean
//       • Internally calls UserDetailsService + PasswordEncoder
//
// 🔑 STATELESS SESSION:
//    JWT app = no server session
//    SessionCreationPolicy.STATELESS = "session NA banao"
//    Every request = JWT validate → identity
//    No HttpSession on server side
//
// 🔑 CSRF DISABLE:
//    CSRF = cookie-based session attack defense
//    JWT uses Authorization header (not cookies) — immune
//    Hence: not needed → disable
//
// 🔑 FILTER ORDER (addFilterBefore):
//    .addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class)
//    = JwtFilter PEHLE chale, phir default Spring filter
//    = JWT first attempt, fallback to form auth (if JWT absent)

@Configuration
@EnableWebSecurity
public class SecurityConfig {

    // ─── JWT FILTER INJECTION ─────────────────────────────────
    // Constructor injection — JwtFilter (we built) chain mein add
    private final JwtFilter jwtFilter;

    public SecurityConfig(JwtFilter jwtFilter) {
        this.jwtFilter = jwtFilter;
    }

    // ═══════════════════════════════════════════════════════════
    //  BEAN 1: SecurityFilterChain — HTTP rules + filter chain
    // ═══════════════════════════════════════════════════════════
    //
    // PURPOSE:
    //    HTTP security rules define + filter chain customize
    //
    // FLOW:
    //    Incoming Request
    //       ↓
    //    CSRF check (skip — disabled)
    //       ↓
    //    Session policy (stateless — no session)
    //       ↓
    //    URL pattern match:
    //       /auth/** → permit all (open)
    //       anything else → authenticated required
    //       ↓
    //    JwtFilter runs (extract + validate JWT)
    //       ↓
    //    Authentication set in SecurityContext (if valid)
    //       ↓
    //    Controller receives request
    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        return http
                // CSRF: JWT app immune — disable
                .csrf(csrf -> csrf.disable())

                // Session: STATELESS (no server session, every req = JWT)
                .sessionManagement(session -> session
                        .sessionCreationPolicy(SessionCreationPolicy.STATELESS))

                // Endpoint rules
                .authorizeHttpRequests(auth -> auth
                        // Auth endpoints (register/login) — OPEN
                        .requestMatchers("/auth/**").permitAll()
                        // Everything else — JWT required
                        .anyRequest().authenticated())

                // JwtFilter add BEFORE default Spring filter
                // = JWT process pehle, then form auth fallback
                .addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class)

                .build();
    }

    // ═══════════════════════════════════════════════════════════
    //  BEAN 2: PasswordEncoder — BCrypt instance
    // ═══════════════════════════════════════════════════════════
    //
    // PURPOSE:
    //    Password hashing + verification
    //
    // USED BY:
    //    UserService.register() → encoder.encode(rawPassword)
    //                              = stores BCrypt hash in DB
    //    Login flow → encoder.matches(raw, storedHash)
    //                  = compares without exposing hash
    //
    // BCrypt:
    //    Industry standard hashing
    //    Auto salt (no manual salt needed)
    //    Configurable rounds (default 10 = 2^10 iterations)
    //    Same input + different salt = different hash
    //    .matches() handles comparison correctly
    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    // ═══════════════════════════════════════════════════════════
    //  BEAN 3: AuthenticationManager — login engine
    // ═══════════════════════════════════════════════════════════
    //
    // PURPOSE:
    //    Spring's authentication pipeline
    //    AuthController iss se login verify karta
    //
    // USED BY:
    //    AuthController.login() → authManager.authenticate(...)
    //
    // INTERNALLY:
    //    1. UserDetailsService.loadByUsername(email) — DB se user
    //    2. PasswordEncoder.matches(raw, hash) — password check
    //    3. Account flag checks (locked/expired/disabled)
    //    4. Return Authentication object (verified)
    //
    // SPRING BOOT 3+ REQUIREMENT:
    //    Pre-3 versions = auto-available
    //    3+ = explicit bean define karna padta (humara case)
    //
    // AuthenticationConfiguration:
    //    Spring's pre-configured manager builder
    //    .getAuthenticationManager() returns standard manager
    //    Uses our CustomUserDetailsService + BCryptPasswordEncoder
    @Bean
    public AuthenticationManager authenticationManager(
            AuthenticationConfiguration config) throws Exception {
        return config.getAuthenticationManager();
    }
}
