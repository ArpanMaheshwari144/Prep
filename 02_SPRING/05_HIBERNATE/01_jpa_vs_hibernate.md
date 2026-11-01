# JPA vs Hibernate

## Kahani: Arpan ka PA

Arpan bada afsar hai. Uske paas ek "PA ka niyam" hai: file kaise rakhni, report kaise deni. Niyam kaagaz pe likha hai,
khud kaam nahi karta. Niyam = **JPA**.

Arpan ne ek asli PA rakha jo register me likhta aur DB tak jaata hai. Ye PA = **Hibernate**. Kal Hibernate hata ke
EclipseLink rakh do, niyam wahi rahega, tera code nahi badlega.

## Tasveer

```
Tera code  (@Entity, @Id, @OneToMany, EntityManager)   <- JPA ka niyam
     |
Hibernate  (PA: asli ORM code, SQL banata)              <- niyam pe kaam
     |
JDBC       (SQL chalana)
     |
Database
```

## 3 khaas baat

1. JPA = spec (interface + annotation), Hibernate = implementation. Boot 3 me package `jakarta.persistence`,
   pehle `javax.persistence`.
2. `JpaRepository` JPA ka nahi, **Spring Data** ka interface hai: JPA ke upar patli parat, `findByEmail` jaise method
   naam se query khud banti. Neeche wahi Hibernate chalta.
3. Code JPA pe likho, Hibernate pe seedha nahi, taaki provider badalna aasaan rahe.

## Prod wala trap

```java
import org.hibernate.annotations.Type;   // Hibernate ka apna, JPA ka nahi
```

`org.hibernate.*` import aate hi code ek provider se bandh jaata. Upgrade me (Hibernate 5 -> 6) yahi line toot-ti.
Isliye `jakarta.persistence.*` pehle.

## Tera project

usercrud: `Author` / `Book` / `Product` = `@Entity` (JPA). `BookRepository extends JpaRepository` = Spring Data.
Hibernate ko Spring Boot ne `starter-data-jpa` se khud lagaya. Code tune JPA ka likha, chalaya Hibernate ne.

## Interview line

```
JPA ek specification hai (jakarta.persistence: @Entity, EntityManager), Hibernate uska
sabse common implementation jo asal me SQL banata aur JDBC se chalata hai. Spring Data JPA
iske upar repository layer deta hai. Main code JPA pe likhta hoon taaki provider se na bandhun.
```
