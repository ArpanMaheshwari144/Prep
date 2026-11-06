# First-Level Cache (L1)

## Kahani: PA ki almaari (N+1 wali)

Arpan lekhak #1 ki file maangta hai. PA record room jaata hai (DB query chalti hai), file laata hai, aur uski ek copy apni almaari me rakh leta hai.
Usi din Arpan dobara lekhak #1 maange, to PA record room nahi jaata, almaari se wahi file de deta hai. Koi query nahi chalti, aur `u1 == u2` true aata hai, kyunki dono
ek hi object hain. Shaam ko PA ghar chala jaata hai (session band ho jaata hai), aur almaari khaali ho jaati hai. Agle din nayi almaari se shuru hota hai.

## Tasveer

```
@Transactional shuru -> almaari khaali
findById(1)  -> almaari me? nahi -> SELECT ... -> almaari me rakha
findById(1)  -> almaari me? haan -> wahi object, NO query
method khatam -> almaari khaali
```

`@Transactional` shuru hote waqt almaari khaali hoti hai. Pehli baar `findById(1)` pe almaari me kuch nahi milta, to SELECT chalta hai aur object almaari me rakh diya jaata hai. Doosri baar almaari me mil jaata hai, to wahi object milta hai aur koi query nahi chalti. Method khatam hote hi almaari phir khaali.

## 4 khaas baat

1. Almaari ek session ki hoti hai, yaani ek `@Transactional` ki. Doosri request ki almaari alag hoti hai. Ye automatic hai, aur ise band nahi kar sakte.
2. Ye sirf **ID se maangne** pe query bachati hai (`findById`). `findByEmail`, `findAll` ya koi bhi `@Query` har baar DB jaati hai. Bas itna hai ki
   agar lauti hui entity pehle se almaari me ho, to tujhe wahi purana object milta hai.
3. **Purana data ka khatra:** beech me kisi aur ne DB me row badal di, to almaari wala object purana reh jaata hai. Hal: `em.refresh(u)` se ek file
   record room se dobara laao, ya `em.clear()` se poori almaari khaali kar do.
4. Teen cache alag-alag hain. **L1** PA ki apni almaari hai, Hibernate ki, aur automatic hai. **L2** poore office ki saanjhi almaari hai, ye bhi Hibernate ki,
   par ise khud lagana padta hai (EhCache ya Redis se). **@Cacheable** Spring ka hai, aur ye method ka result rakhta hai, entity nahi.

## Prod wala trap

```java
@Transactional
public void waitForPayment(Long id) {
    while (repo.findById(id).get().getStatus() != PAID) {   // har baar almaari se -> purana PENDING
        Thread.sleep(1000);                                  // webhook ne DB me PAID kiya, yahan kabhi nahi dikhega
    }
}
```

Loop har baar `findById` almaari se padhta hai, isliye use purana PENDING hi milta hai, chahe webhook ne DB me PAID kar diya ho. Isliye ye loop kabhi nahi rukta. Hal: loop ko transaction ke bahar rakho, ya `em.refresh()` karo. Aur lambi transaction me intezaar karna wahi HikariCP
wali dikkat hai, kyunki tab tak connection pakda rehta hai.

## Tera project

usercrud me ek `@Transactional` method ke andar `getById(1)` do baar bulaya. DB query sirf ek baar chali, aur dono baar same object mila.
