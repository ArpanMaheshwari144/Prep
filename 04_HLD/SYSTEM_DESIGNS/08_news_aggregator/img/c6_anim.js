// Card 6: ek hi feed:latest, sab category mili; mujhe sirf tech + sports chahiye.
// Ilaaj: har category ki list pehle se, padhte waqt milao. Agar: ek khabar 2 list me -> id se dedupe. Wapas.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek hi feed:latest, sab category. Mujhe sirf tech + sports chahiye'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har category ki list pehle se bani. Khola to tech + sports milao'],
  [6, YEL, 'AGAR', 'AGAR khabar 2 category list me? -> dono me ZADD, milate waqt id se dedupe'],
  [9, GRN, 'WAPAS', 'WAPAS: sirf ~20 list, 10 lakh nahi. Pasand bas batati kaunsi list milaani'],
];
const LY = [20, 110, 200];
function setup() {}
function ends(fst, ust) {
  B(20, 140, 110, 70, 'Worker', 'nayi khabar');
  B(580, 140, 120, 70, 'Feed Svc', 'milao', { st: fst });
  B(770, 140, 110, 70, 'User', 'tech + sports', { st: ust });
}
// category list: naam + andar ki khabar
function lists(sts, items) {
  ['feed:tech', 'feed:sports', 'feed:politics'].forEach((nm, i) => {
    B(300, LY[i], 190, 66, nm, '', { st: sts[i] });
    if (items && items[i]) cells(312, LY[i] + 32, items[i], { w: 40, h: 24 });
  });
}
function frame(ph) {
  if (ph === 0) {
    ends('bad', 'bad');
    B(300, 110, 190, 66, 'feed:latest', 'sab category', { st: 'bad' });
    A(130, 175, 298, 143, C.msg, { n: 1 }); L(210, 140, 'ZADD', C.msg);
    A(490, 143, 578, 175, C.bad, { n: 2 }); A(700, 175, 768, 175, C.bad, { n: 3 });
    cells(500, 270, [['tech', C.good], ['politics', C.bad], ['sports', C.good], ['politics', C.bad]], { w: 86 });
    L(680, 258, 'sab category mili', C.bad);
  } else if (ph === 1) {
    ends('good', 'good'); lists(['good', 'good', 'dim']);
    A(130, 165, 298, 53, C.acc, { n: 1 }); A(130, 185, 298, 143, C.acc, { n: 1 });
    L(170, 80, 'apni list me', C.acc);
    A(490, 53, 578, 160, C.good, { n: 2 }); A(490, 143, 578, 180, C.good, { n: 2 });
    A(700, 175, 768, 175, C.good, { n: 3 }); L(735, 240, 'top 20, time se', C.good);
    cells(500, 290, [['tech', C.good], ['sports', C.good], ['tech', C.good]], { w: 86 });
  } else if (ph === 2) {
    ends('acc', 'good');
    lists(['good', 'good', 'dim'], [[['k7', C.acc], ['k1', C.good]], [['k7', C.acc], ['k2', C.good]]]);
    A(130, 165, 298, 53, C.acc, { n: 1 }); A(130, 185, 298, 143, C.acc, { n: 1 });
    L(170, 80, 'k7 dono me', C.acc);
    A(490, 53, 578, 160, C.good, { n: 2 }); A(490, 143, 578, 180, C.good, { n: 2 });
    num(600, 235, 3, C.acc); L(618, 240, 'id se dedupe', C.acc, 'start');
    A(700, 175, 768, 175, C.good, { n: 4 });
    cells(600, 290, [['k7', C.good], ['k1', C.good], ['k2', C.good]], { w: 40 });
    L(780, 307, 'k7 ek baar', C.good, 'start');
    L(20, 330, 'hal: milate waqt id se dedupe', C.good, 'start');
  } else {
    ends('good', 'good'); lists(['good', 'good', 'dim']);
    A(490, 53, 578, 160, C.good, { n: 1 }); A(490, 143, 578, 180, C.good, { n: 1 });
    A(700, 175, 768, 175, C.good, { n: 2 });
    T(395, 300, '.. sirf ~20 list', { c: C.good, b: 1, size: 14 });
    T(395, 325, '10 lakh nahi', { c: C.muted, size: 13 });
    L(735, 240, 'pasand = kaunsi list', C.good);
  }
}
