# PR REVIEW — JP SUPERDAY

> **Round me sirf section 1 (SHIKAAR LIST) chahiye.** Baaki file = samajh / reference.
>
> 28-Sep sudhaar: pichhli file 4 dabbon ki lambi table thi (~60 line). Drill me 17 me se 14 point
> file me LIKHE the, phir bhi 10 chhoote — kyunki lambi table 10 minute me dimaag me scan nahi hoti.
> Isliye ab sabse upar ek chhoti SHIKAAR LIST hai: code me kya DHOONDHNA hai, kis kram me.

---

## 0. ASLI ROUND KYA HAI (source ke saath)

```
KAB / KITNA    Superday: ~10 MINUTE ek asli PR review, phir LC-medium live (techinterview.org, interviewquery)
KAUN           senior developer (Blind)
KYA DHOONDHNA  "bugs and style problems"
PR ME KYA THA  exception handling · variable naam · logging · println · hardcoded credentials ·
               volatile (Blind superday post, interviewquery)
JP SDE-2       "working par ganda code" · clean code · OOP · STATIC / VOLATILE · maintainability ·
               "production me push kar sakte ho?" (3 experience, 27-Sep; devbrainiac)
```
Sources: techinterview.org/companies/jpmorgan · interviewquery.com JP SWE guide ·
teamblind.com "jp morgan chase superday" · devbrainiac.com JPMorgan SDE-2

---

## 1. ★ SHIKAAR LIST — code me YE dhoondho, ISI kram me

> Line-by-line padh ke jo dikhe wo bolna = aadhe chhoot-te hain (28-Sep drill ka data).
> Tareeka: code pe **3 baar nazar**, har baar ALAG cheez dhoondho.

### NAZAR 1 — BHAARI (~4 min). Har ek pe code me ye SHABD dhoondho:

```
 #   CODE ME DHOONDHO                     DIKKAT                          ASAR (ye bhi bolna)
---  -----------------------------------  ------------------------------  ----------------------------------
 1   SQL string me  + "   ya  '" + x + "' SQL INJECTION                   koi bhi data padh / mita de
     (har query pe, EK bhi mat chhodo)                                    -> PreparedStatement + ?
 2   "password" · "admin" · "sk_live" ·   HARDCODED CREDS                 git me prod password
     jdbc:... url string me                                               -> env variable / Vault
 3   getConnection · new FileReader ·     CLOSE kahan? + LOOP me khul     connection khatam, DB/app girega
     openStream · createStatement         raha to aur bura               -> try-with-resources / pool / JdbcTemplate
 4   LOOP ke andar  query / HTTP / repo   N+1                             1 lakh row = 1 lakh query
     call (seedha ya kisi method se)                                      -> ek JOIN / IN / batch
     SELECT * bina WHERE, phir Java me if  poori table memory me          -> WHERE DB me lagao
 5   catch                                nigla? sirf println? chauda     prod me pata hi nahi kya toota
                                          (Exception)? return false/null? -> log.error("..id={}", id, e) + specific
 6   static  (jo final nahi)              saari app me EK copy, sab       thread-safe nahi, kabhi saaf nahi,
     service/controller ka field          thread share                    purane run ka data -> local / Concurrent*
 7   boolean flag / counter jo doosra     volatile nahi = dikhega nahi;   -> volatile (dikhne ke liye)
     thread padhe / badhaye               ++ atomic nahi; check-then-set  -> AtomicInteger / AtomicBoolean.compareAndSet
 8   ==  String / Integer / Long pe       reference compare               ASAR bolo: "if kabhi true nahi,
                                                                          poori job chup-chaap kuch nahi karegi"
 9   double / float  me amount            paisa rounding                  -> BigDecimal / long paise
10   log / println me  email · card ·     PII / secret log me             log leak = data leak -> sirf id / mask
     password · token
11   request se id / amount               owner check? validation?        koi bhi kisi ka data / -ve amount
     ★ HAR {id} wale method pe ALAG se    ek method me check hai to baaki  owner nahi -> 403 (chup "ok" nahi)
     poocho "ye kiska hai?"               me bhi hai, ye maan mat lena
```

### ★ EK LINE PE EK PAKDA, TO RUKO MAT (3-Oct data se)

```
3-Oct me jo chhoota, wo usi line pe tha jahan ek bug pehle se pakda tha:
  line 28  .get() pakda        -> wahi line LOOP me thi = N+1 chhoota
  line 26  == pakda            -> "DUE" magic chhoota
  line 30  BigDecimal pakda    -> 0.02 magic + naam tmp chhoota
Ek bug milte hi dimaag agli line pe chala jaata hai.

ILAAJ: har line pe bug pakadne ke baad wahin 3 sawaal aur:
  (1) ye LOOP ke andar hai?          -> DB / HTTP call = N+1 · String + = StringBuilder
  (2) koi "..." ya number likha hai? -> magic -> enum / constant
  (3) naam kaam batata hai?          -> x · tmp · doIt · data

LOOP dikhe to upar bhi dekho: loop se pehle query SAB utha rahi hai? (SELECT * bina WHERE)
SHAKAL yaad rakho:  if (flag) return;  flag = true;   = CHECK-THEN-ACT -> AtomicBoolean.compareAndSet
                    (shaq hua to NAAM bolo, naam bola tabhi gina jaata hai)
```

### NAZAR 2 — STYLE (~3 min). Ek-ek naam lo:

```
naam           a · b · x · e · st · temp · data · doIt()     -> kaam batane wala naam
println        production me                                  -> SLF4J logger, sahi level
magic          "ACTIVE" · 5 · 0.01 baar-baar                   -> enum / constant
String +       loop ke andar  out = out + ...                  -> StringBuilder
finally        cleanup (flag reset / close) try ke baad        -> finally / try-with-resources
SRP            ek class: DB + logic + mail/HTTP sab           -> repository / service / client alag
injection      @Autowired field pe · new XService() andar     -> constructor + final, bean inject
bekaar         nikala par use nahi (list / field / variable)  -> use karo ya hatao
lamba method / gehri nesting                                   -> chhote method, early return
return type    ek method String, doosra BigDecimal, teesra    -> sab ResponseEntity + sahi status
               ResponseEntity
```

### NAZAR 3 — SAMET DO (~1 min). YE LINE HAMESHA:

```
"Production me abhi nahi jaana chahiye — sabse bhaari: <1> , <2> , <3>.
 Baaki (naam, println, magic string) isi PR me ya follow-up me."
```

---

## 2. 10 MINUTE KA RAASTA

```
0:00  CONTEXT POOCHO   "Ye PR kya karta hai?" (samjhe bina line pe mat kood)
0:30  EK BAAR PADHO    upar se neeche, chup — code kya kar raha hai
1:30  NAZAR 1          bhaari (shikaar list 1-11)
5:30  NAZAR 2          style
8:30  NAZAR 3          faisla line
```
★ Jo pakda wo BOLO. Dimaag me pakda par bola nahi = gina nahi jaata.
★ Har bhaari point pe ASAR ek line me: "isse kya hoga". Sirf "== galat hai" aadha jawab hai.

---

## 3. REFERENCE — samajh ke liye (round me nahi, padhne me)

### 3a. Correctness
```
CODE ME DIKHE                          POOCHO / BOLO                              SAHI
-----------------------------------------------------------------------------------------------------
catch (Exception e) { } / return null  "exception nigal gaya"                     log + rethrow / sahi code
catch (Exception e) — sab ek saath     "bahut chauda catch — kaunsi galti?"       specific exception
(double) map.get(..)                   "null / galat type -> crash"               typed / check
fail pe ok("FAILED") / 200             "fail hua par 200 OK?"                     400 / 404 / 409 / 500
findById(..).get()                     "khaali Optional -> crash"                 orElseThrow(NotFound)
list.get(0) / rows.get(0)              "khaali list -> crash"                     isEmpty check
rs.next() ka return nahi dekha         "row nahi mili -> crash"                   if (!rs.next())
obj.method() bina null check           "null aaya to NPE"                         Optional / null check
String == · Integer/Long ==            "reference compare" (Integer 127 ke upar)  equals / Objects.equals
request ka number (amount · qty)       "-ve / 0 / bahut bada?"                    @Valid, @Positive
DB se value nikali, use nahi hui       "compare honi thi?"                        use karo / hatao
Connection / Stream / File             "close kahan?" (raw JDBC)                  try-with-resources
                                        (JdbcTemplate / JPA khud sambhalta — bug NAHI)
LOOP me DB call                        "N+1"                                      batch / IN / join
list lautaye, pagination nahi          "10 lakh row?"                             Pageable
```

### 3b. Concurrency (JP ne "volatile" / "static" poocha tha)
```
singleton (controller/service) me      "saare thread share karte — thread-safe?"  local / Concurrent* / Atomic*
  field: Map · List · counter
counter++ shared field pe              "++ atomic nahi"                           AtomicInteger
flag ek thread set, doosra padhe       "doosre ko dikhega?"                        volatile (sirf dikhna, atomic nahi)
if (!running) { running = true; ... }  "do thread dono andar"  (check-then-act)   AtomicBoolean.compareAndSet
static MUTABLE field                   "EK copy, sab share, kabhi saaf nahi"      instance / local / bean
static SimpleDateFormat                "thread-safe nahi"                         DateTimeFormatter
```

### 3c. Security
```
password / API key / URL code me       HARDCODED CREDENTIALS                      env / Vault
SQL string me " + "                    SQL injection — POORI string padho         PreparedStatement (?)
log me email / card / token            sensitive data log me                      mask / sirf id
return ok(ENTITY)                      "saare field bahar — password?"            DTO
password seedha save                   plaintext                                  BCrypt
id request se, owner check nahi        "bhejne wala iska maalik hai?"             token se owner check
```

### 3c2. Code quality (padhne / sambhalne layak?) ← JP me sabse zyada yahi

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

### 3d. Design
```
@Autowired field pe                    constructor injection — final, testable    constructor + final
controller me SQL / business logic     layer alag                                  controller -> service -> repo
ek class sab kuch                      Single Responsibility                       tod do
new RestTemplate() / new Service()     tight coupling, test nahi hoga             bean inject
if-else chain type pe ("UPI" / "CARD") naya type = ye method badlo (Open/Closed)  strategy
concrete class pe depend               interface pe depend                        interface
```
**Extensibility:** "kal naya type / rule aaya to kitni jagah badlega?" — ek line zaroor.

### 3e. BONUS — paisa / data badalne wala PR ho to (JP report me nahi aaya, waqt bache tab)
```
do write, @Transactional nahi          beech me fail -> aadha likha               @Transactional
@Transactional private / self-call     proxy -> laga hi nahi                      public, doosri bean se
do write ke beech bahar ka call        bahar gaya, DB rollback — wapas nahi       call bahar + PENDING row
padho -> ghatao -> save                double spend                               WHERE x >= ? / @Version
retry pe do baar                       idempotency?                               DB unique key
```

---

## 4. BOLNE KA TARIKA

```
ILZAAM NAHI, SUJHAAV + ASAR:
  GALAT:  "ye code bekaar hai"
  SAHI:   "Line 20 compares strings with ==, so this condition is never true for a value read from
           the DB — the job will silently send no emails. It should be "ACTIVE".equals(status)."

GAYAB CHEEZ PE SAWAAL:
  "I don't see an owner check here — is it handled in a filter or @PreAuthorize?"

KRAM:  bhaari pehle -> style -> faisla
  "Two blockers first: hardcoded credentials and SQL injection. Then a few smaller ones:
   naming, println, magic strings."

★ Perfect nahi chahiye — soch dikhni chahiye. "Main ye isliye keh raha hoon ki..."
```

---

## 5. DRILL SE NIKLI GALTIYAN (jo chhoota, wahi yahan)

```
DRILL                 CHHOOTA                                       AGLI BAAR
30-Sep Address        owner check (IDOR) · faisla line              {id} dikhe -> "kiska hai?"
1-Oct  CardController owner check: block + getLimit me nahi tha     har {id} method pe alag se dekho
                        (updateLimit me tha -> baaki me maan liya)
                      non-owner pe bhi "ok" return (403 chahiye)    fail / mana -> sahi status, 200 nahi
                      status "BLOCKED" raw String                   magic string -> enum
                      return type alag-alag                         sab ResponseEntity
                      faisla line                                   end me NAZAR 3 wali line HAMESHA
```
2-Oct  OrderController owner check PAKDA (getOrder pe likha)          BOLTE waqt ek line: "same on cancelOrder and
                        concept poore controller pe laagu             updateQty — no {orderId} endpoint checks owner"
                      updateQty: item usi order ka hai? (alag check)  child id + parent id dono ho to rishta check
                      cancel = refund + save: @Transactional nahi,  paisa + status = do write -> transaction,
                        do request ek saath = DOUBLE REFUND         WHERE status='PLACED' / @Version / idempotency
                      Integer == Integer (qty)                       127 ke upar false -> equals
                      "PLACED"/"CANCELLED" magic · naam x, tmp       enum · kaam batane wala naam
                      faisla line (3rd baar)                         end me NAZAR 3 wali line
3-Oct  BillReminder    loop me findById = N+1 · SELECT * bina WHERE      LOOP me repo call dikhe -> N+1 bolo
                      report = report + .. loop me                   StringBuilder
                      "DUE", 0.02 magic · naam doIt, x, tmp          enum / constant · kaam batane wala naam
                      line 14 shaq hua, naam nahi diya              if(flag) return; flag=true = CHECK-THEN-ACT
                                                                     -> AtomicBoolean.compareAndSet
                      faisla line (4th baar)                         end me NAZAR 3 wali line
4-Oct  EmiController   ★ SQLi delete me (jdbc.execute + concat)       HAR query string pe "+" dhoondho, delete/update bhi
                      owner check getEmi pe (delete pe pakda)        {id} wale HAR method pe alag se
                      L36 shaq hua, naam nahi (2nd baar)            if(flag) return; flag=true -> naam: AtomicBoolean.compareAndSet
                      @Transactional delete pe likha (1 query)      DO write kahan hain? -> wahan (pay: loan + payment)
                      connection close · SELECT * bina WHERE ·       raw JDBC dikhe -> close? WHERE? · loop me HTTP / new X()
                        loop me HTTP + new RestTemplate · String +
                      running=false finally me nahi · cache stale    flag reset -> finally · cache -> kab saaf hoga?
                      NAZAR 2 poori chhooti (magic, naam, return     nazar 1 ke baad ruko mat -> naam/magic/return type
                        type, SRP) · faisla line (5th baar)
4-Oct pakda (khud): String == dono · .get() teeno · creds · N+1 · khaali catch · volatile · static HashMap ·
★ SimpleDateFormat thread-safe nahi (naya) · double paisa · PAN + println · field injection · non-owner pe "ok" · delete owner.

3-Oct pakda (khud): ★ OWNER CHECK pay() pe (pehli baar bina chhoote) · SQLi · creds dono jagah · connection close ·
String == · .get() · BigDecimal · PII log · println · khaali catch · fail pe "done" · static + volatile · count++ atomic ·
field injection · new SmsClient() test nahi hoga · SRP.

2-Oct pakda (khud): owner check getOrder · SQLi · secret · static HashMap thread · .get() · catch return null ·
entity bina DTO · N+1 · println · field injection · SRP (controller me SQL).

1-Oct pakda (khud): field injection · .get() · String == · card number log me · catch me 200 "blocked" ·
double/BigDecimal shaq · cardId validation.

★ Owner check: 30-Sep / 1-Oct chhoota, 2-Oct / 3-Oct pakda -> code padhne se PEHLE ek sawaal: "kaunse method {id} lete hain?"
★ Faisla line ab tak har drill me chhooti -> review ka AAKHRI kaam, likh ke rakho.

