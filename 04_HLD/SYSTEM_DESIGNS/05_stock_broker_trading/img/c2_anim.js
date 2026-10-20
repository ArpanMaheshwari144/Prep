// Card 2: wallet 50k, 30k-30k dono lag gaye. Ilaaj: check + block ek UPDATE. Agar: cancel, aadha sauda, cancel + match usi pal. Wapas: umar khatam.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: wallet me 50k, dono server ne 50k padha, 30k-30k dono block'],
  [3, GRN, 'ILAAJ', 'ILAAJ: check aur block ek UPDATE me. Ek jeeta 1 row, doosra 0 row, reject'],
  [6, YEL, 'AGAR', 'AGAR block kiya, phir order cancel ho gaya? -> block hata do'],
  [9, YEL, 'AGAR', 'AGAR block kiya, sauda aadha hua? -> jitna bika utna kata, baaki blocked'],
  [12, YEL, 'AGAR', 'AGAR cancel aur match usi pal? -> jo nahi bika, sirf uska block hatao'],
  [15, GRN, 'WAPAS', 'WAPAS: day order ki umar khatam -> shaam ko block hata, wallet free'],
];
function setup() {}
function wallet(st, sub) { B(600, 125, 240, 100, 'Wallet', sub, { st }); }
function upr(sub, st, s1, s1st, n) {
  B(20, 40, 120, 70, 'Laptop', sub, { st }); B(240, 40, 140, 70, 'Server-1', s1, { st: s1st });
  if (n) A(140, 75, 238, 75, C.msg, { n });
}
function nich(sub, st, s2, s2st, n) {
  B(20, 240, 120, 70, 'Phone', sub, { st }); B(240, 240, 140, 70, 'Server-2', s2, { st: s2st });
  if (n) A(140, 275, 238, 275, C.msg, { n });
}
function frame(ph) {
  if (ph === 0) {
    upr('TCS 30k', '', '', 'bad', 1); nich('INFY 30k', '', '', 'bad', 1);
    wallet('bad', 'total 50k, blocked 60k');
    A(598, 150, 382, 85, C.acc, { dash: 1, n: 2, at: 0.2 }); A(598, 200, 382, 265, C.acc, { dash: 1, n: 2, at: 0.2 });
    A(382, 100, 598, 165, C.bad, { n: 3, at: 0.25 }); A(382, 250, 598, 185, C.bad, { n: 3, at: 0.25 });
    L(470, 80, 'padha: 50k'); L(470, 300, 'padha: 50k');
    L(470, 152, 'block 30k', C.bad); L(470, 205, 'block 30k', C.bad);
    T(720, 268, 'bacha: -10k', { c: C.bad, b: 1, size: 20 });
  } else if (ph === 1) {
    upr('TCS 30k', 'good', '1 row', 'good', 1); nich('reject', 'bad', '0 row', 'bad', 1);
    wallet('good', 'total 50k, blocked 30k');
    L(720, 110, 'DB ek-ek karke chalata', C.acc);
    A(382, 95, 598, 160, C.good, { n: 2, at: 0.15 }); A(382, 255, 598, 190, C.bad, { n: 3, at: 0.15 });
    L(480, 100, 'UPDATE: 1 row', C.good); L(500, 262, '20k bacha: 0 row', C.bad);
    T(720, 268, 'bacha: 20k', { c: C.good, b: 1, size: 20 });
  } else if (ph === 2) {
    B(20, 40, 120, 70, 'Laptop', 'TCS 30k'); B(240, 40, 140, 70, 'Server-1', '');
    A(140, 75, 238, 75, C.acc, { n: 1 }); L(190, 62, 'cancel', C.acc);
    nich('', 'dim', '', 'dim');
    wallet('good', 'total 50k, blocked 0');
    A(382, 90, 598, 160, C.good, { n: 2, at: 0.3 }); L(500, 108, 'block hatao', C.good);
    T(720, 268, 'free: 50k', { c: C.good, b: 1, size: 20 });
  } else if (ph === 3) {
    B(20, 40, 120, 70, 'Laptop', 'TCS 30k'); B(240, 40, 140, 70, 'Server-1', 'order khula', { st: 'acc' });
    B(240, 240, 140, 70, 'Matching', 'TCS', { st: 'acc' });
    A(382, 265, 598, 195, C.acc, { n: 1, at: 0.3 }); L(490, 270, 'aadha bika', C.acc);
    wallet('acc', 'kata + blocked');
    dots(10, 625, 255, [C.good, C.good, C.good, C.good, C.good, C.acc, C.acc, C.acc, C.acc, C.acc], { gap: 20 });
    L(665, 288, 'kata', C.good); L(765, 288, 'blocked', C.acc);
    L(450, 345, 'hal: jitna bika utna kata, baaki blocked, order abhi khula', C.good);
  } else if (ph === 4) {
    B(20, 40, 120, 70, 'Laptop', 'cancel'); B(20, 240, 120, 70, 'Bechne wala', 'SELL TCS');
    B(300, 135, 160, 80, 'TCS book', 'ek haath', { st: 'acc' });
    A(140, 75, 298, 150, C.acc, { n: 1, at: 0.4 }); A(140, 275, 298, 200, C.msg, { n: 1, at: 0.4 });
    L(240, 95, 'cancel', C.acc); L(240, 260, 'match', C.msg);
    num(380, 245, 2, C.acc); L(400, 250, 'ek-ek karke', C.acc, 'start');
    A(462, 175, 598, 175, C.good, { n: 3, at: 0.3 }); L(540, 163, 'hisaab', C.good);
    wallet('good', 'kata + block hata');
    dots(10, 625, 255, [C.good, C.good, C.good, C.good, C.good, C.muted, C.muted, C.muted, C.muted, C.muted], { gap: 20 });
    L(665, 288, 'bika: kata', C.good); L(775, 288, 'block hata', C.muted);
  } else {
    B(20, 40, 120, 70, 'Laptop', 'TCS 30k', { st: 'dim' }); B(240, 40, 140, 70, 'Server-1', 'day order', { st: 'acc' });
    num(395, 30, 1, C.acc); L(415, 35, 'shaam: umar khatam', C.acc, 'start');
    nich('', 'dim', '', 'dim');
    wallet('good', 'total 50k, blocked 0');
    A(382, 95, 598, 160, C.good, { n: 2, at: 0.3 }); L(500, 112, 'block hatao', C.good);
    T(720, 268, 'free: 50k', { c: C.good, b: 1, size: 20 });
  }
}
