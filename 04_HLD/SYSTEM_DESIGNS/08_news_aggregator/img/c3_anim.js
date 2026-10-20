// Card 3: 8 baje 40 guna bheed, ek Feed machine. Ilaaj: LB + kai Feed Svc + read replica. Agar: ek key pe 20,000/sec.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 8 baje election result, 20,000 / sec. Ek Feed machine, CPU bhara'],
  [3, GRN, 'ILAAJ', 'ILAAJ: kai Feed Svc, aage LB. Pehle Redis, miss pe DB ki copy (read replica)'],
  [6, YEL, 'AGAR', 'AGAR Redis ki ek key pe 20,000 / sec? -> Redis replica + App me copy'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Users → Feed Svc', '500 se seedha 20,000 / sec'], [C.bad, 'Feed Svc', 'ek hi machine, CPU 100%'], [C.bad, 'Feed Svc → main DB', 'miss bhi main DB pe,|fetcher ki likhai bhi dheemi']],
  [[C.msg, 'Users → LB', '20,000 / sec'], [C.good, 'LB → Feed Svc x3', 'koi bhi machine le, kuch yaad nahi rakhti'], [C.good, 'Feed Svc → Redis', 'pehle Redis'], [C.acc, 'Feed Svc → Read replica', 'miss to copy se.|Main DB sirf likhai ke liye']],
  [[C.good, 'Feed Svc (RAM)', 'har machine me 2 sec ki copy.|Zyadatar jawab yahin se'], [C.acc, 'Feed Svc → Redis main', 'copy purani hui to ek baar laao'], [C.good, 'Feed Svc → Redis replica', 'baaki machine replica se.|Ek key pe sab nahi aate']],
];
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 120, 190, 80, 'Users', '20,000 / sec', { icon: 'users' });
    B(330, 110, 220, 100, 'Feed Svc', 'ek machine', { st: 'bad', icon: 'server' });
    bar(350, 175, 180, 12, 1, '');
    B(680, 120, 190, 80, 'Main DB', 'likhai bhi', { st: 'bad', icon: 'db' });
    A(210, 160, 328, 160, C.msg, { n: 1 });
    num(440, 250, 2, C.bad); L(460, 255, 'CPU 100%', C.bad, 'start');
    A(550, 160, 678, 160, C.bad, { n: 3 });
    L(450, 345, 'bheed ki padhai ne likhai bhi dheemi kar di', C.bad);
  } else if (ph === 1) {
    B(20, 130, 130, 70, 'Users', '', { icon: 'users' });
    B(200, 130, 110, 70, 'LB', '', { st: 'acc' });
    B(370, 20, 170, 60, 'Feed Svc', '', { st: 'good' });
    B(370, 135, 170, 60, 'Feed Svc', '', { st: 'good' });
    B(370, 250, 170, 60, 'Feed Svc', '', { st: 'good' });
    B(640, 10, 230, 70, 'Redis', 'feed:latest', { st: 'good' });
    B(640, 125, 230, 70, 'Read replica', 'copy', { st: 'acc' });
    B(640, 250, 230, 70, 'Main DB', 'sirf likhai', { icon: 'db' });
    A(150, 165, 198, 165, C.msg, { n: 1 });
    A(310, 150, 368, 55, C.good, { n: 2 }); A(310, 165, 368, 165, C.good); A(310, 180, 368, 280, C.good);
    A(540, 155, 638, 50, C.good, { n: 3 });
    A(540, 170, 638, 165, C.acc, { n: 4, dash: 1 });
    A(755, 248, 755, 197, C.muted, { dash: 1 });
    L(450, 345, 'machine badhao, koi bhi request le. Copy 5 sec purani, chalta hai', C.good);
  } else {
    [[20, 'Feed Svc-1'], [130, 'Feed Svc-2'], [240, 'Feed Svc-3']].forEach(([y, n]) => B(20, y, 240, 80, n, 'RAM copy: 2 sec', { st: 'good' }));
    B(640, 30, 230, 80, 'Redis main', 'feed:latest', { st: 'acc' });
    B(640, 230, 230, 80, 'Redis replica', 'copy', { st: 'good' });
    num(300, 120, 1, C.good); L(320, 125, 'pehle apni copy', C.good, 'start');
    A(260, 55, 638, 65, C.acc, { n: 2, at: 0.6 });
    A(260, 170, 638, 260, C.good, { n: 3, at: 0.7 }); A(260, 280, 638, 280, C.good);
    L(450, 345, 'ek garam key ka bojh baant diya: RAM copy + replica', C.good);
  }
}
