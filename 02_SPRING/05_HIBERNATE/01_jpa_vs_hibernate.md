# JPA vs Hibernate

## Kahani: Arpan ka PA

Arpan ek bada afsar hai. Uske paas ek "PA ka niyam" kaagaz pe likha hai: file kaise rakhni hai, report kaise deni hai. Kaagaz khud koi kaam nahi
karta. Yahi niyam **JPA** hai (sirf interface aur annotation, yaani spec).

Phir Arpan ne ek asli PA rakha jo register me likhta hai aur DB tak jaata hai. Ye PA **Hibernate** hai (JPA ki implementation). Kal Hibernate ko
hata ke EclipseLink rakh do, to bhi niyam wahi rahega, aur tera code nahi badlega.

```
Tera code (@Entity, @Id, EntityManager)   <- JPA ka niyam
Hibernate (SQL banata)                     <- niyam pe kaam
JDBC -> Database
```

## Yaad ki cheez

`JpaRepository` JPA ka interface nahi hai, ye **Spring Data** ka interface hai. Isme `findByEmail` jaise method ke naam se query khud ban jaati hai,
aur iske neeche bhi wahi Hibernate chalta hai. Boot 3 me package ka naam `jakarta.persistence` hai, pehle ye `javax.persistence` tha.

## Prod wala trap

```java
import org.hibernate.annotations.Type;   // Hibernate ka apna, JPA ka nahi
```

Jaise hi code me `org.hibernate.*` ka import aaya, tera code ek hi provider se bandh jaata hai. Upgrade ke waqt (jaise Hibernate 5 se 6) yahi line
toot-ti hai. Isliye jahan tak ho sake `jakarta.persistence.*` hi use karo.

## Tera project

usercrud me `Author`, `Book` aur `Product` JPA wale `@Entity` hain. `BookRepository extends JpaRepository` Spring Data ka hai.
Hibernate ko Spring Boot ne `starter-data-jpa` se khud laga diya. Matlab code tune JPA ka likha, aur use chalaya Hibernate ne.
