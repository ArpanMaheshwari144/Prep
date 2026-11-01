# Optimistic Locking (@Version)

## Kahani: tera Confluence

Tu aur Ravi ek Confluence page khol ke baithe, dono ne version 5 khola. Ravi pehle save -> page version 6. Tu save
dabata -> "page badal gaya, reload karo". Ye rok na hoti to tera save Ravi ka likha chupchaap mita deta = **lost update**.
Wahi rok = `@Version`.

## Tasveer

```
Row: stock=100, version=0. A aur B dono ne v0 padha.

A: UPDATE product SET stock=90, version=1 WHERE id=1 AND version=0   -> 1 row  (OK)
B: UPDATE product SET stock=80, version=1 WHERE id=1 AND version=0   -> 0 row  (DB me ab v1)
   -> OptimisticLockException -> B ko 409 -> B dobara padhe (90, v1) -> 70 bheje
```

## Dikkat -> kyun bana

Do log ek saath ek row padhte, apna hisaab laga ke save karte, baad wala pehle wale ka kaam mita deta. Ek hal = pehle
lock (sab line me). `@Version` ki soch: "clash kabhi-kabhi hota hai, lock mat lagao, likhte waqt check karo ki beech me
kisi ne badla to nahi."

## Trade-off

```
Optimistic (@Version)          Pessimistic (SELECT ... FOR UPDATE)
lock nahi, tez                 pehle lock, baaki line me
clash pe ek haarega -> retry   clash pe intezaar, deadlock ka risk
kam clash (profile, page edit) zyada clash (ek hi seat pe bheed)
```

Seat / balance ke liye teesra: `UPDATE ... SET x = x - ? WHERE x >= ?` (ek atomic query, BMS / banking me yahi).

## Google Docs OT se jod (Arpan)

Dikkat ek hi: do log ek cheez ek saath badal rahe. Farak nateeje me:

```
@Version (Confluence)   clash pe ek HAARTA: reject, "reload karo", dobara padh ke bhejo
OT (Google Docs)        koi nahi haarta: dono ka badlaav JOD deta ("shabd khiska do")
```

Badlaav jud sakein (text: tu line 1, Ravi line 5) -> OT. Na jud sakein (stock 90 aur 80 dono sach nahi) -> ek haare,
dobara padh ke bheje -> version check.

## Prod wala trap

```java
catch (ObjectOptimisticLockingFailureException e) {
    p.setVersion(p.getVersion() + 1);   // version badha ke dobara save
    repo.save(p);
}
```

Wahi lost update wapas. Version chaabi nahi, "maine sabse nayi row padhi" ka saboot. 409 pe version mat chhedo:
dobara padho -> naye data pe hisaab -> phir bhejo.

## Tera project

usercrud `Product @Version` (22-Aug): `?version=0` -> OK (v -> 1) · dobara `?version=0` -> 409 STALE.

## Interview line

```
Optimistic locking: row me version counter, update WHERE id=? AND version=? chalta; beech me
kisi ne badla to 0 rows -> OptimisticLockException, lost update ruk jaata. Lock nahi lagta,
isliye kam clash wale kaam me tez; conflict pe dobara padh ke retry. Zyada clash ho to
pessimistic (SELECT FOR UPDATE). Optimistic locking aur OT dono concurrent edit ke hal: jahan
badlaav jud sakein wahan OT, jahan nahi wahan version check.
```
