// Card 8: 10 lakh log har second price poochh rahe (polling), server baitha. Ilaaj: WebSocket, price badla tab server bheje.
// Agar: connection toota, wapas juda -> abhi ka price alag se do.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 10 lakh log har second poochh rahe "TCS ka price?". Server baith gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: connection khula rakho, price badla tabhi server khud bheje (WebSocket push)'],
  [6, YEL, 'AGAR', 'AGAR connection toota, wapas juda? -> abhi ka price alag se do. Pub/sub kuch rakhta nahi'],
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
  [[C.bad, '10 lakh users → Price server', 'har second "price?"'], [C.acc, 'Price server → users', 'zyadatar "wahi hai". CPU 100%']],
  [[C.msg, 'App → WS server', 'ek baar juda, khula rakha'], [C.acc, 'Matching → Pub/sub', 'TCS 3001, sirf jab badla'], [C.good, 'Pub/sub → WS server', 'har WS server tak'], [C.good, 'WS server → App', 'khud bheja, Arpan ne poochha nahi']],
  [[C.bad, 'WS server → App', 'net gaya: beech ke price nahi pahunche'], [C.acc, 'App → WS server', 'phir juda'], [C.good, 'Price store → WS server', 'abhi ka price: TCS 3007'], [C.good, 'WS server → App', 'push, ab aage live']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 110, 170, 110, '10 lakh users', 'har second', { st: 'bad' }); dots(15, 35, 175, C.bad, { cols: 8, gap: 18, r: 5 });
    B(560, 130, 220, 80, 'Price server', 'CPU 100%', { st: 'bad' }); cross(805, 170);
    A(190, 150, 558, 150, C.bad, { n: 1 }); L(375, 135, '"price?" x 10 lakh / sec', C.bad);
    A(558, 190, 192, 190, C.muted, { n: 2, dash: 1 }); L(375, 220, 'zyadatar jawab: "wahi hai"', C.muted);
    L(450, 320, 'price badla hi nahi tab bhi poochh rahe. Poochhne ki line hi bekaar', C.bad);
  } else if (ph === 1) {
    panel(0, 'connection', ['Arpan ka app', 'WebSocket', ''], ['WS server', 'khula rakha', 'good'], 'juda', true, C.msg, 'ek baar judo');
    panel(1, 'price badla', ['Matching', 'TCS 3001', 'acc'], ['Pub/sub', 'TCS 3001', 'acc'], 'daala', true, C.acc, 'sirf jab badla');
    panel(2, 'har WS server', ['Pub/sub', '', 'acc'], ['WS server', 'TCS 3001', 'good'], 'bheja', true, C.good, 'sab server tak');
    panel(3, 'push', ['WS server', '', ''], ['Arpan ka app', 'TCS 3001', 'good'], 'khud bheja', true, C.good, 'Arpan ne poochha nahi');
    L(450, 320, 'badla tab bhejo. 1000 baar badla to har ~100 ms aakhri wala', C.good);
  } else {
    panel(0, 'toota', ['Arpan ka app', 'net gaya', 'bad'], ['WS server', '', ''], '', true, C.bad, 'beech ke price gaye');
    A(75, 158, 75, 110, C.bad, { dash: 1 }); cross(75, 134); T(90, 139, 'price', { c: C.bad, b: 1, size: 14, a: 'start' });
    panel(1, 'wapas juda', ['Arpan ka app', 'phir juda', 'acc'], ['WS server', '', ''], 'juda', true, C.acc, 'purane nahi chahiye');
    panel(2, 'abhi ka price', ['Price store', 'TCS 3007', 'good'], ['WS server', '', ''], 'abhi ka', true, C.good, 'alag se ek baar');
    panel(3, 'phir push', ['WS server', '', ''], ['Arpan ka app', 'TCS 3007', 'good'], 'bheja', true, C.good, 'ab aage live');
    L(450, 320, 'beech ke purane price bekaar the. Sirf abhi ka chahiye', C.good);
  }
}
