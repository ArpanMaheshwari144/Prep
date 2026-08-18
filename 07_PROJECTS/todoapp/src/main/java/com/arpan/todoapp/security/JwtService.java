package com.arpan.todoapp.security;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/security/JwtService.java
// security/ folder = JWT + auth related yahaan

import com.arpan.todoapp.model.User;
import io.jsonwebtoken.Claims;
import io.jsonwebtoken.JwtException;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.security.Keys;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.security.Key;
import java.util.Date;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    JwtService = JWT lifecycle manager
//    Token generate + verify + extract — sab yahaan
//    Used by: AuthController (login) + JwtFilter (har request)
// ═══════════════════════════════════════════════════════════════════════
//
// HOTEL KEYCARD ANALOGY:
//
//    Hotel Reception     = AuthController.login()
//    Keycard issuance    = JwtService.generateToken()
//    Keycard verification = JwtService.isTokenValid()
//    Guest identification = JwtService.extractEmail()
//    Hotel secret        = jwt.secret (server only)
//    Card expiry         = jwt.expiration (e.g., 15 min)
//
// 6 METHODS TOTAL:
//    PUBLIC (3):
//       1. generateToken    → User → JWT
//       4. extractEmail     → JWT → email
//       6. isTokenValid     → JWT → boolean
//    PRIVATE HELPERS (3):
//       2. getSigningKey    → secret → Key object
//       3. extractAllClaims → JWT → Claims (parse + verify)
//       5. isTokenExpired   → Claims → boolean
// ═══════════════════════════════════════════════════════════════════════

@Service
public class JwtService {

    // ─── CONFIG INJECTION ────────────────────────────────────
    //
    // jwt.secret = signing key (application.properties se aayega)
    //              Min 32 chars (HMAC-SHA256 needs ≥ 256-bit)
    //
    // jwt.expiration = token validity (milliseconds)
    //                  900000 ms = 15 min
    @Value("${jwt.secret}")
    private String secret;

    @Value("${jwt.expiration}")
    private long expiration;


    // ═══════════════════════════════════════════════════════════
    //  METHOD 1: generateToken — User → signed JWT string
    // ═══════════════════════════════════════════════════════════
    //
    // PURPOSE:
    //    User le ke signed JWT banata
    //    Token mein: email (identity) + expiration
    //    Sign with HMAC-SHA256 + secret
    //
    // CALLED BY:
    //    AuthController.login() — credentials verify ke baad
    //    "User genuine = token issue karo"
    //
    // FLOW:
    //    User IN
    //       ↓
    //    Jwts.builder() — fluent builder pattern
    //       ↓ .subject(email)        ← who am I
    //       ↓ .issuedAt(now)         ← when issued
    //       ↓ .expiration(now+15min) ← when invalid
    //       ↓ .signWith(secretKey)   ← HMAC-SHA256 signature
    //       ↓ .compact()              ← produce string
    //    JWT String OUT
    //
    // CLAIMS LOGIC:
    //    subject = user's email
    //       Email = unique identifier (DB constraint)
    //       Login flow email-based
    //       Consistent identity across system
    //
    //    issuedAt = current timestamp
    //       Standard "iat" claim (JWT spec)
    //
    //    expiration = current + configured period
    //       System.currentTimeMillis() = Unix epoch ms
    //       + expiration (e.g., 900000) = future timestamp
    //       new Date(timestamp) wraps for jjwt
    //
    // SIGNING:
    //    .signWith(getSigningKey())
    //       Returns SecretKey from raw secret
    //       jjwt auto-detects HS256 from key size
    //
    // OUTPUT:
    //    "eyJhbGc...eyJzdWIi...K3vIqDf..."
    //    = header.payload.signature (base64url)
    public String generateToken(User user) {
        return Jwts.builder()
                // subject = identity claim (email)
                .subject(user.getEmail())
                // iat = issued-at timestamp
                .issuedAt(new Date())
                // exp = current + configured window
                .expiration(new Date(System.currentTimeMillis() + expiration))
                // sign with HMAC-SHA256
                .signWith(getSigningKey())
                // produce compact base64url string
                .compact();
    }

    // ═══════════════════════════════════════════════════════════
    //  METHOD 2 (helper): getSigningKey — secret → SecretKey
    // ═══════════════════════════════════════════════════════════
    //
    // PURPOSE:
    //    Spring inject karta jwt.secret as String
    //    BUT jjwt ka signWith() + verifyWith() SecretKey expect karte
    //    (String accept nahi karte directly)
    //    Yeh helper conversion karta
    //
    // CALLED BY:
    //    generateToken()     — sign karne ke liye (HMAC)
    //    extractAllClaims()  — verify karne ke liye (same HMAC)
    //
    //    = SAME key BOTH ways (HMAC = symmetric algorithm)
    //
    // WHY 32 CHARS MINIMUM:
    //    HMAC-SHA256 algorithm needs ≥ 256-bit (= 32 bytes = 32 ASCII chars)
    //    Smaller secret = WeakKeyException at runtime
    //
    //    Local dev: any 32+ char string works
    //    Production: random 64+ char from env / secrets manager
    //
    // SAME KEY BOTH WAYS:
    //    HMAC is SYMMETRIC (server holds the only secret)
    //    Sign + verify use SAME key
    //
    //    Asymmetric alternatives (RS256):
    //       Private key signs, public key verifies
    //       Use for distributed scenarios
    //    Hum HMAC use kar rahe (simpler, faster)
    //
    // PRIVATE because:
    //    Internal detail of JwtService
    //    Controller/Filter ko Key object directly nahi chahiye
    //    Sirf JwtService internal methods use karte
    private Key getSigningKey() {
        // String secret → UTF-8 byte array
        // (crypto algorithms bytes pe operate karte, text pe nahi)
        byte[] keyBytes = secret.getBytes();

        // Keys.hmacShaKeyFor() = jjwt factory
        //    Takes byte[] → returns SecretKey
        //    Auto-picks algorithm based on length:
        //       32 bytes (256 bit) → HMAC-SHA256 (our case)
        //       48 bytes (384 bit) → HMAC-SHA384
        //       64 bytes (512 bit) → HMAC-SHA512
        //    < 32 bytes → WeakKeyException
        //
        // Return type Key (parent interface) — caller agnostic
        return Keys.hmacShaKeyFor(keyBytes);
    }

    // ═══════════════════════════════════════════════════════════
    //  METHOD 3 (helper): extractAllClaims — JWT → Claims
    // ═══════════════════════════════════════════════════════════
    //
    // PURPOSE:
    //    Signed JWT token parse karta + signature verify karta
    //    Returns: Claims object (payload data — sub, iat, exp)
    //
    // CALLED BY:
    //    extractEmail()      — uses Claims.getSubject()
    //    isTokenExpired()    — uses Claims.getExpiration()
    //    = DRY — single parsing point (caller specific claim plucks)
    //
    // WHAT parseSignedClaims() DOES INTERNALLY:
    //    1. Token ko 3 parts mein split (header.payload.signature)
    //    2. Base64-decode header + payload
    //    3. Compute HMAC: HMAC(header.payload, getSigningKey())
    //    4. Compare with token's signature
    //       Match? → continue
    //       Mismatch? → SignatureException THROW
    //    5. Check expiration claim
    //       Future? → continue
    //       Past?   → ExpiredJwtException THROW
    //    6. Return Jws<Claims> wrapper
    //    → .getPayload() unwraps Claims
    //
    // EXCEPTIONS jjwt auto-throws:
    //    SignatureException     → tampered token
    //    ExpiredJwtException    → token expired
    //    MalformedJwtException  → broken structure
    //    UnsupportedJwtException → wrong algorithm
    //    IllegalArgumentException → null/empty
    //
    //    Sab JwtException ke subclass
    //    JwtFilter mein catch hoga (isTokenValid wrapper)
    //
    // PRIVATE because:
    //    Internal parsing logic
    //    Caller specific claim chahiye (email or expiration)
    //    Helper saari parsing wrap karta — DRY
    private Claims extractAllClaims(String token) {
        return Jwts.parser()
                // Verify with same key used to sign
                // (HMAC symmetric — sign + verify same secret)
                // Cast Key → SecretKey (jjwt parser specific API)
                .verifyWith((javax.crypto.SecretKey) getSigningKey())
                // Finalize parser builder → JwtParser instance
                .build()
                // Parse + verify signature + check expiration
                // Throws on tampered/expired/malformed
                .parseSignedClaims(token)
                // Get Claims payload (sub, iat, exp, custom)
                .getPayload();
    }

    // ═══════════════════════════════════════════════════════════
    //  METHOD 4: extractEmail — JWT → User's email
    // ═══════════════════════════════════════════════════════════
    //
    // PURPOSE:
    //    Signed JWT le ke email (subject claim) return karta
    //    Email = user identity (we set this in generateToken's .subject())
    //
    // CALLED BY:
    //    JwtFilter — har request pe:
    //       1. Authorization header se token nikalta ("Bearer xxx")
    //       2. extractEmail(token) → email mil gaya
    //       3. UserDetailsService.loadByUsername(email) → User
    //       4. SecurityContextHolder.set(authentication)
    //
    // 1-LINE METHOD KYU:
    //    extractAllClaims = heavy work (parse + verify)
    //    extractEmail = thin wrapper (just pluck subject)
    //
    //    Benefits:
    //       Caller-friendly API
    //          JwtFilter: jwtService.extractEmail(token)
    //          NOT: jwtService.extractAllClaims(token).getSubject()
    //       Future flexibility
    //          Agar subject change to userId (future)
    //          Sirf yahaan update, callers unchanged
    //       Abstraction
    //          Helper internals (parsing, exceptions) hidden
    //
    // EXCEPTION:
    //    Token invalid/expired → extractAllClaims throws
    //    Hum yahaan catch nahi karte
    //    JwtFilter's isTokenValid wrapper catch karega
    public String extractEmail(String token) {
        // extractAllClaims = parse + verify (private helper)
        // .getSubject() = jjwt API → "sub" claim value
        // We set this as user.getEmail() in generateToken
        return extractAllClaims(token).getSubject();
    }

    // ═══════════════════════════════════════════════════════════
    //  METHOD 5 (helper): isTokenExpired — past time check
    // ═══════════════════════════════════════════════════════════
    //
    // PURPOSE:
    //    Token ka "exp" claim past mein hai? Boolean return.
    //    true  = expired (reject)
    //    false = still valid
    //
    // CALLED BY:
    //    isTokenValid() — public wrapper iss helper use karta
    //
    // FLOW:
    //    Token IN
    //       ↓
    //    extractAllClaims(token).getExpiration() → Date
    //       ↓
    //    Compare with new Date() (current time)
    //       ↓
    //    true (expired) / false (valid)
    //
    // NOTE — Double-Layer Protection:
    //    jjwt's parseSignedClaims() bhi expiration check karta
    //    ExpiredJwtException auto-throws on expired tokens
    //
    //    Phir bhi yeh explicit method kyu?
    //       1. Code readability — clear intent in isTokenValid
    //       2. Future flexibility (custom expiration logic)
    //       3. Defensive multi-layer check
    //
    // Date.before(other) LOGIC:
    //    Returns true if "this date is BEFORE other date"
    //
    //    expirationDate.before(now)?
    //       expirationDate is in PAST → expired → return true
    //       expirationDate is in FUTURE → valid → return false
    //
    // PRIVATE because:
    //    Internal validation step
    //    isTokenValid wrapper public hai — caller uses that
    private boolean isTokenExpired(String token) {
        // Token se "exp" claim nikalo (set in generateToken)
        Date expirationDate = extractAllClaims(token).getExpiration();
        // Date.before(newDate()) = "exp time pehle aaya than now?"
        // true = expired, false = valid
        return expirationDate.before(new Date());
    }

    // ═══════════════════════════════════════════════════════════
    //  METHOD 6: isTokenValid — full validation (sign + expiry)
    // ═══════════════════════════════════════════════════════════
    //
    // PURPOSE:
    //    Full token validation:
    //       Signature genuine + Not expired = true
    //       Koi bhi fail = false
    //
    // CALLED BY:
    //    JwtFilter — har request pe gate-check
    //    "Yeh token accept karu ya 401 dedu?"
    //
    // FLOW:
    //    Token IN
    //       ↓
    //    try {
    //       isTokenExpired(token)
    //          ↓ internally calls extractAllClaims
    //          ↓   - parseSignedClaims (verify signature)
    //          ↓   - throws on invalid/expired
    //          ↓ getExpiration() → Date
    //          ↓ before(now)? → boolean
    //       !expired = NOT expired = VALID
    //       return true
    //    }
    //    catch (JwtException) {
    //       // Caught ANY JWT error → reject
    //       return false
    //    }
    //
    // EXCEPTION HANDLING — Single catch:
    //    JwtException = PARENT class
    //    Catches ALL subtypes:
    //       SignatureException     → tampered
    //       ExpiredJwtException    → expired
    //       MalformedJwtException  → broken
    //       UnsupportedJwtException → wrong algo
    //       IllegalArgumentException → null/empty
    //
    //    Single catch = clean boolean API
    //    JwtFilter gets simple true/false
    //
    // 3 SCENARIOS:
    //
    //    SCENARIO 1 — Genuine + active:
    //       Signature + Not expired → true
    //
    //    SCENARIO 2 — Genuine but expired:
    //       parseSignedClaims throws ExpiredJwtException
    //       Catch → return false
    //
    //    SCENARIO 3 — Tampered:
    //       parseSignedClaims throws SignatureException
    //       Catch → return false
    public boolean isTokenValid(String token) {
        try {
            // isTokenExpired internally:
            //    1. extractAllClaims → verify signature
            //    2. Check exp claim vs now
            // !expired = NOT expired = valid token
            return !isTokenExpired(token);
        } catch (JwtException e) {
            // Any JWT error caught = invalid token
            // JwtFilter receives clean false → 401
            return false;
        }
    }
}
