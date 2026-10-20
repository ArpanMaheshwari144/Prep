// Card 6: mera paisa kata, Suresh ko mila hi nahi (beech me crash). Ilaaj: chaaron badlaav ek transaction, COMMIT.
// Agar: commit ho gaya par jawab kho gaya, retry aayi -> trade_id UNIQUE, doosri ruki.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: sauda hua. Arpan -30k likha, phir crash. Suresh ko +30k mila hi nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: chaaron badlaav ek transaction me, phir COMMIT. Ya sab, ya koi nahi'],
  [6, YEL, 'AGAR', 'AGAR commit ho gaya par jawab kho gaya, retry aayi? -> trade_id UNIQUE, doosri ruki'],
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
    panel(0, 'Arpan se kata', ['Settlement', 'trade t1', ''], ['Wallet DB', 'Arpan -30k', 'acc'], 'likha', true, C.acc, 'pehla kadam');
    panel(1, 'crash', ['Settlement', 'gira', 'bad'], ['Wallet DB', 'Arpan -30k', 'acc'], '', true, C.bad, 'aage likha hi nahi');
    panel(2, 'Suresh', ['Suresh', '+30k?', 'bad'], ['Wallet DB', 'Suresh: kuch nahi', 'bad'], '', true, C.bad, 'paisa mila hi nahi');
    panel(3, 'share', ['Arpan', '+10 share?', 'bad'], ['Wallet DB', 'share nahi jude', 'bad'], '', true, C.bad, '30k gayab');
    L(450, 320, 'aadha kaam likha, aadha nahi. Paisa beech me gum', C.bad);
  } else if (ph === 1) {
    B(20, 140, 150, 70, 'Settlement', 'trade t1');
    B(300, 40, 560, 230, 'Ek transaction', '', { st: 'good' });
    cells(320, 90, [['BEGIN', C.acc]], { w: 120, h: 32 });
    cells(320, 135, [['Arpan -30k', C.good], ['Arpan +10 share', C.good]], { w: 160, h: 32 });
    cells(320, 180, [['Suresh +30k', C.good], ['Suresh -10 share', C.good]], { w: 160, h: 32 });
    cells(320, 225, [['COMMIT', C.good]], { w: 120, h: 32 });
    A(170, 175, 298, 175, C.msg, { n: 1 });
    num(700, 105, 2, C.good); L(720, 110, 'chaaron saath', C.good, 'start');
    num(700, 241, 3, C.good); L(720, 246, 'tab pakka', C.good, 'start');
    L(450, 330, 'COMMIT se pehle gira to rollback: kuch nahi likha, user ko PENDING', C.good);
  } else {
    B(20, 60, 150, 60, 'Settlement', 't1 pehli baar');
    B(20, 240, 150, 60, 'Settlement', 't1 retry');
    B(330, 130, 230, 90, 'Wallet DB', 'trade_id UNIQUE', { st: 'acc' });
    B(690, 130, 170, 90, 'Suresh', '+30k ek hi baar', { st: 'good' });
    A(170, 90, 328, 155, C.good, { n: 1 }); L(240, 105, 't1 likha', C.good);
    A(170, 258, 328, 192, C.acc, { n: 2 }); A(345, 225, 172, 300, C.acc, { n: 3, at: 0.55 }); L(330, 305, 't1 pehle se: kuch nahi', C.acc, 'start');
    A(560, 175, 688, 175, C.good, { n: 4 }); L(625, 160, 'ek baar', C.good);
    L(450, 30, 'dobara chala to bhi kuch nahi badla', C.good);
  }
}
