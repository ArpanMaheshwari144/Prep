// Card 5: teeno service ek group me -> sirf email ko mila. Ilaaj: har service ka apna group, sabko poori khabar.
const DUR = 16, FIX = 7.5;
const S = [[620, 80], [620, 192], [620, 304]], SN = ['Email', 'Analytics', 'Dashboard'];
function setup() {
  box(60, 155, 180, 74, 'Topic: orders', 'Ravi ka order');
  S.forEach(([x, y], i) => { wire(`M240 192 L${x - 80} ${y}`); box(x - 80, y - 30, 160, 60, SN[i]); });
}
function frame(t, fix, tl) {
  if (!fix) {
    CAP.textContent = 'DIKKAT: teeno service ek hi group me. Group me message ek hi ko, sirf email ko mila';
    el('rect', { x: 520, y: 40, width: 200, height: 300, rx: 10, fill: 'none', stroke: C.bad, 'stroke-dasharray': '6 4' }, DYN);
    T(620, 355, 'ek hi group', { c: C.bad, size: 12 });
    dot(path([[0.5, [240, 192]], [1.5, [540, 90]]], tl < 1.5 ? tl : -1), C.msg);
    if (tl > 1.5) { T(740, 85, 'mila', { c: C.good, b: 1, a: 'start' }); T(740, 197, 'kuch nahi', { c: C.bad, b: 1, a: 'start' });
      T(740, 309, 'kuch nahi', { c: C.bad, b: 1, a: 'start' }); }
    return;
  }
  CAP.textContent = 'ILAAJ: har service ka apna group. Group ke andar baanto, alag group = sabko poori khabar';
  S.forEach(([x, y], i) => {
    el('rect', { x: x - 90, y: y - 40, width: 180, height: 80, rx: 10, fill: 'none', stroke: C.good, 'stroke-dasharray': '6 4' }, DYN);
    const s = 0.5 + i * 0.2;
    dot(path([[s, [240, 192]], [s + 1.0, [x - 80, y + 10]]], tl < s + 1.0 ? tl : -1), C.good);
    if (tl > s + 1.0) T(x + 100, y + 5, 'mila · apna offset', { c: C.good, b: 1, a: 'start', size: 12 });
  });
}
