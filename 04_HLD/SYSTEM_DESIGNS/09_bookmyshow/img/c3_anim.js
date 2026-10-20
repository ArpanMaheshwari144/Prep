// Card 3: pay dabaya, network gaya, app ne dobara bheja, 500 do baar. Ilaaj: bookingId B-42 idempotency key. Agar: paisa kata, seat fail -> refund.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Pay dabaya, network gaya. App ne khud dobara bheja, server ne do baar kaata'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har pay usi bookingId (B-42) se. Wahi dobara aaya to "ho chuka", purana jawab'],
  [6, YEL, 'AGAR', 'AGAR paisa kat gaya par seat fail? -> refund karo aur user ko batao'],
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
  [[C.msg, 'App → Booking Svc', 'Arpan ne booking B-42 ke 500 pay kiye'], [C.bad, 'Booking Svc → App', 'Paisa kata, par jawab network me kho gaya'], [C.acc, 'App → Booking Svc', 'App ne khud dobara bheja, Arpan ne kuch nahi dabaya'], [C.bad, 'Booking Svc → PSP', 'Booking Svc ne naya maana, 500 phir kat gaye']],
  [[C.msg, 'App → Booking Svc', 'Pay request bookingId B-42 ke saath gayi'], [C.good, 'Booking Svc → PSP', 'B-42 ke 500 kate aur ye baat save hui'], [C.acc, 'App → Booking Svc', 'Network gaya, app ne retry kiya, wahi B-42'], [C.good, 'Booking Svc → App', 'B-42 pehle ho chuka, purana jawab lautaya, dobara nahi kata']],
  [[C.good, 'Booking Svc → PSP', 'PSP se Arpan ke 500 kat gaye'], [C.bad, 'Booking Svc → Seats DB', 'Seat confirm ki, 0 row, seat ja chuki thi'], [C.acc, 'Booking Svc → PSP', 'Booking Svc ne PSP se 500 refund karwaye'], [C.good, 'Booking Svc → Arpan', 'Arpan ko bataya: seat nahi mili, paisa wapas']],
];
function frame(ph) {
  if (ph === 0) {
    panel(0, 'Pay dabaya', ['Arpan ka app', 'B-42: 500', ''], ['Booking Svc', 'kata', 'good'], 'pay', true, C.msg, 'ek baar dabaya');
    panel(1, 'network gaya', ['Arpan ka app', 'jawab nahi', 'bad'], ['Booking Svc', '', ''], 'jawab', false, C.bad, 'beech me gum');
    panel(2, 'app ne dobara', ['Arpan ka app', 'khud retry', 'acc'], ['Booking Svc', '', ''], 'phir pay', true, C.acc, 'Arpan ne kuch nahi kiya');
    panel(3, 'do baar', ['Booking Svc', 'naya maana', 'bad'], ['PSP', '500, phir 500', 'bad'], 'kaato', true, C.bad, '1000 kate');
    L(450, 320, 'server ko pata hi nahi tha ki ye wahi purana pay hai', C.bad);
  } else if (ph === 1) {
    panel(0, 'pay (B-42)', ['Arpan ka app', 'B-42', ''], ['Booking Svc', '', ''], 'pay', true, C.msg, 'bookingId saath');
    panel(1, 'kata', ['Booking Svc', 'B-42 save', 'good'], ['PSP', '500 kata', 'good'], 'kaato', true, C.good, 'ek baar');
    panel(2, 'retry', ['Arpan ka app', 'wahi B-42', 'acc'], ['Booking Svc', '', ''], 'phir B-42', true, C.acc, 'network gaya tha');
    panel(3, 'pehchaana', ['Arpan ka app', '"ho chuka"', 'good'], ['Booking Svc', 'B-42 pehle se', 'good'], 'purana jawab', false, C.good, '500 hi kate');
    L(450, 320, 'kitni baar bhi aaye, ek bookingId = ek payment', C.good);
  } else {
    B(20, 130, 190, 80, 'Booking Svc', 'B-42', { st: 'acc' });
    B(380, 20, 200, 80, 'PSP', '500 kata, phir refund', { st: 'good' });
    B(380, 240, 200, 80, 'Seats DB', 'confirm: 0 row', { st: 'bad', icon: 'db' });
    B(700, 130, 170, 80, 'Arpan', 'paisa wapas', { st: 'good', icon: 'user' });
    A(210, 140, 378, 45, C.good, { n: 1, at: 0.4 });
    A(210, 190, 378, 280, C.bad, { n: 2 });
    A(210, 165, 378, 92, C.acc, { n: 3, at: 0.7 });
    A(210, 175, 698, 170, C.good, { n: 4, at: 0.8 });
    L(450, 345, 'paisa aur seat saga jaisa: ek fail to doosra ulta', C.good);
  }
}
