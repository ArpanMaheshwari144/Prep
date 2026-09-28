# PR REVIEW — JP SUPERDAY

> **Round me sirf section 1 (SHIKAAR LIST) chahiye.** Baaki file = samajh / reference.
> Drill ka code + answer key = section 5 (neeche).
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

## 5. DRILLS — code + answer key (redo: key band, 10 min, phir milao)

### DRILL 1 — StatementJob (nightly statement email)

```java
 1  public class StatementJob {
 2
 3      static List<String> failed = new ArrayList<>();
 4      private boolean running = false;
 5
 6      private String dbUrl  = "jdbc:mysql://prod-db:3306/bank";
 7      private String dbUser = "admin";
 8      private String dbPass = "Admin@123";
 9
10      public void run(String date) {
11          running = true;
12          try {
13              Connection c = DriverManager.getConnection(dbUrl, dbUser, dbPass);
14              Statement s = c.createStatement();
15              ResultSet r = s.executeQuery(
16                  "SELECT id, email, status FROM accounts WHERE stmt_date = '" + date + "'");
17              while (r.next()) {
18                  String e  = r.getString("email");
19                  String st = r.getString("status");
20                  if (st == "ACTIVE") {
21                      System.out.println("sending statement to " + e);
22                      boolean x = send(e, build(r.getLong("id")));
23                      if (!x) failed.add(e);
24                  }
25              }
26          } catch (Exception ex) {
27              System.out.println("error");
28          }
29          running = false;
30      }
31
32      public boolean isRunning() { return running; }
33
34      private String build(long id) throws SQLException {
35          Connection c = DriverManager.getConnection(dbUrl, dbUser, dbPass);
36          ResultSet r = c.createStatement().executeQuery(
37              "SELECT amount, txn_date FROM txns WHERE account_id = " + id);
38          String out = "";
39          while (r.next()) {
40              out = out + r.getDouble("amount") + " " + r.getDate("txn_date") + "\n";
41          }
42          return out;
43      }
44
45      private boolean send(String to, String body) {
46          try {
47              mailClient.send(to, body);
48              return true;
49          } catch (Exception e) {
50              return false;
51          }
52      }
53  }
```

<details><summary>ANSWER KEY (17)</summary>

```
BHAARI
 1  L6-8    hardcoded DB url / user / password -> env / Vault
 2  L16,37  SQL injection (date, id string me jode) -> PreparedStatement + ?
 3  L13,35  Connection / Statement / ResultSet close nahi; build() har account pe NAYA connection
            -> try-with-resources / pool / JdbcTemplate
 4  L20     String == "ACTIVE" -> kabhi true nahi, job ek bhi mail nahi bhejegi -> "ACTIVE".equals(st)
 5  L26-27  catch (Exception) + println("error") -> kya toota pata nahi -> log.error(.., ex)
 6  L49-50  send() exception nigla, log nahi
 7  L3      static mutable List -> sab share, kabhi saaf nahi, ArrayList thread-safe nahi
 8  L4      running doosra thread padhta -> volatile nahi; run() dobara chal sakta -> AtomicBoolean.compareAndSet
 9  L22/37  loop me har account pe query = N+1 -> ek JOIN
10  L40     double me amount -> BigDecimal
11  L21     email (PII) log me
STYLE
12  naam: c, s, r, e, st, x, build
13  L40     loop me String + -> StringBuilder
14  L20     "ACTIVE" magic string -> enum / constant
15  L29     running = false -> finally
16  design: DB + format + mail ek class me (SRP)
17  failed list ka koi use nahi (retry / report nahi)

FAISLA: production me NAHI — creds + SQL injection + connection leak + String == (koi mail nahi jaayegi)
```
</details>

Pehli baar (28-Sep): 17 me se 7 + ek sahi point key se bahar (L21 println -> logger).
Chhoote: SQL injection · connection leak · static · N+1 · PII · StringBuilder · magic string ·
finally / AtomicBoolean · SRP · faisla line.
