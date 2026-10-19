// Chalti tasveer: anim html (draw(t) + DUR) -> GIF. node anim2gif.js <file.html> [fps]
// html ke bagal me same naam ka .gif banta hai. ffmpeg PATH me chahiye (~/bin).
const path = require('path'), fs = require('fs'), os = require('os'), { execFileSync } = require('child_process');
const NPX = path.join(process.env.LOCALAPPDATA, 'npm-cache', '_npx');
const dir = fs.readdirSync(NPX).map(d => path.join(NPX, d, 'node_modules', 'puppeteer')).find(p => fs.existsSync(p));
const puppeteer = require(dir);
(async () => {
  const src = path.resolve(process.argv[2]), fps = +(process.argv[3] || 12);
  const tmp = fs.mkdtempSync(path.join(os.tmpdir(), 'anim-'));
  const b = await puppeteer.launch({ headless: 'new' });
  const p = await b.newPage();
  await p.setViewport({ width: 900, height: 400 });
  await p.setContent(fs.readFileSync(src, 'utf8'));
  const dur = await p.evaluate(() => DUR);
  const el = await p.$('svg');
  const n = Math.round(dur * fps);
  for (let i = 0; i < n; i++) {
    await p.evaluate(t => draw(t), i / fps);
    await el.screenshot({ path: path.join(tmp, `f${String(i).padStart(4, '0')}.png`) });
  }
  await b.close();
  const out = src.replace(/\.html$/, '.gif');
  execFileSync('ffmpeg', ['-y', '-loglevel', 'error', '-framerate', String(fps), '-i', path.join(tmp, 'f%04d.png'),
    '-vf', 'split[a][b];[a]palettegen=max_colors=64[p];[b][p]paletteuse=dither=none', '-loop', '0', out]);
  fs.rmSync(tmp, { recursive: true, force: true });
  console.log(out, (fs.statSync(out).size / 1024).toFixed(0) + ' KB', n + ' frames');
})();
