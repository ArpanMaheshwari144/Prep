// Card 8: attack + poora Redis Cluster gira. Ilaaj: har gateway server apni memory me gine, had baant do (100 / 4 = 25).
const DUR = 18, FIX = 8;
const ATK = [70, 192], G = [[330, 70], [330, 152], [330, 234], [330, 316]], APP = [620, 192], RD = [620, 330];
function setup() {
  G.forEach(([x, y], i) => { box(x - 55, y - 26, 110, 52, `Gateway ${i + 1}`); wire(`M120 192 L${x - 55} ${y}`); wire(`M${x + 55} ${y} L560 192`); });
  box(20, 150, 100, 84, 'Attacker', 'raat ko');
  box(560, 150, 120, 84, 'Server', 'aam API');
  box(560, 300, 120, 50, 'Redis Cluster');
}
function frame(t, fix, tl) {
  cross(620, 325);
  let inn = 0, no = 0;
  const per = [0, 0, 0, 0];
  for (let k = 0; k < 28; k++) {
    const g = k % 4, s = 0.3 + k * 0.17;
    if (tl < s + 0.5) { dot(path([[s, ATK], [s + 0.5, G[g]]], tl), C.msg); continue; }
    per[g]++;
    const allow = !fix || per[g] <= 3;
    if (allow) { inn++; if (tl < s + 0.9) dot(path([[s + 0.5, G[g]], [s + 0.9, APP]], tl), C.msg); }
    else { no++; if (tl < s + 0.9) dot(G[g], C.bad, '', '429'); }
  }
  if (!fix) {
    CAP.textContent = 'DIKKAT: Redis gaya, ginti kahin nahi. Fail-open: attacker ki sab request andar';
    const f = inn / 28; bar(570, 245, 100, 16, f, '');
    T(620, 285, `CPU ${Math.round(100 * f)}%`, { c: f > 0.9 ? C.bad : C.fg, b: 1 });
    if (f >= 1) T(780, 192, 'site band', { c: C.bad, b: 1, size: 15 });
  } else {
    CAP.textContent = 'ILAAJ: har gateway apni memory me gine. Had 100, 4 server -> har ek 25 (local fallback)';
    G.forEach(([x, y], i) => T(x, y + 42, `apni ginti: ${Math.min(per[i], 3) * 8 + (per[i] > 3 ? 1 : 0)} / 25`,
      { c: per[i] > 3 ? C.bad : C.good, size: 11, b: 1 }));
    const f = Math.min(0.5, inn / 28); bar(570, 245, 100, 16, f, '');
    T(620, 285, `CPU ${Math.round(100 * f)}%`, { c: C.good, b: 1 });
    T(780, 192, 'site chalu', { c: C.good, b: 1, size: 15 });
  }
}
