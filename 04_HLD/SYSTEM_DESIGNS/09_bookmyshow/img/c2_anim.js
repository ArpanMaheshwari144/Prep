// Card 2: A1 chuni, pay me 3 min, seat available hi rahi -> Ravi le gaya. Ilaaj: HELD + held_until, Sweeper safai.
const DUR = 18, FIX = 8;
const M = [130, 80], R = [130, 280], AU = [298, 168], AD = [298, 202], OU = [420, 168], OD = [420, 202],
  DU = [568, 168], DD = [568, 202];
function setup() {
  wire('M130 80 L310 168'); wire('M130 280 L310 202'); wire('M420 185 H580');
  wire('M690 280 V230', 'ws');
  box(20, 50, 110, 60, 'Main', 'A1 chuni');
  box(20, 250, 110, 60, 'Ravi');
  box(310, 150, 110, 70, 'App');
  box(580, 120, 230, 110, 'SQL DB', 'seats', { stroke: C.acc });
  box(610, 280, 160, 54, 'Sweeper job', 'purane hold saaf', { id: 'sw' });
}
// ek chakkar: user -> App -> DB (s se), phir wapas (back) user ke paas rest
function go(s, from, au, ou, du, tl, lb, c) {
  if (tl > s && tl < s + 1.2) dot(tl < s + 0.6 ? path([[s, from], [s + 0.6, au]], tl) : path([[s + 0.6, ou], [s + 1.2, du]], tl), c, '', lb, c);
}
function back(s, au, ou, du, rest, tl, lb, c) {
  if (tl > s && tl < s + 1.2) dot(tl < s + 0.6 ? path([[s, du], [s + 0.6, ou]], tl) : path([[s + 0.6, au], [s + 1.2, rest]], tl), c, '', lb, c);
  if (tl >= s + 1.2) dot(rest, c, '', lb, c);
}
function frame(t, fix, tl) {
  vis('sw', fix); vis('ws', fix);
  const clk = ['10:00', '10:01', '10:02', '10:03'][Math.min(3, Math.floor(tl / 1.6))];
  T(890, 30, `samay ${clk}`, { a: 'end', c: C.muted, size: 13, b: 1 });
  go(0.3, M, AU, OU, DU, tl, 'A1 chuni', C.msg);
  if (tl > 1.5 && tl < 5.0) T(140, 40, 'pay chal raha... 3 min', { a: 'start', c: C.acc, size: 12 });
  go(2.0, R, AD, OD, DD, tl, 'A1 book', C.msg);
  go(5.0, M, AU, OU, DU, tl, 'pay ho gaya', C.msg);
  if (!fix) {
    CAP.textContent = 'DIKKAT: maine A1 chuni, pay me 3 min. Seat available hi rahi, beech me Ravi le gaya';
    const st = tl < 3.2 ? ['A1: available', C.good] : ['A1: booked (Ravi)', C.bad];
    T(695, 185, st[0], { c: st[1], b: 1, size: 13 });
    back(6.2, AU, OU, DU, [165, 60], tl, 'A1 gayi', C.bad);
    if (tl > 3.2) back(3.2, AD, OD, DD, [165, 265], tl, 'ticket', C.good);
    if (tl > 7.0) T(890, 340, 'paisa kata, seat Ravi ki', { a: 'end', c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: A1 HELD, held_until = ab + 5 min. Ravi ko held dikhti. Umar khatam = Sweeper chhodta';
  const a1 = tl < 1.5 ? ['A1: available', C.good] : tl < 6.2 ? ['A1: HELD (Main) till 10:05', C.acc] : ['A1: BOOKED (Main)', C.good];
  T(695, 185, a1[0], { c: a1[1], b: 1, size: 13 });
  const a2 = tl < 5.4 ? ['A2: HELD (app gira) till 10:02', C.acc] : ['A2: available', C.good];
  T(695, 210, a2[0], { c: a2[1], size: 12 });
  back(3.2, AD, OD, DD, [165, 265], tl, '0 row: held hai', C.bad);
  back(6.2, AU, OU, DU, [165, 60], tl, 'ticket', C.good);
  if (tl > 4.9 && tl < 5.4) dot(path([[4.9, [690, 280]], [5.4, [690, 232]]], tl), C.acc);
  if (tl > 4.9) T(600, 315, 'A2 umar khatam -> khaali', { a: 'end', c: C.acc, size: 12 });
}
