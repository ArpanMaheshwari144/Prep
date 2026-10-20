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
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
const STEPS = [
  [[C.acc, 'Settlement → Wallet DB', 'Settlement ne Arpan ke wallet se 30k kaat diye'], [C.bad, 'Settlement gira', 'Aage likhne se pehle hi Settlement gir gaya'], [C.bad, 'Settlement → Wallet DB', 'Suresh ke +30k aur share likhe hi nahi gaye'], [C.bad, 'Wallet DB → Arpan / Suresh', 'Arpan ka 30k gaya par share nahi, Suresh ko paisa nahi']],
  [[C.msg, 'Settlement → Wallet DB', 'Settlement ne ek transaction shuru kiya (BEGIN)'], [C.good, 'chaaron saath', 'Arpan -30k +10 share, Suresh +30k -10 share, chaaron ek saath'], [C.good, 'COMMIT', 'COMMIT pe hi pakka; beech me gira to sab rollback']],
  [[C.good, 'Settlement → Wallet DB', 'Settlement ne trade t1 likha, commit ho gaya'], [C.acc, 'Settlement retry → Wallet DB', 'Jawab kho gaya tha, to retry ne t1 phir bheja'], [C.acc, 'Wallet DB → retry', 't1 pehle se tha (UNIQUE), DB ne kuch nahi badla'], [C.good, 'Wallet DB → Suresh', 'Suresh ko +30k sirf ek hi baar mile']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 160, 80, 'Settlement', 'trade t1');
    B(330, 30, 260, 250, 'Wallet DB', '', { st: 'bad' });
    cells(350, 85, [['Arpan: -30k', C.acc]], { w: 220, h: 34 });
    cells(350, 145, [['Suresh: +30k nahi', C.bad]], { w: 220, h: 34 });
    cells(350, 205, [['share: nahi jude', C.bad]], { w: 220, h: 34 });
    A(180, 150, 348, 102, C.acc, { n: 1 });
    num(100, 240, 2, C.bad); L(100, 265, 'phir crash', C.bad);
    A(180, 185, 348, 162, C.bad, { n: 3, dash: 1, at: 0.3 }); cross(290, 172);
    B(690, 40, 180, 80, 'Arpan', '30k gaya', { st: 'bad', icon: 'user' });
    B(690, 210, 180, 80, 'Suresh', 'paisa nahi', { st: 'bad', icon: 'user' });
    A(590, 100, 688, 80, C.bad, { n: 4 }); A(590, 200, 688, 245, C.bad);
    L(450, 330, 'aadha kaam likha, aadha nahi. Paisa beech me gum', C.bad);
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
