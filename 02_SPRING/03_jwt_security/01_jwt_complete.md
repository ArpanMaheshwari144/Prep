# JWT + Spring Security

## Scene: Fargate pe 3 task, aage LB

Teri app Fargate pe 3 task me chal rahi hai aur aage load balancer hai. Pehle **session** wala tareeka tha: login hua to task 1 ki memory
me likha gaya "abc123 = Arpan". Agli request LB ne task 2 pe bhej di. Task 2 ko abc123 ka pata hi nahi, kyunki wo task 1 ki memory me tha,
isliye user achanak logout ho gaya. Iske hal the sticky session ya Redis me session rakhna, aur dono me jhanjhat hai.

JWT me server kuch yaad nahi rakhta. Token khud bolta hai "main userId 7, role USER, itne baje tak valid", aur us pe secret se mohar lagi
hoti hai. Koi bhi task mohar jaanch ke us pe bharosa kar leta hai, DB me jaaye bina. Isliye ise **stateless** kehte hain.

```
LOGIN:    password sahi (BCrypt) -> token = header . payload(userId, role, exp) . MOHAR(HMAC, secret se)
REQUEST:  Authorization: Bearer <token> -> server mohar dobara banata -> mili? haan -> user maan liya (DB nahi)
HACKER:   payload me role=ADMIN kiya -> mohar match nahi -> 401
          naya token banana? secret chahiye, uske paas nahi
```

## Chala ke dekha (plain Java HMAC, wahi mohar wala kaam)

```
token   : eyJhbGciOiJIUzI1NiJ9.eyJ1c2VySWQiOjcsInJvbGUiOiJVU0VSIiwi...UfZxae8Q...
payload : {"userId":7,"role":"USER","exp":1731240000}   <- koi bhi padh le
asli    : 200 OK, {"userId":7,"role":"USER",...}
ADMIN kiya: 401 mohar match nahi
```

Payload sirf base64 hai, chhupa hua nahi, isliye koi bhi use padh sakta hai (jwt.io pe bhi). Isliye password ya card number token me kabhi
mat daalo. Par badal koi nahi sakta: hacker ne role ADMIN kiya to mohar match nahi hui aur 401 mila, aur naya token wo bana nahi sakta kyunki
secret uske paas nahi hai.

## Chhoti dikkat: logout pe token marta nahi

User ne logout kiya, par token apni umar khatam hone tak chalta rahega. Server use beech me "maar" nahi sakta, kyunki server ne kuch yaad
hi nahi rakha.

Hal: access token chhota rakho (15 min ka, har request me jaata hai), aur refresh token lamba rakho (7 din ka, DB me rakha, sirf
`/auth/refresh` pe jaata hai). Logout karna matlab refresh token DB se delete kar dena.

Ek baat saaf: chori hua token badle bina chalaya gaya to mohar sahi hi rahegi. Mohar sirf token badalne aur nakli token banane se bachati hai,
chori se nahi. Isliye chori ka khatra utna hi bachta hai jitni access token ki umar, aur saath me HTTPS lagta hai.

## Tera project (usercrud)

- `SecurityConfig` me session `STATELESS` hai, `/auth/**` pe permitAll hai, baaki sab authenticated hai, aur
  `addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class)` se apna filter pehle lagaya hai. `csrf.disable()` isliye hai
  kyunki token header me jaata hai, cookie me nahi, aur browser header khud se nahi bhejta.
- `JwtFilter extends OncePerRequestFilter`: ye har request pe token jaanchta hai aur user ko `SecurityContextHolder` me rakh deta hai.
- Controller `@AuthenticationPrincipal` se userId **token se** leta hai, body se nahi. Ye PR drill wala point hai: body se userId lena
  matlab spoof ka khatra. Par user mil gaya iska matlab ye nahi ki order bhi usi ka hai, isliye owner check phir bhi karna padta hai.

## Prod wala trap

```java
@Value("${jwt.secret}") private String secret;
private final SecretKey key = Keys.hmacShaKeyFor(secret.getBytes());   // NullPointerException
```

Field wali ye line `@Value` inject hone se PEHLE chal jaati hai. Us waqt `secret` null hota hai, isliye app start hote hi gir jaati hai.
Isliye usercrud me key ek method `getSigningKey()` ke andar banti hai (ya `@PostConstruct` me bana sakte ho).
