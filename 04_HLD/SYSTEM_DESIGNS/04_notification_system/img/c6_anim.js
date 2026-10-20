// Card 6: Twilio dheemi, har call 30 sec, saare SMS worker phanse, OTP ruka.
// Ilaaj: 2-3 sec timeout + circuit breaker -> OTP SNS se. Agar: push / email pe asar, SNS bhi band. Wapas: test call.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Twilio dheemi. Har call 30 sec, saare SMS worker phanse, OTP ruka'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 2-3 sec me chhodo. Baar-baar fail -> breaker khula, OTP SNS se'],
  [6, YEL, 'AGAR', 'AGAR Twilio ka breaker khula, push / email pe asar? -> nahi'],
  [9, YEL, 'AGAR', 'AGAR SNS bhi band ho gaya? -> khabar line me rukti, phenko nahi'],
  [12, GRN, 'WAPAS', 'WAPAS: thodi der baad Twilio ko test call. Theek hai to wapas'],
];
const WY = [80, 190, 300];
function setup() {}
function workers(st) { WY.forEach((y, i) => { A(166, 190, 208, y, C.line); B(210, y - 22, 90, 44, `W${i + 1}`, '', { st }); }); }
function line(items) { cells(20, 177, items, { w: 46 }); }
function tw(st, sub) { B(680, 40, 180, 70, 'Twilio', sub, { st }); }
function sns(st, sub) { B(680, 270, 180, 70, 'SNS', sub, { st }); }
function cb(st, sub) { B(400, 155, 120, 70, 'Circuit', sub || 'breaker', { st }); }
function frame(ph) {
  if (ph === 0) {
    line([['SMS', C.msg], ['SMS', C.msg], ['OTP', C.bad]]); workers('bad'); tw('bad', 'dheemi'); sns('', 'doosri company');
    num(665, 25, 1, C.bad); L(770, 130, 'har call 30 sec', C.bad);
    WY.forEach((y, i) => A(300, y, 678, 60 + i * 12, C.bad, { n: i === 1 ? 2 : 0, at: 0.5 }));
    WY.forEach(y => L(255, y + 38, 'phansa', C.bad));
    num(30, 155, 3, C.bad); L(48, 160, 'OTP line me ruka', C.bad, 'start');
  } else if (ph === 1) {
    line([['SMS', C.msg], ['SMS', C.msg]]); workers('good'); tw('bad', 'dheemi'); sns('good', 'doosri company');
    WY.forEach(y => A(300, y, 398, 190, C.msg));
    cb('bad', 'KHULA');
    A(520, 170, 678, 80, C.bad, { n: 1, dash: 1, at: 0.4 }); L(600, 150, '2 sec me chhoda', C.bad, 'start');
    cross(640, 101); num(440, 245, 2, C.bad); L(458, 250, 'baar-baar fail', C.bad, 'start');
    A(520, 210, 678, 300, C.good, { n: 3, at: 0.4 }); L(600, 300, 'OTP SNS se', C.good, 'end');
  } else if (ph === 2) {
    const R = [['SMS worker', 'KHULA', 'bad', 'Twilio', 'bad'], ['Push worker', 'theek', 'good', 'FCM', 'good'],
      ['Email worker', 'theek', 'good', 'Email co.', 'good']];
    R.forEach(([w, s, st, co, cst], i) => {
      const y = [75, 190, 305][i];
      B(170, y - 25, 130, 50, w, '', { st: i ? 'good' : '' });
      B(400, y - 30, 120, 60, 'breaker', s, { st });
      B(680, y - 30, 180, 60, co, '', { st: cst });
      A(300, y, 398, y, i ? C.good : C.msg, { n: i ? 2 : 0 });
      A(520, y, 678, y, i ? C.good : C.bad, { n: i ? 3 : 0, dash: !i });
    });
    num(400, 32, 1, C.bad); cross(600, 75); L(600, 50, 'call band', C.bad);
    L(600, 175, 'chal raha', C.good); L(600, 290, 'chal raha', C.good);
    L(20, 140, 'hal: har company ka apna breaker,', C.good, 'start');
    L(20, 160, 'worker alag line pe', C.good, 'start');
  } else if (ph === 3) {
    line([['SMS', C.msg], ['SMS', C.msg], ['OTP', C.acc]]); workers(''); tw('bad', 'band'); sns('bad', 'band');
    cb('bad', 'KHULA');
    WY.forEach(y => A(300, y, 398, 190, C.line));
    num(665, 25, 1, C.bad); cross(875, 125);
    A(520, 210, 678, 300, C.bad, { n: 2, dash: 1, at: 0.4 }); cross(875, 255);
    AP([[460, 227], [460, 245], [90, 245], [90, 206]], C.acc, { n: 3, at: 0.3 }); L(380, 268, 'line me ruki, backoff se phir', C.acc);
    num(30, 140, 4, C.bad); L(48, 145, 'OTP ka alert', C.bad, 'start');
    L(20, 20, 'hal: khabar line me rukti, backoff se koshish, phenko nahi', C.good, 'start');
  } else {
    line([['SMS', C.msg], ['SMS', C.msg]]); workers('good'); tw('good', 'theek'); sns('good', 'doosri company');
    cb('good', 'wapas theek');
    A(520, 165, 678, 62, C.acc, { n: 1, dash: 1, at: 0.4 }); L(560, 95, 'test call', C.acc, 'end');
    A(678, 88, 522, 185, C.good, { n: 2, at: 0.5 }); L(640, 150, 'theek', C.good, 'start');
    WY.forEach(y => A(300, y, 398, 190, C.good, { n: y === 300 ? 3 : 0, at: 0.4 }));
    L(775, 130, 'SMS phir Twilio se', C.good);
  }
}
