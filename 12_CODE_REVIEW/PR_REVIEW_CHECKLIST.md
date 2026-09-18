# PR / CODE-REVIEW ROUND — CHECKLIST + SCAN PROTOCOL + DRILLS

> **CONFIRMED (17-Sep, bahar ke source se — andaaza nahi):** JP ke Superday me
> **~10 minute ka asli PR-review segment hota hai** — ek pull request deke bug + style poochte hain.
> Ye alag round nahi, Superday ka hissa hai. (techinterview.org JPMorgan guide + 5 candidate writeups.)
>
> Ye CHECKLIST-skill hai (finite, ~20 recurring issue). **Knowledge pehle se hai — aadat SCAN ki chahiye.**

---

## ★★ SCAN PROTOCOL — "agli baar theek se pelne" ke liye

> **17-Sep ka sabak:** checklist me SQL-injection aur money-in-double dono LIKHE the.
> Drill me dono chhoot gaye. Matlab file padhna kaafi nahi — **file CHALANI** padti hai.
> Isliye ab ye protocol. Ratna nahi — bas har PR pe yahi kram.

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

KADAM 3 — PHIR 5-bucket list pe ek sweep (neeche). Jo (a)(b)(c) me nahi aaya, wo yahan milega.

★★ KADAM 4 — MUST-HAVE LIST  (18-Sep, Arpan-pakdi — ye sabse zaroori kadam hai)

   Kadam 1-3 tak jo kiya wo HUNT tha — galat likhi hui line dhoondhna.
   Par do bilkul alag kism ke bug hote hain:

      GALAT LIKHA HUA   ->  line maujood hai, wo galat hai
                            (SQL concat · Float · == · get(0))
                            -> AANKH pakad leti hai. Hunt kaam karta hai.

      LIKHA HI NAHI     ->  koi line hai hi NAHI
                            (auth · validation · error-code)
                            -> aankh KABHI nahi pakdegi. Dekhne ko kuch hai hi nahi.

   ★ AUR YE JAAN-KAARI ka mamla NAHI hai (Arpan ne saaf kiya):
      "auth to seekha hua hai — par FILE SCAN karte waqt uske liye DEKHNA
       kabhi seekha hi nahi."
      Matlab gap gyaan me nahi, SCAN ki aadat me hai. Aur aadat list se banti hai.

   -> gayab cheez HUNT se nahi milti. Uske liye LIST chahiye. Ginni padti hai:

   Paise/data BADALNE wale kisi bhi endpoint pe — har line pe tick ya cross:
   ```
   [ ] AUTHORIZATION  bulane wale ka HAQ hai is resource pe?
   [ ] VALIDATION     amount > 0? account exist? khud ko khud transfer?
   [ ] TRANSACTION    multi-write ek saath commit/rollback?
   [ ] IDEMPOTENCY    dobara chala to? (aur jo hai wo SACH me kaam karta hai?)
   [ ] AUDIT          kaun, kab, kitna — record hua?
   [ ] ERROR CODE     fail pe 4xx? ya sab 200 OK ja raha?
   ```

KADAM 5 — BOLO structured: security -> java-trap -> resource -> db -> design.
```

> ★★ **AUTH ka SAHI FARAK (18-Sep, Arpan-pushback se nikla — ye maaf karne wali wajah BHI hai):**
> ```
> AUTHENTICATION   "kaun ho tum?"
>                  -> SecurityConfig / JwtFilter me, EK jagah, saalon pehle likha
>                  -> controller me dikhta hi NAHI, aur dikhna bhi nahi chahiye
>                  -> isliye "controller me auth nahi hai" sochna ULTA lagta hai. Sahi lagta hai.
>
> AUTHORIZATION    "is ACCOUNT / ORDER pe tumhara HAQ hai?"
>                  -> GENERIC filter ye nahi kar sakta — usne `fromAcc` dekha hi nahi,
>                     wo request me ABHI aaya hai
>                  -> par ye handler me HI ho aisa zaroori NAHI:
>                     @PreAuthorize("#fromAcc == authentication.name") · service-layer
>                     check · ek aspect — teeno jayaz jagah hain
>                  ★ matlab: is snippet me wo KAHIN nahi dikh raha — aur PR-review me
>                    itna hi bolna hai: "yahan ownership-check nazar nahi aa raha,
>                    kya wo kisi aspect/@PreAuthorize me hai?" — ye SAWAAL hai, ilzaam nahi.
> ```
> ★ **PAKADNE ka NISHAAN** (yaad rakhne se bahut aasan — ye ek SHAKAL hai):
> ```
> jab bhi REQUEST-PARAM me kisi CHEEZ ka naam aaye —
>      accountId · orderId · userId · documentId
> turant poochho: "ye cheez BULANE WALE ki hai — ye kahan check ho raha?"
>
> @RequestParam String fromAcc    <- yahin ruk jaana tha
> ```

> **★ KYUN teen sawaal PEHLE, checklist BAAD me:** checklist **shakal** pakadti hai
> (hardcoded string, naya object, missing close) — wo aankh se dikh jaati hai.
> Teen sawaal **behaviour** pakadte hain (runtime pe, crash pe, retry pe kya hoga) —
> wo aankh se DIKHTA HI NAHI. 17-Sep ko jo 8 chhoote, wo lagbhag saare BEHAVIOUR wale the.

---

## ★ 5-BUCKET CHECKLIST (har PR snippet inhi me se aata)

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

5. DESIGN / ERROR-HANDLING
   [] SRP violation      -> controller/service me DB + business + external sab? -> layer alag
   [] raw JDBC in service-> repository ka kaam service me ghusa hua
   [] exception swallow  -> catch(Exception){} · e.printStackTrace() · return null/"FAILED"
   [] magic numbers / magic string return values
   [] field @Autowired   -> constructor injection better (final + testable)

6. ★ FINANCE-FLAVOUR (JP/GS/Barclays ke liye — ye list me hona hi chahiye)
   [] BAAHRI CALL transaction ke ANDAR
        stripe.charge() -> phir DB update fail
        = paisa asli duniya me kat gaya, DB me kuch nahi. Rollback ho hi nahi sakta.
        -> DB kaam ek transaction me · baahri call USSE BAHAR · phir reconcile/retry
   [] IDEMPOTENCY nahi   -> user ne do baar dabaya / network retry -> DOUBLE CHARGE
                            -> idempotency key (04_HLD/07_payment_system wali baat)
   [] AUDIT trail        -> paise wala kaam bina record ke? kaun, kab, kitna
   [] consistency > availability — paise me ye default jhukaav hai
```

---

## ★ INTERVIEW ME KAISE BOLNA (structured = impress)

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
```

---

## ★ WORKED EXAMPLES

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

### ★ Example 3 — PaymentService (FINANCE-heavy) — asli drill, 15 bug
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
   recentTransfers = plain HashMap    4 bug ek saath:
                                      thread-unsafe · kabhi clear nahi (leak) ·
                                      key me amount/time nahi (dobara bhejna HAMESHA blocked) ·
                                      instance field (2 pod = kaam hi nahi karega)
                                      = idempotency ka DIKHAWA. "nahi hai" bolna SAHI tha.
   har failure pe 200 OK          "INSUFFICIENT"/"FROZEN" bhi ResponseEntity.ok
   MAX_RETRIES declare, use nahi  dead code
```

### ★★ AAJ KE 8 MISS KA NISHAAN — "code me kya DIKHA tha, aur kya POOCHNA tha"

> Miss ki list padhne se kuch nahi hota. Har miss ke saamne ek cheez code me SAAF dikh rahi thi.
> Agli baar wo cheez dikhe -> sawaal apne-aap chalna chahiye. Yahi ratt-ne wali table hai.

```
NISHAAN (jo aankh ke saamne tha)          ->  SAWAAL (jo turant chalna chahiye)
-----------------------------------------------------------------------------------------
request me kisi ki CHEEZ ka naam              "bhejne wala iska MAALIK hai? check kahan?"
(fromAccount · orderId · userId)              (filter me ho hi nahi sakta — usne ye param
                                               dekha hi nahi. @PreAuthorize/service/aspect
                                               me ho sakta -> POOCHO, ilzaam mat lagao)

request se aaya NUMBER, aur usse              "iski HAD kahan check hui?
ghatao/jodo ho raha                            -ve? 0? bahut bada? decimal?"
                                               (-5000 -> balance-(-5000) = paisa BANA)

Object/String pe  ==                          "ye reference jod raha ya value?"
                                               (status=="FROZEN" -> hamesha false ->
                                                frozen account bhi nikal gaya)

do DB-update ke BEECH me koi BAAHAR ka call   "agla step fail hua to jo BAHAR ja chuka
(mail · notification · 3rd-party API)          use wapas laa sakte ho?"
                                               (debit -> "paisa aaya" bhej diya -> credit fail)

float / double + paisa                        "ye paisa hai -> BigDecimal"
                                               ★ ye 17-Sep ko PAKDA tha, 18-Sep ko CHHOOT gaya.
                                               Ek baar pakad lene se ye yaad nahi rehta —
                                               isi liye ye LIST me hai, yaad me nahi.

controller/service me koi MUTABLE field       "ye har request me SAANJHA hai —
(Map · List · counter · flag)                  thread-safe? kabhi khaali hota? 2 pod pe chalega?"
                                               (recentTransfers = idempotency ka DIKHAWA)

ResponseEntity.ok  failure waali branch me    "is haalat ka HTTP code kya hona chahiye?
                                                400 / 409 / 422?"
                                               (INSUFFICIENT bhi 200 OK ja raha tha)

koi constant/field DECLARE hua                "ye use kahan hua?" — kahin nahi = dead code
                                               (MAX_RETRIES)
```

```
★ EK BAAT JO IS DRILL NE SAAF KI:
  "idempotency nahi hai" bolna SAHI tha — jabki Map maujood tha.
  Kyunki wo Map kaam hi nahi karta (thread-unsafe · kabhi clear nahi · key adhoori · instance field).
  -> MAUJOOD HONA aur KAAM KARNA do alag baat hai. Cheez dikhe to ruk ke poochho
     "ye waqai wo kaam karti hai jiske liye rakhi hai?" — dikh gayi, tick laga diya, ye galti hai.
```

### ★ 17-Sep vs 18-Sep — ginti wahi, MAAL badal gaya

```
17-Sep   7 / 15      zyadatar SHAKAL wale (hardcoded string · missing close · field-injection)
18-Sep   7 / 15      ab BEHAVIOUR wale (transaction · idempotency · injection) — teeno naye

-> KISM behtar hui, ginti nahi badli.
   ★ par ye SAAF NAAP nahi hai — do PR alag the, mushkil bhi alag thi.
     "protocol kaam kar gaya" tabhi kehna jab TEESRI drill bhi behaviour-wale bug de.
     Abhi tak ka sach: 2 drill, dono 7/15.
```

### ★ NAYA PATTERN jo 18-Sep ko dikha

```
STRUCTURE ke bug       ->  PAKAD raha   (layering · injection · transaction · idempotency)
BUSINESS-LOGIC ke bug  ->  CHHOOT raha  (authz · negative amount · frozen-check · call ka order)

-> chaaron BUSINESS wale "likha hi nahi" ya "galat niyat se chalao" kism ke hain
-> isi liye KADAM 4 (MUST-HAVE LIST) joda gaya. Aaj ke 8 miss me se TEEN
   seedha usi list se pakde jaate: authz · validation · error-code.
```

---

## ★ MINDSET — 17-Sep ka asli sabak

```
★ GAP KNOWLEDGE KA NAHI HAI.
  SQL-injection aur money-in-double dono is file me PEHLE SE likhe the.
  Phir bhi chhoot gaye. Matlab file PADHNA kaam nahi karta — CHALANA karta hai.

★ PATTERN jo dikha (asli, gina hua):
     jo cheezein SHAKAL se dikhti hain    ->  pakdi gayi   (7/7 jaisi)
     jo BEHAVIOUR me chhupi hain          ->  chhoot gayi  (8 me se lagbhag saari)
                                              SQLi · crash pe aadha kaam · retry pe double

  -> ilaaj = upar wala SCAN PROTOCOL, khaas kar teen sawaal.
     (a) user ka data kahan ja raha  (b) crash pe kya chhootega  (c) do baar chala to

- ye tera sabse SASTA round hai — 700 ticket ka PR-review pehle se kiya hua hai.
  Naya seekhna nahi hai, sirf KRAM se dekhna hai.
- checklist-skill FINITE hai -> kuch drill me "7 pakda" se "12-13 pakda" ho jaayega.
- drill: alternate din 1 snippet -> 30 sec padho -> teen sawaal -> 5-bucket -> bolo.
```

---

[← Home README](../README.md)
