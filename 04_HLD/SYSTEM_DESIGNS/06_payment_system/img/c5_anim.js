// Card 5: PSP ko call kiya, jawab se pehle hamara server gira. Paisa gaya ya nahi?
// Ilaaj: call se PEHLE PENDING, phir webhook / job se SUCCESS. Agar: webhook do baar -> SUCCESS ek baar.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: PSP ko call kiya, jawab se pehle hamara server gira. Paisa gaya ya nahi?'],
  [3, GRN, 'ILAAJ', 'ILAAJ: call se PEHLE DB me PENDING. PSP webhook se bataye (ya job poochhe), tab SUCCESS'],
  [6, YEL, 'AGAR', 'AGAR webhook do baar aaya? -> SUCCESS sirf ek baar, doosra ignore'],
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
  [[C.msg, 'Payment Svc → PSP', 'call gayi, PSP ne 2000 laaye'], [C.bad, 'PSP → Payment Svc', 'SUCCESS bheja, par server gira'], [C.acc, 'Payment DB → Payment Svc', 'uth ke dekha: koi record nahi'], [C.bad, 'Payment Svc → PSP', 'andha dobara call?|ya paisa gum, ya do baar']],
  [[C.acc, 'Payment Svc → Payment DB', 'call se pehle t5: PENDING'], [C.msg, 'Payment Svc → PSP', 'call, phir server gira'], [C.good, 'PSP → Webhook Svc', 't5 SUCCESS: PSP khud batata'], [C.good, 'Webhook Svc → Payment DB', 't5: SUCCESS.|Webhook gum? job poochhti']],
  [[C.good, 'PSP → Webhook Svc', 'webhook #1'], [C.good, 'Webhook Svc → Payment DB', 'PENDING -> SUCCESS'], [C.acc, 'PSP → Webhook Svc', 'webhook #2, wahi'], [C.acc, 'Payment DB', 'WHERE status = PENDING: 0 row, ignore']],
];
function frame(ph) {
  if (ph === 0) {
    panel(0, 'call gayi', ['Payment Svc', 'top-up 2000', ''], ['PSP', '2000 laaya', 'good'], 'call', true, C.msg, 'PSP ne kaam kiya');
    panel(1, 'server gira', ['Payment Svc', 'gira', 'bad'], ['PSP', '"SUCCESS"', 'good'], 'jawab', false, C.bad, 'sunne wala koi nahi');
    panel(2, 'uth ke', ['Payment Svc', 'wapas aaya', 'acc'], ['Payment DB', 'koi record nahi', 'bad'], 'padha', false, C.acc, 'kya hua, pata nahi');
    panel(3, 'andha?', ['Payment Svc', 'dobara call?', 'bad'], ['PSP', '2000 do baar?', 'bad'], 'phir call', true, C.bad, 'ya paisa gum, ya do baar');
    L(450, 320, 'call se pehle kuch likha hi nahi tha, to uth ke kuch yaad nahi', C.bad);
  } else if (ph === 1) {
    panel(0, 'pehle PENDING', ['Payment Svc', 'top-up 2000', ''], ['Payment DB', 't5: PENDING', 'acc'], 'likha', true, C.acc, 'call se pehle');
    panel(1, 'call', ['Payment Svc', 'call ke baad gira', 'bad'], ['PSP', 't5: 2000 laaya', 'good'], 'call', true, C.msg, 'PENDING to bacha hai');
    panel(2, 'webhook', ['PSP', '"t5 SUCCESS"', 'good'], ['Webhook Svc', 'suna', ''], 'batata', true, C.good, 'PSP khud batata');
    panel(3, 'SUCCESS', ['Webhook Svc', '', ''], ['Payment DB', 't5: SUCCESS', 'good'], 'likha', true, C.good, 'gum? job poochhti');
    L(450, 320, 'Arpan ko PROCESSING dikha, phir SUCCESS. Andha dobara call kabhi nahi', C.good);
  } else {
    B(20, 120, 150, 90, 'PSP', 't5 SUCCESS', { st: 'good' });
    B(330, 120, 200, 90, 'Webhook Svc', '', { st: 'acc' });
    B(690, 120, 180, 90, 'Payment DB', 't5: SUCCESS', { st: 'good' });
    A(170, 140, 328, 140, C.good, { n: 1 }); L(250, 125, 'webhook #1', C.good);
    A(530, 140, 688, 140, C.good, { n: 2 }); L(610, 125, 'PENDING -> SUCCESS', C.good);
    A(170, 190, 328, 190, C.acc, { n: 3 }); L(250, 215, 'webhook #2, wahi', C.acc);
    A(530, 190, 688, 190, C.acc, { dash: 1 }); num(780, 240, 4, C.acc); L(780, 265, '0 row: ignore', C.acc);
    T(450, 300, "UPDATE payment SET status = 'SUCCESS' WHERE id = t5 AND status = 'PENDING'", { c: C.fg, size: 12 });
    L(450, 340, 'paisa ek hi baar juda, doosre webhook ne kuch nahi badla', C.good);
  }
}
