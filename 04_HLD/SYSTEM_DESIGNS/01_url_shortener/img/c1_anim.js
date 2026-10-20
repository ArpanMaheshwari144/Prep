// Card 1: viral link, har click DB pe. Ilaaj: Redis cache-aside. Agar: bhara, link badla, Redis gira, copy bhi gayi. Wapas.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: link viral hua, har click DB pe, DB baith gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: click pe pehle Redis dekho. Mila to wahin se, nahi to DB se laao'],
  [6, YEL, 'AGAR', 'AGAR Redis bhar gaya? -> jo link der se nahi khule, unhe nikaalo (LRU)'],
  [9, YEL, 'AGAR', 'AGAR link badla, Redis me purana? -> pehle DB badlo, phir Redis se hatao'],
  [12, YEL, 'AGAR', 'AGAR Redis hi gir gaya? -> copy (cluster + replica) sambhalti hai'],
  [15, YEL, 'AGAR', 'AGAR Redis ki copy bhi gayi? -> 1000 click me sirf 1 DB tak jaata'],
  [18, GRN, 'WAPAS', 'WAPAS: Redis khoyi to DB se wapas ban jaati, garam copy phir taiyaar'],
];
function setup() {}
function base(o = {}) {
  B(20, 150, 110, 70, 'Users', o.us || 'click');
  A(130, 185, 198, 185, C.msg, { n: o.u });
  B(200, 150, 110, 70, 'App');
  if (!o.noR) B(420, 30, 150, 70, 'Redis', o.rs || '', { st: o.rst });
  B(420, 260, 150, 70, 'DB', o.ds || '', { st: o.dst });
}
function copy(st, sub) { B(650, 135, 150, 70, 'Redis copy', sub, { st }); }
function frame(ph) {
  if (ph === 0) {
    base({ u: 1, dst: 'bad', ds: 'CPU', noR: 1 }); L(75, 135, 'link viral', C.msg);
    A(310, 200, 418, 290, C.bad, { n: 2 }); L(350, 278, 'har click DB pe', C.bad, 'end');
    bar(600, 286, 140, 18, 1, ''); num(765, 295, 3, C.bad); L(785, 300, 'baith gaya', C.bad, 'start');
  } else if (ph === 1) {
    base({ rst: 'good', rs: 'garam copy', ds: 'sach yahan', dst: 'good' });
    A(310, 170, 418, 70, C.good, { n: 1 }); L(345, 105, 'mila: wahin se', C.good, 'end');
    T(650, 70, '95% click', { c: C.good, b: 1, size: 15, a: 'start' });
    A(310, 200, 418, 290, C.acc, { n: 2 }); L(350, 278, 'nahi mila: DB se', C.acc, 'end');
    A(495, 258, 495, 102, C.acc, { n: 3 }); L(512, 185, 'Redis me rakho', C.acc, 'start');
  } else if (ph === 2) {
    base({ rst: 'acc', rs: 'bhara' }); A(310, 170, 418, 70, C.msg);
    bar(590, 40, 140, 18, 1, ''); num(755, 49, 1, C.bad); L(775, 54, 'bhar gaya', C.bad, 'start');
    cells(590, 80, [['garam', C.good], ['garam', C.good], ['thanda', C.bad]], { w: 60, h: 26 });
    A(748, 108, 748, 175, C.bad, { n: 2 }); L(733, 146, 'der se nahi khula', C.bad, 'end');
    T(748, 195, 'nikla (LRU)', { c: C.bad, b: 1, size: 13 });
    L(700, 250, 'hal: Redis me sirf garam link', C.good);
  } else if (ph === 3) {
    base({ u: 1, rst: 'bad', rs: 'purana', dst: 'good', ds: 'naya link' }); L(75, 135, 'link badla', C.msg);
    A(310, 200, 418, 290, C.acc, { n: 2 }); L(350, 278, 'pehle DB badlo', C.acc, 'end');
    A(310, 170, 418, 70, C.bad, { dash: 1, n: 3 }); L(345, 105, 'phir Redis se hatao', C.bad, 'end');
    cross(600, 65); L(625, 70, 'purana hata', C.bad, 'start');
    L(700, 200, 'ulta kiya to beech ka click', C.bad); L(700, 220, 'purana link wapas bhar deta', C.bad);
  } else if (ph === 4) {
    base({ rst: 'bad' }); copy('good', 'ab sambhalti');
    cross(600, 65); num(640, 50, 1, C.bad); L(660, 55, 'gira', C.bad, 'start');
    A(310, 180, 648, 170, C.good, { n: 2, at: 0.55 }); L(496, 157, 'copy se mila', C.good);
    L(700, 250, 'hal: copy taiyaar, DB tak nahi', C.good);
  } else if (ph === 5) {
    base({ rst: 'bad', us: '1000 click' }); copy('bad', '');
    cross(600, 65); cross(825, 170); num(610, 118, 1, C.bad); L(625, 123, 'dono gaye', C.bad, 'start');
    A(310, 200, 418, 290, C.acc, { n: 2 }); L(392, 238, 'sirf 1 DB tak', C.acc, 'start');
    dots(5, 215, 250, C.msg, { gap: 20 }); num(190, 250, 3, C.msg);
    L(255, 288, 'baaki usi ka jawab lete', C.msg);
    L(720, 300, 'hal: bojh ho to kuch click mana', C.good); L(720, 320, '(load shedding), DB bachao', C.good);
  } else {
    base({ rst: 'good', rs: 'garam copy', dst: 'good' }); copy('good', 'copy');
    L(495, 20, 'wapas aaya', C.good);
    A(495, 258, 495, 102, C.acc, { n: 1 }); L(512, 185, 'DB se wapas bana', C.acc, 'start');
    A(310, 170, 418, 70, C.good, { n: 2 }); L(345, 105, 'mila: wahin se', C.good, 'end');
  }
}
