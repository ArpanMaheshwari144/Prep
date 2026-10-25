# PR REVIEW — JP SUPERDAY

Drill se pehle: table 1 (chhoot rahe) padho, table 2 pe ek nazar. Baaki table PR ke waqt upar se neeche.

## 1. CHHOOT RAHE — har PR me inhe pehle dhoondo

| # | Code me dikhe | Asar | Sahi | Chhoota |
|---|---|---|---|---|
| 1 | `if (status == ACTIVE) { pay(); setStatus(CLOSED); save }` | do click / retry = do baar paisa | `UPDATE .. SET status='CLOSING' WHERE id=? AND status='ACTIVE'` (1 row tabhi aage) + idempotency key | 21, 22, 24 Oct (25 PAKDA) |
| 2 | `catch (Exception e) { return ok("FAILED"); }` | app ko 200 mila, samjha kaam ho gaya | 502 / 4xx + status FAILED | 20, 21, 22, 24, 25 Oct |
| 3 | `@Transactional` method me `bankClient.post(..)` | bank dheema = DB connection pakda, pool khatam | call txn ke BAHAR + timeout + PENDING | 21, 22, 24 Oct (24 ko timeout pakda, "bahar" nahi; 25 PAKDA) |
| 4 | review ke end me FAISLA line nahi | review adhoora | "Request changes: bhaari 1, 2, 3. Chhote: .." | 24 Oct (25 PAKDA) |

## 2. PAKDE JAA RAHE — PR me mat do

| Cheez | Pakda |
|---|---|
| owner check nahi (IDOR, 403) | 22, 24 Oct |
| request body se userId (spoof) | 21 Oct |
| bahar ki call pe timeout nahi | 24 Oct |
| enum pe `==` SAHI hai (bug nahi) | 24 Oct |
| paisa `double` me -> BigDecimal | 24 Oct |
| `println` -> logger | 24 Oct |
| magic string -> enum / constant | 24 Oct |
| `@Transactional` private / self call · field injection · SQL injection · catch nigalna · singleton me field · log me PII · amount client se · entity bahar | 19-22 Oct |

Niyam: table 1 wala do drill lagaataar pakda -> table 2 me. Table 2 wala kabhi chhoote -> wapas table 1 me.

---

## 3. CHECKLIST — PR upar se neeche, har jagah ruko

| # | Jagah | Kya dekhna |
|---|---|---|
| 1 | FIELDS | method ke bahar non-final field? volatile counter? secret? static SimpleDateFormat? |
| 2 | ID / ROLE | `{id}` aaya -> IS user ka? approve / admin -> role check? |
| 3 | INPUT | `@Valid`? amount -ve / 0? paisa double me? |
| 4 | 2 REQUEST | pehle check phir save -> do request saath = dono andar · retry = do baar |
| 5 | SAVE | 2+ save -> `@Transactional`? public, service pe? status pehle, verify baad? |
| 6 | BAHAR | HTTP / SMS / S3 -> timeout? txn ke andar? DB fail to file anaath? |
| 7 | LOOP | loop me query / HTTP? string `+=`? |
| 8 | CATCH | nigla? uske baad "SUCCESS" / 200? rollback ruka? |
| 9 | LOG | println? PAN / card / token log me? |
| 10 | RETURN | fail pe 200? entity bahar? list bina page? `.get()` -> 500? |
| 11 | STYLE | naam · magic string · SRP · field injection |
| 12 | FAISLA | Request changes + 3 bhaari naam + chhote |

Drill column: **CHHOOT** = abhi chhoot raha (table 1) · **PAKDA** = table 2 · **purana** = shuru ke drill me chhoota, ab drill me nahi aaya · khaali = drill me aaya hi nahi.

## 4. FIELDS (method ke bahar)

| Code me dikhe | Naam | Asar | Sahi | Drill |
|---|---|---|---|---|
| `@Autowired private Repo repo;` | field injection | dependency chhupi, test me mock mushkil | constructor injection + final | PAKDA |
| `API_KEY = "sk_live.."` · URL me key | hardcoded creds | git me leak, access log me bhi | env / Vault, key header me | |
| `private String currentUser;` (non-final) | singleton me state | saare user ek object share -> data mix | local variable | PAKDA |
| `static Map` · `static SimpleDateFormat` | shared mutable static | thread share, galat date | local / ConcurrentHashMap / DateTimeFormatter | |
| `volatile int c; c++` · `boolean running` | visibility / non-atomic | ginti khoyi; volatile sirf dikhata, atomic nahi | flag -> volatile · counter -> AtomicInteger | purana |
| `flag = true; kaam; flag = false;` | reset finally me nahi | exception pe flag phansa | `finally { flag = false; }` | |

## 5. METHOD KI PEHLI LINE (annotation + params)

| Code me dikhe | Naam | Asar | Sahi | Drill |
|---|---|---|---|---|
| `/{accountId}` · `walletId` · `fdId` | OWNER CHECK nahi (IDOR) | kisi ka bhi account / paisa | token ka user = owner? nahi to 403, HAR method pe | PAKDA |
| `orderId` + `itemId` | parent-child check nahi | doosre ke order ka item badla | item is order ka hai? | |
| `@RequestBody r` (`@Valid` nahi) | validation band | DTO ke `@NotNull` chalte hi nahi | `@Valid @RequestBody` | purana |
| `?otp=` · `?password=` · `?apiKey=` | secret URL me | log / history me chhapa | body ya header | |
| `double amt = r.getAmount();` | -ve / 0 check nahi | -ve withdraw = balance badha | `@Positive` / `amt <= 0` -> 400 | purana |
| `double balance` · `float price` | paisa floating me | 0.1 + 0.2 != 0.3 | BigDecimal / long paise | PAKDA |
| `@Transactional` private / `this.save()` / controller pe | galat jagah txn | proxy bypass -> txn chala hi nahi | service, public, doosri bean se call | PAKDA |
| `/approve` · `/admin` (`@PreAuthorize` nahi) | ROLE CHECK nahi | user apna KYC / refund khud approve | `hasRole('ADMIN')` / maker-checker | purana |

## 6. DB SE LAANA

| Code me dikhe | Naam | Asar | Sahi | Drill |
|---|---|---|---|---|
| `"..LIKE '%" + q + "%'"` | SQL injection | `' OR 1=1 --` = poori table | PreparedStatement + `?` | PAKDA |
| `findAll()` · `SELECT *` bina WHERE | poori table memory me | 10 lakh row -> OOM | WHERE / Pageable | |
| `findById(id).get()` · `list.get(0)` | khaali case nahi | NoSuchElement -> 500 | `orElseThrow(NotFound)` -> 404 | |
| `User u = repo.find(id); u.getName();` | null check nahi | NPE -> 500 | Optional / null check | |

## 7. IF / COMPARE

| Code me dikhe | Naam | Asar | Sahi | Drill |
|---|---|---|---|---|
| `if (bal >= amt) { setBalance(bal - amt); save }` | check-then-act (double spend) | do request saath = dono ne kaata | `UPDATE .. SET bal = bal - ? WHERE id=? AND bal >= ?` / `@Version` / FOR UPDATE | CHHOOT |
| `if (existsByUserId) return; save(new X)` | check-then-act (insert) | do row ban gayi | DB UNIQUE + DuplicateKey -> 409 | purana |
| `status == "ACTIVE"` · `Integer == Integer` | `==` se object | chup-chaap false (Integer 127 ke upar) | `.equals()` / enum (enum pe `==` sahi) | PAKDA |
| `if (running) return; running = true;` | check-then-act (flag) | do thread andar | `AtomicBoolean.compareAndSet` | |
| `if (type.equals("UPI")) .. else if ..` | type pe if-else | naya type = method badlo | strategy pattern | |

Check-then-act (do alag request ek waqt) aur idempotency (same request do baar) alag cheez hain.

## 8. DO WRITE / PAISA BADLA

| Code me dikhe | Naam | Asar | Sahi | Drill |
|---|---|---|---|---|
| `save(from); save(to);` bina `@Transactional` | txn nahi | aadha likha: kata, pahuncha nahi | service method pe `@Transactional` | purana |
| `POST /pay` · `/refund` bina unique key | idempotency nahi | retry = do baar charge | Idempotency-Key header + DB unique | CHHOOT |
| `send OTP; setActive(true); save` | status pehle, verify baad | bina OTP ACTIVE | PENDING save, verify pe ACTIVE | purana |
| `if (newLimit > 200000) newLimit = 200000;` | input chup-chaap badla | user ko pata nahi kya save hua | 400 + limit badhane pe OTP | |
| `throws IOException` `@Transactional` se | checked pe rollback nahi | data aadha commit | `rollbackFor = Exception.class` | |
| ek hi query wala method | - | - | yahan `@Transactional` mat maango | |

## 9. BAHAR KI CALL (HTTP / mail / SMS / Kafka / S3)

| Code me dikhe | Naam | Asar | Sahi | Drill |
|---|---|---|---|---|
| `rt.postForObject(URL, ..)` timeout nahi | timeout nahi | vendor atka -> pool khatam -> app giri | timeout + retry limit + circuit breaker | PAKDA |
| paisa wale txn / try me SMS / bank call | bahar ki call flow ke andar | txn me: connection pakda · try me: SMS fail = transfer fail bata diya | commit ke BAAD / @Async / outbox | CHHOOT |
| `s3.putObject(); repo.save();` | bahar likha, DB fail | S3 me file anaath | DB pehle (PENDING), phir upload / cleanup job | purana |
| `new Thread()` · `Thread.sleep()` request me | thread bekaabu | - | Executor / @Async | |

## 10. LOOP · CATCH · LOG

| Code me dikhe | Naam | Asar | Sahi | Drill |
|---|---|---|---|---|
| loop me `repo.findById()` | N+1 | 100 item = 101 query | IN (...) / JOIN / batch | |
| loop me `s = s + x` | string concat | dheema | StringBuilder | |
| `catch (Exception e) { println }` · `catch {}` | exception nigla | prod me pata nahi kya toota | specific exception + `log.error(.., e)` | PAKDA |
| `@Transactional` me catch, rethrow nahi | catch ne rollback roka | commit ho gaya | rethrow | |
| `catch { ..; return "SUCCESS"; }` | fail pe bhi OK | client samjha ho gaya | 500 / rethrow | CHHOOT |
| `println(email / card / PAN / token)` | PII log me | log padhne wala data dekhe | sirf id / masked | PAKDA |
| `System.out.println` | println prod me | - | SLF4J logger | PAKDA |

## 11. RETURN · FILE

| Code me dikhe | Naam | Asar | Sahi | Drill |
|---|---|---|---|---|
| `public String transfer()` · `return "OK"` | String return | status hamesha 200 | ResponseEntity + sahi status | |
| `return "FAILED"` · kam balance pe normal return | kaam na hua phir bhi 200 | client ne 200 dekha | 400 / 422 | CHHOOT |
| `return List<Entity>` · bina Pageable | entity bahar / page nahi | password hash bhi bahar; 10 lakh row | DTO + Pageable (+ mask) | PAKDA |
| `body(e.getMessage())` | andar ki baat client ko | - | generic message, detail log me | |
| `setPassword(req.getPassword())` | plaintext password | - | BCrypt | |
| `getConnection()` · `new FileWriter()` close nahi | resource leak | pool khatam | try-with-resources | |

## 12. STYLE (aakhri 1 min)

| Dikhe | Sahi |
|---|---|
| naam `w` `r` `res` `x` `data` `doIt` | kaam batata naam |
| public fields DTO me | private + getter |
| magic `"ACTIVE"` / `5` / `0.01` | enum / constant |
| lamba method / gehri nesting | early return, chhote method |
| copy-paste · nikala par use nahi | ek method · hatao |
| `new XService()` andar | inject karo |
| controller me DB + logic + mail | controller / service / repo alag |

## 13. FAISLA — hamesha, 3 naam ke saath

```
Request changes. Three blockers: <1>, <2>, <3>. Then smaller ones: naming, println, magic strings.
```

Jo pakda wo likho, dimaag me pakda gina nahi jaata. Har bhaari ke saath ek line asar. Gayab cheez pe sawaal:
"I don't see an owner check here — is it handled in a filter?"
