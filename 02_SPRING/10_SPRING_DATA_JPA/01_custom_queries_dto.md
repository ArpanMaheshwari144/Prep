# Custom @Query + DTO Projection (Repository → Service → Controller)

> Real-project reference: badi query kaise likhi + result kaise return (DTO/projection).
> **Core rule:** query kitni bhi badi (10-table JOIN) ho — return-mechanism WAHI. `SELECT col AS alias` + matching getter wala interface/DTO → Spring khud bhar deta.

---

## 0. Layered flow (ek line)
Client → **Controller** (`@GetMapping`, params) → **Service** (logic) → **Repository** (`@Query` DB) → answer ulta same raste client tak.

---

## 1. Entity vs DTO (kyun DTO?)
- **Entity** = poori table-row (saari columns).
- **DTO** = sirf jo fields client ko dena (10 me se 3). Extra/sensitive fields (password etc.) chhup jaate.
- Jab poori entity nahi, chuni fields / aggregation chahiye → DTO/projection.

---

## 2. @Query ke 2 flavour

| | JPQL | Native |
|--|--|--|
| Likha kis pe | **Entity + field** naam (`User`, `u.age`) | **Asli table + column** (`users`, `age`) |
| Flag | (default) | `nativeQuery = true` |
| DTO kaise | `SELECT new pkg.DTO(...)` constructor-expr | **interface projection** (`new DTO()` NAHI chalta) |

Named param dono me: `:age` + `@Param("age")`.

---

## 3. Simple query + full flow

```java
// ENTITY
@Entity @Table(name = "users")
public class User {
    @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String name;
    private int age;
    private String city;
    // getters/setters
}

// REPOSITORY
@Repository
public interface UserRepository extends JpaRepository<User, Long> {

    // JPQL -- entity/field naam
    @Query("SELECT u FROM User u WHERE u.age > :age AND u.city = :city")
    List<User> findOlderInCity(@Param("age") int age, @Param("city") String city);

    // NATIVE -- asli table/column
    @Query(value = "SELECT * FROM users WHERE age > :age ORDER BY age DESC",
           nativeQuery = true)
    List<User> findOlderNative(@Param("age") int age);

    // single value
    @Query("SELECT COUNT(u) FROM User u WHERE u.city = :city")
    long countByCity(@Param("city") String city);
}

// SERVICE
@Service
public class UserService {
    private final UserRepository repo;
    public UserService(UserRepository repo) { this.repo = repo; }   // constructor injection
    public List<User> getOlder(int age, String city) {
        return repo.findOlderInCity(age, city);
    }
}

// CONTROLLER
@RestController
@RequestMapping("/users")
public class UserController {
    private final UserService service;
    public UserController(UserService service) { this.service = service; }

    @GetMapping("/older")   // GET /users/older?age=25&city=Delhi
    public List<User> older(@RequestParam int age, @RequestParam String city) {
        return service.getOlder(age, city);
    }
}
```

---

## 4. DTO return -- 2 tareeke

### (A) NATIVE query → INTERFACE projection
Native me `new DTO()` nahi chalta → interface banao, getter = column **alias** se match.

```java
public interface UserSummary {          // getters
    String getName();
    int    getAge();
    String getCity();
}

@Query(value = "SELECT name AS name, age AS age, city AS city " +
               "FROM users WHERE age > :age ORDER BY age DESC",
       nativeQuery = true)
List<UserSummary> findOlderSummaryNative(@Param("age") int age);
```
> **Alias ↔ getter match = jaadu.** `col AS name` ↔ `getName()`.

### (B) JPQL query → DTO class (constructor expression)
```java
public class UserSummaryDTO {
    private String name; private int age; private String city;
    public UserSummaryDTO(String name, int age, String city) {   // ZAROORI, fields ke order me
        this.name = name; this.age = age; this.city = city;
    }
    // getters
}

@Query("SELECT new com.example.dto.UserSummaryDTO(u.name, u.age, u.city) " +
       "FROM User u WHERE u.age > :age")
List<UserSummaryDTO> findOlderSummaryJpql(@Param("age") int age);
```
> **Full package path** + matching **constructor** chahiye.

---

## 5. BADI query (JOIN + aggregation) → interface projection

```java
public interface CustomerOrderSummary {
    Long          getUserId();
    String        getName();
    String        getEmail();
    Long          getTotalOrders();
    Double        getTotalSpent();
    LocalDateTime getLastOrderDate();
}

@Query(value =
    "SELECT u.id                                  AS userId, " +
    "       u.name                                AS name, " +
    "       u.email                               AS email, " +
    "       COUNT(DISTINCT o.id)                  AS totalOrders, " +
    "       COALESCE(SUM(oi.quantity * p.price),0) AS totalSpent, " +
    "       MAX(o.created_at)                     AS lastOrderDate " +
    "FROM users u " +
    "LEFT JOIN orders      o  ON o.user_id   = u.id " +
    "LEFT JOIN order_items oi ON oi.order_id = o.id " +
    "LEFT JOIN products    p  ON p.id        = oi.product_id " +
    "WHERE u.city = :city AND o.created_at >= :fromDate " +
    "GROUP BY u.id, u.name, u.email " +
    "HAVING COUNT(DISTINCT o.id) > :minOrders " +
    "ORDER BY totalSpent DESC",
    nativeQuery = true)
List<CustomerOrderSummary> getCustomerSummaries(@Param("city") String city,
                                                @Param("fromDate") LocalDateTime fromDate,
                                                @Param("minOrders") int minOrders);
```

**Badi query ke tukde:**
- **JOINs** — kai tables jodo.
- **Aggregation** `COUNT/SUM/MAX` — count, total, last-date.
- **GROUP BY** — per-user ek row · **HAVING** — group-level filter (`WHERE`=row-level).
- **COALESCE(...,0)** — NULL → 0.

---

## 6. Yaad-rakh (crux)
- Query 10-table JOIN ho ya simple → **return-mechanism NAHI badalta**: `SELECT col AS alias` + matching getter wala DTO/interface → Spring bhar deta, **manual mapping nahi**.
- **Native → interface projection** (alias = getter). **JPQL → `new pkg.DTO(...)`** (full path + constructor).
- Entity heavy/private → DTO se sirf zaroori fields expose.
- **UPDATE/DELETE query** → `@Modifying` + `@Transactional` chahiye (`@Query("UPDATE User u SET u.city=:c WHERE u.id=:id")`).
