// Card 7: kal raat 2200, subah 1100. Sirf balance column, kahani nahi. Ilaaj: ledger double-entry, sirf INSERT.
// Agar: galat entry / refund -> mitao nahi, nayi ulti line jodo.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine poochha, kal raat 2200 tha, subah 1100 kaise? DB me sirf balance hai'],
  [3, GRN, 'ILAAJ', 'ILAAJ: ledger double-entry. Har paisa do line (DEBIT + CREDIT), jod 0. Sirf INSERT'],
  [6, YEL, 'AGAR', 'AGAR galat entry ho gayi ya refund aaya? -> mitao nahi, nayi ulti line jodo'],
];
function setup() {}
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 150, 70, 'Arpan', '2200 se 1100?');
    B(300, 130, 170, 70, 'Support', '', { st: 'bad' });
    B(640, 130, 220, 70, 'Wallet DB', 'balance: 1100', { st: 'bad' });
    A(170, 150, 298, 150, C.msg, { n: 1 }); L(235, 135, 'kaise?', C.msg);
    A(470, 150, 638, 150, C.acc, { n: 2 }); L(555, 135, 'dikhao', C.acc);
    A(638, 180, 472, 180, C.acc, { n: 3 }); L(555, 205, 'bas 1100', C.acc);
    A(298, 180, 172, 180, C.bad, { n: 4 }); L(235, 205, 'pata nahi', C.bad);
    L(450, 300, 'number hai, kahani nahi. Kahan gaya, kisi ke paas record nahi', C.bad);
  } else if (ph === 1) {
    B(20, 140, 130, 70, 'Arpan', '2200 se 1100?');
    B(220, 140, 140, 70, 'Support', '');
    B(470, 30, 410, 250, 'Ledger (sirf INSERT)', '', { st: 'good' });
    cells(490, 75, [['Arpan -800', C.bad], ['bijli +800', C.good]], { w: 180, h: 32 });
    cells(490, 120, [['Arpan -300', C.bad], ['recharge +300', C.good]], { w: 180, h: 32 });
    T(675, 185, 'har jodi ka jod = 0', { c: C.good, size: 12 });
    A(150, 160, 218, 160, C.msg, { n: 1 });
    A(360, 160, 468, 130, C.acc, { n: 2 }); L(400, 120, 'lines', C.acc);
    A(468, 220, 362, 190, C.good, { n: 3 }); L(420, 225, 'line-line', C.good);
    A(218, 195, 152, 195, C.good, { n: 4 });
    T(675, 240, '2200 - 800 - 300 = 1100', { c: C.fg, b: 1, size: 14 });
    L(450, 330, 'balance ek number, ledger uski poori kahani', C.good);
  } else {
    B(20, 130, 160, 70, 'Payment Svc', 'refund 500', { st: 'acc' });
    B(330, 30, 550, 220, 'Ledger', '', { st: 'good' });
    cells(350, 75, [['Arpan -500', C.muted], ['Kirana +500', C.muted]], { w: 180, h: 32 });
    T(800, 92, 'purani: rehti', { c: C.muted, size: 12 });
    cells(350, 125, [['Kirana -500', C.good], ['Arpan +500', C.good]], { w: 180, h: 32 });
    T(800, 142, 'nayi ulti', { c: C.good, size: 12 });
    A(180, 160, 348, 142, C.good, { n: 1 }); L(255, 135, 'INSERT', C.good);
    A(180, 185, 348, 225, C.bad, { dash: 1 }); cross(275, 205); L(255, 240, 'DELETE / UPDATE nahi', C.bad);
    T(605, 215, 'jod phir bhi 0', { c: C.good, size: 12 });
    L(450, 320, 'audit me dono line dikhti: kya hua, phir kya theek kiya', C.good);
  }
}
