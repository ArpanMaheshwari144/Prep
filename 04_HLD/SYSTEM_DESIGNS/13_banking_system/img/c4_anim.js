// Card 4: mere 500 kahan gaye, kab? Ilaaj: khaata (ledger) usi transaction me, sirf jodo. Agar: galat line likhi -> ulti line jodo.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: sirf balance column tha: 1000 dikhta, 500 kahan / kab gaye, support bole "pata nahi"'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har transfer ki 2 line khaate me, usi transaction me (ledger). Sirf jodo'],
  [6, YEL, 'AGAR', 'AGAR khaate me galat line likh di? -> UPDATE nahi, ulti line jodo, galti bhi dikhe'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Arpan → Support', 'Maine poochha: mere 500 kahan gaye, kab?'],
   [C.bad, 'Support → DB', 'DB me sirf balance column: 1000, raasta koi nahi'],
   [C.bad, 'Support → Arpan', 'Support ne bola "pata nahi", purana hisaab mit chuka']],
  [[C.msg, 'Bank svc → DB', 'Transfer ek transaction me: balance bhi aur khaate ki 2 line bhi'],
   [C.acc, 'Khaata', 'Arpan -500, Ravi +500, waqt 8:00 aur T-77 ke saath; sirf jodna'],
   [C.good, 'Support → Khaata', 'Support ne dekha: 8:00 baje, T-77, Ravi ko 500']],
  [[C.bad, 'Khaata', 'Galti se T-78 me Arpan -50 likh diya'],
   [C.good, 'Khaata', 'UPDATE / delete nahi: nayi ulti line T-79 me Arpan +50 jodi'],
   [C.good, 'Support', 'Galti aur sudhaar dono khaate me dikhte, hisaab barabar']],
];
const row = (y, r, c) => cells(445, y, r.map(x => [x, c]), { w: 105, h: 34 });
function frame(ph) {
  if (ph === 0) {
    B(15, 140, 190, 60, 'Arpan', '500 kahan gaye?', { icon: 'user' });
    B(330, 140, 190, 60, 'Support', '"pata nahi"', { st: 'bad', icon: 'server' });
    B(650, 120, 230, 100, 'DB: account', 'balance = 1000', { st: 'bad', icon: 'db' });
    A(205, 160, 328, 160, C.msg, { n: 1 }); A(520, 165, 648, 165, C.bad, { n: 2 });
    A(328, 190, 207, 190, C.bad, { n: 3 }); L(765, 250, 'jod dikhta, raasta nahi', C.bad);
  } else {
    B(15, 30, 190, 60, ph === 1 ? 'Bank svc' : 'Support', ph === 1 ? 'transfer' : 'khaata dekha', { icon: 'server' });
    B(380, 20, 500, 230, 'Khaata (ledger): sirf jodo', '', { st: 'good', icon: 'db' });
    T(445, 85, 'waqt      transfer   account   amount', { c: C.muted, b: 1, size: 13, a: 'start' });
    if (ph === 1) {
      row(100, ['8:00', 'T-77', 'Arpan', '-500'], C.acc); row(140, ['8:00', 'T-77', 'Ravi', '+500'], C.acc);
      A(205, 60, 378, 60, C.msg, { n: 1 }); num(640, 200, 2, C.acc);
      B(15, 260, 190, 60, 'Support', '8:00, T-77, Ravi', { st: 'good', icon: 'user' });
      AP([[205, 290], [630, 290], [630, 252]], C.good, { n: 3, at: 0.4 }); L(420, 320, 'kab, kisko, kyun: sab ek jagah', C.good);
    } else {
      row(100, ['8:00', 'T-77', 'Arpan', '-500']); row(140, ['9:10', 'T-78', 'Arpan', '-50'], C.bad); row(180, ['9:20', 'T-79', 'Arpan', '+50'], C.good);
      num(415, 157, 1, C.bad); num(415, 197, 2, C.good);
      A(205, 60, 378, 60, C.msg, { n: 3 });
      L(630, 290, 'UPDATE nahi: galti bhi khaate me dikhe', C.good);
    }
  }
}
