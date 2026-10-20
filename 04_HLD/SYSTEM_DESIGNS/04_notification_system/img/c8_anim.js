// Card 8: sale me laakhon message, har worker apni speed se, company ne 429 diya. Ilaaj: saanjha token bucket (Redis),
// kul 100 / sec. Agar: bucket wala Redis gira -> har worker apni surakshit had (100 / 4 = 25).
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: company ki had 100 / sec. Har worker apni taraf se 100 / sec, 4 worker = 400. 429'],
  [3, GRN, 'ILAAJ', 'ILAAJ: sab worker ek saanjha token bucket (Redis) se token lete. Kul 100 / sec. Pal-pal'],
  [6, YEL, 'AGAR', 'AGAR bucket wala Redis hi gira? -> har worker apni surakshit had (100 / 4 = 25)'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 30, title, { c: C.fg, b: 1, size: 13 });
  B(x + 40, 50, 125, 48, top[0], top[1], { st: top[2] });
  B(x + 40, 165, 125, 58, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 70, 100, x + 70, 163, c); else A(x + 70, 163, x + 70, 100, c); } // khaali lab = teer nahi
  T(x + 140, 135, lab, { c, b: 1, size: 12 });
  T(x + 102, 252, note, { c: C.muted, size: 11 });
}
function workers(sub, st, c) {
  [0, 1, 2, 3].forEach(i => {
    B(30, 25 + i * 80, 170, 55, 'Worker ' + (i + 1), sub, { st });
    A(200, 52 + i * 80, 548, 175, c, { n: i === 1 ? 1 : 0, at: 0.5 });
  });
}
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
