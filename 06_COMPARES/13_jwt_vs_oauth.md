# JWT vs OAuth

> BADI GALTI-FEHMI: log inhe "ek-doosre ka option" samajhte. Ye hai HI NAHI.
> Alag LEVEL ki cheezein — ek "token dikhta kaisa", doosra "token milta kaise".

---

## 1-Line

```
JWT   = token ka FORMAT       (token dikhta kaisa hai — signed JSON)
OAuth = access-dene ka PROTOCOL (token milta kaise hai — delegated access)
```

Competing NAHI. Aksar SAATH chalte (OAuth flow ek JWT issue karta).

---

## JWT (JSON Web Token)

```
Ek self-contained SIGNED token:

   header . payload . signature      (teeno base64)

   payload = claims (kaun ho, expiry, role)
   signature = tamper-proof seal

Server signature verify karta — bina DB dekhe (stateless).

Ye SIRF "token ka shape" hai. Kaha se aaya, kis flow se — matlab nahi.
```

Tera usercrud yehi use karta:
```
login → server ek JWT sign karta
      → client Authorization header me bhejta
      → server signature verify
= plain JWT-auth (OAuth NAHI)
```

---

## OAuth 2.0

```
Ek AUTHORIZATION framework — ek app ko doosre app ka data
access karne ki PERMISSION dilata, BINA password share kiye.

Classic: "Login with Google"
   → teri app Google se ek token leti
   → taaki user ka data access kar sake
   → user ka Google-PASSWORD teri app ko milta hi NAHI

Ye poora PROCESS hai: redirect → consent → token.
= delegated access (kisi aur ki taraf se access).
```

---

## ANALOGY (concrete)

```
JWT   = ID-card ka FORMAT
        (tamper-proof card: naam + expiry + signature)
        bas card DIKHTA kaisa hai.

OAuth = wo poora PROCESS jisme Google (third-party)
        teri app ko ek PASS deta user ki cheez access karne ko
        — bina user ka password diye.
        = card BANANE / DENE ka tareeka.
```

---

## Side by Side

```
┌──────────────────┬──────────────────────┬───────────────────────┐
│  Feature         │  JWT                  │  OAuth                │
├──────────────────┼──────────────────────┼───────────────────────┤
│ Kya hai          │ token ka FORMAT       │ PROTOCOL / framework  │
├──────────────────┼──────────────────────┼───────────────────────┤
│ Sawaal           │ token dikhta KAISA    │ token milta KAISE     │
├──────────────────┼──────────────────────┼───────────────────────┤
│ Kaam             │ data carry (signed,   │ delegated access      │
│                  │ self-contained)       │ ("login with X")      │
├──────────────────┼──────────────────────┼───────────────────────┤
│ Akela use hota?  │ HAAN (tera usercrud)  │ token issue karta —   │
│                  │                       │ JWT ya opaque         │
├──────────────────┼──────────────────────┼───────────────────────┤
│ Level            │ token ka roop         │ poora auth-flow       │
└──────────────────┴──────────────────────┴───────────────────────┘
```

---

## SAATH KAISE (dono ek-doosre ke dushman NAHI)

```
OAuth flow chalta → jo token issue hota → wo JWT format me ho sakta
(aksar hota hi hai).

   OAuth  = KAISE token milta   (procedure)
   JWT    = token KAISA dikhta   (format)

Do combinations:
   • JWT bina OAuth   → tera usercrud (login → apna JWT sign)
   • OAuth ka token   → JWT ho sakta, ya "opaque" (random string, DB-lookup)
```

---

## ★ EK CONFUSION SAAF: OAuth = authoriz-WHAT? phir authentication?

```
OAuth 2.0 = mool roop se AUTHORIZATION (delegated access — "app ko access do").
Authentication (WHO are you) uske UPAR aata hai:

   OIDC (OpenID Connect) = OAuth2 ke UPAR ek layer
                          jo AUTHENTICATION deta ("login with Google" ka
                          "ye Arpan hai" wala hissa).

To "Login with Google" = OAuth2 (access) + OIDC (identity) dono.
```
(Isse [[07_authentication_vs_authorization]] se jodo: AuthN = WHO, AuthZ = WHAT.)

---

## INTERVIEW LINE

```
"JWT ek token-FORMAT hai — self-contained, signed, stateless verify.
 OAuth ek AUTHORIZATION-framework hai — delegated access, 'login-with-Google',
 bina password share kiye.

 Ye either/or nahi: OAuth aksar JWT ko token ke roop me issue karta.
 Mere project me plain JWT-auth hai (login → signed JWT → header),
 OAuth tab jab third-party delegated-access chahiye.

 OAuth mool roop se authorization deta; authentication OIDC layer se aata."
```

---

## Memory Hook

```
JWT   = token ka ROOP     (kaisa dikhta — signed JSON)
OAuth = token ka RAASTA    (kaise milta — delegated flow)

Dushman nahi → OAuth ka token aksar ek JWT hi hota.
Akela JWT = usercrud login. OAuth = "login with Google".
```

[← COMPARES README](../README.md)
