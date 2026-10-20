// Card 1: Upload Svc Validator ka 3 sec intezaar kare. Ilaaj: turant T-101, Kafka -> worker -> Validator -> DB.
// Agar: worker mara, khabar dobara, Validator 1 ghanta band, circuit breaker. Wapas: Validator aaya.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Upload Svc Validator ka jawab ruke, screen 3 sec ghoomti'],
  [3, GRN, 'ILAAJ', 'ILAAJ: turant T-101 (VALIDATING). Kafka -> worker -> Validator -> DONE'],
  [6, YEL, 'AGAR', 'AGAR worker beech me mar gaya? -> khabar dobara milti hai'],
  [9, YEL, 'AGAR', 'AGAR wahi khabar dobara aa gayi? -> pehle haal dekho, DONE to skip'],
  [12, YEL, 'AGAR', 'AGAR Validator 1 ghanta band? -> 4,000 khabar line me padi rehti'],
  [15, YEL, 'AGAR', 'AGAR band hai, call karte rahoge? -> nahi, circuit breaker lagao'],
  [18, GRN, 'WAPAS', 'WAPAS: Validator wapas aaya -> line ki khabar jaanchi, DB me DONE'],
];
function setup() {}
// o = box st, s = box sub
function base(o = {}, s = {}) {
  B(20, 150, 100, 70, 'User', s.user || 'report.pdf', { st: o.user });
  B(160, 150, 130, 70, 'Upload Svc', s.up || '', { st: o.up });
  if (!o.noQ) {
    B(330, 150, 110, 70, 'Kafka', s.kf || 'line', { st: o.kf });
    B(480, 150, 120, 70, 'Worker', s.wk || '', { st: o.wk });
    B(480, 275, 120, 60, 'DB', s.db || 'haal', { st: o.db });
  }
  B(680, 150, 150, 70, 'Validator', s.val || 'jaanch 3 sec', { st: o.val });
}
function frame(ph) {
  if (ph === 0) {
    base({ noQ: 1, user: 'bad', up: 'bad' }, { user: 'ghoom raha', up: 'thread atka' });
    A(120, 172, 158, 172, C.msg, { n: 1, at: 0.45 }); L(70, 135, 'report.pdf daali', C.msg);
    A(290, 172, 678, 172, C.msg, { n: 2 }); L(485, 160, 'jaancho, jawab ka intezaar', C.msg);
    A(678, 200, 292, 200, C.acc, { n: 3 }); L(485, 225, '3 sec baad jawab', C.acc);
    L(20, 250, 'screen 3 sec ghoomti', C.bad, 'start');
    num(160, 290, 4, C.bad); L(178, 295, '31 March: 200 / sec, har thread 3 sec atka', C.bad, 'start');
    L(178, 320, 'threads khatam', C.bad, 'start');
  } else if (ph === 1) {
    base({ user: 'good', up: 'good', kf: 'acc', wk: 'acc', db: 'good', val: 'good' },
      { user: 'T-101', db: 'T-101: DONE' });
    A(120, 172, 158, 172, C.msg, { n: 1, at: 0.45 });
    A(158, 200, 122, 200, C.good, { n: 2, at: 0.5 }); L(70, 135, 'turant T-101', C.good);
    L(70, 250, 'VALIDATING', C.good);
    A(290, 185, 328, 185, C.acc, { n: 2, at: 0.5 }); L(310, 135, 'khabar', C.acc);
    A(440, 185, 478, 185, C.msg, { n: 3 }); L(460, 240, 'baad me', C.msg);
    A(600, 185, 678, 185, C.msg, { n: 4 }); L(640, 135, 'jaancho', C.msg);
    A(540, 222, 540, 273, C.good, { n: 5 }); L(612, 260, 'DONE likha', C.good, 'start');
  } else if (ph === 2) {
    base({ kf: 'acc', wk: 'bad', db: '' }, { kf: 'khabar bachi', wk: 'mar gaya', db: 'abhi likha nahi' });
    A(290, 185, 328, 185, C.line);
    num(540, 120, 1, C.bad); L(558, 125, 'beech me mara', C.bad, 'start'); cross(625, 165);
    A(540, 222, 540, 273, C.bad, { dash: 1, n: 2 }); L(612, 250, 'DONE nahi likha', C.bad, 'start');
    L(385, 255, 'offset commit nahi', C.acc);
    A(440, 200, 478, 200, C.good, { n: 3, dash: 1 }); L(385, 120, 'khabar dobara milti', C.good);
    L(450, 30, 'hal: offset commit DB me likhne ke baad hi hota hai', C.good);
  } else if (ph === 3) {
    base({ kf: 'acc', wk: 'acc', db: 'good', val: 'dim' }, { kf: 'T-101 dobara', db: 'T-101: DONE' });
    A(290, 185, 328, 185, C.line);
    A(440, 185, 478, 185, C.msg, { n: 1 }); L(400, 120, 'wahi khabar dobara', C.msg);
    A(525, 222, 525, 273, C.msg, { n: 2 }); L(470, 255, 'haal?', C.msg, 'end');
    A(560, 273, 560, 222, C.good, { n: 3 }); L(612, 250, 'DONE hai', C.good, 'start');
    A(600, 185, 678, 185, C.bad, { dash: 1 }); cross(640, 185); num(640, 140, 4, C.bad);
    L(658, 120, 'Validator call nahi', C.bad, 'start');
    L(450, 30, 'hal: DONE hai to skip kar do (idempotent)', C.good);
  } else if (ph === 4) {
    base({ user: 'good', up: 'good', kf: 'acc', wk: 'acc', val: 'bad' }, { kf: '4,000 khabar', val: 'band 1 ghanta' });
    cross(855, 185); num(755, 125, 1, C.bad); L(773, 130, 'band', C.bad, 'start');
    A(600, 185, 678, 185, C.bad, { dash: 1, n: 2 }); L(640, 165, 'fail', C.bad);
    A(120, 185, 158, 185, C.good); A(290, 185, 328, 185, C.acc, { n: 3, at: 0.5 });
    L(225, 135, 'upload chalu', C.good);
    dots(10, 336, 245, C.acc, { gap: 20, cols: 5 }); num(318, 300, 4, C.acc);
    L(336, 310, '4,000 line me padi', C.acc, 'start');
    L(450, 30, 'hal: DONE likhe bina offset aage nahi badhta, khabar nahi khoti', C.good);
  } else if (ph === 5) {
    base({ kf: 'acc', wk: 'acc', val: 'bad' }, { val: 'band' });
    B(680, 275, 150, 60, 'Validator 2', 'doosri company', { st: 'good' });
    cross(855, 185); num(755, 125, 1, C.bad); L(773, 130, 'band', C.bad, 'start');
    A(440, 185, 478, 185, C.line);
    dots(3, 612, 140, C.bad, { gap: 20, hollow: 1 }); num(600, 112, 2, C.bad); L(618, 117, '3-5 fail', C.bad, 'start');
    A(600, 175, 678, 175, C.bad, { dash: 1 }); cross(622, 175); num(656, 175, 3, C.bad);
    L(640, 205, 'call band', C.bad);
    A(600, 215, 678, 290, C.good, { n: 4, at: 0.55 });
    L(450, 30, 'hal: circuit breaker, thodi der baad ek test call', C.good);
    L(450, 52, 'aur doosri company rakho', C.good);
  } else {
    base({ user: 'good', up: 'good', kf: 'acc', wk: 'good', db: 'good', val: 'good' },
      { kf: '4,000 khabar', val: 'wapas', db: 'DONE' });
    num(755, 125, 1, C.good); L(773, 130, 'wapas aaya', C.good, 'start');
    A(440, 185, 478, 185, C.msg, { n: 2 }); L(400, 120, 'line ki khabar', C.msg);
    A(600, 185, 678, 185, C.good, { n: 3 }); L(640, 165, 'jaancho', C.good);
    A(540, 222, 540, 273, C.good, { n: 4 }); L(612, 260, 'DONE likha', C.good, 'start');
  }
}
