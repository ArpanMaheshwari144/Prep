// Card 8: har worker apni 100 / sec -> 100 worker = 100 guna -> company ne 429 diya.
// Ilaaj: saanjha token bucket (Redis), kul 100 / sec. Agar: token nahi, worker badhe, 1 lakh email, Redis gira.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har worker apni speed 100 / sec. 100 worker = 100 guna -> 429'],
  [3, GRN, 'ILAAJ', 'ILAAJ: sab worker ka ek saanjha token bucket (Redis). Token mila to bhejo'],
  [6, YEL, 'AGAR', 'AGAR bucket se token nahi mila? -> line me intezaar, ye fail nahi'],
  [9, YEL, 'AGAR', 'AGAR worker badh gaye? -> speed wahi. Bucket saanjha, kul 100 / sec'],
  [12, YEL, 'AGAR', 'AGAR 1 lakh email, had kam? -> had badhwao + kai account lo'],
  [15, YEL, 'AGAR', 'AGAR bucket wala Redis gir gaya? -> har worker apni chhoti local had'],
  [18, GRN, 'WAPAS', 'WAPAS: sab worker ek hi bucket se token lete. Kul 100 / sec, 429 nahi'],
];
const WY = [55, 135, 215, 305], WN = ['W1', 'W2', 'W3', 'W100'];
function setup() {}
function ws(st, sub) {
  WY.forEach((y, i) => B(30, y - 22, 90, 44, WN[i], sub, { st }));
  T(75, 268, '...', { c: C.muted, size: 16 });
}
function bucket(st, sub) { B(330, 150, 150, 80, 'Redis bucket', sub || '100 token / sec', { st }); }
function co(st, sub, nm) { B(680, 150, 180, 80, nm || 'SMS company', sub || 'had 100 / sec', { st }); }
function toBucket(c, n, dash) { WY.forEach((y, i) => A(120, y, 328, 175 + i * 10, c, { n: i === 1 ? n : 0, at: 0.5, dash })); }
function frame(ph) {
  if (ph === 0) {
    ws('bad'); co('bad', 'had 100 / sec');
    WY.forEach((y, i) => A(120, y, 678, 170 + i * 13, C.msg, { n: i === 1 ? 1 : 0, at: 0.4 }));
    L(400, 40, 'har worker: apni 100 / sec', C.bad);
    A(770, 232, 770, 280, C.bad, { n: 2, at: 0.5 }); L(790, 305, '100 guna aaye -> 429', C.bad);
  } else if (ph === 1) {
    ws('good'); bucket('acc'); co('good', '429: 0');
    toBucket(C.acc, 1); L(230, 40, 'pehle token lo', C.acc);
    A(480, 190, 678, 190, C.good, { n: 2 }); L(580, 170, 'token mila -> bhejo', C.good);
    L(405, 260, 'kul 100 / sec', C.good);
  } else if (ph === 2) {
    ws('acc', 'ruka'); bucket('acc', 'token: 0'); co('good', '429: 0');
    num(405, 252, 1, C.acc); L(423, 257, 'khaali', C.acc, 'start');
    toBucket(C.acc, 2, 1); L(230, 40, 'nahi mila', C.acc);
    A(480, 190, 678, 190, C.line, { dash: 1 });
    num(130, 340, 3, C.acc); L(148, 345, 'line me ruko, retry ginti nahi badhti', C.acc, 'start');
    L(580, 25, 'hal: intezaar karo. Ye fail nahi, khabar phenko nahi', C.good);
  } else if (ph === 3) {
    ws('good'); bucket('acc'); co('good', '429: 0');
    B(170, 15, 110, 44, 'naye worker', '', { st: 'acc' }); num(160, 12, 1, C.acc);
    toBucket(C.acc, 2); A(225, 59, 350, 148, C.acc, { n: 2, at: 0.5 });
    A(480, 190, 678, 190, C.good, { n: 3 }); L(580, 170, 'kul 100 / sec', C.good);
    L(580, 25, 'hal: speed wahi rehti, bucket saanjha', C.good);
  } else if (ph === 4) {
    ws('good'); bucket('acc'); co('acc', 'had kam', 'Email company');
    cells(150, 15, Array(6).fill(['email', C.acc]), { w: 52 }); num(490, 28, 1, C.acc); L(508, 33, '1 lakh email line me', C.acc, 'start');
    toBucket(C.acc, 0);
    A(480, 190, 678, 190, C.acc, { n: 2 }); L(580, 170, 'dheere', C.acc);
    B(680, 280, 180, 50, 'account 2', '', { st: 'good' }); A(480, 215, 678, 300, C.good, { n: 3, at: 0.5 });
    L(770, 360 - 15, 'had badhwao + kai account', C.good);
    L(580, 110, 'hal: tab tak line dheere khaali', C.good);
  } else if (ph === 5) {
    ws('good', 'local had'); bucket('bad', 'gira'); co('good', 'had paar nahi');
    num(405, 215, 1, C.bad); cross(500, 165);
    const YB = [60, 118, 262, 322];
    WY.forEach((y, i) => AP([[120, y], [600, YB[i]], [678, 175 + i * 10]], C.good, { n: i === 1 ? 2 : 0, at: 0.75, dash: 1 }));
    L(580, 25, 'hal: chhoti local had pe chalo (fail-closed)', C.good);
    L(580, 47, 'company ki had paar nahi hoti', C.good);
  } else {
    ws('good'); bucket('good'); co('good', '429: 0');
    toBucket(C.good, 1);
    A(480, 190, 678, 190, C.good, { n: 2 }); L(580, 170, 'kul 100 / sec', C.good);
  }
}
