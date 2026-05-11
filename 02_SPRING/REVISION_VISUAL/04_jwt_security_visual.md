# 🔐 JWT + Security — Visual Revision

---

## 1️⃣ Stateful vs Stateless (ATM Analogy)

```
STATEFUL (old session-based):
   Bank office mein har baar:
      "Sir, mei Arpan hu, password yeh hai"
      Cashier register dekh raha
      "Yes, balance check karta"
   = Cashier ko HAR BAAR yaad rakhna padta tu kaun
   = Server side memory zaroori
```

```
STATELESS (JWT-based):
   ATM card pe sab info encoded:
      Card swipe → ATM ne card padha
      "Yeh Arpan hai, KYC done, daily limit X"
      = ATM ko KUCH yaad nahi rakhna
      = Card mein SAB info hai
   = Server stateless ho gaya
```

```
   JWT = ATM card
   Server = ATM (kuch yaad nahi rakhta)
```

---

## 2️⃣ JWT Structure — 3 Parts

```
   eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiIxMjMifQ.SflKxw...
   ─────────────────── ───────────────── ──────────
        HEADER             PAYLOAD       SIGNATURE
```

```
┌─────────────┬────────────────────────────────────┐
│  Part       │  Contains                           │
├─────────────┼────────────────────────────────────┤
│ HEADER      │ Algorithm (HS256), token type       │
│             │ { "alg": "HS256", "typ": "JWT" }    │
├─────────────┼────────────────────────────────────┤
│ PAYLOAD     │ Claims (user info)                  │
│             │ { "sub": "user_123",                │
│             │   "exp": 1715180400,                │
│             │   "role": "USER" }                  │
├─────────────┼────────────────────────────────────┤
│ SIGNATURE   │ HMAC(header + payload, secret)     │
│             │ = Tampering detection                │
└─────────────┴────────────────────────────────────┘

   Each part = Base64-encoded
   Joined with "." dots
```

---

## 3️⃣ Signature = Tamper Detection

```
   Server has SECRET KEY (only server knows)
   
   Token banate time:
      signature = HMAC(header + payload, secret)
   
   Token verify karte time:
      recalculate signature
      Token mein wala signature == calculated?
         Yes → genuine ✅
         No  → tampered ❌
```

```
Hacker scenario:
   Token mein "role: USER" ko "role: ADMIN" badal diya?
      Payload changed → signature mismatch
      Server reject kar dega ✅
   = Tamper-proof
```

---

## 4️⃣ Auth Flow — Login

```
   USER
    │
    │  POST /api/login
    │  { email, password }
    ▼
   ┌─────────────────────┐
   │  AuthController     │
   └────────┬────────────┘
            │
            ▼
   ┌─────────────────────┐
   │  Verify credentials │  ← password matches?
   │  (BCrypt compare)   │
   └────────┬────────────┘
            │ valid
            ▼
   ┌─────────────────────┐
   │  Generate JWT       │  ← user info + sign
   │  (JwtService)       │
   └────────┬────────────┘
            │
            ▼
   ┌─────────────────────┐
   │  Return JSON:        │
   │  { accessToken,      │
   │    refreshToken }    │
   └────────┬────────────┘
            │
            ▼
   USER stores tokens
   (localStorage / mobile keychain)
```

---

## 5️⃣ Auth Flow — Protected Request

```
   USER
    │
    │  GET /api/users/me
    │  Authorization: Bearer eyJhbGc...
    ▼
   ┌──────────────────────┐
   │  JwtFilter           │  ← Spring Security filter
   │  (extends            │
   │   OncePerRequestFlt) │
   └─────────┬────────────┘
             │
             ▼
   ┌──────────────────────┐
   │  Extract token from  │
   │  "Authorization"      │
   └─────────┬────────────┘
             │
             ▼
   ┌──────────────────────┐
   │  Validate signature   │
   │  Check expiry         │
   └─────────┬────────────┘
             │ valid
             ▼
   ┌──────────────────────┐
   │  Set authentication   │
   │  in SecurityContext   │
   └─────────┬────────────┘
             │
             ▼
   ┌──────────────────────┐
   │  Forward to           │
   │  Controller           │
   └──────────────────────┘
```

---

## 6️⃣ Spring Security Filter Chain

```
   REQUEST
      │
      ▼
   ┌─────────────────────────┐
   │ Spring Security Filters │
   │                          │
   │  1. CORS Filter          │
   │  2. CSRF Filter (skip    │
   │     for stateless API)   │
   │  3. JwtFilter ←──────────┼──── tu yahan custom add
   │  4. UsernamePassword     │
   │     AuthFilter           │
   │  5. ExceptionFilter      │
   │  ...                     │
   └─────────────┬───────────┘
                 │
                 ▼
   ┌─────────────────────────┐
   │  DispatcherServlet       │
   └─────────────┬───────────┘
                 │
                 ▼
            Controller
```

```
SecurityConfig:
   - Define which URLs need auth
   - Add JwtFilter to chain
   - BCrypt password encoder
   - Stateless session (no HttpSession)
```

---

## 7️⃣ Access Token vs Refresh Token

```
   ACCESS TOKEN (short-lived: 15 min)
      │
      ├── Used for API requests
      ├── Sent every request
      └── Expires fast (security)
   
   REFRESH TOKEN (long-lived: 7-30 days)
      │
      ├── Used to get NEW access token
      ├── Stored in DB (so we can revoke)
      ├── Rotated on each refresh (best practice)
      └── User logout → delete from DB
```

```
WHY 2 TOKENS?
   Access token leaked → only 15 min damage
   Refresh token DB-stored → can revoke instantly
   = Security + UX balance
```

---

## 8️⃣ Refresh Token Flow

```
   USER's access token expired (15 min later)
        │
        │  POST /api/refresh
        │  { refreshToken }
        ▼
   ┌─────────────────────────┐
   │  AuthController.refresh │
   └────────────┬────────────┘
                │
                ▼
   ┌─────────────────────────┐
   │  Check refresh token    │
   │  in DB exists?           │
   │  Not expired?            │
   └────────────┬────────────┘
                │ valid
                ▼
   ┌─────────────────────────┐
   │  Generate NEW access     │
   │  token + NEW refresh     │
   │  Save new refresh in DB  │
   │  Delete old refresh      │
   │  (rotation)              │
   └────────────┬────────────┘
                │
                ▼
   Return new tokens
```

---

## 9️⃣ Logout = Delete Refresh Token

```java
@PostMapping("/logout")
public ResponseEntity<?> logout(@RequestBody LogoutRequest req) {
    refreshTokenRepo.deleteByToken(req.refreshToken());
    return ResponseEntity.ok().build();
}
```

```
WHY DB-stored refresh tokens?
   Logout possible (delete from DB)
   
   Pure JWT (no DB)?
   = Can't logout!
   = Token valid till expiry
   = Security risk
```

---

## 🔟 Full Project Architecture

```
   ┌──────────────────────────────────┐
   │  AuthController                   │
   │  /login, /register, /refresh,    │
   │  /logout                          │
   └──────────────┬───────────────────┘
                  │
                  ▼
   ┌──────────────────────────────────┐
   │  JwtService                       │
   │  ├── generateAccessToken()        │
   │  ├── generateRefreshToken()       │
   │  └── validateToken()              │
   └──────────────┬───────────────────┘
                  │
   ┌──────────────────────────────────┐
   │  JwtFilter (extends               │
   │  OncePerRequestFilter)            │
   │  ├── extract token                │
   │  ├── validate                     │
   │  └── set SecurityContext          │
   └──────────────┬───────────────────┘
                  │
   ┌──────────────────────────────────┐
   │  CustomUserDetailsService         │
   │  loadUserByUsername()             │
   └──────────────┬───────────────────┘
                  │
   ┌──────────────────────────────────┐
   │  SecurityConfig                   │
   │  ├── filterChain                  │
   │  ├── BCryptPasswordEncoder        │
   │  ├── stateless session            │
   │  └── add JwtFilter to chain       │
   └──────────────────────────────────┘
   
   PERSISTENCE:
   ┌──────────────────────────────────┐
   │  RefreshTokenRepository (JPA)     │
   │  → DB: refresh_tokens table       │
   └──────────────────────────────────┘
```

---

## 1️⃣1️⃣ 7 Security Pitfalls

```
1. ❌ Storing JWT in cookie WITHOUT httpOnly
   → XSS attacks read it
   ✅ Use httpOnly cookie OR localStorage with care

2. ❌ Long-lived access tokens (1 day+)
   ✅ 15 min access + 7-30 day refresh

3. ❌ No refresh token rotation
   ✅ Generate NEW refresh on each refresh

4. ❌ JWT secret in code
   ✅ Environment variable / secret manager

5. ❌ Symmetric key (HS256) shared with frontend
   ✅ Use RS256 (asymmetric) for distributed systems

6. ❌ Sensitive data in payload
   ✅ JWT is base64, NOT encrypted — anyone can read
   ✅ Only put non-sensitive (id, role)

7. ❌ No logout mechanism
   ✅ DB-stored refresh tokens, delete on logout
```

---

## 🎯 Memory Hooks

```
JWT       = ATM card (encoded info, server stateless)
Header    = card type
Payload   = card holder details
Signature = bank's tamper-proof seal

Access    = quick-use card (short-lived)
Refresh   = master key (DB-stored, revocable)

JwtFilter = security guard at door
SecurityContext = "verified visitor" badge
BCrypt    = password hash (one-way)
```

---

## 💎 One-Line Summary

```
"JWT = stateless authentication.
 ATM card analogy — token mein info encoded + signed.
 Server doesn't store sessions.
 Access (15 min) + Refresh (7d, DB-stored) pattern.
 JwtFilter intercepts → validates → sets SecurityContext."
```

📚 [← Back to SPRING](../)
