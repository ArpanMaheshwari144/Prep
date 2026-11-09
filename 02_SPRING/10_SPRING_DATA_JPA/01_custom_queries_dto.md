# Custom @Query + DTO Projection

## Scene: dashboard pe sirf naam, umar aur shehar chahiye

`userRepo.findAll()` kiya to har user ki poori row aayi: password hash, phone, address, sab. Do dikkat:

- **Bojh:** 10 me se 7 column faaltu aaye.
- **Khatra:** entity seedha JSON me bhej di to password bahar gaya. Upar se dirty checking (password NULL wala trap), LazyInit aur `@Data`
  ka StackOverflow, ye teeno bhi isi raaste aate hain.

## Hal: sirf wahi 3 cheez maango, ek chhote dabbe (DTO) me

```java
public interface UserSummary { String getName(); int getAge(); String getCity(); }

@Query(value = "SELECT name AS name, age AS age, city AS city FROM users WHERE age > :age",
       nativeQuery = true)
List<UserSummary> findOlder(@Param("age") int age);
```

Query sirf 3 column laati hai, aur Spring unhe khud `UserSummary` me bhar deta hai. Niyam ek hi hai: `AS alias` ka naam getter se mile
(`AS city` aur `getCity()`). Query kitni bhi badi ho, 10 table ka JOIN bhi, tareeka yahi rahega. DTO entity nahi hai, isliye Hibernate ise
track nahi karta: na chupi UPDATE, na LazyInit.

## JPQL ya Native

- **JPQL** entity aur field ke naam pe likhte ho (`FROM User u WHERE u.age >= :minAge`, usercrud ke `UserRepository` me hai). DB badlo to bhi
  chalti hai, aur app start hote hi query check ho jaati hai, to galti turant pakdi jaati hai. Isme class DTO bhi likh sakte ho:
  `SELECT new com.example.dto.UserDto(u.name, u.age, u.city) FROM User u`.
- **Native** asli table aur column ke naam pe likhte ho. Isme DB ki poori taakat milti hai (window function waghera), par typo tabhi pakda
  jaata hai jab query chale. DTO ke liye yahan seedha raasta interface wala hai.
- Roz ka kaam JPQL se karo. Bhaari report jo JPQL me na bane, wahan native lo. UPDATE / DELETE wali query pe `@Modifying` aur
  `@Transactional` dono lagte hain.

## Prod wala trap

```sql
FROM users u LEFT JOIN orders o ON o.user_id = u.id
WHERE o.created_at >= '2026-10-01'      -- jinka order nahi (Ravi, Sita) wo GAAYAB
```

LEFT JOIN me jinka order nahi hai, unki order wali columns NULL aati hain. WHERE ki shart un NULL rows ko kaat deti hai, aur LEFT JOIN
chup-chaap INNER JOIN ban jaata hai. Ravi aur Sita list se gaayab ho jaate hain, aur koi error bhi nahi aata. Hal: shart ko
`ON o.user_id = u.id AND o.created_at >= '2026-10-01'` me likho. Tab bina order wale bhi 0 ke saath aate hain.
