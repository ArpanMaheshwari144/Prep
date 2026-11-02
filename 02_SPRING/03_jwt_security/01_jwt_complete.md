# JWT + Spring Security

## Kahani: hotel ka keycard

Tu hotel me check-in karta hai. Reception ID dekh ke ek keycard deta hai. Ab har baar kamra kholne reception nahi jaana,
card tap karo, darwaza khulta hai. Card me kamra number + checkout ki tareekh, aur hotel ki mohar, isliye nakli card
nahi ban sakta. JWT = yahi keycard.

## Dikkat -> kyun bana

Purana tareeka = **session**: login pe server apni memory me likhta "abc123 = Arpan". App 3 server pe hai, load balancer
ne agli request server 2 pe bheji, server 2 ko abc123 ka pata hi nahi -> dobara login. (Hal the: sticky session ya
Redis me session store, dono jhanjhat.)

JWT me server kuch yaad nahi rakhta. Token khud batata "main Arpan, role USER, 15 min tak valid", aur us pe secret se
mohar. Koi bhi server mohar jaanch ke maan leta = **stateless**.

## Tasveer

```
LOGIN:    password sahi (BCrypt) -> token = header . payload(userId, role, exp) . MOHAR(HMAC, secret se)
REQUEST:  Authorization: Bearer <token> -> server mohar dobara banata -> mili? haan -> Arpan maan liya (DB nahi)
HACKER:   payload me role=ADMIN kiya -> mohar match nahi -> 401
          naya token banana? secret chahiye, uske paas nahi
```

Payload sirf base64 hai, encrypt nahi: jwt.io pe koi bhi padh leta. Password / card kabhi andar nahi.

## Trade-off

```
Fayda                                   Kahan kaatta
stateless: kitne bhi server, koi bhi     logout / revoke: token apni umar tak valid,
request le sakta, session store nahi     server beech me "maar" nahi sakta
```

Hal: access token chhota (15 min, har request me), refresh token lamba (7 din, DB me, sirf `/auth/refresh` pe).
Logout = refresh token DB se delete. Chori ka khatra bas 15 min. Chori ka token bina badle chale to mohar sahi hi
rahegi, mohar sirf badalne / nakli se bachati, chori se nahi -> isliye chhoti umar + HTTPS + refresh rotation.

## Tera project (usercrud)

- `SecurityConfig`: `csrf.disable()` (token header me hai, cookie nahi, browser khud nahi bhejta), session `STATELESS`,
  `/auth/**` permitAll, baaki authenticated, `addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class)`.
- `JwtFilter extends OncePerRequestFilter`: har request pe token jaanch, user ko locker (`SecurityContextHolder`) me.
- Controller `@AuthenticationPrincipal` se user leta -> userId **token se**, body se nahi (PR drill wala "body se
  userId = spoof"). User mila matlab RD / order bhi usi ka nahi, owner check phir bhi.

## Asli kaam wala trap

```java
@Value("${jwt.secret}") private String secret;
private final SecretKey key = Keys.hmacShaKeyFor(secret.getBytes());   // NullPointerException
```

Field wali line `@Value` inject hone se PEHLE chalti, tab `secret = null`. Isliye usercrud me key ek method
`getSigningKey()` me banti (ya `@PostConstruct` me).

## Interview line

```
JWT gives stateless auth: on login I issue a signed token with userId, role and expiry; every request
carries it as a Bearer header and a JwtFilter verifies the signature and sets the SecurityContext, so any
server can serve any request. The trade-off is revocation, so I keep the access token short (15 min) and
a refresh token in the DB; logout deletes the refresh token.
```
