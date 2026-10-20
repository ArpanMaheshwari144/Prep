// Card 6: Redis restart, saare miss replica pe, replica baithi. Ilaaj: SET NX lock, pehla hi banaye. Agar: lock wala mara -> EX 5.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 2 baje Redis restart. Saara seat map ek saath replica pe, replica baith gayi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: sirf pehli request lock leti (SET NX EX 5), replica se ek query, map Redis me'],
  [6, YEL, 'AGAR', 'AGAR lock lene wala hi mar gaya? -> EX 5 ki wajah se lock khud hat jaata'],
];
function setup() {}
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
const STEPS = [
  [[C.bad, 'Redis', 'Raat 2 baje Redis restart hua, seat map khaali'], [C.bad, 'App x 1000 → replica', 'Sabko miss mila, 1000 App ek saath replica pe gayi'], [C.bad, 'Read replica', 'Hazaaron query ek saath aayi, replica baith gayi']],
  [[C.acc, 'App-A → Redis', 'App-A ko miss mila, usne lock liya (SET NX EX 5)'], [C.good, 'App-A → replica', 'Sirf App-A ne replica pe ek query chala ke map banaya'], [C.good, 'App-A → Redis', 'App-A ne map wapas Redis me rakh diya'], [C.acc, 'App-B → Redis', 'App-B ko lock nahi mila, 100 ms ruka aur phir Redis se padha']],
  [[C.acc, 'App-A → Redis', 'App-A ne lock liya, 5 sec ki umar ke saath'], [C.bad, 'App-A mara', 'Map banane se pehle hi App-A mar gaya'], [C.good, 'Redis', '5 sec baad lock khud hat gaya (EX)'], [C.good, 'App-B → Redis', 'Ab App-B ko lock mila aur usne map bana diya']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 30, 180, 80, 'Redis', 'restart: khaali', { st: 'bad' });
    [[20, 170], [20, 250]].forEach(([x, y], i) => B(x, y, 180, 60, 'App', 'miss', { st: 'acc' }));
    B(620, 110, 250, 110, 'Read replica', 'baith gayi', { st: 'bad', icon: 'db' });
    bar(640, 190, 210, 12, 1, '');
    num(230, 70, 1, C.bad);
    A(200, 200, 618, 150, C.bad, { n: 2, at: 0.4 }); A(200, 280, 618, 190, C.bad);
    num(745, 260, 3, C.bad); L(765, 265, 'map nahi khulta', C.bad, 'start');
    L(450, 345, 'cache gaya to saara bojh ek saath DB pe', C.bad);
  } else if (ph === 1) {
    B(20, 30, 200, 80, 'App-A', 'pehla miss', { st: 'good' });
    B(20, 240, 200, 80, 'App-B', 'doosra miss', { st: 'acc' });
    B(360, 130, 220, 90, 'Redis', 'lock: A', { st: 'acc' });
    B(690, 20, 180, 80, 'Read replica', '1 query', { icon: 'db' });
    A(220, 60, 358, 142, C.acc, { n: 1, at: 0.45 });
    A(220, 45, 688, 55, C.good, { n: 2, at: 0.75 });
    A(220, 98, 358, 190, C.good, { n: 3, at: 0.5 });
    A(220, 270, 358, 205, C.acc, { n: 4 });
    L(470, 260, 'replica pe 1 query, 1000 nahi', C.good, 'start');
    L(450, 345, 'ek banaye, baaki ruk ke Redis se padhein', C.good);
  } else {
    B(20, 30, 200, 80, 'App-A', 'lock liya, mara', { st: 'bad' }); cross(240, 70);
    B(20, 240, 200, 80, 'App-B', 'thoda ruka', { st: 'good' });
    B(380, 120, 240, 100, 'Redis', 'lock: A (EX 5)', { st: 'acc', icon: 'clock' });
    A(220, 85, 378, 145, C.acc, { n: 1 });
    num(285, 70, 2, C.bad);
    num(500, 255, 3, C.good); L(520, 260, '5 sec: lock khud hata', C.good, 'start');
    A(220, 270, 378, 205, C.good, { n: 4 }); L(250, 225, 'lock mila', C.good);
    L(450, 345, 'umar (EX) na hoti to lock hamesha atka rehta', C.good);
  }
}
