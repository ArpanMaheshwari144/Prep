// Card 5: fail pe turant dobara, 50 worker har second ek saath -> company pe hathoda.
// Ilaaj: backoff + jitter (1, 2, 4, 8 sec), had paar to DLQ + alert. Wapas: DLQ se theek karke main line me.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: fail pe turant dobara. 50 worker har second ek saath, hathoda'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har baar zyada der (1, 2, 4, 8 sec) + thoda random. Had paar: DLQ'],
  [6, GRN, 'WAPAS', 'WAPAS: DLQ ki khabar dashboard pe, theek karke main line me wapas'],
];
const RW = ['W1', 'W2', 'W3', 'W4', 'W50'], RY = [50, 108, 166, 224, 282];
const px = s => 110 + s * 31.5;
const JIT = [0.1, 0.45, 0.8, 0.3, 0.6], OKAT = [2, 3, 2, 1, 99], TRY = [0, 1, 3, 7, 15];
function setup() {}
function axis(dim) {
  RW.forEach((r, i) => B(20, RY[i] - 16, 60, 32, r, '', { st: dim ? 'dim' : '' }));
  el('path', { d: 'M110 312 H620', stroke: C.line, 'stroke-width': 1.5 }, DYN);
  for (let s = 0; s <= 16; s += 2) T(px(s), 330, `${s}s`, { c: C.muted, size: 11 });
}
function frame(ph) {
  if (ph === 0) {
    axis();
    B(670, 40, 190, 70, 'SMS company', 'pareshaan', { st: 'bad' });
    num(655, 75, 1, C.bad);
    RY.forEach(y => { for (let s = 0; s <= 15; s++) el('circle', { cx: px(s), cy: y, r: 6, fill: C.bad }, DYN); });
    num(95, 20, 2, C.bad); L(113, 25, 'har second turant dobara, sab ek hi pal', C.bad, 'start');
    A(630, 170, 690, 112, C.bad, { n: 3, at: 0.5 }); L(700, 150, 'hathoda', C.bad, 'start');
    bar(680, 200, 170, 18, 1, 'ek pal me koshish'); T(765, 245, '50 ek saath', { c: C.bad, b: 1, size: 14 });
    T(765, 270, 'kabhi theek nahi', { c: C.bad, b: 1, size: 13 });
  } else if (ph === 1) {
    axis();
    B(670, 40, 190, 70, 'SMS company', '', { st: 'good' });
    B(690, 250, 160, 60, 'DLQ', '+ alert', { st: 'bad' });
    RY.forEach((y, i) => TRY.forEach((s0, k) => {
      if (k > OKAT[i]) return;
      const s = s0 + (k ? JIT[i] : JIT[i] / 3), ok = k === OKAT[i];
      el('circle', { cx: px(s), cy: y, r: 7, fill: ok ? C.good : C.bad }, DYN);
      if (i === 4 && k > 0) {
        const p = TRY[k - 1] + (k > 1 ? JIT[i] : JIT[i] / 3);
        T((px(s) + px(p)) / 2, y - 12, ['', '1s', '2s', '4s', '8s'][k], { c: C.acc, size: 11, b: 1 });
      }
    }));
    num(95, 20, 1, C.acc); L(113, 25, 'ruk-ruk ke: 1, 2, 4, 8 sec + thoda random', C.acc, 'start');
    A(615, 282, 688, 282, C.bad, { n: 2, at: 0.5 }); L(770, 335, 'had paar -> DLQ', C.bad);
    bar(680, 150, 170, 18, 0.3, 'ek pal me koshish', 1); T(765, 195, 'pal bikhar gaye', { c: C.good, b: 1, size: 14 });
  } else {
    axis(1);
    B(690, 250, 160, 60, 'DLQ', '+ alert', { st: 'acc' });
    B(690, 140, 160, 60, 'Dashboard', 'theek karo', { st: 'good' });
    B(690, 30, 160, 60, 'Main line', '', { st: 'good' });
    A(770, 248, 770, 202, C.acc, { n: 1 }); L(785, 230, 'dikhi', C.acc, 'start');
    A(770, 138, 770, 92, C.good, { n: 2 }); L(785, 120, 'wapas', C.good, 'start');
    AP([[688, 60], [620, 60], [620, 282], [575, 282]], C.good, { n: 3, at: 0.5 });
    el('circle', { cx: 560, cy: 282, r: 7, fill: C.good }, DYN); L(470, 262, 'W50 ki khabar phir chali', C.good);
    T(350, 200, 'khabar phenki nahi', { c: C.good, b: 1, size: 14 });
  }
}
