// Card 3: Suresh ke 10, do thread ne dono ko 10 diye. Ilaaj: ek symbol ek line ek thread. Agar: same daam, 6 hi hain. Wapas: naya seller.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Suresh ke paas 10 the, mujhe aur Mohan dono ko 10 mil gaye'],
  [3, GRN, 'ILAAJ', 'ILAAJ: TCS ke saare order ek line me, ek hi thread. Main pehle, 10 mujhe'],
  [6, YEL, 'AGAR', 'AGAR same daam pe do log aaye? -> jo pehle aaya (chhota number) wo pehle'],
  [9, YEL, 'AGAR', 'AGAR mujhe 10 chahiye, uske paas 6? -> 6 ka sauda, baaki 4 book me'],
  [12, GRN, 'WAPAS', 'WAPAS: naya seller @3000 aaya -> baaki 4 bhi bhare, FILLED'],
];
function setup() {}
// line wala dhaancha: Arpan / Mohan -> line -> TCS thread -> TCS book
function lineSet(aSub, aSt, mSub, mSt, items, w, bookSub, lab3) {
  B(20, 40, 110, 70, 'Arpan', aSub, { st: aSt }); B(20, 240, 110, 70, 'Mohan', mSub, { st: mSt });
  const x0 = 165; cells(x0, 160, items, { w, h: 28 });
  const x1 = x0 + items.length * (w + 4);
  T((x0 + x1) / 2, 150, 'TCS line', { c: C.muted, size: 12 });
  A(130, 75, x0 + 20, 158, C.msg, { n: 1, at: 0.3 }); A(130, 275, x0 + 20, 190, C.msg, { n: 1, at: 0.3 });
  A(x1 + 2, 174, 438, 174, C.good, { n: 2, at: 0.4 });
  B(440, 135, 130, 78, 'TCS thread', 'ek hi', { st: 'good' });
  A(572, 174, 668, 174, C.good, { n: 3, at: 0.3 }); L(625, 162, lab3, C.good);
  B(670, 135, 200, 78, 'TCS book', bookSub);
}
function frame(ph) {
  if (ph === 0) {
    B(20, 40, 110, 70, 'Arpan', 'BUY 10'); B(20, 240, 110, 70, 'Mohan', 'BUY 10');
    A(130, 75, 238, 75, C.msg, { n: 1 }); A(130, 275, 238, 275, C.msg, { n: 1 });
    B(240, 40, 140, 70, 'Thread-1', '', { st: 'bad' }); B(240, 240, 140, 70, 'Thread-2', '', { st: 'bad' });
    B(600, 125, 220, 100, 'TCS book', 'SELL: Suresh 10', { st: 'bad' });
    A(598, 150, 382, 85, C.acc, { dash: 1, n: 2, at: 0.2 }); A(598, 200, 382, 265, C.acc, { dash: 1, n: 2, at: 0.2 });
    A(382, 100, 598, 165, C.bad, { n: 3, at: 0.15 }); A(382, 250, 598, 185, C.bad, { n: 3, at: 0.15 });
    L(470, 80, '"10 hai"'); L(470, 300, '"10 hai"');
    L(440, 170, 'le liye 10', C.bad); L(440, 195, 'le liye 10', C.bad);
    T(710, 268, 'bika: 20', { c: C.bad, b: 1, size: 20 }); T(710, 292, 'tha: 10', { c: C.muted, size: 14 });
  } else if (ph === 1) {
    lineSet('10 mile', 'good', '0, line me', 'acc', [['Mohan', C.acc], ['Arpan', C.good]], 66, 'SELL: 0 bacha', 'Arpan');
    L(505, 125, 'ek-ek karke', C.good);
  } else if (ph === 2) {
    lineSet('BUY @3000', 'good', 'BUY @3000', 'acc', [['#502', C.acc], ['#501', C.good]], 56, 'SELL @3000', '#501');
    L(245, 225, 'dono @3000', C.acc);
    L(450, 330, 'hal: jo pehle aaya (chhota number #501) wo pehle', C.good);
  } else if (ph === 3) {
    B(20, 120, 120, 70, 'Arpan', '6 mile', { st: 'acc' });
    A(140, 155, 238, 155, C.msg, { n: 1, at: 0.3 }); L(195, 143, 'BUY 10', C.msg);
    B(240, 120, 150, 70, 'TCS thread', 'ek hi', { st: 'good' });
    A(390, 155, 518, 155, C.good, { n: 2, at: 0.25 }); L(465, 143, 'sauda 6', C.good);
    B(520, 70, 340, 160, 'TCS book', '');
    T(575, 128, 'SELL', { c: C.muted, size: 12 }); cells(610, 110, [['Suresh 6: bika', C.good]], { w: 150, h: 28 });
    T(575, 188, 'BUY', { c: C.muted, size: 12 }); cells(610, 170, [['Arpan 4: khade', C.acc]], { w: 150, h: 28 });
    L(80, 215, 'PARTIALLY FILLED', C.acc); L(690, 260, 'uske paas 6 hi the', C.acc);
    L(690, 285, '4 ka paisa ruka (blocked)', C.acc);
    L(330, 330, 'hal: 6 ka sauda, baaki 4 book me khade', C.good);
  } else {
    B(20, 120, 120, 70, 'Arpan', '10 mile', { st: 'good' });
    B(240, 120, 150, 70, 'TCS thread', 'ek hi', { st: 'good' });
    B(240, 270, 150, 60, 'Naya seller', 'SELL @3000', { st: 'good' });
    A(315, 268, 315, 192, C.msg, { n: 1, at: 0.5 }); L(375, 240, 'aaya', C.msg);
    A(390, 155, 518, 155, C.good, { n: 2, at: 0.25 }); L(465, 143, 'sauda 4', C.good);
    B(520, 70, 340, 160, 'TCS book', '');
    T(575, 128, 'SELL', { c: C.muted, size: 12 }); cells(610, 110, [['naya: bika', C.good]], { w: 150, h: 28 });
    T(575, 188, 'BUY', { c: C.muted, size: 12 }); cells(610, 170, [['Arpan 4: bhare', C.good]], { w: 150, h: 28 });
    num(80, 215, 3, C.good); L(100, 220, 'FILLED', C.good, 'start');
  }
}
