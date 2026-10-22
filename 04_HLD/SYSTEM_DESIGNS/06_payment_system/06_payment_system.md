# Payment System (UPI-style)

> User A se User B / merchant ko paisa, bharose ke saath. Misaal: Arpan ne kirana dukaan (merchant) ko Rs. 500 bheje.
> Dil do cheezein: paisa do baar na kate, aur paisa kabhi gum na ho. Finance HLD ka core, JP / GS ka favourite.
>
> Mock me sirf heading + Cue + poster. Poster me sab: upar "Pehle aisa tha", tasveer (ilaaj), hara "Kyun chalta", laal number = kya toot sakta + kaise bacha, peela = doosra tareeka kyun nahi, grey = baaki.
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-abf50228.svg)

---


## Shuru

```
FR      A se B / merchant ko bhejo (khaas: ek user se doosre tak). Dobara bhejne pe dobara na kate,
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

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | Arpan se Kirana 500 (**is design ka khaas: paisa kabhi galat nahi = ek transaction + ledger**) · merchant doosre bank me · "2200 se 1100 kaise?" | 1, 6, 7 |
> | 2 bheed aayi | Diwali 3000 payment / sec, machine baithi | 8 |
> | 3 machine giri | kaatne ke baad server gira · PSP call ke baad server gira | 1, 5 |
> | 4 do log ek saath | phone + laptop se 500-500, wallet me 600 · metro me retry, 1000 kate | 2, 3 |
> | 5 kuch dheema | PSP dheema, thread atke · merchant ki report ne payment roka | 4, 9 |
> | 6 data bada | saal me ~1000 crore row, index RAM me nahi | 10 |
> | 7 door ke log | nahi aata | - |
> | 8 bahar ka hamla | body se userId / JWT chori: owner check (card 8 ke cross-Q) | 8 |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Wallet / payment / ledger (SQL) → **AWS RDS (Postgres / MySQL)**: ACID: kaatna aur daalna ek transaction me

---

*ab tootega kahan? sabse pehle beech me, jab kaatne ke baad server gire*

## 1. Kirana ko 500 bheje, beech me server gira

**Rail 1 KAAM CHALA (khaas)** · Cue: EK TRANSACTION (WAL se poora / ulta)

<img src="img/c1_poster.png?v=2" width="1000" alt="card 1: kya toota, kaise bacha">


---

*ab ek payment theek, par do payment ek saath ek hi wallet pe?*

## 2. Wallet me 600, phone se 500 aur laptop se 500 ek saath

**Rail 4 DO LOG EK SAATH (wallet)** · Cue: check + kaato EK atomic query

<img src="img/c2_poster.png?v=2" width="1000" alt="card 2: kya toota, kaise bacha">


---

*ab balance theek, par wahi payment network ne do baar bheji to?*

## 3. Metro me network gaya, app ne dobara bheja, 1000 kate

**Rail 4 DO LOG EK SAATH (retry)** · Cue: IDEMPOTENCY KEY (screen khulte)

<img src="img/c3_poster.png?v=2" width="1000" alt="card 3: kya toota, kaise bacha">


---

*ab andar sab theek, par bahar ka PSP dheema ho to?*

## 4. Bank se 2000 top-up, PSP dheema

**Rail 5 KUCH DHEEMA (PSP)** · Cue: timeout = PENDING, circuit breaker = abhi nahi

<img src="img/c4_poster.png?v=2" width="1000" alt="card 4: kya toota, kaise bacha">


---

*ab call gayi aur jawab se pehle hum hi gir gaye to?*

## 5. PSP ko call kiya aur server gira: paisa gaya ya nahi?

**Rail 3 MACHINE GIRI (PSP call)** · Cue: PENDING pehle + WEBHOOK + RECONCILE

<img src="img/c5_poster.png?v=2" width="1000" alt="card 5: kya toota, kaise bacha">


---

*ab merchant doosre bank me hai, ek transaction ban hi nahi sakti*

## 6. Merchant ka account doosre bank me, ek transaction nahi chalta

**Rail 1 KAAM CHALA (doosra bank)** · Cue: SAGA (step-step, fail pe ulta)

<img src="img/c6_poster.png?v=2" width="1000" alt="card 6: kya toota, kaise bacha">


---

*ab paisa sahi jagah, par hisaab poochha to?*

## 7. Maine poochha: kal raat 2200 tha, subah 1100 kaise?

**Rail 1 KAAM CHALA (hisaab)** · Cue: LEDGER double-entry, sirf INSERT

<img src="img/c7_poster.png?v=2" width="1000" alt="card 7: kya toota, kaise bacha">


---

*ab hisaab theek, par Diwali ki bheed aayi*

## 8. Shaam Diwali, 3000 payment / second, machine baith gayi

**Rail 2 BHEED AAYI** · Cue: STATELESS, kai machine + LB

<img src="img/c8_poster.png?v=2" width="1000" alt="card 8: kya toota, kaise bacha">


---

*ab machine badh gayi, par DB pe report ka bojh*

## 9. Merchant ki report ne mera payment rok diya

**Rail 5 KUCH DHEEMA (report)** · Cue: READ REPLICA (sirf report)

<img src="img/c9_poster.png?v=2" width="1000" alt="card 9: kya toota, kaise bacha">


---

*ab padhna alag, par likhna hi itna ho gaya*

## 10. Saal me ~1000 crore row, primary ki likhai dheemi

**Rail 6 DATA BADA** · Cue: SHARD by account_id

<img src="img/c10_poster.png?v=2" width="1000" alt="card 10: kya toota, kaise bacha">


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
