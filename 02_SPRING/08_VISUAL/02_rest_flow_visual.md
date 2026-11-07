# Spring REST Flow

## Scene

Postman se `POST /api/users` maara, body `{"name":"Ravi","email":"ravi@x.com","password":"abc"}`. Request kahan-kahan se guzarti hai:

```
Postman POST /api/users
 -> Tomcat
 -> Filter chain (JwtFilter: token galat? 401)       <- Spring MVC se PEHLE
 -> DispatcherServlet (URL se controller method dhoondha)
 -> JSON -> UserRequest + @Valid (galat? 400)
 -> Controller -> Service (@Transactional) -> Repository -> DB INSERT
 -> Entity -> DTO -> Jackson JSON -> 201
 kahin bhi exception -> @RestControllerAdvice -> saaf error JSON
```

Domino's jaisa: receptionist (Controller) order leta hai, manager (Service) tay karta hai, kitchen (Repository) pantry (DB) se nikaalta hai. Customer ko plate me saja pizza (DTO) milta hai, kachcha saamaan (Entity) nahi.

## Dikkat (prod trap)

Kisi ne jaldi me entity seedha lauta di:

```java
@GetMapping("/{id}")
public User get(@PathVariable Long id) {
    return repo.findById(id).orElseThrow();
}
```

Response me ye chala jaata hai:

```json
{"id":1,"name":"Ravi","email":"ravi@x.com","password":"$2a$10$Xk...","role":"ADMIN"}
```

Jackson poori entity JSON bana deta hai, password ka hash aur role bhi. Aur entity me gol rishta ho (User me Orders, Order me wapas User), to JSON ghoomta rehta hai aur `StackOverflowError` aata hai.

## Hal

- Bahar hamesha DTO bhejo: `UserResponse { id, name, email }`, sirf wahi jo dikhna chahiye.
- POST pe `@ResponseStatus(HttpStatus.CREATED)` lagao, warna 201 ki jagah 200 jaata hai.

## Khaas baat

- Data 3 jagah se aata hai: `/users/123` -> `@PathVariable` · `?active=true` -> `@RequestParam` · JSON body -> `@RequestBody`.
- `@Valid` method me ghusne se pehle hi rok deta hai: galat email aaya to 400, tera code chalta hi nahi.
- Har controller me try-catch nahi likhte: `@RestControllerAdvice` me ek jagah, `UserNotFound` pe 404, validation fail pe 400.

Tera usercrud: `UserController` -> `UserService` -> `UserRepository`. Upar `JwtFilter` (controller se pehle) aur `GlobalExceptionHandler` (`@RestControllerAdvice`), poora yahi dhaancha hai.
