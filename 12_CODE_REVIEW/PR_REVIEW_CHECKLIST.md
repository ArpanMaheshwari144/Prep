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

KADAM 4 — BOLO structured: security -> java-trap -> resource -> db -> design.
```

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
