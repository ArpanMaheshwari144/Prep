# PR / CODE-REVIEW ROUND — CHECKLIST + DRILL (JP round-2 ka hissa)

> JP/finance-Java round-2 me buggy Spring-Boot code deke poochte: "kya galat? kaise improve?"
> Ye ek CHECKLIST-skill hai (finite, ~20 recurring issue) — knowledge already hai, sirf SYSTEMATIC scan ki aadat.
> Har mock se pehle 30-sec glance -> phir scan. Reps se automatic.

---

## ★ 5-BUCKET CHECKLIST (har PR snippet inhi me se aata)

```
1. SECURITY
   [] SQL injection      -> string-concat query ("...'" + x + "'")? -> parameterized (?)
   [] hardcoded secret   -> password / API-key / URL code me? -> env / vault / config
   [] password handling  -> plaintext store? (BCrypt) · logs me print? (mat karo)
   [] sensitive expose   -> entity seedha return (password-field ke saath)? -> DTO
   [] input validation   -> user-input bina validate use ho raha?

2. JAVA TRAPS (JP round-2 ye HUNT karta)
   [] SimpleDateFormat   -> shared/static? -> NOT thread-safe -> DateTimeFormatter (immutable)
   [] Optional.get()     -> bina check .get()? -> orElseThrow / handle
   [] == on boxed        -> Long / Integer / String == compare? -> .equals()
   [] mutable shared field -> non-final instance/static field (thread-unsafe)?
   [] resource cleanup   -> (neeche bucket-3)

3. RESOURCE
   [] stream/writer/conn -> close hota? -> try-with-resources
   [] connection / thread / file-handle leak

4. DATABASE
   [] N+1 query          -> loop me findById? -> findAllById / join (batch fetch)
   [] @Transactional     -> multi-write bina transaction (atomicity)?
   [] double for money   -> currency double me? -> BigDecimal (rounding-error)

5. DESIGN / ERROR-HANDLING
   [] SRP violation      -> controller me DB + business + external sab? -> service layer
   [] exception swallow  -> catch{} khaali / e.printStackTrace() / return null
   [] magic numbers / hardcoded values
   [] field @Autowired   -> constructor injection better (testable, final)
```

---

## ★ INTERVIEW ME KAISE BOLNA (structured = impress)

```
SCAN ORDER:  security -> java-trap -> resource -> db -> design/error-handling
BOLNE KA TARIKA (loud, structured):
   "Main pehle SECURITY dekhta hoon -- yahan SQL string-concat se ban rahi, injection risk...
    phir CONCURRENCY -- ye static SimpleDateFormat thread-safe nahi...
    phir RESOURCE -- ye writer close nahi ho raha...
    phir DB -- ye loop me N+1 query..."
-> structured review = interviewer ko dikhta tu SYSTEMATIC hai, andaze se nahi.
```

---

## ★ WORKED EXAMPLES (revise ke liye — 7-Sep drill)

### Example 1 — UserController (SECURITY-heavy)
```
Issues:
  hardcoded dbPassword          -> secret code me (CRITICAL)
  requestCount ++ but unused    -> dead code + thread-unsafe shared field
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
Issues:
  static SimpleDateFormat       -> NOT thread-safe -> DateTimeFormatter
  findById(...).get()           -> Optional bina check -> orElseThrow
  loop me itemRepo.findById     -> N+1 query -> findAllById
  double total/price            -> money in double -> BigDecimal (FINANCE!)
  FileWriter never closed       -> resource leak -> try-with-resources
  getCustomerId() == (Long)     -> reference compare -> .equals() (autobox trap)
  catch -> return null          -> exception swallow, caller ko silent null
```

---

## ★ MINDSET
```
- ye tera sabse FIXABLE gap -> underlying Java knowledge already hai, sirf SCAN-aadat.
- checklist-skill = finite -> kuch reps me "1-2 pakda" se "zyada pakda".
- PR-review = round-2 ka ek HISSA (Java-Q + bug-fix ke saath), akela poora-reject shaayad hi.
- roz/alternate din 1 snippet drill -> 5-bucket scan -> automatic ho jaayega.
```

---

[← Home README](../README.md)
