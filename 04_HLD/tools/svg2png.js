// SVG -> PNG (khud dekhne ke liye, GitHub jaisa 720 chauda). node svg2png.js out_dir a.svg b.svg ...
const path = require('path'), fs = require('fs');
const NPX = path.join(process.env.LOCALAPPDATA, 'npm-cache', '_npx');
const dir = fs.readdirSync(NPX).map(d => path.join(NPX, d, 'node_modules', 'puppeteer')).find(p => fs.existsSync(p));
const puppeteer = require(dir);
(async () => {
  const [out, ...files] = process.argv.slice(2);
  const b = await puppeteer.launch({ headless: 'new' });
  const p = await b.newPage();
  await p.setViewport({ width: 760, height: 600 });
  for (const f of files) {
    const svg = fs.readFileSync(f, 'utf8');
    await p.setContent(`<body style="margin:0;background:#fff">${svg}</body>`);
    const el = await p.$('svg');
    const png = path.join(out, path.basename(f).replace(/\.svg$/, '.png'));
    await el.screenshot({ path: png });
    console.log(png);
  }
  await b.close();
})();
