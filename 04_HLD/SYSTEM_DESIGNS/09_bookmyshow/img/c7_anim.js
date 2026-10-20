// Card 7: ek App machine giri, booking band. Ilaaj: stateless App + LB health. Agar: payment beech me, machine giri -> same bookingId.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: shaam ko main seat chun raha tha, ek hi App machine thi, wo gir gayi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: App kuch yaad nahi rakhti, hold DB me. LB /health se dekhta, giri machine bahar'],
  [6, YEL, 'AGAR', 'AGAR payment beech me tha, machine giri? -> same bookingId, doosri machine'],
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
  [[C.msg, 'Arpan → App', 'Shaam ko Arpan seat chun raha tha'], [C.bad, 'App giri', 'Ek hi App machine thi, wo gir gayi'], [C.bad, 'Arpan', 'Booking band, Arpan ke liye kuch nahi khulta']],
  [[C.msg, 'Arpan → LB', 'Arpan ne Book dabaya, request pehle LB pe aayi'], [C.bad, 'LB → App-1', 'App-1 ne /health pe 3 baar jawab nahi diya, LB ne bahar kiya'], [C.good, 'LB → App-2', 'LB ne request App-2 ko de di'], [C.good, 'App-2 → Seats DB', 'A1 ka hold DB me wahi mila, booking aage chali']],
  [[C.good, 'App-1 → PSP', 'App-1 ne B-42 ke 500 PSP se kaat liye'], [C.bad, 'App-1 giri', 'Arpan ko jawab dene se pehle App-1 gir gayi'], [C.acc, 'App-2 → PSP', 'App-2 ne retry kiya, wahi B-42 ke saath'], [C.good, 'PSP → App-2', 'B-42 pehle ho chuka tha, purana jawab mila, paisa ek hi baar']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 170, 80, 'Arpan', 'A1 chun raha', { icon: 'user' });
    B(340, 120, 200, 100, 'App', 'ek hi machine', { st: 'bad', icon: 'server' }); cross(565, 170);
    B(690, 130, 180, 80, 'Seats DB', '', { icon: 'db' });
    A(190, 170, 338, 170, C.msg, { n: 1 });
    num(440, 250, 2, C.bad); L(460, 255, 'giri', C.bad, 'start');
    num(105, 250, 3, C.bad); L(125, 255, 'booking band', C.bad, 'start');
    L(450, 345, 'ek machine = ek kamzor jagah', C.bad);
  } else if (ph === 1) {
    B(20, 130, 140, 80, 'Arpan', '', { icon: 'user' });
    B(220, 130, 130, 80, 'LB', '/health', { st: 'acc' });
    B(430, 20, 190, 70, 'App-1', 'giri', { st: 'bad' }); cross(645, 55);
    B(430, 240, 190, 70, 'App-2', 'chalu', { st: 'good' });
    B(700, 240, 170, 70, 'Seats DB', 'A1 HELD', { st: 'good', icon: 'db' });
    A(160, 170, 218, 170, C.msg, { n: 1 });
    A(350, 150, 428, 60, C.bad, { n: 2, dash: 1 });
    A(350, 190, 428, 275, C.good, { n: 3 });
    A(620, 275, 698, 275, C.good, { n: 4 });
    L(450, 345, 'hold DB me hai, App me nahi. Koi bhi App le sakti', C.good);
  } else {
    B(20, 30, 180, 80, 'App-1', 'B-42 pay', { st: 'bad' }); cross(60, 140);
    B(20, 240, 180, 80, 'App-2', 'retry B-42', { st: 'good' });
    B(560, 120, 300, 100, 'PSP', 'B-42: kata (ek baar)', { st: 'good' });
    A(200, 55, 558, 140, C.good, { n: 1, at: 0.6 });
    num(100, 140, 2, C.bad);
    A(200, 265, 558, 185, C.acc, { n: 3, at: 0.6 });
    A(558, 210, 202, 295, C.good, { n: 4, at: 0.35 });
    L(450, 345, 'idempotent: do baar nahi katega', C.good);
  }
}
