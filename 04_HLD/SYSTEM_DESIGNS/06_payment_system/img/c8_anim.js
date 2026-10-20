// Card 8: Diwali 3000 / sec, ek machine, CPU bhara, timeout, dobara tap. Ilaaj: LB + kai stateless machine, yaad DB me.
// Agar: ek machine mari (GET /health), galat istemaal (403), JWT chori (~15 min, PIN / OTP). Wapas: koi bhi machine.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 3000 payment / sec, ek hi machine. CPU bhara, timeout, dobara tap'],
  [3, GRN, 'ILAAJ', 'ILAAJ: kai machine + LB. Machine kuch yaad nahi rakhti, yaad DB me'],
  [6, YEL, 'AGAR', 'AGAR peeche ek machine mar gayi? -> LB GET /health, jawab nahi = bahar'],
  [9, YEL, 'AGAR', 'AGAR galat istemaal? -> "from" ka maalik JWT wala? Nahi to 403'],
  [12, YEL, 'AGAR', 'AGAR JWT chori? -> chhoti umar (~15 min) + refresh, bade pe PIN / OTP'],
  [15, GRN, 'WAPAS', 'WAPAS: koi bhi machine koi bhi request le, koi bhi badlo ya badhao'],
];
function setup() {}
const SY = [25, 145, 265];
// o: { us, s: [st,st,st], db } ; sub: { us, s: [..] }
function base(o = {}, sub = {}) {
  B(20, 150, 110, 70, o.uname || 'Users', sub.us || '3000 / sec', { st: o.us });
  B(190, 150, 80, 70, 'LB', '', { st: o.lb });
  SY.forEach((y, i) => B(380, y, 150, 60, `Svc ${i + 1}`, (sub.s || [])[i] || 'stateless', { st: (o.s || [])[i] }));
  B(680, 150, 190, 70, 'DB + Idempotency', 'yaad yahan', { st: o.db });
}
function lb(c, n, skip) {
  SY.forEach((y, i) => { if (i !== skip) A(270, 185, 378, y + 30, c, { n, at: 0.6 }); });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 110, 70, 'Users', '3000 / sec', { st: 'bad' });
    B(330, 150, 150, 70, 'Payment Svc', 'ek machine', { st: 'bad' });
    B(680, 150, 190, 70, 'DB + Idempotency', '');
    A(130, 170, 328, 170, C.msg, { n: 1 }); L(230, 155, 'Diwali: 3000 / sec', C.msg);
    bar(330, 245, 150, 16, 1, ''); num(310, 253, 2, C.bad); L(405, 285, 'CPU bhara, payment atke', C.bad);
    A(328, 200, 132, 200, C.bad, { dash: 1, n: 3 }); L(230, 225, 'timeout', C.bad);
    num(75, 250, 4, C.bad); L(75, 275, 'log dobara tap', C.bad); L(75, 295, '-> aur bojh', C.bad);
  } else if (ph === 1) {
    base({ us: 'good', lb: 'good', s: ['good', 'good', 'good'], db: 'good' });
    A(130, 185, 188, 185, C.msg, { n: 1 }); lb(C.msg, 2);
    SY.forEach((y, i) => A(530, y + 30, 678, 175 + (i - 1) * 15, C.good, { n: 3, at: 0.5 }));
    L(230, 135, 'koi bhi machine', C.msg);
    L(775, 245, 'yaad sirf yahan', C.good);
    L(455, 345, 'machine kuch yaad nahi rakhti (stateless)', C.good);
  } else if (ph === 2) {
    base({ lb: 'good', s: ['good', 'bad', 'good'] }, { s: ['', 'mar gayi', ''] });
    num(560, 165, 1, C.bad); cross(560, 200); L(575, 170, 'mar gayi', C.bad, 'start');
    A(270, 185, 378, 175, C.bad, { dash: 1, n: 2, at: 0.5 }); L(455, 238, 'GET /health: jawab nahi', C.bad);
    A(270, 170, 378, 55, C.good, { n: 3, at: 0.6 }); A(270, 200, 378, 295, C.good, { n: 3, at: 0.6 });
    L(455, 345, 'hal: jawab nahi = bahar, baaki machine pe', C.good);
  } else if (ph === 3) {
    base({ uname: 'Koi user', us: 'bad', s: ['', 'good', ''] }, { us: 'galat from' });
    A(130, 185, 188, 185, C.bad, { n: 1 }); A(270, 185, 378, 175, C.bad, { n: 1, at: 0.5 });
    num(545, 236, 2, C.acc); L(563, 241, 'JWT wala = maalik? nahi', C.acc, 'start');
    AP([[400, 207], [400, 250], [75, 250], [75, 222]], C.good, { n: 3, at: 0.5 }); L(240, 275, '403', C.good);
    L(455, 345, 'hal: owner check, body se nahi. Aage WAF + had', C.good);
  } else if (ph === 4) {
    base({ uname: 'Chor', us: 'bad', s: ['', 'good', ''] }, { us: 'chori ka JWT' });
    num(75, 130, 1, C.bad); L(93, 135, 'JWT chori', C.bad, 'start');
    A(130, 185, 188, 185, C.bad, { n: 2 }); A(270, 185, 378, 175, C.bad, { n: 2, at: 0.5 });
    num(545, 236, 3, C.good); L(563, 241, '~15 min me bekaar', C.good, 'start');
    L(563, 263, 'bada payment: PIN / OTP', C.good, 'start');
    AP([[400, 207], [400, 250], [75, 250], [75, 222]], C.good, { n: 4, at: 0.5 }); L(240, 275, 'mana', C.good);
    L(455, 345, 'hal: chhoti umar (~15 min) + refresh. Bade payment pe PIN / OTP', C.good);
  } else {
    base({ us: 'good', lb: 'good', s: ['good', 'good', 'good'], db: 'good' }, { s: ['', 'wapas / nayi', ''] });
    A(130, 185, 188, 185, C.msg, { n: 1 }); lb(C.good, 2);
    SY.forEach((y, i) => A(530, y + 30, 678, 175 + (i - 1) * 15, C.good, { n: 3, at: 0.5 }));
    L(775, 245, 'yaad yahan', C.good);
    L(455, 345, 'koi bhi machine badlo ya badhao, kuch nahi khota', C.good);
  }
}
