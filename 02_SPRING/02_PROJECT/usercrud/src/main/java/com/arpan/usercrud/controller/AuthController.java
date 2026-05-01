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

/* ════════════════════════════════════════════════════════════════════
 *  🚪 AuthController — THE GATE OFFICE (Login/Register/Refresh/Logout)
 * ════════════════════════════════════════════════════════════════════
 *
 *  Yeh controller building ka GATE OFFICE hai — naye log register
 *  karte, login pe ID card issue karte, ID card refresh karte,
 *  logout pe card revoke karte.
 *
 *  ─── 🏢 ANALOGY: Office Reception/Gate Office ─────────────────────
 *
 *      🚶 Visitor (client)
 *           │
 *           ▼
 *      ┌──────────────────────────────────────────┐
 *      │  🚪 Gate Office (this controller)         │
 *      │                                           │
 *      │  📋 Counter 1: REGISTER                   │
 *      │     Naya banda → form fill → password    │
 *      │     locker mein hash karke rakho → save   │
 *      │                                           │
 *      │  🔑 Counter 2: LOGIN                      │
 *      │     Credentials check → ID card factory   │
 *      │     se card banwao → DB mein refresh      │
 *      │     token entry → dono cards do client ko │
 *      │                                           │
 *      │  🔄 Counter 3: REFRESH                    │
 *      │     Old card expired → refresh token DB   │
 *      │     se verify → naya access card do       │
 *      │                                           │
 *      │  🗑️ Counter 4: LOGOUT                    │
 *      │     Refresh token DB se delete → bye      │
 *      │                                           │
 *      └──────────────────────────────────────────┘
 *
 *  ════════════════════════════════════════════════════════════════════
 *  🎓 MASTER JWT SUMMARY — POORA SYSTEM EK JAGAH
 * ════════════════════════════════════════════════════════════════════
 *
 *  Iss project mein JWT-based authentication implement kiya hai.
 *  6 main components hain — sab ek-ek karke wire hue, full flow:
 *
 *  ─── 🧩 6 COMPONENTS (kaun kya karta) ─────────────────────────────
 *
 *  1️⃣  User (model/User.java)
 *      → Entity with email + BCrypt password + role
 *
 *  2️⃣  RefreshToken (model/RefreshToken.java)
 *      → Entity for DB-stored long-lived tokens (revocation ke liye)
 *
 *  3️⃣  JwtService (security/JwtService.java)
 *      → 🎫 ID Card Factory
 *        - generateToken(): naya JWT banao
 *        - isValid(): signature + exp check
 *        - extractEmail/Role/UserId: claims nikalo
 *
 *  4️⃣  CustomUserDetailsService (security/CustomUserDetailsService.java)
 *      → 👨‍💼 HR Department
 *        - loadUserByUsername(email): DB se user load + UserDetails return
 *
 *  5️⃣  JwtFilter (security/JwtFilter.java)
 *      → 🛡️ Gate Guard (extends OncePerRequestFilter)
 *        - Har request pe Authorization header check
 *        - Token validate → SecurityContext set
 *
 *  6️⃣  SecurityConfig (security/SecurityConfig.java)
 *      → 👮 Chief Security Officer
 *        - Filter chain wire (JwtFilter UPAF se pehle)
 *        - Public/protected routes
 *        - BCrypt encoder + AuthenticationManager beans
 *
 *  + AuthController (THIS FILE)
 *      → 🚪 Gate Office (login/register/refresh/logout endpoints)
 *
 *  ─── 🎬 LOGIN END-TO-END FLOW ─────────────────────────────────────
 *
 *      1. Client → POST /auth/login { email, password }
 *
 *      2. AuthController.login():
 *         a. authenticationManager.authenticate(token) call
 *         b. Spring internally:
 *            • CustomUserDetailsService.loadUserByUsername(email)
 *              → DB se user fetch
 *            • PasswordEncoder.matches(rawPassword, storedHash)
 *              → BCrypt compare
 *            • Match → Authentication object return
 *         c. JwtService.generateToken(user) → JWT (15 min)
 *         d. UUID generate → refreshTokenRepo.save() (7 days)
 *         e. Return TokenResponse { accessToken, refreshToken }
 *
 *      3. Client localStorage mein dono store kar leta
 *
 *  ─── 🎬 PROTECTED REQUEST FLOW (after login) ──────────────────────
 *
 *      1. Client → GET /users/me
 *         Header: Authorization: Bearer eyJhbGc...
 *
 *      2. JwtFilter intercepts:
 *         a. "Bearer " prefix check → token extract
 *         b. JwtService.isValid(token) → signature + exp verify
 *         c. extractEmail(token) → email
 *         d. CustomUserDetailsService.loadUserByUsername(email)
 *         e. SecurityContextHolder.setAuthentication(...)
 *
 *      3. Spring routes to UserController.getMe()
 *      4. Controller can access user via SecurityContext
 *      5. Response sent back
 *
 *  ─── 🎬 TOKEN REFRESH FLOW ────────────────────────────────────────
 *
 *      Access expired (15 min) → Frontend interceptor catches 401
 *
 *      1. Client → POST /auth/refresh { refreshToken: "RRRR..." }
 *
 *      2. AuthController.refresh():
 *         a. refreshTokenRepo.findByToken("RRRR")
 *         b. Found? expired nahi?
 *         c. user fetch → JwtService.generateToken() → naya access
 *         d. Return TokenResponse { newAccessToken, sameRefreshToken }
 *
 *      3. Client retries original request with naya accessToken
 *
 *  ─── 🎬 LOGOUT FLOW ───────────────────────────────────────────────
 *
 *      1. Client → POST /auth/logout { refreshToken: "RRRR..." }
 *
 *      2. AuthController.logout():
 *         a. refreshTokenRepo.deleteByToken("RRRR")
 *         b. Return 200 OK
 *
 *      3. Client localStorage clear → done
 *
 *      Result: Refresh token DB se gone → next refresh attempt fail.
 *              Access token apne aap 15 min mein expire.
 *
 *  ─── 🎤 INTERVIEW MASTER ANSWER (ek baar mein sab) ────────────────
 *
 *  Q: "Apne project mein JWT authentication kaise implement kiya?"
 *
 *  Tu bolega:
 *
 *  "Maine UserCRUD project mein full JWT auth system banaya 6
 *   components ke saath:
 *
 *   1. User entity mein email, BCrypt-hashed password, aur role
 *      field rakha. RefreshToken alag entity DB mein refresh
 *      tokens store karta revocation ke liye.
 *
 *   2. JwtService — utility class jjwt library use karke jo token
 *      generate (Jwts.builder se claims set karke HMAC SHA-256 signature),
 *      parse, aur validate karta. Secret key application.properties
 *      se @Value se inject hoti.
 *
 *   3. CustomUserDetailsService Spring Security ka UserDetailsService
 *      interface implement karta — email se DB se user load karta
 *      aur ROLE_ prefix ke saath UserDetails return karta.
 *
 *   4. JwtFilter OncePerRequestFilter extend karke har request pe
 *      Authorization header se 'Bearer' token nikaalta, validate karta,
 *      aur SecurityContextHolder mein UserDetails set karta.
 *
 *   5. SecurityConfig main configuration — CSRF disable kiya stateless
 *      API ke liye, sessionCreationPolicy STATELESS, /auth/** public
 *      banaya, BCryptPasswordEncoder bean diya, aur JwtFilter ko
 *      addFilterBefore se UsernamePasswordAuthenticationFilter se pehle
 *      add kiya.
 *
 *   6. AuthController mein login/register/refresh/logout endpoints —
 *      login pe AuthenticationManager.authenticate() call karta,
 *      match hone pe JwtService se access token banata aur UUID refresh
 *      token DB mein save karta. Refresh endpoint pe DB lookup karke
 *      naya access token issue karta. Logout pe refresh token DB se
 *      delete kar deta.
 *
 *   Stateless rakha — server koi session store nahi karta. Multi-server
 *   scaling, microservices ke liye natural fit. Production mein secret
 *   env variable / AWS Secrets Manager se aata, code mein hardcode nahi."
 *
 *  ════════════════════════════════════════════════════════════════════
 *  📐 SOLID PRINCIPLES APPLIED
 *  ════════════════════════════════════════════════════════════════════
 *  ✅ SRP — Sirf authentication HTTP endpoints (login/register/refresh/logout)
 *  ✅ DIP — Multiple dependencies inject via constructor —
 *           UserRepository, RefreshTokenRepository, JwtService,
 *           AuthenticationManager, PasswordEncoder.
 *           Sab interfaces/abstractions, concrete implementations Spring inject karta.
 *  ✅ OCP — Naye auth methods (e.g., OAuth) add karne mein existing endpoints unchanged
 * ════════════════════════════════════════════════════════════════════
 */
@RestController
@RequestMapping("/auth")
public class AuthController {

    /*
     *  🔌 Dependencies — Spring inject karega constructor se
     */
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
    //  📋 1. REGISTER — New user signup
    // ════════════════════════════════════════════════════════════
    /*
     *  POST /auth/register
     *  Body: { name, email, password, age }
     *
     *  Flow:
     *  1. Email already taken? → 400
     *  2. BCrypt password hash karo
     *  3. role hardcode "USER" (security — client can't set)
     *  4. Save → return saved user (sans password)
     */
    @PostMapping("/register")
    public ResponseEntity<User> register(@Valid @RequestBody RegisterRequest req) {
        // 🛡️ Email uniqueness check
        if (userRepository.findByEmail(req.getEmail()).isPresent()) {
            throw new IllegalArgumentException("Email already registered: " + req.getEmail());
        }

        // 🔒 Password hash with BCrypt — DB mein hash store, plain NEVER
        String hashedPassword = passwordEncoder.encode(req.getPassword());

        // 👤 New User object — role hardcoded "USER" (security!)
        User user = new User(
            null,                          // id (auto-generate)
            req.getName(),
            req.getEmail(),
            hashedPassword,                // BCrypt hash
            req.getAge(),
            "USER"                         // role — server-side hardcode
        );

        User saved = userRepository.save(user);

        // ⚠️ Production: password field response mein nahi bhejna chahiye
        // Yeh demo simplicity ke liye — properly DTO use karke filter karte
        return ResponseEntity.ok(saved);
    }

    // ════════════════════════════════════════════════════════════
    //  🔑 2. LOGIN — Credentials verify + token issue
    // ════════════════════════════════════════════════════════════
    /*
     *  POST /auth/login
     *  Body: { email, password }
     *
     *  Flow:
     *  1. AuthenticationManager.authenticate() —
     *     internally CustomUserDetailsService + PasswordEncoder use karta
     *  2. Match? → user fetch from DB
     *  3. JwtService.generateToken() → 15-min JWT
     *  4. UUID refresh token → DB save (7 days)
     *  5. Return both tokens
     */
    @PostMapping("/login")
    public ResponseEntity<TokenResponse> login(@Valid @RequestBody LoginRequest req) {
        try {
            // 🔐 Spring Security ki magic — credentials verify
            // Internally:
            //   • CustomUserDetailsService.loadUserByUsername(email)
            //   • PasswordEncoder.matches(rawPassword, storedHash)
            authenticationManager.authenticate(
                new UsernamePasswordAuthenticationToken(req.getEmail(), req.getPassword())
            );
        } catch (BadCredentialsException e) {
            throw new IllegalArgumentException("Invalid email or password");
        }

        // ✅ Authenticated — fetch user (UserDetails ke bajaye full User)
        User user = userRepository.findByEmail(req.getEmail())
                .orElseThrow(() -> new UserNotFoundException(0L));

        // 🎫 Generate access token (JWT, 15 min)
        String accessToken = jwtService.generateToken(user);

        // 🔄 Generate refresh token (UUID, 7 days, DB stored)
        String refreshTokenString = UUID.randomUUID().toString();
        RefreshToken refreshToken = new RefreshToken(
            null,                                       // id
            refreshTokenString,
            user.getId(),
            Instant.now().plus(7, ChronoUnit.DAYS)
        );
        refreshTokenRepository.save(refreshToken);

        return ResponseEntity.ok(new TokenResponse(accessToken, refreshTokenString));
    }

    // ════════════════════════════════════════════════════════════
    //  🔄 3. REFRESH — Issue new access token
    // ════════════════════════════════════════════════════════════
    /*
     *  POST /auth/refresh
     *  Body: { refreshToken }
     *
     *  Flow:
     *  1. DB se refresh token find
     *  2. Expired? → DELETE + 401
     *  3. user fetch → naya access token generate
     *  4. Return new access (refresh same rakhte)
     */
    @PostMapping("/refresh")
    public ResponseEntity<TokenResponse> refresh(@Valid @RequestBody RefreshRequest req) {
        // 🔍 DB se refresh token dhundo
        RefreshToken stored = refreshTokenRepository.findByToken(req.getRefreshToken())
                .orElseThrow(() -> new IllegalArgumentException("Invalid refresh token"));

        // ⏰ Expired check
        if (stored.getExpiresAt().isBefore(Instant.now())) {
            refreshTokenRepository.delete(stored);   // cleanup expired row
            throw new IllegalArgumentException("Refresh token expired, please login again");
        }

        // 👤 User fetch
        User user = userRepository.findById(stored.getUserId())
                .orElseThrow(() -> new UserNotFoundException(stored.getUserId()));

        // 🎫 Naya access token
        String newAccessToken = jwtService.generateToken(user);

        // Refresh token same rakhte (rotation optional advanced topic)
        return ResponseEntity.ok(new TokenResponse(newAccessToken, req.getRefreshToken()));
    }

    // ════════════════════════════════════════════════════════════
    //  🗑️ 4. LOGOUT — Refresh token DB se delete
    // ════════════════════════════════════════════════════════════
    /*
     *  POST /auth/logout
     *  Body: { refreshToken }
     *
     *  Flow:
     *  1. DB se refresh token DELETE
     *  2. 200 OK
     *
     *  Note: Access token instantly invalidate nahi hota — woh
     *  apne aap 15 min mein expire ho jata. Production sensitive
     *  apps mein blacklist (Redis) use karte additional layer ke liye.
     */
    @PostMapping("/logout")
    public ResponseEntity<Void> logout(@Valid @RequestBody RefreshRequest req) {
        refreshTokenRepository.deleteByToken(req.getRefreshToken());
        return ResponseEntity.ok().build();
    }
}
