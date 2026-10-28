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


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## ★ PERFORMANCE COST — har cheez volatile kyun NAHI (8-Sep, Arpan-Q)
> Sawaal: "volatile baar-baar main-memory se padhega to slow nahi hoga?" -> HAAN, cost hai.
```
Normal var -> CPU apni FAST cache se padhta (nano-sec).
volatile   -> (1) cache-fayda gaya (har read/write RAM tak) + (2) CPU MEMORY FENCE/BARRIER lagata
              -> har access mehnga + kuch JIT optimizations block (reordering rok deta).
```
- Isiliye har cheez volatile NAHI -> **selective**: sirf jahan cross-thread visibility chahiye.
- Hot single-thread variable ko volatile karna = **faltu waste** (koi doosra thread padh hi nahi raha).
- **TRADE-OFF: correctness (visibility) vs speed.** Flag ke liye cost chhota + zaroori; warna mat lagao.
- (Ye "fence" hi agle section JMM/happens-before ka core hai — volatile sirf cache nahi, REORDERING bhi rokta.)

## ★ JMM + happens-before — volatile ka DUSRA kaam: ORDERING (8-Sep)
> ★ volatile ke 2 kaam: (1) VISIBILITY (upar) + (2) ORDERING (ye). Atomicity phir bhi NAHI.

### Tiffin-locker analogy (rattna nahi — ye picture)
```
Person A = khaana banata      | Person B = lene aata
Shared locker = main memory   | bahar ek FLAG = "READY / NOT READY"
A: (1) tiffin pack -> locker me rakho (data=42)   (2) flag "READY" (ready=true)
B: flag "READY" dikhe -> locker khol ke tiffin le.
```
**PROBLEM = REORDERING:** A ka CPU tez banne ko order badal sakta -> pehle flag "READY", tiffin baad me.
  B ne READY dekha -> locker khola -> tiffin hai hi nahi/aadha -> gadbad.
  (Akela hota to order-badalna theek. **Doosra ORDER pe depend kare -> tootta.**)

**FIX = flag `volatile` = SEALED promise:** "flag READY tabhi ho sakta jab **usse pehle ka SAB kaam** (tiffin locker me + visible) ho chuka; is flip ke aar-paar order badla NAHI jaa sakta."
  -> B jab READY dekha -> tiffin GUARANTEED poora + visible.

### JMM / happens-before (naam, par picture upar)
- **JMM (Java Memory Model)** = official rulebook: KAB ek thread ka likha doosre ko GUARANTEED dikhega + reordering ki limits.
- **happens-before** = "A happens-before B" -> A se pehle ka SAB kaam B ko dikhta = flag-stamp se pehle ka sab, dekhne-wale ko milta.
- **volatile WRITE -> baad ka volatile READ = happens-before edge.** Isliye `data=42; ready(volatile)=true;` me B jab ready=true dekha -> data=42 bhi guaranteed (reorder nahi). volatile "apne se pehle ka SAB publish/seal kar deta" — sirf ek var nahi.
- Doosre happens-before edges (same guarantee): lock RELEASE -> agla lock ACQUIRE (synchronized) · thread.start() -> us thread ka kaam · thread ka kaam -> thread.join() return.

### DCL Singleton — volatile ka killer example (ghar + nameplate)
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
`instance = new Singleton()` = 3 kaam: allocate(plot) -> construct(ghar bano) -> assign(nameplate lagao).
  Reorder -> nameplate PEHLE (`instance!=null`) par ghar AADHA-bana -> doosra thread nameplate dekh ke ghusa -> adhoora object -> crash.
  **volatile reorder rokta -> safe publication.** Isliye DCL me `volatile` compulsory.

### ★ 3-LINE YAAD
```
volatile = VISIBILITY (cache->RAM, latest dikhe) + ORDERING (reorder rok, "pehle ka sab seal" = happens-before)
volatile != ATOMICITY (count++ race rahega -> AtomicInteger/synchronized)
DCL singleton -> instance ko volatile (warna partially-constructed object dikh sakta)
```
