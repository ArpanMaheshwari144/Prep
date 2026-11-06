# Dirty Checking

## Kahani: PA ki photocopy

PA lekhak ki file laata hai aur chupke se uski ek photocopy apne paas rakh leta hai (isi ko snapshot kehte hain). Arpan file me naam kaat ke "Arpan"
likh deta hai (`u.setName("Arpan")`), aur "save karo" bolta bhi nahi. Shaam ko office band hone se pehle (commit pe) PA file ko
photocopy se milata hai. Naam badla hua dikha, to wo khud record room jaake `UPDATE` kar deta hai. Yahi milaan **dirty checking** hai.

Trap ye hai: agar PA pehle hi ghar chala gaya (method pe `@Transactional` nahi hai), to file pe kuch bhi likho, milane wala koi nahi hai, aur
badlaav kho jaata hai. Us haalat me entity **detached** kehlati hai.

## Tasveer

```
findById  -> file + photocopy (name = Suresh)
setName   -> file me "Arpan"
commit    -> PA milata: Suresh != Arpan -> DIRTY
          -> UPDATE users SET name='Arpan' WHERE id=1
```

`findById` pe file aur uski photocopy dono bante hain, jisme naam Suresh hai. `setName` se file me "Arpan" ho gaya. Commit pe PA milata hai ki Suresh aur Arpan alag hain, matlab entity DIRTY hai, isliye wo khud `UPDATE` bhej deta hai.

## 4 khaas baat

1. Ye sirf **managed** (persistent) entity pe kaam karta hai. Entity ki 4 haalat hoti hain. Transient matlab `new User()` banaya, PA ko iska pata hi nahi. Persistent matlab PA
   ise track kar raha hai. Detached matlab PA ghar chala gaya. Removed matlab ise hatane ka mark lag chuka hai.
2. Spring Data ka `repo.save()` nayi entity pe `persist` karta hai aur purani pe `merge`. `merge` ek nayi managed copy lautata hai, isliye
   hamesha `user = repo.save(user)` likho, taaki tere haath me managed copy rahe.
3. Default me UPDATE har column likhta hai. `@DynamicUpdate` lagao to sirf badle hue column likhe jaate hain.
4. Ek hi jad se teen dikkat aati hain, aur wo jad hai "session band". Pehli LazyInit, kyunki parchi khuli nahi. Doosri detached, kyunki badlaav kho gaya. Teesri opt-lock,
   kyunki purani version pe likha (ye aage aayega).

## Flush kya hai

PA ke paas din bhar ke badlaav Java memory me jama hote rehte hain. Flush matlab PA record room jaake wo SQL chala de.

```
flush  = SQL DB me chala diya, par transaction abhi khuli   -> pencil se likha (rollback pe mit jaayega)
commit = pakka                                               -> pen se likha
```

Flush pencil se likhne jaisa hai: SQL DB me chal gaya, par transaction abhi khuli hai, isliye rollback hua to mit jaayega. Commit pen se likhne jaisa hai, wo pakka hai.

Commit pe Hibernate pehle khud flush karta hai, phir commit. Flush teen jagah hota hai: commit pe, `em.flush()` bulane pe, aur usi session me
agli query chalne se pehle. `save()` khud flush nahi karta. Agar beech me hi DB tak bhejna ho, jaise ID chahiye ya UNIQUE wali galti yahin
pakadni hai, to `saveAndFlush()` use karo.

## Bina save ke DB kyun badla (Unit of Work)

1. **save bhoolne ka dar:** maan lo order, uske 5 item aur address teeno badle. Har ek pe save yaad rakhna padta, aur ek bhi bhoole to aadha data badalta.
   Isliye PA khud dekh leta hai ki kya-kya badla.
2. **Kram aur bojh:** saare badlaav commit pe ek saath jaate hain, aur sahi kram me jaate hain, yaani FK ke hisaab se parent pehle aur child baad me.
3. **ORM ka maqsad:** tu entity ko ek normal Java object ki tarah use kare, aur DB tak pahunchana Hibernate ka kaam rahe.

Trade-off: fayda ye hai ki code kam likhna padta hai aur koi badlaav chhootta nahi. Nuksaan ye hai ki chupi write ho jaati hai, aur snapshot rakhne me memory aur CPU lagta hai.

**`@Transactional` aur `save()` dono saath lagaye to:** koi dikkat nahi hai. Managed entity pe `save()` matlab `merge`, jo kuch nahi karta, aur UPDATE
commit pe ek hi baar jaata hai. **Phir bhi `save()` likho** (ye Arpan ka niyam hai): Hibernate ke jaadu pe mat tiko, taaki code padhne wale ko saaf dikhe ki
yahan DB me likha ja raha hai.

## Prod wala trap

```java
@Transactional
public User getProfile(Long id) {
    User u = repo.findById(id).get();
    u.setPassword(null);     // JSON me password hash na jaaye, isliye hataya
    return u;
}                            // commit pe DB me bhi password NULL -> user login nahi kar paata
```

Iraada sirf response se password hatana tha, DB se nahi. Normal Java me object badalne se DB nahi badalta, par yahan entity managed hai, isliye commit pe DB me bhi password NULL ho jaata hai aur user login nahi kar paata.
Hal: response ke liye DTO banao (`new UserDto(u.getName(), u.getEmail())`) aur entity ko haath mat lagao, ya method pe `readOnly = true` lagao.

## Tera project

usercrud ka `UserService.update()` `@Transactional` hai. Usme entity load hoti hai, setter se field badalta hai, aur koi `save()` nahi likha, phir bhi commit
pe Hibernate ne khud UPDATE bhej diya.
