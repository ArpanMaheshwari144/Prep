// Card 2: fetcher hi DB me likhta, Source A atka to B se Z line me -> khabar 20 min purani.
// Ilaaj: fetcher sirf laata, Kafka me daalta, worker apni speed se. Agar: 1000 ek saath, commit se pehle crash.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: fetcher hi DB me likhe. Source A atka, B se Z line me khade'],
  [3, GRN, 'ILAAJ', 'ILAAJ: fetcher sirf laata, Kafka me daalta. Worker apni speed se DB + Redis'],
  [6, YEL, 'AGAR', 'AGAR 1000 khabar ek saath Kafka me? -> line lambi, gira kuch nahi'],
  [9, YEL, 'AGAR', 'AGAR likha, Kafka commit se pehle crash? -> DB khud rokta: url pe UNIQUE'],
  [12, GRN, 'WAPAS', 'WAPAS: ek source atke to baaki chalte. Feed Redis se khulti rahi'],
];
function setup() {}
function src(aSt, aSub) {
  B(20, 40, 120, 55, 'Source A', aSub, { st: aSt });
  B(20, 120, 120, 55, 'Source B'); B(20, 200, 120, 55, 'Source Z');
  T(80, 285, '1000 source', { c: C.muted, size: 12 });
}
function kafka(st, cl) {
  B(380, 100, 170, 100, 'Kafka', '10 partition', { st });
  cells(392, 152, [['p0', cl], ['p1', cl], ['..', cl], ['p9', cl]], { w: 34, h: 24 });
}
// Source se Fetcher tak: A (atka ya chalu), B, Z
function pull(aOk, n) {
  if (aOk) A(140, 67, 198, 130, C.msg, { n });
  else A(142, 72, 198, 128, C.bad, { dash: 1 });
  A(140, 147, 198, 150, C.msg, { n }); A(140, 227, 198, 172, C.msg, { n });
}
function frame(ph) {
  if (ph === 0) {
    src('bad', 'atka');
    B(200, 110, 120, 80, 'Fetcher', 'laao + likho', { st: 'bad' });
    B(730, 40, 150, 70, 'DB');
    A(142, 72, 198, 128, C.bad, { n: 1 }); L(186, 95, 'A pe atka', C.bad, 'start');
    A(142, 147, 198, 150, C.line); A(142, 227, 198, 172, C.line);
    dots(3, 150, 268, C.acc, { gap: 20, hollow: 1 }); num(225, 268, 2, C.acc);
    L(245, 273, 'B se Z line me khade', C.acc, 'start');
    A(320, 130, 728, 75, C.bad, { dash: 1, n: 3, at: 0.6 }); L(560, 125, 'der se likha', C.bad);
    T(600, 300, 'feed me khabar 20 min purani', { c: C.bad, b: 1, size: 16 });
  } else if (ph === 1) {
    src('bad', 'atka'); pull(false, 1); L(186, 95, 'A atka', C.bad, 'start');
    B(200, 110, 120, 80, 'Fetcher', 'sirf laana', { st: 'good' });
    kafka('acc', C.good);
    A(320, 150, 378, 150, C.msg, { n: 2 }); L(465, 225, 'key = source_id', C.msg);
    B(610, 110, 120, 80, 'Worker', '', { st: 'good' });
    A(550, 150, 608, 150, C.good, { n: 3 }); L(670, 215, 'apni speed se', C.good);
    B(770, 40, 110, 60, 'DB', '', { st: 'good' }); B(770, 200, 110, 60, 'Redis', '', { st: 'good' });
    A(730, 130, 768, 75, C.good, { n: 4 }); A(730, 170, 768, 225, C.good, { n: 4 }); L(800, 155, 'likha', C.good);
    L(825, 285, 'feed khulti', C.good);
  } else if (ph === 2) {
    src('', ''); pull(true, 1);
    L(255, 30, '1000 ek saath', C.acc); dots(10, 215, 50, C.acc, { gap: 20, cols: 5, r: 6 });
    B(200, 110, 120, 80, 'Fetcher', '', { st: 'acc' });
    kafka('acc', C.acc);
    A(320, 150, 378, 150, C.acc, { n: 2 }); L(465, 225, 'line lambi', C.acc);
    B(610, 110, 120, 80, 'Worker', '', { st: 'good' });
    A(550, 150, 608, 150, C.good, { n: 3 }); L(670, 215, 'apni speed se', C.good);
    B(770, 40, 110, 60, 'DB', '', { st: 'good' }); B(770, 200, 110, 60, 'Redis', '', { st: 'good' });
    A(730, 130, 768, 75, C.good); A(730, 170, 768, 225, C.good);
    L(20, 330, 'hal: Kafka disk pe rakhta hai, worker apni speed se padhta, gira kuch nahi', C.good, 'start');
  } else if (ph === 3) {
    src('dim', ''); B(200, 110, 120, 80, 'Fetcher', '', { st: 'dim' });
    kafka('acc', C.good);
    B(610, 40, 120, 60, 'Worker', '', { st: 'bad' }); B(610, 220, 120, 60, 'naya Worker', '', { st: 'good' });
    B(770, 110, 110, 80, 'DB', 'url UNIQUE', { st: 'good' });
    A(730, 70, 768, 128, C.good, { n: 1 }); L(800, 95, 'likha', C.good, 'start');
    cross(590, 70); num(590, 22, 2, C.bad); L(608, 27, 'commit se pehle crash', C.bad, 'start');
    A(550, 180, 608, 245, C.acc, { n: 3 }); L(470, 245, 'wahi khabar dobara', C.acc);
    A(730, 250, 768, 175, C.acc, { n: 4 }); L(825, 230, 'url pehle se', C.acc);
    T(825, 260, '-> kuch nahi', { c: C.good, b: 1, size: 13 });
    L(20, 330, 'hal: url pe UNIQUE, ON CONFLICT DO NOTHING. Feed me ek hi baar', C.good, 'start');
  } else {
    src('bad', 'atka'); pull(false, 1); L(186, 95, 'A atka', C.bad, 'start');
    L(80, 310, 'baaki chalte', C.good);
    B(200, 110, 120, 80, 'Fetcher', '', { st: 'good' });
    kafka('good', C.good);
    A(320, 150, 378, 150, C.good, { n: 2 });
    B(610, 110, 120, 80, 'Worker', '', { st: 'good' });
    A(550, 150, 608, 150, C.good, { n: 3 });
    B(770, 40, 110, 60, 'DB', '', { st: 'good' }); B(770, 200, 110, 60, 'Redis', '', { st: 'good' });
    A(730, 130, 768, 75, C.good, { n: 4 }); A(730, 170, 768, 225, C.good, { n: 4 }); L(800, 155, 'likha', C.good);
    B(610, 280, 120, 50, 'Users'); A(768, 245, 732, 295, C.good, { n: 5 });
    L(825, 300, 'feed khuli', C.good);
  }
}
