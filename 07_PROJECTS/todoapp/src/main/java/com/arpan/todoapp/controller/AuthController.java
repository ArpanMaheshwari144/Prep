package com.arpan.todoapp.controller;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/controller/AuthController.java

import com.arpan.todoapp.dto.LoginRequest;
import com.arpan.todoapp.dto.RegisterRequest;
import com.arpan.todoapp.dto.TokenResponse;
import com.arpan.todoapp.model.User;
import com.arpan.todoapp.security.JwtService;
import com.arpan.todoapp.service.UserService;
import jakarta.validation.Valid;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.web.bind.annotation.*;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    AuthController = AUTH endpoints (register + login)
//    Reception desk role — guest entry + keycard issuance
//
//    Endpoints:
//       POST /auth/register → new user create
//       POST /auth/login    → verify + return JWT
//
//    Public endpoints (SecurityConfig mein /auth/** permitAll)
// ═══════════════════════════════════════════════════════════════════════
//
// 🔑 3 DEPENDENCIES (constructor inject):
//    1. UserService            → register user logic
//    2. AuthenticationManager  → login credential verification
//    3. JwtService             → generate JWT tokens
//
// 🔑 REGISTER FLOW:
//    1. Receive RegisterRequest (name, email, password)
//    2. @Valid auto-validates input
//    3. Convert to User entity (raw password)
//    4. UserService.register() = BCrypt hash + save
//    5. Return saved User (no password in response — DTO ideally)
//
// 🔑 LOGIN FLOW:
//    1. Receive LoginRequest (email, password)
//    2. Build UsernamePasswordAuthenticationToken
//    3. AuthenticationManager.authenticate()
//       → Internally calls UserDetailsService + PasswordEncoder
//       → Throws if invalid credentials
//    4. Fetch full User from DB (for generateToken)
//    5. Generate JWT via JwtService
//    6. Return TokenResponse (accessToken + refreshToken)
//
// 🔑 NOTE — Refresh Token:
//    Abhi simplified — accessToken aur refreshToken dono same
//    (humne ek generateToken method banaya hai)
//    Future enhancement: alag generateRefreshToken (longer expiry)
//                        + DB storage for revocation

@RestController
@RequestMapping("/auth")
public class AuthController {

    private final UserService userService;
    private final AuthenticationManager authManager;
    private final JwtService jwtService;

    public AuthController(UserService userService,
                          AuthenticationManager authManager,
                          JwtService jwtService) {
        this.userService = userService;
        this.authManager = authManager;
        this.jwtService = jwtService;
    }

    // ─── POST /auth/register ──────────────────────────────────
    // Naya user banao
    // @Valid → request body validation (name/email/password rules)
    // 200 OK + saved user
    @PostMapping("/register")
    public ResponseEntity<User> register(@Valid @RequestBody RegisterRequest req) {
        // Build User entity from DTO
        User user = new User();
        user.setName(req.getName());
        user.setEmail(req.getEmail());
        user.setPassword(req.getPassword());  // UserService BCrypt hash karega

        // Register (UserService internally hashes password)
        User saved = userService.register(user);

        return ResponseEntity.ok(saved);
    }

    // ─── POST /auth/login ─────────────────────────────────────
    // Email + password verify → JWT return
    //
    // ═══════════════════════════════════════════════════════════
    //  🔥 authManager.authenticate(...) = ONE LINE FULL AUTH PIPELINE
    // ═══════════════════════════════════════════════════════════
    //
    // 1. UsernamePasswordAuthenticationToken banao
    //    = Spring's standard input format for auth
    //    = "Yeh credentials verify karne hain"
    //
    // 2. AuthManager internally DaoAuthenticationProvider use karta:
    //
    //    Step A: LOAD USER FROM DB
    //       userDetailsService.loadUserByUsername(email)
    //       = CustomUserDetailsService trigger
    //       = UserRepository.findByEmail()
    //       = Found? continue
    //       = Not found? UsernameNotFoundException → 401
    //
    //    Step B: PASSWORD VERIFY
    //       passwordEncoder.matches(rawPassword, storedHash)
    //       = BCrypt internally compares
    //       = Match? continue
    //       = Mismatch? BadCredentialsException → 401
    //
    //    Step C: ACCOUNT FLAG CHECKS
    //       isAccountNonExpired?
    //       isAccountNonLocked?
    //       isCredentialsNonExpired?
    //       isEnabled?
    //       = All pass? Authentication returned (verified)
    //       = Any fail? DisabledException / LockedException etc.
    //
    // 3. ONE LINE = ENTIRE Spring security pipeline
    //    Manually karte to 30+ lines
    //    Return value yahaan use NAHI kar rahe (sirf success/fail matter)
    //
    // 🔑 EXCEPTIONS (all extend AuthenticationException):
    //    UsernameNotFoundException → wrong email
    //    BadCredentialsException   → wrong password
    //    LockedException           → account locked
    //    DisabledException         → account disabled
    //    All → 401 Unauthorized auto-respond
    @PostMapping("/login")
    public ResponseEntity<TokenResponse> login(@Valid @RequestBody LoginRequest req) {
        // Spring's authentication pipeline (1 line — full chain)
        // Pass = continue, Fail = exception thrown (method skip)
        authManager.authenticate(
                new UsernamePasswordAuthenticationToken(
                        req.getEmail(),
                        req.getPassword()
                )
        );

        // Auth success — fetch full User for JWT claims
        // (AuthManager verifies, doesn't return User entity directly)
        User user = userService.getByEmail(req.getEmail());

        // Generate JWT (access token — 15 min validity)
        String accessToken = jwtService.generateToken(user);

        // Refresh token = same for now (simplified)
        // Future: separate generateRefreshToken with longer expiry (7 days)
        // Future: DB-stored for revocation capability
        String refreshToken = jwtService.generateToken(user);

        // Return both tokens to client
        return ResponseEntity.ok(new TokenResponse(accessToken, refreshToken));
    }
}
