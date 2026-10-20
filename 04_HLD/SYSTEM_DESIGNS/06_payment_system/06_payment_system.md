# Payment System (UPI-style)

> User A se User B / merchant ko paisa, bharose ke saath. Misaal: Arpan ne kirana dukaan (merchant) ko Rs. 500 bheje.
> Dil do cheezein: paisa do baar na kate, aur paisa kabhi gum na ho. Finance HLD ka core, JP / GS ka favourite.
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card ek block: HAL (↳ agar = cross-Q) · KYUN · NAHI (doosra kyun nahi).
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-abf50228.svg)

---


## Shuru

```
FR      A se B / merchant ko bhejo (main: ek user se doosre tak). Dobara bhejne pe dobara na kate,
        fail ho to paisa wapas, har paisa traceable ho
        scope pehle poochho: P2P / merchant / refund / settlement / fraud? Refund scope me hai?
        Bahar ka PSP (Razorpay / Stripe) lagega ya khud? Audit / RBI ka hisaab chahiye?
        scope ke bahar: fraud engine, kai currency, chargeback
NFR     chaar mushkil hain, poori file inhi pe chalti hai:
        do baar na kate, 500 ki jagah 1000 na ho (idempotency)
        dono taraf ek saath badle (consistency)
        kuch gire to bhi paisa na khoye: kata par pahuncha nahi, ya beech me network gaya to atke nahi
        (failure handling)
        har paisa trace ho (ledger / audit)
NUMBERS ~10 crore user, aam din kuch sau payment / sec, festival / sale pe kuch hazaar / sec.
        Bheed badi dikkat nahi hai, sahi hona badi dikkat hai
        -> pehle sab-ya-kuch-nahi + do-baar-nahi + ledger
PAISA   Arpan aur merchant dono ka wallet hamare hi system me hai, Paytm / PhonePe wallet jaisa.
        Wallet se wallet = hamare DB ke andar, ek transaction. Poori file isi pe chalti hai
DB      SQL. Ek payment me paanch likhai hain: Arpan -500, merchant +500, ledger me 2 line, payment ka haal.
        BEGIN, paanchon likho, COMMIT. Beech me fail hua to ROLLBACK, ek bhi nahi bachti
        (SQL transaction, ACID)
TABLE   teen table hain: wallet (account_id, balance), ledger (isme sirf jodte hain), payment (key, haal)
DHYAAN  "NoSQL me ACID hota hi nahi" mat bolo, kai NoSQL ek row / document pe atomic hote hain. Sahi farak:
        debit + credit + ledger = kai row ek saath, ye SQL me seedha aur pakka hota hai
```

---

*ab tootega kahan? sabse pehle beech me, jab kaatne ke baad server gire*

## 1. Kirana ko 500 bheje, beech me server gira

Cue: EK TRANSACTION (WAL se poora / ulta)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c1_1.png?v=3" width="900">

<img src="img/c1_2.png?v=3" width="900">

<img src="img/c1_3.png?v=3" width="900">

</details>

```
HAL     BEGIN, Arpan -500, Kirana +500, COMMIT, ye sab ek hi transaction me. Ya dono honge ya koi nahi.
        COMMIT se pehle server gira, to DB apna log (WAL) dekh ke ulta kar deta hai
        ↳ agar ledger ka INSERT fail? -> ROLLBACK, Arpan ka -500 bhi ulta
          transaction ke beech fail hua, to paanchon me se ek bhi nahi bachta
        ↳ agar ROLLBACK ho gaya, to user ko kya dikhe?
          -> "nahi hua". Paisa jaisa tha waisa hai, wo dobara try kar sakta hai.
             User ko sirf "hua" ya "nahi hua" dikhta hai, beech ka kabhi nahi
        ↳ agar hisaab me farak aaya? -> turant alert jaata hai
          User ko "hua" ya "nahi hua" hi dikhe, beech ka nahi
        ↳ agar SQL DB ki jagah NoSQL lagate, to?
          -> kai row aur table pe ek saath sab ya kuch nahi chahiye, aur paisa turant sahi chahiye.
             "Thodi der baad sahi" (eventual) paise me nahi chalta
        ↳ agar network toota, consistency ya availability? -> sahi hona pehle
          Balance pakka nahi, to payment mana. Galat balance pe kaatna usse bhi bura hai.
          Dashboard aur report thode purane bhi chalte rahein to chalega
        ↳ agar A->B aur B->A ek saath, dono atke? -> taala pehle chhoti account id pe
          ye deadlock hai. Taala hamesha chhoti account id pe pehle lo. Poora 13_banking card 9 me hai
KYUN    DB ka apna log hota hai (WAL). COMMIT se pehle gira, to DB uth ke log dekhta hai, COMMIT nahi tha,
        to ulta kar deta hai aur 500 wapas
        COMMIT ke baad gira, to log me COMMIT hai, DB kaam poora karta hai, kuch nahi khota.
        Saare kate == saare jude, aadha kabhi nahi (chaar hui aur ek reh gayi = hisaab galat)
NAHI    do alag badlaav, ek ke baad ek -> GAYAB: Arpan -500 hua, phir crash, merchant +500 hua hi nahi
```

---

*ab ek payment theek, par do payment ek saath ek hi wallet pe?*

## 2. Wallet me 600, phone se 500 aur laptop se 500 ek saath

Cue: check + kaato EK atomic query

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c2_1.png?v=2" width="900">

<img src="img/c2_2.png?v=2" width="900">

<img src="img/c2_3.png?v=2" width="900">

</details>

```
HAL     check aur kaatna ek hi query me karo (atomic UPDATE). DB ek-ek karke chalata hai, jeetta sirf ek:
        SET balance = balance - 500 WHERE id = ? AND balance >= 500
        Phone jeeta, 600 se 100 bacha. Laptop ke liye 100 < 500, 0 row aayi, use "paisa kam" dikha
        ↳ agar WHERE wala check bhool gaye? -> DB ka CHECK (balance >= 0) pakadta
          query me WHERE likhna bhool gaye, to bhi DB ka CHECK pakad leta hai. Ye aakhri deewar hai
KYUN    "paisa hai?" aur "kaato" ek hi kadam me hota hai, DB ke andar. Kitni bhi machine hon, DB ek-ek karke
        chalata hai, jeetta ek. 0 row aayi = reject, user ko turant "paisa kam"
NAHI    pehle padho, phir kaato -> RACE: phone (dukaan 500) aur laptop (Swiggy 500), dono server ne 600 dekha,
        dono ne kaata, wallet -400
        idempotency key -> GALAT: ye do alag payment hain, do log ek balance pe. Key sirf ek user ka
        dobara bhejna pakadti hai (card 3)
```

---

*ab balance theek, par wahi payment network ne do baar bheji to?*

## 3. Metro me network gaya, app ne dobara bheja, 1000 kate

Cue: IDEMPOTENCY KEY (screen khulte)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c3_1.png?v=2" width="900">

<img src="img/c3_2.png?v=2" width="900">

<img src="img/c3_3.png?v=2" width="900">

</details>

```
HAL     payment screen khulte hi app ek key banata hai (key-555), retry pe wahi key jaati hai.
        Store me key-555 pehle se hai, to saved jawab wapas jaata hai, 500 hi kate.
        DB me key pe UNIQUE hai (idempotency key). Saath me Pay button pehli tap pe hi band ho jaata hai
        ↳ agar key wahi, par amount alag? -> request ka hash bhi rakho, farak = 422
          farak mila to 422 jaata hai
        ↳ agar key store se key hatani ho, to kab?
          -> sirf pakka fail hone pe (paisa hila hi nahi). DONE wali key ~24 ghante rakho
        ↳ agar sach me do payment, 100 + 100? -> do key, dono honge, 200
          do baar screen khuli = do key (key-555, key-888), dono honge, 200. Maqsad sirf network ke
          dobara bhejne ko do baar ginne se rokna hai
KYUN    ek key = ek payment. Wahi key dobara aayi to kaam nahi hota, saved jawab jaata hai.
        User ko ek hi payment dikhta hai. Nayi payment = nayi screen = nayi key
HAAL    key ka haal: IN_PROGRESS -> kaato + daalo -> DONE + jawab
NAHI    bina key -> DOUBLE: timeout hua, app ne dobara bheja, server ne do naye payment maane, 1000 kate
        key server banaye -> BEKAAR: dobara to app bhejta hai. App ki key hi dono request jodti hai,
        server ko pata hi nahi ki ye wahi wali hai
        sirf DB -> DHEEMA: Redis (SETNX) pehle aur tez pakadta hai
        sirf Redis -> KACHCHA: DB ka UNIQUE aakhri deewar hai
```

---

*ab andar sab theek, par bahar ka PSP dheema ho to?*

## 4. Bank se 2000 top-up, PSP dheema

Cue: timeout = PENDING, circuit breaker = abhi nahi

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c4_1.png?v=2" width="900">

<img src="img/c4_2.png?v=2" width="900">

<img src="img/c4_3.png?v=2" width="900">

</details>

```
HAL     thodi der (2-3 sec) me jawab nahi aaya, to chhodo aur PENDING likho (pata nahi kata ya nahi),
        baad me pata karo (timeout)
        5 fail ho gaye, to circuit OPEN: call jaati hi nahi, saaf "abhi nahi", paisa hila hi nahi
        (circuit breaker)
        ↳ agar circuit OPEN, phir kab khulega? -> 30 sec baad ek test call
          chali to circuit band, nahi to phir OPEN
        ↳ agar PSP hi poora band? -> doosra PSP. Par ek payment ek hi PSP pe
          ek payment hamesha ek hi PSP pe chalti hai
PSP     bahar ka paisa PSP (Razorpay / bank) se aata jaata hai: bank se wallet me (top-up),
        wallet se bank me (payout), ya merchant kisi doosre bank me ho
KYUN    call gayi aur jawab nahi aaya = PENDING, "fail" kabhi nahi. Call gayi hi nahi (circuit OPEN)
        = saaf "abhi nahi"
        PSP dheema hai to jaldi chhodo, warna saare thread atak jaate hain
NAHI    timeout lagao hi mat, wait karo -> ATKA: har thread PSP ke jawab tak ruka (PSP 30 sec),
        sab atke, system band
```

---

*ab call gayi aur jawab se pehle hum hi gir gaye to?*

## 5. PSP ko call kiya aur server gira: paisa gaya ya nahi?

Cue: PENDING pehle + WEBHOOK + RECONCILE

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c5_1.png?v=2" width="900">

<img src="img/c5_2.png?v=2" width="900">

<img src="img/c5_3.png?v=2" width="900">

</details>

```
HAL     call se PEHLE DB me PENDING likho. PSP baad me khud bataye (webhook), aur purane PENDING pe ek job
        PSP se haal poochhe. Dono me se jo bhi bataye, DB me SUCCESS
        ↳ agar webhook do baar aaya? -> SUCCESS sirf ek baar, doosra ignore
        ↳ agar webhook nakli? -> hastakshar jaancho (PSP secret), PENDING se milao
          hastakshar jaancho (PSP secret) aur apne PENDING se milao
        ↳ agar webhook aaya hi nahi? -> job purane PENDING ke liye PSP se poochhe
          job har kuch minute poochhti hai. Raat ko PSP ki file se poora milaan hota hai (reconciliation)
        ↳ agar job ko PSP ne jawab de diya, phir? -> hua = SUCCESS
          Pakka fail = FAILED, kuch wapas nahi karna (paisa hila nahi)
KYUN    "pata nahi" pehle likh diya, to server gire tab bhi tracking number bacha rehta hai. PSP bataye
        (webhook) ya hum poochhe (job). User ko PROCESSING dikhe, andha dobara kabhi nahi
NAHI    sirf webhook -> KHOTA: webhook kho sakta hai (der ho, ya hamara server band ho).
        Job chhoota hua pakadti hai
        sirf job -> DHEEMA: der lagti hai aur kharcha bhi (PSP ki limit). Isliye dono
```

---

*ab merchant doosre bank me hai, ek transaction ban hi nahi sakti*

## 6. Merchant ka account doosre bank me, ek transaction nahi chalta

Cue: SAGA (step-step, fail pe ulta)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c6_1.png?v=2" width="900">

<img src="img/c6_2.png?v=2" width="900">

<img src="img/c6_3.png?v=2" width="900">

</details>

```
HAL     SAGA: step-step chalo, har kadam saga table me likho (DEBITED, PAYOUT). Bank B ka payout fail hua,
        to pichhle kadam ka ulta kaam karo: wallet me 500 wapas (saga, compensate)
        ↳ agar SAGA ke beech service giri? -> uth ke saga table dekhe, wahin se aage
          wahin se aage badhe ya ulta kare
        ↳ agar ulta kaam bhi fail? -> retry + alert, phir manual queue
          Paisa beech me na atke
KYUN    alag system me har step likha hua hai. Aage nahi badh sake to ulta karo. User ko PROCESSING dikhe,
        phir SUCCESS ya paisa wapas. Hisaab saaf
PEHLE   sabse bada sawaal, shuru me poochho: dono account ek hi system / bank me hain ya alag?
        Poora design isse badalta hai. Ek DB = ek transaction (ACID) kaafi, alag bank = SAGA ka khel
NAHI    ek transaction me dono -> NAMUMKIN: do alag system hain, ek transaction chalta hi nahi.
        Paisa beech me latka
```

---

*ab paisa sahi jagah, par hisaab poochha to?*

## 7. Maine poochha: kal raat 2200 tha, subah 1100 kaise?

Cue: LEDGER double-entry, sirf INSERT

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c7_1.png?v=2" width="900">

<img src="img/c7_2.png?v=2" width="900">

<img src="img/c7_3.png?v=2" width="900">

</details>

```
HAL     ledger double-entry: har paisa do line me, DEBIT + CREDIT, jod 0.
        Sirf nayi line jodte hain, sirf INSERT
        ↳ agar galat entry ho gayi ya refund aaya? -> mitao nahi, nayi ulti line jodo
          sirf INSERT hai. Audit me dono line dikhti hain
        ↳ agar 3 saal me ledger bahut bada? -> mahine ke tukde, purane S3 Glacier me
          Kanoon jitna kahe utna rakho
        ↳ agar balance column aur khaata alag? -> raat ko jod == balance, alert
          raat ko jod == balance check karo. Farak mila to alert
        ↳ agar poochhe ledger crash se bhi bachaata hai? -> nahi, wo hisaab hai
          Crash se PENDING + webhook + job bachate hain
MISAAL  raat ki line: Arpan -800 bijli +800, Arpan -300 recharge +300.
        2200 - 800 - 300 = 1100, line-line dikhta hai
KYUN    ledger hi sach hai. Sirf nayi line, kaat-peet nahi, jod hamesha 0.
        Har payment ki 2 line usi transaction me likhi jaati hain
NAHI    sirf balance column -> ANDHA: number hai (1100), kahani nahi. Support ke paas koi record nahi,
        "kaise bana" ka jawab nahi
```

---

*ab hisaab theek, par Diwali ki bheed aayi*

## 8. Shaam Diwali, 3000 payment / second, machine baith gayi

Cue: STATELESS, kai machine + LB

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c8_1.png?v=2" width="900">

<img src="img/c8_2.png?v=2" width="900">

<img src="img/c8_3.png?v=2" width="900">

</details>

```
HAL     kai machine lagao, aage LB, LB kisi bhi machine ko de. Machine kuch yaad nahi rakhti, yaad DB aur
        idempotency store me hai (stateless + LB)
        ↳ agar peeche ek machine mar gayi? -> LB GET /health, jawab nahi = bahar
          LB ke peeche wali machine. LB har kuch sec poochta hai (GET /health)
KYUN    koi bhi machine koi bhi request le sakti hai, koi bhi badlo ya badhao
AUTH    user kaun hai, ye JWT se pata karo, body se kabhi nahi. Har request pe owner check
        ↳ agar galat istemaal? -> "from" ka maalik JWT wala? Nahi to 403
          Aage WAF + had
        ↳ agar JWT chori? -> chhoti umar (~15 min) + refresh, bade pe PIN / OTP
          Bade payment pe PIN / OTP
NAHI    ek hi machine -> BAITHI: CPU bhara, payment atke, timeout, log dobara tap karte, aur bojh
```

---

*ab machine badh gayi, par DB pe report ka bojh*

## 9. Merchant ki report ne mera payment rok diya

Cue: READ REPLICA (sirf report)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c9_1.png?v=2" width="900">

<img src="img/c9_2.png?v=2" width="900">

<img src="img/c9_3.png?v=2" width="900">

</details>

```
HAL     bhaari report copy pe chalao (read replica), primary sirf payment ke liye.
        Copy 1-2 sec peeche chalti hai
        ↳ agar pay kiya, turant balance dekha? -> primary se padho, copy peeche
          Copy 1-2 sec peeche hai
        ↳ agar copy aur peeche chali gayi? -> lag naapo, had paar hui to alert
          kitni peeche hai naapo (replication lag)
        ↳ agar report itni bhaari ki copy bhi baithe? -> alag warehouse (CDC)
        ↳ agar primary gir gaya? -> sync standby ban jaata, kuch nahi khota
KYUN    paisa aur haal PRIMARY se padho, replica peeche ho sakti hai. Report replica / warehouse se.
        Payment tez rehta hai
NAHI    report bhi primary pe -> RUKA: wahi hua, mahine bhar ki report ne primary busy kiya, payment ruka
```

---

*ab padhna alag, par likhna hi itna ho gaya*

## 10. Saal me ~1000 crore row, primary ki likhai dheemi

Cue: SHARD by account_id

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c10_1.png?v=2" width="900">

<img src="img/c10_2.png?v=2" width="900">

<img src="img/c10_3.png?v=2" width="900">

</details>

```
HAL     account_id se tukde karo (shard). Mera balance aur meri history ek hi tukde pe (shard by account_id)
        ↳ agar do account alag shard pe? -> saga: A se kaato, B me daalo
          fail pe ulta. Ek transaction nahi
        ↳ agar ek account bahut bada (merchant)? -> sub-account alag shard pe
          wo shard garam ho jaata. Sub-account alag shard pe, report me jodo
        ↳ agar shard wale system me idempotency key kahan rakhoge?
          -> account_id se usi shard pe. Payment ke saath ek transaction me
        ↳ agar account kis shard pe jaayega, kaise tay hoga?
          -> ring (consistent hashing) ya naksha (lookup table)
        ↳ agar Swiggy pe 3000 credit/sec, lock ki line? -> credit sirf ledger line
          Swiggy ke ek account ki ek row pe lock ki line lag gayi. Credit sirf ledger line me
          (koi lock nahi), balance column peeche batch me jodo; ya merchant ke 10 sub-account,
          report me jodo
KYUN    jo ek transaction me saath chahiye (account + uski history) wo ek shard pe.
        Transaction ek shard tak hi rehti hai
NAHI    ek hi DB -> DHEEMA: 1000 crore row, index RAM me nahi samaata, disk se padhta, har likhai dheemi
```

---

## Poora design ek saans me

```
User -> Gateway (login + rate limit) -> Payment Svc (stateless, owner check, saga)
-> Idempotency (ek key ek baar, unique) -> SQL DB (ACID: kaato + daalo + ledger, shard by account)
-> PSP (sirf bahar ka paisa: top-up / payout / doosra bank, pehle PENDING)
-> webhook (PSP batata) · Reconciliation job (hum poochhte, jaal)
Read replica = sirf dashboard / report
DB: SQL + ACID -> kaatna aur daalna ek saath · ledger double-entry, kabhi badla nahi jaata
    idempotency key -> { haal, jawab }, ~24 ghante
```

Bolne ke liye: har payment pe ek id, dobara bheja to pehla jawab, paisa ek hi baar. Kaatna + daalna ek transaction me,
aur khaate me dono line. Alag bank / alag jagah ho to step-step + fail pe ulta. Wallet se wallet hamare DB me. Bank se
aana / bank me jaana PSP karta, isliye wahan pehle PENDING likho, phir PSP ke webhook aur roz ke milaan se pakka karo.
Aage: fraud check, kai currency, chargeback.

---

## 10 guna traffic

- **Gateway**: bheed / hamla, rate limit.
- **Payment Svc**: stateless, machine badhao.
- **Idempotency**: do request ek saath, unique / SETNX.
- **SQL DB**: kaatna + daalna ek transaction. Padhai replica pe, likhai shard by account_id.
- **PSP**: dheema = PENDING. Band = circuit breaker, "abhi nahi". Jawab nahi = webhook + reconciliation.
- **Ledger**: sirf jodo, mahine ke tukde, purana sasti jagah.
- Asli mushkil alag bank / shard ke beech ka len-den hai, bheed nahi.

**Pooche: 10 guna pe kaise badhaoge?** Paise ka raasta chalo, har dabbe pe "kya toota".

**Pooche: single point of failure kya hai?** DB primary (sync standby promote), PSP (doosra PSP).

---

## Pooche to (kisi card me nahi baitha)

API: POST /pay { from, to, amount } + header Idempotency-Key: <UUID> -> { status, txn_id }.

Sab theek chal raha hai, ye kaise pata chalega? Alert lagao (hisaab ka farak, p99, error, line) aur trace id.

Key kab banti? Key random hai, payment screen khulte banti. Arpan ne do alag payment kiye: key-555, key-888, 100 + 100
= 200, do payment, sahi. Ek tap + network ne dobara bheja: wahi key-555 dobara, "aa chuki", pehla jawab, 100. Farak =
kaun dobara bhej raha. Arpan nayi screen se (nayi key, naya payment), ya system (wahi key, ek hi baar). Cheque jaisa:
key = cheque number. Do cheque = dono cash, ek number do baar = nahi. Bank amount nahi, number dekhta.

Data: ledger double-entry, kabhi badla nahi. Idempotency key -> { haal, jawab }, ~24 ghante. Key ka haal: IN_PROGRESS
-> DONE (dobara aayi request ke liye). Payment ka haal: INITIATED -> PENDING -> SUCCESS / FAILED (paisa kahan tak
pahuncha). Niyam: saare kate == saare jude.

Scam 1992, ledger kyun itna zaroori: 1990-91 me kaagaz ka ledger + haath se bani Bank Receipt (BR). Badli ja sakti,
nakli ban sakti, turant koi jaanch nahi. Nakli / khaali BR + beech ka paisa market me + der se pakda (~Rs. 4000 cr).
Aaj ka ilaaj: nakli entry ke liye niyam (kate == jude) + sirf asli paise ki entry. Paisa gaya, share nahi, uske liye
sab-ya-kuch-nahi settlement (ACID / SAGA). Badli ja sakne wali BR ke liye kabhi na badalne wala double-entry + audit.
Beech ke paise ke liye turant reconciliation. Kaagaz se digital: ledger nahi mara, sirf kaagaz gaya. Blockchain =
sabse pakka na-badalne-wala ledger. Baad me: SEBI ko kanooni taakat (1992), NSE screen trading (1994), NSDL demat
(1996), T+2 (2003), T+1 (2021-23).

---

## Hands-on: idempotency live (usercrud, 27-Aug)

> Ek line me: "map me key hai? -> kuch mat karo, wahi wapas. Nahi? -> process + map me daal do."

```java
@RestController
public class IdempotencyController {

    // key -> pehle jo result diya (duplicate pe wahi wapas)
    private final ConcurrentHashMap<String, String> processed = new ConcurrentHashMap<>();
    // asli orders ka counter (SAABIT karega double hua ya nahi)
    private final AtomicInteger orderCounter = new AtomicInteger(0);

    @PostMapping("/pay")
    public String pay(@RequestHeader("Idempotency-Key") String key,
                      @RequestParam int amount) {

        // ATOMIC claim: key hai? -> existing wapas | nahi? -> "PROCESSING" daal + null return
        String existing = processed.putIfAbsent(key, "PROCESSING");
        if (existing != null) {
            return "DUPLICATE (key already claimed) -> " + existing
                 + "  | total orders = " + orderCounter.get();
        }

        // yahan sirf EK thread pahunchega (jisne putIfAbsent race JEETI)
        int orderId = orderCounter.incrementAndGet();
        String result = "Order #" + orderId + " created, amount=" + amount;

        processed.put(key, result);   // "PROCESSING" ko asli result se replace

        return "OK -> " + result + "  | total orders = " + orderCounter.get();
    }
}
// SecurityConfig: .requestMatchers("/pay").permitAll()
```
```
SEQUENTIAL:  A key abc-123, 100 -> OK Order #1, total 1
             B wahi dobara      -> DUPLICATE Order #1, total 1   (2 NAHI)
             C key xyz-999, 200 -> OK Order #2, total 2

RACE:        naive containsKey + put = 2 step, GAP -> A "nahi", B "nahi" -> dono order = DOUBLE
             putIfAbsent = 1 ATOMIC step -> sirf ek jeet-ta
             atomicity NAAM se nahi, ConcurrentHashMap se (bucket
             lock / CAS). plain HashMap = thread-safe nahi

20 PARALLEL, same key "race-1":
             1..20 | ForEach-Object { Start-Job { curl.exe -s -X POST ".../pay?amount=100"
             -H "Idempotency-Key: race-1" } } | Wait-Job | Receive-Job
             BROKEN (containsKey + put + sleep 50) -> Order #1 AUR #2 -> total 2 = DOUBLE CHARGE live
             ATOMIC (putIfAbsent) -> 1 OK, 19 DUPLICATE -> total 1
             pehli DUPLICATE "-> PROCESSING" = reserve-then-fill live dikha

PROD:        Redis / DB unique constraint (memory restart pe udti) + key TTL · PROCESSING placeholder
JAWAB:    Pehle 'key hai?' dekhna aur phir 'daalna' do alag kadam the, beech me doosri request ghus
          gayi. ConcurrentHashMap ka putIfAbsent dono ek hi kadam me karta, to ek hi jeetti.
          Asli system me ye kaam Redis ya DB ka unique karta. 20 request ek saath maari: purane code ne
          2 order banaye, naye ne 1.
```

---

ARCHETYPE C · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · [saga/ms-comm](../../FOUNDATIONS/10_ms_communication.md) · saath: [bookmyshow](../09_bookmyshow/09_bookmyshow_INTERVIEW.md) · [stock-broker](../05_stock_broker_trading/05_stock_broker_trading.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
