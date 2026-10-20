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
  num(x + 20, 25, k + 1, c); T(x + 112, 30, title, { c: C.fg, b: 1, size: 13 });
  B(x + 40, 50, 125, 48, top[0], top[1], { st: top[2] });
  B(x + 40, 165, 125, 58, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 70, 100, x + 70, 163, c); else A(x + 70, 163, x + 70, 100, c); } // khaali lab = teer nahi
  T(x + 140, 135, lab, { c, b: 1, size: 12 });
  T(x + 102, 252, note, { c: C.muted, size: 11 });
}
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
    panel(1, 'wapas juda', ['Arpan ka app', 'phir juda', 'acc'], ['WS server', '', ''], 'juda', true, C.acc, 'purane nahi chahiye');
    panel(2, 'abhi ka price', ['Price store', 'TCS 3007', 'good'], ['WS server', '', ''], 'abhi ka', true, C.good, 'alag se ek baar');
    panel(3, 'phir push', ['WS server', '', ''], ['Arpan ka app', 'TCS 3007', 'good'], 'bheja', true, C.good, 'ab aage live');
    L(450, 320, 'beech ke purane price bekaar the. Sirf abhi ka chahiye', C.good);
  }
}
