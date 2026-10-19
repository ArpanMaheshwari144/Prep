// Chalti tasveer: anim html (draw(t) + DUR, FIX ho to) -> GIF. node anim2gif.js <file.html> [fps] [slow]
// slow = kitna dheema chale (1.6 = 1.6 guna dheema). Dikkat ka aakhri frame aur ilaaj ka aakhri frame HOLD sec ruke.
// html ke bagal me same naam ka .gif banta hai. ffmpeg PATH me chahiye (~/bin).
const path = require('path'), fs = require('fs'), os = require('os'), { execFileSync } = require('child_process');
const NPX = path.join(process.env.LOCALAPPDATA, 'npm-cache', '_npx');
const dir = fs.readdirSync(NPX).map(d => path.join(NPX, d, 'node_modules', 'puppeteer')).find(p => fs.existsSync(p));
const puppeteer = require(dir);
const HOLD = 3.5;
(async () => {
  const src = path.resolve(process.argv[2]), fps = +(process.argv[3] || 12), slow = +(process.argv[4] || 1.6);
  const tmp = fs.mkdtempSync(path.join(os.tmpdir(), 'anim-'));
  const b = await puppeteer.launch({ headless: 'new' });
  const p = await b.newPage();
  await p.setViewport({ width: 900, height: 400 });
  await p.setContent(fs.readFileSync(src, 'utf8'));
  const dur = await p.evaluate(() => DUR);
  const fix = await p.evaluate(() => (typeof FIX !== 'undefined' ? FIX : 7.5));
  const el = await p.$('svg');
  const outFps = fps / slow, holdN = Math.round(HOLD * outFps);
  let k = 0;
  const shot = async t => {
    await p.evaluate(t => draw(t), t);
    const f = path.join(tmp, `f${String(k++).padStart(5, '0')}.png`);
    await el.screenshot({ path: f });
    return f;
  };
  const n = Math.round(dur * fps);
  for (let i = 0; i < n; i++) {
    const t = i / fps, next = (i + 1) / fps;
    const f = await shot(t);
    // phase ka aakhri frame: thoda ruko
    if ((t < fix && next >= fix) || i === n - 1)
      for (let h = 0; h < holdN; h++) fs.copyFileSync(f, path.join(tmp, `f${String(k++).padStart(5, '0')}.png`));
  }
  await b.close();
  const out = src.replace(/\.html$/, '.gif');
  execFileSync('ffmpeg', ['-y', '-loglevel', 'error', '-framerate', String(outFps), '-i', path.join(tmp, 'f%05d.png'),
    '-vf', 'split[a][b];[a]palettegen=max_colors=64[p];[b][p]paletteuse=dither=none', '-loop', '0', out]);
  fs.rmSync(tmp, { recursive: true, force: true });
  console.log(out, (fs.statSync(out).size / 1024).toFixed(0) + ' KB', k + ' frames', (k / outFps).toFixed(1) + ' sec');
})();
