// Card 5: 2 lakh log ek show pe. Ilaaj: waiting room + Kafka (sirf badi sale). Agar: UPDATE jeeta, user ko kaise pata -> poll / push.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 12 baje Arijit ka concert khula. 2 lakh log ek show ki 3000 seat pe toot pade'],
  [3, GRN, 'ILAAJ', 'ILAAJ: darwaze pe ginti: 3000 andar, baaki line me. Andar walon ki booking Kafka se'],
  [6, YEL, 'AGAR', 'AGAR UPDATE jeet gaya, user ko kaise pata chalega? -> bookingId se poll ya WebSocket push'],
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
  [[C.msg, '2 lakh → App', '12 baje 2 lakh log ne ek saath Book dabaya'], [C.bad, 'App → Seats DB', 'Sab ek hi seat rows ke lock pe ruke'], [C.bad, 'Seats DB', 'Sab dheema, kisi ko jawab nahi mila']],
  [[C.msg, '2 lakh → Gate', 'Waiting room: 3000 andar gaye, baaki line me ruke'], [C.acc, 'Gate → Kafka', 'Andar walon ki booking show 42 ki Kafka line me gayi'], [C.acc, 'Kafka → Worker', 'Worker line se ek-ek booking uthata, bheed nahi'], [C.good, 'Worker → Seats DB', 'UPDATE ... WHERE available, har booking ms me']],
  [[C.msg, 'Arpan → Gate', 'Arpan line me laga, use bookingId B-77 mila'], [C.good, 'Worker → Seats DB', 'Worker ne A1 ka UPDATE kiya, 1 row badli'], [C.good, 'Worker → Status', 'Worker ne Status me B-77 = CONFIRMED likha'], [C.good, 'Status → Arpan', 'Arpan ko WebSocket se push mila, ya app B-77 se poll karti']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 110, 190, 100, '2 lakh log', 'ek saath Book', { icon: 'users' });
    dots(10, 40, 250, C.bad, { cols: 10, gap: 16, r: 6 });
    B(340, 120, 180, 80, 'App', '', { st: 'bad' });
    B(640, 110, 230, 100, 'Seats DB', '3000 rows, lock', { st: 'bad', icon: 'db' });
    bar(660, 180, 190, 12, 1, '');
    A(210, 160, 338, 160, C.msg, { n: 1 });
    A(520, 160, 638, 160, C.bad, { n: 2 });
    num(755, 250, 3, C.bad); L(755, 280, 'sab dheema', C.bad);
    L(450, 345, '3000 seat, 2 lakh log ek hi rows pe', C.bad);
  } else if (ph === 1) {
    B(20, 120, 150, 80, '2 lakh', '', { icon: 'users' });
    B(230, 110, 150, 100, 'Gate', '3000 andar', { st: 'acc' });
    B(440, 120, 140, 80, 'Kafka', 'show 42', { st: 'acc' });
    B(640, 20, 230, 70, 'Worker', 'ek-ek', { st: 'good' });
    B(640, 230, 230, 80, 'Seats DB', 'UPDATE', { st: 'good', icon: 'db' });
    A(170, 160, 228, 160, C.msg, { n: 1 });
    A(380, 160, 438, 160, C.acc, { n: 2 });
    A(580, 145, 638, 60, C.acc, { n: 3 });
    A(755, 92, 755, 228, C.good, { n: 4 });
    L(450, 345, 'sirf badi sale pe. Aam din seedha UPDATE', C.good);
  } else {
    panel(0, 'line me', ['Arpan', 'B-77 mila', 'acc'], ['Gate', 'line: 40,000 aage', 'acc'], 'Book', true, C.msg, 'jawab baad me');
    panel(1, 'jeeta', ['Worker', 'B-77', ''], ['Seats DB', 'A1: 1 row', 'good'], 'UPDATE', true, C.good, 'faisla DB ka');
    panel(2, 'haal', ['Worker', '', ''], ['Status', 'B-77: CONFIRMED', 'good'], 'likha', true, C.good, 'bookingId pe');
    panel(3, 'bataya', ['Arpan', 'A1 mili', 'good'], ['Status', '', ''], 'push / poll', false, C.good, 'WebSocket ya poll');
    L(450, 320, 'line me tha, to jawab bookingId se baad me aata', C.good);
  }
}
