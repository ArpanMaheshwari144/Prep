// Card 3: 500 kata, network gaya, app ne dobara bheja -> 500 phir kata. Ilaaj: idempotency key B-42.
// Agar: double tap, paisa kata par seat fail (refund), webhook der se (PENDING). Wapas: ek hi baar kata.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 500 kata, network gaya. App ne dobara bheja, 500 phir kata'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har pay usi bookingId (B-42) se. Dobara aaya = "ho chuka"'],
  [6, YEL, 'AGAR', 'AGAR user ne Pay double tap kar diya? -> wahi bookingId, ek hi baar kata'],
  [9, YEL, 'AGAR', 'AGAR paisa kat gaya par seat fail? -> refund karo aur user ko batao'],
  [12, YEL, 'AGAR', 'AGAR PSP ka webhook der se aaya? -> PAYMENT_PENDING, hold thoda badhao'],
  [15, GRN, 'WAPAS', 'WAPAS: user ko kuch dobara nahi karna. B-42 done, paisa ek hi baar'],
];
function setup() {}
function core(appSub, appSt, bankSub, bankSt) {
  B(20, 150, 130, 70, 'Main ka App', appSub, { st: appSt });
  B(340, 150, 150, 70, 'Payment Svc'); B(650, 150, 190, 70, 'Bank (PSP)', bankSub, { st: bankSt });
}
function key(sub, st) { B(340, 25, 150, 60, 'bookingId', sub, { st: st || 'acc' }); }
function db(sub, st) { B(650, 280, 190, 56, 'SQL DB', sub, { st }); }
function frame(ph) {
  if (ph === 0) {
    core('Pay 500', '', 'mera khata', 'bad');
    A(150, 162, 338, 162, C.msg, { n: 1 }); L(245, 148, 'Pay 500', C.msg);
    A(490, 165, 648, 165, C.msg, { n: 2, at: 0.3 }); L(600, 145, 'kaato 500', C.msg);
    A(338, 188, 152, 188, C.good, { n: 3, at: 0.2 }); cross(220, 188); L(220, 238, 'network gaya', C.bad);
    AP([[85, 222], [85, 262], [415, 262], [415, 224]], C.msg, { n: 4, at: 0.3 }); L(250, 290, 'Pay 500 (naya)', C.msg);
    A(490, 205, 648, 205, C.bad, { n: 5, at: 0.3 }); L(600, 235, 'kaato 500 phir', C.bad);
    T(745, 262, 'kata: 1000', { c: C.bad, b: 1, size: 18 });
  } else if (ph === 1) {
    core('ho chuka', 'good', 'mera khata', 'good'); key('B-42: done', 'good');
    A(150, 162, 338, 162, C.msg, { n: 1 }); L(245, 148, 'Pay B-42', C.msg);
    A(490, 165, 648, 165, C.msg, { n: 2, at: 0.3 }); L(600, 145, 'kaato 500', C.msg);
    A(338, 188, 152, 188, C.good, { n: 3, at: 0.2 }); cross(220, 188); L(220, 238, 'network gaya', C.bad);
    AP([[85, 222], [85, 262], [415, 262], [415, 224]], C.msg, { n: 4, at: 0.3 }); L(250, 290, 'Pay B-42 (dobara)', C.msg);
    A(415, 148, 415, 88, C.good, { n: 5 }); L(500, 118, 'pehle se: ho chuka', C.good, 'start');
    T(745, 262, 'kata: 500', { c: C.good, b: 1, size: 18 });
  } else if (ph === 2) {
    core('Pay x2', 'acc', 'mera khata', 'good'); key('ek hi B-42');
    A(150, 162, 338, 162, C.msg, { n: 1 }); A(150, 195, 338, 195, C.msg, { n: 1 });
    L(245, 148, 'Pay B-42', C.msg); L(245, 222, 'Pay B-42 (tap 2)', C.msg);
    A(415, 148, 415, 88, C.acc, { n: 2 }); L(430, 118, 'wahi id', C.acc, 'start');
    A(490, 175, 648, 175, C.good, { n: 3, at: 0.3 }); L(600, 155, 'kaato 500', C.good);
    T(745, 262, 'kata: 500', { c: C.good, b: 1, size: 18 });
    L(250, 330, 'hal: doosra tap = "ho chuka", ek hi baar katta', C.good);
  } else if (ph === 3) {
    core('batao: refund', 'good', 'refund: 500', 'good'); db('seat fail', 'bad');
    A(490, 165, 648, 165, C.msg, { n: 1, at: 0.3 }); L(600, 145, 'kaato 500', C.msg);
    A(490, 210, 648, 290, C.bad, { n: 2, at: 0.4 }); cross(870, 308); L(560, 290, 'seat fail', C.bad);
    A(492, 200, 648, 200, C.good, { n: 3, at: 0.3 }); L(600, 228, 'refund karo (PSP ko)', C.good);
    A(338, 188, 152, 188, C.good, { n: 4 }); L(245, 175, 'user ko batao', C.good);
    L(250, 300, 'hal: saga jaisa, ek fail', C.good); L(250, 318, 'to doosra ulta karo', C.good);
  } else if (ph === 4) {
    core('PENDING dikhta', 'acc', 'kata 500', ''); key('PAYMENT_PENDING'); db('hold badha', 'acc');
    A(648, 175, 492, 175, C.acc, { dash: 1, n: 1, at: 0.3 }); L(570, 160, 'webhook der se', C.acc);
    A(415, 148, 415, 88, C.acc, { n: 2 }); L(430, 118, 'PENDING rakho', C.acc, 'start');
    A(490, 210, 648, 290, C.good, { n: 3, at: 0.4 }); L(560, 290, 'hold badhao', C.good);
    L(245, 300, 'hal: roz hisaab milao', C.good); L(245, 318, '(reconciliation)', C.good);
  } else {
    core('ticket', 'good', 'kata: 500', 'good'); key('B-42: done', 'good');
    A(150, 162, 338, 162, C.msg, { n: 1 }); L(245, 148, 'Pay B-42', C.msg);
    A(490, 175, 648, 175, C.good, { n: 2, at: 0.3 }); L(600, 155, 'ek baar', C.good);
    A(338, 195, 152, 195, C.good, { n: 3 }); L(245, 222, 'ho gaya', C.good);
  }
}
