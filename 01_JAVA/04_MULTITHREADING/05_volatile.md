# volatile: Visibility (aur Ordering)

**Kahani:** ek background worker `while (running)` pe ghoom raha hai. Shutdown pe doosra thread `running = false` karta hai, par worker rukta hi nahi. Kaaran ye hai ki worker apni purani copy padh raha hai. JIT ne flag ka read loop se bahar nikaal diya, to worker ke liye flag hamesha `true` hi hai. `volatile` lagao to har read asli memory se hota hai, aur badlaav worker ko dikh jaata hai.

**Tera hands-on:** usercrud [`VolatileDemoController`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/VolatileDemoController.java) (27-Aug, `/volatile/start` aur `/volatile/stop`). Spring me bina volatile ke bhi worker ruk gaya tha. Par 4-Oct ko plain Java me bina volatile wala worker HANG ho gaya, aur volatile wala turant ruk gaya. Matlab bina volatile ke kuch pakka nahi hai, kabhi chalega, kabhi latak jaayega.

**Khaas baat:**

1. `volatile` ka kaam hai ki jo likha gaya wo har thread ko dikhe (visibility). Ye atomicity nahi deta. `volatile int count; count++` me race ab bhi rahegi, kyunki padhna, jodna aur likhna teen alag kadam hain.

2. Teeno ka farak ye hai. `volatile` sirf dikhne ki guarantee deta hai, isliye wo FLAG ke liye hai, jahan ek thread likhta hai aur baaki padhte hain. `AtomicInteger` dikhna bhi deta hai aur atomic bhi hai (CAS se), isliye wo COUNTER ke liye hai. `synchronized` dikhna bhi deta hai aur atomic bhi hai (lock se), isliye wo tab lagta hai jab 2 ya zyada line ka block ek saath chalna chahiye.

```
volatile      -> sirf dikhna            -> FLAG (ek likhe, baaki padhein)
AtomicInteger -> dikhna + atomic (CAS)  -> COUNTER
synchronized  -> dikhna + atomic (lock) -> 2+ line ka block ek saath
```

3. `volatile` ka doosra kaam ORDERING hai. Tiffin wali misaal le: pehle tiffin locker me rakho, phir flag pe "READY" likho. `volatile` flag ye pakka karta hai ki agar READY dikha, to tiffin bhi andar mil jaayega (isse happens-before kehte hain). Isi wajah se DCL singleton me `instance` pe `volatile` zaroori hai, warna doosre thread ko aadha bana object mil sakta hai.

   ```java
   private static volatile Singleton instance;
   ```

4. Har jagah `volatile` mat lagao, kyunki har read mehnga ho jaata hai. Sirf wahan lagao jahan doosra thread wo variable padhta hai.

**Prod wala trap:** tere 27-Aug ke usercrud demo me bina `volatile` ke bhi worker ruk gaya tha, aur plain Java me wahi cheez hang ho gayi. Matlab "mere laptop pe chal raha hai" ka koi matlab nahi hai. Server ki JVM pe ye bug phoot sakta hai, aur shutdown pe app band hi nahi hoga. Is bug ko test pakad nahi paata, isliye flag pe `volatile` hamesha likho.

```
Interview: "volatile guarantees visibility and ordering - every thread sees the latest write, and writes
before it can't be reordered after it. It does NOT make count++ atomic; for that I use
AtomicInteger or synchronized. Typical use: a stop flag, or the instance field in DCL singleton.
Without it, visibility isn't guaranteed - it may work on one JVM and hang on another."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## ★ PERFORMANCE COST: har cheez volatile kyun NAHI (8-Sep, Arpan ka sawaal)

Sawaal ye tha: "volatile baar-baar main memory se padhega, to slow nahi hoga?" Jawab hai haan, iski keemat hai.

Normal variable ko CPU apni fast cache se padhta hai, jo nano-second ka kaam hai. `volatile` me do cheezein hoti hain. Pehli, cache ka fayda chala jaata hai, kyunki har read aur write RAM tak jaata hai. Doosri, CPU ek memory fence (barrier) lagata hai. Isse har access mehnga ho jaata hai, aur JIT ki kuch optimizations bhi ruk jaati hain, kyunki volatile reordering rok deta hai.

- Isiliye har cheez volatile nahi karte, selective rehte hain. Sirf wahan lagate hain jahan ek thread ka likha doosre thread ko dikhna chahiye.
- Jo hot variable sirf ek hi thread use karta hai, use volatile karna faltu ka waste hai, kyunki koi doosra thread use padh hi nahi raha.
- **Trade-off correctness (visibility) aur speed ke beech hai.** Flag ke liye cost chhoti hai aur volatile zaroori hai. Baaki jagah mat lagao.
- Yahi "fence" agle section ke JMM aur happens-before ka core hai. Volatile sirf cache ki dikkat nahi, REORDERING bhi rokta hai.

## ★ JMM + happens-before: volatile ka DOOSRA kaam, ORDERING (8-Sep)

★ volatile ke 2 kaam hain. Pehla VISIBILITY, jo upar dekha. Doosra ORDERING, jo yahan hai. Atomicity phir bhi nahi deta.

### Tiffin-locker wali tasveer (rattna nahi, ye picture yaad rakh)

```
Person A = khaana banata      | Person B = lene aata
Shared locker = main memory   | bahar ek FLAG = "READY / NOT READY"
A: (1) tiffin pack -> locker me rakho (data=42)   (2) flag "READY" (ready=true)
B: flag "READY" dikhe -> locker khol ke tiffin le.
```

**Dikkat REORDERING hai.** A ka CPU tez chalne ke liye kaam ka order badal sakta hai, yaani pehle flag "READY" kar de aur tiffin baad me rakhe. B ne READY dekha, locker khola, aur tiffin hai hi nahi ya aadha hai. Gadbad ho gayi. Agar A akela hota to order badalna theek tha. Dikkat tab hoti hai jab doosra thread us order pe depend karta hai.

**Ilaaj ye hai ki flag ko `volatile` bana do.** Ye ek sealed promise ban jaata hai: flag READY tabhi ho sakta hai jab usse pehle ka saara kaam (tiffin locker me aur sabko visible) ho chuka ho. Is flag ke badalne ke aar-paar order badla nahi ja sakta. Isliye jab B READY dekhta hai, to tiffin guaranteed poora aur visible hai.

### JMM aur happens-before (ye naam hain, picture upar wali hai)

- **JMM (Java Memory Model)** Java ki official rulebook hai. Ye batati hai ki ek thread ka likha doosre thread ko kab guaranteed dikhega, aur reordering ki limit kya hai.
- **happens-before** ka matlab hai ki agar "A happens-before B", to A se pehle ka saara kaam B ko dikhega. Tiffin ki bhasha me, flag pe stamp lagne se pehle ka sab kuch dekhne wale ko milta hai.
- **volatile WRITE aur uske baad ka volatile READ ke beech happens-before ka rishta banta hai.** Isliye `data=42; ready(volatile)=true;` me jab B ne `ready=true` dekha, to `data=42` bhi guaranteed dikhega, kyunki reorder nahi hua. Volatile sirf ek variable nahi, apne se pehle ka saara kaam publish aur seal kar deta hai.
- Aur bhi happens-before rishte hain jo yahi guarantee dete hain. Lock RELEASE ke baad agla lock ACQUIRE (synchronized me). `thread.start()` ke baad us thread ka kaam. Aur thread ka kaam khatam hone ke baad `thread.join()` ka return.

### DCL Singleton: volatile ki sabse badi misaal (ghar aur nameplate)

```java
class Singleton {
    private static volatile Singleton instance;   // volatile ZAROORI
    static Singleton get() {
        if (instance == null) {                    // 1st check (no lock)
            synchronized (Singleton.class) {
                if (instance == null)              // 2nd check (locked)
                    instance = new Singleton();
            }
        }
        return instance;
    }
}
```

`instance = new Singleton()` asal me 3 kaam hai. Pehle memory allocate hoti hai (plot liya), phir constructor chalta hai (ghar bana), phir variable me assign hota hai (nameplate lagi).

Agar reorder ho gaya, to nameplate pehle lag jaayegi, yaani `instance != null` ho jaayega, jabki ghar abhi aadha bana hai. Doosra thread nameplate dekh ke andar ghusa, use adhoora object mila, aur crash.

**volatile ye reorder rok deta hai, isliye object safe tareeke se publish hota hai.** Isliye DCL me `volatile` compulsory hai.

### ★ 3 line me yaad rakh

volatile do cheez deta hai: VISIBILITY (cache ki jagah RAM se, latest value dikhe) aur ORDERING (reorder nahi hoga, pehle ka sab seal ho jaata hai, yahi happens-before hai).

volatile ATOMICITY nahi deta. `count++` me race rahegi, uske liye AtomicInteger ya synchronized chahiye.

DCL singleton me `instance` ko volatile banao, warna doosre thread ko aadha bana object dikh sakta hai.

## Visualization: CPU Cache ki dikkat

```
                   MAIN MEMORY (RAM)
                   running = true
                          ↑
                          │
        ┌─────────────────┴─────────────────┐
        ▼                                   ▼
   Thread 1                             Thread 2
   L1 Cache: running = true             L1 Cache: running = true

  Thread 2 ne main memory mein false kiya:
   MAIN: running = false
   T1 cache: running = true  (purana!)
   T2 cache: running = false

  Thread 1 ko PATA HI NAHI CHALA, loop chalta raha!
```

## synchronized vs volatile vs AtomicInteger

- **Visibility:** teeno dete hain, volatile bhi, synchronized bhi, AtomicInteger bhi.
- **Atomicity:** volatile nahi deta. synchronized aur AtomicInteger dono dete hain.
- **Lock lagta hai?** volatile me nahi. synchronized me haan. AtomicInteger me nahi, wo CAS se kaam karta hai jo hardware ki instruction hai.
- **Performance:** volatile sabse tez hai. synchronized sabse slow hai. AtomicInteger tez hai, aur counters ke liye best hai.
- **Kahan lagta hai:** volatile flag ke liye, jahan zyadatar padhna hota hai. synchronized critical sections ke liye. AtomicInteger counters aur numbers ke liye.

## ★ TRIO CONNECT: volatile, atomic aur synchronized ek jagah

`volatile` sirf VISIBILITY deta hai (cache ki jagah RAM). Ye flag ya boolean ke liye hai, sabse tez hai, na lock lagata hai na atomic hai.

`AtomicInteger` VISIBILITY aur ATOMICITY dono deta hai, CAS se. Ye counter ke liye hai, tez hai, aur lock nahi lagata (optimistic).

`synchronized` VISIBILITY aur ATOMICITY dono deta hai, lock se. Ye multi-line block ke liye hai, sabse slow hai, aur LOCK lagata hai (pessimistic).

### ★ 4-Oct: wahi machine, plain Java (Spring ke bina), is baar HANG hua

Pehle plain flag ko false kiya, aur 3 second baad bhi worker zinda tha, yaani flag ka badalna use dikha hi nahi. Phir volatile flag ko false kiya, to volatile wala worker turant ruk gaya.

```
1) plain flag = false kiya
   3 sec baad worker zinda? true          <- HANG, flag ka badalna dikha hi nahi
2) volatile flag = false kiya
   volatile worker RUKA, count=1701657563
   3 sec baad worker zinda? false         <- turant ruka
```
