# Banking: hand notes (mock se pehle ye padho)

### Shuru

**Kaam:** account me paisa daalo, nikaalo, balance aur history dekho, aur ek account se doosre me transfer, dono isi bank me. Doosra bank, loan, card scope ke bahar hain, ye pehle hi poochh ke hata do.

**Sabse upar:** SAHI hona hai: paisa na gum ho, na bane, har paisa trace ho, dobara bhejne pe dobara na kate, aur balance dekhna tez ho.

**Numbers:** 50 lakh account aur roz 1 crore transaction hain, yaani ~120 likhai har second. Ye bahut kam hai, ek Postgres hazaaron likhai / second aaraam se karta hai. Matlab yahan bheed badi dikkat nahi, sahi hona badi dikkat hai, isliye shard nahi, ek SQL DB.

&nbsp;

**1. SERVICE + SQL DB**

Sabse seedha: ek Banking Service aur ek SQL DB. Arpan ne Ravi ko 500 bheje, to do UPDATE chale: Arpan -500, Ravi +500. SQL isliye ki ek transfer me do row ek saath pakka badalni hain (ACID), aur bheed itni chhoti hai ki NoSQL ki "kai machine" wali taakat chahiye hi nahi.

**Owner check:** userId JWT se lo, body se nahi. from_account ka maalik nahi to 403.

&nbsp;

**2. TRANSACTION (WAL)**

Pehla UPDATE hua, aur doosre se pehle server gir gaya. Arpan ke 500 kate, Ravi ko kuch nahi mila. Isliye dono UPDATE ek **TRANSACTION** me: ya dono, ya koi nahi. DB ko ulta karna **WAL** se pata chalta hai. DB har badlaav pehle log me likhta hai, restart pe jiska COMMIT ka record log me nahi, wo badlaav kabhi dikhta hi nahi. Saga nahi lagaya, kyunki saga tab chahiye jab paisa do alag DB me ho, aur yahan dono account ek hi DB me hain.

&nbsp;

**3. IDEMPOTENCY KEY**

Transfer ho gaya, par jawab aane se pehle metro me network gaya. App ne dobara bheja, aur 1000 kat gaye. Isliye **IDEMPOTENCY KEY**.

**Key kab banti:** app Send wali screen khulte hi ek id (k-77) banati hai, aur dobara bhejne pe bhi wahi id jaati hai (`POST /transfers` + Idempotency-Key header).

**Server kya karta:** server use usi transaction me UNIQUE wali table me likhta hai. Dobara aaya to INSERT fail, paisa nahi katta, purana jawab wapas.

**UNIQUE kyun:** "pehle dekho phir likho" me do retry ek saath "nahi hai" dekh lete.

**Amount se kyun nahi:** amount ke hisaab se block nahi, kyunki Arpan sach me do baar 500 bhej sakta hai.

**Maan lo wahi key, amount alag aaya:** key ke saath request ka hash rakho, mel nahi khaata to 422.

&nbsp;

**4. CHECK + KAATO EK QUERY**

Arpan ke paas 600 the. Netflix ka 500 auto-debit aur 500 ka UPI ek saath aaye. Dono ne 600 padha, dono kat gaye, balance -400. Isliye **CHECK + KAATO EK QUERY** me: `balance - 500 WHERE balance >= 500`. UPDATE row pe lock lagata hai, to doosri query rukti hai, phir naye balance (100) pe jaanchti hai aur 0 row milti hai, yaani mana. Ulti disha ke do transfer pe deadlock ho to Postgres ek ko tod deta hai, app dobara chalati hai, aur key ki wajah se dohra nahi katta.

&nbsp;

**5. LEDGER (DOUBLE-ENTRY)**

Ravi ne complaint daali "500 nahi mile", aur bank ko saboot dena tha ki 500 kab, kahan se, kahan gaye.

**Kyun:** DB me sirf aakhri balance tha, kyunki UPDATE purana number mita deta hai, aur logs kuch din me mit jaate hain.

**Double-entry:** isliye **LEDGER (khaata)**, **DOUBLE-ENTRY** me, usi transaction me: har transfer ki do line, -500 Arpan, +500 Ravi. Poore khaate ka jod hamesha 0 aana chahiye.

**Append-only:** balance column UPDATE hota hai, par ledger ek alag table hai jo kabhi badli nahi jaati, usme sirf nayi line judti hai (append). Galti ho to ulti line daalo, mitao nahi.

**Sach kaun:** khaata hi sach hai, balance column bas tez padhne ki copy hai. Ledger isi DB ki ek table hai, poori DB nahi.

**Kis kaam aata:** isi ek kitaab se app ki history aur PDF statement, raat ka milaan, aur RBI / audit ki report banti hai (RBI ke format me, ledger se). Poori DB ki copy kisi ko nahi jaati.

&nbsp;

**6. OUTBOX + RELAY**

Ravi ko SMS jaana tha, aur SMS service giri hui thi. Transaction ke andar bhejo to transfer atak jaata hai, COMMIT ke baad bhejo to beech me gira server SMS kha jaata hai. Isliye **OUTBOX**: SMS ki khabar usi transaction me outbox table me (sent = false). Ek **RELAY** use uthata hai, SMS Service ko bhejta hai, gira ho to dobara koshish karta hai. Kafka nahi, kyunki abhi sirf SMS ko khabar chahiye. Kai service ko chahiye hoti to relay Kafka me daalta.

&nbsp;

**7. LB + SYNC COPY**

11 baje sabki salary aayi, ek machine nahi jhel paayi, aur DB bhi ek hi tha. Isliye kai Banking Service machine aur aage **LB**. Ye isliye chalta hai ki service kuch yaad nahi rakhti (stateless). Aur DB ki **SYNC COPY** doosri building me (RDS Multi-AZ). Sync isliye ki async me primary gira to aakhri transfer kho jaate. Paise me "thoda dheema" chalta hai, "kho gaya" nahi.

&nbsp;

**8. READ REPLICA**

Salary aate hi sab balance dekh rahe the, padhai bhi primary pe thi, aur transfer dheeme ho gaye. Isliye **READ REPLICA** (async), balance aur history wahan se (`GET /accounts/{id}/balance` · history me cursor (keyset, OFFSET nahi)). Pench ye hai ki Arpan ne abhi bheja aur replica peeche thi, to purana balance dikha. Ilaaj: jisne abhi transfer kiya uska balance kuch second primary se padho, ya transfer ke jawab me hi naya balance bhej do.

**CP ya AP:** transfer / balance CP hain (galat balance dikhane se mana karna behtar). SMS / statement AP hain (2 sec late chalega).

&nbsp;

**9. RAAT KA MILAAN + SNAPSHOT**

Kisi bug ya haath ke UPDATE se balance 600 dikha raha tha, par khaate ka jod 100 tha, aur kisi ko pata nahi chala. Isliye **RAAT KA MILAAN (reconciliation)**, replica pe: har account ka balance khaate ke jod ke barabar hai? Poore bank ka jod 0 hai? Farak mila to khud theek nahi, ALERT, insaan dekhe. 5 saal ki lines roz jodna bhaari hai, isliye mahine ke end me **SNAPSHOT**: balance = snapshot + uske baad ki lines.

&nbsp;

**10. MAHINE KE TUKDE + S3**

5 saal me ~36 arab line ho gayi, index RAM me nahi samaata, aur RBI ki wajah se phenk bhi nahi sakte. Isliye **MAHINE KE TUKDE (partition)**: query sirf us mahine ka tukda dekhti hai. 1 saal se purane tukde **S3** me, Object Lock ke saath, taaki koi badal na sake. Shard nahi kiya, kyunki likhai 120 / sec hai, ek machine kaafi hai. Aur shard me Arpan aur Ravi alag machine pe ho jaate, to ek transaction hi nahi chalta.

**Maan lo 10 guna traffic:** padhai badhi to replica, jagah badhi to tukde + S3, likhai badhi tab account_id se shard (do shard ke beech transfer = saga).

**AWS pe:** purani lines **S3** me **Object Lock** ke saath (koi badal ya mita na sake), aur kabhi padhna ho to **Athena** se seedha S3 pe query.

&nbsp;

### Wrap

sahi = transaction + ek query + double-entry + milaan · trace = ledger sirf INSERT + S3 lock · dobara na kate = idempotency key · tez = read replica · gire to chale = LB + sync copy + outbox

**Chal raha hai kaise pata:** milaan ka alert, p99, outbox me kitni sent = false, har transfer ka trace id.


Poora design (har kadam ka diagram aur "kyun") = [13_banking_system.md](13_banking_system.md)
