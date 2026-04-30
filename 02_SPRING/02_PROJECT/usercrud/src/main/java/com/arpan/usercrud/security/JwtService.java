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

/* ════════════════════════════════════════════════════════════════════
 *  🎫 JwtService — THE ID CARD PRINTING OFFICE
 * ════════════════════════════════════════════════════════════════════
 *
 *  Yeh class ID card factory hai — token banata, padhta, verify karta.
 *  JwtFilter (the guard) aur AuthController (HR jo cards issue karta)
 *  iska use karte hain.
 *
 *  ─── 🏭 ANALOGY: Office ID Card Factory ───────────────────────────
 *
 *      Imagine office ka ID card section:
 *
 *         👨‍💼 HR (AuthController)
 *              │
 *              │  "New employee Arpan ka ID card chahiye"
 *              ▼
 *         🏭 ID Card Factory (JwtService)
 *              │
 *              │  Card pe likhta:
 *              │  ┌──────────────────────────┐
 *              │  │  Name: Arpan             │  ← claim
 *              │  │  Email: arpan@x.com      │  ← claim
 *              │  │  Role: ADMIN             │  ← claim
 *              │  │  Issue: 2026-04-30       │  ← iat
 *              │  │  Expiry: 2026-04-30 +15m │  ← exp
 *              │  │                          │
 *              │  │  [STAMP: Konovo HMAC]    │  ← signature
 *              │  │                          │
 *              │  └──────────────────────────┘
 *              │
 *              ▼
 *         🎫 Final Card (signed JWT)
 *
 *      Stamp ka logic: ek SECRET stamp hai jo sirf factory ke paas
 *      hai. Koi nakli card banaye → stamp fake hoga → verify fail
 *      → guard pakad lega.
 *
 *  ─── 🧰 3 MAIN KAAM (3 public methods) ────────────────────────────
 *
 *      1️⃣  generateToken(user)  → naya card banao (user info se)
 *      2️⃣  isValid(token)       → card asli hai? (stamp check)
 *      3️⃣  extractEmail(token)  → card pe naam padho
 *
 *      + helper: extractUserId, extractRole
 *
 *  ─── 🔐 STAMP (Signature) KAISE KAAM KARTA ────────────────────────
 *
 *      signature = HMAC-SHA256(
 *                       header + "." + payload,
 *                       SECRET
 *                  )
 *
 *      Tampering scenario:
 *
 *         Hacker card mein "role: USER" ko "role: ADMIN" mein change kare
 *              │
 *              ▼
 *         Server scanner stamp recompute kare apne SECRET se
 *              │
 *              ▼
 *         Naya hash = "xyz123"
 *         Card pe stamp = "abc456" ← original
 *              │
 *              ▼
 *         xyz123 ≠ abc456 → 🚫 TAMPERING DETECTED
 *
 *      Hacker SECRET nahi jaanta → wo new stamp generate nahi kar
 *      sakta → tampering possible NAHI.
 *
 *  ─── 📚 LIBRARY: jjwt (io.jsonwebtoken) 0.12.x ────────────────────
 *
 *      • Jwts.builder()  → naya token banao
 *      • Jwts.parser()   → token parse + verify
 *      • SecretKey       → signing key object (256-bit minimum)
 *      • Claims          → token payload (key-value map)
 *
 *  ════════════════════════════════════════════════════════════════════
 *  🎤 INTERVIEW TALKING POINT — JwtService
 *  ════════════════════════════════════════════════════════════════════
 *
 *  Q: "JWT generate aur validate kaise karte ho code mein?"
 *
 *  Tu bolega:
 *  "Maine dedicated `JwtService` banaya jo 3 main kaam karta —
 *   token generate, parse, aur validate. Library use ki `jjwt`
 *   (io.jsonwebtoken 0.12.x) — industry standard.
 *
 *   `generateToken(user)` mein `Jwts.builder()` use karke claims
 *   set karta — sub (userId), email, name, role — `signWith()`
 *   se HMAC SHA-256 signature lagata SECRET key se. `compact()`
 *   final string return karta.
 *
 *   Validation `parseSignedClaims()` se hota — agar signature
 *   tampered ya token expired, JwtException throw hoti, hum
 *   false return karte.
 *
 *   SECRET key `application.properties` se `@Value` se inject
 *   hoti. Production mein env variable / AWS Secrets Manager se
 *   load karte hain — code mein hardcode NEVER."
 *
 *  Q: "256-bit secret kyu mandatory?"
 *  → "HMAC SHA-256 minimum 256 bits chahiye — chhota key
 *     'weak key' exception throw karega jjwt mein."
 *
 *  Q: "Symmetric vs Asymmetric — kya use kiya?"
 *  → "HS256 (symmetric) — same SECRET both sides. Single-app
 *     scenarios ke liye fine. Microservices mein RS256
 *     (asymmetric, public/private key) prefer karte."
 *  ════════════════════════════════════════════════════════════════════
 */
@Service
public class JwtService {

    /*
     *  🔑 Config injection from application.properties
     *
     *  jwt.secret     → "Konovo Stamp" — yeh signing key
     *  jwt.expiration → 15 min in milliseconds
     */
    @Value("${jwt.secret}")
    private String secret;

    @Value("${jwt.expiration}")
    private long expiration;

    // ════════════════════════════════════════════════════════════
    //  🛠️ HELPER: String secret → SecretKey object
    // ════════════════════════════════════════════════════════════
    /*
     *  jjwt 0.12.x raw String accept nahi karta sign karne ke liye.
     *  HMAC-SHA algorithms ko proper SecretKey object chahiye.
     *
     *  Process:
     *      "myStringSecret..." (config se)
     *           ↓ getBytes()
     *      byte[] keyBytes
     *           ↓ Keys.hmacShaKeyFor()
     *      SecretKey object (ready for signing)
     *
     *  Note: Agar secret base64-encoded hai → Decoders.BASE64.decode()
     *        Hum plain string use kar rahe → directly getBytes().
     */
    private SecretKey getSigningKey() {
        byte[] keyBytes = secret.getBytes();
        return Keys.hmacShaKeyFor(keyBytes);
    }

    // ════════════════════════════════════════════════════════════
    //  1️⃣  GENERATE TOKEN — User → New ID Card
    // ════════════════════════════════════════════════════════════
    /*
     *  🎫 Card factory ka main kaam — user info leke signed JWT banao.
     *
     *  Called by: AuthController.login() jab credentials match ho.
     *
     *  Output structure:
     *      header  = { alg: "HS256", typ: "JWT" }
     *      payload = { sub: "5", name: "Arpan", email: "arpan@x.com",
     *                  role: "ADMIN", iat: ..., exp: ... }
     *      signature = HMAC-SHA256(header + "." + payload, SECRET)
     *
     *      final = base64(header) + "." + base64(payload) + "." + signature
     */
    public String generateToken(User user) {
        Date now = new Date();
        Date expiryDate = new Date(now.getTime() + expiration);

        return Jwts.builder()
                // 📛 Subject — primary identifier (userId convention)
                .subject(user.getId().toString())

                // 🏷️ Custom claims — user info embed
                .claim("name", user.getName())
                .claim("email", user.getEmail())
                .claim("role", user.getRole())     // important: authorization ke liye

                // 🕐 Time stamps
                .issuedAt(now)                      // iat: kab issue hua
                .expiration(expiryDate)             // exp: kab expire (now + 15 min)

                // 🔏 Stamp lagao — HMAC SHA-256 with our SECRET
                .signWith(getSigningKey())

                // 📦 Final string banao — header.payload.signature
                .compact();
    }

    // ════════════════════════════════════════════════════════════
    //  2️⃣  EXTRACT USER ID — Card pe ID number padho
    // ════════════════════════════════════════════════════════════
    /*
     *  Token se sub claim (userId) nikaalta.
     *  Used when controller mein "current user ka data" fetch karna ho.
     */
    public String extractUserId(String token) {
        Claims claims = parseClaims(token);
        return claims.getSubject();   // sub claim
    }

    // ════════════════════════════════════════════════════════════
    //  3️⃣  EXTRACT EMAIL — Card pe email padho
    // ════════════════════════════════════════════════════════════
    /*
     *  JwtFilter use karta — token verify hone ke baad email
     *  nikaalta CustomUserDetailsService.loadUserByUsername() ko pass karne ke liye.
     */
    public String extractEmail(String token) {
        Claims claims = parseClaims(token);
        return claims.get("email", String.class);
    }

    // ════════════════════════════════════════════════════════════
    //  4️⃣  EXTRACT ROLE — Card pe role/permissions padho
    // ════════════════════════════════════════════════════════════
    public String extractRole(String token) {
        Claims claims = parseClaims(token);
        return claims.get("role", String.class);
    }

    // ════════════════════════════════════════════════════════════
    //  5️⃣  IS VALID — Card asli hai? (Signature + Expiry check)
    // ════════════════════════════════════════════════════════════
    /*
     *  🔍 Card scanner — yeh karta hai:
     *
     *      • Signature recompute → match? (tampering detect)
     *      • exp claim check    → already expired?
     *      • Format check       → 3 parts dot-separated?
     *
     *  Koi bhi check fail = JwtException throw → false return.
     *  Sab pass = true.
     *
     *  Called by: JwtFilter on every protected request.
     */
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
    //  🔧 PRIVATE HELPER — parseClaims (DRY)
    // ════════════════════════════════════════════════════════════
    /*
     *  Common parsing logic — saare extract methods + isValid use karte.
     *
     *  Process:
     *      token string
     *          ↓ parser with verifyWith(SECRET)
     *      signature verify hota
     *          ↓
     *      tampered? expired? → JwtException throw
     *          ↓
     *      claims (payload) return
     *
     *  parseSignedClaims() vs parseClaims() — signed version mandatory
     *  signature check karta. Unsigned tokens reject.
     */
    private Claims parseClaims(String token) {
        return Jwts.parser()
                .verifyWith(getSigningKey())   // 🔏 stamp verify with our SECRET
                .build()
                .parseSignedClaims(token)       // throws if invalid
                .getPayload();                  // claims map return
    }
}
