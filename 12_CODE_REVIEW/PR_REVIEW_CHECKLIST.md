# PR REVIEW — EK RAASTA (code jis kram me padhte ho, usi kram me check)

> JP ke Superday me ~10 minute ka PR-review hota hai: ek PR dete hain, bug aur style poochte hain.
> 27-Sep: file dobara bani. Pehle checks CATEGORY se the (security / paisa / ...) — review me
> 7 jagah koodna padta tha. Ab **code ke raaste** se hain: request -> logic -> DB -> bahar -> response.
> Har stop pe jo dikhe uska sawaal wahin. Koi purani row hati nahi, sirf sahi stop pe gayi.

---

## 0. POORA KHEL EK NAZAR ME (~10 minute)

```
1. 30 SEC     chup. upar se neeche padho: "ye code kya kar raha hai?" (paisa? data badal raha?)

2. 5 STOP     code ke saath-saath chalo, har stop ka dabba neeche:
                 STOP 1  REQUEST    controller, jo andar aaya
                 STOP 2  LOGIC      service ke andar ki Java
                 STOP 3  DB         har query, har @Transactional
                 STOP 4  BAHAR      bank / PayPal / mail / doosri service
                 STOP 5  RESPONSE   jo wapas gaya: status, error, log

3. 10 TICK    kaagaz pe AAKHRI 10 likho (hissa 6), har ek pe tick / cross
              ye wo hain jo code me LIKHE HI NAHI hote — aankh nahi, GINTI pakadti hai

4. BOLO       pehle 2-3 sabse bhaari (security / paisa), phir baaki (hissa 7)
              jo dikha SAB bolo, chhota lage tab bhi — dimaag me pakda par bola nahi = gina nahi jaata
```

★★ **HAR DRILL ME YAHI 4 CHHOOTE** (27-Sep tak ka data) — stop pe aate hi inhe PEHLE poochho:
```
request ka NUMBER            ->  "-1000 bheju to?"                                  (STOP 1)
DB se nikali VALUE           ->  "ye variable neeche kahan USE hua?"                 (STOP 2)
@Transactional               ->  "public hai? DOOSRI bean se call hota hai?"         (STOP 3)
PADHO -> GHATAO -> SAVE      ->  "do ek saath aaye to? check WHERE me hai?"          (STOP 3)
```

---

## 1. STOP 1 — REQUEST (controller: jo andar aaya)

> Har param pe ruko. id aur amount pe teen sawaal **hamesha** — ye checks code me likhe hi nahi hote.

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
★ kisi CHEEZ ka id request me               "bhejne wala iska MAALIK hai? check kahan?"  owner = logged-in user
  (fromAcc · walletId · orderId · loanId)                                                (token / principal se)
★ userId / accountId REQUEST BODY se        "client jo chahe bheje -> kisi aur ka        userId token se, body se
                                              account" — filter body ka userId nahi       KABHI nahi
                                              jaanta, "authz filter me hoga" maan ke
                                              mat chhodo  (27-Sep drill: aadha pakda)
★ request se aaya NUMBER                    "-1000 bheju to? 0? bahut bada?"             > 0 aur upper limit
  (amount · points · qty)                     balance - (-1000) = paisa / points BANTE     (@Positive / @Max /
                                              (27-Sep: points - (-1000) = points badhe)    service me check)
@RequestBody bina @Valid                    "validation chalegi hi nahi"                 @Valid + annotations
return ok(ENTITY)  (User · Wallet ·         "saare field bahar — password, pinHash?"     DTO
  Account)
@Autowired FIELD pe                         "constructor injection — final + testable"   constructor + final
controller me SQL / JDBC / bahar ka call    "SRP — layer alag"                           controller -> service -> repo
```

---

## 2. STOP 2 — LOGIC (service ke andar ki Java)

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
★★ DB se koi value NIKALI, variable me      "ye nikali KYUN? kisse compare honi thi?"    request ke amount / limit
   rakhi, aur NEECHE KAHIN USE NAHI HUI       koi line galat nahi dikhti — galti ek        se compare -> mismatch
   double billAmount = bill.getAmount();      NAHI LIKHI line hai                          pe 400
   Double balance = jdbc.queryFor...;         bill 5000, client ne 50 bheja -> 50 kate,
                                              bill PAID. (27-Sep: balance nikala, use
                                              hi nahi -> 1000 hain, 5000 redeem)
                                            ★ shakal dead code jaisi: declare hua, use nahi
double / float + paisa                      "paisa double me — rounding, ledger mismatch" BigDecimal / paise LONG
new BigDecimal(0.015)                       "double se bana — 0.01499999..."             BigDecimal.valueOf / "0.015"
amount.equals(BigDecimal.ZERO)              "equals SCALE dekhta — 0.00 != 0"            compareTo(..)==0 / signum()
String pe ==   (status == "FROZEN")         "reference compare -> hamesha false"         "FROZEN".equals(x)
Long / Integer pe == / !=                   "-128..127 tak chalta, uske upar FAIL"       Objects.equals
findById(..).get()                          "khaali Optional -> 500"                     orElseThrow(NotFound)
rows.get(0) / list.get(0)                   "khaali list -> crash"                       isEmpty check
rs.next() ka return check nahi              "row nahi mili -> crash"                     if (!rs.next())
(double) map.get(..)                        "null aaya to? type pakka?"                  check / typed
★ singleton (controller / service) me       "har request ka SAANJHA — thread-safe?"      local variable / DB /
  MUTABLE field (Map · List · counter ·                                                   Concurrent*
  flag)
static SimpleDateFormat                     "thread-safe nahi"                           DateTimeFormatter
"sk_live_..." / password / URL code me      "secret code me hai"                         env / vault / config
constant / field declare, use kahin nahi    "dead code" (MAX_RETRIES)                    hatao
new RestTemplate() har call pe              "bean inject karo"                           ek bean
```

---

## 3. STOP 3 — DB (har query, har @Transactional)

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
★ SQL string ke andar  " + "                "injection" — sabse bada bug, RUK JAO        PreparedStatement (?) /
  jdbc.update( · jdbc.query( ·                jdbc / createQuery dikhe to POORI string     JPA param
  createQuery( · createStatement              aakhri line tak padho (id aksar neeche WHERE
                                              me hota)
★★ PADHO -> GHATAO -> SAVE (bina lock)       "do request ek saath -> dono ne 1000 padha,   UPDATE ... SET x = x - ?
   ya UPDATE x = x - ? WHERE id = ?           dono nikale = double spend"                   WHERE id = ? AND x >= ?
                                              (27-Sep: WHERE me points >= ? nahi tha)       / @Version / FOR UPDATE
                                            JPA save() bhi sirf id pe UPDATE karta —
                                              status / balance check app me hua = RACE
★ @Transactional PRIVATE method pe,         "proxy se jaa raha hai?" — private pe proxy   PUBLIC method, DOOSRI
  ya same class se call (this.x())           lagta hi nahi, andar ka call proxy bypass    bean se call
                                              -> transaction HAI HI NAHI
                                              (27-Sep: private + redeem() se andar call)
do ya zyada write, @Transactional nahi      "beech me fail -> aadha likha"               @Transactional
UPDATE ... WHERE me sirf customer_id        "customer ke DO record hue to galat row"     poori key (loan_id bhi)
naya value purane ko PADHE bina likha        "outstanding = sanctioned - amt?             purana padho, usme se
                                              purana outstanding kahan gaya?"              ghatao
LOOP ke andar repo / DB call                "N+1 — 100 row = 101 query"                  join / findAllById / IN
findByX(..) jo List lautaaye, Pageable nahi "10 lakh row ho gayi to?"                    pagination
Connection / Statement / ResultSet /        "close kahan?" — ek bhi chhoota = leak       try-with-resources
  FileWriter khud khole (raw JDBC)
  ★ JdbcTemplate / JPA repo me ye bug NAHI — pool se khud leta aur wapas deta
    (27-Sep drill me galat flag hua tha)
```

---

## 4. STOP 4 — BAHAR KA CALL (bank · PayPal · gateway · mail · doosri service)

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
★ do DB-write ke BEECH me bahar ka call      "iske baad fail hua to jo BAHAR gaya, wo     bahar ka call transaction
                                              wapas aayega?" — paisa gaya, record nahi.     ke BAHAR + PENDING row
                                              DB rollback se payout wapas NAHI aata          pehle + reconcile / retry
retry / double-click pe koi rok nahi         "dobara chala to double charge"              idempotency key (DB unique)
★ Map / Set jo "idempotency" ka kaam kare   "ye SACH me kaam karta?"                     DB unique key
  (instance field)                            HashMap = thread-unsafe · restart pe gaayab
                                              · 2 pod pe bekaar · containsKey -> put ke
                                              beech race · put() kaam se PEHLE ho to ek
                                              fail = hamesha block
```

---

## 5. STOP 5 — RESPONSE / ERROR / LOG (jo wapas gaya)

```
CODE ME DIKHE                               POOCHO / BOLO                              SAHI
---------------------------------------------------------------------------------------------------------
★ ResponseEntity.ok(..) FAILURE branch me   "is haalat ka HTTP code kya?"                400 · 403 · 404 · 409 ·
  ok("FAILED") · ok("Something went wrong")                                               422 · 500/502
catch(Exception) { } / printStackTrace()    "exception nigla — galti dikhegi hi nahi"    log + rethrow / proper code
  / return null / return "FAILED"
log.error("failed") — e aur id nahi         "stack trace kahan? kis id pe?"              log.error("... id={}", id, e)
log me PII  (email · account · card ·       "log me PII / card"                          sirf id / masked
  IFSC · password) · log.info(... + req)
password seedha save                        "plaintext"                                  BCrypt
```

---

## 6. AAKHRI 10 — KAAGAZ PE LIKHO, TICK KARO

> Ye code me **likhe hi nahi** hote — aankh kabhi nahi pakdegi, **ginna** padta hai.
> Paise ya data badalne wale HAR endpoint pe, 5 stop ke BAAD:

```
★ = sabse zyada chhootne wale — inhe PEHLE tick karo

[ ] ★ AUTHORIZATION   bulane wala is cheez ka MAALIK hai? id BODY se to nahi aa raha?
[ ] ★ VALIDATION      number > 0? -ve? bahut bada? account exist? khud ko khud?
[ ] ★ AMOUNT COMPARE  DB se nikali value request se compare hui? (nikali, use nahi = bug)
[ ] ★ RACE            padh ke ghataya? check UPDATE ke WHERE me (x >= ?) / @Version / FOR UPDATE?
[ ] ★ AUDIT           kaun, kab, kitna — record hua?
[ ]   MONEY TYPE      BigDecimal / long me? (double, float, new BigDecimal(double) nahi)
[ ]   TRANSACTION     saare write ek saath? @Transactional SACH me laga? (public + doosri bean)
[ ]   KRAM            do write ke beech bahar ka call?
[ ]   IDEMPOTENCY     dobara chala to? jo hai wo SACH me kaam karta? (HashMap = nahi)
[ ]   ERROR CODE      fail pe 4xx/5xx? ya sab 200 OK?
```

★ Koi cheez "hai" dikh rahi hai to bhi poochho **"ye SACH me kaam karti hai?"**
(`@Transactional` private pe · idempotency ek HashMap · owner check body ke userId se —
teeno "maujood" the, teeno kaam nahi karte the.)

---

## 7. BOLNE KA TARIKA

```
KRAM (bolte waqt, bhaari pehle):
   security -> paisa -> transaction / race -> bahar ka call -> java trap -> error / log -> design

"Main pehle SECURITY dekhta hoon — yahan SQL string jod ke ban rahi hai, injection ka risk,
 aur userId body se aa raha hai, to koi bhi kisi aur ke account pe chala sakta hai...
 phir PAISA — amount double me hai, -ve ka check nahi, aur DB se balance nikala par compare nahi hua...
 phir TRANSACTION — @Transactional private pe hai to laga hi nahi, aur WHERE me balance check
 nahi to do request ek saath double spend kar sakti hain...
 phir BAHAR — do write ke beech payout hai, fail hua to paisa gaya par record nahi..."

★ TIME: ~10 minute. 2-3 sabse bhaari pehle, style (field injection, dead code) baad me.

★ GAYAB cheez pe SAWAAL, ILZAAM nahi:
  "yahan ownership check nazar nahi aa raha — kya wo kisi filter / @PreAuthorize me hai?"
  AUTHENTICATION = kaun hai (filter me hota).
  AUTHORIZATION  = is cheez ka haq hai? — param dekhe bina ho hi nahi sakta,
                   isliye controller / service me dhoondho.
```
