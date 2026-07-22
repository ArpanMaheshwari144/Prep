# Reliability / SPOF / Cloud reality (doubt-clear note)

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
