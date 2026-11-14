# Docker 02 — Spring laptop pe, MySQL dabbe me

[← Back to README](00_README.md) | [← Foundation](01_foundation.md) | [Volumes →](03_volumes.md)

---

## Koi poochhe to aise bolo

**Kya kiya:** usercrud ka Spring laptop pe hi chalaya, aur uska database (MySQL) Docker ke dabbe me. Spring laptop ke `localhost:3307` darwaze se dabbe wale MySQL tak pahunchta hai, kyunki pichhli file me laptop ka 3307 dabbe ke andar ke 3306 se joda tha.

**Kyun aise:** MySQL laptop pe install karne ka jhanjhat nahi (version, service, purana MySQL). Dabba chalao, kaam khatam to hatao. Aur team me sab ka MySQL ek hi version.

---

## Kahani: ghar badla, saamaan wahi

```
pehle  H2   = kaagaz pe kachcha hisaab (app band, sab gaya)
ab     MySQL = asli bahi-khaata, dabbe me rakha
app ka code = wahi ka wahi, ek line nahi badli
badla sirf pata: kahan hai, kaun si chaabi, kaun si bhasha
```

DB badalne ke liye `application.properties` me sirf chaar cheez badli: DB kahan hai (URL), Java se baat karne wali library (driver), username / password, aur Hibernate ko DB ki SQL bhasha (dialect). Code me koi badlaav nahi, yahi JPA ka fayda hai.

---

## Ek trap jo khud dekha

Build ho gaya, par chalate hi error: Hibernate ko `MySQL8Dialect` mila hi nahi. Purane blog / Stack Overflow yahi naam batate hain, par Spring Boot 4 ke saath naya Hibernate (7) aata hai, jisme wo purana naam hata diya gaya. Hal: seedha `MySQLDialect`, jo khud pehchan leta hai ki MySQL kaunsa version hai. Sabak: version badla to purane tutorial ki line aankh band karke mat utaaro.

(Raste me ek chhota jhatka aur: `mvn` command chali hi nahi, Maven laptop pe tha nahi. Project ke saath aane wala `mvnw` bhi chal jaata, bina install ke.)

---

## Poora chakkar chala ke dekha

Postman se register kiya, phir login. Uske baad dabbe ke andar ghus ke MySQL me dekha to user ki row wahin baithi thi, aur refresh token bhi. Matlab: request laptop ke Spring pe aayi, Spring ne dabbe wale MySQL me likha, aur data sach me dabbe me hai.

Aur laptop pe do MySQL ek saath chal rahe the: purana wala 3306 pe, dabbe wala 3307 pe. Dono ka data alag, ek doosre ko pata bhi nahi. Yahi dabbe ki alag-thalag duniya hai.

---

## Dikkat jisse agla tukda bana

Dabba hata diya to uske andar ka saara data bhi gaya. Asli DB ke liye ye nahi chalega. Data dabbe ke bahar kaise bachaayein, wo **Volume** ([03](03_volumes.md)).

[← Back to README](00_README.md) | [← Foundation](01_foundation.md) | [Volumes →](03_volumes.md)
