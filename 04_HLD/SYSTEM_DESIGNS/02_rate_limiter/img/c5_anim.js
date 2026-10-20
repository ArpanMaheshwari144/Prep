// Card 5: 3 region, 50 + 50 + 50 = 150. Ilaaj: ginti ghar region (India) me. Agar: ghar door, ghar gira. Wapas.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har region ki apni ginti. 50 + 50 + 50 = 150, had 100'],
  [3, GRN, 'ILAAJ', 'ILAAJ: meri ginti mere ghar region (India) me. US / Europe bhi wahin gini'],
  [6, YEL, 'AGAR', 'AGAR ghar door hai? -> sirf paisa / login ghar tak, baaki har second sync'],
  [9, YEL, 'AGAR', 'AGAR ghar region gir gaya? -> Route 53 doosre region pe, ginti 0 se'],
  [12, GRN, 'WAPAS', 'WAPAS: India wapas aaya, ginti phir ghar region me'],
];
const R = [['India', 60], ['US', 175], ['Europe', 290]];
function setup() {}
function regs(st) {
  R.forEach(([n, y], i) => { B(260, y - 28, 140, 56, n, '', { st: st && st[n] });
    A(130, 175, 258, y, C.msg, { n: i === 1 ? 1 : 0, at: 0.4 }); });
}
function frame(ph) {
  B(20, 140, 110, 70, 'Meri script');
  if (ph === 0) {
    regs();
    R.forEach(([n, y]) => { A(400, y, 478, y, C.good, { n: 2 }); B(480, y - 22, 120, 44, '50 / 100', '', { st: 'good' }); });
    num(640, 175, 3, C.bad);
    T(770, 165, 'kul: 150', { c: C.bad, b: 1, size: 26 }); T(770, 200, 'had: 100', { c: C.muted, size: 15 });
  } else if (ph === 1 || ph === 4) {
    regs(); B(560, 30, 190, 70, 'India Redis', '100 / 100', { st: 'acc' });
    A(400, 60, 558, 65, C.good, { n: 2 }); A(400, 175, 558, 80, C.good, { n: 2, at: 0.6 });
    A(400, 290, 558, 95, C.good, { n: 2, at: 0.65 });
    if (ph === 1) { num(565, 175, 3, C.bad); dots(5, 590, 175, C.bad, { gap: 24 }); T(640, 215, '429: 50', { c: C.bad, b: 1, size: 15 }); }
    else L(655, 125, 'India wapas aaya', C.good);
  } else if (ph === 2) {
    regs({ US: 'acc' });
    B(560, 30, 190, 70, 'India Redis', 'ghar'); L(655, 18, 'ghar door', C.acc);
    A(400, 165, 558, 75, C.acc, { n: 2, at: 0.3 }); T(500, 95, 'login / paisa', { c: C.acc, b: 1, size: 13, halo: 1 });
    T(500, 118, '100-200 ms', { c: C.muted, size: 12, halo: 1 });
    B(560, 250, 190, 70, 'US ginti', 'feed', { st: 'good' }); A(400, 185, 558, 270, C.good, { n: 3 });
    A(655, 248, 655, 102, C.muted, { dash: 1, n: 4 }); T(735, 180, 'har 1 sec sync', { c: C.muted, size: 12 });
  } else {
    regs({ India: 'bad' });
    B(560, 30, 190, 70, 'India Redis', '', { st: 'bad' }); cross(775, 65); L(655, 18, 'ghar gira', C.bad);
    B(560, 140, 190, 60, 'Route 53', '', { st: 'acc' });
    B(560, 250, 190, 70, 'US Redis', 'ginti: 0', { st: 'good' });
    A(400, 175, 558, 170, C.msg, { n: 2 }); A(655, 200, 655, 248, C.good, { n: 3 });
  }
}
