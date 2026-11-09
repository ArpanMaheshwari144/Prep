# Dirty Checking

## Scene: usercrud ka `UserService.update()`

```java
@Transactional
public void update(Long id, String name) {
    User u = repo.findById(id).get();   // name = Suresh
    u.setName("Arpan");                 // save() kahin nahi likha
}
```

`save()` likha hi nahi, phir bhi commit pe Hibernate ne khud UPDATE bhej diya aur DB badal gaya (default me UPDATE har column likhta hai,
`@DynamicUpdate` lagao to sirf badle hue).

Kaise hua: `findById` pe Hibernate entity deta hai aur chupke se uski ek **photocopy (snapshot)** apne paas rakh leta hai. Commit se pehle wo
entity ko photocopy se milata hai: Suresh tha, ab Arpan hai, to entity "dirty" hai, aur wo khud UPDATE bhej deta hai. Isi milaan ko
**dirty checking** kehte hain.

```
findById  -> entity + photocopy (name = Suresh)
setName   -> entity me "Arpan"
commit    -> milaya: Suresh != Arpan -> DIRTY -> UPDATE users SET name='Arpan' WHERE id=1
```

## Kab chalta hai

Sirf jab entity **managed** ho, yaani session khula ho. LAZY wali file me jo session band hone pe parchi nahi khuli thi, yahan wahi jad hai.
`@Transactional` nahi hai to entity **detached** ho jaati hai, setter lagao par milane wala koi nahi, aur badlaav kho jaata hai.

## Do yaad ki cheez

- `repo.save()` nayi entity pe `persist` karta hai aur purani pe `merge`. `merge` ek nayi managed copy lautata hai, isliye
  `user = repo.save(user)` likho.
- **Flush aur commit alag hain.** Flush matlab SQL DB me chal gaya par transaction abhi khuli hai, yaani pencil se likha, rollback pe mit
  jaayega. Commit matlab pen se likha, pakka. Beech me hi ID chahiye ya UNIQUE wali galti yahin pakadni ho to `saveAndFlush()`.

**`@Transactional` ke saath bhi `save()` likho** (Arpan ka niyam): managed entity pe `save()` kuch extra nahi karta, UPDATE ek hi baar
jaata hai. Par code padhne wale ko saaf dikhta hai ki yahan DB me likha ja raha hai.

## Prod wala trap

```java
@Transactional
public User getProfile(Long id) {
    User u = repo.findById(id).get();
    u.setPassword(null);     // JSON me hash na jaaye, isliye hataya
    return u;
}                            // commit pe DB me bhi password NULL -> user login nahi kar paata
```

Iraada sirf response se password hatana tha, par entity managed thi, to dirty checking ne DB me bhi NULL likh diya. Hal: response ke liye
DTO banao aur entity ko haath mat lagao, ya method pe `@Transactional(readOnly = true)` lagao, jisme Hibernate flush hi nahi karta.
