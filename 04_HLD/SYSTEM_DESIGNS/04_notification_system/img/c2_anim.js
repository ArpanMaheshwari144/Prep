// Card 2: push / email / SMS ek hi line, worker SMS pe atka to sab atke. Ilaaj: har raaste ki alag line + worker.
// Agar: SMS ki line lambi. Wapas: har raasta apni line, apni speed.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: push, email, SMS ek hi line. Worker SMS pe atka, sab peeche'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har raaste ki alag line + alag worker. SMS dheema to sirf SMS ruka'],
  [6, YEL, 'AGAR', 'AGAR SMS ki line lambi? -> sirf SMS ke worker badhao'],
  [9, GRN, 'WAPAS', 'WAPAS: har raasta apni line me, apni speed se chalta hai'],
];
const ROW = [70, 190, 310], NM = ['Push', 'Email', 'SMS'];
function setup() {}
function ns() { B(20, 150, 120, 80, 'Notification', 'Svc'); }
function co(i, st, sub) { B(690, ROW[i] - 25, 120, 50, `${NM[i]} co.`, sub, { st }); }
// alag lines: k[i] = line me kitne, st = [push, email, sms] worker box st
function rows(k, wst, wsub = ['', '', '']) {
  ns();
  ROW.forEach((y, i) => {
    A(140, 190, 166, y, C.msg, { n: i === 1 ? 1 : 0, at: 0.5 });
    cells(170, y - 13, Array(k[i]).fill([NM[i], i === 2 ? C.bad : C.msg]), { w: 46 });
    A(170 + k[i] * 50, y, 478, y, C.line);
    B(480, y - 25, 110, 50, `${NM[i]} worker`, wsub[i], { st: wst[i] });
  });
}
function frame(ph) {
  if (ph === 0) {
    ns(); A(140, 190, 168, 190, C.line);
    cells(170, 177, [['Email', C.msg], ['Push', C.msg], ['Email', C.msg], ['Push', C.msg], ['SMS', C.bad]], { w: 48 });
    T(290, 165, 'ek hi line', { c: C.muted, size: 12 });
    A(430, 190, 478, 190, C.line);
    B(480, 165, 110, 50, 'Worker', '', { st: 'bad' });
    co(0, '', ''); co(1, '', ''); co(2, 'bad', 'dheemi');
    num(835, 300, 1, C.bad); L(835, 345, 'dheemi', C.bad);
    A(590, 200, 688, 305, C.bad, { n: 2, at: 0.45 }); L(600, 270, 'SMS pe atka', C.bad, 'end');
    A(590, 180, 688, 75, C.line, { dash: 1 }); A(590, 190, 688, 190, C.line, { dash: 1 });
    num(205, 230, 3, C.bad); L(222, 235, 'push + email peeche atke', C.bad, 'start');
    T(840, 75, '0', { c: C.bad, b: 1, size: 15 }); T(840, 195, '0', { c: C.bad, b: 1, size: 15 });
  } else if (ph === 1) {
    rows([2, 2, 3], ['good', 'good', 'bad']);
    L(80, 130, '3 chhote message', C.msg);
    A(590, 70, 688, 70, C.good, { n: 2 }); co(0, 'good', 'pahuncha');
    A(590, 190, 688, 190, C.good, { n: 2 }); co(1, 'good', 'pahuncha');
    A(590, 310, 688, 310, C.bad, { n: 3, dash: 1 }); co(2, 'bad', 'dheemi');
    L(640, 345, 'sirf SMS ruka', C.bad);
  } else if (ph === 2) {
    rows([2, 2, 6], ['good', 'good', 'acc'], ['', '', 'x3']);
    num(155, 345, 2, C.bad); L(172, 350, 'SMS line lambi', C.bad, 'start');
    A(590, 70, 688, 70, C.good, { n: 4 }); co(0, 'good', 'asar nahi');
    A(590, 190, 688, 190, C.good, { n: 4 }); co(1, 'good', 'asar nahi');
    num(535, 260, 3, C.acc); L(553, 265, 'worker badhe', C.acc, 'start');
    A(590, 300, 688, 300, C.acc, { n: 5, at: 0.4 }); A(590, 318, 688, 318, C.acc);
    co(2, 'acc', 'dheemi');
    L(560, 25, 'hal: sirf SMS ke worker badhao, push / email pe asar nahi', C.good);
  } else {
    rows([1, 1, 2], ['good', 'good', 'good']);
    A(590, 70, 688, 70, C.good, { n: 2 }); co(0, 'good', 'apni speed');
    A(590, 190, 688, 190, C.good, { n: 2 }); co(1, 'good', 'apni speed');
    A(590, 310, 688, 310, C.acc, { n: 2 }); co(2, 'acc', 'dheere');
    L(640, 345, 'SMS dheere, par chal raha', C.acc);
  }
}
