# Mini Banking System (accounts · transactions · balances)

> Ek bank ke andar accounts, un pe paisa daalo / nikaalo / bhejo, aur balance dikhao.
> Dil: khaata (ledger) sach hai, balance sirf uska jod. Aur paisa na bane, na gayab ho.
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card: HAL · KYUN · NAHI, phir YAAD RAKH dabba + CROSS-Q dabba (sawaal → cue, poora jawab).
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
> | 3 machine giri | 500 bhejte server gira (**is design ka khaas: paisa na bane, na gayab**) · SMS service giri · main DB gira (card 7 ka cross-Q) | 1, 5, 7 |
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

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c1_1.png?v=1" width="900">

<img src="img/c1_2.png?v=1" width="900">

<img src="img/c1_3.png?v=1" width="900">

</details>

```
TOOTA   mera -500 hua, server gira, Ravi ka +500 hua hi nahi, to 500 gayab
HAL     BEGIN, Arpan -500, Ravi +500, COMMIT. Dono hon ya koi nahi (transaction)
KYUN    ek DB me ek transaction kabhi aadha nahi rehta. Crash pe DB apne log se khud ulta karta hai
        padhne wale ko aadha kaam kabhi nahi dikhta
NAHI    SAGA
        -> FALTU: sab ek hi DB me hai, ek transaction kaafi. SAGA tab jab paisa do alag DB me ho
```

> [!IMPORTANT]
> **Yaad rakh:** BEGIN, -500, +500, COMMIT: dono ya koi nahi. Beech me gira to DB log (WAL) se ulta. SAGA sirf do DB pe.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - beech me server gira, DB kaise jaanta hai kya karna? → **log (WAL) se**: COMMIT likha hai to poora, nahi to ulta, mera wapas 1000
> - -500 ke baad, COMMIT se pehle koi padhe to? → **COMMIT se pehle wala hi dikhta hai**: aadha kisi ko nahi
> - Ravi doosre bank me ho to ek transaction chalega? → **do bank = do DB**: ek transaction nahi. PENDING + SAGA + raat ko milaan


---

*ab ek transfer to pakka, par wahi transfer do baar aa gaya to?*

## 2. Send dabaya, network gaya, app ne dobara bheja

**Rail 4 DO LOG EK SAATH (retry)** · Cue: IDEMPOTENCY KEY (UNIQUE)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c2_1.png?v=1" width="900">

<img src="img/c2_2.png?v=1" width="900">

<img src="img/c2_3.png?v=1" width="900">

</details>

```
TOOTA   koi id nahi thi, timeout pe app ne dobara bheja, server ne do transfer samjhe, to 1000 kate
HAL     screen khulte hi app id banata hai (T-77), send pe wahi id DB me UNIQUE ke saath (idempotency key)
        dobara T-77 aayi to pehla jawab wapas, paisa doosri baar nahi katta
KYUN    aadha nahi, ye transaction ka kaam hai. Do baar nahi, ye id ka kaam hai
        id app banata hai, server nahi, kyunki dobara bhejne wala wahi id laata hai.
        Wahi id to wahi jawab, retry safe
NAHI    send button band kar do
        -> ADHOORA: retry app khud karta hai (timeout pe), user ke dabaye bina
```

> [!IMPORTANT]
> **Yaad rakh:** App screen pe id banata (T-77), DB me UNIQUE. Wahi id dobara = pehla jawab. Alag amount = 422.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - wahi id wali do request ek saath aayi → **DB khud rokta hai**: doosra insert fail. UNIQUE hi taala hai
> - wahi id, par amount 5000 → **request ka hash nahi mila**: to 422, purana jawab nahi
> - id kab tak rakhoge? → **24-48 ghante**: phir saaf. Retry itni der me hi aata hai


---

*ab do alag transfer ek hi account pe ek saath aaye to?*

## 3. 9 baje Netflix ka auto-debit aur mera UPI ek saath

**Rail 4 DO LOG EK SAATH (ek account)** · Cue: CHECK + KAATNA EK QUERY

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c3_1.png?v=1" width="900">

<img src="img/c3_2.png?v=1" width="900">

<img src="img/c3_3.png?v=1" width="900">

</details>

```
SAWAAL  ek account pe do transfer ek saath, kaun rokega? Balance minus na jaaye, ye niyam kahan? Yahi card
TOOTA   300 the, Netflix 200 + UPI 200, dono ne 300 padha (pass), dono ne kaata, to -100
HAL     check aur kaatna ek hi query me (atomic UPDATE):
        UPDATE account SET balance = balance - 200 WHERE id = Arpan AND balance >= 200
KYUN    row pe taala lagta hai, dono line me. Pehla 300 se 100. Doosra: 100 >= 200? nahi, to 0 row,
        kuch nahi kata, 422 "paisa kam"
        balance kabhi minus nahi jaata
NAHI    pehle padho, phir kaato
        -> RACE: dono ne 300 dekha, beech me doosra ghus jaata hai
        niyam Java me
        -> CHHOOT: raaste kai hain (API, batch, haath se), DB ek. Niyam DB me to har raasta usse guzarta hai
```

> [!IMPORTANT]
> **Yaad rakh:** Check + kaatna ek query: UPDATE ... WHERE balance >= 200. 0 row = "paisa kam". Niyam DB me, Java me nahi.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - UPDATE me WHERE likhna bhool gaye? → **DB ka CHECK (balance >= 0)**: table pe CHECK (balance >= 0) aakhri deewar hai. Code bhoola to bhi DB minus wali likhai mana kar deta
> - SELECT ... FOR UPDATE kyun nahi? → **ek UPDATE kaafi**: FOR UPDATE me do kadam (padho, phir likho), DB tak do chakkar aur taala lamba. Ek UPDATE me check + kaatna dono, taala sabse chhota
> - transfer me -500 aur +500 do row, do UPDATE, kram kya? → **chhoti account id pehle**: dono row hamesha chhoti id pehle pakdo, warna ulte transfer ke saath gol ghera (deadlock, card 9)


---

*ab paisa sahi kata, par support poochhe paisa gaya kahan?*

## 4. Maine poochha: mere 500 kahan gaye, kab?

**Rail 1 KAAM CHALA (hisaab)** · Cue: KHAATA (ledger) usi transaction me

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c4_1.png?v=1" width="900">

<img src="img/c4_2.png?v=1" width="900">

<img src="img/c4_3.png?v=1" width="900">

</details>

```
TOOTA   sirf balance column tha, 1000 dikhta, 500 kahan / kab gaye koi hisaab nahi, support bole "pata nahi"
HAL     har transfer ki 2 line khaate me, usi transaction me: Arpan -500, Ravi +500 (ledger).
        Sirf jodo, kabhi badlo nahi
KYUN    transaction wapas hua (paisa kam) to khaata bhi wapas, jhoothi line bachti nahi
        har line me waqt + transfer id, to support dekhta hai: 8:00, T-77, Ravi ko 500
NAHI    sirf balance column
        -> ANDHA: jod dikhta hai, raasta nahi, purana hisaab mit jaata hai
        khaata Kafka me
        -> JHOOTH: Kafka + DB ek transaction nahi, rollback pe Kafka me jhoothi line reh jaati
```

> [!IMPORTANT]
> **Yaad rakh:** Har transfer ki 2 line khaate me, usi transaction me. Sirf jodo, galti pe ulti line. Waqt + transfer id.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - khaate me galat line likh di → **ulti line jodo**: UPDATE nahi, ulti line jodo, taaki galti bhi khaate me dikhe
> - support ko kab, kisko, kyun chahiye? → **har line me waqt + transfer id**: sab ek jagah


---

*ab paisa pakka gaya, par user ko khabar (SMS) bhi to jaani hai*

## 5. Salary aayi, Ravi ko rent bheja, SMS service giri

**Rail 3 MACHINE GIRI (SMS)** · Cue: OUTBOX, Kafka key = account_id

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c5_1.png?v=1" width="900">

<img src="img/c5_2.png?v=1" width="900">

<img src="img/c5_3.png?v=1" width="900">

</details>

```
TOOTA   transfer karte hi SMS bhejte the, Ravi ko 20,000 gaye, SMS service band, to SMS kabhi nahi gaya
HAL     DB me outbox table, khabar usi transaction me likhi jaati hai (sent=false)
        relay har second outbox dekhta hai, sent=false wali Kafka me (COMMIT ke ~1 sec baad).
        SMS service padhti hai, bhejti hai, sent=true
KYUN    transfer SMS pe nahi rukta. Kafka tak khabar COMMIT ke baad hi, to khabar tabhi jab paisa pakka
        DB gira to bhi khabar outbox me hai, kuch nahi khota
NAHI    seedha SMS service bulao
        -> KHOYA: service band thi, khabar gayi
        pehle Kafka, DB baad me
        -> JHOOTH: "Ravi ko 20,000 aaye" nikal gaya, DB fail, paisa gaya hi nahi.
           Paisa + khabar ek saath DB me
```

> [!IMPORTANT]
> **Yaad rakh:** Khabar outbox me usi transaction me, relay COMMIT ke baad Kafka. Dobara = eventId skip. Key = account_id.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - SMS service ghanta bhar giri → **khabar Kafka me padi hai**: wapas aate hi wahin se padhegi
> - relay ne Kafka me daala, sent=true se pehle gira → **khabar dobara jaayegi**: SMS service eventId dekh ke skip karti hai
> - outbox bharti hi jaaye to? → **roz saaf karo**: sent=true ka kaam khatam
> - relay likhna hi na pade, aisa ho sakta hai? → **CDC (Debezium)**: DB ke log se seedha Kafka (CDC, Debezium)
> - SMS ulte kram me pahunche to? → **Kafka key = account_id**: ek account ek partition, to kram se


---

*ab khaata bhi hai aur balance column bhi, dono me farak aaya to?*

## 6. Salary ke baad maine balance khola

**Rail 1 KAAM CHALA (balance)** · Cue: BALANCE = CACHE, raat RECONCILIATION

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c6_1.png?v=1" width="900">

<img src="img/c6_2.png?v=1" width="900">

<img src="img/c6_3.png?v=1" width="900">

</details>

```
TOOTA   column aur khaata alag ho gaye (bug): khaata 29,800, column 30,300, kisi ne pakda nahi
HAL     khaata sach hai, balance column uska jod (cache), usi transaction me update hota hai
        raat ko job milata hai: khaate ka jod (snapshot + baad ki line) == column? (reconciliation)
KYUN    balance sabse zyada dekha jaata hai, har baar 50,000 line jodna dheema, isliye column.
        Par sach ek hi jagah hai, khaate me
NAHI    column hi mat rakho, har baar khaate se jodo
        -> DHEEMA: har baar 50,000 line
        column hi sach
        -> ANDHA: galat hua to pata nahi chala, milane ko kuch tha hi nahi
```

> [!IMPORTANT]
> **Yaad rakh:** Khaata sach, balance column uska cache. Raat ko milaan; farak = alert + account roko, khaata jeetta.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - raat ki reconciliation me farak mila, 29,800 vs 30,300 → **alert + account roko**: Pehle KYUN dhoondho, phir khaata jeetega


---

*ab sahi hai, par 11 baje sabki salary ek saath aayi*

## 7. 11 baje sabki salary aa gayi, ek Banking service bhar gayi

**Rail 2 BHEED AAYI (salary)** · Cue: KAI MACHINE + LB, DB sync copy

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c7_1.png?v=1" width="900">

<img src="img/c7_2.png?v=1" width="900">

<img src="img/c7_3.png?v=1" width="900">

</details>

```
TOOTA   ek machine, laakhon transfer, CPU bhara, machine giri, koi paisa nahi bhej pa raha
HAL     kai machine + aage LB, machine kuch yaad nahi rakhti (stateless). DB ki sync copy doosri building me
KYUN    sach DB me hai, to koi bhi machine koi bhi transfer le sakti hai. Jitni chaho lagao, ek giri to
        baaki chalti hain
NAHI    ek badi machine
        -> GIRA: ek hi thi, bhari aur giri
```

> [!IMPORTANT]
> **Yaad rakh:** Kai machine + LB, stateless, sach DB me. DB ki sync copy doosri building, gira to promote, WAHI id se retry.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - ek machine giri → **LB health check**: LB health check se use list se hata deta hai
> - machine giri aur transfer adhoora reh gaya → **app WAHI id se dobara bhejta hai**: do baar nahi katta (card 2)
> - LB akela hai? → **nahi, do zone**: nahi, LB bhi do zone me
> - main DB gira → **sync copy (doosri building) promote**: ~1-2 min (Multi-AZ). Beech me error aaya to WAHI id se dobara
> - sync copy matlab dheema? → **haan, ek chakkar**: haan, ek network chakkar lagta hai, paise me chalega. Async me aakhri transfer kho sakta hai
> - 11 baje ek dukaan pe 50,000 payment ek saath, to ek row pe lock ki line? → **credit sirf INSERT**: credit ledger line me likho (sirf INSERT, lock nahi)
> - phir dukaan ka balance kaise? → **column har second batch me jodo**: Atomic check sirf debit pe, credit pe nahi


---

*ab machine kaafi, par bheed padhne wali hai*

## 8. Salary ki bheed me sab balance dekh rahe, transfer dheeme

**Rail 2 BHEED AAYI (padhna)** · Cue: READ REPLICA + read-your-own-writes

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c8_1.png?v=1" width="900">

<img src="img/c8_2.png?v=1" width="900">

<img src="img/c8_3.png?v=1" width="900">

</details>

```
TOOTA   padhna + likhna ek DB pe, laakhon log balance / history padh rahe, primary pe dono bojh,
        transfer dheema
HAL     padhna copy se, likhna sirf primary pe (read replica). Primary ka log (WAL) copy tak jaata hai
KYUN    bheed padhne ki hai, copy use le leti hai. Primary pe likhna + paise ka faisla, to transfer tez
        paise ka faisla sirf primary pe, kyunki copy thodi peeche ho sakti hai
NAHI    sab primary se padho
        -> DHEEMA: padhne ki bheed ne transfer dheema kiya
```

> [!IMPORTANT]
> **Yaad rakh:** Padhna read replica se, likhna + paise ka faisla primary pe. Apna abhi likha primary se padho.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - abhi paisa bheja, aur copy 5 sec peeche hai → **apna likha primary se**: apna abhi likha primary se padho (last_write_at Redis me), warna purana balance dekh ke. user dobara bhejega
> - copy minute bhar peeche ho gayi → **lag naapo**: alert, us copy ko list se hatao
> - failover wali copy aur read replica ek hi hain? → **nahi, sync vs async**: nahi. Failover standby sync hai, kuch nahi khota. Read replica async hai, peeche chalti hai


---

*ab ek alag dikkat: do transfer ek doosre ke ulte*

## 9. Ravi ki dukaan ko 500, usi second dukaan se 200 refund (deadlock)

**Rail 4 DO LOG EK SAATH (deadlock)** · Cue: CHHOTI ID PEHLE LOCK

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c9_1.png?v=1" width="900">

<img src="img/c9_2.png?v=1" width="900">

<img src="img/c9_3.png?v=1" width="900">

</details>

```
FARAK   card 3 me paisa do baar kat raha tha (double spend). Yahan dono ek doosre ke intezaar me atke
TOOTA   Txn1 (Arpan se dukaan) ne Arpan pakda, dukaan ka intezaar. Txn2 (refund) ne dukaan pakdi,
        Arpan ka intezaar. Gol ghera ban gaya (deadlock)
HAL     taala hamesha ek kram me, chhoti id pehle (lock order): dono pehle 7 (Arpan), phir 9 (dukaan)
KYUN    ek hi kram hai to ghera banta hi nahi. Txn2 7 pe ruka, Txn1 poora hua, phir Txn2
NAHI    jo pehle aaye wo pehle pakde
        -> ATKA: ulte kram me pakda, dono hamesha atke
```

> [!IMPORTANT]
> **Yaad rakh:** Taala hamesha chhoti id pehle, ek kram = ghera nahi. Phir bhi phansa to DB ek ko wapas, WAHI id se retry.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - lock kram ke baad bhi phans gaye → **DB ghera khud pakadta hai**: ek ko wapas karta hai. App ruk ke WAHI id se dobara bhejta hai


---

*ab sab chal raha, par purana hisaab maanga gaya*

## 10. Shaam ko dukaan ke CA ka export, page 5000 pe atka

**Rail 5 KUCH DHEEMA (export)** · Cue: KEYSET (ts, id) + index

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c10_1.png?v=1" width="900">

<img src="img/c10_2.png?v=1" width="900">

<img src="img/c10_3.png?v=1" width="900">

</details>

```
TOOTA   page 5000 = LIMIT 20 OFFSET 100000, DB 1 lakh line padh ke phenkta hai, export bahut dheema
HAL     pichhle page ki aakhri line yaad rakho, agla page usse aage (keyset), index ke saath:
        WHERE account_id = ? AND (ts, id) < (aakhri ts, id) ORDER BY ts DESC, id DESC LIMIT 20
KYUN    "kahan tak dekha" yaad hai, "kitne chhode" nahi. Index se seedha wahan, sirf 20.
        Page 1 ho ya 5000, ek jaisa
NAHI    OFFSET
        -> DHEEMA: har page pe pichhli saari line padh ke phenko
        OFFSET aur beech me nayi line aa gayi
        -> KHISAKTA: page khisakta hai, line dobara aati hai
```

> [!IMPORTANT]
> **Yaad rakh:** OFFSET nahi, keyset: (ts, id) < aakhri, index se seedha. Har page ek jaisa tez. id barabari todti.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - ek hi ts pe do line hain → **(ts, id)**: isliye (ts, id), id barabari todti hai, line na chhoote na dohraye
> - keyset me beech me nayi line aa gayi → **purana page nahi khisakta**: "kahan tak" yaad hai
> - (ts, id) app ko kaise doge? → **next_cursor (base64)**: band lifafe me (next_cursor, base64), app andar nahi dekhti
> - seedha page 500 pe jaana ho → **keyset me nahi hota**: agla / pichhla kaafi hai


---

*ab saal guzre, khaata bas badhta hi ja raha*

## 11. Raat ko: 5 saal ka khaata ek hi table me

**Rail 6 DATA BADA (khaata)** · Cue: MAHINE KE TUKDE + S3, snapshot

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c11_1.png?v=1" width="900">

<img src="img/c11_2.png?v=1" width="900">

<img src="img/c11_3.png?v=1" width="900">

</details>

```
NUMBERS khaata 1 crore x 2 line x 365 = ~730 crore line saal me, 3 saal ~22 arab, 5 saal ~36 arab
TOOTA   5 saal ki arabon line ek table me, roz ki query + backup saari line chhoote, dheema + mehnga
HAL     mahine ke tukde (partition), is mahine ke paas aur tez. Mahina band hua to alag karke S3 pe
        (archive), sasta
KYUN    table me sirf haal ke mahine, to chhoti. Naya paas, purana sasta
        balance ka jod 5 saal se nahi, mahine ka snapshot + baad ki line
NAHI    purana delete kar do
        -> NIYAM: 5 saal (PMLA) se pehle nahi, ye bank ka niyam hai
```

> [!IMPORTANT]
> **Yaad rakh:** Mahine ke tukde, band mahina S3 (sasta, Object Lock). Balance = snapshot + baad ki line. 5 saal rakhna (PMLA).

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - line S3 pe chali gayi, to balance ka jod kaise? → **poora nahi jodte**: mahine ka snapshot + baad ki line
> - RBI ko 2021 ka statement chahiye → **Athena (Parquet)**: S3 pe hi query (Athena, Parquet), ya tukda wapas laao
> - koi S3 pe purani file badal de to? → **S3 pe taala**: na delete na badal (Object Lock, WORM)


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
