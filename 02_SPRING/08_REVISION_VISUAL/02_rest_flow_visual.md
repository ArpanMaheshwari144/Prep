# Spring REST Flow — Visual Revision

---

## 1 Typical Request Flow (Pizza Restaurant)

```
ANALOGY: Tu Domino's app pe pizza order karta

   YOU (browser)
        │
        ▼
   ┌──────────────┐
   │  Receptionist │  ← Controller (takes order)
   │  (@Controller)│
   └──────┬───────┘
          │
          ▼
   ┌──────────────┐
   │  Manager     │  ← Service (business logic)
   │  (@Service)   │     decides flow
   └──────┬───────┘
          │
          ▼
   ┌──────────────┐
   │  Kitchen     │  ← Repository (DB access)
   │  (@Repository)│     fetches data
   └──────┬───────┘
          │
          ▼
   ┌──────────────┐
   │  PANTRY      │  ← Database
   │  (MySQL)      │     stores stuff
   └──────────────┘
```

---

## 2 4 Layers — Visual

```
   USER REQUEST
        │
        ▼
   ┌─────────────────────────────────┐
   │  CONTROLLER (REST API)          │  ← Handle HTTP
   │  @RestController                 │     URL mapping
   └────────────┬────────────────────┘     Validation
                │
                ▼
   ┌─────────────────────────────────┐
   │  SERVICE (Business Logic)       │  ← Decide flow
   │  @Service                        │     Coordinate
   └────────────┬────────────────────┘     Transactions
                │
                ▼
   ┌─────────────────────────────────┐
   │  REPOSITORY (Data Access)       │  ← DB queries
   │  @Repository / JpaRepository     │     CRUD
   └────────────┬────────────────────┘
                │
                ▼
   ┌─────────────────────────────────┐
   │  DATABASE (MySQL/Postgres)      │
   └─────────────────────────────────┘
```

---

## 3 Code Reality — User Endpoint

### Controller Layer
```java
@RestController
@RequestMapping("/api/users")
public class UserController {

    private final UserService service;                 // constructor injection (field @Autowired nahi)
    public UserController(UserService service) { this.service = service; }

    @GetMapping("/{id}")
    public UserResponse getUser(@PathVariable Long id) {   // DTO lautao, entity nahi (section 6)
        return service.findById(id);
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)                    // bina iske POST bhi 200 deta
    public UserResponse create(@RequestBody @Valid UserRequest req) {
        return service.create(req);
    }
}
```

### Service Layer
```java
@Service
public class UserService {

    private final UserRepository repo;
    public UserService(UserRepository repo) { this.repo = repo; }

    @Transactional(readOnly = true)
    public UserResponse findById(Long id) {
        User user = repo.findById(id)
                        .orElseThrow(() -> new UserNotFound(id));
        return UserResponse.from(user);                    // entity -> DTO (password bahar nahi jaata)
    }

    @Transactional
    public UserResponse create(UserRequest req) {
        // Business logic yahan
        User user = new User(req.name(), req.email());
        return UserResponse.from(repo.save(user));
    }
}
```

### Repository Layer
```java
@Repository
public interface UserRepository
        extends JpaRepository<User, Long> {

    Optional<User> findByEmail(String email);
    // Spring Data JPA = auto-implementation
}
```

---

## 4 HTTP Annotations Cheat

```
┌─────────────────────┬──────────────────────┐
│  Annotation         │  HTTP Method         │
├─────────────────────┼──────────────────────┤
│  @GetMapping        │  GET (read)          │
│  @PostMapping       │  POST (create)       │
│  @PutMapping        │  PUT (full update)   │
│  @PatchMapping      │  PATCH (partial)     │
│  @DeleteMapping     │  DELETE              │
└─────────────────────┴──────────────────────┘
```

---

## 5 Path Variable vs Request Param vs Body

```
URL:  GET /api/users/123?active=true

   /api/users/123       ← @PathVariable
                         (part of URL path)

   ?active=true         ← @RequestParam
                         (query string)


URL:  POST /api/users
Body: { "name": "Arpan", "email": "..." }

   Body content         ← @RequestBody
                         (JSON in body)
```

```java
@GetMapping("/{id}")
public User get(
    @PathVariable Long id,                    // 123
    @RequestParam(required=false) Boolean active  // true
) { ... }

@PostMapping
public User create(@RequestBody UserRequest req) { ... }
```

---

## 6 DTO Pattern — Why?

```
PROBLEM:
   Tu ne User entity expose ki API mein:
      User { id, name, email, password, role, createdAt }
   = Password leak
   = Internal fields exposed
```

```
SOLUTION: DTO (Data Transfer Object)

   ┌─────────────────────────────────┐
   │  USER ENTITY (DB)                │
   │  id, name, email, password, role │
   └────────────────────────────────┘
              │
              │ map (only safe fields)
              ▼
   ┌─────────────────────────────────┐
   │  USER DTO (API response)         │
   │  id, name, email                 │
   └────────────────────────────────┘
              │
              ▼
        Sent to client
```

```
INPUT DTOs:
   LoginRequest    { email, password }
   RegisterRequest { name, email, password }

OUTPUT DTOs:
   UserResponse   { id, name, email }  ← password chhupa
   TokenResponse  { accessToken, refreshToken }

ERROR DTOs:
   ErrorResponse  { code, message, timestamp }
```

---

## 7 Validation — @Valid

```java
public record UserRequest(
    @NotBlank
    @Size(min=2, max=50)
    String name,

    @Email
    @NotBlank
    String email,

    @Size(min=8)
    String password
) {}

// Controller:
@PostMapping
public User create(@RequestBody @Valid UserRequest req) {
    // Spring validates BEFORE entering method
    // Invalid? throws MethodArgumentNotValidException
}
```

```
VALIDATION ANNOTATIONS:
   @NotBlank   → string not empty/whitespace
   @NotNull    → not null
   @Size       → length check
   @Min/@Max   → number range
   @Email      → email format
   @Pattern    → regex
```

---

## 8 Exception Handler — Global

```
PROBLEM:
   Har controller mein try-catch?
   = Repetitive
   = DRY principle break

SOLUTION:
   @RestControllerAdvice = global exception handler
```

```java
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(UserNotFound.class)
    public ResponseEntity<ErrorResponse> handle(UserNotFound e) {
        return ResponseEntity
                .status(404)
                .body(new ErrorResponse(404, e.getMessage()));
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResponseEntity<?> handleValidation(...) {
        // Validation errors
    }
}
```

```
FLOW:
   Anywhere in code:
      throw new UserNotFound(id)
                  │
                  ▼
   Spring intercepts
                  │
                  ▼
   GlobalExceptionHandler ka matching method
                  │
                  ▼
   Return clean error response (404)
```

---

## 9 Full Request Flow (End-to-End)

```
   Postman: POST /api/users
   Body: { name: "Arpan", email: "..." }
        │
        ▼
   ┌─────────────────────────┐
   │ Tomcat (embedded)       │  ← HTTP request aayi
   └────────────┬────────────┘
                ▼
   ┌─────────────────────────┐
   │ Filter chain            │  ← Security (JwtFilter) yahin — Spring MVC se PEHLE
   │ token galat? → 401      │
   └────────────┬────────────┘
                ▼
   ┌─────────────────────────┐
   │ DispatcherServlet       │  ← Spring MVC ka front controller
   │ HandlerMapping          │     URL + method se kaunsa controller method? dhoondha
   └────────────┬────────────┘
                ▼
   ┌─────────────────────────┐
   │ Argument resolve        │  ← JSON body -> UserRequest (Jackson)
   │ + Validation (@Valid)   │
   │ Invalid? → 400 Bad Req  │
   └────────────┬────────────┘
                │ valid
                ▼
   ┌─────────────────────────┐
   │ UserController.create() │
   └────────────┬────────────┘
                │
                ▼
   ┌─────────────────────────┐
   │ UserService.create()    │  ← business logic
   │ @Transactional starts   │
   └────────────┬────────────┘
                │
                ▼
   ┌─────────────────────────┐
   │ UserRepository.save()   │
   └────────────┬────────────┘
                │
                ▼
   ┌─────────────────────────┐
   │ DB INSERT               │
   └────────────┬────────────┘
                │
                ▼
   ┌─────────────────────────┐
   │ Entity → DTO mapping    │  ← service me (UserResponse.from)
   └────────────┬────────────┘
                │
                ▼
   ┌─────────────────────────┐
   │ Jackson: DTO → JSON     │  ← HttpMessageConverter
   │ 201 (@ResponseStatus    │     bina iske 200
   │      CREATED ki wajah)  │
   └─────────────────────────┘

   Kahin bhi exception -> @RestControllerAdvice -> saaf error JSON (404 / 400 ...)
```

---

## Memory Hook

```
Receptionist = Controller
Manager     = Service
Kitchen     = Repository
Pantry      = Database

DTO = Plate (presented)
Entity = Raw ingredient (kitchen)
@Valid = Quality check at receptionist
@RestControllerAdvice = Manager handles ALL complaints
```

[← Back to SPRING](../)
