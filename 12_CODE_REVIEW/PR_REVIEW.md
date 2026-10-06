# PR REVIEW — JP SUPERDAY

> Code upar se neeche chalo. Har JAGAH pe ruko, uski list dekho — bug wahi chhupta hai.
> Ek line pe bug mila to usi line pe aur dekho (loop me hai? magic value? naam?).
> ★ = drills me baar-baar chhoota.

```
 JAGAH                                      YAHAN KYA PRONE HAI                         SAHI
 ----------------------------------------   -----------------------------------------   ---------------------------------

 1. CLASS KE FIELDS (method ke bahar)
    @Autowired field pe                     hidden dep, test mushkil                    constructor + final
    "sk_live" · "password" · jdbc · host    HARDCODED CREDS (URL me = log me chhapegi)  env / Vault, header me
    static Map · static SimpleDateFormat    saare thread share, stale                   local / Concurrent* / DateTimeFormatter
    koi bhi non-final field (userId, Map)   bean SINGLETON = do user ka data mix        local variable
    flag / counter doosra thread padhe      dikhega nahi / ++ atomic nahi               volatile / AtomicInteger

 2. METHOD KI PEHLI LINE (annotation + params)
  ★ {id} · walletId · orderId aaya          OWNER CHECK — HAR method pe alag            token se owner, nahi to 403
    child id + parent id dono               rishta check                                parent ka hai ye child?
    @RequestBody bina @Valid                DTO ke @NotNull bekaar                      @Valid
  ★ amount · qty · date                     -ve / 0 ? (-ve withdraw = balance BADHTA)   @Positive / YearMonth
    double / float me paisa                 rounding                                    BigDecimal / long paise
    @Transactional controller pe ·          galat layer / proxy laga hi nahi            service pe, public, doosri bean se
    private · this.method()

 3. DB SE LAANA
  ★ SQL string me + x                       SQL INJECTION (delete / update pe bhi)      PreparedStatement + ?
    SELECT * bina WHERE · findAll           poori table memory me                       WHERE / Pageable
    .get() · list.get(0) · rs.next() chhoda khaali = 500                                orElseThrow(NotFound)
    obj aaya, seedha obj.getX()             null = NPE                                  Optional / null check

 4. IF / COMPARE
    == String / Integer / Long              reference -> if kabhi true nahi, chup       equals
  ★ if (bal >= amt) { ghatao; save }        CHECK-THEN-ACT: 2 request saath = DOUBLE    UPDATE .. WHERE bal >= ? / @Version
                                            SPEND
    if (flag) return; flag = true           check-then-act (NAAM bolo)                  AtomicBoolean.compareAndSet
    if-else type pe ("UPI" / "CARD")        naya type = method badle                    strategy

 5. DO WRITE / PAISA BADLA
    save + save · refund + status           @Transactional nahi = aadha likha           @Transactional
  ★ same request do baar (retry / click)    do baar charge                              idempotency key (DB unique)
    checked exception throw                 rollback sirf Runtime pe                    rollbackFor = Exception.class

 6. BAHAR KI CALL (RestTemplate / HTTP / mail / Kafka)
    timeout nahi                            thread atka, pool khatam, app giri          timeout + retry limit
  ★ @Transactional ke ANDAR                 connection pakda + rollback pe bahar ka     commit ke BAAD / outbox
                                            kaam wapas nahi
    new Thread() · Thread.sleep request me  thread bekaabu                              Executor / @Async

 7. LOOP
  ★ andar query / HTTP / findById           N+1                                         ek query (IN / JOIN) / batch
    andar String +                          naya object har baar                        StringBuilder

 8. try / catch
    catch(Exception) + println / khaali     nigla, prod me pata nahi                    log.error("id={}", id, e) + specific
    @Transactional me catch ne nigla        rollback NAHI                               rethrow
  ★ catch ke BAAD neeche dekho              fail pe bhi "OK" / 200                      500 / rethrow

 9. LOG / println
    card · email · PAN · password · token   PII leak                                    sirf id / mask
    println                                 prod me                                     SLF4J, sahi level

10. RETURN
    "OK" · "done" String                    status hi nahi                              ResponseEntity + sahi status
  ★ kaam na hua (kam balance) phir bhi 200  client samjha ho gaya                       400 / 422
    return ENTITY · List bina page          saare field bahar / 10 lakh row             DTO / Pageable
    e.getMessage() client ko                andar ki baat bahar                         generic message
    password seedha save                    plaintext                                   BCrypt

11. FILE / CONNECTION
    getConnection · FileWriter · openStream close / flush nahi = leak, file adhoori    try-with-resources

12. STYLE (aakhri nazar)
    naam w · r · res · x · data · doIt · public fields DTO me · magic "ACTIVE" / 5 ·
    lamba method · copy-paste · nikala par use nahi


★ FAISLA — HAMESHA, 3 NAAM ke saath
  "Request changes. Three blockers: <1>, <2>, <3>.
   Then smaller ones: naming, println, magic strings."
```
