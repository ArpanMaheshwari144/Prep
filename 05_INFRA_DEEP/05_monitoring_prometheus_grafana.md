# Monitoring — Prometheus + Grafana (app ki live metrics + graphs)

> "App theek chal rahi?" ka jawab NUMBERS + GRAPHS se. Actuator metrics deta -> Prometheus scrape+store karta
> -> Grafana graph banata. Netflix/Uber ka standard monitoring stack. (hands-on: usercrud pe live spike dekha)

---

## PURA FLOW (ek line me)
```
APP (Actuator + Micrometer)  --/actuator/prometheus-->  PROMETHEUS (har 2s scrape + store, time-series)
                                                              |
                                                         GRAFANA (query + graph)  <-- tu dekhta
Load maara -> request-rate badhi -> Grafana line UPAR -> load ruka -> neeche.
```

## HAR TUKDA KYA KARTA
```
Actuator      = Spring ke built-in endpoints (/actuator/health, /metrics, /prometheus). app ki andar ki halat.
Micrometer    = metrics ko Prometheus-FORMAT me export karta (/actuator/prometheus pe text).
Prometheus    = pull-based: khud app se har few-sec metrics KHEENCHTA (scrape) + time-series DB me store.
Grafana       = Prometheus ko query karke GRAPH/dashboard banata. (khud data store nahi karta)
```

---

## ═══ HANDS-ON — usercrud pe live monitoring (22-Aug) ═══

### 1. App me metrics expose (usercrud)
```xml
<!-- pom.xml -->
<dependency><groupId>org.springframework.boot</groupId><artifactId>spring-boot-starter-actuator</artifactId></dependency>
<dependency><groupId>io.micrometer</groupId><artifactId>micrometer-registry-prometheus</artifactId><scope>runtime</scope></dependency>
```
```properties
# application.properties
management.endpoints.web.exposure.include=health,info,prometheus,metrics
management.metrics.tags.application=usercrud
```
```java
// SecurityConfig -> Prometheus scrape kar sake
.requestMatchers("/actuator/**").permitAll()
```
-> run -> `http://localhost:8080/actuator/prometheus` pe metrics-text dikhi (jvm_memory, http_server_requests_seconds_count, hikaricp_connections...).

### 2. Prometheus config (kis app ko scrape karna)
```yaml
# prometheus.yml
global:
  scrape_interval: 2s                       # har 2 sec (demo -> fast graph)
scrape_configs:
  - job_name: 'usercrud'
    metrics_path: '/actuator/prometheus'
    static_configs:
      - targets: ['host.docker.internal:8080']   # app HOST pe, docker ke bahar -> host.docker.internal
```

### 3. Prometheus + Grafana chalao (docker)
```
docker network create mon
docker run -d --name prom --network mon -p 9090:9090 -v "<path>\prometheus.yml:/etc/prometheus/prometheus.yml" prom/prometheus
docker run -d --name graf --network mon -p 3000:3000 grafana/grafana
```
- `localhost:9090/targets` -> usercrud **UP** (green) = scrape ho raha.

### 4. Grafana setup
```
localhost:3000 -> login admin/admin
Connections -> Data sources -> Add -> Prometheus -> URL: http://prom:9090 -> Save & test (Success)
Dashboards -> New -> Add visualization -> Configure visualization
  query (Code mode):  rate(http_server_requests_seconds_count[1m])
  time range: Last 5-15 min, auto-refresh: 5s
```

### 5. LIVE spike dekha
```
terminal:  for /L %i in (1,1,2000) do @curl -s -o nul http://localhost:8080/product/1

Grafana:   line UPAR chad gayi (request-rate spike). load ruka -> neeche.
           legend me SUCCESS aur CLIENT_ERROR ki alag lines (200 vs 400) -> Grafana ne breakdown diya.
```

---

## PromQL basics (jo query likhi)
```
http_server_requests_seconds_count   = total requests (COUNTER, sirf badhta)
rate(...[1m])                        = per-second rate last-1-min ki (COUNTER -> rate se "kitni req/sec" milti)
   -> counter ko seedha graph mat karo (bas upar jaata); rate() se "speed" nikaalo.
Aur: histogram_quantile(0.95, ...) = p95 LATENCY. sum by (status)(...) = status-wise.
```

## ★★ GEMS / interview
```
1. Stack: APP(actuator+micrometer) -> PROMETHEUS(pull/scrape+store) -> GRAFANA(graph). Alertmanager = alerts.
2. Prometheus = PULL model (khud scrape karta), push nahi. scrape_interval config.
3. Metric types: COUNTER (badhta -> rate() lo), GAUGE (up-down, seedha), HISTOGRAM (latency percentiles p95/p99).
4. host.docker.internal = docker-container se HOST-machine reach (app docker ke bahar ho to).
5. Grafana khud data store nahi karta -> sirf Prometheus (ya aur source) se query+visualize.
6. Real use: dashboards (RPS/latency/errors/CPU) + ALERTS ("p95 latency > 500ms -> notify"). = observability.
```

## INTERVIEW LINE
```
"Monitoring: Spring Actuator + Micrometer app metrics ko /actuator/prometheus pe expose karte.
 Prometheus pull-model se har few-sec scrape karke time-series me store karta. Grafana usse query karke
 live dashboards banata (rate() se RPS, histogram_quantile se p95 latency). Alerts Alertmanager se.
 Maine usercrud pe laga ke load-test kiya -> request-rate spike Grafana graph pe real-time dikhi."
```

### Dobara kaise
```
app run (actuator on) -> docker start prom graf -> localhost:3000 -> panel query rate(http_server_requests_seconds_count[1m])
-> load maaro -> line upar.   (targets check: localhost:9090/targets)
```

[← HLD README](../README.md)
