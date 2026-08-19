# SCALE + LOAD BALANCER — Hands-on Live Demo (Locust)

> Ye maine KHUD kar ke, live graph dekh ke samjha — kagaz-theory nahi, chal ke dekha hua.
> Neeche: kya / kaise / kyun step-by-step + poora code + explain.

---

## 0. MAQSAD
"Scale" ko aankhon se dekhna: apni API pe bahut saari request maar ke dekhna —
kaha tak sambhalti hai, kaha TOOT-ti hai, aur FIX karke kaise theek hoti.

## 1. TOOLS (kya use kiya)
- **Node.js** — chhota API server banane ko.
- **Locust** — LOAD-TEST tool. Nakli users banata jo baar-baar API ko request maarte.
  Browser dashboard (localhost:8089) LIVE graph deta: RPS, latency, users.
- **Load Balancer (LB)** — baad me banaya (fix ke liye). Traffic baari-baari servers me baantta.

---

## 2. STEP 1 — EK server (single) banaya   [server.js]
Ek endpoint `/api/users` jo har request pe ~8ms "kaam" karta
(simulate: asli API DB-query/compute karti).

```js
const http = require('http');

function doWork(ms) {                 // 8ms ka nakli kaam (busy loop)
  const end = Date.now() + ms;
  while (Date.now() < end) {}
}

const server = http.createServer((req, res) => {
  if (req.url === '/api/users') {
    doWork(8);                        // har request 8ms khaati
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify([{ id: 1, name: 'Arpan' }]));
  } else { res.writeHead(404); res.end('not found'); }
});

server.listen(3000, () => console.log('API on http://localhost:3000'));
```
Chalaya: `node server.js`  -> API port 3000 pe zinda.

## 3. STEP 2 — Locust se LOAD maara   [locustfile.py]
```py
from locust import HttpUser, task, between

class ApiUser(HttpUser):
    wait_time = between(0.1, 0.5)     # har nakli user 2 request ke beech 0.1-0.5s rukta
    @task
    def get_users(self):
        self.client.get("/api/users")
```
Chalaya: `python -m locust -f locustfile.py --host http://localhost:3000`
Phir browser: **localhost:8089** -> Users=100, Ramp=10 -> START -> **Charts** tab.

### 3 CHART (kya dekha)
- **RPS**        = ek sec me kitni request nipti (throughput). ZYADA = achha.
- **Latency (ms)** = ek request ka time (kitni slow). KAM = achha. (orange=normal, purple=slow-wale)
- **Users**      = kitne nakli banda ek saath.

## 4. RESULT-1 (single server) — DEEWAR (saturation)
- RPS ~120 pe **ATAK gaya** (isse zyada nahi kar pa raha).
- Latency 9ms se **500ms** tak chad gayi.
- Failures 0 (par 500ms = slow = kharaab experience).

**Kyun:** server single-thread, har request 8ms. To 1 sec me max = 1000/8 = **~125 request**. Bas -> CEILING.
100 banda ek saath maar rahe, server sirf 125/sec -> baaki **LINE me** (queue) -> wait -> latency 500ms.

---

## 5. STEP 3 — FIX: 2 server + LB   (code KAISE ADD kiya)

### (a) server ko "worker" banaya — PORT bahar se   [worker.js]
Wahi server, bas PORT ab command se aata -> ek file se DO worker chala paaye (3001, 3002).
```js
const http = require('http');
const PORT = process.argv[2] || 3001;   // <- YE line ADD: port command-line se

function doWork(ms){ const end=Date.now()+ms; while(Date.now()<end){} }

http.createServer((req,res)=>{
  if(req.url==='/api/users'){
    doWork(8);
    res.writeHead(200,{'Content-Type':'application/json'});
    res.end(JSON.stringify([{id:1,name:'Arpan',servedBy:PORT}]));  // servedBy = kisne diya
  } else { res.writeHead(404); res.end('not found'); }
}).listen(PORT, ()=>console.log('WORKER on '+PORT));
```
Chalaya: `node worker.js 3001`  aur  `node worker.js 3002`  -> 2 server.

### (b) LOAD BALANCER banaya   [lb.js] — asli FIX
LB khud ek server hai (port 3000). Client sirf ISSE baat karta. Peeche 2 worker.
```js
const http = require('http');
const workers = [3001, 3002];    // peeche ke 2 server
let i = 0;                        // baari ka counter

http.createServer((clientReq, clientRes) => {
  const target = workers[i % workers.length];   // <- ROUND-ROBIN: baari-baari worker
  i++;                                           //    i%2 -> 0,1,0,1 -> 3001,3002,3001...

  const proxy = http.request(                    // us worker ko request AAGE bhejo
    { host:'127.0.0.1', port:target, path:clientReq.url, method:clientReq.method, headers:clientReq.headers },
    (workerRes) => {
      clientRes.writeHead(workerRes.statusCode, workerRes.headers);
      workerRes.pipe(clientRes);                 // worker ka jawab -> client ko WAPAS
    }
  );
  clientReq.pipe(proxy);
}).listen(3000, () => console.log('LB on 3000 -> workers 3001,3002'));
```
Chalaya: `node lb.js`  -> LB port 3000 pe (jaha pehle single server tha).
Test: `curl localhost:3000/api/users` baar-baar -> `servedBy` 3001,3002,3001,3002
(baari-baari aaya = LB kaam kar raha).

Ab Locust wahi 3000 pe maar raha, PAR peeche 2 server + LB.

## 6. RESULT-2 (2 server + LB) — DEEWAR khisak gayi
Same 100 users:

```
                 1 server      2 server + LB
   RPS           ~120 (atka)   ~250   (DOUBLE)
   Latency       ~500 ms       ~100 ms
   Failures      0             0
```
RPS double, latency 500 -> 100. **Horizontal scaling ne kaam kiya.**

---

## 7. SAMAJH (dukaan analogy — yaad rakhne wali baat)
- 1 counter(server) ki speed FIXED. Bheed badhe -> line lambi -> RPS atka + latency chadhi = SATURATION.
- FIX = HORIZONTAL SCALE: aur server + LB (baari-baari baante) -> RPS multiply, latency divide.
- Ek server ka RPS ceiling  =~  1000ms / per-request-time.   (yahan 1000/8 =~ 125)
- RPS badhna = achha (zyada log handle). Latency girna = achha (sabko jaldi jawab).

## 8. LB CODE ka NICHOD (RATNA NAHI)
   workers-list  +  i % n (baari-baari)  +  request forward -> jawab wapas.
   Asli duniya me haath se nahi likhte -> **Nginx / HAProxy / AWS-ELB** ye 3-line config se karte.
   Yaad rakhne layak = CONCEPT, code nahi.

## 9. INTERVIEW LINE
"Single server saturate hota hai — RPS ceiling aata, latency badhti.
 Fix = load balancer ke peeche multiple replicas (horizontal scale);
 LB round-robin se traffic baanta -> throughput multiply, latency down.
 Maine ise Locust se load-test karke live verify kiya."

## 10. DEMO DOBARA KAISE CHALAYE (isi folder me)
```
Terminal-1:  node worker.js 3001
Terminal-2:  node worker.js 3002
Terminal-3:  node lb.js
Terminal-4:  python -m locust -f locustfile.py --host http://localhost:3000
Browser:     localhost:8089  -> Users 100, Ramp 10 -> START -> Charts
```
(sirf single-server dekhna ho: `node server.js` + locust, LB/worker mat chalao)
Locust install (ek baar): `pip install locust`
