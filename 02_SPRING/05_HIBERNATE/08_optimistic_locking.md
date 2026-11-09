# Optimistic Locking (@Version)

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
