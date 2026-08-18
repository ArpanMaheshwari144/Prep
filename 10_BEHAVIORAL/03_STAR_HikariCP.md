# STAR — HikariCP Pool Exhaustion (emails failing)  [Konovo]

> Q ye cover karta: "challenging bug batao" · "prod incident" · "ownership" · "cross-team debugging".

## STAR (Hinglish notes — yaad ke liye)
```
   S (Situation): Konovo pe client ne raise kiya — emails/notifications ja hi nahi rahe (~95% fail). critical PROD issue, routine nahi.
   T (Task)     : root-cause dhundhna + email delivery jaldi restore karna (mujhe own karna tha).
   A (Action)   :
        1. pehle APP LOGS + DB dekha -> sab normal dikha, obvious error nahi.
        2. deeper debug -> HikariCP CONNECTION POOL EXHAUSTION -> baar-baar pool-timeout error logs me.
        3. AWS RDS metrics dekhe -> DB utilization abnormally HIGH (~85%+).
        4. trace kiya -> ek bhari WRITE query bahut saare records update kar rahi thi -> normal traffic nahi,
           lagta kisi ne PROD me manually chalayi.
        5. us query ne LONG time + LOCK hold kiya -> hamara flow BLOCK: system pehle MySQL me rows INSERT karta,
           phir AWS SNS se email bhejta -> inserts ruk gaye -> emails ruk gaye -> held connections ne POOL exhaust kiya -> tool slow.
        6. DevOps team ke saath coordinate -> woh runaway query KILL/stop karvayi.
   R (Result)   : query rukte hi -> pool recover -> RDS utilization normal -> email delivery restore (~95% fail -> normal). critical incident RESOLVED.
```

## SPOKEN (English — interview me bolna, ~60-90 sec)
```
   "At Konovo, a client reported that nearly all their emails had stopped going out — around 95% were failing. It was a
    critical production issue, so I took it on.

    I first checked the application logs and the database, but everything looked normal. Digging deeper, I noticed HikariCP
    connection-pool exhaustion — repeated pool-timeout errors. I then checked AWS RDS metrics and saw database utilization
    was abnormally high, around 85%+.

    Tracing it, I found a heavy write query updating a large number of records — not part of normal traffic, likely run
    manually in production. It held a lock for a long time and blocked our flow: our system inserts rows into MySQL first
    and then sends emails via AWS SNS. With the inserts blocked, no emails went out, and the held connections exhausted the
    pool, slowing the whole tool.

    I coordinated with the DevOps team to stop that runaway query. Once it was killed, the pool recovered, RDS normalized,
    and email delivery was restored."
```

## KYA DEMONSTRATE karta (interviewer ye dekhta)
```
   - SYSTEMATIC debugging (app-logs -> pool -> RDS -> query -> lock) — jaldi conclusion nahi, layer-by-layer.
   - FULL-STACK samajh (MySQL insert -> SNS email -> connection pool ka connection).
   - OWNERSHIP (critical issue apne haath me liya) + CROSS-TEAM (DevOps ke saath).
   - ★ connection-pooling / lock ka concept (tera HLD Q16 se bhi connect).
```

## DELIVERY tips
```
   - "MAINE / I" bolo action me (team nahi, TU). result me NUMBERS (95%, 85%).
   - calm + structured -> S->T->A->R kram me. bolne ki PRACTICE (loud, 2-3 baar).
   - confirm/add: exact email-fail % (95%?) + koi aur metric (kitne der me resolve?).
```
