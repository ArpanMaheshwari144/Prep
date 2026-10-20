// Card 6: 2 baje Redis restart, har miss replica pe -> replica baith gayi. Ilaaj: SET NX EX 5, pehla hi banaye.
// Agar: lock wala mar gaya (EX 5), 100 ms baad bhi khaali (retry, phir replica). Wapas: map Redis me, sab Redis se.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 2 baje Redis restart, cache khaali. Har miss replica pe, baith gayi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: pehla SET NX EX 5 se lock, replica se ek query, map Redis me'],
  [6, YEL, 'AGAR', 'AGAR lock lene wala hi mar gaya? -> EX 5 ki wajah se lock khud hat jaata'],
  [9, YEL, 'AGAR', 'AGAR 100 ms baad bhi Redis khaali? -> 2-3 retry, phir khud replica se'],
  [12, GRN, 'WAPAS', 'WAPAS: map Redis me aa gaya, baaki sab Redis se padhti, replica pe 1 query'],
];
function setup() {}
function core(uSub, uSt, appSt, rSub, rSt, rpSt) {
  T(880, 22, 'samay 2:00', { a: 'end', c: C.muted, size: 13, b: 1 });
  if (uSub !== null) B(20, 150, 110, 80, 'Hazaaron user', uSub, { st: uSt }); B(220, 150, 110, 80, 'App', '', { st: appSt });
  B(450, 30, 180, 70, 'Redis', rSub, { st: rSt }); if (rpSt !== 'none') B(680, 170, 170, 80, 'Read replica', '', { st: rpSt });
}
function rload(f, good, s) { bar(680, 280, 170, 16, f, '', good); T(765, 318, s, { c: good ? C.good : C.bad, size: 12, b: 1 }); }
function frame(ph) {
  if (ph === 0) {
    core('map nahi khulta', 'bad', '', 'khaali', 'bad', 'bad');
    num(650, 40, 1, C.bad); L(670, 45, 'restart', C.bad, 'start');
    A(130, 190, 218, 190, C.msg, { n: 2 });
    A(330, 165, 448, 80, C.acc, { dash: 1, n: 3 }); L(345, 110, 'miss', C.acc);
    A(330, 205, 678, 210, C.bad, { n: 4, at: 0.15 }); dots(9, 420, 230, C.msg, { gap: 26, r: 6 });
    cross(872, 210); L(765, 160, 'replica baith gayi', C.bad);
    rload(1, false, 'replica load');
  } else if (ph === 1) {
    core('', '', 'good', 'seatmap:show42', 'good', '');
    A(300, 148, 448, 55, C.acc, { n: 1, at: 0.35 }); L(300, 85, 'SET NX EX 5: lock', C.acc);
    A(330, 205, 678, 210, C.acc, { n: 2 }); L(505, 235, 'ek query', C.acc);
    A(330, 172, 460, 102, C.good, { n: 3, at: 0.6 }); L(445, 150, 'map rakha', C.good, 'start');
    dots(4, 245, 275, C.msg, { gap: 22 }); num(220, 275, 4, C.acc);
    L(280, 305, 'baaki ruke (50-100 ms), phir Redis se', C.acc);
    rload(0.1, true, 'replica: sirf 1 query');
  } else if (ph === 2) {
    core(null, '', 'bad', 'lock: EX 5', 'acc', 'none');
    A(330, 172, 448, 75, C.acc, { n: 1 }); L(345, 105, 'lock liya', C.acc);
    cross(275, 135); num(305, 128, 2, C.bad); L(275, 250, 'App mar gaya', C.bad);
    num(650, 40, 3, C.good); L(670, 45, '5 sec: lock khud hata', C.good, 'start');
    L(300, 340, 'hal: EX 5 ki wajah se lock khud hat jaata, agla le sakta', C.good);
  } else if (ph === 3) {
    core('dheema', 'acc', '', 'abhi khaali', 'bad', '');
    dots(4, 245, 275, C.msg, { gap: 22 }); num(220, 275, 1, C.acc); L(280, 305, 'ruke 100 ms', C.acc);
    A(330, 165, 448, 80, C.bad, { dash: 1, n: 2 }); L(345, 105, '2-3 retry: khaali', C.bad);
    A(330, 205, 678, 210, C.acc, { n: 3 }); L(505, 235, 'khud replica se', C.acc);
    L(600, 340, 'hal: user ko bas dheema lagega', C.good);
  } else {
    core('map khula', 'good', 'good', 'seatmap:show42', 'good', 'good');
    A(130, 190, 218, 190, C.msg, { n: 1 });
    A(330, 172, 448, 75, C.good, { n: 2 }); L(345, 105, 'Redis se', C.good);
    A(330, 205, 678, 210, C.line);
    rload(0.1, true, 'replica: sirf 1 query');
  }
}
