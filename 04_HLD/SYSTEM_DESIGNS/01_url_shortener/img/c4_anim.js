// Card 4: ginti DB me, redirect dheema. Ilaaj: 302 turant, khabar Kafka me. Agar: 301, Analytics dheema, do baar, Kafka band. Wapas: replay.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har click ki ginti seedha DB me, redirect likhai ka intezaar karta'],
  [3, GRN, 'ILAAJ', 'ILAAJ: redirect turant (302), khabar Kafka me, Analytics peeche gine'],
  [6, YEL, 'AGAR', 'AGAR 302 kyun, 301 kyun nahi? -> 301 pe browser yaad rakhta, ginti band'],
  [9, YEL, 'AGAR', 'AGAR Analytics dheema ho gaya? -> khabar Kafka me padi rehti'],
  [12, YEL, 'AGAR', 'AGAR wahi click do baar aaya? -> har click ki apni id, dobara to ginti nahi'],
  [15, YEL, 'AGAR', 'AGAR Kafka hi band ho gaya? -> redirect phir bhi chalta'],
  [18, GRN, 'WAPAS', 'WAPAS: naya report? Kafka me pade purane click dobara padh lo (replay)'],
];
function setup() {}
function ua(n1, n2, c2, lab) {
  B(20, 150, 100, 70, 'User', 'click'); A(120, 172, 198, 172, C.msg, { n: n1 });
  B(200, 150, 110, 70, 'App', '');
  A(198, 200, 122, 200, c2 || C.good, { n: n2 }); if (lab) L(160, 235, lab, c2 || C.good);
}
function kafka(st, sub) { B(430, 150, 120, 70, 'Kafka', sub, { st }); }
function an(st, sub) { B(690, 150, 140, 70, 'Analytics', sub, { st }); }
function frame(ph) {
  if (ph === 0) {
    ua(1, 3, C.bad, 'ruka, dheema');
    B(430, 150, 120, 70, 'DB', 'likhai', { st: 'bad' });
    A(310, 185, 428, 185, C.bad, { n: 2 }); L(370, 165, 'ginti likho', C.bad);
    T(160, 280, 'DB ka intezaar', { c: C.bad, b: 1, size: 15 });
  } else if (ph === 1) {
    ua(1, 2, C.good, '302 turant'); kafka('acc', 'khabar'); an('good', 'ginti');
    A(310, 185, 428, 185, C.acc, { n: 3 }); L(370, 165, 'khabar', C.acc);
    A(550, 185, 688, 185, C.acc, { n: 4 }); L(620, 165, 'apni speed se', C.acc);
  } else if (ph === 2) {
    ua(1, 2, C.acc, '301'); kafka('dim', ''); an('bad', 'ginti band');
    A(310, 185, 428, 185, C.line);
    B(200, 285, 110, 50, 'Asli site', '');
    AP([[70, 222], [70, 310], [198, 310]], C.bad, { n: 3, at: 0.3 });
    L(100, 345, 'agli baar: browser ne yaad rakha', C.bad, 'start');
    L(255, 120, 'server tak aaya hi nahi', C.bad);
    L(760, 280, 'hal: 302, har click server tak', C.good); L(760, 298, 'ginti aur expiry chalti', C.good);
  } else if (ph === 3) {
    ua(0, 0); kafka('acc', 'khabar padi'); an('acc', 'dheema');
    num(760, 125, 1, C.acc); L(780, 130, 'dheema', C.acc, 'start');
    A(310, 185, 428, 185, C.acc, { n: 2 }); L(370, 165, 'khabar', C.acc);
    cells(420, 240, [['c1', C.acc], ['c2', C.acc], ['c3', C.acc]], { w: 40, h: 24 });
    A(550, 185, 688, 185, C.acc, { n: 3, dash: 1 }); L(620, 165, 'offset tak padha', C.acc);
    L(620, 320, 'hal: apni speed se padhta, kahan tak padha yaad', C.good);
  } else if (ph === 4) {
    ua(0, 0); kafka('', ''); an('good', 'ginti: +1');
    A(310, 185, 428, 185, C.acc, { n: 1 }); L(370, 165, 'wahi click', C.acc);
    cells(420, 240, [['id X', C.good], ['id X', C.bad]], { w: 58, h: 24 });
    A(550, 185, 688, 185, C.good, { n: 2 });
    num(530, 290, 3, C.bad); L(550, 295, 'wahi id dobara: ginti nahi', C.bad, 'start');
    L(760, 120, 'hal: har click ki apni id', C.good);
  } else if (ph === 5) {
    ua(2, 3, C.good, '302 phir bhi'); kafka('bad', ''); cross(490, 245); an('dim', '');
    num(490, 125, 1, C.bad); L(510, 130, 'band', C.bad, 'start');
    A(310, 185, 428, 185, C.bad, { dash: 1, n: 4 }); L(370, 165, 'khabar gayi', C.bad);
    L(620, 290, 'kuch click ginti me nahi', C.bad);
    L(160, 300, 'hal: redirect chalta rehta', C.good);
  } else {
    ua(0, 0); kafka('good', 'purane click'); an('good', '');
    cells(420, 240, [['c1', C.good], ['c2', C.good], ['c3', C.good]], { w: 40, h: 24 });
    B(690, 270, 140, 60, 'Naya report', '', { st: 'acc' });
    AP([[550, 205], [620, 205], [620, 300], [688, 300]], C.acc, { n: 1, at: 0.5 });
    L(605, 340, 'dobara padho', C.acc);
    A(550, 175, 688, 175, C.line);
  }
}
