# volatile — Visibility (aur Ordering)

**Kahani:** ek background worker `while (running)` pe ghoom raha hai. Shutdown pe doosra thread `running = false` karta hai, par worker rukta hi nahi. Kaaran ye hai ki worker apni purani copy padh raha hai: JIT ne flag ka read loop se bahar nikaal diya, to worker ke liye wo hamesha `true` hi hai. `volatile` lagao to har read asli memory se hota hai, aur badlaav dikh jaata hai.

**Tera hands-on:** usercrud [`VolatileDemoController`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/VolatileDemoController.java) (27-Aug, `/volatile/start` + `/volatile/stop`). Spring me bina volatile ke bhi worker ruk gaya, par 4-Oct plain Java me bina volatile HANG hua aur volatile wala turant ruka. Matlab bina volatile kuch pakka nahi, kabhi chalega, kabhi latkega.

**Khaas baat:**
1. `volatile` = jo likha gaya wo har thread ko dikhe (visibility). Atomicity nahi deta: `volatile int count; count++` me race ab bhi rahegi, kyunki padhna, jodna aur likhna teen alag kadam hain.
2. Teeno ka farak:

```
volatile      -> sirf dikhna            -> FLAG (ek likhe, baaki padhein)
AtomicInteger -> dikhna + atomic (CAS)  -> COUNTER
synchronized  -> dikhna + atomic (lock) -> 2+ line ka block ek saath
```

3. Doosra kaam ORDERING hai. Tiffin wali misaal: pehle tiffin locker me rakho, phir flag pe "READY" likho. `volatile` flag ye pakka karta hai ki READY dikha to tiffin bhi andar mil jaayega (happens-before). Isi wajah se DCL singleton me `instance` pe `volatile` zaroori hai, warna doosre thread ko aadha bana object mil sakta hai.
   ```java
   private static volatile Singleton instance;
   ```
4. Har jagah `volatile` mat lagao, har read mehnga ho jaata hai. Sirf wahan lagao jahan doosra thread wo variable padhta hai.

**Prod wala trap:** tere 27-Aug ke usercrud demo me bina `volatile` ke bhi worker ruk gaya tha, aur plain Java me hang ho gaya. Matlab "mere laptop pe chal raha hai" ka koi matlab nahi. Server ki JVM pe ye bug phoot sakta hai, aur shutdown pe app band hi nahi hoga. Is bug ko test pakad nahi paata, isliye flag pe `volatile` hamesha likho.

```
Interview: "volatile guarantees visibility and ordering - every thread sees the latest write, and writes
before it can't be reordered after it. It does NOT make count++ atomic; for that I use
AtomicInteger or synchronized. Typical use: a stop flag, or the instance field in DCL singleton.
Without it, visibility isn't guaranteed - it may work on one JVM and hang on another."
```
