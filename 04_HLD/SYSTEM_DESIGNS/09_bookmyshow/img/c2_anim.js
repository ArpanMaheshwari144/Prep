// Card 2: payment 3 min, beech me Ravi A1 le gaya. Ilaaj: HOLD + held_until. Agar: umar khatam, phir pay -> 0 row, refund.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine A1 chuni, pay me 3 min lag rahe. Seat available hi thi, Ravi le gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: A1 HELD (Arpan), held_until = ab + 5 min. Ravi ko held dikhti, umar khatam to chhuti'],
  [6, YEL, 'AGAR', 'AGAR umar khatam, phir pay aaya? -> confirm pe bhi WHERE, 0 row, refund'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'App (Arpan) → Payment', 'A1 chuni, pay me 3 min'], [C.bad, 'App (Ravi) → Seats DB', 'A1 abhi available thi: book'], [C.bad, 'Arpan', 'paisa kata, seat Ravi ki']],
  [[C.acc, 'App → Seats DB', 'A1 HELD (Arpan), 7:05 tak'], [C.good, 'Seats DB → App (Ravi)', 'A1 held dikhi, nahi le sakta'], [C.good, 'App → Seats DB', 'pay hua: A1 BOOKED'], [C.acc, 'Sweeper → Seats DB', 'pay nahi hua to 7:05 pe chhod do']],
  [[C.acc, 'Sweeper → Seats DB', '7:05: umar khatam, A1 available'], [C.bad, 'App (Ravi) → Seats DB', 'A1 Ravi ne le li'], [C.bad, 'App → Seats DB', 'Arpan ka confirm: WHERE held +|Arpan: 0 row'], [C.good, 'App → Payment', 'refund. Arpan ko "seat gayi"']],
];
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
function frame(ph) {
  if (ph === 0) {
    B(20, 30, 170, 80, 'App (Arpan)', 'A1 chuni');
    B(20, 230, 170, 80, 'App (Ravi)', 'A1 book');
    B(340, 30, 200, 80, 'Payment', '3 min...', { st: 'acc' });
    B(640, 130, 230, 90, 'Seats DB', 'A1: Ravi', { st: 'bad', icon: 'db' });
    A(190, 70, 338, 70, C.msg, { n: 1 });
    A(190, 270, 638, 195, C.bad, { n: 2 });
    num(440, 160, 3, C.bad); L(460, 165, 'paisa kata, seat gayi', C.bad, 'start');
    L(450, 345, 'pay karte-karte seat chhin gayi', C.bad);
  } else if (ph === 1) {
    panel(0, 'A1 chuni', ['App (Arpan)', '', ''], ['Seats DB', 'A1: HELD, 7:05', 'acc'], 'hold', true, C.acc, '5 min ki umar');
    panel(1, 'Ravi', ['App (Ravi)', 'A1 held', 'good'], ['Seats DB', 'A1: HELD', 'acc'], 'dekha', false, C.good, 'nahi le sakta');
    panel(2, 'pay hua', ['App (Arpan)', 'paisa aaya', 'good'], ['Seats DB', 'A1: BOOKED', 'good'], 'confirm', true, C.good, 'Arpan ki');
    panel(3, 'pay nahi', ['Sweeper', '7:05', 'acc'], ['Seats DB', 'A1: available', ''], 'chhodo', true, C.acc, 'seat atki nahi rehti');
    L(450, 320, 'pay ke beech seat pakdi rahi, par hamesha ke liye nahi', C.good);
  } else {
    panel(0, 'umar khatam', ['Sweeper', '7:05', 'acc'], ['Seats DB', 'A1: available', ''], 'chhodo', true, C.acc, 'Arpan ne pay nahi kiya');
    panel(1, 'Ravi', ['App (Ravi)', 'A1 chuni', ''], ['Seats DB', 'A1: Ravi', 'bad'], 'book', true, C.bad, 'khaali thi, le li');
    panel(2, 'Arpan ka pay', ['App (Arpan)', '7:06 confirm', 'bad'], ['Seats DB', '0 row', 'bad'], 'UPDATE', true, C.bad, 'WHERE held + Arpan');
    panel(3, 'refund', ['Arpan', 'paisa wapas', 'good'], ['App (Arpan)', '', ''], 'refund', false, C.good, '"seat gayi" bataya');
    L(450, 320, 'der se aaya pay seat nahi chheenta, paisa wapas jaata', C.good);
  }
}
