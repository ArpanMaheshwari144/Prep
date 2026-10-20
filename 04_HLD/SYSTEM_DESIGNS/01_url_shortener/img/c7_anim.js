// Card 7: ek LB gira, koi andar nahi. Ilaaj: 2 LB + Route 53 health check. Agar: purana pata (TTL), region gaya. Wapas.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek hi LB tha, wo gira, koi request andar nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: do LB, aage Route 53. Jo LB 3 baar chup, wo bahar, sab LB-2 pe'],
  [6, YEL, 'AGAR', 'AGAR phone ne purana pata yaad rakha? -> ~60 sec kuch request fail'],
  [9, YEL, 'AGAR', 'AGAR poora region hi chala gaya? -> Route 53 doosre region pe bhejta'],
  [12, GRN, 'WAPAS', 'WAPAS: phone Route 53 se poochta, zinda LB ka pata milta, request andar'],
];
function setup() {}
function base(s1, s2) {
  B(20, 160, 100, 70, 'Phone', '');
  B(200, 20, 140, 60, 'Route 53', '');
  B(430, 110, 110, 60, 'LB-1', '', { st: s1 }); B(430, 250, 110, 60, 'LB-2', '', { st: s2 });
  B(720, 180, 130, 60, 'App x N', '');
}
function frame(ph) {
  if (ph === 0) {
    B(20, 160, 100, 70, 'Phone', ''); B(430, 165, 110, 60, 'LB', 'ek hi', { st: 'bad' }); cross(485, 250);
    B(720, 165, 130, 60, 'App x N', '');
    num(485, 140, 1, C.bad); L(505, 145, 'gira', C.bad, 'start');
    A(120, 195, 428, 195, C.bad, { dash: 1, n: 2 }); L(274, 180, 'koi andar nahi', C.bad);
    A(540, 195, 718, 195, C.line);
  } else if (ph === 1) {
    base('bad', 'good'); cross(565, 140);
    num(605, 140, 1, C.bad); L(625, 145, 'gira', C.bad, 'start');
    A(340, 60, 428, 125, C.acc, { dash: 1, n: 2 }); L(400, 75, '3 baar chup: bahar', C.acc, 'start');
    A(70, 158, 198, 60, C.msg, { n: 3 }); L(115, 95, 'kahan jaun?', C.msg, 'end');
    A(120, 210, 428, 280, C.good, { n: 4 }); L(270, 265, 'sab LB-2 pe', C.good);
    A(540, 280, 718, 220, C.good);
  } else if (ph === 2) {
    base('bad', 'good'); cross(565, 140);
    num(605, 140, 1, C.bad); L(625, 145, 'gira', C.bad, 'start');
    A(120, 180, 428, 145, C.bad, { dash: 1, n: 2 }); L(270, 145, 'purana pata: LB-1', C.bad);
    T(270, 200, '~60 sec fail (DNS TTL)', { c: C.bad, b: 1, size: 13 });
    A(540, 280, 718, 220, C.line);
    L(170, 330, 'hal: LB khud kai zone me (ALB),', C.good, 'start'); L(170, 348, 'har jagah 2+', C.good, 'start');
  } else if (ph === 3) {
    B(20, 160, 100, 70, 'Phone', ''); B(200, 20, 140, 60, 'Route 53', '');
    el('rect', { x: 410, y: 10, width: 460, height: 150, rx: 10, fill: 'none', stroke: C.bad, 'stroke-dasharray': '6 5' }, DYN);
    el('rect', { x: 410, y: 190, width: 460, height: 150, rx: 10, fill: 'none', stroke: C.good, 'stroke-dasharray': '6 5' }, DYN);
    T(640, 32, 'Region-1', { c: C.bad, b: 1, size: 13 }); T(640, 212, 'Region-2', { c: C.good, b: 1, size: 13 });
    B(440, 60, 110, 60, 'LB', '', { st: 'bad' }); B(720, 60, 120, 60, 'App x N', '', { st: 'bad' });
    B(440, 240, 110, 60, 'LB', '', { st: 'good' }); B(720, 240, 120, 60, 'App x N', '', { st: 'good' });
    cross(640, 90); num(600, 130, 1, C.bad); L(620, 135, 'region gaya', C.bad, 'start');
    A(340, 60, 438, 90, C.bad, { dash: 1 });
    A(270, 80, 438, 255, C.acc, { n: 2, at: 0.4 }); L(355, 215, 'doosra region', C.acc, 'end');
    A(120, 210, 438, 285, C.good, { n: 3 }); A(550, 270, 718, 270, C.good);
    L(20, 320, 'hal: Route 53 doosre region pe bhejta', C.good, 'start');
    L(20, 342, 'naye link ke aakhri kuch second kho sakte', C.acc, 'start');
  } else {
    base('dim', 'good');
    A(70, 158, 198, 60, C.msg, { n: 1 }); L(115, 95, 'kahan jaun?', C.msg, 'end');
    AP([[300, 80], [300, 228], [485, 228], [485, 248]], C.good, { dash: 1, n: 2, at: 0.5 }); L(312, 190, 'zinda LB ka pata', C.good, 'start');
    A(120, 215, 428, 290, C.good, { n: 3, at: 0.4 }); A(540, 280, 718, 220, C.good);
    L(200, 290, 'request andar', C.good);
  }
}
