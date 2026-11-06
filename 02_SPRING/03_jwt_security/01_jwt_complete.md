# JWT + Spring Security

## Kahani: hotel ka keycard

Tu hotel me check-in karta hai. Reception teri ID dekh ke tujhe ek keycard de deta hai. Ab har baar kamra kholne ke liye reception nahi jaana padta,
bas card tap karo aur darwaza khul jaata hai. Card me kamra number aur checkout ki tareekh likhi hai, aur us pe hotel ki mohar hai, isliye koi nakli card
nahi bana sakta. JWT yahi keycard hai.

## Dikkat aur ye kyun bana

Purana tareeka **session** tha. Login pe server apni memory me likh leta tha "abc123 = Arpan". Ab maan lo app 3 server pe chal rahi hai, aur load balancer
ne agli request server 2 pe bhej di. Server 2 ko abc123 ka pata hi nahi, kyunki wo server 1 ki memory me tha, isliye user ko dobara login karna padta. Iske hal the sticky session ya
Redis me session store karna, par dono me jhanjhat hai.

JWT me server kuch yaad nahi rakhta. Token khud batata hai "main Arpan hoon, mera role USER hai, main 15 min tak valid hoon", aur us pe secret se
mohar lagi hoti hai. Koi bhi server mohar jaanch ke us pe bharosa kar leta hai, isliye ise **stateless** kehte hain.

## Tasveer

```
LOGIN:    password sahi (BCrypt) -> token = header . payload(userId, role, exp) . MOHAR(HMAC, secret se)
REQUEST:  Authorization: Bearer <token> -> server mohar dobara banata -> mili? haan -> Arpan maan liya (DB nahi)
HACKER:   payload me role=ADMIN kiya -> mohar match nahi -> 401
          naya token banana? secret chahiye, uske paas nahi
```

Login pe password BCrypt se check hota hai, aur sahi ho to token banta hai jisme header, payload (userId, role, exp) aur secret se bani HMAC mohar hoti hai. Har request me token `Authorization: Bearer` header me aata hai, server wahi mohar dobara bana ke milata hai, aur mil gayi to user ko Arpan maan leta hai, DB me jaaye bina. Hacker ne payload me role=ADMIN kar diya to mohar match nahi hogi aur 401 milega. Naya token wo bana nahi sakta, kyunki uske liye secret chahiye jo uske paas nahi hai.

Payload sirf base64 hai, encrypt nahi hai, isliye jwt.io pe koi bhi use padh sakta hai. Isliye password ya card jaisi cheez token ke andar kabhi mat daalo.

## Trade-off

Fayda ye hai ki JWT stateless hai: kitne bhi server ho, koi bhi server koi bhi request le sakta hai, aur session store ki zaroorat nahi padti.

Ye kaat-ta wahan hai jahan logout ya revoke karna ho. Token apni umar khatam hone tak valid rehta hai, aur server beech me use "maar" nahi sakta.

Hal: access token chhota rakho (15 min ka, har request me jaata hai), aur refresh token lamba rakho (7 din ka, DB me rakha, sirf `/auth/refresh` pe jaata hai).
Logout karna matlab refresh token DB se delete kar dena. Is tarah chori ka khatra bas 15 min ka bachta hai. Ek baat samajh lo: agar chori ka token bina badle chalaya gaya, to mohar to sahi hi
rahegi. Mohar sirf token badalne aur nakli token banane se bachati hai, chori se nahi. Isliye chhoti umar, HTTPS aur refresh rotation teeno lagte hain.

## Tera project (usercrud)

- `SecurityConfig` me `csrf.disable()` hai, kyunki token header me jaata hai, cookie me nahi, aur browser header khud se nahi bhejta. Session `STATELESS` hai,
  `/auth/**` pe permitAll hai, baaki sab authenticated hai, aur `addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class)` se apna filter pehle lagaya hai.
- `JwtFilter extends OncePerRequestFilter`: ye har request pe token jaanchta hai aur user ko locker (`SecurityContextHolder`) me rakh deta hai.
- Controller `@AuthenticationPrincipal` se user leta hai, matlab userId **token se** aata hai, body se nahi. Ye PR drill wala point hai ki body se userId lena matlab
  spoof ka khatra. Par user mil gaya iska matlab ye nahi ki RD ya order bhi usi ka hai, isliye owner check phir bhi karna padta hai.

## Asli kaam wala trap

```java
@Value("${jwt.secret}") private String secret;
private final SecretKey key = Keys.hmacShaKeyFor(secret.getBytes());   // NullPointerException
```

Field wali ye line `@Value` inject hone se PEHLE chal jaati hai, aur us waqt `secret` null hota hai, isliye NullPointerException aata hai. Isliye usercrud me key ek method
`getSigningKey()` ke andar banti hai (ya `@PostConstruct` me bana sakte ho).
