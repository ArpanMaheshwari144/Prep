# Payment System (UPI-style)

> User A se User B / merchant ko paisa, bharose ke saath. Misaal: Arpan ne kirana dukaan (merchant) ko Rs. 500 bheje.
> Dil do cheezein: paisa do baar na kate, aur paisa kabhi gum na ho. Finance HLD ka core, JP / GS ka favourite.
>
> Mock me sirf heading + Cue + GIF. Neeche har card: **hal** · **kyun** · **nahi** (doosra kyun nahi) · **Pooche**.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-abf50228.svg)

---


## Shuru

- **FR:** A se B / merchant ko bhejo (main: ek user se doosre tak), dobara bhejne pe dobara na kate, fail ho to paisa wapas, har paisa traceable · scope pehle poochho: P2P / merchant / refund / settlement / fraud? refund scope me, bahar ka PSP (Razorpay / Stripe) ya khud? audit / RBI ka hisaab? · scope bahar: fraud engine, kai currency, chargeback
- **NFR:** chaar mushkil, poori file inhi pe: do baar na kate, 500 ki jagah 1000 (idempotency) · dono taraf ek saath (consistency) · kuch gire to bhi paisa na khoye, kata par pahuncha nahi, beech me network gaya to atke nahi (failure handling) · har paisa trace ho (ledger / audit)
- **numbers:** ~10 crore user, aam din kuch sau payment / sec, festival / sale pe kuch hazaar / sec. Bheed badi dikkat nahi, sahi hona badi dikkat -> pehle sab-ya-kuch-nahi + do-baar-nahi + ledger

---

*ab tootega kahan? sabse pehle beech me, jab kaatne ke baad server gire*

## 1. Kirana ko 500 bheje, beech me server gira

**Cue:** EK TRANSACTION (WAL se poora / ulta) · dabba: SQL DB

- **hal:** dono badlaav ek transaction: BEGIN, Arpan -500, Kirana +500, COMMIT. Ya dono ya koi nahi.
- **paisa kahan:** Arpan + merchant dono ka wallet hamare system me (Paytm / PhonePe wallet jaisa). Wallet se wallet = hamare DB ke andar, ek transaction. Poori file isi pe.
- **DB:** SQL. Ek payment = paanch likhai: Arpan -500, merchant +500, ledger me 2 line, payment ka haal. BEGIN, paanchon, COMMIT. Beech me fail = ROLLBACK, ek bhi nahi (SQL transaction, ACID).
- **teen table:** wallet (account_id, balance), ledger (sirf jodte), payment (key, haal)
- **kyun:** DB ka apna log (WAL). COMMIT se pehle gira = utha ke log dekhta, COMMIT nahi tha, ulta, 500 wapas.
- **kyun:** COMMIT ke baad gira = log me COMMIT hai, poora karo, kuch nahi khota. Saare kate == saare jude, aadha kabhi nahi (chaar hui, ek reh gayi = hisaab galat).
- **nahi:** sabse simple: do alag badlaav, ek ke baad ek (Arpan -500, phir merchant +500) -> beech me crash = ek hua, doosra nahi, paisa gayab
- **Pooche:** ledger INSERT fail ho gaya? -> ROLLBACK. Arpan ka -500 bhi ulta, paanchon me se ek bhi nahi.
- **Pooche:** ROLLBACK hua, user ko kya? -> "nahi hua", paisa jaisa tha, dobara try kar sakta. User ko sirf "hua" / "nahi hua", beech ka kabhi nahi.
- **Pooche:** hisaab me farak aaya to? -> turant alert. User ko "hua" ya "nahi hua" dikhe, beech ka nahi.
- **Pooche:** NoSQL kyun nahi? -> kai row / table pe ek saath sab ya kuch nahi + paisa turant sahi. "Thodi der baad sahi" (eventual) paise me nahi chalta.
- **dhyan:** "NoSQL me ACID hota hi nahi" mat bolo, kai NoSQL ek row / document pe atomic. Sahi farak: debit + credit + ledger = kai row ek saath, SQL me seedha + pakka.
- **Pooche:** consistency ya availability? -> sahi hona pehle. Network toota, balance pakka nahi = payment mana. Galat balance pe kaatna usse bura. Dashboard / report thode purane bhi chalte rahein.
- **Pooche:** A->B aur B->A ek saath, dono atke (deadlock)? -> taala hamesha chhoti account id pehle. Poora 13_banking card 9 me.

---

*ab ek payment theek, par do payment ek saath ek hi wallet pe?*

## 2. Wallet me 600, phone se 500 aur laptop se 500 ek saath

**Cue:** check + kaato EK atomic query · dabba: (DB query)

- **hal:** check aur kaatna ek hi query: `SET balance = balance - 500 WHERE id = ? AND balance >= 500`. Phone jeeta, 600 se 100. Laptop ko 100 < 500, 0 row, "paisa kam" (atomic UPDATE)
- **kyun:** "hai?" aur "kaato" ek hi kadam, DB ke andar. Kitni bhi machine, DB ek-ek karke chalata, jeetta ek. 0 row = reject, user ko turant "paisa kam".
- **nahi:** pehle padho, phir kaato -> phone (dukaan 500) + laptop (Swiggy 500), dono server ne 600 dekha, dono ne kaata, wallet -400
- **nahi:** idempotency key -> ye do alag payment, do log ek balance pe. Key sirf ek user ka dobara bhejna pakadti (card 3).
- **Pooche:** WHERE check likhna bhool gaye? -> DB ka CHECK (balance >= 0) pakad leta. Aakhri deewar.

---

*ab balance theek, par wahi payment network ne do baar bheji to?*

## 3. Metro me network gaya, app ne dobara bheja, 1000 kate

**Cue:** IDEMPOTENCY KEY (screen khulte) · dabba: Idempotency store

- **hal:** payment screen khulte hi app key banata (key-555), retry pe wahi key. Store me key-555 pehle se = saved jawab, 500 hi kate. DB me key pe UNIQUE (idempotency key). Saath me Pay button pehli tap pe band.
- **kyun:** ek key = ek payment. Wahi key dobara = kaam nahi, saved jawab. User ko ek hi payment dikhe. Nayi payment = nayi screen = nayi key.
- **key ka haal:** IN_PROGRESS -> kaato + daalo -> DONE + jawab
- **nahi:** bina key -> timeout, app ne dobara bheja, server ne do naye payment maane, 1000 kate
- **nahi:** key server banaye -> dobara app bhejta hai. App ki key hi dono request jodti, server ko pata hi nahi ye wahi wali.
- **nahi:** sirf DB -> Redis (SETNX) pehle tez pakadta
- **nahi:** sirf Redis -> DB ka UNIQUE aakhri deewar
- **Pooche:** wahi key, amount alag? -> request ka hash bhi rakho. Farak = 422.
- **Pooche:** key kab hatao? -> pakka fail pe hi (paisa hila nahi). DONE wali ~24 ghante.
- **Pooche:** sach me do payment, 100 + 100? -> do baar screen khuli = do key (key-555, key-888), dono honge, 200. Maqsad sirf network ke dobara bhejne ko do baar ginne se rokna.

---

*ab andar sab theek, par bahar ka PSP dheema ho to?*

## 4. Bank se 2000 top-up, PSP dheema

**Cue:** timeout = PENDING, circuit breaker = abhi nahi · dabba: PSP

- **hal:** thodi der (2-3 sec) me jawab nahi -> chhodo, PENDING likho (pata nahi kata), baad me pata karo (timeout)
- **hal:** 5 fail -> circuit OPEN: call jaati hi nahi, saaf "abhi nahi", paisa hila hi nahi (circuit breaker)
- **PSP kab:** bahar ka paisa PSP (Razorpay / bank) se: bank se wallet me (top-up), wallet se bank me (payout), ya merchant kisi doosre bank me
- **kyun:** call gayi aur jawab nahi = PENDING, "fail" kabhi nahi. Call gayi hi nahi (circuit OPEN) = saaf "abhi nahi".
- **kyun:** PSP dheema to jaldi chhodo, warna saare thread atke
- **nahi:** timeout lagao hi mat, wait karo -> har thread PSP ke jawab tak ruka (PSP 30 sec), sab atke, system band
- **Pooche:** circuit OPEN, phir kab khulega? -> 30 sec baad ek test call. Chali to circuit band, nahi to phir OPEN.
- **Pooche:** PSP hi poora band? -> doosra PSP. Par ek payment hamesha ek hi PSP pe.

---

*ab call gayi aur jawab se pehle hum hi gir gaye to?*

## 5. PSP ko call kiya aur server gira: paisa gaya ya nahi?

**Cue:** PENDING pehle + WEBHOOK + RECONCILE · dabba: Reconciliation job

- **hal:** call se PEHLE DB me PENDING. PSP baad me khud bataye (webhook). Purane PENDING pe ek job PSP se haal poochhe. Dono me se jo bhi bataye, DB me SUCCESS.
- **kyun:** "pata nahi" pehle likha, to server gire tab bhi tracking number bacha. PSP bataye (webhook) ya hum poochhe (job). User ko PROCESSING, andha dobara kabhi nahi.
- **nahi:** sirf webhook -> webhook kho sakta (der, ya hamara server band). Job chhoota hua pakadti.
- **nahi:** sirf job -> der lagti + kharcha (PSP ki limit). Isliye dono.
- **Pooche:** webhook do baar aaya? -> SUCCESS sirf ek baar, doosra ignore
- **Pooche:** webhook nakli ho to? -> hastakshar jaancho (PSP secret) + apne PENDING se milao
- **Pooche:** webhook aaya hi nahi? -> job har kuch minute purane PENDING PSP se poochhti. Raat ko PSP ki file se poora milaan (reconciliation).
- **Pooche:** job ko PSP ne kya bataya, phir? -> hua = SUCCESS. Pakka fail = FAILED, kuch wapas nahi karna (paisa hila nahi).
- **Pooche:** sab theek chal raha, kaise pata? -> alert (hisaab ka farak, p99, error, line) + trace id

---

*ab merchant doosre bank me hai, ek transaction ban hi nahi sakti*

## 6. Merchant ka account doosre bank me, ek transaction nahi chalta

**Cue:** SAGA (step-step, fail pe ulta) · dabba: Payment Svc + SAGA

- **hal:** step-step chalo, har kadam saga table me (DEBITED, PAYOUT). Bank B ka payout fail = pichhle ka ulta kaam: wallet me 500 wapas (saga, compensate)
- **kyun:** alag system me har step likha hua. Aage nahi badh sake to ulta. User ko PROCESSING, phir SUCCESS ya paisa wapas. Hisaab saaf.
- **sabse bada sawaal (shuru me poochho):** dono account ek hi system / bank me ya alag? Poora design isse badalta. Ek DB = ek transaction (ACID) kaafi, alag bank = SAGA ka khel.
- **nahi:** ek transaction me dono -> do alag system, ek transaction chalta hi nahi. Paisa beech me latka.
- **Pooche:** service beech me giri? -> uth ke saga table dekhe, wahin se aage ya ulta
- **Pooche:** ulta kaam bhi fail? -> retry + alert, manual queue. Paisa beech me na atke.

---

*ab paisa sahi jagah, par hisaab poochha to?*

## 7. Maine poochha: kal raat 2200 tha, subah 1100 kaise?

**Cue:** LEDGER double-entry, sirf INSERT · dabba: Ledger table

- **hal:** har paisa do line: DEBIT + CREDIT, jod 0. Sirf nayi line jodte (ledger, double-entry).
- **misaal:** raat ki line: Arpan -800 bijli +800, Arpan -300 recharge +300. 2200 - 800 - 300 = 1100, line-line dikhta.
- **kyun:** ledger hi sach. Sirf nayi line, kaat-peet nahi, jod hamesha 0. Har payment ki 2 line usi transaction me.
- **nahi:** sirf balance column -> number hai (1100), kahani nahi. Support ke paas koi record nahi, "kaise bana" ka jawab nahi.
- **Pooche:** galat entry ya refund? -> mitao nahi (sirf INSERT). Nayi ulti line, audit me dono dikhti.
- **Pooche:** balance column aur khaata alag ho gaye? -> raat ko jod == balance. Farak = alert.
- **Pooche:** ledger crash se bhi bachaata? -> nahi, wo hisaab hai. Crash se PENDING + webhook + job bachate.
- **Pooche:** 3 saal me ledger bahut bada? -> mahine ke tukde, purane S3 Glacier me. Kanoon jitna kahe utna rakho.

---

*ab hisaab theek, par Diwali ki bheed aayi*

## 8. Shaam Diwali, 3000 payment / second, machine baith gayi

**Cue:** STATELESS, kai machine + LB · dabba: LB / Gateway + Svc x N

- **hal:** kai machine, aage LB, LB kisi bhi machine ko de. Machine kuch yaad nahi rakhti, yaad DB + idempotency store me (stateless + LB).
- **kyun:** koi bhi machine koi bhi request le, koi bhi badlo / badhao
- **auth:** user kaun hai = JWT se, body se kabhi nahi. Har request pe owner check.
- **nahi:** ek hi machine -> CPU bhara, payment atke, timeout, log dobara tap karte, aur bojh
- **Pooche:** ek machine mari? -> LB har kuch sec poochta (GET /health). Jawab nahi = bahar.
- **Pooche:** galat istemaal kaise roko? -> "from" ka maalik JWT wala hai? Nahi to 403. Aage WAF + had.
- **Pooche:** JWT chori ho gaya? -> chhoti umar (~15 min) + refresh. Bade payment pe PIN / OTP.

---

*ab machine badh gayi, par DB pe report ka bojh*

## 9. Merchant ki report ne mera payment rok diya

**Cue:** READ REPLICA (sirf report) · dabba: Read replica

- **hal:** bhaari report copy pe (read replica), primary sirf payment. Copy 1-2 sec peeche chalti.
- **kyun:** paisa / haal PRIMARY se padho, replica peeche ho sakti. Report replica / warehouse se. Payment tez.
- **nahi:** report bhi primary pe -> wahi hua: mahine bhar ki report ne primary busy kiya, payment ruka
- **Pooche:** pay kiya, turant balance dekha? -> primary se padho. Copy 1-2 sec peeche hai.
- **Pooche:** copy aur peeche chali gayi? -> kitni peeche naapo (replication lag), had paar = alert
- **Pooche:** report itni bhaari ki copy bhi baith jaaye? -> alag warehouse (CDC)
- **Pooche:** primary gira? -> sync standby bane, kuch nahi khota

---

*ab padhna alag, par likhna hi itna ho gaya*

## 10. Saal me ~1000 crore row, primary ki likhai dheemi

**Cue:** SHARD by account_id · dabba: SQL DB shard

- **hal:** account_id se tukde (shard). Mera balance + meri history ek hi tukde pe (shard by account_id).
- **kyun:** jo ek transaction me saath chahiye (account + uski history) wo ek shard pe. Transaction ek shard tak hi rehti.
- **nahi:** ek hi DB -> 1000 crore row, index RAM me nahi samaata, disk se padhta, har likhai dheemi
- **Pooche:** do account alag shard pe? -> saga: A se kaato, B me daalo, fail pe ulta. Ek transaction nahi.
- **Pooche:** ek account bahut bada (merchant)? -> wo shard garam. Sub-account alag shard pe, report me jodo.
- **Pooche:** idempotency key kahan rakhoge? -> account_id se usi shard pe. Payment ke saath ek transaction me.
- **Pooche:** account kis shard pe, kaise tay? -> ring (consistent hashing) ya naksha (lookup table)
- **Pooche:** Swiggy ke ek account pe 3000 credit/sec, ek row pe lock ki line? -> credit sirf ledger line me (koi lock nahi), balance column peeche batch me jodo; ya merchant ke 10 sub-account, report me jodo

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
