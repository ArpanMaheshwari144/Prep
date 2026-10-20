// Card 5: fail pe har second turant dobara, 50 worker ek hi pal me -> company pe hathoda. Ilaaj: backoff + jitter (1, 2, 4, 8), had ke baad DLQ + alert.
const DUR = 18, FIX = 8;
const RW = ['W1', 'W2', 'W3', 'W4', 'W50'], RY = [50, 108, 166, 224, 282];
const px = s => 110 + s * 31.5;
const JIT = [0.1, 0.45, 0.8, 0.3, 0.6], OKAT = [2, 3, 2, 1, 99];
function setup() {
  RW.forEach((r, i) => box(20, RY[i] - 16, 60, 32, r));
  el('path', { d: 'M110 312 H620', stroke: C.line, 'stroke-width': 1.5 });
  for (let s = 0; s <= 16; s += 2) { el('path', { d: `M${px(s)} 308 V316`, stroke: C.line });
    el('text', { x: px(s), y: 330, 'text-anchor': 'middle', 'font-size': 11, fill: C.muted }, ST, `${s}s`); }
  box(670, 40, 190, 70, 'SMS company', 'pareshaan');
  box(690, 250, 160, 60, 'DLQ', '+ alert', { id: 'dlq', stroke: C.bad });
}
function frame(t, fix, tl) {
  vis('dlq', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: fail pe turant dobara. 50 worker har second ek hi pal me, company pe hathoda';
    const cur = Math.min(16, tl * 2.2);
    RY.forEach(y => { for (let s = 0; s <= cur; s++) dot([px(s), y], C.bad); });
    el('path', { d: `M${px(cur)} 30 V305`, stroke: C.acc, 'stroke-width': 1 }, DYN);
    bar(680, 150, 170, 18, 1, 'ek pal me koshish');
    T(765, 195, '50 ek saath', { c: C.bad, b: 1, size: 14 });
    if (tl > 3) T(765, 225, 'kabhi theek nahi', { c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: har baar zyada der (1, 2, 4, 8 sec) + thoda random. Had paar to DLQ + alert';
  const cur = Math.min(16, tl * 1.95);
  const tries = [0, 1, 3, 7, 15];
  let busy = 0;
  RY.forEach((y, i) => {
    tries.forEach((s0, k) => {
      if (k > OKAT[i]) return;
      const s = s0 + (k ? JIT[i] : JIT[i] / 3);
      if (s > cur) return;
      const ok = k === OKAT[i];
      dot([px(s), y], ok ? C.good : C.bad);
      if (cur - s < 0.6) busy++;
      if (i === 4 && k > 0) T((px(s) + px(tries[k - 1] + (k > 1 ? JIT[i] : JIT[i] / 3))) / 2, y - 14,
        ['', '1s', '2s', '4s', '8s'][k], { c: C.acc, size: 11, b: 1 });
    });
  });
  el('path', { d: `M${px(cur)} 30 V305`, stroke: C.acc, 'stroke-width': 1 }, DYN);
  bar(680, 150, 170, 18, Math.max(0.2, busy / 5), 'ek pal me koshish', true);
  T(765, 195, 'pal bikhar gaye', { c: C.good, b: 1, size: 14 });
  if (tl > 8.4) {
    const p = tl < 9.0 ? path([[8.4, [px(15.6), 282]], [9.0, [678, 282]]], tl) : [678, 282];
    dot(p, C.bad);
    if (tl >= 9.0) T(770, 235, 'W50 ki khabar DLQ me', { c: C.bad, b: 1, size: 12 });
  }
}
