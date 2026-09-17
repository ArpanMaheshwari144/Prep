# STAR — HikariCP Pool Exhaustion (emails failing)  [Konovo]

> Q ye cover karta: "challenging bug batao" · "prod incident" · "ownership" · "cross-team debugging".

## STAR (Hinglish notes — yaad ke liye)
```
   S (Situation): Konovo pe client ne raise kiya — emails/notifications ja hi nahi rahe. ★ EK BHI email nahi ja raha tha (100% blocked, partial nahi). critical PROD issue, routine nahi.
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
   R (Result)   : query rukte hi -> pool recover -> RDS utilization normal -> email delivery restore.
                  ★ NAAP (ye 4 bolne hain):
                     1. ZERO email ja raha tha — poora blocker, partial failure nahi
                     2. report se fix tak ~1 - 1.5 GHANTA (logs normal dikh rahe the, isliye
                        andar tak jaana pada — wahi time laga)
                     3. DATA GAYA NAHI — MySQL insert hua hi nahi tha, to kuch aadha-adhoora
                        ya gum hua nahi (AWS/SNS ka mamla tha hi nahi, MySQL end ka tha)
                     4. wajah hamare CODE ki nahi thi — PROD pe haath se chalayi gayi query thi
```

## SPOKEN (English — interview me bolna, ~60-90 sec)
```
   "At Konovo, a client reported that their emails had stopped going out completely — not a single email was going
    through. It was a critical production issue, so I took it on.

    I first checked the application logs and the database, but everything looked normal. Digging deeper, I noticed HikariCP
    connection-pool exhaustion — repeated pool-timeout errors. I then checked AWS RDS metrics and saw database utilization
    was abnormally high, around 85%+.

    Tracing it, I found a heavy write query updating a large number of records — not part of normal traffic, likely run
    manually in production. It held a lock for a long time and blocked our flow: our system inserts rows into MySQL first
    and then sends emails via AWS SNS. With the inserts blocked, no emails went out, and the held connections exhausted the
    pool, slowing the whole tool.

    I coordinated with the DevOps team to stop that runaway query. Once it was killed, the pool recovered, RDS normalized,
    and email delivery was restored — about an hour to an hour and a half from the report, most of it spent digging past
    logs that looked normal. And because the MySQL inserts had never gone through, nothing was half-sent or lost — the
    blocker was entirely on the database side, not on AWS."
```

## KYA DEMONSTRATE karta (interviewer ye dekhta)
```
   - SYSTEMATIC debugging (app-logs -> pool -> RDS -> query -> lock) — jaldi conclusion nahi, layer-by-layer.
   - FULL-STACK samajh (MySQL insert -> SNS email -> connection pool ka connection).
   - OWNERSHIP (critical issue apne haath me liya) + CROSS-TEAM (DevOps ke saath).
   - ★ connection-pooling / lock ka concept (tera HLD Q16 se bhi connect).
```

## FOLLOW-UP — "dobara na ho, kya kiya?" (interviewer YE poochega)
```
   HONEST jawab (jo ACTUAL kiya — oversell nahi):
   - ye query hamare CODE ki NAHI thi -- kisi ne PROD me manually/ad-hoc chalayi thi.
     Isliye koi permanent "code-fix" tha hi nahi -- asli guardrail = jaldi PAKADNA.
   - MONITORING + ALERTING lagayi: aisa DB-load / lock / pool-timeout dobara aaye to
     ALERT apne-aap chali jaati -> DevOps turant pakad ke handle kar lete.
   - LOGGING behtar ki -> next baar root-cause jaldi trace ho.
   - aur ye ownership dikhaya ki jo bhi PROD pe aisi bhaari query chalaye, wo dhyaan rakhe.

   LINE (interview): "Since the query wasn't part of our codebase, there wasn't a code-level
     fix -- so I focused on fast detection: I added monitoring and alerts on DB load and
     connection-pool timeouts, and improved logging, so if it ever recurs it's caught
     immediately and the team can act before it impacts users."
   -> ye band-aid se ENGINEER wala jawab hai: har issue ka "perfect fix" nahi hota, kabhi
      asli engineering = detection + fast-response. (ye honest hai, interviewer ko pasand aata.)
```

## DELIVERY tips
```
   - "MAINE / I" bolo action me (team nahi, TU). result me NUMBERS (ZERO email, ~1-1.5 hr, 85% RDS).
   - calm + structured -> S->T->A->R kram me. bolne ki PRACTICE (loud, 2-3 baar).
   - ★ RESULT pe "sab theek ho gaya" MAT bolo — wo bhaari nahi lagta. Chaar naap bolo:
     zero email · ~1-1.5 ghanta · data gaya nahi · wajah code ki nahi thi.
   - FOLLOW-UP wala honest jawab ready rakh (upar) -- band-aid nahi, detection-focus.
```
