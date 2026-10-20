// Card 8: attack + Redis Cluster gira. Ilaaj: har gateway apni ginti, had 100 / 4 = 25. Agar: baanti nahi, gateway badhe.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Redis gaya, ginti kahin nahi. Fail-open: attacker ki sab andar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har gateway apni memory me ginta. Had 100, 4 gateway -> har ek 25'],
  [6, YEL, 'AGAR', 'AGAR had baanti hi nahi, har gateway ne 100 di? -> 4 x 100 = 400 mil gayi'],
  [9, YEL, 'AGAR', 'AGAR gateway badh gaye? -> N config se lo, aur had dobara baanto'],
  [12, GRN, 'WAPAS', 'WAPAS: Redis wapas aaya -> phir se sabki ek hi ginti Redis me'],
];
function setup() {}
function gws(n, sub, st) {
  const h = n === 5 ? 46 : 52, g = n === 5 ? 66 : 82;
  for (let i = 0; i < n; i++) {
    const y = 20 + i * g;
    B(260, y, 130, h, `Gateway ${i + 1}`, sub, { st });
    A(130, 175, 258, y + h / 2); A(390, y + h / 2, 598, 175);
  }
}
function frame(ph) {
  B(20, 135, 110, 80, 'Attacker', '', { st: 'bad' });
  const up = ph === 4;
  B(600, 285, 160, 50, 'Redis Cluster', '', { st: up ? 'good' : 'bad' }); if (!up) cross(785, 310);
  if (ph === 0) {
    gws(4, '', ''); B(600, 130, 140, 90, 'Server', 'CPU', { st: 'bad' });
    bar(760, 165, 110, 18, 1, ''); T(815, 205, '100%', { c: C.bad, b: 1, size: 16 });
    dots(5, 430, 115, C.bad, { gap: 26 });
  } else if (ph === 1) {
    gws(4, 'had 25', 'good'); B(600, 130, 140, 90, 'Server', 'CPU', { st: 'good' });
    bar(760, 165, 110, 18, 0.4, '', 1); T(815, 205, '40%', { c: C.good, b: 1, size: 16 });
    for (let i = 0; i < 4; i++) dot([228, 46 + i * 82], C.bad, '', '429', C.bad);
  } else if (ph === 2) {
    gws(4, 'had 100', 'acc'); B(600, 130, 140, 90, 'Server', 'CPU', { st: 'acc' });
    T(760, 100, 'user ko: 400', { c: C.bad, b: 1, size: 18, a: 'start' });
    T(760, 250, 'had: 100', { c: C.muted, size: 14, a: 'start' });
  } else if (ph === 3) {
    gws(5, 'had 20', 'good'); B(600, 130, 140, 90, 'Server');
    T(820, 100, 'N = 5', { c: C.acc, b: 1, size: 18 }); T(820, 255, '100 / 5 = 20', { c: C.good, b: 1, size: 14 });
  } else {
    gws(4, '', 'good'); B(600, 130, 140, 90, 'Server', '', { st: 'good' });
    for (let i = 0; i < 4; i++) A(390, 46 + i * 82, 598, 305, C.good, { dash: 1 });
    T(830, 260, 'ek ginti', { c: C.good, b: 1, size: 15 });
  }
}
