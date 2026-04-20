package com.example.practice.controller;

/*
 * ============================================================
 * YE KYA HAI: Auth Controller — login/register endpoints. Password hash. JWT token de.
 * KYUN CHAHIYE: Bina authentication ke koi bhi API access kar lega. Register+login se user verify.
 * KYA KARTA: Register pe password BCrypt se hash karke store. Login pe verify karke JWT token de.
 * NAHI HUA TO: Koi bhi /api/students pe CRUD kar lega — no security, no identity.
 *
 * REAL WORLD ANALOGY:
 *   Building mein ENTRY GATE hai. Guard (AuthController) hai.
 *   Register = naya visitor card banwao (naam + photo store).
 *   Login    = card dikhaao, guard verify kare, PASS de (JWT token).
 *   Baaki floors (APIs) pe jaane ke liye PASS (token) dikhana padega.
 *   Bina AuthController: building mein koi bhi ghus jaaye — no guard, no security.
 *
 * FLOW:
 *   REGISTER: Client → POST /api/auth/register {username, password}
 *             → BCrypt hash → store → "Registered successfully"
 *
 *   LOGIN:    Client → POST /api/auth/login {username, password}
 *             → BCrypt.matches() verify → sahi? → JWT token generate → return
 *             → galat? → 401 Unauthorized
 * ============================================================
 */

import com.example.practice.security.JwtUtil;
import org.springframework.http.ResponseEntity;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.web.bind.annotation.*;
import java.util.*;

@RestController              // REST API — JSON response (explained in StudentController)
@RequestMapping("/api/auth") // Base URL: /api/auth/register, /api/auth/login
public class AuthController {

    /*
     * ============================================================
     * HashMap users — In-memory users (DEMO ke liye)
     *
     * YE KYA HAI: Users ka data HashMap mein store — RAM mein.
     *
     * KYUN LAGAYA: Quick demo ke liye. DB setup ki zaroorat nahi.
     *
     * NAHI LAGAYA TO (koi storage nahi): Register ka data kahan jaayega? Kahi nahi.
     *
     * REAL PROJECT MEIN KYA KARNA CHAHIYE:
     *   - User entity banao (id, username, password, role)
     *   - UserRepository banao (JpaRepository<User, Long>)
     *   - DB mein save karo (MySQL mein "users" table)
     *   - App restart = users SAFE (HashMap mein restart = SAB GONE!)
     *
     * REAL WORLD ANALOGY:
     *   HashMap = KAAGAZ pe naam likhe (temporary, uda toh gaye).
     *   DB = REGISTER BOOK mein naam likhe (permanent, safe).
     *   Production mein kaagaz pe mat likho — register book (DB) use karo.
     * ============================================================
     */
    private Map<String, String> users = new HashMap<>();

    /*
     * ============================================================
     * BCryptPasswordEncoder — Password hashing
     *
     * YE KYA HAI: One-way hash algorithm. Password → hash. Hash → password IMPOSSIBLE (irreversible).
     *
     * KYUN LAGAYA (BCrypt kyun? SHA256 kyun nahi?):
     *   SHA256 PROBLEMS:
     *     1. FAST hai — 1 billion hashes/sec possible → brute force mein seconds lagenge
     *     2. Same input = same output HAMESHA → rainbow table attack possible
     *     3. Salt built-in NAHI — manually lagana padta
     *
     *   BCrypt ADVANTAGES:
     *     1. SLOW hai (intentionally!) — 10 rounds of hashing → brute force mein SAAL lagenge
     *        MD5: 1 billion guesses/sec. BCrypt: ~1000 guesses/sec. Attacker THAK jaayega.
     *     2. SALT auto add karta — same password = ALAG hash har baar
     *        "abc" → "$2a$10$salt1...hash1"  (pehli baar)
     *        "abc" → "$2a$10$salt2...hash2"  (doosri baar — ALAG!)
     *        Rainbow table attack FAIL — pre-computed hashes match nahi honge.
     *     3. matches() mein salt extraction built-in — salt alag store nahi karna padta
     *
     * NAHI LAGAYA TO KYA HOGA (plain text store kiya):
     *   - DB leak ho gayi → SAB passwords visible → sab accounts compromised
     *   - News mein aayega: "XYZ company ka data breach — 1 million passwords leaked"
     *   - Users ne same password doosri sites pe use kiya → chain reaction
     *   - LEGAL consequences — GDPR, data protection laws violate
     *
     * REAL WORLD ANALOGY:
     *   BCrypt = MEAT GRINDER.
     *   Meat (password) daalo → keema (hash) nikle → keema se wapas meat NAHI ban sakta.
     *   SHA256 = BLENDER. Fast hai but kaam same — wapas nahi aa sakta.
     *   BCrypt BETTER kyun? Grinder SLOW chalta (intentional) → chor thak jaaye.
     *   SHA256 blender FAST chalta → chor jaldi try kar le → crack kar le.
     * ============================================================
     */
    private BCryptPasswordEncoder encoder = new BCryptPasswordEncoder();

    // ==========================================
    // REGISTER — naya user banao
    // ==========================================
    @PostMapping("/register")   // POST /api/auth/register
    public ResponseEntity<String> register(@RequestBody Map<String, String> request) {
        /*
         * @RequestBody Map<String, String>:
         *   JSON → Map convert. { "username": "arpan", "password": "abc123" }
         *   Map.get("username") → "arpan"
         *
         *   Real project mein RegisterRequest DTO banao (type safe, validation ke saath).
         *   Map use = quick demo. DTO use = production code.
         *   NAHI LAGAYA TO (@RequestBody nahi): request body parse nahi hoga → Map empty/null.
         */
        String username = request.get("username");
        String password = request.get("password");

        if (users.containsKey(username)) {
            return ResponseEntity.badRequest().body("User already exists");  // 400
        }

        users.put(username, encoder.encode(password));
        /*
         * encoder.encode("abc123") → "$2a$10$randomSaltAndHash..."
         *
         * STORE: { "arpan" → "$2a$10$xYz..." }  <-- HASH store, plain text KABHI nahi.
         *
         * GALAT: users.put(username, password);        <-- PLAIN TEXT = SECURITY DISASTER
         * SAHI:  users.put(username, encoder.encode(password));  <-- HASH = SAFE
         */
        return ResponseEntity.ok("Registered successfully");
    }

    // ==========================================
    // LOGIN — password verify, JWT token do
    // ==========================================
    @PostMapping("/login")   // POST /api/auth/login
    public ResponseEntity<Map<String, String>> login(@RequestBody Map<String, String> request) {
        String username = request.get("username");
        String password = request.get("password");

        String storedHash = users.get(username);
        if (storedHash == null || !encoder.matches(password, storedHash)) {
            /*
             * encoder.matches("abc123", "$2a$10$xYz..."):
             *   YE KYA HAI: Password verify karo — hash compare, plain text compare NAHI.
             *   1. Hash se salt extract karo
             *   2. Input password ko SAME salt se hash karo
             *   3. Dono hash compare karo → match? → true. Nahi? → false.
             *
             *   GALAT: password.equals(storedHash)  — plain text compare = KABHI kaam nahi karega
             *   SAHI:  encoder.matches(password, storedHash) — hash compare = SAFE + correct
             *
             *   NAHI KIYA TO (matches nahi use kiya): Login KABHI success nahi hoga
             *   (plain text != hash — hamesha false)
             */
            return ResponseEntity.status(401).body(Map.of("error", "Invalid credentials"));
            // 401 Unauthorized — "tum kaun ho? Pehchaan nahi" — credentials galat
        }

        String token = JwtUtil.generateToken(username);
        /*
         * JWT TOKEN GENERATE:
         *   Input: "arpan" (username)
         *   Output: "eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJhcnBhbiJ9.abc123"
         *
         *   Client ye token SAVE karega (localStorage/cookie).
         *   Har request mein header mein bhejega: Authorization: Bearer <token>
         *   JwtFilter verify karega — valid? → access. Invalid? → 403.
         *
         *   ANALOGY: Login = movie ticket counter pe ID dikhaao → ticket (JWT) milo.
         *   Ab har screen pe ticket dikhaao → entry milo.
         */
        return ResponseEntity.ok(Map.of("token", token));
        // { "token": "eyJhbGci..." } — client ko JWT de diya
    }
}
