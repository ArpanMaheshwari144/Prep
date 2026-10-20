# Mini Banking System (accounts · transactions · balances)

> Ek bank ke andar accounts, un pe paisa daalo / nikaalo / bhejo, aur balance dikhao.
> Dil: khaata (ledger) sach hai, balance sirf uska jod. Aur paisa na bane, na gayab ho.
>
> Mock me sirf heading + Cue + GIF. Neeche har card: **hal** · **kyun** · **nahi** (doosra kyun nahi) · **Pooche**.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c11_2-defadb32.svg)

---


## Shuru

- **FR:** user ke ek ya kai account · deposit, withdraw, transfer A se B (dono isi bank), balance, history · bahar: doosra bank, loan, byaaj, card, KYC
- **NFR:** sahi hona sabse upar (paisa na gum, na bane, hisaab har waqt barabar) · har paisa trace (audit) · dobara bhejne pe dobara na kate (idempotent) · balance dekhna tez
- **numbers:** 50 lakh account, 1 crore transaction roz = ~120 likhai / sec, padhna kahin zyada. 120 / sec = ek Postgres ka dasva hissa -> shard nahi, primary + replica kaafi

---

*ab tootega kahan? sabse pehle transfer ke beech me, kyunki ek transfer me do row badalti hain*

## 1. Maine Ravi ko 500 bheje, beech me server gira

**Cue:** EK TRANSACTION (WAL) · dabba: SQL DB

- **dabba 0:** sabse simple, ek Banking Service (saara niyam / logic) + peeche ek SQL DB, jaan-boojh ke seedha
- **scope pehle:** "ek hi bank ke andar, ya doosre bank ko bhi?" warna interviewer jitna bada chaahe sar pe daal dega
- **scope:** "dono isi bank" ye ek shabd poora design badalta
- **sawaal:** transfer me do row badalti, dono ho ya koi nahi, kaun sambhalega? -> ye card
- **toota:** mera -500 hua, server gira, Ravi ka +500 hua hi nahi -> 500 gayab
- **hal:** BEGIN, Arpan -500, Ravi +500, COMMIT -> dono ya koi nahi (transaction)
- **kyun:** ek DB me ek transaction kabhi aadha nahi. Crash pe DB apne log se khud ulta karta.
- **kyun:** padhne wale ko aadha kaam kabhi nahi dikhta
- **nahi:** SAGA -> sab ek hi DB me, ek transaction kaafi. SAGA tab jab paisa do alag DB me.
- **Pooche:** beech me server gira, DB kaise jaanta? -> log (WAL). COMMIT likha to poora, nahi to ulta, mera wapas 1000
- **Pooche:** -500 ke baad, COMMIT se pehle, koi padhe? -> COMMIT se pehle wala hi dikhta, aadha kisi ko nahi
- **Pooche:** Ravi doosre bank me? -> do bank = do DB, ek transaction nahi. PENDING + SAGA + raat ko milaan.

---

*ab ek transfer to pakka, par wahi transfer do baar aa gaya to?*

## 2. Send dabaya, network gaya, app ne dobara bheja

**Cue:** IDEMPOTENCY KEY (UNIQUE) · dabba: Banking Svc (+ key)

- **toota:** koi id nahi, timeout pe app ne dobara bheja, server ne do transfer samjhe -> 1000 kate
- **hal:** screen khulte hi app id banata (T-77), send pe wahi id DB me UNIQUE ke saath (idempotency key)
- **hal:** dobara T-77 aayi -> pehla jawab wapas, paisa doosri baar nahi katta
- **kyun:** aadha nahi = transaction ka kaam. Do baar nahi = id ka kaam.
- **kyun:** id app banata, server nahi, kyunki dobara bhejne wala wahi id laata. Wahi id = wahi jawab, retry safe.
- **nahi:** send button band -> retry app khud karta (timeout pe), user ke dabaye bina
- **Pooche:** dono request ek saath? -> DB khud rokta, doosra insert fail, UNIQUE hi taala
- **Pooche:** wahi id, amount 5000? -> request ka hash nahi mila -> 422, purana jawab nahi
- **Pooche:** id kab tak? -> 24-48 ghante, phir saaf. Retry itni der me hi aata.

---

*ab do alag transfer ek hi account pe ek saath aaye to?*

## 3. 9 baje Netflix ka auto-debit aur mera UPI ek saath

**Cue:** CHECK + KAATNA EK QUERY · dabba: (SQL DB: balance >= x)

- **sawaal:** ek account pe do transfer ek saath, kaun rokega? balance minus na jaaye, niyam kahan? -> ye card
- **toota:** 300 the, Netflix 200 + UPI 200, dono ne 300 padha (pass), dono ne kaata -> -100
- **hal:** check + kaatna ek query (atomic UPDATE): `UPDATE account SET balance = balance - 200 WHERE id = Arpan AND balance >= 200`
- **kyun:** row pe taala, dono line me. Pehla 300 se 100. Doosra: 100 >= 200? nahi -> 0 row, kuch nahi kata, 422 "paisa kam".
- **kyun:** balance kabhi minus nahi
- **nahi:** pehle padho, phir kaato -> dono ne 300 dekha, beech me doosra ghus jaata
- **nahi:** niyam Java me -> raaste kai (API, batch, haath se), DB ek. Niyam DB me to har raasta usse guzarta.

---

*ab paisa sahi kata, par support poochhe paisa gaya kahan?*

## 4. Maine poochha: mere 500 kahan gaye, kab?

**Cue:** KHAATA (ledger) usi transaction me · dabba: (SQL DB: ledger table)

- **toota:** sirf balance column, 1000 dikhta, 500 kahan / kab koi hisaab nahi, support "pata nahi"
- **hal:** har transfer ki 2 line khaate me, usi transaction me: Arpan -500, Ravi +500 (ledger). Sirf jodo, kabhi badlo nahi.
- **kyun:** transaction wapas (paisa kam) to khaata bhi wapas, jhoothi line bachti nahi
- **kyun:** har line me waqt + transfer id -> support dekhta: 8:00, T-77, Ravi ko 500
- **nahi:** sirf balance column -> jod dikhta, raasta nahi, purana hisaab mit jaata
- **nahi:** khaata Kafka me -> Kafka + DB ek transaction nahi, rollback pe Kafka me jhoothi line
- **Pooche:** galat line likhi? -> UPDATE nahi, ulti line jodo, galti bhi khaate me dikhe
- **Pooche:** support ko kab, kisko, kyun? -> har line me waqt + transfer id, sab ek jagah

---

*ab paisa pakka gaya, par user ko khabar (SMS) bhi to jaani hai*

## 5. Salary aayi, Ravi ko rent bheja, SMS service giri

**Cue:** OUTBOX, Kafka key = account_id · dabba: Kafka

- **toota:** transfer karte hi SMS bhejte, Ravi ko 20,000 gaye, SMS service band -> SMS kabhi nahi gaya
- **hal:** DB me outbox table, khabar usi transaction me (sent=false)
- **hal:** relay har second outbox dekhe, sent=false Kafka me (COMMIT ke ~1 sec baad). SMS service padhe, bheje, sent=true.
- **kyun:** transfer SMS pe nahi rukta. Kafka tak COMMIT ke baad hi -> khabar tabhi jab paisa pakka.
- **kyun:** DB gira to bhi khabar outbox me, kuch nahi khota
- **nahi:** seedha SMS service bulao -> service band thi, khabar gayi
- **nahi:** pehle Kafka, DB baad me -> "Ravi ko 20,000 aaye" nikla, DB fail, paisa gaya hi nahi. Paisa + khabar ek saath DB me.
- **Pooche:** SMS service ghanta bhar giri? -> khabar Kafka me padi, wapas aate hi wahin se
- **Pooche:** relay ne Kafka me daala, sent=true se pehle gira? -> khabar dobara, SMS service eventId dekh ke skip
- **Pooche:** outbox bharti jaaye? -> roz saaf, sent=true ka kaam khatam
- **Pooche:** relay likhna hi na pade? -> DB ke log se seedha Kafka (CDC, Debezium)
- **Pooche:** SMS ulte kram me? -> Kafka key = account_id, ek account ek partition, kram se

---

*ab khaata bhi hai aur balance column bhi, dono me farak aaya to?*

## 6. Salary ke baad maine balance khola

**Cue:** BALANCE = CACHE, raat RECONCILIATION · dabba: Reconciliation job

- **toota:** column aur khaata alag (bug): khaata 29,800, column 30,300, kisi ne pakda nahi
- **hal:** khaata sach, balance column uska jod (cache), usi transaction me update
- **hal:** raat job milata: khaata ka jod (snapshot + baad ki line) == column? (reconciliation)
- **kyun:** balance sabse zyada dekha jaata, har baar 50,000 line jodna dheema -> column. Par sach ek jagah, khaate me.
- **nahi:** column hi mat rakho, har baar khaate se jodo -> har baar 50,000 line, dheema
- **nahi:** column hi sach -> galat hua, pata nahi chala, milane ko kuch tha hi nahi
- **Pooche:** raat farak mila, 29,800 vs 30,300? -> alert + account roko. Pehle KYUN, phir khaata jeete.

---

*ab sahi hai, par 11 baje sabki salary ek saath aayi*

## 7. 11 baje sabki salary aa gayi, ek Banking service bhar gayi

**Cue:** KAI MACHINE + LB, DB sync copy · dabba: LB + Banking Svc x N

- **toota:** ek machine, laakhon transfer, CPU bhara, giri, koi paisa nahi bhej pa raha
- **hal:** kai machine + aage LB, machine kuch yaad nahi rakhti (stateless). DB ki sync copy doosri building me.
- **kyun:** sach DB me, koi bhi machine koi bhi transfer le. Jitni chaho lagao, ek giri to baaki chalti.
- **nahi:** ek badi machine -> ek hi thi, bhari aur giri
- **Pooche:** ek machine giri? -> LB health check se list se hata deta
- **Pooche:** transfer adhoora? -> app WAHI id se dobara, do baar nahi katta (card 2)
- **Pooche:** LB akela? -> nahi, LB bhi do zone me
- **Pooche:** main DB gira? -> sync copy (doosri building) promote, ~1-2 min (Multi-AZ). Beech me error to WAHI id se dobara.
- **Pooche:** sync copy = dheema? -> haan, ek network chakkar, paise me chalega. Async me aakhri transfer kho sakta.
- **Pooche:** 11 baje ek dukaan pe 50,000 payment ek saath, ek row pe lock ki line? -> credit ledger line me (sirf INSERT, lock nahi)
- **Pooche:** phir dukaan ka balance? -> column har second batch me jodo. Atomic check sirf debit pe, credit pe nahi.

---

*ab machine kaafi, par bheed padhne wali hai*

## 8. Salary ki bheed me sab balance dekh rahe, transfer dheeme

**Cue:** READ REPLICA + read-your-own-writes · dabba: Read replica

- **toota:** padhna + likhna ek DB pe, laakhon balance / history padh rahe, primary pe dono bojh, transfer dheema
- **hal:** padhna copy se, likhna sirf primary pe (read replica). Primary ka log (WAL) copy tak jaata.
- **kyun:** bheed padhne ki, copy le leti. Primary pe likhna + paise ka faisla, transfer tez.
- **kyun:** paise ka faisla sirf primary pe, copy thodi peeche ho sakti
- **nahi:** sab primary se -> padhne ki bheed ne transfer dheema kiya
- **Pooche:** abhi bheja, copy 5 sec peeche? -> apna abhi likha primary se (last_write_at Redis me), warna purana balance dekh user dobara bhejega
- **Pooche:** copy minute bhar peeche? -> lag naapo, alert, us copy ko list se hatao
- **Pooche:** failover copy = read replica? -> nahi. Failover standby sync, kuch nahi khota. Read replica async, peeche.

---

*ab ek alag dikkat: do transfer ek doosre ke ulte*

## 9. Ravi ki dukaan ko 500, usi second dukaan se 200 refund (deadlock)

**Cue:** CHHOTI ID PEHLE LOCK · dabba: (SQL DB: lock kram)

- **farak:** card 3 me paisa do baar kat raha tha (double spend). Yahan dono ek doosre ke intezaar me atke.
- **toota:** Txn1 (Arpan se dukaan) ne Arpan pakda, dukaan ka intezaar. Txn2 (refund) ne dukaan pakdi, Arpan ka intezaar. Gol ghera (deadlock).
- **hal:** taala hamesha ek kram me, chhoti id pehle (lock order): dono pehle 7 (Arpan), phir 9 (dukaan)
- **kyun:** ek hi kram -> ghera banta hi nahi. Txn2 7 pe ruka, Txn1 poora, phir Txn2.
- **nahi:** jo pehle aaye wo pehle pakde -> ulte kram me pakda, dono hamesha atke
- **Pooche:** kram ke baad bhi phansa? -> DB ghera khud pakadta, ek ko wapas karta. App ruk ke WAHI id se dobara.

---

*ab sab chal raha, par purana hisaab maanga gaya*

## 10. Shaam ko dukaan ke CA ka export, page 5000 pe atka

**Cue:** KEYSET (ts, id) + index · dabba: (SQL DB: index)

- **toota:** page 5000 = LIMIT 20 OFFSET 100000, DB 1 lakh line padh ke phenkta, export bahut dheema
- **hal:** pichhle page ki aakhri line yaad, agla usse aage (keyset), index ke saath:
  `WHERE account_id = ? AND (ts, id) < (aakhri ts, id) ORDER BY ts DESC, id DESC LIMIT 20`
- **kyun:** "kahan tak dekha" yaad, "kitne chhode" nahi. Index se seedha wahan, sirf 20. Page 1 ho ya 5000, ek jaisa.
- **nahi:** OFFSET -> har page pe pichhli saari line padh ke phenko
- **nahi:** OFFSET me beech me nayi line -> page khisakta, line dobara aati
- **Pooche:** ek hi ts pe do line? -> isliye (ts, id), id barabari todti, line na chhoote na dohraye
- **Pooche:** beech me nayi line? -> purana page nahi khisakta, "kahan tak" yaad hai
- **Pooche:** (ts, id) app ko kaise? -> band lifafe me (next_cursor, base64), app andar nahi dekhti
- **Pooche:** seedha page 500? -> keyset me nahi hota, agla / pichhla kaafi

---

*ab saal guzre, khaata bas badhta hi ja raha*

## 11. Raat ko: 5 saal ka khaata ek hi table me

**Cue:** MAHINE KE TUKDE + S3, snapshot · dabba: Archive (S3)

- **numbers:** khaata 1 crore x 2 line x 365 = ~730 crore line saal me, 3 saal ~22 arab, 5 saal ~36 arab
- **toota:** 5 saal ki arabon line ek table me, roz ki query + backup saari line chhoote, dheema + mehnga
- **hal:** mahine ke tukde (partition), is mahine ke paas aur tez. Mahina band -> alag karke S3 (archive), sasta.
- **kyun:** table me sirf haal ke mahine, chhoti. Naya paas, purana sasta.
- **kyun:** balance ka jod 5 saal se nahi, mahine ka snapshot + baad ki line
- **nahi:** purana delete -> 5 saal (PMLA) se pehle nahi, bank ka niyam
- **Pooche:** line S3 gayi, balance ka jod? -> poora nahi, mahine ka snapshot + baad ki line
- **Pooche:** RBI ko 2021 ka statement? -> S3 pe hi query (Athena, Parquet), ya tukda wapas laao
- **Pooche:** koi purani file badle? -> S3 pe taala, na delete na badal (Object Lock, WORM)

---

## Poora design ek saans me

```
User -> LB -> Banking Svc (stateless, idempotency check)
     -> SQL DB: EK transaction (dono khaata line + dono balance + idempotency key + outbox)
     -> Kafka (COMMIT ke baad, outbox relay se: SMS / fraud / analytics / statement)
     -> Read replica = balance / history
Reconciliation (raat: snapshot + baad ki line vs balance -> ALERT)
Archive S3 (band mahine, had se pehle delete nahi)
DB: SQL, ek DB me ek transaction (SAGA nahi) · khaata = sach (sirf jodo, double-entry, mahine ke tukde)
    · balance = cache (jod, usi transaction me)
```

Bolne ke liye: ek transfer ka sab kuch, dono khaata line, dono balance, id, aur SMS wali khabar, ek transaction me,
SQL me. Paisa na khota na banta. Khaata sach, balance uska jod, raat ko milaan. Balance check aur lock ka kram DB me.
Padhai copy se, history "pichhli line ke baad se", purane mahine sasti jagah, aur SMS COMMIT ke baad Kafka se.

---

## 10 guna traffic

Kram: sasta pehle, shard aakhir me.

- **Read replica**: pehla kadam, kyunki padhna likhne se bahut zyada. Keemat: copy thodi peeche, isliye apna
  abhi-kiya transfer primary se padho.
- **Mahine ke tukde**: archive aur query dono ke liye. Badi table (jagah) ka ilaaj yahi, kanoon ki had se pehle delete nahi.
- **Shard by account_id**: aakhri, keemat badi. Arpan shard-1 pe, Ravi shard-2 pe, to transfer ek transaction nahi
  rehta, wapas SAGA / 2PC, aur beech me crash wali dikkat wapas. Hamare 120 / sec pe ye sawaal hi nahi, pehle badi
  machine + replica.
- **Likhai se DB maar kha raha**: cache / index sirf padhai ka ilaaj, har naya index INSERT dheema karta. Likhai ke
  liye shard by account_id, ek account ki khaata line ek shard pe. Zaroori-nahi wali likhai (statement, SMS) queue pe.
- "Badi transaction chhoti karo" galat hai: kaatna + daalna ek transaction me hi.
- Replica sirf padhai baantti, likhai ke liye shard. Desh / date bura key hai, ek garam ho jaata.

**Pooche: replica laga di, par likhai abhi bhi DB ko maar rahi, ab?** Copy sirf padhai ka bojh baantti, likhai sab
main DB pe hi jaati. Index bhi sirf padhai tez karta, likhai ulta dheemi. Likhai bahut ho to account id se baanto:
Arpan ki saari khaata line ek tukde pe. Do tukdon ke beech transfer SAGA se. SMS / statement jaisa kaam queue me.
Jagah ke liye mahine ke tukde, purane sasti jagah. Kanoon ki had (5+ saal) se pehle delete nahi.

**Pooche: 10 guna kaise?** Pehle jo toote: padhai -> replica, jagah -> tukde, likhai -> shard.

**Pooche: single point of failure?** DB primary (sync copy), Banking Svc (LB + kai machine).

---

## Pooche to (kisi card me nahi baitha)

API: GET /accounts/{id}/balance · GET /accounts/{id}/transactions?cursor=...&limit=20 · POST /transfers { from, to,
amount } · POST /accounts/{id}/deposit { amount } · POST /accounts/{id}/withdraw { amount }. Har POST me
Idempotency-Key header.

Har request pe owner check: userId JWT se, body / URL se nahi. from_account ka maalik nahi to 403 (Ravi "Arpan ke
account se 500" bheje to). Balance / history pe bhi.

Tables: ledger_entries (sach, sirf jodo, mahine ke tukde) · accounts.balance (cache, jod) · idempotency_keys
(UNIQUE) · outbox · account_balance_snapshot (mahine ke aakhir ka balance).

Transfer me CP ya AP? Transfer / balance = CP, galat balance dikhane se mana karna behtar. SMS / statement = AP,
2 sec late chalta.

Sab chal raha, pata kaise? Alert: reconciliation + p99 + error rate + queue lag + DB connection, aur har transfer
ka trace id.

Ye design kyun: candidates batate payment flow, transaction ledger, ATM, do account ke beech bank ke andar
transfer. JP ka sabse sambhavit HLD.

Paanch cheez jinpe ye round tay hota: idempotency, transactions, queues, reconciliation, audit. Saath me ACID vs
eventual (network toote to sahi hona chuno, chalte rehne ki keemat pe), aur asli khaate ke liye relational DB,
NoSQL nahi.

---

ARCHETYPE C · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · [saga/ms-comm](../../FOUNDATIONS/10_ms_communication.md) · [caching](../../FOUNDATIONS/04_caching.md) · saath: [payment-system](../06_payment_system/06_payment_system.md) (iska bada bhai) · [stock-broker](../05_stock_broker_trading/05_stock_broker_trading.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
