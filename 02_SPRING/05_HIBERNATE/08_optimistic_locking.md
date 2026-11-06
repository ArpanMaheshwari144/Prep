# Optimistic Locking (@Version)

## Kahani: tera Confluence

Tu aur Ravi ek hi Confluence page khol ke baithe ho, aur dono ne version 5 khola hai. Ravi ne pehle save kiya, to page version 6 ho gaya. Ab tu save
dabata hai, to message aata hai "page badal gaya, reload karo". Agar ye rok na hoti, to tera save Ravi ka likha hua chup-chaap mita deta. Isi ko **lost update** kehte hain.
Wahi rok code me `@Version` hai.

## Tasveer

```
Row: stock=100, version=0. A aur B dono ne v0 padha.

A: UPDATE product SET stock=90, version=1 WHERE id=1 AND version=0   -> 1 row  (OK)
B: UPDATE product SET stock=80, version=1 WHERE id=1 AND version=0   -> 0 row  (DB me ab v1)
   -> OptimisticLockException -> B ko 409 -> B dobara padhe (90, v1) -> 70 bheje
```

Row me stock 100 aur version 0 hai, aur A aur B dono ne version 0 padha. A ka UPDATE pehle chala, 1 row badli, aur version 1 ho gaya. Ab B ka UPDATE bhi `version=0` dhoondhta hai, par DB me ab version 1 hai, isliye 0 row badalti hain. Tab `OptimisticLockException` aata hai, B ko 409 milta hai, B dobara padhta hai (90, v1), aur naye data pe hisaab laga ke 70 bhejta hai.

## Dikkat aur ye kyun bana

Do log ek saath ek row padhte hain, apna-apna hisaab lagate hain aur save karte hain, to baad wala pehle wale ka kaam mita deta hai. Ek hal ye hai ki pehle
lock laga do, taaki sab line me lagein. `@Version` ki soch alag hai: "clash kabhi-kabhi hi hota hai, to lock mat lagao, bas likhte waqt check kar lo ki beech me
kisi ne badla to nahi."

## Trade-off

Optimistic (`@Version`) me koi lock nahi lagta, isliye ye tez hai. Clash hone pe ek party haar jaati hai aur use retry karna padta hai. Ye wahan sahi hai jahan clash kam hote hain, jaise profile ya page edit.

Pessimistic (`SELECT ... FOR UPDATE`) me pehle lock lagta hai aur baaki sab line me intezaar karte hain. Clash pe intezaar hota hai, aur deadlock ka risk bhi rehta hai. Ye wahan sahi hai jahan clash zyada hote hain, jaise ek hi seat pe bheed.

Seat ya balance ke liye ek teesra tareeka bhi hai: `UPDATE ... SET x = x - ? WHERE x >= ?`. Ye ek atomic query hai, aur BMS aur banking me yahi lagta hai.

## Google Docs OT se jod (Arpan)

Dikkat dono jagah ek hi hai: do log ek cheez ek saath badal rahe hain. Farak nateeje me hai:

```
@Version (Confluence)   clash pe ek HAARTA: reject, "reload karo", dobara padh ke bhejo
OT (Google Docs)        koi nahi haarta: dono ka badlaav JOD deta ("shabd khiska do")
```

`@Version` me clash pe ek haarta hai: uska save reject hota hai, use "reload karo" milta hai, aur wo dobara padh ke bhejta hai. OT me koi nahi haarta, wo dono ka badlaav jod deta hai, jaise shabd thoda khiska ke dono ki line rakh do.

Agar badlaav jud sakte hain, jaise text me tu line 1 badal raha hai aur Ravi line 5, to OT lagao. Agar nahi jud sakte, jaise stock 90 aur 80 dono ek saath sach nahi ho sakte, to ek ko haarna padega aur
dobara padh ke bhejna padega, yaani version check.

## Prod wala trap

```java
catch (ObjectOptimisticLockingFailureException e) {
    p.setVersion(p.getVersion() + 1);   // version badha ke dobara save
    repo.save(p);
}
```

Isse wahi lost update wapas aa jaata hai. Version koi chaabi nahi hai jise badha ke darwaza khol lo, wo is baat ka saboot hai ki "maine sabse nayi row padhi hai". Isliye 409 aaye to version ko mat chhedo:
row dobara padho, naye data pe hisaab lagao, phir bhejo.

## Tera project

usercrud me `Product` pe `@Version` hai (22-Aug). `?version=0` bheja to OK aaya aur version 1 ho gaya. Dobara `?version=0` bheja to 409 STALE aaya.
