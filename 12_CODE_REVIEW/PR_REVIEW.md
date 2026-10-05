# PR REVIEW — JP SUPERDAY

> Round me sirf **section 1** chahiye. Code pe **3 baar nazar**, har baar ALAG cheez dhoondho.
> Line-by-line padh ke jo dikhe wo bolna = aadhe chhoot-te hain.

---

## 0. ASLI ROUND

```
~10 MINUTE · senior dev · ek asli PR · "bugs + style problems" · end me "prod me jaa sakta?"
PR me aaya tha: exception handling · variable naam · logging · println · hardcoded creds · static / volatile
```
Source: Blind "jp morgan superday" · interviewquery JP SWE · techinterview.org · devbrainiac SDE-2

---

## 1. ★ SHIKAAR LIST

### NAZAR 1 — BHAARI (~4 min). Code me ye SHABD dhoondho:

```
 #   CODE ME DHOONDHO                       DIKKAT                           SAHI  +  ASAR (ye bhi bolna)
---  -------------------------------------  -------------------------------  ----------------------------------------
 1   SQL string me  + "  ya  '" + x + "'    SQL INJECTION                    koi bhi data padh / mita de
     HAR query pe (delete / update bhi)                                      -> PreparedStatement + ?

 2   "password" · "admin" · "sk_live" ·     HARDCODED CREDS                  git me prod password
     jdbc:... · smtp host/user/pass                                          -> env variable / Vault

 3   getConnection · createStatement ·      CLOSE kahan?                     connection khatam, app girega
     new FileReader / FileWriter ·          FileWriter close nahi = FLUSH    file KHAALI / adhoori, phir bhi bhej di
     openStream                             bhi nahi                         -> try-with-resources / JdbcTemplate
     raw JDBC -> WHERE bhi dekho            SELECT * bina WHERE              poori table memory me -> WHERE DB me

 4   LOOP ke andar query / HTTP / repo      N+1                              1 lakh row = 1 lakh call
     · ids nikale, phir har id pe findById                                   -> ek query (IN / JOIN / WHERE) / batch

 5   catch                                  nigla? sirf println?             prod me pata hi nahi kya toota
                                            chauda (Exception)?              -> log.error("..id={}", id, e) + specific
     ★ catch ke BAAD neeche dekho           fail par bhi "OK" / 200?         client samjha ho gaya -> 500 / rethrow

 6   .get() Optional pe · list.get(0) ·     khaali hua to crash (500)        -> orElseThrow(NotFound) / isEmpty check
     rs.next() ka return nahi dekha
     repo / map / request se aaya obj ->    null hua to NPE                  -> Optional / null check
     seedha obj.getX()

 7   static (jo final nahi) · static Map    EK copy, saare thread share,     thread-safe nahi · cache STALE
     · static SimpleDateFormat              kabhi saaf nahi                  -> local / Concurrent* / DateTimeFormatter
     @Service / @Controller me FIELD        bean SINGLETON = har request     do user ka data mix
     Map · List · counter                   wahi field share karti           -> local variable / Concurrent* / Atomic*

 8   boolean flag / counter doosra thread   volatile nahi = dikhega nahi     -> volatile (sirf dikhna)
     padhe / badhaye                        ++ atomic nahi                   -> AtomicInteger
     SHAKAL: if (flag) return; flag=true;   CHECK-THEN-ACT  (NAAM BOLO)      -> AtomicBoolean.compareAndSet
     flag = false kahan?                    exception pe phansa              -> finally

 9   ==  String / Integer / Long pe         reference compare                ASAR: "if kabhi true nahi, kaam chup-chaap
                                            (Integer 127 ke upar false)      hota hi nahi" -> equals

10   double / float me amount               paisa rounding                   -> BigDecimal / long paise
     request se amount / qty / date         -ve? 0? format?                  -> @Positive / YearMonth / @Valid
     @RequestBody pe @Valid hi nahi         DTO ke @NotNull bekaar           -> @Valid lagao

11   log / println me email · card · PAN ·  PII / secret log me              log leak = data leak -> sirf id / mask
     password · token
     return ok(ENTITY)                      saare field bahar (password?)    -> DTO
     password seedha save                   plaintext                        -> BCrypt
     e.getMessage() / stack trace client    andar ki baat bahar              -> generic message, detail sirf log me
     ko lauta diya

12   ★ {id} wala HAR method                 "ye kiska hai?" OWNER CHECK      koi bhi kisi ka data dekhe / mitaye
     (GET / pay / cancel / DELETE / export) ek me hai to baaki me maan mat   -> token se owner, nahi to 403
     child id + parent id dono?            rishta check                     (chup "ok" / 200 NAHI)
     ★ dimaag me "koi bhi kuch bhi bhej sakta" aaye = wahi OWNER CHECK -> NAAM se bolo

13   ★ TRANSACTION — @Transactional dikhe YA do write dikhein, ye SAB dekho:
     DO write ek method me                  @Transactional nahi = aadha      beech me fail -> paisa gaya, status nahi
     (save + save · refund + status)        likha                            -> @Transactional
     ★ ek hi query wale pe NAHI
     @Transactional private / self-call     proxy -> laga hi nahi            -> public, doosri bean se
     (this.method())
     @Transactional ke ANDAR try-catch      exception bahar nahi gaya =      aadha data commit
     ne exception nigal liya                rollback NAHI                    -> rethrow
     checked exception (IOException ...)    default rollback sirf Runtime    -> rollbackFor = Exception.class
                                            pe; checked pe COMMIT
     PADHO -> GHATAO -> SAVE                do request ek saath = DOUBLE     balance minus me
     (balance / stock / seat)               SPEND (lost update)              -> UPDATE .. WHERE bal >= ? / @Version
     txn ke ANDAR HTTP / mail / Kafka       DB rollback hua, bahar gaya      paisa kata nahi par mail chala gaya
                                            kaam wapas nahi · lamba txn =    -> commit ke BAAD bhejo / outbox /
                                            lock der tak                     PENDING row
     retry / double click / same request    do baar charge                   -> idempotency key (DB unique)
     do baar

14   RestTemplate / WebClient / HTTP call   TIMEOUT nahi = thread atka,      ek slow service poori app gira de
                                            pool khatam                      -> timeout + retry limit (+ circuit breaker)
     new Thread() har request pe ·          thread bekaabu / request atki    -> ExecutorService / @Async
     Thread.sleep request me

15   list / findAll lautaya, PAGE nahi      10 lakh row ek saath memory me   -> Pageable / limit
```

### ★ EK BUG PAKDA TO USI LINE PE RUKO — 3 sawaal aur

```
(1) ye LOOP ke andar hai?           -> DB / HTTP = N+1 · String + = StringBuilder
(2) koi "..." ya number likha hai?  -> magic -> enum / constant
(3) naam kaam batata hai?           -> x · a · tmp · doIt · data
```
Wajah: ek bug milte hi dimaag agli line pe chala jaata hai, usi line ka doosra bug chhoot jaata (3-Oct).

### NAZAR 2 — STYLE (~3 min). Ek-ek naam lo:

```
naam          a · b · x · s · fw · temp · data · repo · doIt()   -> kaam batane wala naam
println       production me                                       -> SLF4J logger, sahi level
magic         "ACTIVE" · "/tmp/" · 5 · 0.01 baar-baar             -> enum / constant / config
String +      loop ke andar  out = out + ...                      -> StringBuilder
injection     @Autowired field pe · new XService() andar          -> constructor + final, bean inject
SRP           ek class: DB + logic + file + mail/HTTP sab         -> service / repository / client alag
return type   String "OK" · alag-alag method alag type            -> ResponseEntity + sahi status
bekaar        nikala par use nahi · copy-paste do jagah           -> hatao / ek method
lamba method / gehri nesting                                      -> chhote method, early return
if-else type pe ("UPI" / "CARD")                                  -> strategy (naya type = method na badle)
```

### NAZAR 3 — FAISLA (~1 min). YE LINE HAMESHA, 3 NAAM ke saath:

```
"Production me abhi nahi jaana chahiye — sabse bhaari: <1> , <2> , <3>.
 Baaki (naam, println, magic string) isi PR me ya follow-up me."
```

---

## 2. 10 MINUTE KA RAASTA

```
0:00  CONTEXT POOCHO   "Ye PR kya karta hai?"
0:30  EK BAAR PADHO    upar se neeche, chup
1:30  NAZAR 1          bhaari (1-15)
5:30  NAZAR 2          style
8:30  NAZAR 3          faisla line
```
Jo pakda wo BOLO — dimaag me pakda par bola nahi = gina nahi jaata. Har bhaari point pe ek line ASAR.

---

## 3. BOLNE KA TARIKA

```
ILZAAM NAHI, SUJHAAV + ASAR:
  "Line 21 compares strings with ==, so it is never true for a value from the DB —
   the statement will always be empty and the user still gets OK. It should use equals."

GAYAB CHEEZ PE SAWAAL:
  "I don't see an owner check here — is it handled in a filter or @PreAuthorize?"

KRAM:
  "Three blockers first: missing owner check, hardcoded credentials, string comparison.
   Then smaller ones: naming, println, magic strings."
```

---

## 4. DRILL KA HISAAB

```
DRILL                  PAKDA (khud)                          SABSE BADA CHHOOTA
30-Sep Address         -                                     owner check · faisla line
1-Oct  CardController  field inj · .get() · == · card log    owner check baaki method pe · 403 · faisla
2-Oct  OrderController owner check · SQLi · N+1 · static     @Transactional (double refund) · Integer == · faisla
3-Oct  BillReminder    17/23 · owner check pay() pe          N+1 loop me · check-then-act naam · faisla
4-Oct  EmiController   9 + 6 aadhe / 25 · SimpleDateFormat   SQLi (delete me) · nazar 2 poori · faisla
5-Oct  Statement       10 + 3 aadhe / 16 · faisla PEHLI BAAR FileWriter close · "OK" 200 fail pe · owner check NAAM se
```
