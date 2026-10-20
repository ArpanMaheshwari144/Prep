// Card 4: app crash, /complete nahi aaya, T-103 hamesha UPLOADING. Ilaaj: sweeper. Agar: do machine pe -> lock.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: video S3 pe pahunch gayi, phir app crash. "/complete" aaya hi nahi, T-103 atka'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har 5 min sweeper: 10 min se haal nahi badla = atka. S3 me bytes hain to dobara line me'],
  [6, YEL, 'AGAR', 'AGAR sweeper do machine pe chal gaya? -> ek hi chale (lock / leader)'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 30, title, { c: C.fg, b: 1, size: 13 });
  B(x + 40, 50, 125, 48, top[0], top[1], { st: top[2] });
  B(x + 40, 165, 125, 58, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 70, 100, x + 70, 163, c); else A(x + 70, 163, x + 70, 100, c); } // khaali lab = teer nahi
  T(x + 140, 135, lab, { c, b: 1, size: 12 });
  T(x + 102, 252, note, { c: C.muted, size: 11 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 150, 70, 'Arpan ka app', 'crash', { st: 'bad' });
    B(420, 30, 200, 70, 'S3', 'T-103 ke bytes', { st: 'good' });
    B(420, 230, 200, 70, 'Upload Svc', '"/complete"?', { st: 'bad' });
    B(700, 230, 170, 70, 'Status DB', 'T-103: UPLOADING', { st: 'bad' });
    A(170, 145, 418, 65, C.good, { n: 1 }); L(280, 85, '5 GB', C.good);
    num(195, 165, 2, C.bad); L(215, 170, 'phir crash', C.bad, 'start');
    A(170, 190, 418, 265, C.bad, { n: 3, dash: 1 }); L(280, 260, '/complete gaya hi nahi', C.bad);
    A(620, 265, 698, 265, C.muted); num(785, 175, 4, C.bad); L(785, 200, 'hamesha atka', C.bad);
    L(450, 345, 'bytes S3 me pade, paisa lag raha, koi dekhta hi nahi', C.bad);
  } else if (ph === 1) {
    panel(0, 'har 5 min', ['Sweeper', 'atki file?', 'acc'], ['Status DB', 'T-103: 10 min se', 'bad'], 'mili', false, C.acc, 'jaanch 3 sec ki hai');
    panel(1, 'S3 dekha', ['Sweeper', 'bytes hain?', ''], ['S3', 'T-103: haan', 'good'], 'poochha', true, C.acc, 'nahi to FAILED');
    panel(2, 'line me', ['Sweeper', 'T-103', ''], ['Kafka', 'T-103 dobara', 'acc'], 'daala', true, C.acc, 'dobara daalna safe');
    panel(3, 'DONE', ['Worker', 'jaancha', 'good'], ['Status DB', 'T-103: DONE', 'good'], 'likha', true, C.good, 'Arpan ko kuch nahi karna');
    L(450, 320, 'koi file hamesha ke liye atak nahi sakti', C.good);
  } else {
    B(20, 40, 170, 70, 'Sweeper-1', 'machine A');
    B(20, 240, 170, 70, 'Sweeper-2', 'machine B');
    B(350, 130, 220, 90, 'Redis lock', 'sweeper: A ke paas', { st: 'acc' });
    B(700, 40, 170, 70, 'Status DB', 'sirf A dhoondhta', { st: 'good' });
    A(190, 85, 348, 145, C.acc, { n: 1 }); L(290, 105, 'lock?', C.acc, 'start');
    A(348, 180, 192, 108, C.good, { n: 2, at: 0.6 }); L(250, 165, 'mila', C.good);
    A(190, 50, 698, 65, C.good, { n: 3, at: 0.8 }); L(560, 45, 'chala', C.good);
    A(190, 255, 348, 195, C.acc, { n: 4 }); L(230, 222, 'lock?', C.acc);
    A(360, 222, 192, 295, C.bad, { n: 5, at: 0.55 }); L(330, 300, 'nahi: is baar skip', C.bad, 'start');
    L(450, 345, 'do bhi chal jaayein to safe, kyunki worker DONE dekh ke skip karta', C.good);
  }
}
