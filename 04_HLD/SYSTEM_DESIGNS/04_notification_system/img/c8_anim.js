// Card 8: sale me laakhon message, har worker apni speed se, company ne 429 diya. Ilaaj: saanjha token bucket (Redis),
// kul 100 / sec. Agar: bucket wala Redis gira -> har worker apni surakshit had (100 / 4 = 25).
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: company ki had 100 / sec. Har worker apni taraf se 100, 4 worker = 400. 429'],
  [3, GRN, 'ILAAJ', 'ILAAJ: sab worker ek saanjha token bucket (Redis) se token lete. Kul 100 / sec. Pal-pal'],
  [6, YEL, 'AGAR', 'AGAR bucket wala Redis hi gira? -> har worker apni surakshit had (100 / 4 = 25)'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
function workers(sub, st, c) {
  [0, 1, 2, 3].forEach(i => {
    B(30, 25 + i * 80, 170, 55, 'Worker ' + (i + 1), sub, { st });
    A(200, 52 + i * 80, 548, 175, c, { n: i === 1 ? 1 : 0, at: 0.5 });
  });
}
const STEPS = [
  [[C.bad, '4 worker → Email company', 'Har worker ne apni taraf se 100 email / sec bheje'], [C.bad, 'Email company', 'Company ki had 100 thi, 400 aaye to 300 ko 429']],
  [[C.acc, 'Worker 1 → Redis bucket', 'Worker 1 ne bhejne se pehle Redis bucket se token maanga'], [C.good, 'Worker 1 → Email company', 'Token mila, to Worker 1 ne email bheja'], [C.acc, 'Worker 2 → Redis bucket', 'Worker 2 ne maanga, par is second ke token khatam'], [C.acc, 'Worker 2', 'Worker 2 line me ruka, agle second bhejega, fail nahi hua']],
  [[C.good, '4 worker → Email company', 'Bucket wala Redis gira, to har worker ne apni had 25 / sec rakhi; kul 100, 429 nahi']],
];
function frame(ph) {
  if (ph === 0) {
    workers('100 / sec', 'bad', C.bad);
    B(550, 135, 200, 80, 'Email company', 'had 100 / sec', { st: 'bad' });
    T(650, 250, 'aaye 400 / sec', { c: C.bad, b: 1, size: 16 });
    num(650, 285, 2, C.bad); L(650, 310, '300 ko 429', C.bad);
  } else if (ph === 1) {
    panel(0, 'token lo', ['Worker 1', 'email bhejna', ''], ['Redis bucket', '100 / sec, bache 1', 'acc'], 'token?', true, C.acc, 'pehle token');
    panel(1, 'mila, bhejo', ['Worker 1', 'token mila', 'good'], ['Email company', 'ek email', 'good'], 'bheja', true, C.good, 'had ke andar');
    panel(2, 'Worker 2', ['Worker 2', 'email bhejna', ''], ['Redis bucket', 'bache 0', 'bad'], 'token?', true, C.acc, 'is second ke khatam');
    panel(3, 'ruko', ['Worker 2', 'line me ruka', 'acc'], ['Redis bucket', 'agle second 100', 'good'], '', true, C.acc, 'fail nahi, bas intezaar');
    A(735, 158, 735, 110, C.muted, { dash: 1 }); T(750, 139, 'agle second', { c: C.muted, b: 1, size: 14, a: 'start' });
    A(735, 158, 735, 110, C.muted, { dash: 1 }); T(750, 139, 'agle second', { c: C.muted, b: 1, size: 14, a: 'start' });
    L(450, 320, 'worker kitne bhi hon, company tak kul 100 / sec. 429 nahi aata', C.good);
  } else {
    workers('apni had: 25 / sec', 'acc', C.good);
    B(550, 135, 200, 80, 'Email company', 'had 100 / sec', { st: 'good' });
    B(560, 20, 190, 50, 'Redis bucket', 'gira', { st: 'bad' }); cross(775, 45);
    T(650, 250, '4 x 25 = 100', { c: C.good, b: 1, size: 16 });
    L(650, 285, 'company ki had paar nahi', C.good);
    L(450, 350, 'thoda dheema, par 429 nahi. Redis lauta to phir saanjha bucket', C.good);
  }
}
