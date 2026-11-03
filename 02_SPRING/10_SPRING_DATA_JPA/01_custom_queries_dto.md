# Custom @Query + DTO Projection

## Kahani: PA ko parchi pe sirf 3 cheez

Arpan ko dashboard pe sirf naam, umar aur shehar chahiye. Usne PA ko bola "user lao", to PA poori file le aaya: password, phone,
address sab kuch. Isme bojh bhi hai aur khatra bhi, jaise dirty checking wala password trap. Isliye Arpan PA ko ek parchi deta hai: "record room se sirf ye 3
cheez likh ke lana". Yahi parchi **DTO** hai. Isse faaltu data nahi aata, password bahar nahi jaata, aur kyunki ye entity nahi hai, PA ise track nahi karta. Matlab na koi chupi
write hoti hai, na LazyInit wali galti aati hai.

## Tasveer

```
Controller -> Service -> Repository (@Query) -> DB
                              |
             SELECT u.name, u.age, u.city ...   <- sirf 3 column
                              |
             UserSummary (getName, getAge, getCity)  <- parchi, Spring khud bharta
```

Request Controller se Service, phir Repository ki `@Query` se DB tak jaati hai. Query sirf 3 column maangti hai, aur jo aata hai use Spring khud `UserSummary` parchi me bhar deta hai.

## Dikkat aur DTO kyun

Agar entity seedha bahar bhej di, to chaar dikkat aati hain. Pehli, password leak ho jaata hai. Doosri, 10 me se 7 column faaltu jaate hain. Teesri, JSON banate waqt koi LAZY field chhua gaya to LazyInit exception aata hai. Chauthi, agar rishta dono taraf ka hai to JSON gol-gol ghoom ke StackOverflow de deta hai. In sab ko ek saath kaatne ke liye DTO hai.

Query kitni bhi badi ho, chahe 10 table ka JOIN ho, lautane ka tareeka ek hi hai: `SELECT col AS alias` likho, aur ek interface banao jiske getter us alias se
milein. Phir Spring khud parchi bhar deta hai, haath se mapping nahi likhni padti.

```java
public interface UserSummary { String getName(); int getAge(); String getCity(); }

@Query(value = "SELECT name AS name, age AS age, city AS city FROM users WHERE age > :age",
       nativeQuery = true)
List<UserSummary> findOlder(@Param("age") int age);

// JPQL me class DTO bhi: SELECT new com.example.dto.UserSummaryDTO(u.name, u.age, u.city) FROM User u
```

## Trade-off: JPQL ya Native

JPQL entity aur field ke naam pe likhi jaati hai. Iska fayda ye hai ki DB badal do to bhi query chalti rehti hai, aur app start pe hi query check ho jaati hai, isliye galti turant pakdi jaati hai. JPQL me DTO ke liye `new pkg.UserDto(u.name, ...)` jaisa class DTO bhi likh sakte ho.

Native query asli table aur column ke naam pe likhi jaati hai. Iska fayda ye hai ki DB ki poori taakat milti hai, jaise window function aur DB ke khaas function. Par typo tab pakda jaata hai jab query chalti hai, pehle nahi. Aur native me DTO ke liye sirf interface projection chalta hai, jisme alias getter se milta hai.

Roz ka kaam JPQL se karo. Bhaari report ya aggregation jo JPQL me likhi na ja sake, wahan native lo.
UPDATE ya DELETE wali query pe `@Modifying` aur `@Transactional` dono lagte hain.

## Prod wala trap

```sql
FROM users u LEFT JOIN orders o ON o.user_id = u.id
WHERE o.created_at >= '1-Oct'          -- jinka order nahi (Ravi, Sita) wo GAAYAB
```

LEFT JOIN me jinka order nahi hai, unki order wali columns NULL aati hain. Par WHERE ki shart un NULL rows ko kaat deti hai, isliye LEFT JOIN chup-chaap INNER JOIN ban jaata hai aur Ravi, Sita gaayab ho jaate hain. Hal ye hai ki shart ko `ON ... AND o.created_at >= ...` me likho,
tab bina order wale users bhi 0 ke saath aate hain.

## Tera project

usercrud ke `UserRepository` me `@Query("FROM User u WHERE u.age >= :minAge")` hai, aur `AuthorRepository` me N+1 wala `LEFT JOIN FETCH` hai.

## Interview line

```
Custom @Query JPQL me entity naam pe likhta hoon, start pe check hoti aur DB se nahi bandhti;
bhaari report ho to native. Client ko entity nahi, DTO deta hoon: SELECT alias + interface
projection ya JPQL constructor expression, taaki sirf zaroori column jaayein, password jaisa
kuch leak na ho, aur LAZY / chupi write ka jhanjhat na rahe.
```
