// Card 8: 2 saal purana doc, 10 lakh op shuru se -> khaali screen. Ilaaj: har 1000 op pe photo, kholna = photo + baad ke op.
const DUR = 17, FIX = 8;
function setup() {
  wire('M180 175 H380');
  box(40, 140, 140, 70, 'Main', 'doc khola');
  box(380, 120, 210, 100, 'Cassandra', 'op log: 10 lakh op');
  wire('M380 300 L192 195', 'ws');
  box(380, 270, 210, 64, 'Snapshot (photo)', 'har 1000 op', { id: 'sn', stroke: C.acc });
}
function frame(t, fix, tl) {
  vis('sn', fix); vis('ws', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: 2 saal purana doc kholne pe 10 lakh op shuru se ek-ek chale';
    for (let k = 0; k < 34; k++) { const s = 0.3 + k * 0.22;
      dot(path([[s, [368, 175]], [s + 0.6, [192, 175]]], tl > s && tl < s + 0.6 ? tl : -1), C.msg); }
    const f = Math.min(0.14, tl / 50);
    el('rect', { x: 40, y: 245, width: 140, height: 16, rx: 4, fill: 'none', stroke: C.line }, DYN);
    el('rect', { x: 42, y: 247, width: 136 * f, height: 12, rx: 3, fill: C.bad }, DYN);
    T(110, 285, `op chale: ${(f * 10).toFixed(1)} lakh / 10 lakh`, { c: C.fg, size: 12 });
    if (tl >= 2) T(110, 315, 'minuton khaali screen', { c: C.bad, b: 1, size: 14 });
    return;
  }
  CAP.textContent = 'ILAAJ: har 1000 op pe photo (snapshot), background job leta. Kholna = aakhri photo + baad ke op';
  T(700, 300, 'photo: v9,99,000 tak', { c: C.acc, size: 12, b: 1 });
  if (tl > 0.5 && tl < 1.4) dot(path([[0.5, [380, 300]], [1.4, [192, 195]]], tl), C.acc, '', 'photo', C.acc);
  for (let k = 0; k < 6; k++) { const s = 1.6 + k * 0.2;
    dot(path([[s, [368, 175]], [s + 0.5, [192, 175]]], tl > s && tl < s + 0.5 ? tl : -1), C.msg); }
  if (tl > 1.6 && tl < 3.0) T(280, 160, 'baad ke ~1000 op', { c: C.msg, b: 1, size: 12 });
  const f = tl < 1.4 ? 0 : Math.min(1, 0.5 + (tl - 1.4) / 3.2);
  bar(40, 245, 140, 16, f, '', 1);
  if (tl >= 3.0) { T(110, 285, 'doc turant khula', { c: C.good, b: 1, size: 14 });
                   T(110, 307, 'shuru se kabhi nahi', { c: C.good, size: 12 }); }
}
