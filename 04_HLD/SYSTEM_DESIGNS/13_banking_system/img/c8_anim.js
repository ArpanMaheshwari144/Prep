// Card 8: salary ki bheed me sab balance dekh rahe, transfer dheeme. Ilaaj: read replica. Agar: abhi bheja, copy 5 sec peeche.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: padhna + likhna ek hi DB pe. Laakhon log balance dekh rahe, transfer dheema'],
  [3, GRN, 'ILAAJ', 'ILAAJ: padhna copy se (read replica), likhna sirf primary pe. Primary ka log copy tak'],
  [6, YEL, 'AGAR', 'AGAR abhi paisa bheja, copy 5 sec peeche? -> apna abhi likha primary se padho'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'log → Primary', 'Laakhon log balance / history padh rahe, sab primary pe'],
   [C.msg, 'Transfer → Primary', 'Transfer bhi usi primary pe likhta'],
   [C.bad, 'Primary', 'Padhne ke bojh me transfer dheema ho gaya']],
  [[C.good, 'log → Replica', 'Balance / history copy (read replica) se padhi'],
   [C.good, 'Transfer → Primary', 'Likhna aur paise ka faisla sirf primary pe, tez'],
   [C.acc, 'Primary → Replica', 'Primary ka log (WAL) copy tak jaata, copy thoda peeche chalti']],
  [[C.msg, 'Arpan → Primary', 'Abhi 500 bheja; Redis me last_write_at = abhi likha'],
   [C.acc, 'Arpan → Redis', 'Balance khola: dekha 5 sec ke andar likha tha'],
   [C.good, 'Arpan → Primary', 'To primary se padha: naya balance, purana dekh ke dobara nahi bheja']],
];
function frame(ph) {
  if (ph < 2) {
    dots(20, 18, 40, C.msg, { gap: 13, cols: 5 }); L(90, 115, 'balance dekhne wale', C.msg);
    B(15, 250, 170, 60, 'Transfer', 'likhna', { icon: 'phone' });
  }
  if (ph === 0) {
    B(420, 120, 260, 90, 'DB (primary)', 'padhna + likhna', { st: 'bad', icon: 'db' }); bar(420, 222, 260, 12, 1, '');
    A(90, 65, 418, 140, C.msg, { n: 1 }); A(185, 280, 418, 195, C.msg, { n: 2 });
    num(710, 165, 3, C.bad); L(780, 200, 'transfer dheema', C.bad);
  } else if (ph === 1) {
    B(420, 230, 260, 80, 'DB (primary)', 'likhna + faisla', { st: 'good', icon: 'db' });
    B(420, 30, 260, 80, 'Read replica', 'sirf padhna', { st: 'good', icon: 'db' });
    A(90, 65, 418, 65, C.good, { n: 1 }); A(185, 280, 418, 275, C.good, { n: 2 });
    A(550, 228, 550, 112, C.acc, { n: 3 }); L(640, 170, 'log (WAL)', C.acc); L(800, 70, 'thoda peeche', C.acc);
  } else {
    B(15, 140, 170, 60, 'Arpan', 'abhi 500 bheja', { icon: 'user' });
    B(420, 230, 260, 80, 'DB (primary)', 'naya balance', { st: 'good', icon: 'db' });
    B(420, 30, 260, 80, 'Read replica', '5 sec peeche', { st: 'dim', icon: 'db' });
    B(735, 140, 155, 60, 'Redis', 'last_write', { st: 'acc', icon: 'clock' });
    A(185, 185, 418, 255, C.msg, { n: 1 });
    AP([[100, 138], [100, 120], [812, 120], [812, 138]], C.acc, { n: 2, at: 0.75 });
    A(185, 200, 418, 290, C.good, { n: 3, at: 0.6 }); L(300, 330, 'apna likha primary se', C.good);
    L(300, 70, 'copy se nahi padha', C.muted);
  }
}
