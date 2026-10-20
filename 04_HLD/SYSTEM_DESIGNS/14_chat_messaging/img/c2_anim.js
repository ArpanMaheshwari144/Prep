// Card 2: har server ke map me sirf apne log -> server-1 bola "Ravi offline". Ilaaj: Redis me pata (Ravi -> server-7).
const DUR = 17, FIX = 7.5;
const AR = [120, 180], S1L = [188, 180], S1R = [350, 170], S1B = [275, 220], S7L = [548, 170], S7R = [710, 180],
  RV = [758, 180], RD = [455, 268];
function setup() {
  wire('M120 180 H200'); wire('M710 180 H770');
  wire('M350 170 H560', 'w17');
  wire('M275 220 L400 280', 'wr1'); wire('M510 280 L635 220', 'wr7');
  box(20, 150, 100, 60, 'Arpan');
  box(200, 140, 150, 80, 'Server-1', 'map: Arpan');
  box(560, 140, 150, 80, 'Server-7', 'map: Ravi');
  box(770, 150, 100, 60, 'Ravi');
  box(380, 280, 150, 60, 'Redis', 'Ravi -> server-7', { id: 'rd', stroke: C.acc });
}
function frame(t, fix, tl) {
  vis('rd', fix); vis('w17', fix); vis('wr1', fix); vis('wr7', fix);
  T(820, 135, 'online', { c: C.good, size: 12, b: 1 });
  if (!fix) {
    CAP.textContent = 'DIKKAT: har server ke map me sirf apne log. Ravi server-7 pe, server-1 bola "Ravi offline"';
    dot(path([[0.4, AR], [1.2, S1L]], tl < 2.0 ? tl : -1), C.msg, '', 'hi');
    if (tl > 1.4) T(275, 250, 'apne map me Ravi? nahi', { c: C.muted, size: 12 });
    if (tl > 2.4 && tl < 3.2) dot(path([[2.4, S1L], [3.2, AR]], tl), C.bad);
    if (tl > 3.2) { T(70, 245, '"Ravi offline"', { c: C.bad, b: 1, size: 14 }); }
    if (tl > 4.0) T(450, 330, 'galat: Ravi to juda hai, bas doosre server pe', { c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: Redis me pata (Ravi -> server-7). Server-1 poochta, server-7 ko deta, server-7 Ravi ko';
  dot(path([[0.4, AR], [1.2, S1L]], tl < 1.2 ? tl : -1), C.msg, '', 'hi');
  if (tl > 1.2 && tl < 2.0) dot(path([[1.2, S1B], [2.0, [390, 268]]], tl), C.acc, '', 'Ravi kahan?', C.acc);
  if (tl > 2.0 && tl < 2.8) dot(path([[2.0, [390, 268]], [2.8, S1B]], tl), C.acc, '', 'server-7', C.acc);
  if (tl > 2.8 && tl < 3.8) dot(path([[2.8, S1R], [3.8, S7L]], tl), C.good, '', 'hi', C.good);
  if (tl > 3.8 && tl < 4.6) dot(path([[3.8, S7R], [4.6, RV]], tl), C.good, '', 'hi', C.good);
  if (tl > 4.6) T(820, 240, '"hi" mila', { c: C.good, b: 1, size: 14 });
  if (tl > 5.4) T(700, 320, 'pata ek jagah, sab server dekhte', { c: C.good, size: 13 });
}
