// Card 5: 3 region, har ek ki apni ginti, 50 + 50 + 50 = 150 (had 100). Ilaaj: ginti ghar region me.
const DUR = 18, FIX = 8;
const SCR = [70, 192], REG = [[330, 80], [330, 192], [330, 304]], NAMES = ['India', 'US', 'Europe'];
const HOME = [640, 80];
function setup() {
  for (const [x, y] of REG) wire(`M120 192 L${x - 70} ${y}`);
  wire('M400 192 L570 90', 'wh1'); wire('M400 304 L570 100', 'wh2'); wire('M400 80 H570', 'wh0');
  box(20, 150, 100, 84, 'Meri script', '3 region se');
  REG.forEach(([x, y], i) => box(x - 70, y - 28, 140, 56, NAMES[i]));
  box(570, 50, 150, 60, 'India Redis', 'ghar ki ginti', { id: 'home', stroke: C.acc });
}
function frame(t, fix, tl) {
  vis('home', fix); vis('wh0', fix); vis('wh1', fix); vis('wh2', fix);
  T(890, 345, 'had: 100 / minute', { a: 'end', c: C.muted, size: 12 });
  for (let i = 0; i < 3; i++) for (let k = 0; k < 4; k++) {
    const s = 0.4 + k * 0.9 + i * 0.2;
    const pts = fix && i !== 0 ? [[s, SCR], [s + 0.6, REG[i]], [s + 1.3, HOME]] : [[s, SCR], [s + 0.6, REG[i]]];
    if (fix && i === 0) pts.push([s + 1.0, HOME]);
    const end = pts[pts.length - 1][0];
    if (tl < end + 0.2) dot(path(pts, tl), C.msg);
  }
  const f = Math.min(1, Math.max(0, (tl - 0.8) / 3.6));
  if (!fix) {
    CAP.textContent = 'DIKKAT: har region ka apna Redis, apni ginti. Teeno me 50-50, jod 150';
    REG.forEach(([x, y]) => T(x + 105, y + 5, `${Math.round(50 * f)} / 100`, { c: C.good, b: 1, a: 'start', size: 13 }));
    const tot = Math.round(150 * f);
    T(890, 290, `kul andar: ${tot}`, { a: 'end', c: tot > 100 ? C.bad : C.fg, b: 1, size: 15 });
    if (tot > 100) T(890, 312, 'had tooti!', { a: 'end', c: C.bad, b: 1, size: 13 });
  } else {
    CAP.textContent = 'ILAAJ: meri ginti mere ghar region (India) me. US / Europe ki request bhi wahin gini';
    const c = Math.round(150 * f), inn = Math.min(100, c);
    T(645, 135, `ginti: ${inn} / 100`, { c: inn >= 100 ? C.bad : C.good, b: 1, size: 14 });
    T(890, 290, `kul andar: ${inn}`, { a: 'end', c: C.good, b: 1, size: 15 });
    if (c > 100) T(890, 312, `429: ${c - 100}`, { a: 'end', c: C.bad, b: 1, size: 13 });
  }
}
