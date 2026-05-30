package com.arpan.usercrud.security;

import java.util.Date;

import javax.crypto.SecretKey;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.arpan.usercrud.model.User;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.JwtException;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.security.Keys;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    JwtService = JWT TOKEN FACTORY
//    Generate, parse, validate JWT tokens
//    3 main jobs:
//       1. generateToken(user) — banao
//       2. extractEmail/UserId/Role — padho
//       3. isValid(token) — verify
// ═══════════════════════════════════════════════════════════════════════
//
// ID CARD FACTORY ANALOGY:
//    AuthController (HR)
//         │  "User Arpan ka token chahiye"
//         ▼
//    ┌────────────────────────────────┐
//    │  JwtService (Factory)           │
//    │                                  │
//    │  Card pe likhta:                 │
//    │  ┌──────────────────────────┐   │
//    │  │ sub: "5"     (user id)   │   │
//    │  │ name: "Arpan"            │   │
//    │  │ email: "arpan@x.com"     │   │
//    │  │ role: "ADMIN"            │   │
//    │  │ iat: 2026-05-15...       │   │
//    │  │ exp: +15 min             │   │
//    │  │ [SIGNATURE: HMAC-SHA256] │   │
//    │  └──────────────────────────┘   │
//    └────────────────────────────────┘
//         │
//         ▼
//    Final signed token string
//
// JWT STRUCTURE:
//    eyJhbGc...header... . eyJzdWIi...payload... . sig...signature
//    ─────────────────── ─────────────────────── ─────────────────
//         HEADER             PAYLOAD                 SIGNATURE
//    (algorithm: HS256)   (user claims)        (HMAC of header+payload)
//
// 5 METHODS:
//    1. generateToken(user)  → user → signed JWT string
//    2. extractUserId(token) → token → "5" (sub)
//    3. extractEmail(token)  → token → "arpan@x.com"
//    4. extractRole(token)   → token → "ADMIN"
//    5. isValid(token)       → token → true/false (signature + expiry)
//
//    + Private helpers:
//       getSigningKey()  → SecretKey object banata
//       parseClaims()    → parse + verify (DRY for extracts)
//
// generateToken() — Builder Pattern:
//    Jwts.builder()
//        .subject(userId)        ← sub claim
//        .claim("name", ...)
//        .claim("email", ...)
//        .claim("role", ...)
//        .issuedAt(now)           ← iat
//        .expiration(now + 15m)   ← exp
//        .signWith(secret)        ← stamp
//        .compact();              ← final string
//
//    = Fluent chain (each returns Builder)
//    = .compact() = final immutable JWT string
//
// isValid() — Verification Flow:
//    parseClaims(token) checks ALL:
//       Signature match (tampering detect)
//       Expiration date
//       Format (3 parts dot-separated)
//       Not empty/null
//
//    Any fail → JwtException → return false
//
// parseClaims() — DRY helper:
//    Jwts.parser()
//        .verifyWith(getSigningKey())    ← verify with SECRET
//        .build()
//        .parseSignedClaims(token)        ← throws if invalid
//        .getPayload();                   ← claims map
//
//    parseSignedClaims = MANDATORY signature check
//    parseClaims (old) = skip verification (DON'T USE)
//
// SECRET KEY:
//    @Value("${jwt.secret}") → from application.properties
//    String → byte[] → SecretKey
//    Min 256 bits (32 chars) for HS256 — JJWT enforces
//
//    Production:
//       NEVER hardcode in code
//       Environment variable / AWS Secrets Manager
//       application.properties just for dev
//
// TAMPERING DETECTION FLOW:
//    Original: payload = {role: "USER"}
//              signature = HMAC(payload + secret) = "abc123"
//
//    Hacker modifies → role: "ADMIN"
//    Server verifies:
//       Recompute signature with secret = "xyz789"
//       Token has = "abc123"
//       xyz789 ≠ abc123 → SignatureException
//       = REJECTED
//
//    Hacker can't fake signature (no SECRET access)
//
// HS256 vs RS256:
//    HS256 (Symmetric — used here):
//       Same SECRET for sign + verify
//       Single app scenario
//       Simple
//
//    RS256 (Asymmetric):
//       Private key signs, public key verifies
//       Microservices (one signer, many verifiers)
//       More complex, more secure
//       Industry default for distributed systems
//
// PATTERN: BUILDER (used via Jwts.builder())
//
// SOLID — SRP:
//    Sirf JWT operations
//    User loading → CustomUserDetailsService
//    Filtering → JwtFilter
//    Config → SecurityConfig
//    Endpoints → AuthController
//    = Each class one job
//
// INTERVIEW LINE:
//    "JwtService manages 3 core operations:
//     • Generate — Jwts.builder() Builder pattern with claims,
//                  expiry, HMAC-SHA256 signature
//     • Parse + Extract — verifyWith(secret) ensures signature valid
//     • Validate — true if signature + expiry pass
//
//     Uses JJWT 0.12.x library. HS256 (symmetric) — same SECRET both sides.
//     For microservices, RS256 (asymmetric) preferred — public key verification.
//
//     SECRET loaded via @Value — production needs env vars or secrets manager,
//     never hardcode."
// ═══════════════════════════════════════════════════════════════════════

@Service
public class JwtService {

    // Config injection from application.properties
    // jwt.secret     → signing key
    // jwt.expiration → 15 min in milliseconds
    @Value("${jwt.secret}")
    private String secret;

    @Value("${jwt.expiration}")
    private long expiration;

    // ════════════════════════════════════════════════════════════
    //  HELPER: String secret → SecretKey object
    // ════════════════════════════════════════════════════════════
    //  jjwt 0.12.x raw String accept nahi karta sign karne ke liye.
    //  HMAC-SHA algorithms ko proper SecretKey object chahiye.
    //
    //  Process:
    //      "myStringSecret..." (config se)
    //           ↓ getBytes()
    //      byte[] keyBytes
    //           ↓ Keys.hmacShaKeyFor()
    //      SecretKey object (ready for signing)
    private SecretKey getSigningKey() {
        byte[] keyBytes = secret.getBytes();
        return Keys.hmacShaKeyFor(keyBytes);
    }

    // ════════════════════════════════════════════════════════════
    //  1  GENERATE TOKEN — User → New ID Card
    // ════════════════════════════════════════════════════════════
    //  Called by: AuthController.login() jab credentials match ho.
    //
    //  Output structure:
    //      header  = { alg: "HS256", typ: "JWT" }
    //      payload = { sub, name, email, role, iat, exp }
    //      signature = HMAC-SHA256(header + "." + payload, SECRET)
    //
    //      final = base64(header) + "." + base64(payload) + "." + signature
    public String generateToken(User user) {
        Date now = new Date();
        Date expiryDate = new Date(now.getTime() + expiration);

        return Jwts.builder()
                .subject(user.getId().toString())   // sub claim
                .claim("name", user.getName())
                .claim("email", user.getEmail())
                .claim("role", user.getRole())       // authorization
                .issuedAt(now)                       // iat
                .expiration(expiryDate)              // exp
                .signWith(getSigningKey())           // stamp
                .compact();                          // final string
    }

    // ════════════════════════════════════════════════════════════
    //  2  EXTRACT USER ID — Card pe ID number padho
    // ════════════════════════════════════════════════════════════
    //  Token se sub claim (userId) nikaalta.
    //  Used when controller mein "current user ka data" fetch ho.
    public String extractUserId(String token) {
        Claims claims = parseClaims(token);
        return claims.getSubject();   // sub claim
    }

    // ════════════════════════════════════════════════════════════
    //  3  EXTRACT EMAIL — Card pe email padho
    // ════════════════════════════════════════════════════════════
    //  JwtFilter use karta — token verify hone ke baad email nikaalta
    //  CustomUserDetailsService.loadUserByUsername() ko pass karne ke liye.
    public String extractEmail(String token) {
        Claims claims = parseClaims(token);
        return claims.get("email", String.class);
    }

    // ════════════════════════════════════════════════════════════
    //  4  EXTRACT ROLE — Card pe role/permissions padho
    // ════════════════════════════════════════════════════════════
    public String extractRole(String token) {
        Claims claims = parseClaims(token);
        return claims.get("role", String.class);
    }

    // ════════════════════════════════════════════════════════════
    //  5  IS VALID — Card asli hai? (Signature + Expiry check)
    // ════════════════════════════════════════════════════════════
    //  Checks (all done by parseClaims):
    //      • Signature recompute → match? (tampering detect)
    //      • exp claim check    → already expired?
    //      • Format check       → 3 parts dot-separated?
    //
    //  Koi bhi check fail = JwtException → false return
    //  Called by: JwtFilter on every protected request
    public boolean isValid(String token) {
        try {
            parseClaims(token);   // throws if invalid/expired/tampered
            return true;
        } catch (JwtException | IllegalArgumentException e) {
            // JwtException covers: signature, expired, malformed, unsupported
            // IllegalArgumentException: empty/null token
            return false;
        }
    }

    // ════════════════════════════════════════════════════════════
    //  PRIVATE HELPER — parseClaims (DRY)
    // ════════════════════════════════════════════════════════════
    //  Common parsing logic — saare extract methods + isValid use karte.
    //
    //  Process:
    //      token string
    //          ↓ parser with verifyWith(SECRET)
    //      signature verify hota
    //          ↓
    //      tampered? expired? → JwtException throw
    //          ↓
    //      claims (payload) return
    //
    //  parseSignedClaims() = MANDATORY signature check
    //                         Unsigned tokens reject
    private Claims parseClaims(String token) {
        return Jwts.parser()
                .verifyWith(getSigningKey())   // stamp verify with our SECRET
                .build()
                .parseSignedClaims(token)       // throws if invalid
                .getPayload();                  // claims map return
    }
}
