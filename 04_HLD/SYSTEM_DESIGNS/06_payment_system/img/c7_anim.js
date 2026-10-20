// Card 7: sirf balance column, 2200 -> 1100, "kaise?" ka jawab nahi. Ilaaj: ledger double-entry, line-line dikhta.
// Agar: galat entry / refund, 3 saal me bada, balance aur khaata alag. Wapas: raat ka check theek.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: sirf balance column, 2200 se 1100. Number hai, kahani nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: ledger double-entry. Har paisa do line, jod 0, sirf INSERT'],
  [6, YEL, 'AGAR', 'AGAR galat entry ho gayi ya refund aaya? -> mitao nahi, nayi ulti line jodo'],
  [9, YEL, 'AGAR', 'AGAR 3 saal me ledger bahut bada? -> mahine ke tukde, purane S3 Glacier me'],
  [12, YEL, 'AGAR', 'AGAR balance column aur khaata alag? -> raat ko jod == balance, alert'],
  [15, GRN, 'WAPAS', 'WAPAS: raat ka check: jod == balance. Hisaab saaf'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 40, 140, 70, 'Arpan', '"1100 kaise?"', { st: o.ar });
  B(20, 240, 140, 70, 'Support', '', { st: o.su });
  B(240, 40, 190, 70, 'wallet', s.wl || 'balance: 1100', { st: o.wl });
  if (!o.noLg) B(500, 40, 380, o.lgh || 250, 'Ledger (sirf INSERT)', '', { st: o.lg || 'acc' });
}
function rows(list, y0) {
  list.forEach(([t, c], i) => T(520, (y0 || 100) + i * 28, t, { a: 'start', c: c || C.fg, size: 13, b: 1 }));
}
const R1 = 'Arpan -800   bijli +800      jod 0', R2 = 'Arpan -300   recharge +300   jod 0';
function frame(ph) {
  if (ph === 0) {
    base({ wl: 'bad', noLg: 1 });
    L(445, 80, 'kal raat 2200, subah 1100', C.muted, 'start');
    A(90, 112, 90, 238, C.msg, { n: 1 }); L(108, 175, 'kaise?', C.msg, 'start');
    A(160, 245, 238, 80, C.acc, { n: 2, at: 0.5 }); L(215, 160, 'kyun?', C.acc, 'start');
    A(335, 112, 162, 290, C.acc, { dash: 1, n: 3, at: 0.5 }); L(265, 215, 'sirf 1100', C.acc, 'start');
    L(450, 330, 'sirf number, record nahi, jawab nahi', C.bad);
  } else if (ph === 1) {
    base({ wl: 'good' });
    rows([[R1], [R2]]);
    A(90, 112, 90, 238, C.msg, { n: 1 }); L(108, 175, 'kaise?', C.msg, 'start');
    A(160, 275, 498, 200, C.acc, { n: 2, at: 0.5 }); L(330, 270, 'line dikhao', C.acc);
    L(690, 200, '2200 - 800 - 300 = 1100', C.good);
    L(690, 230, 'line-line dikhta', C.good);
  } else if (ph === 2) {
    base({ wl: '' });
    rows([[R1, C.bad], [R2], ['bijli -800   Arpan +800      jod 0', C.good]], 130);
    num(530, 100, 1, C.bad); L(548, 105, 'galat entry', C.bad, 'start');
    A(160, 275, 498, 190, C.good, { n: 2, at: 0.5 }); L(330, 260, 'nayi ulti line', C.good);
    L(690, 230, 'audit me dono dikhti', C.good);
    L(690, 330, 'hal: mitao nahi (sirf INSERT), ulti line jodo', C.good);
  } else if (ph === 3) {
    base({ wl: '', lg: 'acc', lgh: 150 });
    num(560, 25, 1, C.bad); L(578, 30, '3 saal, bahut bada', C.bad, 'start');
    cells(520, 110, [['mahina', C.muted], ['mahina', C.muted], ['mahina', C.muted], ['is mahine', C.good]], { w: 80, h: 30 });
    num(700, 172, 2, C.acc); L(718, 177, 'mahine ke tukde', C.acc, 'start');
    B(560, 260, 200, 70, 'S3 Glacier', 'sasti jagah', { st: 'good' });
    A(560, 142, 600, 258, C.good, { n: 3, at: 0.5 }); L(560, 215, 'purane', C.good, 'end');
    L(250, 330, 'hal: kanoon jitna kahe utna rakho', C.good);
  } else if (ph === 4) {
    base({ wl: 'bad', lg: 'bad' }, { wl: 'balance column' });
    rows([[R1], [R2]]);
    B(240, 200, 190, 60, 'raat ka check', 'jod == balance?', { st: 'acc' });
    A(335, 112, 335, 198, C.acc, { n: 1 }); A(498, 200, 432, 220, C.acc, { n: 1 });
    num(335, 285, 2, C.bad); L(355, 290, 'farak mila', C.bad, 'start');
    A(240, 240, 162, 268, C.bad, { n: 3, at: 0.5 }); L(90, 330, 'alert', C.bad);
    L(690, 330, 'hal: farak mila to alert', C.good);
  } else {
    base({ wl: 'good', lg: 'good' }, { wl: 'balance: 1100' });
    rows([[R1], [R2]]);
    B(240, 200, 190, 60, 'raat ka check', 'jod == balance', { st: 'good' });
    A(335, 112, 335, 198, C.good, { n: 1 }); A(498, 200, 432, 220, C.good, { n: 1 });
    num(335, 285, 2, C.good); L(355, 290, 'barabar', C.good, 'start');
    L(690, 230, 'hisaab saaf', C.good);
  }
}
