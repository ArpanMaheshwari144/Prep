// Card 7: Bangalore se US, 300 ms. Ilaaj: har region me copy, Route 53 paas wala region de.
// Agar: network toota, celeb door, region gira, do jagah likha. Wapas: network juda, copy phir aati.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har request samundar paar US tak. 300 ms, feed atak ke aati'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har region me data ki copy. Route 53 paas wala region (India) deta'],
  [6, YEL, 'AGAR', 'AGAR regions ka network toota? -> feed chalta rahe, chahe thoda purana'],
  [9, YEL, 'AGAR', 'AGAR US ke celeb ka tweet, India se door? -> garam tweet India cache me bhi'],
  [12, YEL, 'AGAR', 'AGAR poora region gira? -> Route 53 zinda region pe palat deta'],
  [15, YEL, 'AGAR', 'AGAR dono region me ek saath likha? -> baad wala jeetega (last write wins)'],
  [18, GRN, 'WAPAS', 'WAPAS: network juda, copy US se phir aati rehti'],
];
function setup() {}
function base(ist, usst, ussub) {
  B(20, 220, 120, 70, 'Main', 'Bangalore');
  B(200, 235, 110, 45, 'Route 53', '', { st: 'acc' });
  B(400, 215, 170, 75, 'India region', 'data ki copy', { st: ist });
  B(690, 30, 180, 75, 'US region', ussub || 'data yahan', { st: usst });
}
// Main -> Route 53 -> India -> wapas Main
function route(c, n1, n2, n3) {
  A(140, 257, 198, 257, C.msg, { n: n1 });
  A(310, 257, 398, 257, c, { n: n2 });
  if (n3 !== undefined) AP([[485, 292], [485, 325], [80, 325], [80, 292]], c, { n: n3, at: 0.3 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 220, 120, 70, 'Main', 'Bangalore', { st: 'bad' });
    B(690, 30, 180, 75, 'US region', 'data yahan');
    A(140, 240, 688, 80, C.msg, { n: 1, at: 0.25 }); L(300, 165, 'request', C.msg);
    A(688, 100, 140, 262, C.bad, { n: 2, at: 0.3 }); L(560, 175, 'jawab', C.bad);
    T(450, 100, '300 ms', { c: C.bad, b: 1, size: 22 }); L(450, 125, 'samundar paar', C.bad);
    T(450, 320, 'har feed, har scroll atak ke', { c: C.bad, b: 1, size: 14 });
  } else if (ph === 1) {
    base('good');
    route(C.good, 1, 2, 3); L(355, 225, 'paas wala', C.good);
    L(280, 345, 'feed India se, tez', C.good);
    A(760, 107, 540, 213, C.acc, { dash: 1, n: 4 }); L(700, 175, 'copy US se aati rehti', C.acc, 'start');
  } else if (ph === 2) {
    base('good');
    A(760, 107, 540, 213, C.bad, { dash: 1 }); cross(650, 160);
    num(690, 185, 1, C.bad); L(710, 190, 'network toota', C.bad, 'start');
    route(C.good, 2, 3, 4); L(280, 345, 'feed chalti, thodi purani', C.acc);
    L(300, 60, 'hal: chalta rahe, chahe thoda purana', C.good);
  } else if (ph === 3) {
    base('', '', 'celeb tweet');
    B(400, 60, 170, 55, 'India cache', 'garam tweet', { st: 'good' });
    A(688, 70, 572, 85, C.acc, { dash: 1, n: 1 }); L(630, 125, 'pehle se copy', C.acc);
    AP([[80, 218], [80, 87], [398, 87]], C.good, { n: 2, at: 0.6 }); L(240, 110, 'paas se', C.good);
    L(730, 330, 'hal: garam tweet India cache me bhi', C.good);
  } else if (ph === 4) {
    base('bad', 'good');
    cross(600, 252); num(640, 252, 1, C.bad); L(660, 257, 'gira', C.bad, 'start');
    A(140, 257, 198, 257, C.msg, { n: 2 });
    A(310, 257, 355, 257, C.bad, { dash: 1 }); cross(372, 257);
    AP([[255, 233], [255, 67], [688, 67]], C.good, { n: 3, at: 0.5 }); L(470, 50, 'palat diya', C.good);
    L(780, 125, 'feed thodi purani', C.acc);
    L(450, 340, 'hal: Route 53 zinda region pe palat deta', C.good);
  } else if (ph === 5) {
    B(60, 200, 180, 70, 'India region', 'likha: pehle', { st: 'acc' });
    B(660, 200, 180, 70, 'US region', 'likha: baad me', { st: 'good' });
    B(360, 40, 180, 70, 'tweet t1', 'US wala', { st: 'good' });
    A(150, 198, 380, 112, C.acc, { n: 1 }); A(750, 198, 520, 112, C.good, { n: 2 });
    L(450, 150, 'baad wala jeeta', C.good);
    L(450, 320, 'hal: baad wala jeetega (last write wins)', C.good);
  } else {
    base('good', 'good');
    A(760, 107, 540, 213, C.good, { dash: 1, n: 1 }); L(700, 175, 'copy phir aane lagi', C.good, 'start');
    route(C.good, 2, 3, 4); L(280, 345, 'feed India se', C.good);
  }
}
