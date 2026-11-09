# JPA vs Hibernate

## Scene: JDBC tu jaanta hai

Java me `Connection`, `PreparedStatement` sirf interface hain. Inme koi code nahi jo sach me MySQL tak jaaye. Asli kaam MySQL ka driver
(`mysql-connector-j` jar) karta hai. Kal Postgres pe jaana ho to bas driver ka jar badlo, tera `PreparedStatement` wala code wahi rehta hai.

JPA aur Hibernate bilkul yahi jodi hain, bas ek manzil upar:

- **JPA** = niyam (interface aur annotation): `@Entity`, `@Id`, `EntityManager.persist()`. Ye bas batata hai ki "object ko table me aise
  rakhna hai". Khud koi SQL nahi likhta.
- **Hibernate** = wo jar jo is niyam pe sach me kaam karta hai. `persist(book)` aaya to Hibernate `insert into book ...` banata hai, aur
  neeche JDBC se DB tak bhejta hai.

```
Tera code      @Entity, @Id, persist()        <- JPA (niyam, jaise JDBC ka interface)
Hibernate      insert into book ... banata     <- kaam karne wala (jaise MySQL driver)
JDBC           SQL DB tak le jaata
MySQL
```

## Tera project

usercrud me `Book.java` ki pehli line `import jakarta.persistence.*;` hai, matlab tune code JPA ka likha. Hibernate tune kahin naam se nahi
likha, `spring-boot-starter-data-jpa` ne use chupchaap jod diya. App start hote hi log me ye line aati hai:
`HHH000412: Hibernate ORM core version 6.x`. "HHH" Hibernate ki apni pehchaan hai, yaani niyam JPA ka hai aur chal Hibernate raha hai.
Boot 3 me package ka naam `jakarta.persistence` hai, pehle ye `javax.persistence` tha.

## Teesra naam jo confuse karta hai

`BookRepository extends JpaRepository` na JPA ka hai, na Hibernate ka. Ye **Spring Data** ka hai, jo JPA ke upar baitha hai. `findByEmail`
jaisa method naam padh ke query bana deta hai, aur iske neeche bhi wahi Hibernate chalta hai.

## Prod wala trap

```java
import org.hibernate.annotations.Type;   // Hibernate ka apna, JPA ka nahi
```

Code me `org.hibernate.*` ka import aaya to tu niyam chhod ke seedha driver se chipak gaya, wahi baat jaise JDBC chhod ke MySQL driver ki
andar ki class use kar lo. Hibernate 5 se 6 ke upgrade me aisi lines hi tootti hain. Isliye jahan tak ho sake `jakarta.persistence.*` hi
use karo.
