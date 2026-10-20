// Card 1: Kirana ko 500, Arpan -500 likha, server gira, Kirana +500 hua hi nahi.
// Ilaaj: sab ek transaction (BEGIN ... COMMIT). Agar: ledger INSERT fail -> ROLLBACK, -500 bhi ulta.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Kirana ko 500. Arpan -500 likha, phir server gira. Kirana +500 hua hi nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: BEGIN, Arpan -500, Kirana +500, ledger, COMMIT. Ek transaction, ya sab ya kuch nahi'],
  [6, YEL, 'AGAR', 'AGAR ledger ka INSERT fail? -> ROLLBACK, Arpan ka -500 bhi ulta'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 30, title, { c: C.fg, b: 1, size: 13 });
  B(x + 40, 50, 125, 48, top[0], top[1], { st: top[2] });
  B(x + 40, 165, 125, 58, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 70, 100, x + 70, 163, c); else A(x + 70, 163, x + 70, 100, c); } // khaali lab = teer nahi
  T(x + 140, 135, lab, { c, b: 1, size: 12 });
  T(x + 102, 252, note, { c: C.muted, size: 11 });
}
function frame(ph) {
  if (ph === 0) {
    panel(0, 'Arpan se kata', ['Payment Svc', 'Kirana ko 500', ''], ['Wallet DB', 'Arpan 1000 -> 500', 'acc'], 'likha', true, C.acc, 'pehla badlaav');
    panel(1, 'server gira', ['Payment Svc', 'gira', 'bad'], ['Wallet DB', 'Arpan 500', 'acc'], '', true, C.bad, 'aage likha hi nahi');
    panel(2, 'Kirana', ['Kirana', '+500?', 'bad'], ['Wallet DB', 'Kirana: kuch nahi', 'bad'], '', true, C.bad, 'paisa pahuncha nahi');
    panel(3, 'hisaab', ['Arpan', '500 kata', 'bad'], ['Ledger', 'koi line nahi', 'bad'], '', true, C.bad, '500 gayab');
    L(450, 320, 'do alag badlaav, beech me crash. Aadha hua, aadha nahi', C.bad);
  } else if (ph === 1) {
    B(20, 140, 150, 70, 'Payment Svc', 'Kirana ko 500');
    B(300, 30, 560, 250, 'Wallet DB: ek transaction', '', { st: 'good' });
    cells(320, 80, [['BEGIN', C.acc]], { w: 120, h: 32 });
    cells(320, 125, [['Arpan -500', C.good], ['Kirana +500', C.good]], { w: 160, h: 32 });
    cells(320, 170, [['ledger 2 line', C.good], ['payment: DONE', C.good]], { w: 160, h: 32 });
    cells(320, 215, [['COMMIT', C.good]], { w: 120, h: 32 });
    A(170, 175, 298, 175, C.msg, { n: 1 });
    num(700, 95, 2, C.good); L(720, 100, 'paanchon saath', C.good, 'start');
    num(700, 231, 3, C.good); L(720, 236, 'tab pakka', C.good, 'start');
    L(450, 320, 'COMMIT se pehle gira to DB apna log (WAL) dekh ke ulta karta, 500 wapas', C.good);
  } else {
    B(20, 30, 520, 220, 'Wallet DB: ek transaction', '', { st: 'acc' });
    cells(40, 80, [['BEGIN', C.acc]], { w: 120, h: 32 });
    cells(40, 125, [['Arpan -500', C.acc], ['Kirana +500', C.acc]], { w: 160, h: 32 });
    cells(40, 170, [['ledger INSERT', C.bad]], { w: 160, h: 32 });
    cross(230, 186); num(270, 186, 1, C.bad); L(290, 191, 'fail', C.bad, 'start');
    B(680, 110, 190, 90, 'Wallet DB', 'Arpan 1000, Kirana jaisa', { st: 'good' });
    A(540, 155, 678, 155, C.good, { n: 2 }); L(610, 140, 'ROLLBACK', C.good);
    L(450, 320, 'paanchon me se ek bhi nahi bacha. User ko "nahi hua", dobara try kar sakta', C.good);
  }
}
