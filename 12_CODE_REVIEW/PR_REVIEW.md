# PR REVIEW — JP SUPERDAY

Code upar se neeche chalo. Har JAGAH pe ruko, neeche uska section dekho.
Har bug ki pehchaan = code ki SHAKAL. Shakal dikhi -> naam bolo -> asar bolo -> sahi bolo.
★ = drills me chhoota tha.

---

## 0. MOCK CUE — drill se pehle SIRF ye padho (number click = wo section)

| # | sawaal | kya dekhna (cue) |
|---|--------|------------------|
| [1](#s1) | FIELDS | method ke bahar non-final field? request ka data, volatile counter, secret, static SimpleDateFormat |
| [2](#s2) | ID / ROLE | {id} aaya -> IS user ka hai? approve / admin -> role check? |
| [3](#s2) | INPUT | @Valid hai? amount -ve / 0? paisa double me? |
| [4](#s4) | 2 REQUEST | pehle check phir save (exists / balance) -> 2 request saath = dono andar · retry = 2 baar |
| [5](#s5) | SAVE | 2+ save -> @Transactional? public, service pe? status PEHLE verify BAAD? |
| [6](#s6) | BAHAR | HTTP / SMS / S3 call -> timeout? txn ke andar? DB fail to file / SMS anaath? |
| [7](#s7) | LOOP | loop me query / HTTP? string += ? |
| [8](#s8) | CATCH | catch ne nigla? uske baad "SUCCESS" / 200? rollback ruka? |
| [9](#s9) | LOG | println? PAN / Aadhaar / card / token log me? |
| [10](#s10) | RETURN | fail pe 200? entity bahar (full data)? list bina page? · .get() -> 500 = [§3](#s3) |
| [11](#s12) | STYLE (1 min) | naam · magic string -> enum · URL me verb · SRP · field injection · date String |
| [12](#s13) | FAISLA | Request changes + 3 bhaari naam + chhote |

Tareeka: ye table padho -> file band -> PR pe 1 se 12 ek-ek -> jo chhoota sirf uska section kholo.

Aakhir me FAISLA line (neeche section 13). Bina faisla ke review adhoora.

---

<a id="s1"></a>
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
★ 13-Oct chhoota (KYC: private KycRequest current; current = r;)
★ 14-Oct chhoota (Loan: private Loan loan; loan = findById(..))
```

```
CODE ME DIKHE:   static Map cache  ·  static SimpleDateFormat
NAAM:            shared mutable static
ASAR:            saare thread share, stale / galat date
SAHI:            local / ConcurrentHashMap / DateTimeFormatter
DHYAAN:          static field class ke NEECHE likha ho to bhi chalega (class load pe pehle ban jaata).
                 dikkat jagah nahi, thread-unsafe hona hai.
```

```
CODE ME DIKHE:   boolean running;  count++   ·  private volatile int pendingCount;  pendingCount++
NAAM:            visibility / non-atomic
ASAR:            doosre thread ko dikhega nahi; ++ me ginti khoyegi
SAHI:            flag -> volatile · counter -> AtomicInteger
                 (volatile int ++ / -- = ab bhi race, volatile sirf DIKHATA hai, atomic nahi banata)
★ 13-Oct chhoota (KYC: volatile pendingCount++ / --)
```

```
CODE ME DIKHE:   flag = true;  ...kaam...  flag = false;
NAAM:            flag reset finally me nahi
ASAR:            beech me exception -> flag hamesha true phansa
SAHI:            finally { flag = false; }
```

---

<a id="s2"></a>
## 2. METHOD KI PEHLI LINE (annotation + params)

```
CODE ME DIKHE:   @GetMapping("/{accountId}")  ·  r.getFromAccountId()  ·  walletId  ·  orderId
NAAM:            ★ OWNER CHECK nahi (IDOR)
ASAR:            koi bhi kisi ka bhi account dekh / paisa nikaal sakta
SAHI:            token se user lo, ye ID usi ki hai? nahi to 403. HAR method pe alag dekho.
BOL:             "I don't see an owner check here — is it handled in a filter?"
★ 14-Oct: GET /loan/{id} pe pakda · doPrepay(loanId) + close(id) pe chhoota -> HAR method alag
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
CODE ME DIKHE:   @RequestParam String otp  ·  ?password=  ·  ?apiKey=   (secret URL me)
NAAM:            secret URL / query me
ASAR:            URL access log / proxy / browser history me chhap jaata
SAHI:            body ya header me
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
ASAR:            private / this.save() = proxy bypass -> transaction chala hi nahi · controller pe = chalta hai, par galat layer
SAHI:            service pe, public method, doosri bean se call
★ 12-Oct chhoota (Beneficiary: @Transactional private updateLimit)
```

```
CODE ME DIKHE:   /approve  ·  /admin/...  ·  status / limit / role badalne wala endpoint   (@PreAuthorize nahi)
NAAM:            ★ ROLE CHECK nahi
ASAR:            normal user apna hi KYC / refund / limit khud approve kar le
SAHI:            @PreAuthorize("hasRole('ADMIN')") / maker-checker (banane wala approve na kare)
BOL:             "Who is allowed to call approve? I don't see a role check."
★ 13-Oct chhoota (KYC: approveKyc)
```

---

<a id="s3"></a>
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

<a id="s4"></a>
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
★ 14-Oct chhoota (Loan: if (acc.getBalance() >= amt) -> setBalance)
```

```
CODE ME DIKHE:   if (repo.existsByUserId(id)) return "already";  ...  repo.save(new X(id));
NAAM:            ★ CHECK-THEN-ACT (insert wala)
ASAR:            2 request ek saath -> dono ko "nahi hai" dikha -> 2 row ban gayi
SAHI:            DB UNIQUE(user_id) + DuplicateKey pakdo -> 409
★ 13-Oct chhoota (KYC: existsByUserId -> save)
```

```
CODE ME DIKHE:   if (status == "ACTIVE")  ·  Integer a == Integer b  ·  Long == Long
NAAM:            == se object compare
ASAR:            reference compare -> chup-chaap false (Integer 127 ke upar false)
SAHI:            "ACTIVE".equals(status) / enum / equals()
★ 12-Oct chhoota (Beneficiary: otp == lastOtp -> hamesha FAILED)
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

<a id="s5"></a>
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
★ 14-Oct chhoota (Loan: POST /doPrepay)
```

```
CODE ME DIKHE:   otpService.send(...);  b.setActive(true);  repo.save(b);   (verify alag endpoint pe)
NAAM:            ★ status PEHLE, verify BAAD
ASAR:            OTP verify hone se pehle hi beneficiary / payee ACTIVE -> bina OTP paisa bhej do
SAHI:            PENDING save, verify sahi ho tab ACTIVE
★ 12-Oct chhoota (Beneficiary)
```

```
CODE ME DIKHE:   if (newLimit > 200000) newLimit = 200000;   (limit / amount chupchaap badla)
NAAM:            galat input chupchaap theek kiya + limit badhana bina OTP
ASAR:            user ko pata hi nahi kya save hua · chori ka login limit badha de
SAHI:            400 lautao · limit badhane pe OTP / cooling period
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

<a id="s6"></a>
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
CODE ME DIKHE:   s3.putObject(...);  repo.save(k);     (file pehle, DB baad)
NAAM:            bahar likha, DB fail
ASAR:            save fail -> S3 me file anaath padi, kisi row se judi nahi
SAHI:            DB pehle (PENDING) phir upload, ya fail pe file delete / cleanup job
★ 13-Oct chhoota (KYC)
```

```
CODE ME DIKHE:   new Thread(...)  ·  Thread.sleep() request ke andar
NAAM:            thread bekaabu
SAHI:            Executor / @Async
```

---

<a id="s7"></a>
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

<a id="s8"></a>
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
★ 13-Oct chhoota (KYC: catch printStackTrace, phir bhi "approved")
```

---

<a id="s9"></a>
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

<a id="s10"></a>
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
★ 13-Oct chhoota (KYC: poora Kyc entity, full Aadhaar ke saath -> DTO + mask)
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

<a id="s11"></a>
## 11. FILE / CONNECTION

```
CODE ME DIKHE:   getConnection()  ·  new FileWriter()  ·  openStream()   (close nahi)
NAAM:            resource leak
ASAR:            connection pool khatam / file adhoori
SAHI:            try-with-resources
```

---

<a id="s12"></a>
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

<a id="s13"></a>
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
