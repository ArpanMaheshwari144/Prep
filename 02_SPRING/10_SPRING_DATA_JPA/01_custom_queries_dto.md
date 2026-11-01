# Custom @Query + DTO Projection

## Kahani: PA ko parchi pe sirf 3 cheez

Arpan ko dashboard pe sirf naam, umar, shehar chahiye. PA ko bola "user lao" to wo poori file laaya: password, phone,
address sab. Bojh bhi, khatra bhi (dirty checking wala password trap). Isliye PA ko parchi: "record room se sirf ye 3
cheez likh ke lana" = **DTO**. Faaltu data nahi, password bahar nahi, aur entity nahi to PA track nahi karta (na chupi
write, na LazyInit).

## Tasveer

```
Controller -> Service -> Repository (@Query) -> DB
                              |
             SELECT u.name, u.age, u.city ...   <- sirf 3 column
                              |
             UserSummary (getName, getAge, getCity)  <- parchi, Spring khud bharta
```

## Dikkat -> kyun

Entity seedha bahar = password leak · 10 me se 7 column faaltu · JSON banate waqt LAZY chhua to LazyInit · dono taraf
rishta to StackOverflow. Sab ek saath kaatne ko DTO.

Query kitni bhi badi (10 table JOIN) ho, lautane ka tareeka ek: `SELECT col AS alias` + interface jiske getter alias se
milein -> Spring khud bharta, hath se mapping nahi.

```java
public interface UserSummary { String getName(); int getAge(); String getCity(); }

@Query(value = "SELECT name AS name, age AS age, city AS city FROM users WHERE age > :age",
       nativeQuery = true)
List<UserSummary> findOlder(@Param("age") int age);

// JPQL me class DTO bhi: SELECT new com.example.dto.UserSummaryDTO(u.name, u.age, u.city) FROM User u
```

## Trade-off: JPQL ya Native

```
JPQL (entity + field naam)              Native (asli table + column)
DB badlo, query chalti                  DB ki poori taakat (window fn, DB ke khaas function)
app start pe hi check, galti turant     typo tab pakda jaata jab query chale
DTO: new pkg.UserDto(u.name, ...)       DTO: sirf interface projection (alias = getter)
```

Roz ka kaam JPQL, bhaari report / aggregation jo JPQL me na likhe -> native.
UPDATE / DELETE query -> `@Modifying` + `@Transactional`.

## Prod wala trap

```sql
FROM users u LEFT JOIN orders o ON o.user_id = u.id
WHERE o.created_at >= '1-Oct'          -- jinka order nahi (Ravi, Sita) wo GAAYAB
```

WHERE ki shart NULL rows kaat deti, LEFT JOIN chupchaap INNER ban jaata. Shart `ON ... AND o.created_at >= ...` me,
tab bina order wale 0 ke saath aate.

## Tera project

usercrud `UserRepository`: `@Query("FROM User u WHERE u.age >= :minAge")` · `AuthorRepository`: N+1 wala `LEFT JOIN FETCH`.

## Interview line

```
Custom @Query JPQL me entity naam pe likhta hoon, start pe check hoti aur DB se nahi bandhti;
bhaari report ho to native. Client ko entity nahi, DTO deta hoon: SELECT alias + interface
projection ya JPQL constructor expression, taaki sirf zaroori column jaayein, password jaisa
kuch leak na ho, aur LAZY / chupi write ka jhanjhat na rahe.
```
