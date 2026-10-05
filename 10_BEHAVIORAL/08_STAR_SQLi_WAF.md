# STAR — SQL injection attack pakda, kami AWS WAF me thi (code me nahi)  [Konovo]

> Q ye cover karta: "security issue kaise handle kiya" · "logs se kuch pakda" · "root cause" ·
> "escalation" · "jo cheez tumhara area nahi thi usme kya kiya".

## STAR (Hinglish)

```
   S: logs dekh raha tha -> ek hi IP se bahut saari requests.
      aage dekha -> request me ADMIN / SQL jaisa payload. normal traffic nahi.

   T: samajhna -> ye SQL injection ki koshish hai; kya ye kaam karegi? aur ise rokna.

   A: code dekha -> query PARAMETERIZED thi, input string seedha SQL me nahi judti
         -> hamla DB pe waise bhi fail hota. code me fix ki zaroorat NAHI thi.
      asli kami: AWS WAF me SQL injection rules nahi the -> bura traffic backend tak pahunch raha tha.
      devops ko escalate kiya (AWS ka WRITE access sirf unke paas, hamare paas nahi)
         -> unhone WAF me us IP ka BLOCK + SQL injection rules lagaye.
      maine test server pe WAHI requests dobara chalayi -> ab WAF pe hi block ho rahi.

   R: aisa traffic ab backend tak pahunchta hi nahi.
      code safe tha, edge pe deewar nahi thi -> wo lagi.
      LEARNING: app safe ho tab bhi edge pe filter chahiye — do deewar (WAF + parameterized query).
```

## SPOKEN (English)

```
   "At Konovo, while going through production logs, I noticed a large number of requests coming
    from a single IP. Looking closer, the payloads had SQL-like strings, things like ADMIN, so this
    was clearly not normal traffic but a SQL injection attempt.

    First I checked whether it could actually work. Our queries were parameterized, so the input
    never became part of the SQL itself — the attack would fail at the database. So there was no
    code fix needed. The real gap was at the edge: our AWS WAF had no SQL injection rules, so this
    traffic was reaching the backend at all.

    AWS write access sat with DevOps, not with our team, so I escalated it to them with the log
    evidence, and they blocked the IP and added SQL injection rules in AWS WAF. I then
    replayed the same requests on our test server to confirm they were now blocked at the WAF.

    After that, this kind of traffic stopped at the edge. My learning was that even when the
    application is safe, you want a second wall in front of it — the WAF plus parameterized queries."
```

## POOCHE TO

```
   "Code fix kyun nahi kiya?"      -> query parameterized thi; payload data ki tarah jaata, SQL nahi banta.
   "Phir problem kya thi?"         -> WAF rules missing -> bura traffic app tak aa raha (load + risk).
   "Tumhara role kya tha?"         -> pakda, check kiya ki app safe hai, escalate kiya, fix verify kiya.
                                      WAF rules devops ne lagaye — ye saaf bolo, apna nahi batao.
   "Verify kaise kiya?"            -> test server pe wahi requests replay -> WAF pe block.
   "Devops ko kyun?"               -> AWS ka write access sirf devops ke paas tha, hamari team ke paas nahi.
```
