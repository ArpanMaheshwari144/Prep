// Card 2: wallet 50k, laptop (TCS 30k) + phone (INFY 30k) ek saath, dono ne 50k dekha, dono lag gaye.
// Ilaaj: check + block ek hi atomic UPDATE. Agar: block kiya, phir order cancel -> block hata do.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: wallet me 50k. Laptop se TCS 30k, phone se INFY 30k, ek saath. Pal-pal dekho'],
  [3, GRN, 'ILAAJ', 'ILAAJ: check + block ek hi query me (atomic UPDATE). Ek jeeta, doosra turant reject'],
  [6, YEL, 'AGAR', 'AGAR block kiya, phir order cancel ho gaya? -> block hata do'],
];
function setup() {}
const STEPS = [
  [[C.acc, 'Wallet → Laptop', 'Laptop ne wallet padha, 50k bacha hai'], [C.acc, 'Wallet → Phone', 'Usi pal phone ne bhi padha, abhi bhi 50k'], [C.bad, 'Laptop → Wallet', 'Laptop ne TCS ke liye 30k block kiya'], [C.bad, 'Phone → Wallet', 'Phone ne INFY ke liye bhi 30k block kiya, 60k jabki tha 50k']],
  [[C.acc, 'Laptop → Wallet DB', 'Laptop ne ek query bheji: UPDATE ... WHERE bacha >= 30k'], [C.good, 'Wallet DB → Laptop', '1 row badli, matlab 30k block ho gaya'], [C.acc, 'Phone → Wallet DB', 'Phone ki wahi query ek pal baad chali'], [C.bad, 'Wallet DB → Phone', 'Ab sirf 20k bacha tha, 0 row badli, order reject']],
  [[C.msg, 'Arpan → Order Svc', 'Arpan ne TCS wala order cancel kar diya'], [C.good, 'Order Svc → Wallet DB', 'Order Svc ne 30k ka block hata diya'], [C.good, 'Wallet DB', 'Arpan ke poore 50k phir se free ho gaye']],
];
function frame(ph) {
  if (ph === 0) {
    const F = [
      ['pal 1', 'laptop padhta', 1, 'acc', 'bacha: 50k', 'padha: 50k', '30k <= 50k, theek'],
      ['pal 2', 'phone padhta', 2, 'acc', 'bacha: 50k', 'padha: 50k', 'abhi block nahi hua'],
      ['pal 3', 'laptop block', 1, 'bad', 'blocked: 30k', 'block +30k', 'TCS laga'],
      ['pal 4', 'phone block', 2, 'bad', 'blocked: 60k', 'block +30k', 'INFY bhi laga'],
    ];
    F.forEach(([pal, kya, who, st, val, l1, l2], k) => {
      const x = 15 + k * 220;
      el('rect', { x, y: 5, width: 205, height: 255, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
      num(x + 20, 25, k + 1, st === 'bad' ? C.bad : C.acc); T(x + 115, 31, kya, { c: C.fg, b: 1, size: 14 });
      B(x + 10, 50, 80, 40, 'Laptop', '', { st: who === 1 ? st : 'dim' });
      B(x + 115, 50, 80, 40, 'Phone', '', { st: who === 2 ? st : 'dim' });
      B(x + 40, 165, 125, 55, 'Wallet', val, { st: 'acc' });
      const ax = who === 1 ? x + 50 : x + 155;
      if (st === 'acc') A(ax + (who === 1 ? 30 : -30), 163, ax, 92, C.acc); else A(ax, 92, ax + (who === 1 ? 30 : -30), 163, C.bad);
      T(who === 1 ? x + 150 : x + 55, 135, l1, { c: st === 'bad' ? C.bad : C.acc, b: 1, size: 14 });
      T(x + 102, 245, l2, { c: C.muted, size: 12.5 });
    });
    L(450, 300, '60k block, jabki wallet me sirf 50k. Paisa minus me', C.bad);
    L(450, 335, 'galti: padhna aur block do alag kadam. Beech me doosra purana 50k padh gaya', C.acc);
  } else if (ph === 1) {
    B(20, 130, 150, 80, 'Laptop', 'TCS 30k', { st: 'good' });
    B(340, 120, 220, 100, 'Wallet DB', 'total 50k, blocked 30k', { st: 'acc' });
    B(730, 130, 150, 80, 'Phone', 'INFY 30k: reject', { st: 'bad' });
    A(170, 150, 338, 150, C.acc, { n: 1 }); L(255, 135, 'UPDATE', C.acc);
    A(338, 195, 172, 195, C.good, { n: 2 }); L(255, 225, '1 row: block hua', C.good);
    A(728, 150, 562, 150, C.acc, { n: 3 }); L(645, 135, 'UPDATE', C.acc);
    A(562, 195, 728, 195, C.bad, { n: 4 }); L(645, 225, '0 row: 20k hi bacha', C.bad);
    T(450, 290, 'SET blocked = blocked + 30k WHERE total - blocked >= 30k', { c: C.fg, size: 13 });
    L(450, 330, '"hai?" aur "block" ek hi kadam. DB ek-ek karke chalata, beech me koi nahi ghusta', C.good);
  } else {
    B(20, 140, 150, 60, 'Arpan', 'TCS order cancel');
    B(280, 140, 160, 60, 'Order Svc', '');
    B(560, 120, 300, 100, 'Wallet DB: blocked', '', { st: 'good' });
    cells(578, 165, [['30k', C.bad], ['->', C.line], ['0', C.good]], { w: 85, h: 32 });
    A(170, 170, 278, 170, C.msg, { n: 1 });
    A(440, 170, 558, 170, C.good, { n: 2 }); L(500, 155, 'block hatao', C.good);
    num(710, 245, 3, C.good); L(710, 270, 'poore 50k phir free', C.good);
    L(450, 330, 'blocked sirf sauda / cancel / umar khatam se ghatta hai', C.good);
  }
}
