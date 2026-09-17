# STAR — HikariCP Pool Exhaustion (emails failing)  [Konovo]

> Ye story cover karti: "challenging bug" · "prod incident" · "ownership" · "cross-team debugging".
> **Is file me sirf wahi hai jo MUH SE BOLNA hai.** Notes/analysis hata diye — do jagah ek hi baat nahi rakhni.

---

## SPOKEN — main answer (~60-90 sec)

```
"At Konovo, a client reported that their emails had effectively stopped going out — it wasn't a slowdown,
 delivery had stopped. I can't give you an exact failure rate; we didn't have that kind of instrumentation
 at the time. It was a critical production issue, so I took it on.

 I first checked the application logs and the database, but everything looked normal. Digging deeper, I
 noticed HikariCP connection-pool exhaustion — repeated pool-timeout errors. I then checked AWS RDS metrics
 and saw database utilization was abnormally high, around 85%.

 Tracing it, I found a heavy write query updating a large number of records — not part of normal traffic,
 likely run manually in production. It held a lock for a long time and blocked our flow: our system inserts
 rows into MySQL first and then sends emails via AWS SNS. With the inserts blocked, no emails went out, and
 the held connections exhausted the pool, slowing the whole tool.

 I coordinated with the DevOps team to stop that runaway query. Once it was killed, the pool recovered, RDS
 normalized, and email delivery was restored — about an hour to an hour and a half from the report, most of
 it spent digging past logs that looked normal. And because the MySQL inserts had never gone through,
 nothing was half-sent or lost — the blocker was entirely on the database side, not on AWS."
```

---

## SPOKEN — follow-up: *"Dobara na ho, uske liye kya kiya?"*

```
"Since the query wasn't part of our codebase, there wasn't a code-level fix — so I focused on fast
 detection: I added monitoring and alerts on DB load and connection-pool timeouts, and improved logging,
 so if it ever recurs it's caught immediately and the team can act before it impacts users."
```

> Ye band-aid nahi hai — har issue ka "perfect code-fix" hota hi nahi. Kabhi asli engineering =
> **detection + fast response.** Ye honest hai, aur wahi bolna hai.

---

## ★ EK NIYAM — jo NAAPA nahi, uska NUMBER mat bolo

```
is story me sirf DO cheez sach me naapi gayi:
     RDS utilization  ~85%        (RDS metrics me DEKHI thi)
     ~1 - 1.5 ghanta              (apna time)

email-fail %  ->  KABHI naapa hi nahi gaya. Us waqt measuring tool tha hi nahi.
                  "effectively all" bolo. 95 / 99 / 100 — koi bhi MAT bolo.

banaya hua number follow-up pe TOOTTA hai, aur wahi asli nuksaan hai.
"I don't have an exact figure — we didn't have that instrumentation then"
= kamzori nahi, ye ZYADA mazboot jawab hai.
```
