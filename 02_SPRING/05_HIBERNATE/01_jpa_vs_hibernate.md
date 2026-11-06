# JPA vs Hibernate

## Kahani: Arpan ka PA

Arpan ek bada afsar hai. Uske paas ek "PA ka niyam" likha hua hai: file kaise rakhni hai, report kaise deni hai. Ye niyam bas kaagaz pe likha hai,
khud koi kaam nahi karta. Yahi niyam **JPA** hai.

Phir Arpan ne ek asli PA rakha jo register me likhta hai aur DB tak jaata hai. Ye PA **Hibernate** hai. Kal Hibernate ko hata ke
EclipseLink rakh do, to bhi niyam wahi rahega, aur tera code nahi badlega.

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

Tera code JPA ke niyam (`@Entity`, `@Id`, `EntityManager`) pe likha hota hai. Uske neeche Hibernate us niyam pe asli kaam karta hai aur SQL banata hai. Hibernate wo SQL JDBC ko deta hai, aur JDBC use Database pe chalata hai.

## 3 khaas baat

1. JPA ek spec hai, yaani sirf interface aur annotation. Hibernate uski implementation hai. Boot 3 me package ka naam `jakarta.persistence` hai,
   pehle ye `javax.persistence` tha.
2. `JpaRepository` JPA ka interface nahi hai, ye **Spring Data** ka interface hai. Ye JPA ke upar ek patli parat hai, jisme `findByEmail` jaise method ke
   naam se query khud ban jaati hai. Iske neeche bhi wahi Hibernate chalta hai.
3. Code JPA pe likho, Hibernate pe seedha nahi, taaki kal provider badalna ho to aasaan rahe.

## Prod wala trap

```java
import org.hibernate.annotations.Type;   // Hibernate ka apna, JPA ka nahi
```

Jaise hi code me `org.hibernate.*` ka import aaya, tera code ek hi provider se bandh jaata hai. Upgrade ke waqt (jaise Hibernate 5 se 6) yahi line toot-ti hai.
Isliye pehle `jakarta.persistence.*` hi use karo.

## Tera project

usercrud me `Author`, `Book` aur `Product` `@Entity` hain, jo JPA ka hissa hai. `BookRepository extends JpaRepository` Spring Data ka hai.
Hibernate ko Spring Boot ne `starter-data-jpa` se khud laga diya. Matlab code tune JPA ka likha, aur use chalaya Hibernate ne.
