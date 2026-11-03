# Spring REST Flow

## Kahani

Tu Domino's app pe pizza order karta hai:

```
Tu -> Receptionist (Controller) -> Manager (Service) -> Kitchen (Repository) -> Pantry (DB)
```

- **Receptionist:** order leta hai aur check karta hai ki order poora likha hai ya nahi.
- **Manager:** tay karta hai ki kaam kaise hoga.
- **Kitchen:** pantry se saamaan nikaalta hai.
- **Customer ko kya milta hai:** plate me saja hua pizza (DTO), kitchen ka kachcha saamaan (Entity) nahi.

## Tasveer

Ek POST request ka poora safar:

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

Request pehle Tomcat pe aati hai, phir filter chain se guzarti hai. Yahin `JwtFilter` token jaanchta hai, aur ye Spring MVC se PEHLE hota hai, isliye token galat hai to 401 yahin se laut jaata hai. Phir DispatcherServlet URL dekh ke sahi controller method dhoondhta hai. JSON ko `UserRequest` me badla jaata hai aur `@Valid` chalta hai, galat data hai to 400. Phir Controller, Service (`@Transactional`) aur Repository se hoke DB me INSERT hota hai. Wapsi me Entity ko DTO me badla jaata hai, Jackson use JSON banata hai, aur 201 jaata hai. Raste me kahin bhi exception aayi, to `@RestControllerAdvice` use ek saaf error JSON me badal deta hai.

## 4 khaas baat

1. **Request me data teen jagah se aata hai:**
   - `/users/123` jaisa URL ka hissa `@PathVariable` se milta hai.
   - `?active=true` jaisi query `@RequestParam` se milti hai.
   - JSON body `@RequestBody` se milti hai.
2. **Bahar hamesha DTO bhejo, entity kabhi nahi.** Entity me password, role aur andar ke fields hote hain. `UserResponse { id, name, email }` me sirf wahi jaata hai jo dikhna chahiye.
3. **`@Valid` method me ghusne se pehle hi rok deta hai.** Galat email ya khaali naam aaya to 400 jaata hai, aur tera code chalta hi nahi.
4. **Har controller me try-catch nahi likhna padta.** `@RestControllerAdvice` me ek jagah likho: `UserNotFound` aaye to 404, validation fail ho to 400. Aur security ka filter DispatcherServlet se bhi pehle chalta hai, isliye galat token wali request controller tak pahunchti hi nahi.

## Prod wala trap

```java
@GetMapping("/{id}")
public User get(@PathVariable Long id) {    // seedha entity lauta di
    return repo.findById(id).orElseThrow();
}
```

Jackson poori entity JSON bana deta hai, aur `password` (hash) bhi response me chala jaata hai. Agar entity me doosri entity ka link ho (User ke andar Orders, aur Orders ke andar wapas User), to JSON gol-gol ghoom ke `StackOverflowError` bhi de deta hai. Ilaaj: DTO lautao. Aur POST pe `@ResponseStatus(HttpStatus.CREATED)` lagao, warna wo bhi 200 hi deta hai.

## Tera project

usercrud me poora yahi dhaancha hai: `UserController` request leta hai, `UserService` ko deta hai, aur service `BookRepository` ya `UserRepository` se DB tak jaati hai. Global error ke liye `GlobalExceptionHandler` (`@RestControllerAdvice`) hai, aur controller se pehle `JwtFilter` chalta hai.

## Interview line

```
A request goes through the filter chain (security), then DispatcherServlet maps it to a controller.
Spring binds path variables, query params and the JSON body, runs @Valid, and the controller calls
the service, which owns the transaction and calls the repository. I return DTOs, never entities,
and handle errors centrally with @RestControllerAdvice.
```

[← Back to SPRING](../)
