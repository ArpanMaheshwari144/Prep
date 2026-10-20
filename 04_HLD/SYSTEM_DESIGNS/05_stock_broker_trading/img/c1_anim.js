// Card 1: ek baar dabaya, network gaya, app ne khud dobara bheja, order do. Ilaaj: screen khulte id, server UNIQUE.
// Agar: wahi id dobara, pehli abhi chal rahi -> 409, thoda ruko.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine ek baar Buy dabaya. Jawab aate network gaya, app ne khud dobara bheja'],
  [3, GRN, 'ILAAJ', 'ILAAJ: screen khulte hi app ek id banata. Dobara bheja to wahi id, server UNIQUE se pakadta'],
  [6, YEL, 'AGAR', 'AGAR wahi id dobara, pehli abhi chal rahi? -> thoda ruko (IN_PROGRESS, 409)'],
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
    panel(0, 'Buy dabaya', ['Arpan ka app', 'TCS x 10', ''], ['Order Svc', 'order#1 bana', 'good'], 'bheja', true, C.msg, 'ek baar dabaya');
    panel(1, 'network gaya', ['Arpan ka app', 'jawab nahi aaya', 'bad'], ['Order Svc', 'order#1', ''], 'jawab', false, C.bad, 'beech me gum');
    panel(2, 'app ne dobara', ['Arpan ka app', 'khud retry', 'acc'], ['Order Svc', '', ''], 'phir bheja', true, C.acc, 'Arpan ne kuch nahi kiya');
    panel(3, 'do order', ['Order Svc', 'naya maana', 'bad'], ['Orders', 'order#1 + order#2', 'bad'], 'bana', true, C.bad, '20 share, paisa dugna');
    L(450, 320, 'server ko pata hi nahi tha ki ye wahi purani request hai', C.bad);
  } else if (ph === 1) {
    panel(0, 'screen khuli', ['Arpan ka app', 'id: k7', 'acc'], ['Arpan', 'Buy screen', ''], 'banaya', false, C.acc, 'tap se pehle hi id');
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
