// Card 3: limiter App me, 429 wali bhi App tak. Ilaaj: Gateway pe. Agar: sab ek saath lautein, gateway gira.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: limiter App me. 429 wali bhi LB -> App tak ghoom ke aayi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: limiter Gateway pe, sabse aage. Had paar to wahin 429, App tak nahi'],
  [6, YEL, 'AGAR', 'AGAR sab 30 sec baad ek saath aaye? -> Retry-After alag-alag do (28, 33)'],
  [9, YEL, 'AGAR', 'AGAR gateway gir gaya? -> 2+ machine pe, aage LB, darwaza akela nahi'],
  [12, GRN, 'WAPAS', 'WAPAS: gateway wapas aaya, LB phir dono pe bhejta'],
];
function setup() {}
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 100, 80, 'Bot', '10,000/s'); A(120, 180, 198, 180);
    B(200, 155, 80, 50, 'LB'); A(280, 180, 378, 180, C.bad);
    B(380, 130, 130, 100, 'App', 'limiter', { st: 'bad' });
    bar(395, 250, 100, 18, 1, 'bojh');
    B(700, 150, 130, 60, 'Redis'); A(510, 180, 698, 180);
    dots(12, 545, 110, C.bad, { gap: 22, cols: 6 }); T(600, 90, '429', { c: C.bad, b: 1, size: 14 });
  } else if (ph === 1) {
    B(20, 140, 100, 80, 'Bot', '10,000/s'); A(120, 180, 178, 180);
    B(180, 150, 110, 60, 'Gateway', 'limiter', { st: 'acc' }); A(290, 180, 368, 180, C.good);
    dots(12, 140, 250, C.bad, { gap: 22, cols: 6 }); T(195, 310, '429', { c: C.bad, b: 1, size: 14 });
    B(370, 155, 80, 50, 'LB'); A(450, 180, 528, 180, C.good);
    B(530, 130, 130, 100, 'App', '', { st: 'good' }); bar(545, 250, 100, 18, 0.2, 'bojh');
    dots(3, 310, 150, C.good, { gap: 18 });
    B(730, 150, 130, 60, 'Redis'); A(660, 180, 728, 180);
  } else if (ph === 2) {
    const ys = [70, 180, 290], tt = ['28 s', '31 s', '33 s'];
    ys.forEach((y, i) => { B(20, y - 25, 110, 50, `user ${i + 1}`); A(130, y, 448, 180, C.acc);
      T(290, y + (180 - y) * 0.5 - 12, tt[i], { c: C.acc, b: 1, size: 14, halo: 1 }); });
    B(450, 150, 120, 60, 'Gateway', '', { st: 'good' });
    bar(460, 235, 100, 18, 0.3, 'bojh', 1);
    el('line', { x1: 640, y1: 200, x2: 880, y2: 200, stroke: C.line, 'stroke-width': 2 }, DYN);
    [[680, '28'], [760, '31'], [840, '33']].forEach(([x, s]) => { dot([x, 180], C.acc); T(x, 225, s + ' s', { c: C.muted, size: 12 }); });
  } else {
    const down = ph === 3;
    B(20, 150, 100, 60, 'users'); A(120, 180, 198, 180);
    B(200, 155, 80, 50, 'LB');
    B(400, 60, 130, 60, 'Gateway-1', '', { st: down ? 'bad' : 'good' }); if (down) cross(555, 90);
    B(400, 240, 130, 60, 'Gateway-2', '', { st: 'good' });
    A(280, 170, 398, 95, down ? C.bad : C.good, { dash: down });
    A(280, 190, 398, 265, C.good);
    B(680, 150, 120, 60, 'App');
    if (!down) A(530, 90, 678, 170, C.good);
    A(530, 270, 678, 190, C.good);
  }
}
