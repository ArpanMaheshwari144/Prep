// Card 7: 12 baje sale, ek hi line me OTP 50,000 offer ke peeche -> 10 min late, login fail.
// Ilaaj: high / beech / low alag line + apne worker. Agar: OTP line khaali, offer ne OTP ki company had khaayi.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 12 baje sale. Ek hi line me OTP 50,000 offer ke peeche'],
  [3, GRN, 'ILAAJ', 'ILAAJ: OTP ki apni tez line + apne worker. High OTP, beech order, low offer'],
  [6, YEL, 'AGAR', 'AGAR OTP wali tez line khaali? -> uske worker beech wali line padh lein'],
  [9, YEL, 'AGAR', 'AGAR offer wale OTP ki company had kha jaayein? -> OTP ka alag account'],
  [12, GRN, 'WAPAS', 'WAPAS: OTP kabhi kisi ke peeche nahi. Apni line, apne worker, apna account'],
];
const RY = [70, 190, 310], WN = ['OTP worker', 'Order worker', 'Offer worker'];
function setup() {}
function lanes(otp, ord, off, wst = ['good', '', '']) {
  T(140, RY[0] - 22, 'high line', { c: C.good, size: 12, b: 1, a: 'start' });
  T(140, RY[1] - 22, 'beech wali line', { c: C.muted, size: 12, a: 'start' });
  T(140, RY[2] - 22, 'low line', { c: C.muted, size: 12, a: 'start' });
  [otp, ord, off].forEach((k, i) => {
    const nm = ['OTP', 'order', 'offer'][i], c = [C.good, C.msg, C.acc][i];
    if (k) cells(140, RY[i] - 13, Array(k).fill([nm, c]), { w: 52 });
    A(140 + Math.max(k, 0) * 56, RY[i], 618, RY[i], C.line);
    B(620, RY[i] - 25, 110, 50, WN[i], '', { st: wst[i] });
  });
}
function frame(ph) {
  if (ph === 0) {
    B(770, 30, 110, 310, 'Users', '', { st: 'bad' });
    cells(140, 177, [['OTP', C.bad]].concat(Array(7).fill(['offer', C.acc])), { w: 52 });
    A(588, 190, 618, 190, C.line); B(620, 165, 110, 50, 'Worker', '', { st: 'acc' });
    num(130, 140, 1, C.acc); L(148, 145, '50,000 offer OTP se aage', C.acc, 'start');
    A(730, 190, 768, 190, C.acc, { n: 2, at: 0.5 }); L(675, 150, 'offer bhej raha', C.acc);
    num(160, 245, 3, C.bad); L(178, 250, 'OTP 10 min late -> login FAIL', C.bad, 'start');
  } else if (ph === 1) {
    B(770, 30, 110, 310, 'Users', 'OTP turant', { st: 'good' });
    lanes(1, 3, 8);
    A(730, 70, 768, 70, C.good, { n: 1, at: 0.5 });
    A(730, 190, 768, 190, C.msg, { n: 2, at: 0.5 });
    A(730, 310, 768, 310, C.acc, { n: 3, at: 0.5 }); L(530, 345, 'offer ruk sakta', C.acc);
    L(400, 105, 'OTP kisi ke peeche nahi', C.good);
  } else if (ph === 2) {
    B(770, 30, 110, 310, 'Users', '', { st: 'good' });
    lanes(0, 4, 8, ['good', '', '']);
    num(160, 70, 1, C.muted); L(178, 75, 'khaali', C.muted, 'start');
    AP([[364, 177], [364, 120], [600, 120], [640, 97]], C.good, { n: 2, at: 0.25 }); L(380, 112, 'OTP worker beech wali padhe', C.good, 'start');
    A(730, 70, 768, 70, C.good, { n: 3 }); A(730, 190, 768, 190, C.msg, { n: 3 });
    AP([[675, 285], [675, 245], [590, 245], [590, 85]], C.bad, { dash: 1, n: 4, at: 0.3 }); cross(590, 155);
    L(520, 268, 'ulta kabhi nahi', C.bad);
    L(450, 20, 'hal: OTP worker beech wali line padh lein. Ulta kabhi nahi', C.good);
  } else if (ph === 3) {
    lanes(1, 3, 8);
    B(770, 30, 110, 110, 'OTP account', 'apna', { st: 'good' });
    B(770, 230, 110, 110, 'Offer account', 'had khatam', { st: 'bad' });
    num(825, 222, 1, C.bad); L(808, 228, 'offer ne had khaayi', C.bad, 'end');
    A(730, 310, 768, 310, C.bad, { n: 2, dash: 1 });
    A(730, 70, 768, 70, C.good, { n: 3 }); L(750, 160, 'OTP gaya', C.good);
    L(400, 20, 'hal: OTP ka alag account / number. Had wala card 8', C.good);
  } else {
    lanes(1, 3, 8);
    B(770, 30, 110, 110, 'OTP account', 'apna', { st: 'good' });
    B(770, 230, 110, 110, 'Offer account', '', { st: 'acc' });
    A(730, 70, 768, 70, C.good, { n: 1 }); L(400, 105, 'apni line, apne worker', C.good);
    A(730, 190, 768, 250, C.msg, { n: 2 }); A(730, 310, 768, 310, C.acc, { n: 2 });
    L(820, 165, 'OTP turant', C.good);
  }
}
