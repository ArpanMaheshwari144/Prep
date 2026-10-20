// Card 4: 1 lakh log seat map main DB se, meri booking dheemi. Ilaaj: map Redis (seatmap:show42, ~5 sec) + replica, faisla main DB.
const DUR = 18, FIX = 8;
const V = [140, 75], M = [140, 280], AL = [218, 180], AR = [340, 180], PL = [688, 240], RL = [438, 72];
function setup() {
  wire('M140 75 L230 165'); wire('M140 280 L230 200'); wire('M340 195 L700 240');
  wire('M340 165 L450 72', 'wr'); wire('M620 72 H710', 'wrr'); wire('M785 200 V104', 'wpr');
  box(20, 40, 120, 70, '1 lakh log', 'map dekh rahe');
  box(20, 250, 120, 60, 'Main', 'Book A1');
  box(230, 145, 110, 70, 'App');
  box(450, 40, 170, 64, 'Redis', 'seatmap:show42', { id: 'rd', stroke: C.acc });
  box(710, 40, 150, 64, 'Read replica', 'copy', { id: 'rp' });
  box(700, 200, 170, 80, 'SQL primary', 'faisla yahin', { stroke: C.acc });
}
function frame(t, fix, tl) {
  ['rd', 'rp', 'wr', 'wrr', 'wpr'].forEach(id => vis(id, fix));
  const end = fix ? RL : PL;
  for (let k = 0; k < 40; k++) {
    const s = k * 0.22, p = path([[s, V], [s + 0.4, AL], [s + 0.5, AR], [s + 1.0, end]], tl);
    if (tl < s + 1.0) dot(p, C.msg);
  }
  if (!fix) {
    CAP.textContent = 'DIKKAT: 11 baje 1 lakh log seat map main DB se padh rahe. Meri booking bhi usi DB pe, dheemi';
    bar(700, 300, 170, 16, Math.min(0.98, tl / 2), '');
    T(785, 335, 'primary load', { c: C.muted, size: 12 });
    if (tl > 2.0) dot(tl < 2.6 ? path([[2.0, M], [2.6, AL]], tl) : path([[2.6, [340, 205]], [7.5, [600, 232]]], tl),
      C.acc, '', 'Book A1 (ruka)', C.acc);
    if (tl > 4.0) T(420, 320, 'dekhne wale booking ko bhi dheema kar rahe', { c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: map Redis se (seatmap:show42, ~5 sec umar), DB tak nahi. Faisla sirf main DB ka UPDATE';
  bar(700, 300, 170, 16, 0.15, '', true);
  T(785, 335, 'primary load', { c: C.muted, size: 12 });
  T(535, 125, 'umar ~5 sec', { c: C.acc, size: 12 });
  if (tl > 0.6 && tl < 1.6) dot(path([[0.6, [620, 72]], [1.1, [698, 72]], [1.6, [620, 72]]], tl), C.acc, '', 'miss', C.acc);
  if (tl > 2.0 && tl < 3.8) {
    const p = tl < 2.6 ? path([[2.0, M], [2.6, AL]], tl) : tl < 3.2 ? path([[2.6, [340, 205]], [3.2, PL]], tl)
      : path([[3.2, PL], [3.8, [340, 205]]], tl);
    dot(p, tl < 3.2 ? C.acc : C.good, '', tl < 3.2 ? 'UPDATE A1' : '1 row', tl < 3.2 ? C.acc : C.good);
  }
  if (tl >= 3.8) dot(path([[3.8, AL], [4.4, [175, 260]]], tl), C.good, '', 'A1 meri, turant', C.good);
}
