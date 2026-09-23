# PR REVIEW — KYA DHOONDHNA HAI, KAISE DIKHTA HAI, KYA BOLNA HAI

> JP ke Superday me ~10 minute ka PR-review hota hai: ek PR dete hain, bug aur style poochte hain.
> Is file me sirf wo hai jo review KARTE WAQT kaam aata hai.

```
1. KAISE CHALANA      5 kadam
2. MASTER LIST        jo code me GALAT LIKHA hai — code me kaisa DIKHTA hai -> kya POOCHNA -> sahi kya
3. AAKHRI 9 TICK      jo code me LIKHA HI NAHI hota — aankh se nahi milta, gin ke milta hai
4. BOLNE KA TARIKA
```

---

## 1. KAISE CHALANA

```
KADAM 1   30 second, kuch mat bolo. Poora code upar se neeche. "ye class kya kar rahi hai?"

KADAM 2   teen sawaal, poori file pe:
            (a) USER ka data kahan-kahan ja raha?   SQL me · log me · seedha return me · bina check ke
            (b) beech me CRASH hua to aadha kaam kis haal me?   do write · beech me bahar ka call
            (c) ye DO BAAR / EK SAATH chala to?   double charge · race · shared field

KADAM 3   MASTER LIST (hissa 2) pe ek sweep — har category ek baar

KADAM 4   ★ AAKHRI 9 (hissa 3) KAAGAZ PE LIKHO, har ek pe tick ya cross.
          Ye kadam chhoda to jo "likha hi nahi" hai wo kabhi nahi milega.

KADAM 5   bolo: pehle 2-3 sabse bhaari (security / paisa), phir baaki
```

---

## 2. MASTER LIST

### SECURITY

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
SQL string ke andar  " + "                  "injection" — sabse bada bug, RUK JAO      PreparedStatement (?)
                                                                                        / JPA param
"sk_live_..." / password / URL              "secret code me hai"                        env / vault / config
  code me likha

log.info(... + req)  /  account / card /    "log me PII / card"                         sirf id / masked
  IFSC / password log me

return ok(ENTITY)  (User · Wallet ·         "saare field bahar — password, pinHash?"    DTO
  Account)

password seedha save                        "plaintext"                                  BCrypt

request me kisi ki CHEEZ ka id              "bhejne wala iska MAALIK hai? check kahan?"  owner = logged-in user
  (fromAcc · walletId · orderId · loanId)     userId BODY se aa raha = client jo chahe    (token se, body se NAHI)
                                              bheje -> authz toota hua
```

### PAISA

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
double / float + paisa                      "paisa double me — rounding, ledger mismatch" BigDecimal / paise LONG
new BigDecimal(0.015)                       "double se bana — 0.01499999..."            BigDecimal.valueOf / "0.015"
amount.equals(BigDecimal.ZERO)              "equals SCALE dekhta — 0.00 != 0"            compareTo(..)==0 / signum()
request ka amount, aur DB ka asli           "dono kabhi COMPARE hue?"                    compare karo
  amount / limit bhi nikala gaya              (nikala, variable me rakha, use nahi kiya
  (sanctioned · dailyLimit · orderAmount)      -> 5 lakh sanction, 50 lakh nikal gaya)
request se aaya number, usse jod/ghata       "HAD kahan check hui? -ve? 0? bahut bada?"   > 0 aur upper limit
                                              (-5000 -> balance - (-5000) = paisa BANA)
```

### TRANSACTION / KRAM / DO-BAAR

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
do ya zyada write, @Transactional nahi      "beech me fail -> aadha likha"               @Transactional
@Transactional PRIVATE method pe,           "proxy se jaa raha hai?" — private pe proxy   public method, doosri
  ya same class se call (this.x())           lagta hi nahi, self-call proxy se nahi       bean se call
                                              jaata -> transaction HAI HI NAHI
do DB-write ke BEECH me bahar ka call        "iske baad fail hua to jo BAHAR gaya,        bahar ka call transaction
  (bank · gateway · mail · notification)      wo wapas aayega?" (paisa gaya, record nahi)  ke BAHAR + reconcile/retry
balance PADHO -> ghatao -> SAVE             "do request ek saath -> dono ne 1000 padha,  @Version / SELECT FOR UPDATE
  (bina lock)                                 dono nikale = double spend"                  / UPDATE ... WHERE bal>=?
retry / double-click pe koi rok nahi         "dobara chala to double charge"              idempotency key (unique)
Map / Set jo "idempotency" ka kaam kare     "ye SACH me kaam karta?"                     DB unique key
  (instance field)                            HashMap = thread-unsafe · kabhi clear nahi
                                              · 2 pod pe bekaar · put() kaam se PEHLE
                                              -> ek fail = hamesha block
```

### JAVA / SPRING TRAP (code me "sahi dikhte" hain)

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
String pe ==   (status == "FROZEN")         "reference compare -> hamesha false"         "FROZEN".equals(x)
Long / Integer pe == / !=                   "-128..127 tak chalta, uske upar FAIL"        Objects.equals
findById(..).get()                          "khaali Optional -> 500"                     orElseThrow(NotFound)
rs.next() ka return check nahi              "row nahi mili -> crash"                     if (!rs.next())
rows.get(0) / list.get(0)                   "khaali list -> crash"                       isEmpty check
static SimpleDateFormat                     "thread-safe nahi"                           DateTimeFormatter
controller/service me mutable field         "har request me SAANJHA — thread-safe?"      local / DB / concurrent
  (Map · List · counter · flag)
(double) map.get(..)                        "null aaya to? type pakka?"                   check / typed
```

### RESOURCE / DB

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
Connection / Statement / ResultSet /        "close kahan?" — teeno me ek bhi chhoota     try-with-resources
  FileWriter khule
LOOP ke andar repo / DB call                "N+1 — 100 row = 101 query"                  join / findAllById / IN
findByX(..) jo List lautaaye, Pageable nahi "10 lakh row ho gayi to?"                     pagination
UPDATE ... WHERE me sirf customer_id        "customer ke DO record hue to galat row"      poori key (loan_id bhi)
naya value purane ko PADHE bina likha        "outstanding = sanctioned - amt?              purana padho, usme se ghatao
                                              purana outstanding kahan gaya?"
```

### ERROR HANDLING

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
ResponseEntity.ok(..) FAILURE branch me     "is haalat ka HTTP code kya?"                400 · 403 · 404 · 409 ·
  ok("FAILED") · ok("INSUFFICIENT")                                                       422 · 500/502
catch(Exception) { }  /  printStackTrace()  "exception nigla — galti dikhegi hi nahi"    log + rethrow / proper code
  / return null / return "FAILED"
log.error("failed")  — e aur id nahi         "stack trace kahan? kis id pe?"              log.error("... id={}", id, e)
```

### DESIGN

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
@Autowired field pe                         "constructor injection — final + testable"
controller me SQL / JDBC / bahar ka call    "SRP — layer alag"                           controller -> service -> repo
new RestTemplate() har call pe              "bean inject karo"
constant / field declare, use kahin nahi    "dead code" (MAX_RETRIES)
```

---

## 3. AAKHRI 9 — KAAGAZ PE LIKHO, TICK KARO

> Ye wo cheezein hain jo code me **likhi hi nahi** hoti — isliye aankh kabhi nahi pakdegi.
> Dekhne ko kuch hai hi nahi, **ginna** padta hai. Paise ya data badalne wale HAR endpoint pe:

```
[ ] AUTHORIZATION   bulane wala is cheez ka MAALIK hai? check kahan hai?
[ ] VALIDATION      amount > 0? -ve? bahut bada? account exist? khud ko khud?
[ ] AMOUNT COMPARE  request ka amount DB ke asli amount / limit se compare hua?
[ ] MONEY TYPE      paisa BigDecimal / long me? (double, float, new BigDecimal(double) nahi)
[ ] TRANSACTION     saare write ek saath commit/rollback? (aur @Transactional SACH me laga?)
[ ] KRAM            do write ke beech bahar ka call?
[ ] IDEMPOTENCY     dobara chala to? (aur jo hai wo SACH me kaam karta?)
[ ] ERROR CODE      fail pe 4xx/5xx? ya sab 200 OK?
[ ] AUDIT           kaun, kab, kitna — record hua?
```

★ Koi cheez "hai" dikh rahi hai to bhi ek baar poochho **"ye SACH me kaam karti hai?"**
(`@Transactional` private pe laga hai · idempotency ek HashMap hai · owner check body ke userId se hai
— teeno "maujood" the, teeno kaam nahi karte the.)

---

## 4. BOLNE KA TARIKA

```
KRAM:   security -> paisa -> transaction/kram -> java-trap -> resource/db -> error -> design

"Main pehle SECURITY dekhta hoon — yahan SQL string jod ke ban rahi hai, injection ka risk...
 phir PAISA — amount double me hai, aur request ka amount DB se compare nahi hua...
 phir TRANSACTION — do write hain aur beech me bank call hai...
 aur ye payment flow hai, to retry pe double charge to nahi hoga — idempotency dekhta hoon."

★ TIME: ~10 minute. 2-3 sabse bhaari pehle, style baad me.

★ GAYAB cheez pe SAWAAL, ILZAAM nahi:
  "yahan ownership check nazar nahi aa raha — kya wo kisi filter / @PreAuthorize me hai?"
  (AUTHENTICATION = kaun hai, filter me hota. AUTHORIZATION = is cheez ka haq hai? — wo
   param dekhe bina ho hi nahi sakta, isliye controller/service me dhoondho.)
```
