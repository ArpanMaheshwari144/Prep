// Card 1: ek tap, jawab nahi aaya, app ne dobara bheja, do order. Ilaaj: wahi id + UNIQUE. Agar: pehli abhi chal rahi. Wapas.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek baar dabaya, network gaya, app ne dobara bheja, order do'],
  [3, GRN, 'ILAAJ', 'ILAAJ: app ek id banata, dobara bhi wahi id. Server UNIQUE: purana jawab'],
  [6, YEL, 'AGAR', 'AGAR wahi id dobara, pehli abhi chal rahi? -> thoda ruko (IN_PROGRESS, 409)'],
  [9, GRN, 'WAPAS', 'WAPAS: pehli ho gayi, wahi id phir aayi -> saved jawab, order ek hi'],
];
function setup() {}
function base(appSub, dbSt, dbSub) {
  B(20, 90, 130, 240, 'App', appSub);
  B(360, 90, 150, 240, 'Order Svc', '');
  B(700, 90, 160, 240, 'Orders DB', dbSub, { st: dbSt });
}
// App <-> Order Svc teer: y pe, label upar (y - 12)
function lr(y, c, n, s, o = {}) { A(152, y, 358, y, c, { n, at: 0.15, dash: o.dash }); L(270, y - 12, s, c); }
function rl(y, c, n, s, o = {}) { A(358, y, 152, y, c, { n, at: 0.15, dash: o.dash }); L(275, y - 12, s, c); }
function od(y, c, n, s, o = {}) { A(512, y, 698, y, c, { n, at: 0.15, dash: o.dash }); L(615, y - 12, s, c); }
function frame(ph) {
  if (ph === 0) {
    base('', 'bad', '');
    lr(130, C.msg, 1, 'BUY TCS');
    od(145, C.good, 2, 'order #1 save');
    rl(200, C.bad, 3, 'jawab nahi aaya', { dash: 1 }); cross(172, 200);
    lr(270, C.bad, 4, 'dobara bheja');
    od(285, C.bad, 5, 'order #2 save');
    cells(722, 160, [['#1', C.bad], ['#2', C.bad]], { w: 54 });
    L(780, 230, 'ek tap, do order', C.bad);
  } else if (ph === 1) {
    base('id: a7', 'good', '');
    lr(120, C.msg, 1, 'BUY TCS, id a7');
    od(140, C.good, 2, 'save, a7 UNIQUE');
    rl(180, C.bad, 3, 'jawab nahi aaya', { dash: 1 }); cross(172, 180);
    lr(240, C.msg, 4, 'dobara: wahi a7');
    od(255, C.good, 5, 'a7 pehle se hai');
    rl(300, C.good, 6, 'purana jawab');
    cells(752, 160, [['a7', C.good]], { w: 54 });
    L(780, 230, 'order ek hi', C.good);
  } else if (ph === 2) {
    base('id: a7', 'acc', 'a7: chal raha');
    lr(120, C.msg, 1, 'BUY TCS, id a7');
    od(140, C.acc, 2, 'a7: IN_PROGRESS');
    lr(190, C.msg, 3, 'dobara: wahi a7');
    rl(260, C.acc, 4, '409: thoda ruko');
    cells(752, 170, [['a7', C.acc]], { w: 54 });
    L(780, 230, 'abhi chal rahi', C.acc);
    L(450, 345, 'hal: pehli chal rahi to 409 thoda ruko, order ek hi', C.good);
  } else {
    base('id: a7', 'good', 'a7: ho gaya');
    lr(120, C.msg, 1, 'phir: wahi a7');
    od(140, C.good, 2, 'a7 ho chuka');
    rl(200, C.good, 3, 'saved jawab');
    cells(752, 170, [['a7', C.good]], { w: 54 });
    L(780, 230, 'order ek hi', C.good);
  }
}
