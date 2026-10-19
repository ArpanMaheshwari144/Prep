// Card 2: ek line, ek worker 500/sec, aaye 5,000/sec -> lag. Ilaaj: 10 partition, har tukda apna worker.
const DUR = 17, FIX = 8;
function setup() {
  box(20, 150, 100, 84, 'Orders', '5,000 / sec');
}
function frame(t, fix, tl) {
  if (!fix) {
    CAP.textContent = 'DIKKAT: topic ek line, worker ek (500 / sec). Aaye 5,000 / sec, line badhti ja rahi (lag)';
    el('rect', { x: 200, y: 165, width: 420, height: 54, rx: 8, fill: C.box, stroke: C.line }, DYN);
    T(410, 155, 'ek line', { c: C.muted, size: 12 });
    const q = Math.min(14, Math.floor(tl * 2.2));
    for (let i = 0; i < q; i++) el('circle', { cx: 600 - i * 28, cy: 192, r: 9, fill: C.msg }, DYN);
    el('rect', { x: 680, y: 162, width: 120, height: 60, rx: 8, fill: C.box, stroke: C.line }, DYN);
    T(740, 187, 'Worker', {}); T(740, 205, '500 / sec', { c: C.muted, size: 12 });
    T(410, 260, `lag: ${Math.round(tl * 4500)} message`, { c: C.bad, b: 1, size: 15 });
    T(410, 285, 'Ravi ka email ghanton baad', { c: C.bad, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: topic ke tukde (partition), har tukda alag machine pe, apna worker. 10 x 500 = 5,000';
  for (let i = 0; i < 5; i++) {
    const y = 30 + i * 62;
    el('rect', { x: 200, y, width: 300, height: 44, rx: 8, fill: C.box, stroke: C.line }, DYN);
    T(218, y + 27, `P${i}`, { c: C.muted, size: 12, a: 'start' });
    el('rect', { x: 600, y, width: 110, height: 44, rx: 8, fill: C.box, stroke: C.good }, DYN);
    T(655, y + 27, `Worker ${i}`, { size: 12 });
    for (let k = 0; k < 2; k++) { const ph = ((tl + k * 0.5 + i * 0.13) % 1.0);
      dot(lerp([260, y + 22], [600, y + 22], ph), C.good); }
  }
  T(450, 345, '(10 tukde, yahan 5 dikhaye)', { c: C.muted, size: 12 });
  T(810, 192, 'lag: 0', { c: C.good, b: 1, size: 15 });
}
