// SINGLE server (STEP 1) — ek endpoint jo har request pe ~8ms kaam karta.
// Single-thread -> bahut request aayengi to SATURATE hoga (RPS atkega, latency chadhegi).
const http = require('http');

function doWork(ms) {                 // 8ms ka nakli kaam (busy loop)
  const end = Date.now() + ms;
  while (Date.now() < end) {}
}

const server = http.createServer((req, res) => {
  if (req.url === '/api/users') {
    doWork(8);
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify([{ id: 1, name: 'Arpan' }]));
  } else {
    res.writeHead(404);
    res.end('not found');
  }
});

server.listen(3000, () => console.log('API on http://localhost:3000'));
