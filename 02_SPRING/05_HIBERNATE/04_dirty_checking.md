# Dirty Checking

## Kahani: PA ki photocopy

PA lekhak ki file laata aur chupke se ek photocopy apne paas rakhta (snapshot). Arpan file me naam kaat ke "Arpan"
likh deta (`u.setName("Arpan")`), "save karo" nahi bolta. Shaam ko office band hone se pehle (commit) PA file ko
photocopy se milata, naam badla dikha to khud record room jaake `UPDATE`. Ye milaan = **dirty checking**.

Trap: PA pehle hi ghar chala gaya (method pe `@Transactional` nahi) to file pe kuch bhi likho, milane wala koi nahi,
badlaav kho jaata. Entity **detached**.

## Tasveer

```
findById  -> file + photocopy (name = Suresh)
setName   -> file me "Arpan"
commit    -> PA milata: Suresh != Arpan -> DIRTY
          -> UPDATE users SET name='Arpan' WHERE id=1
```

## 4 khaas baat

1. Sirf **managed** (persistent) entity pe. 4 haalat: transient (`new User()`, PA ko pata nahi) · persistent (PA
   track kar raha) · detached (PA ghar gaya) · removed (hatane ka mark).
2. Spring Data `repo.save()`: nayi entity pe `persist`, purani pe `merge`. `merge` nayi managed copy lautata, isliye
   hamesha `user = repo.save(user)`.
3. Default UPDATE har column likhta, `@DynamicUpdate` = sirf badle column.
4. Ek jad, teen dikkat = "session band": LazyInit (parchi khuli nahi) · detached (badlaav kho gaya) · opt-lock
   (purani version, aage aayega).

## Flush kya hai

PA ke paas din bhar ke badlaav jama hote (Java memory me). Flush = PA record room jaake SQL chala de.

```
flush  = SQL DB me chala diya, par transaction abhi khuli   -> pencil se likha (rollback pe mit jaayega)
commit = pakka                                               -> pen se likha
```

Commit pe Hibernate pehle khud flush karta, phir commit. Flush teen jagah: commit pe · `em.flush()` · usi session me
agli query se pehle. `save()` khud flush nahi karta, beech me hi DB tak bhejna ho (ID chahiye, UNIQUE galti yahin
pakadni) to `saveAndFlush()`.

## Bina save ke DB kyun badla (Unit of Work)

1. **save bhoolna:** order + 5 item + address badla, har ek pe save yaad rakhna padta, ek bhoola to aadha data badla.
   PA khud dekh leta kya-kya badla.
2. **Kram + bojh:** saare badlaav commit pe ek saath, sahi kram me (parent pehle, child baad, FK ke hisaab se).
3. **ORM ka maqsad:** entity ko normal Java object ki tarah use karo, DB tak pahunchana Hibernate ka kaam.

Trade-off: fayda = kam code, koi badlaav chhootta nahi. Nuksaan = chupi write + snapshot ka memory / CPU.

**`@Transactional` + `save()` dono:** koi dikkat nahi. Managed entity pe `save()` = `merge` jo kuch nahi karta, UPDATE
commit pe ek hi baar. **Phir bhi `save()` likho** (Arpan): Hibernate ke jaadu pe mat tiko, padhne wale ko saaf dikhe ki
yahan likha ja raha.

## Prod wala trap

```java
@Transactional
public User getProfile(Long id) {
    User u = repo.findById(id).get();
    u.setPassword(null);     // JSON me password hash na jaaye, isliye hataya
    return u;
}                            // commit pe DB me bhi password NULL -> user login nahi kar paata
```

Iraada sirf response se hatana tha, DB nahi. Normal Java me object badalne se DB nahi badalta, yahan badal jaata.
Hal: response ke liye DTO (`new UserDto(u.getName(), u.getEmail())`), entity ko haath mat lagao · ya `readOnly = true`.

## Tera project

usercrud `UserService.update()` `@Transactional`: entity load, setter se field badla, koi `save()` nahi, phir bhi commit
pe Hibernate ne khud UPDATE bheja.

## Interview line

```
Dirty checking: entity load hote hi Hibernate snapshot rakhta, commit / flush pe compare karke
badle field ka UPDATE khud bhejta (Unit of Work). Ye sirf managed entity pe; detached pe change
kho jaata, wahan merge / save. Main phir bhi save() likhta hoon taaki write saaf dikhe, aur read
path pe DTO / readOnly taaki entity galti se badle to DB me na jaaye.
```
