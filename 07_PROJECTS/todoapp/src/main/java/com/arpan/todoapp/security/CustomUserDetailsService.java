package com.arpan.todoapp.security;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/security/CustomUserDetailsService.java

import java.util.Collections;

import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import com.arpan.todoapp.repository.UserRepository;
// ─── IMPORTS ─────────────────────────────────────────────────
// UserRepository    → DB se user fetch
// User (Spring's)   → builder for UserDetails
// UserDetails       → return type Spring contract
// UserDetailsService → interface we implement
// UsernameNotFoundException → Spring's expected exception
//
// ⚠️ NOTE — humara User entity NAHI import kiya
// Reason: Spring's User import kiya — naming conflict avoid
// Hum apne User ko 'var' (Java 21) se use karenge

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    CustomUserDetailsService = SPRING SECURITY KA BRIDGE to our DB
//
//    Spring Security ko humara User entity ka pata nahi
//    Wo apna interface (UserDetailsService) provide karta
//    Hum implement karte → Spring se humara DB connect hota
// ═══════════════════════════════════════════════════════════════════════
//
// 🏨 HR ANALOGY (hotel context):
//    AuthController = Reception
//    JwtService    = Keycard machine
//    UserDetailsService = HR (knows employee records)
//
//    Reception ko employee verify karna ho:
//       HR se pucho: "Yeh employee hai? Details do"
//       HR record check → return UserDetails
//
// 🔑 SPRING SECURITY KA CONTRACT:
//    Implement UserDetailsService interface
//    Override loadUserByUsername(String username) method
//    Return UserDetails object
//
//    "username" = Spring Security's generic term
//    Hum email use kar rahe (unique identifier)
//
// 🔑 UserDetails KYA HAI:
//    Spring's standard user representation
//    Fields: username, password, authorities, enabled flags
//    Hum User.builder() use karke banayenge (Spring's User class)
//
// 🔑 WHEN CALLED:
//    1. Login pe — AuthenticationManager iss se user fetch karta
//    2. JWT validation — JwtFilter optionally use kar sakta
//
// 🔑 'var' KEYWORD (Java 10+, we're on Java 21):
//    Type inference — compiler figure out variable type
//    var user = repo.findByEmail(...) → compiler knows
//       type = com.arpan.todoapp.model.User
//    Avoids explicit import of our User
//    Clean + modern
@Service
public class CustomUserDetailsService implements UserDetailsService {

    // ─── REPOSITORY INJECTION ────────────────────────────────
    private final UserRepository repo;

    public CustomUserDetailsService(UserRepository repo) {
        this.repo = repo;
    }

    // ─── loadUserByUsername — Spring Security contract ──────
    @Override
    public UserDetails loadUserByUsername(String email) throws UsernameNotFoundException {

        // ─── STEP 1: Fetch user from DB ────────────────────
        // var = Java type inference
        // Compiler sees: findByEmail returns Optional<com.arpan.todoapp.model.User>
        //                .orElseThrow returns User
        // No explicit import needed
        var user = repo.findByEmail(email)
                .orElseThrow(() -> new UsernameNotFoundException(
                "User not found: " + email));

        // ─── STEP 2: Build Spring's UserDetails ─────────────
        //
        // 🎨 DESIGN PATTERN: BUILDER (jo humne padha tha — 01_builder.md)
        //    Spring's User class Builder pattern implement karta
        //    Step-by-step construction with fluent chaining
        //
        //    User.builder()              → static factory entry
        //         .username(email)        → setter on builder
        //         .password(hash)         → setter on builder
        //         .authorities(roles)     → setter on builder
        //         .build()                → finalize → UserDetails
        //
        //    Same pattern as:
        //       Jwts.builder()....compact()  (JwtService mein)
        //       Lombok @Builder              (entities mein)
        //       StringBuilder                (Java built-in)
        //
        // User class IMPORTED at top:
        //    import org.springframework.security.core.userdetails.User;
        // = No fully qualified path needed
        //
        // .authorities(Collections.emptyList()):
        //    Empty list pass kar rahe — koi roles assign nahi abhi
        //    Future: agar role-based access chahiye, set kar denge
        //    (USER / ADMIN, etc.)
        return User.builder()
                .username(user.getEmail())
                .password(user.getPassword())
                .authorities(Collections.emptyList())
                .build();


    
    }
}
