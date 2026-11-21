# Monitoring — app theek chal rahi? numbers + graph se jawab

> Demo files: [MONITORING_DEMO](../../05_INFRA_DEEP/MONITORING_DEMO) · [LOADTEST_SCALE_DEMO](../../05_INFRA_DEEP/LOADTEST_SCALE_DEMO)

---

## Koi poochhe to aise bolo

Monitoring matlab app ki halat ko numbers aur graph me lagaatar dekhna: kitni request aa rahi, kitni der lag rahi, kitni
fail ho rahi, CPU / memory kitni. Spring me Actuator ye numbers ek URL pe khol deta. Prometheus har kuch second me jaake
ye numbers kheench ke rakhta (pull). Grafana un numbers ka graph banata aur alert lagata ("p95 latency 500 ms se upar to
batao"). Office me yahi kaam New Relic / Grafana se hota, aur logs Papertrail me.

---

## Kahani: gaadi ka dashboard

```
App = gaadi, Actuator = engine ke sensor (speed, temp, petrol)
Prometheus = har 2 sec meter padh ke diary me likhne wala (pull, khud jaata)
Grafana = dashboard pe suiyan aur graph; laal batti = alert
Logs (Papertrail) = gaadi ki black box: kya hua, kab hua, line-line
```

---

## Chala ke dekha (usercrud me)

- Actuator + Micrometer lagaya: `localhost:8080/actuator/prometheus` pe numbers ka text (JVM memory, request count, DB
  connection pool).
- Prometheus + Grafana Docker me chalaye; Prometheus ke targets page pe usercrud **UP** (hara).
- Loop se 2000 request maari: Grafana ki line upar chadhi, load ruka to neeche. 200 aur 400 ki alag-alag line bhi dikhi.

---

## Dikkat se hal

Request ki ginti (counter) hamesha badhti hi rehti, to seedha graph bas upar jaati line. Hal: `rate()` lagao, wo batata
"abhi har second kitni request" (raftaar), yahi kaam ka graph.

---

## Trap

**p95 nahi aata:** p95 latency (100 me se 95 request isse jaldi) ka graph chahiye to Spring me alag se ek setting on
karni padti; default me wo data nikalta hi nahi, graph khaali.

**Grafana pe bharosa:** Grafana khud kuch store nahi karta, Prometheus se poochh ke dikhata. Prometheus gira to graph khaali.

---

## HLD se jod

Har design me "kuch gira to pata kaise chale" ka jawab yahi: inventory me email band hua to alert CTO tak, News me "sync
job fail ho to alert".
