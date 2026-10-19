// Card 6: Broker A ki disk mari, data gaya. Ilaaj: leader + ISR copy, acks=all, naya leader ISR se.
const DUR = 18, FIX = 8;
const PR = [120, 192];
function setup() {
  box(20, 150, 100, 84, 'Producer');
  wire('M120 192 L340 80');
  box(340, 50, 120, 60, 'Broker A', 'leader');
  box(340, 162, 120, 60, 'Broker B', 'copy', { id: 'bb' }); box(340, 274, 120, 60, 'Broker C', 'copy', { id: 'bc' });
  wire('M400 110 L400 162', 'w1'); wire('M460 80 C530 80 530 304 460 304', 'w2');
}
function frame(t, fix, tl) {
  vis('bb', fix); vis('bc', fix); vis('w1', fix); vis('w2', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: har tukda sirf ek broker pe. Dopahar Broker A ki disk mari, data gaya';
    dot(path([[0.4, PR], [1.2, [335, 85]]], tl < 1.2 ? tl : -1), C.msg, '', 'order #9', C.msg);
    if (tl > 1.2 && tl < 3) T(560, 85, 'producer ko "OK"', { c: C.good, size: 12, a: 'start' });
    if (tl > 3) { cross(400, 80); T(560, 85, 'disk mari: order #9 gaya', { c: C.bad, b: 1, a: 'start' }); }
    return;
  }
  CAP.textContent = 'ILAAJ: leader A + 2 copy (ISR). Saari ISR copy le lein, tab producer ko OK (acks=all)';
  dot(path([[0.4, PR], [1.2, [335, 85]]], tl < 1.2 ? tl : -1), C.msg, '', 'order #9', C.msg);
  if (tl > 1.2 && tl < 1.9) { dot(path([[1.2, [400, 110]], [1.8, [400, 162]]], tl), C.acc);
                              dot(path([[1.2, [500, 120]], [1.9, [500, 265]]], tl), C.acc); }
  if (tl > 1.9) { T(480, 197, 'copy hai', { c: C.good, size: 12, a: 'start' }); T(480, 309, 'copy hai', { c: C.good, size: 12, a: 'start' }); }
  if (tl > 2.1 && tl < 4) T(220, 130, 'ab producer ko "OK"', { c: C.good, b: 1, size: 12 });
  if (tl > 4) { cross(400, 80); T(600, 185, 'A mara -> B naya leader', { c: C.good, b: 1, a: 'start' });
                T(600, 205, 'order #9 bacha', { c: C.good, size: 12, a: 'start' }); }
}
