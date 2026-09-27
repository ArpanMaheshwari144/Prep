# PR REVIEW — JP SUPERDAY (asli round ke hisaab se, 27-Sep)

> Purani PR_REVIEW_CHECKLIST.md 27-Sep ko hataayi (git history me hai). Wo paise wale gehre bug
> (race / idempotency / transaction) pe tiki thi — asli JP round me jo aata, wo usme peeche daba tha.
> Ye file NET pe candidates ne jo bataya, USI hisaab se bani hai.

---

## 0. ASLI ROUND KYA HAI (source ke saath)

```
KAB / KITNA    Superday ka coding slot: ~10 MINUTE ek asli PR review, phir LC-medium live
               (techinterview.org, interviewquery)
KAUN           senior developer lete hain (Blind)
JP SDE-2 (3 aur experience, 27-Sep Arpan ne diye): teeno me code review ALAG round / hissa tha —
               "working par ganda code", clean code · OOP · STATIC / VOLATILE · maintainability ·
               "production me push kar sakte ho?"
KYA DHOONDHNA  "bugs and style problems"
PR ME KYA THA  (candidates ne bataya)
                 exception handling · variable ke naam · logging · println
                 hardcoded credentials · poor variable names · volatile (Java concurrency)
                 (Blind superday post, interviewquery)
4 DABBE        correctness · code quality · design · extensibility
               "perfect jawab nahi — tu software quality ke baare me KAISE sochta hai"
               (JP SDE-2 experience, devbrainiac)
```

**Ek line me:** PR ek colleague ka code hai. Tu senior reviewer hai. Pehle bada (bug / security),
phir style — aur har baat **samjha ke, politely** bolo.

Sources: techinterview.org/companies/jpmorgan · interviewquery.com JP SWE guide ·
teamblind.com "jp morgan chase superday" · devbrainiac.com JPMorgan SDE-2 (3 rounds)

---

## 1. 10 MINUTE KA RAASTA

```
0:00  CONTEXT POOCHO     "Ye PR kya karne ke liye hai? kaunsi business problem?"
                         (samjhe bina line-by-line mat kood)
0:30  EK BAAR PADHO      upar se neeche, chup — "code kya kar raha hai?"
1:30  BOLTE HUE REVIEW   neeche ke 4 DABBE, isi kram me:
                           1 CORRECTNESS   -> chalega bhi? crash / galat nateeja?
                           2 SECURITY      -> kuch leak / hack to nahi?
                           3 CODE QUALITY  -> padhne / sambhalne layak?
                           4 DESIGN        -> sahi jagah, sahi tukde?
8:00  PAISA WALA PR ho   -> dabba 5 (bonus) pe ek nazar
9:00  SAMET DO           "sabse bhaari 2-3 ye hain, baaki chhote"
```
★ Jo dikha SAB bolo, chhota lage tab bhi. Dimaag me pakda par bola nahi = gina nahi jaata.

---

## 2. DABBA 1 — CORRECTNESS (chalega bhi? crash / galat nateeja?)

```
CODE ME DIKHE                          POOCHO / BOLO                              SAHI
-----------------------------------------------------------------------------------------------------
catch (Exception e) { }                "exception nigal gaya — galti dikhegi       log + rethrow / sahi
  / printStackTrace() / return null      hi nahi"                                   exception / HTTP code
catch (Exception e) — sab ek saath     "bahut chauda catch — kaunsi galti?"        specific exception
fail pe ok("FAILED") / 200             "fail hua par 200 OK?"                      400 / 404 / 409 / 500
findById(..).get()                     "khaali Optional -> crash"                  orElseThrow(NotFound)
list.get(0) / rows.get(0)              "khaali list -> crash"                      isEmpty check
rs.next() ka return nahi dekha         "row nahi mili -> crash"                    if (!rs.next())
obj.method() bina null check           "null aaya to NPE"                          Optional / null check
String pe ==  (status == "DONE")       "reference compare -> galat"                "DONE".equals(x)
Integer / Long pe ==                   "127 ke upar fail"                          Objects.equals
(double) map.get(..)                   "null / galat type -> crash"                typed / check
request ka number (amount · qty)       "-ve / 0 / bahut bada bheju to?"            validation (@Valid,
                                                                                    @Positive, range)
DB se value nikali, use nahi hui       "ye kyun nikali? compare honi thi?"         use karo / hatao
Connection / Stream / File khola       "close kahan?" (raw JDBC)                   try-with-resources
                                        (JdbcTemplate / JPA khud sambhalta — ye bug NAHI)
LOOP me DB call                        "N+1 — 100 row = 101 query"                 batch / IN / join
list lautaye, pagination nahi          "10 lakh row?"                              Pageable
```

**Concurrency (JP ne "volatile" poocha tha):**
```
controller / service (singleton) me    "saare thread share karte — thread-safe?"   local / Concurrent* /
  field: Map · List · int counter                                                   AtomicInteger
counter++ shared field pe              "++ atomic nahi — ginti kho jaayegi"        AtomicInteger
boolean flag ek thread set, doosra     "doosre thread ko dikhega?" -> volatile     volatile / Atomic*
  padhe (bina volatile)                 (visibility, atomicity nahi deta)
static MUTABLE field (static List /     "saari app me EK copy, sab thread share —   instance / local /
  static int count / static Map cache)    thread-safe? test me ek doosre ko gande"   Concurrent* / bean
static SimpleDateFormat                "thread-safe nahi"                          DateTimeFormatter
```

---

## 3. DABBA 2 — SECURITY (kuch leak / hack to nahi?)

```
CODE ME DIKHE                          POOCHO / BOLO                              SAHI
-----------------------------------------------------------------------------------------------------
password / API key / URL code me       "HARDCODED CREDENTIALS" (JP me aaya tha)   env / vault / config
  "sk_live_..." · "admin123"
SQL string me  " + "                   "SQL injection" — POORI string padho        PreparedStatement (?)
log me email / card / account /        "log me sensitive data"                     masked / sirf id
  password / token
return ok(ENTITY)                      "saare field bahar — password?"             DTO
password seedha save                   "plaintext"                                 BCrypt
id request se (orderId · userId)       "bhejne wala iska MAALIK hai?"              token se owner check
  — owner check nahi                    (userId BODY se = koi bhi kuch bhi bheje)
```

---

## 4. DABBA 3 — CODE QUALITY (padhne / sambhalne layak?)  ← JP me sabse zyada yahi

```
CODE ME DIKHE                          POOCHO / BOLO                              SAHI
-----------------------------------------------------------------------------------------------------
naam: a · b · temp · data · x ·        "POOR VARIABLE NAMES" (JP me aaya tha)      kaam batane wala naam
  flag · obj · list1 · doIt()                                                       (refundAmount, isActive)
System.out.println                     "println — production me logger"           SLF4J log.info / debug
log sahi level pe nahi                 "error ko info me? debug ko info me?"      sahi level
  / log.error("failed") bina e aur id   "stack trace + kaunsi id?"                 log.error("... id={}", id, e)
magic number / string  (0.01 · 5 ·     "ye 5 kya hai? naam do"                     named constant / config
  "ACTIVE" baar-baar)
lamba method (sab kuch ek me)          "method bahut kaam kar raha"                chhote method
copy-paste code do jagah               "duplicate — ek jagah badla, doosri bhooli" ek method
dead code (field / constant /          "declare hua, use nahi"                     hatao (ya niyam lagao)
  import use nahi)
gehri nesting (if ke andar if ...)     "padhna mushkil"                            early return
comment galat / purana / bekaar        "comment code se mel nahi khata"            sahi / hatao
```

---

## 5. DABBA 4 — DESIGN (sahi jagah, sahi tukde?)

```
CODE ME DIKHE                          POOCHO / BOLO                              SAHI
-----------------------------------------------------------------------------------------------------
@Autowired FIELD pe                    "constructor injection — final, testable"  constructor + final
controller me SQL / business logic     "SRP — layer alag"                          controller -> service
                                                                                    -> repository
ek class sab kuch kar rahi             "Single Responsibility tooti"              tod do
new RestTemplate() / new Service()     "tight coupling, test nahi hoga"            bean inject
  andar
if-else chain type pe (if "UPI" ...    "naya type = ye method badlo (Open/Closed)" strategy / polymorphism
  else if "CARD" ...)
concrete class pe depend               "interface pe depend karo"                 interface
```
**Extensibility:** "kal naya type / naya rule aaya to kitni jagah badlega?" — ek line zaroor bolo.

---

## 6. DABBA 5 (BONUS) — PAISA / DATA BADALNE WALA PR ho to

> Ye kisi candidate ne JP round me report NAHI kiya — par PR paise ka ho to ek nazar.
> Waqt bache tab. Pehle dabba 1-4.

```
double / float me paisa                "rounding"                                  BigDecimal / long paise
do write, @Transactional nahi          "beech me fail -> aadha likha"              @Transactional
@Transactional private / andar se call "laga hi nahi (proxy)"                      public, doosri bean se
do write ke beech bahar ka call        "bahar gaya, DB rollback — wapas nahi aata" call bahar + PENDING row
padho -> ghatao -> save                "do ek saath -> double spend"               WHERE x >= ? / @Version
retry pe do baar                       "idempotency?" (HashMap = nahi)             DB unique key
```

---

## 7. BOLNE KA TARIKA (politely, samjha ke)

```
ILZAAM NAHI, SUJHAAV:
  GALAT:  "ye code bekaar hai, fail hoga"
  SAHI:   "Yahan catch me exception log nahi ho raha, to production me pata hi nahi chalega
           kya toota. Main isse log karke sahi HTTP code lautaunga."

GAYAB CHEEZ PE SAWAAL:
  "Yahan owner check nazar nahi aa raha — kya ye kisi filter / @PreAuthorize me hai?"

KRAM:   bhaari pehle (crash / security) -> phir quality -> phir design
        "Sabse pehle do bade issue: hardcoded password aur SQL injection. Phir kuch chhote:
         naam, println, lamba method..."

★ AAKHIR ME FAISLA (JP me seedha poochha gaya: "ye production me ja sakta hai?"):
  "Abhi nahi — pehle ye 2-3 bhaari (creds, injection, exception) theek ho. Baaki (naam, println,
   magic number) isi PR me ya follow-up me."
★ Soch bolo, sirf nateeja nahi: "Main ye isliye keh raha hoon ki..."
★ Perfect nahi chahiye — soch dikhni chahiye.
```
