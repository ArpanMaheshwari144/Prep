# Mini Banking System (accounts · transactions · balances)

> Ek bank ke andar accounts, un pe paisa daalo / nikaalo / bhejo, aur balance dikhao.
> Dil: khaata (ledger) sach hai, balance sirf uska jod. Aur paisa na bane, na gayab ho.
>
> Mock me sirf heading + Cue + poster. Poster me sab: upar "Pehle aisa tha", tasveer (ilaaj), hara "Kyun chalta", laal number = kya toot sakta + kaise bacha, peela = doosra tareeka kyun nahi, grey = baaki.
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c11_2-defadb32.svg)

---


## Shuru

```
FR      user ke ek ya kai account. Deposit, withdraw, transfer A se B (dono isi bank), balance, history.
        Bahar: doosra bank, loan, byaaj, card, KYC
NFR     sahi hona sabse upar (paisa na gum ho, na bane, hisaab har waqt barabar). Har paisa trace ho (audit).
        Dobara bhejne pe dobara na kate (idempotent). Balance dekhna tez ho
NUMBERS 50 lakh account, 1 crore transaction roz = ~120 likhai / sec, padhna kahin zyada.
        120 / sec ek Postgres ka dasva hissa hai, to shard nahi, primary + replica kaafi
DABBA 0 sabse simple: ek Banking Service (saara niyam / logic) + peeche ek SQL DB, jaan-boojh ke seedha
SCOPE   pehle poochho "ek hi bank ke andar, ya doosre bank ko bhi?", warna interviewer jitna bada chaahe
        sar pe daal dega. "dono isi bank", ye ek shabd poora design badalta hai
SAWAAL  transfer me do row badalti hain, dono hon ya koi nahi, kaun sambhalega? -> card 1
```

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | mere 500 kahan gaye, kab? · balance khola, column aur khaata alag | 4, 6 |
> | 2 bheed aayi | 11 baje sabki salary · sab balance dekh rahe, transfer dheeme | 7, 8 |
> | 3 machine giri | 500 bhejte server gira (**is design ka khaas: paisa na bane, na gayab**) · SMS service giri · primary DB gira (card 7 ka cross-Q) | 1, 5, 7 |
> | 4 do log ek saath | network gaya, app ne dobara bheja · Netflix + UPI ek saath · dukaan ko 500 + refund (deadlock) | 2, 3, 9 |
> | 5 kuch dheema | CA ka export, page 5000 | 10 |
> | 6 data bada | 5 saal ka khaata, ~36 arab line | 11 |
> | 7 door ke log | nahi aata | - |
> | 8 bahar ka hamla | wahi id, par amount 5000 (card 2 ka cross-Q) | 2 |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Account / khaata (SQL) → **AWS RDS (Postgres / MySQL)**: ek transaction, Multi-AZ = sync copy doosri building
> - Purana khaata → **S3** (Object Lock): band mahina sasta, badal nahi sakte

---

*ab tootega kahan? sabse pehle transfer ke beech me, kyunki ek transfer me do row badalti hain*

## 1. Maine Ravi ko 500 bheje, beech me server gira

**Rail 3 MACHINE GIRI (khaas)** · Cue: EK TRANSACTION (WAL)

<img src="img/c1_poster.png?v=3" width="1000" alt="card 1: kya toota, kaise bacha">


---

*ab ek transfer to pakka, par wahi transfer do baar aa gaya to?*

## 2. Send dabaya, network gaya, app ne dobara bheja

**Rail 4 DO LOG EK SAATH (retry)** · Cue: IDEMPOTENCY KEY (UNIQUE)

<img src="img/c2_poster.png?v=3" width="1000" alt="card 2: kya toota, kaise bacha">


---

*ab do alag transfer ek hi account pe ek saath aaye to?*

## 3. 9 baje Netflix ka auto-debit aur mera UPI ek saath

**Rail 4 DO LOG EK SAATH (ek account)** · Cue: CHECK + KAATNA EK QUERY

<img src="img/c3_poster.png?v=3" width="1000" alt="card 3: kya toota, kaise bacha">


---

*ab paisa sahi kata, par support poochhe paisa gaya kahan?*

## 4. Maine poochha: mere 500 kahan gaye, kab?

**Rail 1 KAAM CHALA (hisaab)** · Cue: KHAATA (ledger) usi transaction me

<img src="img/c4_poster.png?v=3" width="1000" alt="card 4: kya toota, kaise bacha">


---

*ab paisa pakka gaya, par user ko khabar (SMS) bhi to jaani hai*

## 5. Salary aayi, Ravi ko rent bheja, SMS service giri

**Rail 3 MACHINE GIRI (SMS)** · Cue: OUTBOX, Kafka key = account_id

<img src="img/c5_poster.png?v=3" width="1000" alt="card 5: kya toota, kaise bacha">


---

*ab khaata bhi hai aur balance column bhi, dono me farak aaya to?*

## 6. Salary ke baad maine balance khola

**Rail 1 KAAM CHALA (balance)** · Cue: BALANCE = CACHE, raat RECONCILIATION

<img src="img/c6_poster.png?v=3" width="1000" alt="card 6: kya toota, kaise bacha">


---

*ab sahi hai, par 11 baje sabki salary ek saath aayi*

## 7. 11 baje sabki salary aa gayi, ek Banking service bhar gayi

**Rail 2 BHEED AAYI (salary)** · Cue: KAI MACHINE + LB, DB sync copy

<img src="img/c7_poster.png?v=3" width="1000" alt="card 7: kya toota, kaise bacha">


---

*ab machine kaafi, par bheed padhne wali hai*

## 8. Salary ki bheed me sab balance dekh rahe, transfer dheeme

**Rail 2 BHEED AAYI (padhna)** · Cue: READ REPLICA + read-your-own-writes

<img src="img/c8_poster.png?v=3" width="1000" alt="card 8: kya toota, kaise bacha">


---

*ab ek alag dikkat: do transfer ek doosre ke ulte*

## 9. Ravi ki dukaan ko 500, usi second dukaan se 200 refund (deadlock)

**Rail 4 DO LOG EK SAATH (deadlock)** · Cue: CHHOTI ID PEHLE LOCK

<img src="img/c9_poster.png?v=3" width="1000" alt="card 9: kya toota, kaise bacha">


---

*ab sab chal raha, par purana hisaab maanga gaya*

## 10. Shaam ko dukaan ke CA ka export, page 5000 pe atka

**Rail 5 KUCH DHEEMA (export)** · Cue: KEYSET (ts, id) + index

<img src="img/c10_poster.png?v=3" width="1000" alt="card 10: kya toota, kaise bacha">


---

*ab saal guzre, khaata bas badhta hi ja raha*

## 11. Raat ko: 5 saal ka khaata ek hi table me

**Rail 6 DATA BADA (khaata)** · Cue: MAHINE KE TUKDE + S3, snapshot

<img src="img/c11_poster.png?v=3" width="1000" alt="card 11: kya toota, kaise bacha">


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
primary DB pe hi jaati. Index bhi sirf padhai tez karta, likhai ulta dheemi. Likhai bahut ho to account id se baanto:
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
