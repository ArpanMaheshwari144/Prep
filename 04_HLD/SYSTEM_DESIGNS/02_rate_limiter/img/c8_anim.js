// Card 8: attack ke waqt poora Redis Cluster gira, ginti kahin nahi. Ilaaj: har gateway apni memory me ginta,
// had 100 / 4 gateway = 25 (local fallback). Agar: had baanti hi nahi -> 4 x 100 = 400.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: attack ke waqt Redis + copy dono gaye. Ginti kahin nahi, sab andar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har gateway apni memory me ginta. Had 100, 4 gateway -> har ek ki had 25'],
  [6, YEL, 'AGAR', 'AGAR had baanti hi nahi, har gateway ne 100 di? -> 4 x 100 = 400 mil gayi'],
];
const GY = [20, 105, 190, 275];
function setup() {}
function front() {
  B(20, 150, 110, 60, 'Attacker', 'key: k1', { st: 'bad' }); B(180, 155, 70, 50, 'LB');
  A(130, 180, 178, 180, C.bad, { n: 2 });
}
function gws(sub, st, c) {
  GY.forEach((y, i) => {
    B(320, y, 170, 60, 'Gateway ' + (i + 1), sub, { st });
    A(250, 180, 318, y + 30, C.msg);
    A(490, y + 30, 648, 185, c);
  });
}
const STEPS = [
  [[C.bad, 'Redis Cluster', 'Attack ke beech Redis main aur copy dono gir gaye'], [C.bad, 'Attacker → LB', 'Attacker key k1 se lagaataar request bhej raha'], [C.bad, 'Gateway x4 → Server', 'Gateway ke paas ginti nahi, to sab request aage jaane di'], [C.bad, 'Server', 'Server ka CPU 100% pe pahuncha aur poori site band ho gayi']],
  [[C.bad, 'Redis Cluster', 'Redis abhi bhi band hai, saanjhi ginti kahin nahi'], [C.bad, 'Attacker → LB', 'Attacker abhi bhi wahi key k1 se request bhej raha'], [C.msg, 'LB → Gateway x4', 'LB ne request 4 gateway me baant di'], [C.good, 'Gateway → Server', 'Har gateway apni memory me ginta, had 100 / 4 = 25, kul 100 hi andar']],
  [[C.acc, 'had baanti nahi', 'Galti se har gateway ko poori had 100 de di gayi'], [C.bad, 'Attacker → LB', 'Attacker ki request 4 gateway me bati'], [C.bad, 'Gateway → Server', 'Har gateway ne 100 andar jaane diye, kul 400, had 4 guna dheeli']],
];
function frame(ph) {
  if (ph === 0) {
    front(); gws('Redis nahi: sab andar', 'bad', C.bad);
    B(660, 20, 180, 50, 'Redis Cluster', 'main + copy', { st: 'bad' }); cross(860, 45); num(750, 90, 1, C.bad); L(770, 95, 'gira', C.bad, 'start');
    B(650, 155, 180, 60, 'Server', 'CPU 100%', { st: 'bad' }); cross(855, 185);
    num(560, 120, 3, C.bad); L(580, 125, 'sab aage', C.bad, 'start');
    num(740, 245, 4, C.bad); L(740, 270, 'site band', C.bad);
    L(450, 350, 'ginti Redis me thi, Redis gaya to had hi gayi', C.bad);
  } else if (ph === 1) {
    front(); gws('apni ginti: had 25', 'good', C.good);
    B(660, 20, 180, 50, 'Redis Cluster', 'abhi bhi gira', { st: 'bad' }); cross(860, 45); num(750, 90, 1, C.bad); L(770, 95, 'gira', C.bad, 'start');
    B(650, 155, 180, 60, 'Server', 'CPU 40%', { st: 'good' });
    num(285, 105, 3, C.msg); L(240, 110, 'LB baante', C.msg, 'end');
    num(560, 120, 4, C.good); L(580, 125, 'har ek se 25', C.good, 'start');
    T(740, 260, '100 / 4 = 25', { c: C.good, b: 1, size: 16 });
    L(740, 290, '25 se upar: wahin 429', C.good);
    L(450, 352, 'thodi galat had, bina had se behtar', C.good);
  } else {
    front(); gws('apni had: 100', 'acc', C.bad);
    B(650, 155, 180, 60, 'Server', 'andar: 400', { st: 'bad' });
    num(560, 120, 3, C.bad); L(580, 125, 'har ek se 100', C.bad, 'start');
    T(740, 260, '4 x 100 = 400', { c: C.bad, b: 1, size: 16 }); L(740, 285, 'had 4 guna dheeli', C.bad);
    num(285, 105, 1, C.acc); L(240, 110, 'had baanti nahi', C.acc, 'end');
    L(450, 352, 'hal: had / N (N = kitne gateway, config se). N badle to dobara baanto', C.good);
  }
}
