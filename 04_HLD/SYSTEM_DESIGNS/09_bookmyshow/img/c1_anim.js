// Card 1: Arpan aur Ravi ek second me A1, dono ko mili. Ilaaj: UPDATE ... WHERE available. Agar: 4 seat, A4 gayi -> ROLLBACK.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine aur Ravi ne ek hi second me A1 book ki. Dono ko mil gayi. Pal-pal'],
  [3, GRN, 'ILAAJ', 'ILAAJ: dekhna + book karna ek hi query me (UPDATE ... WHERE available). Jeetta ek'],
  [6, YEL, 'AGAR', 'AGAR 4 seat li, A4 pehle hi ja chuki thi? -> badli rows 4 nahi, ROLLBACK'],
];
function setup() {}
const STEPS = [
  [[C.acc, 'DB → App-1 (Arpan)', 'A1 available padha'], [C.acc, 'DB → App-2 (Ravi)', 'A1 abhi bhi available padha'], [C.bad, 'App-1 → DB', 'A1 booked (Arpan)'], [C.bad, 'App-2 → DB', 'A1 booked (Ravi). Ek seat, do ticket']],
  [[C.acc, 'App-1 → DB', 'UPDATE ... WHERE A1 available'], [C.good, 'DB → App-1', '1 row: A1 Arpan ki'], [C.acc, 'App-2 → DB', 'wahi UPDATE, ek pal baad'], [C.bad, 'DB → App-2', '0 row: Ravi ko "seat gayi"']],
  [[C.acc, 'App → DB', 'A1-A4 ek transaction me UPDATE'], [C.bad, 'DB', 'A4 pehle se booked: sirf 3 row badli'], [C.good, 'App → DB', '3 != 4: ROLLBACK. A1-A3 wapas khaali']],
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
    panel(0, 'Arpan padhta', ['App-1', 'Arpan: A1', 'acc'], ['Seats DB', 'A1: available', ''], 'padha', false, C.acc, 'khaali hai');
    panel(1, 'Ravi padhta', ['App-2', 'Ravi: A1', 'acc'], ['Seats DB', 'A1: available', ''], 'padha', false, C.acc, 'abhi bhi khaali');
    panel(2, 'Arpan likhta', ['App-1', '', 'bad'], ['Seats DB', 'A1: Arpan', 'bad'], 'booked', true, C.bad, 'ticket 1');
    panel(3, 'Ravi likhta', ['App-2', '', 'bad'], ['Seats DB', 'A1: Ravi', 'bad'], 'booked', true, C.bad, 'ticket 2');
    L(450, 320, 'padhna aur likhna alag kadam. Beech me doosra ghus gaya', C.bad);
  } else if (ph === 1) {
    B(20, 100, 190, 80, 'App-1', 'Arpan', { st: 'good' });
    B(690, 100, 190, 80, 'App-2', 'Ravi', { st: 'bad' });
    B(330, 90, 250, 100, 'Seats DB', 'A1: Arpan', { st: 'acc', icon: 'db' });
    A(210, 120, 328, 120, C.acc, { n: 1 }); A(328, 160, 212, 160, C.good, { n: 2 });
    A(688, 120, 582, 120, C.acc, { n: 3 }); A(582, 160, 688, 160, C.bad, { n: 4 });
    T(450, 250, "UPDATE seats SET status = 'booked'", { c: C.fg, size: 14 });
    T(450, 275, "WHERE seat_id = 'A1' AND status = 'available'", { c: C.fg, size: 14 });
    L(450, 330, 'DB ek-ek karke chalata, jeetta ek', C.good);
  } else {
    B(20, 130, 150, 80, 'App', 'A1-A4');
    B(260, 40, 380, 230, 'Ek transaction', '', { st: 'acc' });
    cells(280, 100, [['A1', C.good], ['A2', C.good], ['A3', C.good], ['A4', C.bad]], { w: 80, h: 36 });
    T(450, 180, 'A4 pehle se booked', { c: C.bad, size: 14 });
    T(450, 210, 'badli rows = 3, chahiye 4', { c: C.fg, size: 14 });
    B(720, 110, 150, 120, 'Seats DB', 'A1-A3 khaali', { st: 'good' });
    A(170, 170, 258, 170, C.acc, { n: 1 });
    num(615, 118, 2, C.bad);
    A(640, 170, 718, 170, C.good, { n: 3 }); L(680, 155, 'ROLLBACK', C.good);
    L(450, 345, 'aadhi booking bekaar. Ya chaaron, ya koi nahi', C.good);
  }
}
