// Card 6: paisa kata, Suresh ko mila nahi. Ilaaj: chaaron ek transaction. Agar: retry ke baad do baar? Wapas: raat ko ledger jod 0.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Arpan -30k likha, phir crash. Suresh ko +30k mila hi nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: chaaron badlaav ek transaction me, phir COMMIT. Ya sab, ya koi nahi'],
  [6, YEL, 'AGAR', 'AGAR rollback ke baad retry, Suresh ko do baar? -> nahi, trade_id UNIQUE'],
  [9, GRN, 'WAPAS', 'WAPAS: raat ko ledger joda, jod 0 aaya, hisaab sahi'],
];
function setup() {}
function db(st, sub) { B(330, 40, 530, 280, 'DB (paisa + share)', sub, { st }); }
function four(a, b, c, d) {
  cells(360, 110, [['Arpan -30k', a], ['Arpan +10 share', b]], { w: 230, h: 30 });
  cells(360, 160, [['Suresh +30k', c], ['Suresh -10 share', d]], { w: 230, h: 30 });
}
function sd(y, c, n, s, o = {}) { A(170, y, 328, y, c, { n, at: 0.15, dash: o.dash }); L(260, y - 12, s, c); }
function frame(ph) {
  if (ph === 0) {
    B(20, 90, 150, 200, 'Settlement', 'trade T1', { st: 'bad' });
    db('bad', '');
    sd(130, C.bad, 1, 'Arpan -30k');
    four(C.bad, C.muted, C.muted, C.muted);
    num(60, 315, 2, C.bad); L(80, 320, 'crash', C.bad, 'start');
    num(380, 262, 3, C.bad); L(400, 267, 'Arpan -30k, Suresh +0', C.bad, 'start');
  } else if (ph === 1) {
    B(20, 90, 150, 200, 'Settlement', 'trade T1', { st: 'good' });
    db('good', '');
    el('rect', { x: 348, y: 98, width: 492, height: 104, rx: 6, fill: 'none', stroke: C.good, 'stroke-width': 2,
      'stroke-dasharray': '6 5' }, DYN);
    sd(150, C.good, 1, 'chaaron likho');
    four(C.good, C.good, C.good, C.good);
    num(380, 240, 2, C.good); L(400, 245, 'COMMIT: ya sab, ya koi nahi', C.good, 'start');
  } else if (ph === 2) {
    B(20, 90, 150, 200, 'Settlement', 'trade T1', { st: 'acc' });
    L(95, 75, 'rollback ke baad retry', C.acc);
    db('good', '');
    sd(130, C.good, 1, 'T1: COMMIT');
    sd(230, C.acc, 2, 'T1 phir aaya');
    four(C.good, C.good, C.good, C.good);
    cells(360, 215, [['trade_id T1: UNIQUE', C.acc]], { w: 230, h: 30 });
    num(615, 230, 3, C.good); L(635, 235, 'dobara = kuch nahi', C.good, 'start');
    L(595, 300, 'Suresh ko +30k ek hi baar', C.good);
  } else {
    B(20, 90, 150, 200, 'Raat ka check', '', { st: 'acc' });
    db('good', 'ledger');
    sd(150, C.acc, 1, 'ledger jodo');
    four(C.good, C.good, C.good, C.good);
    num(420, 255, 2, C.good); T(560, 262, 'jod: -30k + 30k = 0', { c: C.good, b: 1, size: 20 });
  }
}
