# PR / CODE-REVIEW ROUND — ATTACK + SAMAJH + RECORD

> **CONFIRMED (17-Sep, bahar ke source se — andaaza nahi):** JP ke Superday me
> **~10 minute ka asli PR-review segment hota hai** — ek pull request deke bug + style poochte hain.
> Ye alag round nahi, Superday ka hissa hai. (techinterview.org JPMorgan guide + 5 candidate writeups.)
>
> Ye CHECKLIST-skill hai (finite, ~20 recurring issue). **Knowledge pehle se hai — aadat SCAN ki chahiye.**

---

## ★ IS FILE KO KAISE USE KARNA — teen hisse, teen kaam

```
HISSA 1 — ATTACK    drill ke WAQT sirf ye khula rakho. 5 kadam + 2 list. Bas.
HISSA 2 — SAMAJH    ek baar padhne ki cheez. Har bucket ka poora detail + "kyun".
HISSA 3 — RECORD    drill ke BAAD bharne ki cheez. Score, kya chhoota, pattern.
```

* [HISSA 1 — ATTACK](#hissa-1--attack) — 5 kadam · MUST-HAVE list · NISHAAN table · bolne ka tarika
* [HISSA 2 — SAMAJH](#hissa-2--samajh) — 6 bucket · auth ka farak · kyun ye kram
* [HISSA 3 — RECORD](#hissa-3--record) — 4 drill ka data · 6 example · pattern · mindset

---
---

# HISSA 1 — ATTACK

> Drill ke waqt neeche ki char cheez ke alawa kuch nahi chahiye.
> **17-Sep ka sabak:** checklist me SQL-injection aur money-in-double dono LIKHE the.
> Drill me dono chhoot gaye. File PADHNA kaam nahi karta — **CHALANA** karta hai.

## ★ 1.1 — PAANCH KADAM

```
KADAM 1 — 30 SECOND, kuch mat bolo. Sirf POORA code upar-neeche padho.
           "ye class kar kya rahi hai?" bas itna.
           (seedha bug dhoondhne lage -> pehla dikhne wala hi bol doge, baaki chhoot jaayega)

KADAM 2 — TEEN SAWAAL, is kram me. Har ek pe POORI file pe nazar daudao.

   (a) "USER ka data KAHAN-KAHAN ja raha hai?"
         -> SQL string me jud raha?        = INJECTION
         -> log me ja raha?                 = card/password leak
         -> seedha return ho raha?          = entity expose
         -> bina validate use ho raha?

   (b) "Yahan CRASH ho gaya to AADHA kaam kis haal me chhootega?"
         -> do write, ek transaction nahi?  = paisa kata, order nahi bana
         -> beech me BAAHRI call?           = uska rollback hota hi nahi
         -> resource khula reh jaayega?     = leak
         -> exception nigli ja rahi?        = galti dikhegi hi nahi

   (c) "Ye DO BAAR chal gaya to?"
         -> idempotency hai?                = double charge
         -> shared mutable state?           = thread-safety
         -> retry safe hai?

KADAM 3 — 5-BUCKET pe ek sweep (HISSA 2 me poori list).
           Jo (a)(b)(c) me nahi aaya, wo yahan milega.

KADAM 4 — MUST-HAVE LIST (1.2) — ek-ek line pe tick ya cross. Ye SABSE zaroori kadam hai.

KADAM 5 — BOLO structured: security -> java-trap -> resource -> db -> design.  (tarika 1.4 me)
```

## ★★ 1.2 — MUST-HAVE LIST — paise/data badalne wale HAR endpoint pe

> **Kyun list, aur kyun kadam 1-3 kaafi nahi** (18-Sep, Arpan-pakdi):
> kadam 1-3 tak jo kiya wo **HUNT** tha — galat likhi hui line dhoondhna. Do kism ke bug hote hain:
>
> ```
> GALAT LIKHA HUA   ->  line maujood hai, wo galat hai   (SQL concat · Float · == · get(0))
>                       -> AANKH pakad leti hai. Hunt kaam karta hai.
>
> LIKHA HI NAHI     ->  koi line hai hi NAHI              (auth · validation · error-code)
>                       -> aankh KABHI nahi pakdegi. Dekhne ko kuch hai hi nahi.
> ```
>
> Aur ye JAAN-KAARI ka mamla NAHI hai (Arpan ne saaf kiya): *"auth to seekha hua hai — par FILE
> SCAN karte waqt uske liye DEKHNA kabhi seekha hi nahi."* Gap gyaan me nahi, SCAN ki aadat me
> hai. Aur aadat list se banti hai — gayab cheez hunt se nahi milti, ginni padti hai.

```
[ ] AUTHORIZATION   bulane wale ka HAQ hai is resource pe?
[ ] VALIDATION      amount > 0? -ve? bahut bada? decimal? account exist? khud ko khud transfer?
[ ] AMOUNT COMPARE  request ka amount aur DB ka asli amount — kabhi COMPARE hue?
[ ] MONEY TYPE      paisa double/float me hai? -> BigDecimal ya paise LONG me
[ ] TRANSACTION     multi-write ek saath commit/rollback?
[ ] KRAM            do DB-write ke BEECH me koi BAAHRI call? (uska rollback hota hi nahi)
[ ] IDEMPOTENCY     dobara chala to? (aur jo hai wo SACH ME kaam karta hai?)
[ ] ERROR CODE      fail pe 4xx? ya sab 200 OK ja raha?
[ ] AUDIT           kaun, kab, kitna — record hua?
```

★ **20-Sep tak in 9 ka haal** (gina hua, andaaza nahi):

```
AB AA JAATE HAIN    AUTHORIZATION · IDEMPOTENCY   (drill 3 aur 4, lagataar)
                    MONEY TYPE · KRAM             (drill 2-3 me chhoote the, drill 4 me AAYE)
                    TRANSACTION                   (drill 2 se aa raha hai)

ABHI BHI CHHOOTE    VALIDATION      (-ve · 0 · bahut bada)
                    AMOUNT COMPARE  (drill 3 aur 4 dono)
                    ERROR CODE      (drill 2, 3 aur 4 — ★ TEEN baar)
                    AUDIT           (drill 3 aur 4 dono)
```

Jo aa gaye, wo list me rehne se aaye — yaad se nahi. Jo chhoot rahe hain, unke liye list
PADHNI padegi; wo nazar se nahi milte, gin ke milte hain.

## ★★ 1.3 — NISHAAN TABLE — "code me kya DIKHA, aur kya POOCHNA hai"

> Miss ki list padhne se kuch nahi hota. Har miss ke saamne ek cheez code me SAAF dikh rahi thi.
> Agli baar wo cheez dikhe -> sawaal apne-aap chalna chahiye. **Yahi ratt-ne wali table hai.**

```
NISHAAN (jo aankh ke saamne hota hai)          ->  SAWAAL (jo turant chalna chahiye)
-------------------------------------------------------------------------------------------
SQL ke andar kahin bhi  " + "                      "ye PreparedStatement kyun nahi?"
                                                   = sabse bada bug, RUK JAO

request me kisi ki CHEEZ ka naam                   "bhejne wala iska MAALIK hai? check kahan?"
(fromAccount · orderId · userId · documentId)      (filter me ho hi nahi sakta — usne ye param
@RequestParam String fromAcc  <- yahin rukna hai    dekha hi nahi. @PreAuthorize / service /
                                                    aspect me ho sakta -> POOCHO, ilzaam nahi)

request se aaya NUMBER, aur usse ghatao/jodo       "iski HAD kahan check hui?
ho raha                                             -ve? 0? bahut bada? decimal?"
                                                   (-5000 -> balance-(-5000) = paisa BANA)

request se aaya amount, aur DB me uska asli        "kya ye DONO kabhi COMPARE hue?"
amount bhi paas me hai                             (19-Sep: orderAmount nikala, service tak
                                                    bheja, aur kabhi compare hi nahi kiya
                                                    -> Rs.500 ke order pe Rs.50,000 ka refund)

float / double + paisa                             "ye paisa hai -> BigDecimal"
                                                   ★ 17-Sep pakda, 18 aur 19 me chhoota,
                                                   20-Sep dobara AAYA — ab list se aata hai

Object / String pe  ==                             "ye reference jod raha ya value?"
                                                   (status=="FROZEN" -> hamesha false ->
                                                    frozen account bhi nikal gaya)

do DB-write ke BEECH me BAAHAR ka call             "iske BAAD wala step fail hua to jo BAHAR
(gateway · mail · notification · 3rd-party API)     ja chuka, wo wapas aayega?"
                                                   (debit -> "paisa aaya" bhej diya -> credit fail)
                                                   (19-Sep: paisa gateway se nikla, company ke
                                                    paas record hi nahi bana)
                                                   ★ drill-2 aur 3 me chhoota, drill-4 me AAYA

controller/service me koi MUTABLE field            "ye har request me SAANJHA hai — thread-safe?
(Map · List · counter · flag)                       kabhi khaali hota? 2 pod pe chalega?"
                                                   (recentTransfers = idempotency ka DIKHAWA)

ResponseEntity.ok  failure waali branch me         "is haalat ka HTTP code kya hona chahiye?
                                                    400 / 409 / 422?"
                                                   ★★ TEEN BAAR CHHOOTA: drill 2, 3 aur 4
                                                   (aur "ALREADY_IN_PROGRESS" bhi 200 OK tha
                                                    -> wo 409 hai)

koi constant / field DECLARE hua                   "ye use kahan hua?" — kahin nahi = dead code
                                                   (MAX_RETRIES · MAX_REFUND_ATTEMPTS)

paise wala kaam, aur koi INSERT-into-audit nahi    "kisne maanga, kisne approve kiya, kab?"
                                                   finance me ye COMPLIANCE ki cheez hai
```

## ★ 1.4 — BOLNE KA TARIKA

```
SCAN ORDER:  security -> java-trap -> resource -> db -> design/error-handling
BOLNE KA TARIKA (loud, structured):
   "Main pehle SECURITY dekhta hoon -- yahan SQL string-concat se ban rahi, injection risk...
    phir CONCURRENCY -- ye shared SimpleDateFormat thread-safe nahi...
    phir RESOURCE -- ye connection close nahi ho raha...
    phir DB -- ye loop me N+1...
    aur ye payment flow hai, to main ye bhi dekhunga ki retry pe double-charge to nahi hoga."
-> structured review = interviewer ko dikhta tu SYSTEMATIC hai, andaze se nahi.

★ TIME: ~10 min ka segment hai. 2-3 CRITICAL pehle bolo (security/paisa), style baad me.
  Ek candidate ne 30 min PR-review me laga diye aur DSA ka time chala gaya — ye mat karna.

★ GAYAB cheez ke liye SAWAAL, ILZAAM nahi:
  "yahan ownership-check nazar nahi aa raha — kya wo kisi aspect/@PreAuthorize me hai?"
```

---
---

# HISSA 2 — SAMAJH

## ★ 2.1 — 5-BUCKET CHECKLIST (har PR snippet inhi me se aata)

```
1. SECURITY
   [] SQL injection      -> string-concat query ("...'" + x + "'")? -> PreparedStatement (?)
                            ★ SQL ke andar kahin bhi  " + "  dikhe = RUK JAO. Sabse bada bug.
   [] hardcoded secret   -> password / API-key / URL code me? -> env / vault / config
   [] password handling  -> plaintext store? (BCrypt) · logs me print? (mat karo)
   [] LOG me sensitive   -> log.info("..." + req) -> poora object = card/password logs me (PCI)
   [] sensitive expose   -> entity seedha return (password-field ke saath)? -> DTO
   [] input validation   -> user-input bina validate use ho raha?

2. JAVA TRAPS (round-2 ye HUNT karta)
   [] SimpleDateFormat   -> shared/static? -> NOT thread-safe -> DateTimeFormatter (immutable)
   [] Optional.get()     -> bina check .get()? -> orElseThrow / handle
   [] == on String/boxed -> Long / Integer / String == compare? -> .equals()
                            ("INR".equals(x) likho -> null-safe bhi ho jaata)
   [] mutable shared field -> non-final instance/static field (thread-unsafe)?
   [] rs.next() ka return -> check kiya? row na mile to seedha crash
   [] blind cast          -> (double) map.get(...) — type pata hai? null aaya to?

3. RESOURCE
   [] stream/writer/conn -> close hota? -> try-with-resources
   [] Connection + Statement + ResultSet — teeno (ek bhi chhoota = leak)
   [] connection / thread / file-handle leak

4. DATABASE
   [] N+1 query          -> loop me findById / save? -> findAllById / saveAll (batch)
   [] @Transactional     -> multi-write bina transaction (atomicity)?
   [] double for money   -> ★ BigDecimal (scale + RoundingMode) ya paise LONG me.
                            0.1 + 0.2 = 0.30000000000000004 -> lakhon txn pe ledger MISMATCH
                            -> reconciliation FAIL. Finance code ka PEHLA niyam.
   [] rows.get(0)        -> khaali list pe crash

5. DESIGN / ERROR-HANDLING
   [] SRP violation      -> controller/service me DB + business + external sab? -> layer alag
   [] raw JDBC in service-> repository ka kaam service me ghusa hua
   [] exception swallow  -> catch(Exception){} · e.printStackTrace() · return null/"FAILED"
   [] magic numbers / magic string return values
   [] field @Autowired   -> constructor injection better (final + testable)
   [] dead code          -> declare hua constant/field jo kahin use nahi

6. ★ FINANCE-FLAVOUR (JP/GS/Barclays ke liye — ye list me hona hi chahiye)
   [] BAAHRI CALL transaction ke ANDAR
        stripe.charge() -> phir DB update fail
        = paisa asli duniya me kat gaya, DB me kuch nahi. Rollback ho hi nahi sakta.
        -> DB kaam ek transaction me · baahri call USSE BAHAR · phir reconcile/retry
   [] IDEMPOTENCY nahi   -> user ne do baar dabaya / network retry -> DOUBLE CHARGE
                            -> idempotency key (04_HLD/07_payment_system wali baat)
   [] AUDIT trail        -> paise wala kaam bina record ke? kaun, kab, kitna
   [] AMOUNT validation  -> request ka amount DB ke asli amount se compare hua?
   [] consistency > availability — paise me ye default jhukaav hai
```

## ★★ 2.2 — AUTH ka SAHI FARAK (18-Sep, Arpan-pushback se nikla)

```
AUTHENTICATION   "kaun ho tum?"
                 -> SecurityConfig / JwtFilter me, EK jagah, saalon pehle likha
                 -> controller me dikhta hi NAHI, aur dikhna bhi nahi chahiye
                 -> isliye "controller me auth nahi hai" sochna ULTA lagta hai. Sahi lagta hai.

AUTHORIZATION    "is ACCOUNT / ORDER pe tumhara HAQ hai?"
                 -> GENERIC filter ye nahi kar sakta — usne fromAcc dekha hi nahi,
                    wo request me ABHI aaya hai
                 -> par ye handler me HI ho aisa zaroori NAHI:
                    @PreAuthorize("#fromAcc == authentication.name") · service-layer check ·
                    ek aspect — teeno jayaz jagah hain
                 ★ matlab: is snippet me wo KAHIN nahi dikh raha — aur PR-review me itna hi
                   bolna hai: "yahan ownership-check nazar nahi aa raha, kya wo kisi
                   aspect/@PreAuthorize me hai?" — ye SAWAAL hai, ilzaam nahi.
```

## ★ 2.3 — KYUN teen sawaal PEHLE, checklist BAAD me

checklist **shakal** pakadti hai (hardcoded string, naya object, missing close) — wo aankh se
dikh jaati hai. Teen sawaal **behaviour** pakadte hain (runtime pe, crash pe, retry pe kya hoga) —
wo aankh se DIKHTA HI NAHI. 17-Sep ko jo 8 chhoote, wo lagbhag saare BEHAVIOUR wale the.

## ★★ 2.4 — MAUJOOD HONA aur KAAM KARNA do alag baat hai

```
18-Sep drill ne ye saaf kiya:
  "idempotency nahi hai" bolna SAHI tha — jabki Map maujood tha.
  Kyunki wo Map kaam hi nahi karta:
     HashMap                   = thread-unsafe
     kabhi clear nahi          = leak
     key me amount/time nahi   = dobara bhejna HAMESHA blocked
     instance field            = restart pe gayab, 2 pod pe bekaar
     put() PROCESSING se PEHLE = fail hone pe entry hamesha ke liye retry BLOCK kar degi

-> Cheez dikhe to ruk ke poochho: "ye waqai wo kaam karti hai jiske liye rakhi hai?"
   Dikh gayi, tick laga diya — ye galti hai.
```

---
---

# HISSA 3 — RECORD

## ★ 3.1 — SAARE DRILL ka data

```
17-Sep   PaymentService        7 / 15  (47%)   zyadatar SHAKAL wale (string · missing close · field-injection)
18-Sep   TransferController    7 / 15  (47%)   ab BEHAVIOUR wale (transaction · idempotency · injection)
19-Sep   RefundController     11 / 18  (61%)   ★ authz + idempotency DONO aaye (pichhli baar chhoote the)
20-Sep   DisbursalController  11 / 18  (61%)   ★ MONEY-TYPE + KRAM dono aaye (pichhli DO baar chhoote the)
                                              chhoote 7 me se 4 MUST-HAVE list me likhe hue the

★ IMAANDARI: chaaron PR alag the, mushkil bhi alag.
  Abhi tak ka sach: 7/15, 7/15, 11/18, 11/18.
  Do baar lagataar 61% — 47% se upar, aur ab ye ek baar ka ittefaaq nahi hai.
  Score wahi raha par CHHOOTNE WALI cheezein badal gayi — wo asli badlav hai, %-nahi.
```

## ★ 3.2 — PATTERN, waqt ke saath

```
PURANA pattern (17+18 Sep)  ->  "LIKHA HI NAHI" wale chhoot rahe the (authz · idempotency)
                                ★ YE THEEK HO GAYA — 19-Sep ko dono aaye

NAYA pattern (19-Sep)       ->  PAKDA   : STRUCTURE + JAVA-TRAP
                                          (injection · transaction · == · thread-safety · leak)
                                CHHOOTA : PAISE KE NIYAM
                                          (double for money · range check · amount se compare)
                                          KRAM aur NAAKAAMI
                                          (bahar ka call beech me · crash pe kya · 200 on failure)

20-Sep ka pattern           ->  PAKDA   : upar wala SAB, aur uske OOPAR
                                          double-for-money aur bahar-ka-call-beech-me
                                          -> yaani jo cheez LIKHI hui hai, wo ab aa jaati hai

                                CHHOOTA : sirf wo jo LIKHI HI NAHI HAI
                                          validation · amount-compare · error-code · audit
                                          (chaaron MUST-HAVE list ki line hain)

★ ISKA MATLAB EK HI HAI: HUNT (kadam 1-3) kaam kar raha hai.
  Jo chhoot raha hai wo sirf KADAM 4 hai — list padhi hi nahi jaati.
  Agle drill me sudhaar list se aayega, aur zyada dhyaan se dekhne se nahi.
```

## ★ 3.3 — WORKED EXAMPLES

### Example 1 — UserController (SECURITY-heavy)
```
hardcoded dbPassword          -> secret code me (CRITICAL)
requestCount++ but unused     -> dead code + thread-unsafe shared field
SQL string-concat (dono query)-> SQL INJECTION (sabse bada)
password plaintext store + LOG-> security
return User (password expose) -> DTO use karo
role == "ADMIN"               -> String == -> .equals()
catch(Exception){printStackTrace} -> swallow
SRP: controller me DB+external+logic -> service layer
RestTemplate new per-call     -> injected bean
```

### Example 2 — OrderService (JAVA-TRAP heavy)
```
static SimpleDateFormat       -> NOT thread-safe -> DateTimeFormatter
findById(...).get()           -> Optional bina check -> orElseThrow
loop me itemRepo.findById     -> N+1 query -> findAllById
double total/price            -> money in double -> BigDecimal (FINANCE!)
FileWriter never closed       -> resource leak -> try-with-resources
getCustomerId() == (Long)     -> reference compare -> .equals() (autobox trap)
catch -> return null          -> exception swallow, caller ko silent null
```

### ★ Example 3 — PaymentService (FINANCE-heavy) — 17-Sep drill, 15 bug
```
KHUD PAKDE (7):
   hardcoded STRIPE_KEY            SECURITY
   Connection close nahi           RESOURCE
   SimpleDateFormat shared         CONCURRENCY
   service me raw JDBC             DESIGN / layering
   field @Autowired                DESIGN
   log.info("..." + req)           SECURITY (card logs me)
   double for money                DB/FINANCE (rounding tak pahuncha)

CHHOOT GAYE (8):
   ★ SQL INJECTION — DO jagah      SECURITY  <- file ka sabse bada bug
   req.getCurrency() == "INR"      JAVA TRAP
   findById(id).get()              JAVA TRAP
   rs.next() unchecked             JAVA TRAP
   loop me findById + save (N+1)   DB
   catch(Exception)+printStackTrace+return "FAILED"   DESIGN
   ★ no @Transactional + baahri charge beech me       FINANCE
   ★ no IDEMPOTENCY -> double charge                  FINANCE
```

### ★ Example 4 — TransferController (paisa transfer) — 18-Sep drill, 15 bug
```
PAKDE (7) — aur teen BHAARI wale jo pichli baar chhoote the, teeno aaye:
   controller me DB query          layering / SRP
   ★ @Transactional nahi           atomicity        <- 17-Sep ko chhoota tha
   ★ SQL injection                  security         <- 17-Sep ko chhoota tha
   constructor injection nahi      design
   log me sensitive data           security
   ★ idempotency nahi               finance          <- 17-Sep ko chhoota tha
   rows.get(0) — khaali list pe crash

CHHOOT GAYE (8):
   ★★ KOI AUTHORIZATION NAHI      fromAcc request-param hai, maalik ka check kahin nahi
                                   -> koi bhi kisi ka bhi account khaali kar sakta
                                   = is file ka SABSE BADA bug
   ★ NEGATIVE AMOUNT             "-5000" -> balance < amt paas -> balance-(-5000) = paisa BANA
   status == "FROZEN"             Object pe == -> hamesha false -> frozen account bhi chal gaya
   notifier.send() DO UPDATE ke BEECH  -> debit + "you received" bheja + credit FAIL
   Float for money                ★ ye 17-Sep ko PAKDA tha, aaj chhoot gaya
   recentTransfers = plain HashMap    4 bug ek saath (detail 2.4 me)
   har failure pe 200 OK          "INSUFFICIENT"/"FROZEN" bhi ResponseEntity.ok
   MAX_RETRIES declare, use nahi  dead code
```

### ★★ Example 5 — RefundController + RefundService — 19-Sep drill, 18 bug
```
PAKDE (11):
   SQL injection                        security
   @Transactional nahi                  atomicity
   field injection                      design
   Connection/PreparedStatement close nahi   resource leak
   SimpleDateFormat static              ★ thread-unsafe (classic Java trap)
   status == "REFUNDED"                 java-trap
   rows.get(0)                          khaali list pe crash
   (double) order.get(...)              blind cast
   ★ idempotency nahi                   Map DIKH raha tha, phir bhi sahi bola
   log me poora request                 security
   ★★ AUTHORIZATION nahi               aur SAHI SHABDON me: "dusri file me ho to theek"

   ★ aakhri DO pichhli baar CHHOOTE the. authz + idempotency — wahi do jinke liye
     MUST-HAVE LIST banayi thi. List ne kaam kiya.

CHHOOTE (7):
   ★ PAISA double me             teen jagah (orderAmount, refundAmount, ps.setDouble)
                                  -> customer_balance me jud raha hai
                                  ★★ DRILL-1 me PAKDA tha, ab DO BAAR lagatar chhoota

   ★ refundAmount pe koi JAANCH nahi
                                  orderAmount DB se nikala, service tak bheja bhi —
                                  aur KABHI COMPARE hi nahi kiya
                                  -> Rs.500 ke order pe Rs.50,000 ka refund nikal jayega
                                  -> negative bhejo to balance + (-5000) = paisa KAT gaya

   ★ gateway call DO DB-write ke BEECH me
                                     status = REFUND_IN_PROGRESS   (DB)
                                     gateway.refund(...)           (BAAHAR — paisa nikal gaya)
                                     INSERT refunds                (DB)  <- yahan crash?
                                  -> paisa customer ko ja chuka, company ke paas RECORD NAHI
                                  ★ ye MUST-HAVE list ka sawaal hai: "beech me crash hua to?"

   catch(Exception) -> ok(...)    refund FAIL hua, API 200 OK bhejti hai
                                  log.error("refund failed") — na orderId na stack trace
                                  ★ ERROR CODE pichhli baar BHI chhoota tha

   refundCache ke andar 4 bug     "idempotency nahi" bolna SAHI tha, par Map me:
                                    HashMap = thread-unsafe
                                    kabhi clear nahi = leak
                                    ★ put() PROCESSING se PEHLE — fail hone pe entry
                                      hamesha ke liye retry BLOCK kar degi
                                    instance field = restart pe gayab, 2 pod pe bekaar

   MAX_REFUND_ATTEMPTS            declare hua, kahin use nahi = dead code

   AUDIT TRAIL nahi               kisne maanga, kisne approve kiya, kab — kuch record nahi
                                  finance me ye COMPLIANCE ki cheez hai

   (bonus: RefundService me dataSource declare/inject hua hi nahi — compile nahi karega)
```

### ★★ Example 6 — DisbursalController + DisbursalService (loan disbursal) — 20-Sep drill, 18 bug

```
PAKDE (11):
   constructor injection nahi           design
   static SimpleDateFormat              java-trap / thread-unsafe
   SQL injection — 4 jagah              security  <- sabse bada
   ★ double har jagah                   MONEY TYPE — drill 2 aur 3 me chhoota tha, AAJ AAYA
   @Transactional nahi                  atomicity
   idempotency nahi                     finance   <- teesri baar lagataar
   status == "APPROVED"                 java-trap
   Connection / Statement / RS close nahi    resource leak
   log me sensitive (aur log line khud ek DB call kar rahi hai)   security
   ★ NEFT call DO DB-write ke BEECH me  KRAM — drill 2 aur 3 me chhoota tha, AAJ AAYA
   ★★ AUTHORIZATION nahi                aur shabd theek the: "kahin likha ho to theek,
                                        yahan nazar nahi aa raha"

   ★ money-type aur kram DONO wo the jo pichhli DO baar chhoote the. List ne kaam kiya.

CHHOOTE (7) — saat me se CHAAR must-have list ki line hain:
   ★ amount pe koi JAANCH nahi      -ve amount -> outstanding BADH jayega, paisa bahar
                                     0 -> NEFT call phir bhi hoga

   ★ amount vs sanctioned_amount     sanctioned DB se NIKALA, variable me rakha,
     kabhi COMPARE nahi hua          aur kabhi use hi nahi kiya
                                     -> 5 lakh sanction, 50 lakh disburse

   ★ har failure pe 200 OK           catch -> ok("FAILED")  ·  ok("ALREADY_IN_PROGRESS")
                                     e.printStackTrace() + log.error("disbursal failed")
                                     — na loanId na stack trace
                                     ★★ ERROR CODE — ab TEEN baar chhoota

   rs.next() ka return unchecked     galat loanId -> rs.getString pe seedha crash

   UPDATE loan_accounts              WHERE me sirf customer_id, loan_id kahin nahi
                                     -> customer ke DO loan hue to GALAT row overwrite
                                     aur outstanding = sanctioned - amount
                                     (purana outstanding padha hi nahi gaya)

   AUDIT trail nahi                  kisne disburse kiya, kab, kitna — koi record nahi

   MAX_DISBURSAL_RETRY               declare hua, use kahin nahi = dead code

★ EK CHEEZ JO PAKDI PAR KHODI NAHI — inFlight Map:
  "idempotency nahi" bolna SAHI tha. Par us Map ke andar chaar alag bug hain —
  HashMap (thread-unsafe) · kabhi clear nahi (leak) · instance field (2 pod pe bekaar) ·
  aur put() kaam se PEHLE, to ek baar fail hone pe wo loanId hamesha ke liye BLOCK.
  -> dikhe to poochho "ye waqai kaam karti hai?" (2.4 wali baat)

★ IS SNIPPET ME JO FAMILY TEST HI NAHI HUI (taaki tasveer honest rahe):
  N+1 · entity/DTO expose · Optional.get() · hardcoded secret — inme se kuch tha hi nahi.
```

## ★ 3.4 — MINDSET (17-Sep ka asli sabak)

```
★ GAP KNOWLEDGE KA NAHI HAI.
  SQL-injection aur money-in-double dono is file me PEHLE SE likhe the.
  Phir bhi chhoot gaye. Matlab file PADHNA kaam nahi karta — CHALANA karta hai.

★ PATTERN jo dikha (asli, gina hua):
     jo cheezein SHAKAL se dikhti hain    ->  pakdi gayi   (7/7 jaisi)
     jo BEHAVIOUR me chhupi hain          ->  chhoot gayi  (8 me se lagbhag saari)
                                              SQLi · crash pe aadha kaam · retry pe double

  -> ilaaj = HISSA 1 ke 5 kadam, khaas kar teen sawaal aur MUST-HAVE list.

- ye tera sabse SASTA round hai — 700 ticket ka PR-review pehle se kiya hua hai.
  Naya seekhna nahi hai, sirf KRAM se dekhna hai.
- checklist-skill FINITE hai -> kuch drill me "7 pakda" se "12-13 pakda" ho jaayega.
- drill: alternate din 1 snippet -> 30 sec padho -> teen sawaal -> 5-bucket -> must-have -> bolo.
```

---

[← Home README](../README.md)
