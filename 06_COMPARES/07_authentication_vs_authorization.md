# Authentication vs Authorization

## Scene: office ki building

Reception pe ID card dikhaya, receptionist ne scan kiya: "haan, tu Arpan hai". Ye **authentication** hai, ki tu kaun hai. Phir 10th
floor (CEO) ke darwaze pe card lagaya: "nahi, tu sirf 5th floor ka hai". Ye **authorization** hai, ki tu kya kar sakta hai. Pehle
pehchaan, phir ijaazat, hamesha isi kram me.

Galti ka jawab bhi alag hota hai:
- **401:** pehchaan hi nahi hui. Token nahi hai, expire ho gaya, ya mohar galat hai. Matlab "pehle login kar".
- **403:** pehchaan ho gayi, par ijaazat nahi. Jaise USER role wala ADMIN ka kaam kare, ya kisi aur ka order khole.

Naam yaad rakhne ko: pehchaan ke tareeke = password + OTP (MFA), JWT, Google se login (OAuth / OpenID Connect), passkey. Ijaazat ke
tareeke = role se (RBAC: USER, ADMIN), cheez ke maalik / rishte se (owner check, Google Drive jaisa share), attribute ya policy se
(AWS IAM jaisa).

## Tere usercrud me dekha (code padh ke)

- Authentication poora laga hai: `JwtFilter` token jaanchta hai, aur `SecurityConfig` me `.anyRequest().authenticated()` hai.
- Authorization lagaya hi nahi gaya. `@PreAuthorize` / `hasRole` sirf comments me likhe hain, code me kahin nahi.

## Prod wala trap (isi project se)

```java
@DeleteMapping("/{id}")
public ResponseEntity<Void> delete(@PathVariable Long id) {
    service.delete(id);          // koi role check nahi, koi owner check nahi
```

Ravi ne login kiya, uske paas sahi token hai. Ab wo `DELETE /users/5` maare to Priya ka account ud jaayega. Token sahi hai, isliye 401
nahi aayega, aur role / owner check hai hi nahi, isliye 403 bhi nahi. Bas login hona kaafi ho gaya. Docs design me "har edit pe dekho
abhi haq hai ya nahi" aur PR drill ka "owner check" yahi cheez hai.

**Hal:** admin wale kaam pe `@PreAuthorize("hasRole('ADMIN')")` (ya `SecurityConfig` me `.requestMatchers("/admin/**").hasRole("ADMIN")`),
aur apni cheez wale kaam pe owner check: token wala userId == jiska data hai, warna 403.

[← HLD README](../README.md)
