// Card 7: Bangalore se US, 300 ms. Ilaaj: har region me copy, Route 53 paas wala region de.
const DUR = 16, FIX = 7.5;
const ME = [70, 250], R53 = [250, 250], IN = [450, 250], US = [800, 100];
function setup() {
  const fix = grp('fix');
  box(20, 215, 100, 70, 'Main', 'Bangalore');
  box(720, 60, 160, 80, 'US region', 'data yahan');
  wire('M120 240 L720 110', 'wus');
  box(200, 225, 100, 50, 'Route 53', '', { parent: fix, stroke: C.acc }); wire('M120 250 H200', '', fix);
  box(380, 215, 150, 70, 'India region', 'data ki copy', { parent: fix }); wire('M300 250 H380', '', fix);
  wire('M720 120 L530 230', '', fix);
}
function frame(t, fix, tl) {
  vis('fix', fix); vis('wus', !fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: har request samundar paar US tak. 300 ms aana-jaana, feed atak ke aati';
    for (let k = 0; k < 2; k++) { const s = 0.4 + k * 2.6;
      dot(path([[s, [120, 240]], [s + 1.1, [720, 110]], [s + 2.2, [120, 240]]], tl < s + 2.2 ? tl : -1), C.msg); }
    if (tl > 0.4) T(420, 150, `${Math.min(300, Math.round((tl - 0.4) * 140))} ms`, { c: C.bad, b: 1, size: 16 });
    T(420, 330, 'har feed, har scroll', { c: C.bad, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: har region me data ki copy. Route 53 paas wale region (India) ka pata deta';
  for (let k = 0; k < 4; k++) { const s = 0.4 + k * 1.1;
    dot(path([[s, [120, 250]], [s + 0.3, R53], [s + 0.6, [380, 250]], [s + 0.9, [120, 260]]], tl < s + 0.9 ? tl : -1), C.good); }
  T(330, 320, '~30 ms', { c: C.good, b: 1, size: 16 });
  dot(path([[0, [720, 120]], [1.5, [530, 230]]], tl % 1.5), C.acc);
  T(800, 165, 'copy US se aati rehti', { c: C.acc, size: 12 });
}
