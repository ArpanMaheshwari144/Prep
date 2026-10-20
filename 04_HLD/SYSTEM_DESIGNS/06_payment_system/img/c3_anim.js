// Card 3: metro me network gaya, app ne dobara bheja, server ne do payment maane, 1000 kate.
// Ilaaj: screen khulte key-555, retry pe wahi, UNIQUE. Agar: key wahi, amount alag -> hash, 422.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine ek baar Pay dabaya. Metro me network gaya, app ne khud dobara bheja'],
  [3, GRN, 'ILAAJ', 'ILAAJ: screen khulte hi app key-555 banata. Retry pe wahi key, server UNIQUE se pakadta'],
  [6, YEL, 'AGAR', 'AGAR key wahi, par amount alag? -> request ka hash bhi rakho, farak = 422'],
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
  [[C.msg, 'App → Payment Svc', 'Kirana ko 500'], [C.bad, 'Payment Svc → App', 'jawab metro me gum'], [C.acc, 'App → Payment Svc', 'khud dobara bheja'], [C.bad, 'Payment Svc → Wallet DB', 'naya maana: -500 phir, 1000 kate']],
  [[C.acc, 'Arpan → App', 'screen khuli: key-555 bani'], [C.good, 'App → Payment Svc', 'key-555 ke saath, UNIQUE save'], [C.acc, 'App → Payment Svc', 'network gaya, retry: wahi key-555'], [C.good, 'Payment Svc → App', 'key-555 pehle se: saved jawab']],
  [[C.msg, 'Request 1 → Payment Svc', 'key-555, 500: hash(500) rakha'], [C.good, 'Payment Svc → Wallet DB', '500 kata'], [C.acc, 'Request 2 → Payment Svc', 'wahi key-555, par 5000'], [C.bad, 'Payment Svc → Request 2', 'hash alag: 422']],
];
function frame(ph) {
  if (ph === 0) {
    panel(0, 'Pay dabaya', ['Arpan ka app', 'Kirana ko 500', ''], ['Payment Svc', 'payment#1: -500', 'good'], 'bheja', true, C.msg, 'ek baar dabaya');
    panel(1, 'network gaya', ['Arpan ka app', 'jawab nahi aaya', 'bad'], ['Payment Svc', 'payment#1', ''], 'jawab', false, C.bad, 'metro me gum');
    panel(2, 'app ne dobara', ['Arpan ka app', 'khud retry', 'acc'], ['Payment Svc', '', ''], 'phir bheja', true, C.acc, 'Arpan ne kuch nahi kiya');
    panel(3, 'do payment', ['Payment Svc', 'naya maana', 'bad'], ['Wallet DB', '-500, phir -500', 'bad'], 'likha', true, C.bad, '1000 kate');
    L(450, 320, 'server ko pata hi nahi tha ki ye wahi purani request hai', C.bad);
  } else if (ph === 1) {
    panel(0, 'screen khuli', ['Arpan ka app', 'key-555', 'acc'], ['Arpan', 'Pay screen khola', ''], 'khola', false, C.acc, 'tap se pehle hi key');
    panel(1, 'Pay (key-555)', ['Arpan ka app', 'key-555', ''], ['Payment Svc', 'key-555: -500', 'good'], 'bheja', true, C.good, 'key UNIQUE, save');
    panel(2, 'network gaya', ['Arpan ka app', 'retry: key-555', 'acc'], ['Payment Svc', '', ''], 'phir key-555', true, C.acc, 'key wahi rahi');
    panel(3, 'pehchaana', ['Arpan ka app', '"ho gaya"', 'good'], ['Payment Svc', 'key-555 pehle se', 'good'], 'saved jawab', false, C.good, '500 hi kate');
    L(450, 320, 'app kitni baar bhi bheje, ek key = ek payment. Pay button bhi pehli tap pe band', C.good);
  } else {
    B(20, 40, 170, 70, 'Request 1', 'key-555, 500');
    B(20, 240, 170, 70, 'Request 2', 'key-555, 5000', { st: 'bad' });
    B(350, 130, 240, 90, 'Payment Svc', 'key-555: hash(500)', { st: 'acc' });
    B(700, 40, 170, 70, 'Wallet DB', '-500, ek baar', { st: 'good' });
    A(190, 75, 348, 150, C.msg, { n: 1 }); L(250, 85, 'pehli baar', C.msg);
    A(590, 150, 698, 80, C.good, { n: 2 }); L(615, 100, 'kata', C.good);
    A(190, 265, 348, 200, C.acc, { n: 3 }); L(265, 210, 'key wahi', C.acc);
    A(360, 222, 192, 295, C.bad, { n: 4, at: 0.55 }); L(330, 300, '422: amount alag', C.bad, 'start');
    L(450, 30, 'sirf key nahi, request ka hash bhi milao', C.acc);
  }
}
