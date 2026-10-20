// Card 1: order ne khud email company ko call kiya, company band, order hi fail. Ilaaj: outbox + Kafka, order turant.
// Agar: email company band -> khabar Kafka me ~7 din padi, offset se wahin se aage.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Order Svc khud email company ko call karta. Company band, order hi fail'],
  [3, GRN, 'ILAAJ', 'ILAAJ: order + khabar ek hi DB txn (outbox), Kafka se peeche email. Pal-pal dekho'],
  [6, YEL, 'AGAR', 'AGAR email company ghanton band? -> khabar Kafka me ~7 din padi, offset se aage'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 30, title, { c: C.fg, b: 1, size: 13 });
  B(x + 40, 50, 125, 48, top[0], top[1], { st: top[2] });
  B(x + 40, 165, 125, 58, bot[0], bot[1], { st: bot[2] });
  if (down) A(x + 70, 100, x + 70, 163, c); else A(x + 70, 163, x + 70, 100, c);
  T(x + 140, 135, lab, { c, b: 1, size: 12 });
  T(x + 102, 252, note, { c: C.muted, size: 11 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 120, 60, 'Arpan', 'order kiya');
    B(260, 140, 150, 60, 'Order Svc', 'email ka wait');
    B(600, 130, 200, 80, 'Email company', 'band', { st: 'bad' }); cross(825, 170);
    A(140, 160, 258, 160, C.msg, { n: 1 });
    A(410, 160, 598, 160, C.bad, { n: 2, dash: 1 }); L(505, 145, 'email bhejo', C.bad);
    num(700, 240, 3, C.bad); L(700, 265, 'jawab hi nahi', C.bad);
    A(258, 185, 142, 185, C.bad, { n: 4 }); L(200, 220, 'order fail', C.bad);
    L(450, 330, 'email ki wajah se paisa wala order hi gaya', C.bad);
  } else if (ph === 1) {
    panel(0, 'ek hi txn', ['Order Svc', 'order aaya', ''], ['DB', 'order + outbox', 'good'], 'likha', true, C.good, 'dono saath, ya koi nahi');
    panel(1, 'turant jawab', ['Arpan', '"order ho gaya"', 'good'], ['Order Svc', '', ''], 'turant', false, C.good, 'email ka wait nahi');
    panel(2, 'relay', ['DB outbox', 'khabar: order#9', 'acc'], ['Kafka', 'order#9', 'acc'], 'daala', true, C.acc, 'Order Svc gira to bhi bachi');
    panel(3, 'peeche email', ['Kafka', 'order#9', 'acc'], ['Notification Svc', 'email bheja', 'good'], 'padha', true, C.good, 'apni speed se');
    L(450, 320, 'order kabhi email ki wajah se fail nahi hota. Khabar khoti bhi nahi', C.good);
  } else {
    B(20, 130, 300, 90, 'Kafka log (7 din rehta)', '', { st: 'acc' });
    cells(35, 175, [['38', C.good], ['39', C.good], ['40', C.good], ['41', C.acc], ['42', C.acc], ['43', C.acc]], { w: 42, h: 30 });
    B(420, 130, 180, 90, 'Notification Svc', 'offset: 40 tak bheja');
    B(700, 130, 170, 90, 'Email company', '3 ghante band', { st: 'bad' });
    A(320, 175, 418, 175, C.acc, { n: 1 }); L(370, 160, 'padha', C.acc);
    A(600, 175, 698, 175, C.bad, { n: 2, dash: 1, at: 0.2 }); cross(672, 175);
    num(510, 250, 3, C.good); L(510, 275, 'company wapas: 41 se aage', C.good);
    L(450, 330, '41, 42, 43 Kafka me pade rahe, khoye nahi', C.good);
  }
}
