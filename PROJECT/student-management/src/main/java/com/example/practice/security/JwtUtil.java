package com.example.practice.security;

/*
 * ============================================================
 * YE KYA HAI: JWT Utility — token banana, verify karna, username nikalna.
 * KYUN CHAHIYE: Stateless auth ke liye JWT zaroori. Ye helper class sab handle karta.
 * KYA KARTA: Login pe token generate, har request pe token verify, token se username extract.
 * NAHI HUA TO: Login ke baad client ko kya doge? Session? Server pe state rakhoge? Scalable nahi.
 *
 * REAL WORLD ANALOGY — JWT = MOVIE TICKET:
 *   Login   = ticket counter pe jaao, ID dikhaao → ticket mile (JWT token).
 *   Ticket pe likha: naam (payload), show time (expiry), counter stamp (signature).
 *   Har screen pe (API request pe): ticket dikhaao → guard (JwtFilter) verify kare:
 *     - Naam sahi? (username match)
 *     - Show time baaki? (not expired)
 *     - Stamp asli? (signature valid — tampered nahi)
 *     Sab sahi → andar jaao. Ek bhi galat → BAHAR NIKLO.
 *
 *   SERVER PE KUCH STORE NAHI — ticket mein SAB info hai.
 *   10 cinema halls (10 servers) — kisi mein bhi ticket dikhaao, chalega.
 *   Ye = STATELESS. Horizontally scalable.
 *
 * JWT TOKEN STRUCTURE (3 parts, dot separated):
 *   eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJBcnBhbiJ9.abc123signature
 *   |--- HEADER ---|    |--- PAYLOAD ---|    |--- SIGNATURE ---|
 *
 *   HEADER:    { "alg": "HS256" }           — algorithm batata
 *   PAYLOAD:   { "sub": "Arpan", "exp": ..., "iat": ... }  — data (claims)
 *              sub = subject (username), exp = expiry, iat = issued at
 *   SIGNATURE: HMAC-SHA256(base64(header) + "." + base64(payload), SECRET_KEY)
 *              — tamper proof. Token modify karo → signature match nahi → INVALID.
 *
 *   IMPORTANT: Payload ENCRYPTED nahi hai — sirf BASE64 encoded.
 *   Koi bhi decode kar sakta (jwt.io pe jaake dekho).
 *   SENSITIVE data mat daalo (password, card number).
 *   Sirf username, role, expiry daalo.
 * ============================================================
 */

import io.jsonwebtoken.*;
import io.jsonwebtoken.security.Keys;
import java.security.Key;
import java.util.Date;

public class JwtUtil {
    // @Component NAHI lagaya — kyunki static methods hain, bean ki zaroorat nahi.
    // Seedha JwtUtil.generateToken() call kar sakte bina object banaye.
    // Agar @Component lagate toh Spring bean banata — but static methods ko bean ki need nahi.

    /*
     * ============================================================
     * SECRET KEY
     *
     * YE KYA HAI: Token SIGN karne ki key. Sirf server ke paas honi chahiye — TOP SECRET.
     *
     * KYUN LAGAYA: Iske bina koi bhi fake token bana le. Secret se SIGNATURE banti — tamper proof.
     *   Server sign karta secret se → client ke paas token jaata → client modify kare toh
     *   signature match nahi hogi (kyunki client ke paas secret NAHI hai) → CAUGHT.
     *
     * NAHI LAGAYA TO (weak/leaked SECRET):
     *   - Attacker ko SECRET pata chal gaya → apna token bana lega → kisi bhi user ban jaayega
     *   - Admin ban jaaye, data delete kare, sab kuch kar sakta — COMPLETE TAKEOVER
     *   - Ye = "master key" hai — leak hui toh poora system compromise
     *
     * REAL WORLD ANALOGY:
     *   SECRET = BANK ka STAMP (official seal).
     *   Stamp se cheque sign karo → bank accept kare.
     *   Stamp chori ho gaya → chor fake cheques sign kare → bank ka paisa loot le.
     *   Isliye stamp SAFE rakhna ZAROORI = secret SAFE rakhna ZAROORI.
     *
     * PRODUCTION MEIN:
     *   - Code mein KABHI nahi rakhna (git mein push ho jaayega → sab dekhenge)
     *   - Environment variable se laao: System.getenv("JWT_SECRET")
     *   - Ya application.properties se: @Value("${jwt.secret}")
     *   - Minimum 256 bits (32 characters) for HS256 algorithm
     * ============================================================
     */
    private static final String SECRET = "mySecretKeyWhichIsAtLeast32CharsLong!!";

    /*
     * ============================================================
     * EXPIRATION — Token Expiry Time
     *
     * YE KYA HAI: Token kitni der valid rahega — 1 hour (1000ms * 60sec * 60min).
     *
     * KYUN 1 HOUR?: Security aur convenience ka balance.
     *   - Token leak ho gaya → sirf 1 ghante tak damage. Uske baad DEAD.
     *   - User ko har 1 ghante mein re-login → tolerable.
     *
     * NAHI LAGAYA TO (koi expiry nahi):
     *   - Token FOREVER valid → ek baar leak = PERMANENT access
     *   - User ne 5 saal pehle login kiya → aaj bhi token chalega → DANGEROUS
     *   - Revoke karne ka koi tarika nahi (stateless — server pe store nahi)
     *
     * REAL WORLD ANALOGY:
     *   EXPIRATION = PARKING TICKET ki validity.
     *   "Valid for 1 hour" — 1 ghante baad guard bole "ticket expire, naya lo."
     *   Bina validity: ek baar ticket liya → zindagi bhar free parking → system collapse.
     *
     * REAL APPS PATTERN:
     *   Access token  = 15 min (short, API calls ke liye)
     *   Refresh token = 7 days (long, naya access token lene ke liye)
     *   Access expire → refresh se naya access lo → user ko pata bhi nahi chalta.
     * ============================================================
     */
    private static final long EXPIRATION = 1000 * 60 * 60; // 1 hour in milliseconds

    /*
     * getKey() — Secret string se HMAC signing key banao.
     * SECRET string → bytes → HMAC-SHA key object.
     * HMAC = Hash-based Message Authentication Code.
     * Ye key token sign karne aur verify karne DONO mein use hoti (symmetric = same key both sides).
     */
    private static Key getKey() {
        return Keys.hmacShaKeyFor(SECRET.getBytes());
    }

    // ==========================================
    // TOKEN GENERATE — login ke baad user ko JWT dena
    // ==========================================
    public static String generateToken(String username) {
        /*
         * REAL WORLD ANALOGY: Ticket print ho rahi:
         *   Naam: Arpan (subject)
         *   Print time: abhi (issuedAt)
         *   Valid till: 1 hour baad (expiration)
         *   Counter stamp: HMAC-SHA256 sign (tamper proof)
         */
        return Jwts.builder()                            // JWT builder start
                .setSubject(username)                    // PAYLOAD: "sub": "Arpan" — kaun hai
                .setIssuedAt(new Date())                 // PAYLOAD: "iat": 1234567890 — kab bana
                .setExpiration(new Date(System.currentTimeMillis() + EXPIRATION))  // PAYLOAD: "exp": ... — kab expire
                .signWith(getKey(), SignatureAlgorithm.HS256)   // SIGNATURE: HMAC-SHA256 se sign
                .compact();                              // Build → "eyJhbG..." string return
        /*
         * OUTPUT: "eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJBcnBhbiIsImlhdCI6MTY4...,ImV4cCI6MTY4...}.signature"
         * Client ye save karega. Har request mein header mein bhejega:
         *   Authorization: Bearer eyJhbGciOiJIUzI1NiJ9...
         */
    }

    // ==========================================
    // USERNAME EXTRACT — token se username nikalo
    // ==========================================
    public static String extractUsername(String token) {
        /*
         * Token parse karo → signature verify → claims nikalo → subject (username) return.
         * AGAR TOKEN TAMPERED: signature match nahi → SignatureException throw → token REJECTED.
         * AGAR TOKEN EXPIRED: expiry check fail → ExpiredJwtException throw → token REJECTED.
         */
        return Jwts.parserBuilder()
                .setSigningKey(getKey())       // SAME key se verify (symmetric — sign bhi isi se, verify bhi isi se)
                .build()
                .parseClaimsJws(token)         // Parse + signature verify — tampered toh exception
                .getBody()                     // Claims body nikalo (payload)
                .getSubject();                 // "sub" claim = username return
    }

    // ==========================================
    // TOKEN VALID CHECK — username match + not expired
    // ==========================================
    public static boolean isTokenValid(String token, String username) {
        String extractedUser = extractUsername(token);
        return extractedUser.equals(username) && !isTokenExpired(token);
        /*
         * 2 checks:
         *   1. Token mein ka username == expected username? (identity match)
         *   2. Token expired nahi hai? (time valid)
         *   Dono true → VALID. Ek bhi false → INVALID → access deny.
         *
         * ANALOGY: Ticket pe naam "Arpan" + valid time → sab sahi → ENTER.
         *   Naam galat ya time expire → REJECT.
         */
    }

    // Token expire hua ya nahi
    private static boolean isTokenExpired(String token) {
        Date expiration = Jwts.parserBuilder()
                .setSigningKey(getKey())
                .build()
                .parseClaimsJws(token)
                .getBody()
                .getExpiration();
        return expiration.before(new Date());
        // Expiry time < current time → EXPIRED → true → token DEAD
    }
}
