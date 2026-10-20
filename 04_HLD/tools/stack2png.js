// SVG -> PNG (stack ki poori tasveer). node stack2png.js <in.svg> <out.png>
const path = require('path'), fs = require('fs');
const NPX = path.join(process.env.LOCALAPPDATA, 'npm-cache', '_npx');
const dir = fs.readdirSync(NPX).map(d => path.join(NPX, d, 'node_modules', 'puppeteer')).find(p => fs.existsSync(p));
const puppeteer = require(dir);
(async () => {
  const [src, out] = process.argv.slice(2);
  const b = await puppeteer.launch({ headless: 'new' });
  const p = await b.newPage();
  await p.setViewport({ width: 800, height: 900, deviceScaleFactor: 1.5 });
  await p.setContent('<body style="margin:0;background:#0d1117">' + fs.readFileSync(src, 'utf8') + '</body>');
  await (await p.$('svg')).screenshot({ path: out, omitBackground: true });
  await b.close();
})();
