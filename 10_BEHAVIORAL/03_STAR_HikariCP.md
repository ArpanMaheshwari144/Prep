# STAR — HikariCP Pool Exhaustion (emails stopped)  [Konovo]

> Q ye cover karta: "challenging bug" · "prod incident" · "ownership" · "cross-team debugging".

## STAR (Hinglish)

```
   S: client ne bola emails jaana BAND ho gaye (slow nahi, band). critical prod issue.
      fail % kabhi NAAPA nahi gaya -> "effectively all" bolo, koi number nahi.

   T: issue maine uthaya -> pata karna kyun band hai aur chalu karna.

   A: logs + DB dekhe -> sab normal dikha.
      aur khoda -> HikariCP POOL khatam (baar-baar pool-timeout).
      AWS RDS metrics -> DB load ~85%.
      trace kiya -> dev team ki ek BHAARI write query peak time pe chal rahi thi, email table pe LOCK.
      hamara flow: pehle MySQL me row insert -> phir SNS se email. insert ruka -> email ruke -> pool bhi khatam.
      mere paas prod access nahi tha -> DevOps se coordinate karke query KILL karwayi.
      query BATCH me commit ho rahi thi -> jo batch pehle commit ho chuke the, wo rows dhundh ke haath se ROLLBACK.

   R: pool theek, RDS normal, email chalu -> report se ~1-1.5 ghante me.
      dobara na ho: code me fix ki jagah nahi thi (query hamari nahi thi) ->
      DB load + pool-timeout pe ALERT + behtar logging -> agli baar turant pakda jaaye.
```

## SPOKEN (English)

```
   "At Konovo, a client reported that their emails had effectively stopped going out. It wasn't a slowdown,
    delivery had stopped. I can't give an exact failure rate; we didn't have that instrumentation then.
    It was a critical production issue, so I took it on.

    The application logs and the database looked normal at first. Digging deeper, I found HikariCP
    connection-pool exhaustion, with repeated pool-timeout errors, and AWS RDS utilization around 85%.

    I traced it to a heavy write query run by the dev team during peak hours. It held a lock on the email
    table, and our flow inserts rows into MySQL before sending through AWS SNS, so with inserts blocked no
    emails went out, and the held connections exhausted the pool.

    I didn't have production access, so I coordinated with DevOps to kill the query. It had been committing
    in batches, so I identified the batches that had already committed and rolled those rows back to keep
    the data consistent. The pool recovered, RDS normalized and email delivery was restored, about an hour
    to an hour and a half from the report.

    Since the query wasn't part of our codebase, there was no code fix, so I focused on detection: alerts on
    DB load and pool timeouts, and better logging, so it's caught immediately if it happens again."
```
