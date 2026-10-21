// Card 3: Netflix auto-debit + UPI ek saath, balance -100. Ilaaj: check + kaatna ek query (atomic UPDATE). Agar: niyam Java me -> raasta chhoot.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 300 the. Netflix 200 + UPI 200, dono ne 300 padha (pass), dono ne kaata: -100'],
  [3, GRN, 'ILAAJ', 'ILAAJ: check aur kaatna ek query: UPDATE ... SET balance = balance - 200 WHERE balance >= 200'],
  [6, YEL, 'AGAR', 'AGAR niyam Java me? -> batch / admin ka raasta chhoot jaata; DB me ho to sab usse guzarte'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Netflix → Account', '9 baje auto-debit ne balance padha: 300, 200 kaat sakte'],
   [C.msg, 'UPI → Account', 'Usi pal mere UPI ne bhi 300 padha, wo bhi pass'],
   [C.bad, 'Account', 'Dono ne 200 kaata: balance -100, paisa bana hi nahi tha']],
  [[C.good, 'Netflix → DB', 'UPDATE ... WHERE balance >= 200: 300 se 100, 1 row badli'],
   [C.acc, 'UPI → DB', 'Row ke taale pe ruka, phir 100 >= 200? nahi, 0 row'],
   [C.bad, 'DB → UPI', '0 row = kuch nahi kata, 422 "paisa kam"; balance kabhi minus nahi']],
  [[C.good, 'App → DB', 'App me Java wala check tha, ye raasta theek'],
   [C.acc, 'Batch → DB', 'Raat ka batch Java se nahi guzarta; niyam sirf Java me hota to minus ho jaata'],
   [C.good, 'DB', 'Niyam DB ki WHERE me hai: batch bhi yahin ruka, har raasta isi se']],
];
function frame(ph) {
  if (ph < 2) {
    B(15, 30, 190, 60, 'Netflix', 'auto-debit 200', { icon: 'server' });
    B(15, 250, 190, 60, 'UPI', 'mera 200', { icon: 'phone' });
  }
  if (ph === 0) {
    B(380, 130, 230, 80, 'Account: Arpan', '300 -> -100', { st: 'bad', icon: 'user' });
    A(205, 60, 378, 150, C.msg, { n: 1 }); L(290, 70, '300 padha', C.msg);
    A(205, 280, 378, 190, C.msg, { n: 2 }); L(290, 280, '300 padha', C.msg);
    num(640, 170, 3, C.bad); L(760, 170, 'minus!', C.bad); L(495, 300, 'dono ne dekha 300, dono ne kaata', C.bad);
  } else if (ph === 1) {
    B(380, 120, 300, 100, 'DB: account', '', { st: 'good', icon: 'db' });
    T(530, 170, 'WHERE balance >= 200', { c: C.acc, b: 1, size: 14 }); T(530, 195, '300 -> 100', { c: C.good, b: 1, size: 14 });
    A(205, 60, 378, 140, C.good, { n: 1 }); L(290, 70, '1 row', C.good);
    A(205, 270, 378, 200, C.acc, { n: 2, at: 0.35 }); L(240, 215, 'line me ruka', C.acc);
    AP([[530, 222], [530, 300], [207, 300]], C.bad, { n: 3, at: 0.4 }); L(530, 330, '0 row: 422 "paisa kam"', C.bad);
  } else {
    B(15, 20, 190, 60, 'App', 'Java check', { icon: 'server' });
    B(15, 140, 190, 60, 'Batch job', 'raat ko', { st: 'acc', icon: 'clock' });
    B(15, 260, 190, 60, 'Admin', 'haath se', { icon: 'user' });
    B(430, 120, 300, 100, 'DB: account', 'WHERE balance >= 200', { st: 'good', icon: 'db' }); L(580, 100, 'batch bhi ruka', C.good);
    A(205, 50, 428, 140, C.good, { n: 1 }); A(205, 170, 428, 170, C.acc, { n: 2 }); L(316, 140, 'Java check yahan nahi', C.acc); A(205, 290, 428, 200, C.muted);
    num(580, 250, 3, C.good); L(580, 285, 'har raasta DB se guzarta', C.good);
  }
}
