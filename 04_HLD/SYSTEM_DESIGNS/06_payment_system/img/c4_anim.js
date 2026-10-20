// Card 4: PSP 30 sec, timeout nahi -> har thread ruka, sab atke. Ilaaj: 2-3 sec me PENDING, 5 fail -> circuit OPEN.
// Agar: circuit kab khulega (30 sec baad test call), PSP poora band (doosra PSP). Wapas: test call chali, circuit band.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: timeout nahi. Har thread PSP (30 sec) tak ruka, sab atke, band'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 2-3 sec me jawab nahi -> PENDING. 5 fail -> circuit OPEN'],
  [6, YEL, 'AGAR', 'AGAR circuit OPEN, phir kab khulega? -> 30 sec baad ek test call'],
  [9, YEL, 'AGAR', 'AGAR PSP hi poora band? -> doosra PSP. Par ek payment ek hi PSP pe'],
  [12, GRN, 'WAPAS', 'WAPAS: test call chali -> circuit band, payment phir PSP tak'],
];
function setup() {}
function threads(s, c) { const it = [0, 1, 2, 3].map(() => [s, c]); cells(250, 150, it, { w: 50, h: 28 }); cells(250, 190, it, { w: 50, h: 28 }); }
// o: { us, sv, cb, ps, p2 } st ; s: { cb, ps }
function base(o = {}, s = {}) {
  B(20, 150, 110, 70, 'Users', 'top-up 2000', { st: o.us });
  B(230, 100, 250, 140, 'Payment Svc', '', { st: o.sv });
  if (!o.noCb) B(540, 150, 130, 70, 'Circuit', s.cb || '', { st: o.cb });
  B(730, 150, 150, 70, 'PSP (bank)', s.ps || 'dheema: 30 sec', { st: o.ps });
  if (o.p2) B(730, 270, 150, 60, 'PSP 2', 'doosra', { st: o.p2 });
}
function frame(ph) {
  if (ph === 0) {
    base({ us: 'bad', sv: 'bad', noCb: 1, ps: 'acc' });
    A(130, 175, 228, 175, C.msg, { n: 1 });
    A(480, 185, 728, 185, C.acc, { n: 2, at: 0.3 }); L(605, 170, 'jawab ka intezaar: 30 sec', C.acc);
    threads('ruka', C.bad); num(215, 262, 3, C.bad); L(235, 267, 'saare thread ruke', C.bad, 'start');
    A(130, 205, 228, 205, C.bad, { dash: 1, n: 4, at: 0.4 });
    L(75, 290, 'naye atke', C.bad); L(355, 300, 'system band', C.bad);
  } else if (ph === 1) {
    base({ us: 'good', sv: 'good', cb: 'bad', ps: 'acc' }, { cb: 'OPEN' });
    A(130, 165, 228, 165, C.msg, { n: 1 });
    A(480, 185, 538, 185, C.acc, { n: 2 });
    threads('free', C.good); L(355, 270, '2-3 sec -> PENDING', C.acc);
    L(355, 290, '(baad me pata karo)', C.acc);
    num(605, 130, 3, C.bad); L(605, 110, '5 fail -> OPEN', C.bad);
    A(670, 185, 728, 185, C.bad, { dash: 1 }); cross(700, 185);
    L(700, 265, 'call jaati hi nahi', C.bad);
    A(228, 210, 132, 210, C.good, { n: 4, at: 0.5 }); L(75, 245, '"abhi nahi"', C.good);
    L(75, 265, 'paisa hila nahi', C.good);
  } else if (ph === 2) {
    base({ sv: '', cb: 'acc', ps: 'acc' }, { cb: 'OPEN' });
    A(130, 175, 228, 175, C.line); A(480, 185, 538, 185, C.line);
    threads('free', C.good);
    num(605, 130, 1, C.acc); L(605, 110, '30 sec ho gaye', C.acc);
    A(670, 185, 728, 185, C.acc, { dash: 1, n: 2 }); L(700, 250, 'ek test call', C.acc);
    L(450, 330, 'hal: chali to circuit band, nahi to phir OPEN', C.good);
  } else if (ph === 3) {
    base({ sv: 'good', cb: '', ps: 'bad', p2: 'good' }, { ps: 'poora band' });
    cross(712, 128); num(750, 128, 1, C.bad); L(768, 133, 'poora band', C.bad, 'start');
    threads('free', C.good);
    A(130, 175, 228, 175, C.msg, { n: 2 });
    A(480, 185, 538, 185, C.good, { n: 3 });
    AP([[670, 210], [700, 300], [728, 300]], C.good, { n: 4, at: 0.5 }); L(640, 300, 'doosra PSP', C.good, 'end');
    L(450, 330, 'hal: doosra PSP. Par ek payment hamesha ek hi PSP pe', C.good);
  } else {
    base({ us: 'good', sv: 'good', cb: 'good', ps: 'good' }, { cb: 'band (CLOSED)', ps: 'wapas' });
    num(605, 130, 1, C.good); L(605, 110, 'test call chali', C.good);
    A(130, 175, 228, 175, C.msg, { n: 2 });
    A(480, 185, 538, 185, C.good, { n: 3 });
    A(670, 185, 728, 185, C.good, { n: 3 }); L(700, 250, 'payment phir PSP tak', C.good);
    threads('free', C.good);
  }
}
