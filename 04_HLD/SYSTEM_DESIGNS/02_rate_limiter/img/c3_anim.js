// Card 3: limiter App me, 429 wali bhi App tak aayi. Ilaaj: limiter Gateway pe, sabse aage.
// Agar: sab 30 sec baad ek saath lautein -> Retry-After alag-alag (jitter).
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: limiter App ke andar. Mana wali request bhi LB -> App tak ghoom ke aayi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: limiter Gateway pe, sabse aage. Had paar to wahin 429, App tak nahi'],
  [6, YEL, 'AGAR', 'AGAR sabko Retry-After 30 diya? -> 30 sec baad sab ek saath. Alag-alag do (jitter)'],
];
function setup() {}
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 110, 60, 'Bot', '1000 / sec', { st: 'bad' });
    B(240, 145, 90, 50, 'LB');
    B(450, 130, 170, 80, 'App', 'limiter yahan', { st: 'bad' });
    A(130, 165, 238, 165, C.bad, { n: 1 }); A(330, 170, 448, 170, C.bad, { n: 2 });
    bar(450, 225, 170, 14, 1, ''); T(535, 260, 'CPU 100%', { c: C.bad, b: 1, size: 14 });
    num(640, 170, 3, C.bad); L(655, 175, 'ginti dekhi: 429', C.bad, 'start');
    AP([[535, 128], [535, 80], [75, 80], [75, 138]], C.bad, { n: 4, at: 0.5 }); L(300, 65, '429 (par App tak aa ke)', C.bad);
    L(450, 330, 'mana hone wali request bhi andar aayi, App usi me bhar gaya', C.bad);
  } else if (ph === 1) {
    B(20, 60, 110, 60, 'Bot', '1000 / sec', { st: 'bad' });
    B(20, 240, 110, 60, 'Asli user', '');
    B(240, 140, 150, 70, 'Gateway', 'limiter yahan', { st: 'good' });
    A(130, 80, 290, 138, C.bad, { n: 1 });
    A(238, 175, 80, 122, C.bad, { n: 2, at: 0.5 }); L(150, 190, '429 + Retry-After', C.bad, 'end');
    A(130, 270, 238, 195, C.good, { n: 3 });
    B(490, 150, 90, 50, 'LB'); B(680, 135, 170, 80, 'App', 'sirf asli kaam', { st: 'good' });
    A(390, 175, 488, 175, C.good, { n: 4 }); A(580, 175, 678, 175, C.good);
    bar(680, 230, 170, 14, 0.3, '', 1); T(765, 265, 'CPU 30%', { c: C.good, b: 1, size: 14 });
    L(450, 330, 'jitna aage roko, utna kam kaam bekaar', C.good);
  } else {
    B(20, 150, 140, 60, 'Gateway', '429 bhej raha', { st: 'acc' });
    const ys = [50, 160, 270], ra = ['28', '31', '33'];
    ys.forEach((y, i) => {
      B(330, y, 110, 50, 'Client ' + (i + 1));
      A(160, 180, 328, y + 25, C.acc, { n: i + 1, at: 0.55 });
    });
    L(250, 85, 'Retry-After: 28', C.acc); L(250, 165, 'Retry-After: 31', C.acc); L(250, 290, 'Retry-After: 33', C.acc);
    // time ki line: kab lautte
    el('line', { x1: 520, y1: 180, x2: 860, y2: 180, stroke: C.line, 'stroke-width': 2 }, DYN);
    T(520, 205, '20 sec', { c: C.muted, size: 12 }); T(860, 205, '40 sec', { c: C.muted, size: 12 });
    [28, 31, 33].forEach(s => { const x = 520 + (s - 20) * 17; dot([x, 180], C.good); T(x, 160, s + 's', { c: C.good, size: 12, b: 1 }); });
    num(690, 240, 4, C.good); L(690, 270, 'alag-alag lautte, bheed nahi', C.good);
    L(450, 345, 'sabko 30 dete to 30 sec baad sab ek saath = phir wahi bheed', C.bad);
  }
}
