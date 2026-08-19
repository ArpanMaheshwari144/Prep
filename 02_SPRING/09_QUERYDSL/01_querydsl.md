# QueryDSL — type-safe dynamic queries (flexible search)

> Jab OPTIONAL/dynamic filters bahut hon. @Query string nahi, Streams nahi —
> Q-classes + method-chaining, COMPILE-TIME checked.

---

## Kab QueryDSL (3 tools ka scale — decide)
- **@Query (null-check)**  → 2-3 optional filter tak theek. `(:p IS NULL OR u.f = :p)`.
- **JPA Specification**    → many dynamic filters. verbose lambda `(root,q,cb)->cb.equal(...)`.
- **QueryDSL**             → many dynamic + TYPE-SAFE + kam-verbose. (Specification ka clean bhai.)

## Setup (ek baar — pom + build)
1. `pom.xml` dependency: `querydsl-jpa` (classifier `jakarta` for Spring Boot 3+/4).
2. Annotation processor (maven-compiler-plugin annotationProcessorPaths): `querydsl-apt` (jakarta) + `jakarta.persistence-api`.
3. `mvn compile` → **Q-classes auto-generate** (`QUser`, target/generated-sources me).
   -> QUser = User ka type-safe mirror (user.role, user.age compile-time check).

## ★ Compile kyun zaroori (direct-run nahi hota)
Java = COMPILE-then-run. `mvn compile` do kaam karta:
1. **Q-classes GENERATE** — `QUser` pehle exist NAHI karti; `querydsl-apt` build-time pe `User` entity se banata. Bina compile → `QUser.user` milega hi nahi.
2. **CHECK (safety net)** — types/syntax verify. e.g. `user.age.goe("hi")` (number me string) → COMPILE-time error, run tak jaane hi nahi deta. Galti pehle pakdi, production me nahi.

BUILD SUCCESS = Q generate + sab wiring green. (Python/JS = direct-run; Java = compile pehle.)

## Code — 3 tukde
```java
// 1. CONFIG — JPAQueryFactory bean (ek baar)
@Bean JPAQueryFactory jpaQueryFactory() { return new JPAQueryFactory(entityManager); }

// 2. SERVICE — asli query
QUser user = QUser.user;
BooleanBuilder where = new BooleanBuilder();
if (role != null)   where.and(user.role.eq(role));      // optional filter
if (minAge != null) where.and(user.age.goe(minAge));    // goe = >=
return queryFactory.selectFrom(user).where(where).fetch();

// 3. CONTROLLER — @RequestParam(required=false) -> service call
```

## Operators (yaad)
`.eq()` == · `.goe()` >= · `.loe()` <= · `.gt()` > · `.lt()` < · `.like("%x%")` · `.in(list)`
BooleanBuilder + `.and()` / `.or()` se conditions jodo (null skip = optional filter).

## JOIN + baaki clauses (SQL ke jaisa hi)
`selectFrom(user)` = `SELECT * FROM user` (poori entity). Aur chahiye to chain karte jao:

```java
QUser user = QUser.user;  QOrder order = QOrder.order;
queryFactory
    .selectFrom(user)
    .leftJoin(user.orders, order)   // LEFT JOIN (path, alias); .innerJoin / .join bhi
    .where(order.amount.gt(1000))
    .fetch();
```

| SQL | QueryDSL |
|---|---|
| ORDER BY age DESC        | `.orderBy(user.age.desc())` |
| GROUP BY role            | `.groupBy(user.role)` |
| HAVING avg(age)>30       | `.having(user.age.avg().gt(30))` |
| LIMIT 10 OFFSET 20       | `.limit(10).offset(20)` |
| SELECT name,email (not *) | `.select(user.name, user.email)` (selectFrom ki jagah → Tuple) |
| DISTINCT                 | `.distinct()` |
| single row               | `.fetchOne()` / `.fetchFirst()` |

extra join-condition chahiye → `.on(...)`.

## Interview line
"Dynamic optional filters ke liye QueryDSL — Q-classes se type-safe query, compile-time checked.
 BooleanBuilder me conditions conditionally add karta, string-query ki galti ka risk nahi.
 Specification ka cleaner, less-verbose alternative."
