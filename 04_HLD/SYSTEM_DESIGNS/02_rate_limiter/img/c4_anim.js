// Card 4: Redis gira. Ilaaj: copy taiyaar. Agar: beech ki ginti, CPU full, copy bhi gayi, sab fail-open. Wapas.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Redis gira, har request pe ?, koi ginti nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: Redis ki copy taiyaar. Main gira to copy main ban jaati'],
  [6, YEL, 'AGAR', 'AGAR copy main ban rahi thi, beech ki ginti? -> kuch second ki kho gayi'],
  [9, YEL, 'AGAR', 'AGAR CPU full, copy laga du? -> nahi, INCR = likhna, likhna sirf main pe'],
  [12, YEL, 'AGAR', 'AGAR copy bhi gayi? -> login band (fail-closed), feed khula (fail-open)'],
  [15, YEL, 'AGAR', 'AGAR sab fail-open? -> login khula, koi bhi bina had password guess karega'],
  [18, GRN, 'WAPAS', 'WAPAS: Redis wapas aaya -> phir sabki ek ginti, pehle jaisa'],
];
function setup() {}
function left(lst, fst) {
  B(20, 110, 130, 50, 'login', '', { st: lst }); B(20, 220, 130, 50, 'feed', '', { st: fst });
  A(150, 135, 238, 180); A(150, 245, 238, 200);
  B(240, 160, 120, 60, 'Gateway', '', { st: 'acc' });
}
function main(st, sub) { B(520, 50, 160, 70, 'Redis main', sub, { st }); if (st === 'bad') cross(705, 85); }
function copy(st, sub) { B(520, 250, 160, 70, 'Redis copy', sub, { st }); if (st === 'bad') cross(705, 285); }
function frame(ph) {
  if (ph === 0) {
    left(); main('bad');
    A(360, 180, 515, 95, C.bad, { dash: 1 });
    dot([410, 157], C.msg, '', '?', C.acc); dot([465, 125], C.msg, '', '?', C.acc);
  } else if (ph === 1) {
    left(); main('bad'); copy('good', 'ab MAIN');
    A(360, 200, 515, 280, C.good);
    dots(2, 410, 228, C.good, { gap: 40 });
  } else if (ph === 2) {
    left(); main('bad', 'ginti: 6'); copy('good', 'ginti: 4');
    dots(6, 740, 85, C.good, { gap: 22 });
    dots(4, 740, 285, C.good, { gap: 22 });
    dots(2, 828, 285, C.bad, { gap: 22, hollow: 1 });
    T(850, 250, '-2', { c: C.bad, b: 1, size: 18 });
  } else if (ph === 3) {
    left(); main('acc', 'CPU'); copy('', 'CPU');
    bar(710, 76, 140, 18, 1, ''); T(780, 118, '100%', { c: C.bad, b: 1, size: 14 });
    bar(710, 276, 140, 18, 0.1, '', 1); T(780, 318, '10%', { c: C.good, b: 1, size: 14 });
    A(360, 180, 515, 95, C.acc);
    dots(4, 395, 150, C.acc, { gap: 24 });
    T(440, 125, 'INCR', { c: C.acc, b: 1, size: 13, halo: 1 });
    A(360, 200, 515, 280, C.bad, { dash: 1 }); cross(440, 240);
  } else if (ph === 4) {
    left('bad', 'good'); main('bad'); copy('bad');
    dot([195, 112], C.bad, '', 'band', C.bad);
    A(360, 190, 470, 190, C.good); dot([492, 190], C.good, '', 'khula', C.good);
  } else if (ph === 5) {
    B(20, 15, 130, 50, 'Attacker', '', { st: 'bad' }); A(85, 65, 85, 106, C.bad);
    left('bad', 'good'); main('bad'); copy('bad');
    A(360, 190, 768, 190, C.bad);
    dots(10, 385, 165, C.bad, { gap: 13 });
    B(770, 160, 110, 60, 'App', '', { st: 'bad' });
    T(825, 250, '1000 guess', { c: C.bad, b: 1, size: 14 });
  } else {
    left('good', 'good'); main('good', 'ginti: ek'); copy('good', 'copy');
    A(360, 180, 515, 95, C.good);
    dots(2, 410, 157, C.good, { gap: 40 });
    A(600, 120, 600, 248, C.muted, { dash: 1 });
  }
}
