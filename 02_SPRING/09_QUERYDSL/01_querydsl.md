# QueryDSL — jitne filter aaye, utni hi shart

---

## Koi poochhe to aise bolo

Search page pe bahut saare filter hon aur sab optional (role, umar, shehar; koi bhara, koi khaali), to `@Query` ki string me
har filter ke liye `(:p IS NULL OR ...)` likhna gandagi ban jaata. QueryDSL me query Java code se banti: jo filter bhara
hai, sirf wahi shart jodo. Saath me `QUser` class compile ke waqt apne aap banti, to `user.age` jaisi field ki spelling ya
type galat ho to app chalne se pehle hi error, production me nahi.

---

## Scene: usercrud ka search

```
UI se aaya: role = ADMIN, umar = khaali, shehar = khaali
@Query string: (:role IS NULL OR u.role = :role) AND (:age IS NULL OR u.age >= :age) AND ...   <- har filter pe ek tukda
QueryDSL:      role hai?   -> where.and(user.role.eq(role))
               umar hai?   -> where.and(user.age.goe(minAge))     (goe = >=)
               baaki khaali -> kuch nahi joda
```

```java
QUser user = QUser.user;
BooleanBuilder where = new BooleanBuilder();
if (role != null)   where.and(user.role.eq(role));
if (minAge != null) where.and(user.age.goe(minAge));
return queryFactory.selectFrom(user).where(where).fetch();
```

Yaad ke liye: `.eq` == · `.goe` >= · `.loe` <= · `.gt` > · `.lt` < · `.like` · `.in`; join `.leftJoin(user.orders, order)`,
baaki SQL jaisa (`.orderBy`, `.groupBy`, `.limit`).

---

## Tere project me

usercrud ka [`UserSearchService`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/service/UserSearchService.java):
`JPAQueryFactory` inject, `BooleanBuilder` me optional shartein, endpoint
[`UserSearchController`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/UserSearchController.java).

---

## Kab kya lo

2-3 optional filter: `@Query` + null check kaafi. Bahut saare filter: JPA Specification bhi chalta, par code lamba;
QueryDSL usi ka saaf roop.

---

## Trap

`QUser` pehle se kahin nahi hoti: `mvn compile` pe `querydsl-apt` entity se banata. Compile kiye bina code likha to "`QUser`
nahi mili". Aur Spring Boot 3 ke baad dependency `jakarta` wali chahiye, warna Q-class banti hi nahi.
