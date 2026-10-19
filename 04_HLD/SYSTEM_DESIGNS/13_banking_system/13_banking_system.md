# Mini Banking System (accounts · transactions · balances)

> Ek bank ke andar accounts, un pe paisa daalo / nikaalo / bhejo, aur balance dikhao.
> Dil: khaata (ledger) sach hai, balance sirf uska jod. Aur paisa na bane, na gayab ho.
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c11_2-defadb32.svg)

---

## Mock cue (mock me sirf ye dekho: kya hua -> ilaaj -> kaunsa dabba juda)

```
    KYA HUA                                     ILAAJ                                 DABBA JUDA
1   8 baje Ravi ko 500, beech me server gira -> EK TRANSACTION (WAL)                  SQL DB
2   8:30 network gaya, app ne dobara bheja   -> IDEMPOTENCY KEY (UNIQUE)              Banking Svc (+ key)
3   9:00 Netflix auto-debit + mera UPI       -> CHECK + KAATNA EK QUERY               (SQL DB: balance >= x)
4   mera 500 kahan gaya? (support / audit)  -> KHAATA (ledger) usi transaction me    (SQL DB: ledger table)
5   10 baje salary, Ravi ko rent, SMS giri    -> OUTBOX, Kafka key = account_id        Kafka
6   salary ke baad maine balance khola       -> BALANCE = CACHE, raat RECONCILIATION  Reconciliation job
7   11 baje sabki salary, ek machine bhari   -> KAI MACHINE + LB, DB sync copy        LB + Banking Svc x N
8   bheed me sab balance dekh rahe           -> READ REPLICA + read-your-own-writes   Read replica
9   dukaan ko 500, usi sec 200 refund        -> CHHOTI ID PEHLE LOCK                  (SQL DB: lock kram)
10  CA ka export, page 5000 dheema           -> KEYSET (ts, id) + index               (SQL DB: index)
11  raat: 5 saal khaata ek table me          -> MAHINE KE TUKDE + S3, snapshot        Archive (S3)

POORA DESIGN (upar ke dabbe kram se):
   User -> LB -> Banking Svc (idempotency key) -> SQL DB: EK transaction (khaata + balance + outbox)
        -> Kafka (commit ke baad, key = account_id) · Read replica = balance / history
   Reconciliation (raat) · Archive S3 (mahine ke tukde, had se pehle delete nahi)
```

---

## Shuru

Sabse simple: ek Banking Service (saara niyam / logic) aur uske peeche ek SQL DB. Jaan-boojh ke seedha.

Pehle scope baandho: "Bahut bada hai, use-cases baandh lete hain. Ek hi bank ke andar, ya doosre bank ko bhi?"
Scope pehle, warna interviewer jitna bada system chaahe sar pe daal dega. Maan ke chalo: user ke ek ya kai account,
deposit, withdraw, transfer A se B (dono isi bank), balance, history. "Dono isi bank" ye ek shabd poora design
badalta. Bahar: doosra bank, loan, byaaj, card, KYC.

Kaisa chale: sahi hona sabse upar (paisa na gum ho, na bane, hisaab har waqt barabar). Har paisa trace ho (audit).
Dobara bhejne pe dobara na kate (idempotent). Balance dekhna tez, kyunki wahi sabse zyada padha jaata.

Numbers: 50 lakh account, 1 crore transaction roz. Din me ~86,400 second, to ~120 likhai har second. Padhna likhne se
bahut zyada, log balance baar-baar dekhte. 120 / second ek theek-thaak Postgres machine ka dasva hissa hai, isliye
sharding ki zaroorat nahi, primary + replica kaafi. Khaata: 1 crore x 2 line x 365 = ~730 crore line saal me,
3 saal me ~22 arab, 5 saal me ~36 arab.

Data se teen sawaal nikalte: transfer me do row badalti, dono ho ya koi nahi, kaun sambhalega? Ek account pe do
transfer ek saath, kaun rokega? Balance minus me na jaaye, ye niyam kahan likha? Aage ke card inhi ke jawab hain.

---

*ab tootega kahan? sabse pehle transfer ke beech me, kyunki ek transfer me do row badalti hain*

## 1. Maine Ravi ko 500 bheje, beech me server gira

Pehle mera -500 aur Ravi ka +500 do alag kaam the. Mera 500 kat gaya, server gira, Ravi ka +500 hua hi nahi.
500 gayab.

Isliye dono kaam ek transaction me: BEGIN, Arpan -500, Ravi +500, COMMIT. Ya dono hote hain, ya koi nahi (transaction).

Ye kyun chalta? Ek DB me ek transaction kabhi aadha nahi rehta. Crash hua to DB apne log se khud ulta kar deta.
Aur padhne wale ko aadha kaam kabhi nahi dikhta.

SAGA kyun nahi? Sab ek hi DB me hai, to ek transaction kaafi. SAGA tab jab paisa do alag DB me ho.

**Pooche: beech me server gira to DB kaise jaanta?** DB ke log (WAL) se. COMMIT likh gaya tha to poora karo,
nahi likha to ulta karo. Mera account wapas 1000.

**Pooche: -500 ke baad, COMMIT se pehle, koi padhe to?** Use COMMIT se pehle wala hi dikhta. Aadha kisi ko nahi dikhta.

**Pooche: Ravi doosre bank me ho to?** Do bank = do DB, ek transaction nahi ho sakta. PENDING rakho + SAGA, aur raat
ko milaan.

---

*ab ek transfer to pakka, par wahi transfer do baar aa gaya to?*

## 2. Send dabaya, network gaya, app ne dobara bheja

Pehle app har baar naya transfer bhejta tha, koi id nahi. Timeout hua, app ne dobara bheja, server ne do alag
transfer samjhe. 1000 kat gaye.

Isliye screen khulte hi app ek id banata hai (T-77). Send pe wahi id DB me jaati, UNIQUE ke saath. Pehli baar
paisa kata. Dobara wahi T-77 aayi to server pehla jawab wapas deta, paisa doosri baar nahi katta (idempotency key).

Ye kyun chalta? Aadha nahi, wo transaction ka kaam. Do baar nahi, wo id ka kaam. Id app banata hai, server nahi,
kyunki dobara bhejne wala wahi id le ke aata. Isliye retry hamesha safe: wahi id = wahi jawab.

Send button band kar dein, kaafi nahi? Nahi. Retry app khud karta hai (timeout pe), user ke dabaye bina. Isliye id
phir bhi chahiye.

**Pooche: dono request ek saath pahunchi to?** DB khud rokta. Doosra insert fail, UNIQUE hi taala hai.

**Pooche: wahi id, par amount 5000 aaya?** Request ka hash mila nahi, to 422. Purana jawab nahi dete.

**Pooche: id kab tak rakhein?** 24-48 ghante, phir saaf. Retry itni der me hi aata.

---

*ab do alag transfer ek hi account pe ek saath aaye to?*

## 3. 9 baje Netflix ka auto-debit aur mera UPI ek saath

Mere paas 300. Netflix 200 aur UPI 200 ek saath aaye. Pehle dono ne balance padha (300, pass), phir dono ne kaata.
Balance -100.

Isliye check aur kaatna ek hi query me: `UPDATE account SET balance = balance - 200 WHERE id = Arpan AND balance >= 200`
(atomic UPDATE).

Ye kyun chalta? Row pe taala lagta, dono line me lagte. Pehla 300 se 100, ho gaya. Doosre ke liye 100 >= 200? Nahi,
to 0 row badli. Kuch nahi kata, user ko saaf 422 "paisa kam". Balance kabhi minus nahi.

Pehle padho, phir kaato? Wahi hua: dono ne 300 dekha, dono pass. Check aur kaatna alag hue to beech me doosra ghus jaata.

Ye niyam Java me likh do? Nahi, DB me. Raaste kai hain (API, batch, haath se), DB ek hai. Niyam DB me ho to har
raasta usse guzarta.

---

*ab paisa sahi kata, par support poochhe paisa gaya kahan?*

## 4. Maine poochha: mere 500 kahan gaye, kab?

Pehle sirf balance column tha. 1000 likha hai, par 500 kahan gaye, kab gaye, koi hisaab nahi bacha. Support bola
"pata nahi".

Isliye har transfer ki 2 line khaate me likhte hain, usi transaction me: Arpan -500, Ravi +500 (ledger). Khaata
sirf jodte hain, kabhi badalte nahi.

Ye kyun chalta? Khaata usi transaction me jahan paisa hila. Transaction wapas hua (paisa kam tha) to khaata bhi
wapas, jhoothi line bachti hi nahi. Aur har line me waqt + transfer id, to support dekhta: 8:00 baje, T-77, Ravi
ko 500 gaye.

Sirf balance column? Jod dikhta, raasta nahi. Purana hisaab mit jaata.

Khaata Kafka me likh dein? Kafka aur DB alag hain, ek transaction nahi ban sakta. Rollback hua to Kafka me jhoothi
line reh jaati.

**Pooche: galat line likhi gayi to?** UPDATE nahi. Ulti line jodo, taaki galti bhi khaate me dikhe.

**Pooche: support ko kab, kisko, kyun chahiye?** Har line me waqt + transfer id hai. Sab ek jagah.

---

*ab paisa pakka gaya, par user ko khabar (SMS) bhi to jaani hai*

## 5. Salary aayi, Ravi ko rent bheja, SMS service giri

Pehle transfer karte hi SMS bhejne jaate the. Ravi ko 20,000 gaye, SMS service band thi. Transfer ho gaya, SMS
kabhi nahi gaya.

Isliye DB me ek aur table, outbox. SMS ki khabar bhi usi transaction me likhte hain (sent=false). Ek chhota
program (relay) har second outbox dekhta, sent=false wali Kafka me daal deta, COMMIT ke ~1 sec baad. SMS service
Kafka se padh ke SMS bhejti, phir sent=true (outbox).

Ye kyun chalta? Transfer SMS pe nahi rukta. Khabar outbox me usi transaction me, aur Kafka tak COMMIT ke baad hi
jaati. To khabar tabhi nikalti jab paisa pakka gaya. DB gira to bhi khabar outbox me, kuch nahi khota.

Seedha SMS service ko bulao? Wahi hua: service band thi, khabar gayi.

Pehle Kafka me daal do, DB me baad me likho? Khabar nikal gayi "Ravi ko 20,000 aaye", phir DB fail. Paisa gaya hi
nahi, par Ravi ko SMS aa gaya. Isliye paisa aur khabar dono DB me ek saath: ya dono, ya koi nahi.

**Pooche: SMS service ghanta bhar giri rahi?** Khabar Kafka me padi rehti. Wapas aate hi wahin se padh leti.

**Pooche: relay ne Kafka me daala, sent=true se pehle gir gaya?** Khabar dobara jaati. SMS service eventId dekh ke
skip karti.

**Pooche: outbox bharti hi jaaye?** Roz saaf karo. sent=true ka kaam khatam.

**Pooche: relay likhna hi na pade?** DB ke log se seedha Kafka (CDC, Debezium).

**Pooche: SMS ulte kram me pahunche?** Kafka key = account_id. Ek account ki khabar ek partition me, kram se.

---

*ab khaata bhi hai aur balance column bhi, dono me farak aaya to?*

## 6. Salary ke baad maine balance khola

Pehle balance column aur khaata alag ho gaye (bug). Khaate ka jod 29,800, column me 30,300. Kisi ne pakda hi nahi.

Isliye khaata hi sach, balance column uska jod (cache), usi transaction me update. Raat ko ek job dono milata hai:
khaate ka jod (snapshot + baad ki line) column ke barabar? Haan to theek (reconciliation).

Ye kyun chalta? Balance dekhna sabse zyada hota, har baar 50,000 line jodna dheema, isliye column. Par sach ek hi
jagah, khaate me. Farak mila to alert + account roko. Pehle wajah dhoondo, phir khaata jeete.

Har baar khaate se jod lo, column hi mat rakho? Har balance dekhne pe 50,000 line jodna dheema.

Column hi sach maan lo? Wahi hua: column galat ho gaya aur kisi ko pata nahi chala, kyunki milane ko kuch tha hi nahi.

**Pooche: raat ko farak mila, 29,800 vs 30,300?** Alert + account roko. Pehle KYUN, phir khaata jeete.

---

*ab sahi hai, par 11 baje sabki salary ek saath aayi*

## 7. 11 baje sabki salary aa gayi, ek Banking service bhar gayi

Pehle sirf ek machine. Laakhon transfer, CPU bhar gaya, phir giri. Koi paisa nahi bhej pa raha.

Isliye kai machine, aage LB jo baante. Machine kuch yaad nahi rakhti (stateless). DB ki copy sync, doosri building me.

Ye kyun chalta? Sach DB me hai, isliye koi bhi machine koi bhi transfer le sakti. Jitni chaho lagao. Ek machine
giri to baaki chalti rehti.

Ek badi machine? Wahi hua: ek hi thi, bhari aur giri.

**Pooche: ek machine giri?** LB health check se use list se hata deta.

**Pooche: transfer adhoora reh gaya?** App WAHI id se dobara bhejta. Do baar nahi katta (card 2).

**Pooche: LB khud akela?** Nahi. LB bhi do zone me, ek gire to doosra.

**Pooche: main DB hi gir gaya?** Sync copy (doosri building) promote, ~1-2 min (Multi-AZ). Us beech error aaye to
app dobara, WAHI id se. Do baar nahi katta.

**Pooche: sync copy matlab dheema?** Haan, ek network chakkar. Paise me chalega. Async me aakhri transfer kho sakta.

**Pooche: 11 baje ek dukaan ke account me 50,000 payment ek saath aaye, us ek row pe lock ki line?** Credit
ledger line me likho (sirf INSERT, lock nahi), balance column har second batch me jodo. Atomic check sirf debit pe
chahiye, credit pe nahi.

---

*ab machine kaafi, par bheed padhne wali hai*

## 8. Salary ki bheed me sab balance dekh rahe, transfer dheeme

Pehle padhna aur likhna ek hi DB pe. Laakhon log balance / history padh rahe, usi DB pe transfer bhi. Primary pe
dono ka bojh, transfer dheema.

Isliye padhna copy se, likhna sirf primary pe (read replica). Primary ka log (WAL) copy tak jaata.

Ye kyun chalta? Bheed padhne ki hai, wo copy le leti. Primary ke paas likhna + paise ka faisla bacha, transfer tez.
Paise ka faisla sirf primary pe, kyunki copy thodi peeche ho sakti.

Sab primary se hi padho? Wahi hua: padhne ki bheed ne transfer dheema kar diya.

**Pooche: abhi bheja, aur copy 5 sec peeche hai?** Apna abhi likha primary se padho (last_write_at Redis me).
Warna purana balance dikha to user dobara bhejega.

**Pooche: copy minute bhar peeche chali gayi?** Kitni peeche, ye naapo (lag). Alert, aur us copy ko list se hatao.

**Pooche: failover wali copy aur read replica ek hi cheez?** Nahi. Failover standby sync hai, kuch nahi khota. Read
replica async hai, peeche rehti. Alag cheez.

---

*ab ek alag dikkat: do transfer ek doosre ke ulte*

## 9. Ravi ki dukaan ko 500, usi second dukaan se 200 refund (deadlock)

Ye card 3 wali dikkat nahi hai. Wahan paisa do baar kat raha tha (double spend). Yahan dono transfer ek doosre ka
intezaar karte atak gaye. Txn1 (Arpan se dukaan) ne Arpan pakda, dukaan ka intezaar. Txn2 (dukaan se Arpan refund)
ne dukaan pakdi, Arpan ka intezaar. Gol ghera (deadlock).

Isliye taala hamesha ek kram me, chhoti id pehle (lock order). Txn1 pehle 7 (Arpan), phir 9 (dukaan). Txn2 bhi
wahi kram, to 7 pe intezaar.

Ye kyun chalta? Dono ek hi kram me taala lete, to ghera banta hi nahi. Txn1 poora, phir Txn2.

Jo pehle aaye wo pehle pakde? Wahi hua: dono ne ulte kram me pakda, dono hamesha ke liye atke.

**Pooche: kram ke baad bhi phans gaya?** DB ghera khud pakadta, ek ko wapas karta. App ruk ke WAHI id se dobara.

---

*ab sab chal raha, par purana hisaab maanga gaya*

## 10. Shaam ko dukaan ke CA ka export, page 5000 pe atka

Pehle page 5000 = LIMIT 20 OFFSET 100000. DB 1 lakh line padhta, phir phenk deta. Export bahut dheema.

Isliye pichhle page ki aakhri line yaad rakhte hain, agla page usse aage (keyset). Query:
`WHERE account_id = ? AND (ts, id) < (aakhri ts, id) ORDER BY ts DESC, id DESC LIMIT 20`, index ke saath.

Ye kyun chalta? "Kahan tak dekha" yaad hai, "kitne chhode" nahi. Index se seedha wahan, sirf 20 padhe. Page 1 ho
ya 5000, ek jaisa.

OFFSET kyun nahi? Har page pe pichhli saari line padh ke phenkni padti. Aur beech me nayi line aayi to page khisak
jaata, line dobara aati.

**Pooche: ek hi ts pe do line?** Isliye (ts, id) dono. id barabari todti, line na chhootti na dohrati.

**Pooche: beech me nayi line aayi?** Purana page nahi khiskta. Ginti nahi, "kahan tak" yaad hai.

**Pooche: (ts, id) app ko kaise dete?** Band lifafe me (next_cursor, base64). App andar nahi dekhti.

**Pooche: seedha page 500 pe jaana?** Keyset me nahi hota. "Kahan tak" yaad hai, "kitne chhode" nahi. Agla /
pichhla kaafi.

---

*ab saal guzre, khaata bas badhta hi ja raha*

## 11. Raat ko: 5 saal ka khaata ek hi table me

Pehle 5 saal ki arabon line ek hi table me. Roz ki query aur backup har baar saari line chhoote. Dheema + mehnga.

Isliye mahine ke tukde (partition). Is mahine ki line apne tukde me, paas aur tez. Mahina band hua to alag karke S3
pe (archive), sasta.

Ye kyun chalta? Table me sirf haal ke mahine, chhoti rehti. Naya paas, purana sasta. Balance ka jod poore 5 saal se
nahi, mahine ke snapshot + baad ki line se.

Purana delete hi kar do? Nahi. 5 saal (PMLA) se pehle delete nahi, bank ka niyam.

**Pooche: line S3 chali gayi, balance ka jod kaise?** Poora nahi jodte. Mahine ka snapshot + baad ki line.

**Pooche: RBI ko 2021 ka statement chahiye?** S3 pe hi query (Athena, Parquet), ya wo tukda wapas laao.

**Pooche: koi purani file badal de to?** S3 pe taala: na delete, na badal (Object Lock, WORM).

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
