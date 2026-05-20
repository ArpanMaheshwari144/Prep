# Spring Annotations + DI Types

> **V90 — Spring 101: Topic 3**

---

## WHY — Annotations Kyu?

**Pre Spring 2.5:** XML mein bean definitions:
```xml
<bean id="userService" class="com.app.UserService">
    <property name="userRepo" ref="userRepo"/>
</bean>
```
**100 beans = 1000 lines XML hell.**

**Spring 2.5+:** Annotations — same kaam, 1/10 code, Java mein hi sab.

---

## Component Annotations — Family Tree

```
                @Component
                (generic — Spring-managed bean)
                     │
                     │ specializations
                     ▼
        ┌────────────┼────────────┬────────────┐
        ▼            ▼            ▼            ▼
   @Service    @Repository   @Controller   @RestController
   (business)  (DB layer)    (HTTP MVC)    (REST API)
```

**Sab internally `@Component`.** Spring ke liye fark nahi — code readability ke liye semantic specialization.

---

## Specialization Table

| Annotation | Layer | Extra Behavior |
|-----------|-------|----------------|
| `@Component` | Generic / utility | None — basic bean |
| `@Service` | Business logic | None — just semantic |
| `@Repository` | DB / data access | **Auto translates DB exceptions** to Spring's `DataAccessException` |
| `@Controller` | Web MVC (returns view names) | None |
| `@RestController` | REST API (returns JSON) | `@Controller + @ResponseBody` |

**Real value-add only `@Repository`** — exception translation. Baki sirf semantic.

---

## `@Component` vs `@Bean`

### `@Component` — class-level (TUMHARI class)
```java
@Component
class MyService { }     // tumhari class
```

### `@Bean` — method-level (3rd party class)
```java
@Configuration
class AppConfig {
    @Bean
    RestTemplate restTemplate() {        // 3rd party class
        return new RestTemplate();
    }
}
```

**`RestTemplate`** Spring/library ka class — **source modify nahi kar sakte**. Toh `@Bean` method banao.

---

## `@Autowired` — 3 Types of DI

### 1. Field Injection (DISCOURAGED)
```java
@Service
class UserService {
    @Autowired
    private UserRepository userRepo;
}
```
- `final` field nahi
- Test mein mock pass mushkil
- Hidden dependencies

### 2. Setter Injection (Rare)
```java
@Service
class UserService {
    private UserRepository userRepo;
    
    @Autowired
    public void setUserRepo(UserRepository userRepo) {
        this.userRepo = userRepo;
    }
}
```
**Use case:** Optional dependencies.

### 3. Constructor Injection (BEST)
```java
@Service
class UserService {
    private final UserRepository userRepo;     // final
    
    public UserService(UserRepository userRepo) {
        this.userRepo = userRepo;
    }
}
```
- `final` fields
- Test easy (`new UserService(mockRepo)`)
- Required deps enforced
- Spring official recommendation

---

## DI Comparison Table

| Feature | Field | Setter | Constructor |
|---------|-------|--------|-------------|
| `final` field | | | |
| Test without Spring | Mushkil | OK | Easy |
| Required deps enforced | | | |
| Industry standard | | | YES |

---

## Multiple Beans Resolution

```java
@Service class JpaUserRepo implements UserRepository { }
@Service class MongoUserRepo implements UserRepository { }
```

**Ambiguity!** Spring confused — kaunsa inject kare?

### Solution 1: `@Primary` (default pick)
```java
@Primary
@Service class JpaUserRepo implements UserRepository { }
```

### Solution 2: `@Qualifier` (explicit)
```java
@Autowired
@Qualifier("mongoUserRepo")
private UserRepository userRepo;
```

---

## POWER PHRASES

> *"`@Component` is the generic Spring-managed bean. `@Service`, `@Repository`, `@Controller` are semantic specializations — only `@Repository` adds real behavior (exception translation to Spring's DataAccessException)."*

> *"Constructor injection is recommended in Spring — final fields, easy testing, required dependencies enforced. Field injection via `@Autowired` is discouraged because it relies on reflection and harms testability."*

---

## TRAPS

| Trap | Reality |
|------|---------|
| "@Component vs @Bean — same hai" | NO — Component class-level, Bean method-level for 3rd party |
| "Field injection easy" | Yes but discouraged — final, testing, required-check fail |
| "Multiple beans = error" | Solve via `@Primary` ya `@Qualifier` |
| "@Repository = just label" | NO — adds exception translation, real value |
