// Card 1: ek baar dabaya, network gaya, app ne khud dobara bheja, order do. Ilaaj: screen khulte id, server UNIQUE.
// Agar: wahi id dobara, pehli abhi chal rahi -> 409, thoda ruko.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine ek baar Buy dabaya. Jawab aate network gaya, app ne khud dobara bheja'],
  [3, GRN, 'ILAAJ', 'ILAAJ: screen khulte hi app ek id banata. Retry pe wahi id, server UNIQUE se pakadta'],
  [6, YEL, 'AGAR', 'AGAR wahi id dobara, pehli abhi chal rahi? -> thoda ruko (IN_PROGRESS, 409)'],
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
  [[C.msg, 'App → Order Svc', 'Arpan ne app se TCS ke 10 share kharidne ka order bheja'], [C.bad, 'Order Svc → App', 'Order ho gaya, par jawab network me kho gaya'], [C.acc, 'App → Order Svc', 'App ne khud retry kiya, Arpan ne kuch nahi dabaya'], [C.bad, 'Order Svc → Orders', 'Order Svc ne naya order#2 bana diya, 20 share aur paisa dugna']],
  [[C.acc, 'Arpan → App', 'Buy screen khulte hi app ne ek id k7 bana li'], [C.good, 'App → Order Svc', 'Order k7 ke saath gaya, UNIQUE se order#1 bana'], [C.acc, 'App → Order Svc', 'Network gaya to app ne retry kiya, wahi k7 bheja'], [C.good, 'Order Svc → App', 'k7 pehle se tha, to naya order nahi, purana jawab lautaya']],
  [[C.msg, 'Request 1 → Order Svc', 'Pehli request k7 abhi chal rahi thi (IN_PROGRESS)'], [C.msg, 'Request 2 → Order Svc', 'Usi beech retry wahi k7 leke aa gaya'], [C.acc, 'Order Svc → Request 2', 'Order Svc ne 409 diya: thoda ruko, kaam chal raha'], [C.good, 'Order Svc → Orders', 'Pehli poori hui (order#1), ruk ke aayi retry ko saved jawab mila']],
];
function frame(ph) {
  if (ph === 0) {
    panel(0, 'Buy dabaya', ['Arpan ka app', 'TCS x 10', ''], ['Order Svc', 'order#1 bana', 'good'], 'bheja', true, C.msg, 'ek baar dabaya');
    panel(1, 'network gaya', ['Arpan ka app', 'jawab nahi aaya', 'bad'], ['Order Svc', 'order#1', ''], 'jawab', false, C.bad, 'beech me gum');
    panel(2, 'app ne dobara', ['Arpan ka app', 'khud retry', 'acc'], ['Order Svc', '', ''], 'phir bheja', true, C.acc, 'Arpan ne kuch nahi kiya');
    panel(3, 'do order', ['Order Svc', 'naya maana', 'bad'], ['Orders', 'order#1 + order#2', 'bad'], 'bana', true, C.bad, '20 share, paisa dugna');
    L(450, 320, 'server ko pata hi nahi tha ki ye wahi purani request hai', C.bad);
  } else if (ph === 1) {
    panel(0, 'screen khuli', ['Arpan ka app', 'id: k7', 'acc'], ['Arpan', 'Buy screen khola', ''], 'khola', false, C.acc, 'tap se pehle hi id');
    panel(1, 'Buy (k7)', ['Arpan ka app', 'k7', ''], ['Order Svc', 'k7 UNIQUE: order#1', 'good'], 'bheja', true, C.good, 'k7 save');
    panel(2, 'network gaya', ['Arpan ka app', 'retry: wahi k7', 'acc'], ['Order Svc', '', ''], 'phir k7', true, C.acc, 'id wahi rahi');
    panel(3, 'pehchaana', ['Arpan ka app', 'order#1', 'good'], ['Order Svc', 'k7 pehle se', 'good'], 'purana jawab', false, C.good, 'order ek hi');
    L(450, 320, 'app kitni baar bhi bheje, ek id = ek order', C.good);
  } else {
    B(20, 50, 160, 60, 'Request 1', 'k7');
    B(20, 240, 160, 60, 'Request 2', 'wahi k7, retry');
    B(350, 140, 210, 80, 'Order Svc', 'k7: IN_PROGRESS', { st: 'acc' });
    B(700, 140, 170, 80, 'Orders', 'order#1', { st: 'good' });
    A(180, 80, 348, 160, C.msg, { n: 1 }); L(250, 100, 'chal rahi', C.msg);
    A(180, 258, 348, 192, C.msg, { n: 2 }); A(360, 222, 182, 298, C.acc, { n: 3, at: 0.6 }); L(330, 300, '409: thoda ruko', C.acc, 'start');
    A(560, 180, 698, 180, C.good, { n: 4 }); L(630, 165, 'pehli poori', C.good);
    L(450, 30, 'ruk ke phir aaya to k7 DONE milega, saved jawab', C.good);
  }
}
