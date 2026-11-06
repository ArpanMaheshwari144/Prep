# static keyword

## Scene: har account ko apna number

Bank me har naye account ko apna number milna chahiye: ACC1, ACC2, ACC3. Ek bande ne counter normal field me rakha, doosre ne
`static` me:

```java
class A1 {        int counter = 0; String id; A1() { counter++; id = "ACC" + counter; } }
class A2 { static int counter = 0; String id; A2() { counter++; id = "ACC" + counter; } }
```

Teen-teen account banaye, chala ke dekha:

```
A1:  ACC1 ACC1 ACC1
A2:  ACC1 ACC2 ACC3
```

Pehle wale me teeno ko ACC1 mila, kyunki normal field har object ka apna hota hai, aur har naya account apna counter 0 se
shuru karta hai. `static` field class ka hota hai: poori class me uski ek hi copy hoti hai jo saare object share karte hain,
isliye ginti aage badhti gayi.

```
A2 class ->  counter = 3   (ek hi copy)
               ▲    ▲    ▲
             acc1 acc2 acc3    (har object ka apna sirf id)
```

## Tera project

Insurance ke `UserServiceImpl` me `private static final String EMAIL_REGEX` hai, kyunki regex sab user ke liye ek hi hai, to
har object me alag copy rakhne ka koi matlab nahi. `Helper.checkExcelFormat()` bhi `static` hai, kyunki wo kisi object ki
cheez nahi padhta, bas file leke jawab deta hai. Isliye use `Helper.checkExcelFormat(file)` se seedha bulate hain, `new
Helper()` banaye bina, bilkul `Math.sqrt()` ki tarah.

## Khaas baat

`static` method me `this` nahi chalta, kyunki wo object ke bina chalta hai, to wahan "ye object" hai hi nahi.

`static` cheez class load hote hi ek baar banti hai, object banne se pehle. Isliye `new` ke waqt sabse pehle parent ka static
chalta hai, phir child ka static, aur uske baad hi constructor chalte hain.

## Prod wala trap

Upar wala `static int counter` ek hi JVM ke andar chalta hai. Production me app ke 3 server chal rahe hon to teeno ka apna
counter hoga, aur teeno ACC1 de denge. Isliye asli id DB sequence ya UUID se aati hai, static counter se nahi.

Doosra trap: `static Map` ya `static SimpleDateFormat` ko saari request ek saath chhooti hain, aur data mix ho jaata hai. Ye
wahi "shared field" wali galti hai jo StringBuilder wali file me dekhi.
