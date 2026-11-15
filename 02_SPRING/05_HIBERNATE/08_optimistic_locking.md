# Optimistic Locking (@Version)

## Koi poochhe "optimistic locking kya hai" to aise bolo

```
Entity pe @Version: Hibernate har UPDATE me "AND version = jo padha tha" jod deta.
Beech me kisi ne badla to 0 row badlti, exception: "purani row padhi thi, dobara padho".
Lock nahi, isliye tez; jahan takraav kabhi-kabhi (profile / page edit). Bheed wali cheez (seat, balance) = ek query me shart ya FOR UPDATE.
```

## Scene: tera Confluence

Tu aur Ravi ek hi page khol ke baithe ho, dono ke paas version 5 hai. Ravi ne pehle save kiya, page version 6 ho gaya. Ab tu save dabata hai
to aata hai "page badal gaya, reload karo". Ye rok na hoti to tera save Ravi ka likha chup-chaap mita deta. Isi ko **lost update** kehte hain,
aur code me wahi rok `@Version` hai.

usercrud (22-Aug): `Product` pe `@Version` hai. `?version=0` bheja to OK aaya aur version 1 ho gaya. Dobara `?version=0` bheja to
**409 STALE** aaya.

## Andar kya hua

```
Row: stock=100, version=0. A aur B dono ne v0 padha.

A: UPDATE product SET stock=90, version=1 WHERE id=1 AND version=0   -> 1 row badli  (OK)
B: UPDATE product SET stock=80, version=1 WHERE id=1 AND version=0   -> 0 row (DB me ab v1)
   -> OptimisticLockException -> B ko 409 -> B dobara padhe (90, v1) -> 70 bheje
```

Hibernate har UPDATE me `AND version = jo padha tha` jod deta hai. Beech me kisi ne badla to 0 row badlengi, aur Hibernate samajh jaata hai
ki tu purani row pe likh raha tha.

Aaj chala ke dekha (Boot 4 + Hibernate 7, stock 100, A aur B dono ne v0 padha):

```
@Version ke saath
update product set stock=?,version=? where id=? and version=?   <- Hibernate ne khud jodi shart
>> A ne 90 save kiya -> OK
>> B ne 80 save kiya -> ObjectOptimisticLockingFailureException
>> DB me: stock=90, version=1

bina @Version
>> A ne 90 -> OK,  B ne 80 -> OK (lost update!)
>> DB me: stock=80   <- A ka 90 chup-chaap mit gaya
```

DB 07 wala hi "dekho aur likho ek jagah", bas shart `version` ki aur Hibernate khud jodta.

## Soch aur trade-off

"Clash kabhi-kabhi hi hota hai, to lock mat lagao, bas likhte waqt check kar lo."

- **Optimistic (`@Version`):** koi lock nahi, tez. Clash pe ek haarta hai aur dobara karta hai. Jahan clash kam hon wahan sahi, jaise profile
  ya page edit.
- **Pessimistic (`SELECT ... FOR UPDATE`):** pehle lock, baaki line me rukte hain, deadlock ka risk bhi. Jahan ek hi cheez pe bheed ho wahan sahi.
- **Seat / balance:** `UPDATE ... SET x = x - ? WHERE x >= ?`, ek atomic query. BMS aur banking me yahi lagta hai.

## Google Docs se farak (Arpan ka jod)

Dikkat dono jagah ek hi hai, do log ek cheez ek saath badal rahe hain. `@Version` me ek haarta hai ("reload karo"). OT me koi nahi haarta,
dono ka badlaav jud jaata hai. Text me line 1 aur line 5 jud sakti hain, to OT. Stock 90 aur 80 dono ek saath sach nahi ho sakte, to version
check.

## Prod wala trap

```java
catch (ObjectOptimisticLockingFailureException e) {
    p.setVersion(p.getVersion() + 1);   // version badha ke dobara save
    repo.save(p);
}
```

Isse wahi lost update wapas aa jaata hai. Version koi chaabi nahi hai jise badha ke darwaza khol lo. Wo is baat ka saboot hai ki "maine sabse
nayi row padhi hai". 409 aaye to version ko mat chhedo: row dobara padho, naye data pe hisaab lagao, phir bhejo.
