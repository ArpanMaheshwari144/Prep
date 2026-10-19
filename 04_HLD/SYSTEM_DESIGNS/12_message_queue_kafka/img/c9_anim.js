// Card 9: C2 mara, P4-6 koi nahi padh raha, lag badha. Ilaaj: rebalance (baaki ko baanto) + DLQ.
const DUR = 18, FIX = 8;
const P = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9].map(i => [150, 40 + i * 31]);
const C1 = [540, 80], C3 = [540, 304];
function owner(i, fix, tl) {
  if (i <= 3) return C1; if (i >= 7) return C3;
  if (!fix || tl < 1.5) return null;   // C2 mara
  return i === 6 ? C3 : C1;
}
function setup() {
  P.forEach(([x, y], i) => box(x - 50, y - 12, 100, 24, `P${i}`, '', { r: 4 }));
  box(540, 50, 120, 60, 'C1'); box(540, 162, 120, 60, 'C2'); box(540, 274, 120, 60, 'C3');
  box(760, 290, 110, 50, 'DLQ', '', { id: 'dlq', stroke: C.bad });
}
function frame(t, fix, tl) {
  vis('dlq', fix);
  cross(600, 192);
  P.forEach(([x, y], i) => {
    const o = owner(i, fix, tl);
    if (o) el('path', { d: `M200 ${y} L540 ${o[1]}`, stroke: i >= 4 && i <= 6 ? C.good : C.line, 'stroke-width': 1.2, fill: 'none', 'stroke-dasharray': '4 4' }, DYN);
    else T(215, y + 5, 'koi nahi padh raha', { c: C.bad, size: 11, a: 'start' });
  });
  if (!fix) {
    CAP.textContent = 'DIKKAT: shaam ko C2 mar gaya. P4, P5, P6 kisi ke paas nahi, lag badhta ja raha';
    T(800, 192, `lag: ${Math.round(tl * 3000)}`, { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: mare ke tukde baaki ko (rebalance): P4, P5 C1 ko, P6 C3 ko. Baar-baar fail = DLQ';
  T(800, 192, tl > 2 ? 'lag: 0' : 'lag ghat raha', { c: tl > 2 ? C.good : C.acc, b: 1, size: 15 });
  if (tl > 3) { dot(path([[3, [660, 304]], [3.8, [760, 315]]], tl < 3.8 ? tl : -1), C.bad, '', 'kharab khabar', C.bad);
                if (tl > 3.8) T(815, 350, 'line nahi rukti', { c: C.good, size: 12 }); }
}
