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
function left(lst, fst, n) {
  B(20, 110, 130, 50, 'login', '', { st: lst }); B(20, 220, 130, 50, 'feed', '', { st: fst });
  A(150, 135, 238, 180, C.msg, { n }); A(150, 245, 238, 200, C.msg, { n });
  B(240, 160, 120, 60, 'Gateway', '', { st: 'acc' });
}
function main(st, sub) { B(520, 50, 160, 70, 'Redis main', sub, { st }); if (st === 'bad') cross(705, 85); }
function copy(st, sub) { B(520, 250, 160, 70, 'Redis copy', sub, { st }); if (st === 'bad') cross(705, 285); }
function frame(ph) {
  if (ph === 0) {
    left('', '', 1); main('bad'); L(600, 140, 'gira', C.bad);
    A(360, 180, 515, 95, C.bad, { dash: 1, n: 2, at: 0.2 });
    dot([425, 150], C.msg, '', '?', C.acc); dot([470, 128], C.msg, '', '?', C.acc);
  } else if (ph === 1) {
    left('', '', 2); main('bad'); copy('good', 'ab MAIN');
    A(600, 122, 600, 247, C.acc, { dash: 1, n: 1 }); L(660, 190, 'copy main bani', C.acc, 'start');
    A(360, 200, 515, 280, C.good, { n: 3, at: 0.25 });
    dots(2, 430, 238, C.good, { gap: 40 });
  } else if (ph === 2) {
    left(); main('bad', 'ginti: 6'); copy('good', 'ginti: 4');
    num(735, 60, 1, C.good); dots(6, 760, 85, C.good, { gap: 22, cols: 3 });
    A(600, 122, 600, 247, C.acc, { dash: 1, n: 2 }); L(530, 190, 'switch ho raha', C.acc);
    num(735, 260, 3, C.good); dots(4, 760, 285, C.good, { gap: 22, cols: 3 });
    dots(2, 804, 307, C.bad, { gap: 22, hollow: 1 });
    T(860, 345, '-2', { c: C.bad, b: 1, size: 18 });
    L(330, 330, 'hal: chalta hai, rate limit me thodi chhoot theek', C.good);
  } else if (ph === 3) {
    left('', '', 1); main('acc', 'CPU'); copy('', 'CPU');
    L(600, 140, 'CPU full, gira nahi', C.bad);
    bar(710, 76, 140, 18, 1, ''); T(780, 118, '100%', { c: C.bad, b: 1, size: 14 });
    bar(710, 276, 140, 18, 0.1, '', 1); T(780, 318, '10%', { c: C.good, b: 1, size: 14 });
    A(360, 180, 515, 95, C.acc, { n: 2, at: 0.85 });
    dots(4, 385, 155, C.acc, { gap: 24 });
    L(430, 128, 'INCR');
    A(360, 200, 515, 280, C.bad, { dash: 1, n: 3, at: 0.25 }); cross(450, 248);
  } else if (ph === 4) {
    left('bad', 'good'); main('bad'); copy('bad'); L(600, 345, 'copy bhi gayi', C.bad);
    A(150, 135, 238, 180, C.bad, { n: 1 }); A(150, 245, 238, 200, C.good, { n: 1 });
    L(300, 145, 'login: band', C.bad);
    A(360, 190, 470, 190, C.good, { n: 2 }); dot([492, 190], C.good, '', 'feed: khula', C.good);
  } else if (ph === 5) {
    B(20, 15, 130, 50, 'Attacker', '', { st: 'bad' }); A(85, 65, 85, 106, C.bad, { n: 1 });
    left('bad', 'good'); main('bad'); copy('bad');
    L(300, 245, 'sab fail-open', C.bad);
    A(150, 135, 238, 180, C.bad, { n: 2 });
    A(360, 190, 768, 190, C.bad, { n: 3, at: 0.15 });
    dots(10, 430, 165, C.bad, { gap: 13 });
    B(770, 160, 110, 60, 'App', '', { st: 'bad' });
    T(825, 250, '1000 guess', { c: C.bad, b: 1, size: 14 });
    L(160, 320, 'hal: login fail-closed (band)', C.good);
  } else {
    left('good', 'good', 1); main('good', 'ginti: ek'); copy('good', 'copy');
    L(600, 30, 'wapas aaya', C.good);
    A(360, 180, 515, 95, C.good, { n: 2 });
    dots(2, 420, 152, C.good, { gap: 40 });
    A(600, 122, 600, 247, C.muted, { dash: 1, n: 3 });
  }
}
