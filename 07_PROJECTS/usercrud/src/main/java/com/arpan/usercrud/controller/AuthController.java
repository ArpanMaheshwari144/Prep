package com.arpan.usercrud.controller;

import java.time.Instant;
import java.time.temporal.ChronoUnit;
import java.util.UUID;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.BadCredentialsException;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.arpan.usercrud.dto.LoginRequest;
import com.arpan.usercrud.dto.RefreshRequest;
import com.arpan.usercrud.dto.RegisterRequest;
import com.arpan.usercrud.dto.TokenResponse;
import com.arpan.usercrud.exception.UserNotFoundException;
import com.arpan.usercrud.model.RefreshToken;
import com.arpan.usercrud.model.User;
import com.arpan.usercrud.repository.RefreshTokenRepository;
import com.arpan.usercrud.repository.UserRepository;
import com.arpan.usercrud.security.JwtService;

import jakarta.validation.Valid;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    AuthController = GATE OFFICE
//    /register, /login, /refresh, /logout endpoints
//    Token issue + revoke karta
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — GATE OFFICE:
//    Client
//         │
//         ▼
//    ┌──────────────────────────────────────────┐
//    │  AuthController @ /auth/*              │
//    │                                           │
//    │  /register  → Naya banda signup        │
//    │  /login     → Credentials → tokens     │
//    │  /refresh   → Naya access token        │
//    │  /logout    → Refresh DB se delete    │
//    └──────────────────────────────────────────┘
//
// 5 DEPENDENCIES (constructor inject):
//    • UserRepository
//    • RefreshTokenRepository
//    • JwtService
//    • AuthenticationManager
//    • PasswordEncoder
//
// LOGIN END-TO-END FLOW:
//
//    1. Client → POST /auth/login { email, password }
//
//    2. AuthController.login():
//       a. authenticationManager.authenticate(token)
//       b. Spring internally:
//          • CustomUserDetailsService.loadUserByUsername(email)
//            → DB se user fetch
//          • PasswordEncoder.matches(rawPassword, storedHash)
//            → BCrypt compare
//          • Match → Authentication object
//          • Mismatch → BadCredentialsException
//       c. JwtService.generateToken(user) → JWT (15 min)
//       d. UUID generate → refreshTokenRepo.save() (7 days)
//       e. Return TokenResponse { accessToken, refreshToken }
//
//    3. Client localStorage mein dono store kar leta
//
// PROTECTED REQUEST FLOW (after login):
//
//    1. Client → GET /users/me
//       Header: Authorization: Bearer eyJhbGc...
//
//    2. JwtFilter intercepts:
//       a. "Bearer " prefix check → token extract
//       b. JwtService.isValid(token) → signature + exp verify
//       c. extractEmail(token) → email
//       d. CustomUserDetailsService.loadUserByUsername(email)
//       e. SecurityContextHolder.setAuthentication(...)
//
//    3. Spring routes to UserController
//    4. Controller can access user via SecurityContext
//    5. Response sent back
//
// TOKEN REFRESH FLOW:
//    Access expired → Frontend interceptor catches 401
//
//    1. Client → POST /auth/refresh { refreshToken }
//
//    2. AuthController.refresh():
//       a. refreshTokenRepo.findByToken("RRRR")
//       b. Found? expired nahi?
//       c. user fetch → JwtService.generateToken() → naya access
//       d. Return TokenResponse { newAccess, sameRefresh }
//
//    3. Client retries original request with naya accessToken
//
// LOGOUT FLOW:
//    1. Client → POST /auth/logout { refreshToken }
//
//    2. AuthController.logout():
//       a. refreshTokenRepo.deleteByToken("RRRR")
//       b. Return 200 OK
//
//    3. Client localStorage clear → done
//
//    Result: Refresh token DB se gone → next refresh fail
//            Access token apne aap 15 min mein expire
//
// MASTER JWT SUMMARY — 7 COMPONENTS:
//    1. User entity                  → email + BCrypt password + role
//    2. RefreshToken entity          → DB-stored long-lived tokens
//    3. JwtService                   → Token Factory
//    4. CustomUserDetailsService     → HR (DB lookup)
//    5. JwtFilter                    → Guard (per-request validate)
//    6. SecurityConfig               → CSO (filter chain wire)
//    7. AuthController (THIS FILE)   → Gate Office (endpoints)
//
// SECURITY HIGHLIGHTS:
//    • Email uniqueness check on register
//    • BCrypt password hash (slow, salted)
//    • role="USER" hardcode on register (no privilege escalation)
//    • Refresh token DB-stored (revocation possible on logout)
//    • Access 15 min, Refresh 7 days
//
// SOLID:
//    SRP — Sirf auth HTTP endpoints
//    DIP — 5 dependencies via constructor (all Spring abstractions)
//    OCP — Naye auth methods (OAuth) add easy, existing unchanged
//
// INTERVIEW LINE:
//    "AuthController handles authentication — register/login/refresh/logout.
//     Login uses AuthenticationManager.authenticate() which internally
//     calls CustomUserDetailsService + PasswordEncoder.
//     Match → JWT access (15 min) + UUID refresh (7 days, DB-stored).
//     Refresh endpoint DB lookup → new access. Logout deletes refresh row."
// ═══════════════════════════════════════════════════════════════════════

@RestController
@RequestMapping("/auth")
public class AuthController {

    // Dependencies — Spring inject karega constructor se
    private final UserRepository userRepository;
    private final RefreshTokenRepository refreshTokenRepository;
    private final JwtService jwtService;
    private final AuthenticationManager authenticationManager;
    private final PasswordEncoder passwordEncoder;

    @Value("${jwt.refresh.expiration}")
    private long refreshExpiration;     // 7 days in ms

    public AuthController(
            UserRepository userRepository,
            RefreshTokenRepository refreshTokenRepository,
            JwtService jwtService,
            AuthenticationManager authenticationManager,
            PasswordEncoder passwordEncoder) {
        this.userRepository = userRepository;
        this.refreshTokenRepository = refreshTokenRepository;
        this.jwtService = jwtService;
        this.authenticationManager = authenticationManager;
        this.passwordEncoder = passwordEncoder;
    }

    // ════════════════════════════════════════════════════════════
    //  1. REGISTER — New user signup
    // ════════════════════════════════════════════════════════════
    //  POST /auth/register
    //  Body: { name, email, password, age }
    //
    //  Flow:
    //  1. Email already taken? → 400
    //  2. BCrypt password hash karo
    //  3. role hardcode "USER" (security — client can't set)
    //  4. Save → return saved user
    @PostMapping("/register")
    public ResponseEntity<User> register(@Valid @RequestBody RegisterRequest req) {
        // Email uniqueness check
        if (userRepository.findByEmail(req.getEmail()).isPresent()) {
            throw new IllegalArgumentException("Email already registered: " + req.getEmail());
        }

        // Password hash with BCrypt — DB mein hash store, plain NEVER
        String hashedPassword = passwordEncoder.encode(req.getPassword());

        // New User — role hardcoded "USER" (privilege escalation prevent)
        User user = new User(
            null,                          // id (auto-generate)
            req.getName(),
            req.getEmail(),
            hashedPassword,                // BCrypt hash
            req.getAge(),
            "USER"                         // role — server-side hardcode
        );

        User saved = userRepository.save(user);

        // Production: password field response mein nahi bhejna chahiye
        // Yeh demo simplicity ke liye — properly DTO use karke filter karte
        return ResponseEntity.ok(saved);
    }

    // ════════════════════════════════════════════════════════════
    //  2. LOGIN — Credentials verify + token issue
    // ════════════════════════════════════════════════════════════
    //  POST /auth/login
    //  Body: { email, password }
    //
    //  Flow:
    //  1. AuthenticationManager.authenticate() — Spring's magic:
    //     • CustomUserDetailsService.loadUserByUsername(email)
    //     • PasswordEncoder.matches(rawPassword, storedHash)
    //  2. Match? → user fetch from DB
    //  3. JwtService.generateToken() → 15-min JWT
    //  4. UUID refresh token → DB save (7 days)
    //  5. Return both tokens
    @PostMapping("/login")
    public ResponseEntity<TokenResponse> login(@Valid @RequestBody LoginRequest req) {
        try {
            // Spring Security ki magic — credentials verify
            authenticationManager.authenticate(
                new UsernamePasswordAuthenticationToken(req.getEmail(), req.getPassword())
            );
        } catch (BadCredentialsException e) {
            throw new IllegalArgumentException("Invalid email or password");
        }

        // Authenticated — fetch full user
        User user = userRepository.findByEmail(req.getEmail())
                .orElseThrow(() -> new UserNotFoundException(0L));

        // Generate access token (JWT, 15 min)
        String accessToken = jwtService.generateToken(user);

        // Generate refresh token (UUID, 7 days, DB stored)
        String refreshTokenString = UUID.randomUUID().toString();
        RefreshToken refreshToken = new RefreshToken(
            null,
            refreshTokenString,
            user.getId(),
            Instant.now().plus(7, ChronoUnit.DAYS)
        );
        refreshTokenRepository.save(refreshToken);

        return ResponseEntity.ok(new TokenResponse(accessToken, refreshTokenString));
    }

    // ════════════════════════════════════════════════════════════
    //  3. REFRESH — Issue new access token
    // ════════════════════════════════════════════════════════════
    //  POST /auth/refresh
    //  Body: { refreshToken }
    //
    //  Flow:
    //  1. DB se refresh token find
    //  2. Expired? → DELETE + 400
    //  3. user fetch → naya access token generate
    //  4. Return new access (refresh same rakhte)
    @PostMapping("/refresh")
    public ResponseEntity<TokenResponse> refresh(@Valid @RequestBody RefreshRequest req) {
        // DB se refresh token dhundo
        RefreshToken stored = refreshTokenRepository.findByToken(req.getRefreshToken())
                .orElseThrow(() -> new IllegalArgumentException("Invalid refresh token"));

        // Expired check
        if (stored.getExpiresAt().isBefore(Instant.now())) {
            refreshTokenRepository.delete(stored);   // cleanup expired row
            throw new IllegalArgumentException("Refresh token expired, please login again");
        }

        // User fetch
        User user = userRepository.findById(stored.getUserId())
                .orElseThrow(() -> new UserNotFoundException(stored.getUserId()));

        // Naya access token
        String newAccessToken = jwtService.generateToken(user);

        // Refresh token same rakhte (rotation = optional advanced topic)
        return ResponseEntity.ok(new TokenResponse(newAccessToken, req.getRefreshToken()));
    }

    // ════════════════════════════════════════════════════════════
    //  4. LOGOUT — Refresh token DB se delete
    // ════════════════════════════════════════════════════════════
    //  POST /auth/logout
    //  Body: { refreshToken }
    //
    //  Flow:
    //  1. DB se refresh token DELETE
    //  2. 200 OK
    //
    //  Note: Access token instantly invalidate nahi hota — woh
    //  apne aap 15 min mein expire ho jata. Production sensitive
    //  apps mein Redis blacklist use karte additional layer ke liye.
    @PostMapping("/logout")
    public ResponseEntity<Void> logout(@Valid @RequestBody RefreshRequest req) {
        refreshTokenRepository.deleteByToken(req.getRefreshToken());
        return ResponseEntity.ok().build();
    }
}
