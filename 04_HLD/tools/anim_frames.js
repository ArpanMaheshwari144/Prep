// Khud dekhne ke liye: anim html ke kuch pal PNG me. node anim_frames.js <out_dir> <file.html> t1 t2 ...
// t = scene ka time (sec). Har html ka <naam>_<t>.png banta hai.
// t na do aur scene me PH ho: har phase ke end se 0.1 s pehle ka frame -> <out_dir>/cN_1.png, cN_2.png ...
const path = require('path'), fs = require('fs');
const NPX = path.join(process.env.LOCALAPPDATA, 'npm-cache', '_npx');
const dir = fs.readdirSync(NPX).map(d => path.join(NPX, d, 'node_modules', 'puppeteer')).find(p => fs.existsSync(p));
const puppeteer = require(dir);
(async () => {
  const [out, src, ...ts] = process.argv.slice(2);
  const b = await puppeteer.launch({ headless: 'new' });
  const p = await b.newPage();
  await p.setViewport({ width: 900, height: 620 });
  await p.setContent(fs.readFileSync(src, 'utf8'));
  const el = await p.$('svg');
  if (!ts.length) {
    const ends = await p.evaluate(() => PH.map((ph, i) => (i + 1 < PH.length ? PH[i + 1][0] : DUR) - 0.1));
    const base = path.basename(src, '.html').replace(/_anim$/, '');
    for (let i = 0; i < ends.length; i++) {
      await p.evaluate(t => draw(t), ends[i]);
      await el.screenshot({ path: path.join(out, `${base}_${i + 1}.png`) });
    }
  }
  for (const t of ts) {
    await p.evaluate(t => draw(t), +t);
    await el.screenshot({ path: path.join(out, path.basename(src, '.html') + '_' + t + '.png') });
  }
  await b.close();
})();
