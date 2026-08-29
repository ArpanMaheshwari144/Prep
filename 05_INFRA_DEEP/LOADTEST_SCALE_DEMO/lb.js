// LOAD BALANCER (STEP 3b) — gate pe traffic-police (port 3000). Client sirf ISSE baat karta.
// Peeche 2 worker (3001, 3002). Har request ko BAARI-BAARI (round-robin) bhejta.
const http = require('http');

const workers = [3001, 3002];   // peeche ke servers
let i = 0;                      // round-robin counter

http.createServer((clientReq, clientRes) => {
  const target = workers[i % workers.length];   // baari-baari worker: i%2 -> 0,1,0,1...
  i++;

  const proxy = http.request(
    { host: '127.0.0.1', port: target, path: clientReq.url, method: clientReq.method, headers: clientReq.headers },
    (workerRes) => {
      clientRes.writeHead(workerRes.statusCode, workerRes.headers);
      workerRes.pipe(clientRes);   // worker ka jawab client ko wapas
    }
  );
  proxy.on('error', () => { clientRes.writeHead(502); clientRes.end('worker down'); });
  clientReq.pipe(proxy);
}).listen(3000, () => console.log('LOAD BALANCER on 3000 -> baant raha workers: ' + workers.join(', ')));
