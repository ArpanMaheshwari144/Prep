// Card 6: A me likha, C se padha, 404. Ilaaj: quorum 2 + 2. Agar: C giri, farak, A + B giri. Wapas: hinted handoff.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: naya link A me likha, C se padha, wahan abhi nahi pahuncha, 404'],
  [3, GRN, 'ILAAJ', 'ILAAJ: likhna 2 copy me, padhna 2 copy se. A, B me likha, A + C se padha'],
  [6, YEL, 'AGAR', 'AGAR likhte waqt copy C gir gayi? -> 2 me ho gaya to save'],
  [9, YEL, 'AGAR', 'AGAR padhte waqt A aur C me farak nikla? -> wahin C ko theek kar do'],
  [12, YEL, 'AGAR', 'AGAR A aur B dono gir gayi, sirf 1 bachi? -> likhna fail, user ko error'],
  [15, GRN, 'WAPAS', 'WAPAS: C uthi, padosi ke paas rakha note use mil gaya (hinted handoff)'],
];
function setup() {}
const CY3 = [30, 150, 270];
function base(sts, subs) {
  B(20, 150, 100, 70, 'Arpan', 'naya link');
  B(170, 150, 110, 70, 'Likho', '');
  ['A', 'B', 'C'].forEach((n, i) => B(420, CY3[i], 120, 60, `copy ${n}`, subs[i] || '', { st: sts[i] }));
  B(690, 150, 110, 70, 'Padho', '');
}
function w(i, c, n, o = {}) { A(280, 185, 418, CY3[i] + 30, c, { n, at: 0.6, ...o }); }
function r(i, c, n, o = {}) { A(540, CY3[i] + 30, 688, 185, c, { n, at: 0.4, ...o }); }
function frame(ph) {
  if (ph === 0) {
    base(['good', '', 'bad'], ['naya link', '', 'abhi nahi']);
    A(120, 185, 168, 185, C.msg);
    w(0, C.good, 1); L(330, 100, 'likha', C.good, 'end');
    r(2, C.bad, 2); L(640, 290, 'C se padha', C.bad);
    num(745, 250, 3, C.bad); T(745, 290, '404', { c: C.bad, b: 1, size: 24 });
  } else if (ph === 1) {
    base(['good', 'good', ''], ['naya link', 'naya link', 'abhi nahi']);
    A(120, 185, 168, 185, C.msg);
    w(0, C.good, 1); w(1, C.good, 1); L(330, 100, 'likha: 2 me', C.good, 'end');
    r(0, C.msg, 2); r(2, C.msg, 2); L(660, 90, 'padha: 2 se', C.msg);
    num(745, 250, 3, C.good); L(745, 280, 'A me mil gaya', C.good);
  } else if (ph === 2) {
    base(['good', 'good', 'bad'], ['naya link', 'naya link', '']); cross(570, 300);
    num(610, 300, 1, C.bad); L(630, 305, 'giri', C.bad, 'start');
    A(120, 185, 168, 185, C.msg);
    w(0, C.good, 2); w(1, C.good, 2); w(2, C.bad, 0, { dash: 1 });
    L(330, 100, 'likha: 2 me', C.good, 'end');
    T(745, 120, 'save', { c: C.good, b: 1, size: 20 });
    L(745, 255, 'hal: 2 me ho gaya = save', C.good);
  } else if (ph === 3) {
    base(['good', '', 'acc'], ['naya', '', 'purana']);
    r(0, C.msg, 1); r(2, C.msg, 1); L(660, 90, 'A aur C padha', C.msg);
    num(475, 250, 2, C.acc); L(495, 255, 'farak', C.acc, 'start');
    AP([[745, 222], [745, 315], [542, 315]], C.good, { n: 3, at: 0.4 });
    L(720, 345, 'hal: wahin C theek (read repair)', C.good);
  } else if (ph === 4) {
    base(['bad', 'bad', 'good'], ['', '', 'bachi']); cross(570, 60); cross(570, 180);
    num(610, 60, 1, C.bad); L(630, 65, 'giri', C.bad, 'start');
    num(610, 180, 1, C.bad); L(630, 185, 'giri', C.bad, 'start');
    A(120, 185, 168, 185, C.msg);
    w(2, C.acc, 2); A(168, 200, 122, 200, C.bad, { n: 3 }); L(70, 250, 'error', C.bad);
    L(330, 340, 'sirf 1, quorum nahi', C.acc);
    L(720, 300, 'hal: jo done bola', C.good); L(720, 318, 'wo kabhi khota nahi', C.good);
  } else {
    base(['good', 'good', 'good'], ['naya link', 'note: C ka', 'uthi']);
    num(610, 300, 1, C.good); L(630, 305, 'uthi', C.good, 'start');
    AP([[540, 200], [580, 200], [580, 280], [542, 280]], C.acc, { n: 2, at: 0.5 });
    L(600, 250, 'note mila', C.acc, 'start');
  }
}
