# First-Level Cache (L1)

## Koi poochhe "first-level cache kya hai" to aise bolo

```
Har transaction (session) ki apni almaari: jo entity aayi wo almaari me.
Usi transaction me dobara ID se maangi to DB nahi jaata, wahi object almaari se.
Transaction khatam to almaari khaali, apne aap chalti hai.
L2 = poore app ki saanjhi almaari (khud lagani padti); @Cacheable = Spring ka, method ka result.
```

## Scene: usercrud, ek `@Transactional` me do baar `getById(1)`

```java
User u1 = repo.findById(1L).get();   // SELECT ... chala
User u2 = repo.findById(1L).get();   // koi query nahi
System.out.println(u1 == u2);        // true
```

Logs me select sirf ek baar dikha, aur dono baar **wahi object** mila (`==` true).

Kaise hua: ye wahi **almaari** hai jo dirty checking me photocopy rakh rahi thi. Session jo entity laata hai, use almaari me rakh leta hai.
Usi transaction me dobara ID se maanga to DB nahi jaata, almaari se de deta hai. Transaction khatam hote hi almaari khaali, aur agli request
ki almaari alag.

```
@Transactional shuru -> almaari khaali
findById(1)  -> almaari me? nahi -> SELECT -> almaari me rakha
findById(1)  -> almaari me? haan -> wahi object, NO query
method khatam -> almaari khaali
```

## Yaad ki cheez

- Query sirf **ID se** maangne pe bachti hai (`findById`). `findByEmail`, `findAll` ya `@Query` har baar DB jaati hai. Bas lauti hui entity
  pehle se almaari me ho to tujhe wahi purana object milta hai.
- Ye automatic hai, band nahi kar sakte.
- Teen cache alag hain:
  - **L1**: ek session ki almaari, Hibernate ki, apne aap chalti.
  - **L2**: poore app ki saanjhi almaari, Hibernate ki, khud lagani padti hai (EhCache / Redis).
  - **`@Cacheable`**: Spring ka hai, method ka result rakhta hai, entity nahi.

## Prod wala trap

```java
@Transactional
public void waitForPayment(Long id) {
    while (repo.findById(id).get().getStatus() != PAID) {   // har baar almaari se -> PENDING
        Thread.sleep(1000);                                  // webhook ne DB me PAID kiya, yahan kabhi nahi dikhega
    }
}
```

Chala ke dekha (Boot 4 + Hibernate 7, 1.5 sec pe webhook DB me PAID likhta):

```
>> @Transactional ke andar loop
select ...                     <- query sirf pehli baar
   sec 1: PENDING   sec 2: PENDING
   [webhook] DB me PAID likh diya
   sec 3: PENDING   sec 4: PENDING   -> loop kabhi nahi rukta
>> wahi loop, par em.refresh()
select ... (har baar)
   sec 2: PENDING
   [webhook] DB me PAID likh diya
   sec 3: PAID                       -> ruk gaya
```

Webhook ne DB me PAID kar diya, par loop har baar almaari se padh raha hai, to use PENDING hi dikhta hai aur loop kabhi nahi rukta. Hal: loop
ko transaction ke bahar rakho, ya `em.refresh(entity)` se DB se dobara lao. Upar se lambi transaction me intezaar = connection pakda raha,
wahi Hikari wali dikkat.
