// Card 1: Kirana ko 500, Arpan -500 likha, server gira, Kirana +500 hua hi nahi.
// Ilaaj: sab ek transaction (BEGIN ... COMMIT). Agar: ledger INSERT fail -> ROLLBACK, -500 bhi ulta.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Kirana ko 500. Arpan -500 likha, phir server gira. Kirana +500 hua hi nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: BEGIN, Arpan -500, Kirana +500, ledger, COMMIT. Ek transaction: sab ya kuch nahi'],
  [6, YEL, 'AGAR', 'AGAR ledger ka INSERT fail? -> ROLLBACK, Arpan ka -500 bhi ulta'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
const STEPS = [
  [[C.acc, 'Payment Svc → Wallet DB', 'Payment Svc ne Arpan ke wallet se 500 kaat diye'], [C.bad, 'Payment Svc gira', 'Aage likhne se pehle hi Payment Svc gir gaya'], [C.bad, 'Payment Svc → Wallet DB', 'Kirana ke +500 kabhi likhe hi nahi gaye'], [C.bad, 'Wallet DB → Arpan / Kirana', 'Arpan ka 500 kata, aur Kirana ko kuch nahi mila']],
  [[C.msg, 'Payment Svc → Wallet DB', 'Payment Svc ne ek transaction shuru kiya (BEGIN)'], [C.good, 'paanchon saath', 'Arpan -500, Kirana +500, ledger ki 2 line aur payment DONE, paanchon ek saath'], [C.good, 'COMMIT', 'COMMIT pe hi pakka; pehle gira to DB apne log (WAL) se sab ulta karta']],
  [[C.bad, 'ledger INSERT', 'Transaction ke beech ledger wali line likhte waqt fail hua'], [C.good, 'Transaction → Wallet DB', 'ROLLBACK hua: Arpan ke 1000, Kirana jaisa tha, user ko "nahi hua"']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 160, 80, 'Payment Svc', 'Kirana ko 500');
    B(330, 40, 260, 240, 'Wallet DB', '', { st: 'bad' });
    cells(350, 95, [['Arpan: 1000 -> 500', C.acc]], { w: 220, h: 34 });
    cells(350, 195, [['Kirana: +500 nahi', C.bad]], { w: 220, h: 34 });
    A(180, 150, 348, 112, C.acc, { n: 1 }); L(255, 112, 'Arpan -500', C.acc);
    num(100, 240, 2, C.bad); L(100, 265, 'phir server gira', C.bad);
    A(180, 190, 348, 212, C.bad, { n: 3, dash: 1, at: 0.3 }); cross(300, 205); L(265, 300, 'Kirana +500 likha hi nahi', C.bad);
    B(690, 40, 180, 70, 'Arpan', 'app pe 500 kam', { st: 'bad' });
    B(690, 210, 180, 70, 'Kirana', 'kuch nahi mila', { st: 'bad' });
    A(590, 112, 688, 80, C.bad, { n: 4 });
    A(590, 212, 688, 245, C.bad);
    L(450, 340, 'do alag badlaav, beech me crash. 500 kata, kisi ko mila nahi', C.bad);
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
    B(610, 110, 260, 90, 'Wallet DB', 'Arpan 1000, Kirana jaisa', { st: 'good' });
    A(540, 155, 608, 155, C.good, { n: 2 }); L(575, 140, 'ROLLBACK', C.good);
    L(450, 320, 'paanchon me se ek bhi nahi bacha. User ko "nahi hua", dobara try kar sakta', C.good);
  }
}
