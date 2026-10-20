// Card 3: 500 kata, jawab me network gaya, app ne dobara bheja -> 500 phir kata. Ilaaj: idempotency key B-42.
const DUR = 18, FIX = 8;
const P = [130, 185], PL = [328, 185], PR = [490, 185], BL = [638, 185], NET = [235, 185];
function setup() {
  wire('M130 185 H340'); wire('M490 185 H650'); wire('M415 220 V270', 'wk');
  box(20, 150, 110, 70, 'Main ka App', 'Pay 500');
  box(340, 150, 150, 70, 'Payment Svc');
  box(650, 150, 160, 70, 'Bank (PSP)', 'mera khata');
  box(340, 270, 150, 64, 'bookingId', '', { id: 'kt', stroke: C.acc });
}
function lost(s, tl, lb) {   // jawab wapas aata, beech me network gaya
  if (tl > s && tl < s + 1.2) dot(tl < s + 0.6 ? path([[s, BL], [s + 0.6, PR]], tl) : path([[s + 0.6, PL], [s + 1.2, NET]], tl), C.good, '', lb, C.good);
  if (tl >= s + 1.2) { cross(NET[0], NET[1]); T(235, 150, 'network gaya', { c: C.bad, b: 1, size: 12 }); }
}
function frame(t, fix, tl) {
  vis('kt', fix); vis('wk', fix);
  let kata = 0;
  if (!fix) {
    CAP.textContent = 'DIKKAT: 500 kata, jawab me network gaya. App ne dobara bheja = naya payment, phir 500 kata';
    if (tl > 0.3 && tl < 1.6) dot(tl < 1.0 ? path([[0.3, P], [1.0, PL]], tl) : path([[1.0, PR], [1.6, BL]], tl), C.msg, '', 'Pay 500', C.msg);
    if (tl >= 1.6) kata = 500;
    lost(1.6, tl, 'ho gaya');
    if (tl > 3.3) T(75, 245, 'jawab nahi -> dobara', { c: C.acc, size: 12 });
    if (tl > 3.8 && tl < 5.1) dot(tl < 4.5 ? path([[3.8, P], [4.5, PL]], tl) : path([[4.5, PR], [5.1, BL]], tl), C.msg, '', 'Pay 500 (naya)', C.msg);
    if (tl >= 5.1) kata = 1000;
    T(730, 255, `kata: ${kata}`, { c: kata > 500 ? C.bad : C.fg, b: 1, size: 15 });
    if (tl > 5.4) T(730, 280, '500 do baar kata', { c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: har pay usi bookingId (B-42) se. Dobara aaya = "ho chuka", purana jawab, paisa ek baar';
  if (tl > 0.3 && tl < 1.0) dot(path([[0.3, P], [1.0, PL]], tl), C.msg, '', 'Pay B-42', C.msg);
  if (tl > 1.0 && tl < 1.4) dot(path([[1.0, [415, 222]], [1.4, [415, 268]]], tl), C.acc);
  if (tl > 1.6 && tl < 2.2) dot(path([[1.6, PR], [2.2, BL]], tl), C.msg, '', 'kaato 500', C.msg);
  if (tl >= 2.2) kata = 500;
  lost(2.2, tl, 'ho gaya');
  const ks = tl < 1.4 ? '' : tl < 2.8 ? ['B-42: PENDING', C.acc] : ['B-42: done', C.good];
  if (ks) T(415, 318, ks[0], { c: ks[1], b: 1, size: 12 });
  if (tl > 4.0 && tl < 4.7) dot(path([[4.0, P], [4.7, PL]], tl), C.msg, '', 'Pay B-42 (dobara)', C.msg);
  if (tl > 4.7 && tl < 5.1) dot(path([[4.7, [415, 222]], [5.1, [415, 268]]], tl), C.acc);
  if (tl > 5.1) T(500, 300, 'pehle se: ho chuka', { a: 'start', c: C.good, b: 1, size: 13 });
  if (tl > 5.4) { const p = path([[5.4, PL], [6.1, [165, 130]]], tl); dot(p, C.good, '', 'ho chuka', C.good); }
  T(730, 255, `kata: ${kata}`, { c: C.good, b: 1, size: 15 });
  if (tl > 5.4) T(730, 280, 'paisa ek baar', { c: C.good, b: 1, size: 13 });
}
