// Card 4: 1 lakh log seat map dekh rahe, sab main DB pe. Ilaaj: map Redis me (5 sec), faisla main DB UPDATE. Agar: map me khaali, bik chuki.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 11 baje 1 lakh log seat map dekh rahe, book koi-koi. Sab main DB pe, booking dheemi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: seat map Redis se (seatmap:show42, ~5 sec umar). Faisla sirf main DB ka UPDATE'],
  [6, YEL, 'AGAR', 'AGAR A1 bik chuki, map me khaali dikhi? -> chalta hai, UPDATE 0 row bolega'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Users → App', '1 lakh log map dekh rahe'], [C.bad, 'App → main DB', 'har map seedha DB se'], [C.bad, 'main DB', 'dekhne walon ne booking bhi dheemi kar di']],
  [[C.msg, 'Users → App', 'map dekhna'], [C.good, 'App → Redis', 'seatmap:show42, 5 sec purana chalega'], [C.acc, 'Arpan → App → main DB', 'Book: UPDATE sirf main DB pe'], [C.acc, 'miss → read replica', 'Redis me nahi to copy se banao']],
  [[C.acc, 'Redis → App → Arpan', 'map me A1 khaali (5 sec purana)'], [C.msg, 'Arpan → App → main DB', 'A1 book: UPDATE'], [C.bad, 'main DB → App', '0 row: A1 bik chuki'], [C.good, 'App → Arpan', '"seat gayi, doosri chuno"']],
];
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
function frame(ph) {
  if (ph === 0) {
    B(20, 120, 190, 80, 'Users', '1 lakh, map', { icon: 'users' });
    B(330, 120, 180, 80, 'App', '');
    B(640, 110, 230, 100, 'main DB', 'map + booking', { st: 'bad', icon: 'db' });
    bar(660, 180, 190, 12, 0.95, '');
    A(210, 160, 328, 160, C.msg, { n: 1 });
    A(510, 160, 638, 160, C.bad, { n: 2 });
    num(755, 250, 3, C.bad); L(755, 280, 'booking bhi dheemi', C.bad);
    L(450, 345, 'dekhne wale bahut, book karne wale kam. Dono ek DB pe', C.bad);
  } else if (ph === 1) {
    B(20, 30, 170, 70, 'Users', 'map', { icon: 'users' });
    B(20, 240, 170, 70, 'Arpan', 'Book', { icon: 'user' });
    B(300, 130, 170, 80, 'App', '', { st: 'good' });
    B(620, 10, 250, 70, 'Redis', 'seatmap:show42', { st: 'good' });
    B(620, 125, 250, 70, 'Read replica', 'map ki copy', { st: 'acc' });
    B(620, 245, 250, 75, 'main DB', 'sirf UPDATE', { st: 'acc', icon: 'db' });
    A(190, 65, 298, 150, C.msg, { n: 1 });
    A(470, 150, 618, 45, C.good, { n: 2 });
    A(190, 268, 298, 205, C.acc, { n: 3 }); A(470, 200, 618, 278, C.acc);
    A(470, 175, 618, 160, C.acc, { n: 4, dash: 1 });
    L(450, 345, 'dekhna cache se, faisla DB se', C.good);
  } else {
    B(20, 130, 170, 90, 'Arpan', 'map dekha', { icon: 'user' });
    B(330, 130, 200, 90, 'App', '', { st: 'good' });
    B(670, 20, 200, 70, 'Redis map', 'A1: khaali', { st: 'acc' });
    B(670, 240, 200, 80, 'main DB', 'A1: Ravi', { st: 'bad', icon: 'db' });
    A(668, 60, 532, 145, C.acc, { n: 1 }); A(328, 150, 192, 150, C.acc);
    A(190, 175, 328, 175, C.msg, { n: 2 }); A(530, 190, 668, 265, C.msg);
    A(668, 300, 532, 210, C.bad, { n: 3 });
    A(328, 205, 192, 205, C.good, { n: 4 });
    L(450, 345, 'map thoda purana chalta. Faisla hamesha UPDATE ka', C.good);
  }
}
