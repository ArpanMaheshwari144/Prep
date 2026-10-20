// Card 7: sale, har user apni had me, phir bhi kul jod se server bhara. Ilaaj: system ki had + kam zaroori ko 503.
// Agar: had paar hui to 429 ya 503? -> user ki had = 429, system ki had = 503.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: sale. Har user apni had me, par sab ka jod server ki had se upar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: gateway pe system ki had (5,000 / sec). CPU 90% paar: kam zaroori ko 503'],
  [6, YEL, 'AGAR', 'AGAR had paar hui, 429 bhejoge ya 503? -> user ki had = 429, system ki had = 503'],
];
const US = [['login', 1], ['payment', 1], ['feed', 0], ['search', 0], ['recommend', 0]];
function setup() {}
function users() {
  US.forEach(([u], i) => {
    B(20, 25 + i * 64, 120, 44, u, 'had me: 80/100');
    A(140, 47 + i * 64, 318, 175, C.msg, { n: i === 2 ? 1 : 0, at: 0.5 });
  });
}
const STEPS = [
  [[C.msg, 'Users → Gateway', 'har user apni had me (80 / 100)'], [C.bad, 'Gateway → App', 'sab andar'], [C.bad, 'App', 'CPU 100%: login / payment bhi atke']],
  [[C.msg, 'Users → Gateway', 'system ki had: 5,000 / sec'], [C.good, 'Gateway → App', 'zaroori: login, payment'], [C.acc, 'Gateway → feed, search', 'kam zaroori: 503, baad me aao']],
  [[C.msg, 'Arpan → Gateway', '101 / 100'], [C.bad, 'Gateway → Arpan', '429: tu dheere chal'], [C.msg, 'Ravi → Gateway', '20 / 100, par system bhara'], [C.acc, 'Gateway → Ravi', '503: galti humari, baad me aa']],
];
function frame(ph) {
  if (ph === 0) {
    users();
    B(320, 140, 150, 70, 'Gateway', 'sab had me: pass');
    B(640, 140, 170, 70, 'App', '', { st: 'bad' }); cross(835, 175);
    A(470, 175, 638, 175, C.bad, { n: 2 }); L(555, 160, 'sab andar', C.bad);
    bar(640, 225, 170, 14, 1, ''); T(725, 260, 'CPU 100%', { c: C.bad, b: 1, size: 14 });
    num(725, 290, 3, C.bad); L(725, 315, 'login / payment bhi atke', C.bad);
    L(450, 350, 'har user ki had theek thi, par system ki had kisi ne nahi dekhi', C.bad);
  } else if (ph === 1) {
    users();
    B(320, 140, 150, 70, 'Gateway', 'system had: 5,000/s', { st: 'good' });
    B(660, 60, 170, 60, 'App', 'login, payment', { st: 'good' });
    A(470, 160, 658, 95, C.good, { n: 2 }); L(560, 105, 'zaroori: andar', C.good);
    bar(660, 135, 170, 12, 0.85, '', 1); T(745, 165, 'CPU 85%', { c: C.good, b: 1, size: 13 });
    B(660, 240, 170, 60, 'feed, search ...', '503: baad me aao', { st: 'acc' });
    A(470, 195, 658, 265, C.acc, { n: 3 }); L(540, 290, 'kam zaroori: 503', C.acc);
    L(450, 350, 'site gira nahi: zaroori kaam chala, baaki thodi der ruke', C.good);
  } else {
    // baayein: user ki had
    B(20, 60, 120, 60, 'Arpan', '101 / 100');
    B(240, 60, 150, 60, 'Gateway', 'Arpan ki had paar', { st: 'acc' });
    A(140, 80, 238, 80, C.msg, { n: 1 }); A(238, 105, 142, 105, C.bad, { n: 2 }); L(190, 140, '429', C.bad);
    L(205, 190, '429 = tu dheere chal', C.bad);
    // daayein: system ki had
    B(480, 60, 120, 60, 'Ravi', '20 / 100');
    B(700, 60, 150, 60, 'Gateway', 'system bhara', { st: 'acc' });
    A(600, 80, 698, 80, C.msg, { n: 3 }); A(698, 105, 602, 105, C.acc, { n: 4 }); L(650, 140, '503', C.acc);
    L(665, 190, '503 = galti humari, tu baad me aa', C.acc);
    el('line', { x1: 450, y1: 30, x2: 450, y2: 230, stroke: C.line, 'stroke-width': 1, 'stroke-dasharray': '4 4' }, DYN);
    L(450, 290, 'Ravi apni had me tha, use 429 dena galat. Kiski had paar hui, wahi code', C.good);
  }
}
