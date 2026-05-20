# Authentication vs Authorization

---

## 1-Line Analogy

```
Authentication = "Who are you?"        (verify identity)
Authorization  = "What can you do?"    (verify permissions)
```

---

## OFFICE BUILDING ANALOGY

```
You enter an office building:

STEP 1: AUTHENTICATION (at reception)
   • Show ID card
   • Receptionist scans → "Yes, you're Arpan"
   = Identity CONFIRMED

STEP 2: AUTHORIZATION (at floor doors)
   • Try to enter CEO's floor (10th)
   • Card scan → "Access DENIED, you're 5th floor only"
   • Try 5th floor → "Access GRANTED"
   = Permissions CHECKED

   Authentication = WHO
   Authorization  = WHAT
```

---

## Side by Side

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Feature         │  Authentication   │  Authorization    │
├──────────────────┼──────────────────┼──────────────────┤
│ Question         │ "Who are you?"     │ "What can you do?"│
├──────────────────┼──────────────────┼──────────────────┤
│ Order             │ Comes FIRST        │ Comes AFTER       │
├──────────────────┼──────────────────┼──────────────────┤
│ Checks            │ Identity / creds  │ Permissions/roles │
├──────────────────┼──────────────────┼──────────────────┤
│ Method            │ Password, OTP,    │ Roles, ACLs,      │
│                  │ Biometric, JWT     │ Policies, RBAC    │
├──────────────────┼──────────────────┼──────────────────┤
│ Failure response  │ 401 Unauthorized  │ 403 Forbidden     │
│                  │ "Login again"       │ "Not allowed"     │
├──────────────────┼──────────────────┼──────────────────┤
│ Where stored     │ User DB            │ Role/permission DB│
│                  │ (passwords hashed) │ (RBAC tables)     │
├──────────────────┼──────────────────┼──────────────────┤
│ Frequency        │ Once per session   │ Every request      │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## FLOW VISUAL

```
   USER: "I want to delete user #5"
        │
        ▼
   ┌──────────────────────┐
   │ STEP 1: AUTHENTICATION│
   │                        │
   │ Check JWT token        │
   │ Verify signature       │
   │ Extract user info       │
   │                        │
   │ It's Arpan          │
   │    (role: USER)         │
   └──────────┬───────────┘
              │
              ▼
   ┌──────────────────────┐
   │ STEP 2: AUTHORIZATION │
   │                        │
   │ Endpoint: DELETE /users│
   │ Required: ADMIN         │
   │                        │
   │ User role: USER         │
   │ Denied              │
   │    403 Forbidden        │
   └──────────────────────┘
```

---

## AUTHENTICATION METHODS

```
1. PASSWORD-BASED
   Username + password → check hash in DB
   
2. MULTI-FACTOR (MFA)
   Password + OTP / TOTP / SMS code
   
3. BIOMETRIC
   Fingerprint, face ID, voice
   
4. TOKEN-BASED
   JWT (stateless)
   Session ID (stateful)
   
5. SSO (Single Sign-On)
   OAuth 2.0 (Google, Facebook login)
   SAML (enterprise)
   OpenID Connect (modern)
   
6. PASSWORDLESS
   Magic links (email)
   WebAuthn (FIDO2 hardware keys)
   Passkey
```

---

## AUTHORIZATION MODELS

```
┌──────────────┬──────────────────────────────────────┐
│  Model       │  How                                  │
├──────────────┼──────────────────────────────────────┤
│ RBAC          │ Role-Based Access Control             │
│              │ User → Role → Permissions             │
│              │ Example: USER, ADMIN, MANAGER         │
│              │ Common in enterprise                  │
├──────────────┼──────────────────────────────────────┤
│ ABAC          │ Attribute-Based Access Control        │
│              │ Decisions based on attributes         │
│              │ (user.dept = "HR" AND time < 18:00)    │
├──────────────┼──────────────────────────────────────┤
│ ACL           │ Access Control Lists                   │
│              │ Per-resource permissions               │
│              │ Like Linux file permissions            │
├──────────────┼──────────────────────────────────────┤
│ PBAC          │ Policy-Based Access Control            │
│              │ Centralized policies (AWS IAM style)  │
├──────────────┼──────────────────────────────────────┤
│ ReBAC         │ Relationship-Based                     │
│              │ "Owner" of resource, "Member of group"│
│              │ Google Drive, Notion style             │
└──────────────┴──────────────────────────────────────┘
```

---

## SPRING SECURITY ANGLE

```
UserCRUD mein dono use ho rahe:

AUTHENTICATION (JwtFilter):
   • Extract JWT from header
   • Verify signature with secret
   • Load UserDetails
   • Set SecurityContext
   = "Aap kaun ho? — Arpan"

AUTHORIZATION (per endpoint):
   @PreAuthorize("hasRole('ADMIN')")
   @DeleteMapping("/users/{id}")
   public void delete() { ... }
   
   OR in SecurityConfig:
   .requestMatchers("/admin/**").hasRole("ADMIN")
   .requestMatchers("/users/**").authenticated()
   = "Aapko allowed hai? — Check role"
```

---

## HTTP STATUS CODES

```
401 UNAUTHORIZED:
   = "Authentication failed"
   = "Login first / Token invalid"
   
   Triggers:
   • No JWT header
   • JWT expired
   • Wrong password
   • Invalid signature

403 FORBIDDEN:
   = "Authentication OK, but not allowed"
   = "Login is valid, but you can't do this"
   
   Triggers:
   • User trying ADMIN endpoint
   • Cross-tenant access
   • Resource not owned
   
   Common confusion:
      401 ≠ 403
      401 = identity issue
      403 = permission issue
```

---

## REAL FLOW — UserCRUD

```
1. User registers (POST /register)
   → Server stores password hash + role
   → Returns success

2. User logs in (POST /login)
   → Server checks credentials = AUTHENTICATION
   → Generates JWT with userId + role
   → Returns JWT

3. User calls API (GET /users)
   → JWT in Authorization header
   → JwtFilter verifies = AUTHENTICATION
   → SecurityConfig checks role = AUTHORIZATION
   → Allowed? → Controller runs
   → Denied? → 403

4. User calls admin API (DELETE /users/5)
   → JWT verified 
   → Role check: needs ADMIN
   → User is USER → 403 Forbidden
```

---

## INTERVIEW LINE

```
"Authentication verifies WHO you are — credentials,
 tokens, biometrics. Returns 401 on failure.

 Authorization verifies WHAT you can do — roles,
 permissions, policies. Returns 403 on failure.

 Authentication comes first; authorization is checked
 on every request based on authenticated identity.

 Common authentication: passwords + MFA, JWT, OAuth.
 Common authorization: RBAC (roles), ABAC (attributes),
 ACLs (per-resource), PBAC (policies).

 Spring Security uses filters for authentication
 (extract JWT, load UserDetails) and @PreAuthorize
 or SecurityConfig matchers for authorization."
```

---

## Memory Hook

```
Authentication = ID Card → WHO are you?
Authorization  = Access Card → WHAT can you do?

401 = Not authenticated (login first)
403 = Authenticated but forbidden (no permission)

Order: AuthN → AuthZ (always this order)

Models: RBAC most common (roles)
        ABAC for fine-grained
        ACL for per-resource
        PBAC for centralized policies
```

[← HLD README](../README.md)
