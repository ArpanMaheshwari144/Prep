# PR REVIEW — JP SUPERDAY

Code upar se neeche chalo. Har JAGAH pe ruko, neeche uska section dekho.
Har bug ki pehchaan = code ki SHAKAL. Shakal dikhi -> naam bolo -> asar bolo -> sahi bolo.
★ = drills me chhoota tha.

---

## 0. PEHLE YE 10 SAWAAL (har PR pe, isi kram me)

```
 1  FIELDS     method ke bahar kaun-sa field hai? saare user share karenge kya?
 2  ID         {id} / accountId aaya -> ye ID IS USER ki hai, kisne check kiya?
 3  INPUT      @Valid hai? amount -ve / 0 aa sakta? paisa double me to nahi?
 4  2 REQUEST  ek saath 2 request aayi (ya retry hua) to kya hoga?
 5  SAVE       kitne save/update hain? @Transactional hai?
 6  BAHAR      HTTP / mail / SMS call kahan hai? timeout? txn ke andar?
 7  LOOP       loop ke andar query / HTTP to nahi?
 8  CATCH      catch ne kya kiya? uske BAAD kya return hua?
 9  LOG        println? log me email / card / token?
10  RETURN     fail pe bhi 200? entity bahar? list bina page? SQL me + ?
```

Aakhir me FAISLA line (neeche section 13). Bina faisla ke review adhoora.

---

## 1. CLASS KE FIELDS (method ke bahar)

```
CODE ME DIKHE:   @Autowired private AccountRepository repo;
NAAM:            field injection
ASAR:            dependency chhupi, test me mock mushkil
SAHI:            constructor injection + final
```

```
CODE ME DIKHE:   private String API_KEY = "sk_live_...";  ·  "password"  ·  jdbc url me pass  ·  smtp creds
                 rt.post(URL + "?key=" + KEY)
NAAM:            HARDCODED CREDS
ASAR:            git me leak; URL me ho to access log me bhi chhapegi
SAHI:            env / Vault, key header me
```

```
CODE ME DIKHE:   private String currentUser;   (koi bhi non-final field: userId, Map, list)
NAAM:            singleton bean me state
ASAR:            controller/service ek hi object, saare user share -> do user ka data mix
SAHI:            local variable
★ 7-Oct chhoota (Transfer): tune L5 static final URL pakda, wo constant hai, theek hai.
  Asli dikkat non-final field hoti hai.
```

```
CODE ME DIKHE:   static Map cache  ·  static SimpleDateFormat
NAAM:            shared mutable static
ASAR:            saare thread share, stale / galat date
SAHI:            local / ConcurrentHashMap / DateTimeFormatter
```

```
CODE ME DIKHE:   boolean running;  count++   (doosra thread padhe)
NAAM:            visibility / non-atomic
ASAR:            doosre thread ko dikhega nahi; ++ me ginti khoyegi
SAHI:            volatile / AtomicInteger
```

```
CODE ME DIKHE:   flag = true;  ...kaam...  flag = false;
NAAM:            flag reset finally me nahi
ASAR:            beech me exception -> flag hamesha true phansa
SAHI:            finally { flag = false; }
```

---

## 2. METHOD KI PEHLI LINE (annotation + params)

```
CODE ME DIKHE:   @GetMapping("/{accountId}")  ·  r.getFromAccountId()  ·  walletId  ·  orderId
NAAM:            ★ OWNER CHECK nahi (IDOR)
ASAR:            koi bhi kisi ka bhi account dekh / paisa nikaal sakta
SAHI:            token se user lo, ye ID usi ki hai? nahi to 403. HAR method pe alag dekho.
BOL:             "I don't see an owner check here — is it handled in a filter?"
```

```
CODE ME DIKHE:   orderId + itemId dono aaye
NAAM:            parent-child rishta check nahi
ASAR:            doosre ke order ka item badal diya
SAHI:            ye item is order ka hai? check
```

```
CODE ME DIKHE:   @RequestBody TransferRequest r      (@Valid nahi)
NAAM:            validation band
ASAR:            DTO ke @NotNull / @Positive likhe hain par chalenge hi nahi
SAHI:            @Valid @RequestBody
★ 7-Oct chhoota (Transfer)
```

```
CODE ME DIKHE:   double amt = r.getAmount();   (amount · qty · days)
NAAM:            ★ -ve / 0 check nahi
ASAR:            -ve withdraw = balance BADHTA; -ve transfer = ulta paisa
SAHI:            @Positive / if (amt <= 0) 400
★ 7-Oct chhoota (Transfer)
```

```
CODE ME DIKHE:   double balance  ·  float price
NAAM:            paisa floating point me
ASAR:            0.1 + 0.2 != 0.3 -> rounding
SAHI:            BigDecimal / long paise
```

```
CODE ME DIKHE:   @Transactional controller pe  ·  private method pe  ·  this.save() se andar ka call
NAAM:            galat jagah transaction
ASAR:            proxy laga hi nahi -> transaction chala hi nahi
SAHI:            service pe, public method, doosri bean se call
```

---

## 3. DB SE LAANA

```
CODE ME DIKHE:   "SELECT * FROM t WHERE note LIKE '%" + q + "%'"   (delete / update me bhi)
NAAM:            ★ SQL INJECTION
ASAR:            q = "' OR 1=1 --" -> poori table; drop bhi
SAHI:            PreparedStatement + ?   /   jdbc.query(sql, args)
```

```
CODE ME DIKHE:   findAll()  ·  SELECT * bina WHERE
NAAM:            poori table memory me
ASAR:            10 lakh row -> OOM / slow
SAHI:            WHERE / Pageable
```

```
CODE ME DIKHE:   repo.findById(id).get()  ·  list.get(0)  ·  rs.next() check nahi
NAAM:            khaali case nahi socha
ASAR:            nahi mila -> NoSuchElement -> 500
SAHI:            orElseThrow(NotFoundException) -> 404
```

```
CODE ME DIKHE:   User u = repo.find(id);  u.getName();
NAAM:            null check nahi
ASAR:            NPE -> 500
SAHI:            Optional / null check
```

---

## 4. IF / COMPARE

```
CODE ME DIKHE:   if (from.getBalance() >= amt) {
                     from.setBalance(from.getBalance() - amt);
                     save(from);
                 }
NAAM:            ★ CHECK-THEN-ACT -> DOUBLE SPEND
ASAR:            2 request ek saath -> dono ko poora balance dikha -> dono ne kaata
SAHI:            UPDATE acc SET bal = bal - ? WHERE id = ? AND bal >= ?   (rows = 0 -> fail)
                 ya @Version (optimistic) / SELECT .. FOR UPDATE
YAAD:            ye RETRY wali idempotency se ALAG hai. Idempotency = same request do baar.
                 Ye = do alag request ek hi waqt.
★ 7-Oct chhoota (Transfer)
```

```
CODE ME DIKHE:   if (status == "ACTIVE")  ·  Integer a == Integer b  ·  Long == Long
NAAM:            == se object compare
ASAR:            reference compare -> chup-chaap false (Integer 127 ke upar false)
SAHI:            "ACTIVE".equals(status) / enum / equals()
```

```
CODE ME DIKHE:   if (running) return;  running = true;
NAAM:            check-then-act (flag wala)
ASAR:            do thread dono andar ghus gaye
SAHI:            AtomicBoolean.compareAndSet(false, true)
```

```
CODE ME DIKHE:   if (type.equals("UPI")) ... else if (type.equals("CARD")) ...
NAAM:            type pe if-else
ASAR:            naya type = ye method badlo (open-closed toota)
SAHI:            strategy pattern
```

---

## 5. DO WRITE / PAISA BADLA

```
CODE ME DIKHE:   repo.save(from);
                 repo.save(to);
                 transferRepo.save(t);          (method pe @Transactional nahi)
NAAM:            ★ @Transactional nahi
ASAR:            beech me fail -> from se kata, to me nahi pahuncha = aadha likha
SAHI:            service method pe @Transactional
★ 7-Oct: dimaag me aaya, likha nahi -> PR me jo likha wahi gina jaata
```

```
CODE ME DIKHE:   POST /transfer  ·  /pay  ·  /refund   (request me koi unique key nahi)
NAAM:            ★ idempotency nahi
ASAR:            retry / double click -> do baar charge
SAHI:            Idempotency-Key header, DB me unique -> doosri baar wahi purana jawab
```

```
CODE ME DIKHE:   throws IOException  (checked) @Transactional method se
NAAM:            checked exception pe rollback nahi
ASAR:            Spring default sirf RuntimeException pe rollback -> data aadha commit
SAHI:            @Transactional(rollbackFor = Exception.class)
```

```
CODE ME DIKHE:   ek hi query wala method
NAAM:            -
SAHI:            yahan @Transactional mat maango
```

---

## 6. BAHAR KI CALL (RestTemplate / HTTP / mail / SMS / Kafka)

```
CODE ME DIKHE:   rt.postForObject(SMS_URL, ...)    (timeout set nahi)
NAAM:            timeout nahi
ASAR:            vendor atka -> thread atka -> pool khatam -> app giri
SAHI:            timeout + retry limit + circuit breaker
★ 7-Oct chhoota (Transfer)
```

```
CODE ME DIKHE:   save(from); save(to);  rt.post(SMS...);   (sab ek try / ek txn me)
NAAM:            ★ bahar ki call paisa wale flow ke andar
ASAR:            txn me ho: DB connection pakda rehta; rollback pe SMS wapas nahi aata
                 try me ho: SMS fail -> catch -> transfer fail bata diya jabki paisa gaya
SAHI:            commit ke BAAD / @Async / outbox
★ 7-Oct chhoota (Transfer)
```

```
CODE ME DIKHE:   new Thread(...)  ·  Thread.sleep() request ke andar
NAAM:            thread bekaabu
SAHI:            Executor / @Async
```

---

## 7. LOOP

```
CODE ME DIKHE:   for (Transfer t : list) { repo.findById(t.getToId()); }
NAAM:            ★ N+1
ASAR:            100 transfer = 101 query
SAHI:            ek query: IN (...) / JOIN / batch fetch
```

```
CODE ME DIKHE:   s = s + x;   loop ke andar
NAAM:            String concat in loop
SAHI:            StringBuilder
```

---

## 8. try / catch

```
CODE ME DIKHE:   catch (Exception e) { System.out.println(e.getMessage()); }   ·  catch {} khaali
NAAM:            exception nigla
ASAR:            prod me pata hi nahi chalega kya toota
SAHI:            specific exception + log.error("transfer failed id={}", id, e)
```

```
CODE ME DIKHE:   @Transactional method ke andar catch (Exception e) { log... }   (rethrow nahi)
NAAM:            catch ne rollback roka
ASAR:            exception bahar gaya hi nahi -> commit ho gaya
SAHI:            rethrow
```

```
CODE ME DIKHE:   } catch (Exception e) { ...; return "SUCCESS"; }
NAAM:            ★ fail pe bhi OK
ASAR:            client samjha ho gaya, paisa phansa
SAHI:            500 / rethrow
```

---

## 9. LOG / println

```
CODE ME DIKHE:   println("done for " + user.getEmail())  ·  card · PAN · password · token
NAAM:            PII log me
ASAR:            log padhne wala har banda data dekh le, compliance toota
SAHI:            sirf id / masked
```

```
CODE ME DIKHE:   System.out.println
NAAM:            println prod me
SAHI:            SLF4J logger, sahi level
```

---

## 10. RETURN

```
CODE ME DIKHE:   return "OK";  ·  return "done";  ·  public String transfer(...)
NAAM:            String return
ASAR:            HTTP status hamesha 200
SAHI:            ResponseEntity + sahi status
```

```
CODE ME DIKHE:   return "FAILED";   /   kam balance pe bhi normal return
NAAM:            ★ kaam na hua phir bhi 200
ASAR:            client ne 200 dekha, samjha ho gaya
SAHI:            400 / 422
★ 7-Oct chhoota (Transfer)
```

```
CODE ME DIKHE:   return List<Transfer>  ·  return userEntity  ·  list bina Pageable
NAAM:            entity bahar / pagination nahi
ASAR:            saare field bahar (password hash bhi); 10 lakh row ek saath
SAHI:            DTO + Pageable
★ 7-Oct chhoota (Transfer, history)
```

```
CODE ME DIKHE:   body(e.getMessage())
NAAM:            andar ki baat client ko
SAHI:            generic message, detail sirf log me
```

```
CODE ME DIKHE:   user.setPassword(req.getPassword()); save
NAAM:            plaintext password
SAHI:            BCrypt
```

---

## 11. FILE / CONNECTION

```
CODE ME DIKHE:   getConnection()  ·  new FileWriter()  ·  openStream()   (close nahi)
NAAM:            resource leak
ASAR:            connection pool khatam / file adhoori
SAHI:            try-with-resources
```

---

## 12. STYLE (aakhri nazar)

```
naam: w · r · res · t · x · data · doIt          -> kaam batata naam
public fields DTO me                              -> private + getter
magic "ACTIVE" / 5                                -> enum / constant
lamba method / gehri nesting                      -> early return, chhote method
copy-paste                                        -> ek method
nikala par use nahi                               -> hatao
new XService() andar                              -> inject karo
SRP: controller me DB + logic + mail sab          -> controller / service / repo alag
```

---

## 13. FAISLA — HAMESHA, 3 NAAM ke saath

```
"Request changes. Three blockers: <1>, <2>, <3>.
 Then smaller ones: naming, println, magic strings."

Misaal (Transfer, 7-Oct):
"Request changes. Three blockers: no owner check on transfer and history,
 double spend because balance check and update are not atomic and not in a transaction,
 and SQL injection in search. Then smaller ones: hardcoded key, println with email, naming."
```

```
Jo pakda wo LIKHO / BOLO — dimaag me pakda = gina nahi jaata.
Har bhaari pe ek line ASAR: kya tootega.
Gayab cheez pe sawaal: "I don't see an owner check here — is it handled in a filter?"
```
