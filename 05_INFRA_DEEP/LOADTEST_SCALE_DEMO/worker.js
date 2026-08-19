// WORKER (STEP 3a) — wahi server, bas PORT command-line se aata (3001, 3002...).
// Isliye ek file se DO worker chala paaye. LB inke peeche baithta.
const http = require('http');
const PORT = process.argv[2] || 3001;   // port command se: `node worker.js 3001`

function doWork(ms) {
  const end = Date.now() + ms;
  while (Date.now() < end) {}
}

http.createServer((req, res) => {
  if (req.url === '/api/users') {
    doWork(8);
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify([{ id: 1, name: 'Arpan', servedBy: PORT }]));  // servedBy = kisne diya
  } else {
    res.writeHead(404);
    res.end('not found');
  }
}).listen(PORT, () => console.log('WORKER live on port ' + PORT));
