// Card 8: Diwali 3000 payment / sec, ek machine baith gayi. Ilaaj: kai machine + LB, stateless.
// Agar: peeche ek machine mari -> LB GET /health, jawab nahi = bahar.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Diwali shaam, 3000 payment / sec. Ek hi machine, CPU bhara, sab atke'],
  [3, GRN, 'ILAAJ', 'ILAAJ: kai machine, aage LB. Machine kuch yaad nahi rakhti, yaad DB aur key store me'],
  [6, YEL, 'AGAR', 'AGAR peeche ek machine mar gayi? -> LB GET /health, jawab nahi = bahar'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Users → Payment Svc', 'Diwali: 3000 payment / sec'], [C.bad, 'Payment Svc → Users', 'CPU 100%, timeout'], [C.bad, 'Users', 'log dobara tap karte: aur bojh']],
  [[C.msg, 'Users → LB', '3000 / sec'], [C.good, 'LB → App x3', 'koi bhi App koi bhi payment le'], [C.good, 'App → DB + key store', 'yaad yahan, App me kuch nahi']],
  [[C.acc, 'LB → App-2', '/health? jawab nahi'], [C.bad, 'App-2', 'list se bahar'], [C.good, 'LB → App-1, App-3', 'payment wahan. User ko pata bhi nahi']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 160, 70, 'Users', '3000 / sec');
    B(350, 120, 220, 90, 'Payment Svc', 'ek hi machine', { st: 'bad' });
    bar(370, 180, 180, 12, 1, ''); T(460, 235, 'CPU 100%', { c: C.bad, b: 1, size: 13 });
    A(180, 150, 348, 150, C.msg, { n: 1 });
    A(348, 185, 182, 185, C.bad, { n: 2, dash: 1 }); L(265, 210, 'timeout', C.bad);
    num(560, 290, 3, C.bad); L(580, 295, 'log dobara tap karte: aur bojh', C.bad, 'start');
    L(450, 320, 'ek machine ki had, aur Diwali ki bheed', C.bad);
  } else if (ph === 1) {
    B(20, 140, 120, 70, 'Users', '3000 / sec');
    B(200, 140, 120, 70, 'LB', '', { st: 'acc' });
    B(400, 30, 160, 60, 'App-1', 'kuch yaad nahi', { st: 'good' });
    B(400, 145, 160, 60, 'App-2', 'kuch yaad nahi', { st: 'good' });
    B(400, 260, 160, 60, 'App-3', 'kuch yaad nahi', { st: 'good' });
    B(670, 130, 200, 90, 'DB + key store', 'yaad yahan', { st: 'acc' });
    A(140, 175, 198, 175, C.msg, { n: 1 });
    A(320, 160, 398, 65, C.good, { n: 2 }); A(320, 175, 398, 175, C.good); A(320, 190, 398, 285, C.good);
    A(560, 60, 668, 150, C.muted); A(560, 175, 668, 175, C.muted, { n: 3 }); A(560, 290, 668, 200, C.muted);
    L(450, 345, 'koi bhi App koi bhi payment le. Bheed badhi to aur App jodo', C.good);
  } else {
    B(20, 140, 120, 70, 'Users', '');
    B(200, 140, 120, 70, 'LB', 'har 5 sec', { st: 'acc' });
    B(450, 30, 170, 60, 'App-1', 'chalu', { st: 'good' });
    B(450, 145, 170, 60, 'App-2', 'mari', { st: 'bad' }); cross(645, 175);
    B(450, 260, 170, 60, 'App-3', 'chalu', { st: 'good' });
    A(140, 175, 198, 175, C.msg);
    A(320, 175, 448, 175, C.acc, { n: 1, dash: 1 }); L(385, 160, '/health?', C.acc);
    num(720, 175, 2, C.bad); L(740, 180, 'jawab nahi: bahar', C.bad, 'start');
    A(320, 160, 448, 65, C.good, { n: 3 }); A(320, 190, 448, 285, C.good); L(360, 250, 'payment yahan', C.good, 'end');
    L(450, 345, 'user ko pata bhi nahi chala. App-2 theek hui to wapas list me', C.good);
  }
}
