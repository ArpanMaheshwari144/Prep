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
function regs(st) { R.forEach(([n, y]) => { B(260, y - 28, 140, 56, n, '', { st: st && st[n] }); A(130, 175, 258, y); }); }
function frame(ph) {
  B(20, 140, 110, 70, 'Meri script');
  if (ph === 0) {
    regs();
    R.forEach(([n, y]) => { A(400, y, 478, y); B(480, y - 22, 120, 44, '50 / 100', '', { st: 'good' }); });
    T(760, 165, 'kul: 150', { c: C.bad, b: 1, size: 26 }); T(760, 200, 'had: 100', { c: C.muted, size: 15 });
  } else if (ph === 1 || ph === 4) {
    regs(); B(560, 30, 190, 70, 'India Redis', '100 / 100', { st: 'acc' });
    A(400, 60, 558, 65, C.good); A(400, 175, 558, 80, C.good); A(400, 290, 558, 95, C.good);
    if (ph === 1) { dots(5, 590, 175, C.bad, { gap: 24 }); T(640, 215, '429: 50', { c: C.bad, b: 1, size: 15 }); }
    else T(655, 140, 'ghar = India', { c: C.good, b: 1, size: 14 });
  } else if (ph === 2) {
    regs({ US: 'acc' });
    B(560, 30, 190, 70, 'India Redis', 'ghar');
    A(400, 165, 558, 75, C.acc); T(470, 95, 'login / paisa', { c: C.acc, b: 1, size: 13, halo: 1 });
    T(470, 118, '100-200 ms', { c: C.muted, size: 12, halo: 1 });
    B(560, 250, 190, 70, 'US ginti', 'feed', { st: 'good' }); A(400, 185, 558, 270, C.good);
    A(655, 248, 655, 102, C.muted, { dash: 1 }); T(730, 180, 'har 1 sec sync', { c: C.muted, size: 12 });
  } else {
    regs({ India: 'bad' });
    B(560, 30, 190, 70, 'India Redis', '', { st: 'bad' }); cross(775, 65);
    B(560, 140, 190, 60, 'Route 53', '', { st: 'acc' });
    B(560, 250, 190, 70, 'US Redis', 'ginti: 0', { st: 'good' });
    A(400, 175, 558, 170); A(655, 200, 655, 248, C.good);
  }
}
