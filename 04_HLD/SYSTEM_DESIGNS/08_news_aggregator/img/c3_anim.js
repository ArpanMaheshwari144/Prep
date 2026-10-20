// Card 3: 8 baje 40 guna bheed, ek hi Feed machine, CPU bhar gaya.
// Ilaaj: kai Feed machine + LB, miss pe read replica. Agar: aur badha, replica peeche, ek key garam. Wapas.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek hi Feed machine, 500 se seedha 20,000 / sec, CPU bhar gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: Feed Svc kai machine, aage LB. Redis miss to read replica se'],
  [6, YEL, 'AGAR', 'AGAR 20,000 / sec aur badh gaya? -> aur machine lagao, koi bhi request le'],
  [9, YEL, 'AGAR', 'AGAR read replica peeche reh gayi? -> kuch second, koi nuksaan nahi'],
  [12, YEL, 'AGAR', 'AGAR Redis ki ek key pe 20,000 / sec? -> Redis replica + App me copy'],
  [15, GRN, 'WAPAS', 'WAPAS: bheed sambhal li. Padhai copy pe, likhai main pe'],
];
const SY = [30, 95, 160, 225, 290];
function setup() {}
function users(n) {
  B(20, 145, 110, 60, 'Users', '8 baje');
  A(130, 175, 178, 175, C.msg, { n }); L(152, 135, '20,000 / sec', C.msg);
}
function lb() { B(180, 145, 80, 60, 'LB', '', { st: 'good' }); L(215, 235, 'LB baant deta', C.msg); }
// Feed machine slots (k = slot list), LB se teer
function feeds(ks, o = {}) {
  ks.forEach(k => {
    const nw = (o.nw || []).includes(k);
    B(320, SY[k], 120, 46, 'Feed Svc', nw ? 'nayi' : o.sub, { st: nw ? 'acc' : 'good' });
    A(260, 175, 318, SY[k] + 23, nw ? C.acc : C.msg, { n: o.n, at: 0.7 });
  });
}
function right(o = {}) {
  B(560, 30, 140, 60, 'Redis', o.r, { st: o.rst || 'good' });
  B(560, 250, 140, 70, 'read replica', o.rep, { st: o.repst || 'good' });
  B(750, 30, 130, 60, 'Worker', '');
  B(750, 250, 130, 70, 'Main DB', o.main || 'sirf likhai', { st: 'good' });
  A(815, 90, 815, 248, o.wc || C.line, { n: o.wn }); L(832, 175, 'likhai', o.wc || C.muted, 'start');
  A(748, 285, 702, 285, o.cn ? C.acc : C.line, { dash: 1, n: o.cn });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 145, 110, 60, 'Users', '8 baje'); L(152, 135, '20,000 / sec', C.msg);
    A(130, 175, 318, 183, C.msg, { n: 1, at: 0.3 });
    B(320, 160, 120, 46, 'Feed Svc', 'CPU', { st: 'bad' }); num(460, 160, 2, C.bad);
    L(478, 165, 'CPU bhar gaya', C.bad, 'start');
    bar(320, 230, 120, 18, 1, ''); T(380, 275, '500 -> 20,000 / sec', { c: C.bad, b: 1, size: 14 });
    B(560, 30, 140, 60, 'Redis', '', { st: 'dim' }); B(750, 250, 130, 70, 'Main DB', '', { st: 'dim' });
    T(650, 330, '40 guna bheed', { c: C.bad, b: 1, size: 16 });
  } else if (ph === 1) {
    users(1); lb(); feeds([0, 2, 4], { n: 2 }); right();
    A(440, 53, 558, 60, C.good, { n: 3 }); L(500, 35, 'hit', C.good);
    A(440, 313, 558, 290, C.acc, { n: 4, dash: 1 }); L(500, 330, 'miss', C.acc);
  } else if (ph === 2) {
    users(1); lb(); feeds([0, 1, 2, 3, 4], { n: 2, nw: [1, 3] }); right();
    num(470, 135, 3, C.acc); L(488, 140, 'aur machine lagi', C.acc, 'start');
    L(470, 205, 'hal: Feed Svc kuch yaad nahi rakhti,', C.good, 'start');
    L(470, 225, 'koi bhi request le leti hai', C.good, 'start');
  } else if (ph === 3) {
    users(); lb(); feeds([0, 2, 4]);
    right({ rep: '5 sec peeche', repst: 'acc', main: 'nayi khabar', cn: 1 });
    L(725, 240, 'copy der se', C.acc);
    A(440, 313, 558, 290, C.acc, { n: 2 }); L(500, 330, 'miss', C.acc);
    L(470, 140, 'hal: kuch second ki baat,', C.good, 'start');
    L(470, 160, '5 sec purani khabar se', C.good, 'start');
    L(470, 180, 'koi nuksaan nahi', C.good, 'start');
  } else if (ph === 4) {
    users(); lb(); feeds([0, 2, 4], { sub: 'copy 1-2 sec' }); right({ r: 'feed:latest', rst: 'bad' });
    num(548, 110, 1, C.bad); L(566, 115, 'ek key pe 20,000 / sec', C.bad, 'start');
    B(560, 150, 140, 50, 'Redis replica', '', { st: 'good' });
    A(440, 183, 558, 175, C.good, { n: 2 }); L(500, 225, 'baant li', C.good);
    L(320, 22, 'hal: App me 1-2 sec ki copy, sab ek machine pe nahi', C.good, 'start');
  } else {
    users(1); lb(); feeds([0, 2, 4], { n: 2 }); right({ wc: C.good, wn: 1 });
    A(440, 53, 558, 60, C.good, { n: 3 }); L(500, 35, 'hit', C.good);
    A(440, 313, 558, 290, C.good, { n: 4 }); L(500, 330, 'padhai copy pe', C.good);
    L(650, 140, 'bheed sambhal li', C.good);
  }
}
