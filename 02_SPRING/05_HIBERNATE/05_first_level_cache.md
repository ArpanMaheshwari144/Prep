# First-Level Cache (L1)

## Kahani: PA ki almaari (N+1 wali)

Arpan lekhak #1 ki file maangta, PA record room jaata (DB query), file laata, ek copy apni almaari me rakhta.
Usi din dobara lekhak #1 maange to PA record room nahi jaata, almaari se wahi file (koi query nahi, `u1 == u2` true,
ek hi object). Shaam ko PA ghar (session band) -> almaari khaali. Agle din nayi almaari.

## Tasveer

```
@Transactional shuru -> almaari khaali
findById(1)  -> almaari me? nahi -> SELECT ... -> almaari me rakha
findById(1)  -> almaari me? haan -> wahi object, NO query
method khatam -> almaari khaali
```

## 4 khaas baat

1. Almaari ek session (ek `@Transactional`) ki. Doosri request = alag almaari. Automatic, band nahi hoti.
2. Sirf **ID se maangne** pe bachati (`findById`). `findByEmail`, `findAll`, koi bhi `@Query` har baar DB jaati; bas
   lauti entity pehle se almaari me ho to wahi object milta.
3. **Purana data:** beech me kisi aur ne DB me row badli to almaari wala object purana. Hal: `em.refresh(u)` (ek file
   record room se dobara) ya `em.clear()` (poori almaari khaali).
4. Teen cache alag: **L1** = PA ki almaari (Hibernate, automatic) · **L2** = poore office ki saanjhi almaari
   (Hibernate, khud lagani, EhCache / Redis) · **@Cacheable** = Spring ka, method ka result, entity nahi.

## Prod wala trap

```java
@Transactional
public void waitForPayment(Long id) {
    while (repo.findById(id).get().getStatus() != PAID) {   // har baar almaari se -> purana PENDING
        Thread.sleep(1000);                                  // webhook ne DB me PAID kiya, yahan kabhi nahi dikhega
    }
}
```

Loop kabhi nahi rukta. Hal: loop transaction ke bahar, ya `em.refresh()`. Aur lambi transaction me intezaar = HikariCP
wali dikkat (connection pakda).

## Tera project

usercrud: ek `@Transactional` method me `getById(1)` do baar -> DB query sirf ek baar, dono baar same object.

## Interview line

```
First-level cache session ka hota hai: ek transaction me same ID dobara maango to DB nahi
jaata, wahi object milta. Automatic hai, band nahi hota, session ke saath khaali. Dhyaan: sirf
ID lookup bachta, JPQL har baar DB; aur beech me DB badla to purana data, wahan refresh / clear.
```
