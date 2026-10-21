// Card 5: 7 baje home page key khatam, 1000 log DB pe (stampede). Ilaaj: lock SET NX EX. Agar: lock wala mar gaya.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 7 baje "home" key ki umar khatam, 1000 log ko MISS, sab ek saath DB pe'],
  [3, GRN, 'ILAAJ', 'ILAAJ: pehla lock leta (SET NX EX 5), sirf wahi DB jaata. Baaki 999 ruk ke cache se'],
  [6, YEL, 'AGAR', 'AGAR lock lene wala hi mar gaya? -> lock ki umar (EX 5), khud khulta, agla le leta'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'log → App', '7 baje 1000 log ek saath home page khol rahe'],
   [C.bad, 'App → Cache', '"home" key ki umar abhi khatam hui, sabko MISS'],
   [C.bad, 'App → DB', 'Sab 1000 ek saath DB pe (stampede), DB dheema, page dheema']],
  [[C.acc, 'App 1 → lock', 'Pehle App ne lock liya (SET lock:home NX EX 5), mil gaya'],
   [C.good, 'App 1 → DB', 'Sirf wahi ek DB gaya, home page ka data laaya'],
   [C.good, 'DB → Cache', 'Laaya hua data cache me bhar diya, key phir se bhari'],
   [C.good, 'baaki → Cache', 'Baaki 999 ko lock nahi mila, 50-100 ms ruke, phir cache se HIT']],
  [[C.bad, 'App 1 → lock', 'App 1 ne lock liya, par DB jaane se pehle hi mar gaya'],
   [C.acc, 'lock', 'Lock ki umar 5 sec thi, khatam hote hi lock khud khul gaya'],
   [C.good, 'App 2 → lock', 'Agle App ne lock liya, ab wahi DB jaayega'],
   [C.good, 'App 2 → DB', 'App 2 DB se laaya, cache bhara, 999 hamesha nahi ruke']],
];
function frame(ph) {
  if (ph === 0) {
    dots(20, 22, 115, C.msg, { gap: 14, cols: 5 }); L(55, 200, '1000 log', C.msg);
    B(170, 120, 130, 60, 'App', '', { icon: 'server' }); A(98, 150, 168, 150, C.msg, { n: 1 });
    B(420, 30, 200, 60, 'Cache', 'home: khaali', { st: 'bad' }); L(740, 60, 'umar khatam: MISS', C.bad);
    A(300, 135, 418, 68, C.bad, { n: 2 });
    B(420, 235, 200, 60, 'DB', '1000 query ek saath', { st: 'bad', icon: 'db' });
    A(300, 160, 418, 248, C.bad); A(300, 170, 418, 265, C.bad, { n: 3 }); A(300, 180, 418, 282, C.bad);
    bar(650, 258, 160, 14, 1, ''); L(730, 300, 'DB dheema (stampede)', C.bad);
  } else if (ph === 1) {
    dots(20, 22, 130, C.msg, { gap: 14, cols: 5 }); L(55, 215, '1000 log', C.msg);
    B(170, 30, 160, 60, 'App 1', 'pehla', { icon: 'server' }); A(98, 140, 168, 70, C.msg);
    B(150, 240, 180, 60, 'baaki App', '999 log', { icon: 'server' }); A(98, 165, 148, 260, C.msg);
    B(420, 140, 210, 60, 'lock:home', 'SET NX EX 5', { st: 'acc', icon: 'lock' });
    A(330, 75, 418, 155, C.acc, { n: 1 });
    B(700, 30, 180, 60, 'DB', 'sirf 1 query', { st: 'good', icon: 'db' }); A(330, 55, 698, 55, C.good, { n: 2 });
    B(700, 240, 180, 60, 'Cache', 'home: bhara', { st: 'good' }); A(790, 92, 790, 238, C.good, { n: 3 });
    A(330, 280, 698, 280, C.good, { n: 4 }); L(510, 318, 'lock nahi mila: 50-100 ms ruke, phir HIT', C.good);
  } else {
    B(140, 30, 190, 60, 'App 1', 'lock leke mara', { st: 'bad', icon: 'server' }); cross(355, 60);
    B(420, 140, 230, 60, 'lock:home', 'EX 5: khud khula', { st: 'acc', icon: 'clock' });
    A(260, 92, 418, 150, C.bad, { n: 1, dash: 1 }); num(680, 170, 2, C.acc);
    B(170, 250, 160, 60, 'App 2', 'agla', { icon: 'server' });
    A(330, 262, 418, 195, C.good, { n: 3 });
    B(720, 250, 160, 60, 'DB', '', { st: 'good', icon: 'db' }); A(330, 290, 718, 290, C.good, { n: 4 });
    L(540, 330, 'lock hamesha atka nahi rehta', C.muted);
  }
}
