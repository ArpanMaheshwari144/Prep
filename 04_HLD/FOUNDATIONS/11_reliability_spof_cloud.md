# Reliability / SPOF / Cloud reality (doubt-clear note)

> **NAV** — KYA: SPOF / reliability basics. UP: [MASTER](../00_MASTER_SHEET.md) · lagta hai: [file-upload](../SYSTEM_DESIGNS/08_file_upload_validate_system/08_file_upload_INTERVIEW.md) · [rate-limiter](../SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md)

> Arpan ke apne sawaal se bana (3-Jul) — "AWS bhi to physical machine, girta hoga na?" wali samajh.

---

## Cloud (AWS) ki REALITY

```
   AWS servers = REAL physical machines hi hain (datacenter me hardware, koi alien/magic nahi).
   individual server FAIL bhi hota hai (hardware roz kahin na kahin girta).
   -> phir bhi tujhe pata nahi chalta. KYUN? -> REDUNDANCY.

   teri machine = EK -> crash -> sab khatam (SPOF).
   AWS = LAKHS servers, AVAILABILITY ZONES (alag datacenters) + REGIONS (alag geography) me faile ->
         ek gira -> doosra apne aap le leta (failover) -> tu UP rehta.
   poora ek datacenter gir jaaye (aag/power/flood) -> doosri AZ pick kar leti.

   -> cloud "reliable" isliye nahi ki girta NAHI -> balki itni redundancy ki girna DIKHTA nahi.
```

---

## SPOF (Single Point of Failure)

```
   SPOF = ek aisa point jiske girne se POORA system gir jaaye.
   e.g. sab kuch EK server pe -> woh gira -> sab gaya = SPOF.
   FIX = spread karo (kai machines) -> koi ek point nahi jiske girne se sab jaaye -> SPOF khatam.
```

---

## SHARDING vs REPLICATION — failure me kaun kya karta

```
   SHARDING    = data ko tukdon me spread (kai servers).
                 PRIMARY kaam = SCALE (write/storage baant do).
                 failure me MADAD: FAULT ISOLATION -> ek shard gira -> sirf US shard ke users affected,
                                   BAAKI system chalu (blast-radius chhota). + spread se SPOF bhi khatam.
                 PAR: sharding AKELE gire hue shard ka DATA nahi bachata (uski copy na ho to unavailable).
                      ulta zyada servers = zyada failure-points.

   REPLICATION = data ki COPIES rakho.
                 kaam = actual failure RECOVERY -> primary gira -> REPLICA le leti (data safe + failover).

   ★ REAL SYSTEM: SHARDING + har shard REPLICATED -> scale BHI, fault-tolerance BHI.
   crisp: SHARDING = failure CONTAIN (isolation, blast-radius) | REPLICATION = failure RECOVER (data safe).
```

---

```
   YAAD (ek line):
   AWS girta hai par REPLICATION + FAILOVER + no-SPOF itna solid ki pata nahi chalta.
   sharding = spread/scale + isolation (SPOF khatam) | replication = copies/recovery (data safe).
```

---

## ★★ DEPTH-PASS (25-Sep) — SPOF kaise DHOONDHO + redundancy KHUD kaise fail hoti hai

> Upar = SPOF kya hai, fix = spread. Ye section = chhupe SPOF kahan milte hain, aur wo tarike
> jinse "2 copy rakh di" wala ilaaj bhi kaam nahi karta. (LB ka SPOF = 03_load_balancing.md,
> DB failover = 05_database_replication.md — yahan repeat nahi.)

### A. SPOF dhoondhne ka tareeka — request ka raasta khincho, har DABBE pe ek sawaal

```
User -> DNS -> CDN -> LB -> App (x3) -> Redis -> DB primary -> payment gateway

har dabbe pe:  "YE gira to kya hoga?"
   App x3        -> ek gira, baaki 2    = SPOF nahi
   Redis x1      -> sab app ek pe       = SPOF (3 app ho ke bhi!)
   DB primary x1 -> writes band         = SPOF (jab tak failover na ho)
```

**Chhupe SPOF — jo diagram ke DABBE nahi hote:**
```
DNS provider (ek hi)        -> naam hi resolve nahi, sab khatam
EK region / EK AZ           -> sab server ek hi building me
TLS CERTIFICATE             -> expire hua = har server zinda, par koi connect nahi kar sakta
config / secret store       -> start hi nahi hota koi service
NAT gateway (ek AZ me)      -> bahar ki call (payment, SMS) sab band
third-party (payment / SMS) -> tera code theek, bahar wala gira
EK INSAAN                   -> sirf ek banda jaanta kaise deploy / restore hota hai
```

### B. ★★ CHHE TARIKE JINSE REDUNDANCY KHUD FAIL HOTI HAI

```
1. DONO COPY EK SAATH GIRI  (correlated failure)
   2 server, par same rack / same AZ / same power -> ek jhatke me dono
   ilaaj: copies alag FAILURE DOMAIN me (alag AZ, alag rack)

2. STANDBY kabhi TEST nahi hua
   failover ki ghadi aayi -> standby ka config purana / backup restore hi nahi hua
   "backup jo kabhi restore nahi kiya = backup nahi hai"
   ilaaj: failover DRILL / game day — jaan-boojh ke primary maaro, dekho

3. SAB COPY pe EK SAATH galat config / code
   redundancy HARDWARE se bachati hai, GALTI se nahi — same bug har copy me
   asli: Facebook Oct 2021 (ek network config change ne sab band kiya, andar ke tools bhi)
         CrowdStrike July 2024 (ek update sab Windows machines pe ek saath)
   ilaaj: dheere rollout — canary (1%), phir ek AZ, phir baaki; turant ROLLBACK ka raasta

4. CHHUPA SHARED dependency
   3 app server, par teeno ek hi Redis / ek hi auth service / ek hi certificate pe
   -> diagram me redundant dikhta, asal me ek dhaaga sabko baandhe
   ilaaj: upar wala A — har dependency pe "ye gira to?"

5. FAILOVER KHUD outage ban gaya
   jhootha "mara" signal (GC pause, blip) -> failover -> phir wapas -> flapping
   ya dono active (split brain)
   ilaaj: timeout + threshold + majority quorum + fencing (CAP depth-pass C dekho)

6. BACHE HUE bojh jhel hi nahi paaye
   3 me se 1 gira, bache 2 pe 150% -> wo bhi gire (cascading)
   ilaaj: N+1 capacity (LB depth-pass #3 dekho)
```

### C. Har SPOF hatana zaroori NAHI — keemat vs nuksaan

```
multi-AZ           -> sasta, lagbhag hamesha karo
multi-region       -> MEHNGA (data sync, latency, double infra, complexity)
admin tool / report -> ek instance chalega, ghanta bhar down = koi nahi marta

senior soch: "ye gira to kitna nuksaan (blast radius) aur kitni der?" vs "hatane ki keemat"
money-path (payment, ledger) -> multi-AZ zaroori, region soch ke
```

### D. Ek shakal (wahi LB / cache / CDN / CAP wali)

```
redundancy bachane ko lagayi -> par same jagah rakhi (correlated), kabhi test nahi ki,
galti sab copy pe ek saath gayi, ya failover khud flap hua
=> "2 copy hai" kaafi nahi. Poochho: ALAG jagah? TESTED? galti se bhi bachegi? bache hue jhel lenge?
```

> **BOLNE WALI LINE:** *"SPOF dhoondhne ko main request ka raasta khinchta hoon aur har dabbe pe
> poochhta hoon 'ye gira to?' — aur un cheezon pe bhi jo dabba nahi hain: DNS, certificate, ek AZ,
> third-party. Redundancy ke saath chaar cheez check karta hoon: copies alag failure domain me hain,
> failover test hua hai, rollout dheere hai taaki galti har copy pe ek saath na jaaye, aur bache hue
> server poora bojh jhel lenge. Har SPOF hatana zaroori nahi — money-path pe multi-AZ, admin tool pe
> ek instance bhi theek."*
