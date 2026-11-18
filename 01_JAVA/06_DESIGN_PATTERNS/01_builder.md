# Builder pattern

**Koi poochhe "Builder kya hai" to aise bolo:**
```
Jab object me bahut field hon aur aadhe optional, to constructor me null-null ki line ban jaati.
Builder me har field naam se set hoti, jo nahi chahiye chhod do, aakhir me build(). JWT banate
waqt Jwts.builder() yahi hai; apni class pe Lombok @Builder.
```

Builder matlab object ko kadam-kadam banana.

## Dikkat

Kisi object me 8-10 field hon aur unme se aadhe optional, to constructor aisa ban jaata hai:
`new User("Arpan", null, null, "ADMIN", true, null)`. Ab koi nahi bata sakta ki teesra `null` kya hai. Builder me har cheez ka
naam saath me likha hota hai, aur jo nahi chahiye wo chhod do.

## Tera usercrud

```java
Jwts.builder()                          // JwtService
    .subject(user.getId().toString())
    .claim("role", user.getRole())
    .expiration(expiryDate)
    .signWith(getSigningKey())
    .compact();                         // yahi build()
```

Usercrud me ye do jagah hai: `JwtService` me token banane ke liye `Jwts.builder()`, aur `CustomUserDetailsService` me Spring ka
`User.builder()...build()`. Apni class pe Lombok ka `@Builder` lagao to ye sab khud ban jaata hai.

## Prod wala trap

Class me `private String status = "ACTIVE";` likha aur upar `@Builder` lagaya. Ab `User.builder().name("Arpan").build()` se jo
object banega uska status `null` hoga, `"ACTIVE"` nahi, kyunki builder field ki default value ko nahi dekhta. Ilaaj: us field
pe `@Builder.Default` lagao.

Chala ke dekha (Lombok 1.18.46):

```
@Builder, status = "ACTIVE"                 -> User(name=Arpan, status=null)
@Builder.Default, status = "ACTIVE"         -> User2(name=Arpan, status=ACTIVE)
```

Compile pe Lombok warning bhi deta hai ("@Builder will ignore the initializing expression"), par warning aksar koi padhta nahi.
