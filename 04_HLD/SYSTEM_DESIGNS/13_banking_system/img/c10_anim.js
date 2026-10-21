// Card 10: CA ka export, page 5000 pe atka. Ilaaj: keyset (ts, id) + index. Agar: ek hi ts pe do line.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: page 5000 = LIMIT 20 OFFSET 1,00,000. DB 1 lakh line padh ke phenkta, bahut dheema'],
  [3, GRN, 'ILAAJ', 'ILAAJ: aakhri line yaad rakho, agla page usse aage (keyset), index se seedha wahan'],
  [6, YEL, 'AGAR', 'AGAR ek hi ts pe do line? -> isliye (ts, id), id barabari todti, na chhoote na dohraye'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'CA → DB', 'Page 5000 maanga: LIMIT 20 OFFSET 1,00,000'],
   [C.bad, 'DB', 'DB ne pehle 1 lakh line padhi aur phenk di'],
   [C.bad, 'DB → CA', 'Sirf 20 line di, par page bahut dheema; aage aur dheema']],
  [[C.acc, 'CA', 'Pichhle page ki aakhri line yaad: (10:05, id 42)'],
   [C.msg, 'CA → DB', 'WHERE (ts, id) < (10:05, 42) LIMIT 20: index se seedha wahan'],
   [C.good, 'DB → CA', 'Sirf 20 line padhi; page 1 ho ya 5000, ek jaisa tez']],
  [[C.acc, 'page 1', 'Page ki aakhri line 10:05, id 42; usi waqt pe id 41 bhi hai'],
   [C.bad, 'sirf ts', 'Sirf ts < 10:05 lete to id 41 chhoot jaati'],
   [C.good, '(ts, id)', 'id barabari todti: (10:05, 41) agle page me, na chhooti na dohrayi']],
];
function frame(ph) {
  B(15, 140, 170, 60, 'CA export', ph === 0 ? 'page 5000' : 'agla page', { icon: 'user' });
  if (ph === 0) {
    B(290, 90, 590, 160, 'DB: khaata', '', { st: 'bad', icon: 'db' });
    cells(340, 150, [['line 1 .. 1,00,000: padh ke phenki', C.bad], ['20 di', C.good]], { w: 260, h: 40 });
    A(185, 160, 288, 160, C.msg, { n: 1 }); num(470, 215, 2, C.bad);
    AP([[730, 252], [730, 320], [100, 320], [100, 202]], C.bad, { n: 3, at: 0.15 }); L(420, 340, 'page jitna aage, utna dheema', C.bad);
  } else if (ph === 1) {
    B(15, 30, 170, 60, 'Yaad', '(10:05, id 42)', { st: 'acc', icon: 'clock' }); num(205, 60, 1, C.acc);
    B(290, 90, 590, 160, 'DB: khaata + index (ts, id)', '', { st: 'good', icon: 'db' });
    cells(340, 150, [['... pichhli line (chhui nahi)'], ['agli 20 (10:05, 41 se)', C.good]], { w: 260, h: 40 });
    AP([[185, 150], [265, 150], [265, 139], [735, 139], [735, 148]], C.msg, { n: 2, at: 0.45 });
    AP([[730, 252], [730, 320], [100, 320], [100, 202]], C.good, { n: 3, at: 0.15 }); L(420, 340, 'sirf 20 padhi', C.good);
  } else {
    T(300, 40, 'ts       id', { c: C.muted, b: 1, size: 13, a: 'start' });
    [['10:06', '43', ''], ['10:05', '42', C.acc], ['10:05', '41', C.good], ['10:04', '40', '']].forEach(([t, id, c], i) =>
      cells(300, 55 + i * 42, [[t, c || undefined], [id, c || undefined]], { w: 100, h: 34 }));
    L(620, 113, 'page 1 ki aakhri', C.acc); num(520, 113, 1, C.acc);
    L(620, 155, 'agle page ki pehli', C.good); num(520, 155, 3, C.good);
    num(300, 260, 2, C.bad); L(500, 262, 'sirf ts < 10:05: id 41 chhoot jaati', C.bad);
    L(500, 300, '(ts, id) < (10:05, 42): 41 aati hai', C.good);
  }
}
