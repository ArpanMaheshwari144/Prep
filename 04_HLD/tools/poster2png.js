// Poster (cN_poster.html, ek SVG) -> cN_poster.png, poore asli size me. node poster2png.js a_poster.html b_poster.html ...
const path = require('path'), fs = require('fs');
const NPX = path.join(process.env.LOCALAPPDATA, 'npm-cache', '_npx');
const dir = fs.readdirSync(NPX).map(d => path.join(NPX, d, 'node_modules', 'puppeteer')).find(p => fs.existsSync(p));
const puppeteer = require(dir);
(async () => {
  const b = await puppeteer.launch({ headless: 'new' });
  const p = await b.newPage();
  await p.setViewport({ width: 1600, height: 1000, deviceScaleFactor: 1 });
  for (const f of process.argv.slice(2)) {
    await p.goto('file:///' + path.resolve(f).split(path.sep).join('/'));
    const el = await p.$('svg');
    const png = f.replace(/\.html$/, '.png');
    await el.screenshot({ path: png });
    console.log(png);
  }
  await b.close();
})();
