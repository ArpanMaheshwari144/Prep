// Card 6: balance column aur khaata alag, kisi ne pakda nahi. Ilaaj: khaata sach, column = cache, raat reconciliation. Agar: farak mila.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: bug se column aur khaata alag: khaata 29,800, column 30,300, kisi ne pakda nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: khaata sach, column uska jod (cache). Raat ko job dono milata (reconciliation)'],
  [6, YEL, 'AGAR', 'AGAR milaan me farak mila? -> alert + account roko. Pehle KYUN dhoondho, phir khaata jeetega'],
];
function setup() {}
const STEPS = [
  [[C.good, 'Khaata', 'Khaate ki saari line jodo to 29,800 banta hai'],
   [C.bad, 'Column', 'Ek bug se balance column 30,300 ho gaya'],
   [C.bad, 'App → Column', 'App column dikhata: 500 zyada dikha, milane wala koi nahi']],
  [[C.msg, 'Bank svc → DB', 'Har transfer pe khaate ki line aur column, ek hi transaction me'],
   [C.acc, 'Job → Khaata', 'Raat ko job ne snapshot + baad ki line jodi: 29,800'],
   [C.good, 'Job → Column', 'Column bhi 29,800: jod barabar, sab theek (reconciliation)']],
  [[C.bad, 'Job', 'Raat ke milaan me farak mila: 29,800 vs 30,300'],
   [C.acc, 'Job → Alert', 'Alert gaya aur account roka, aage paisa na hile'],
   [C.good, 'Team', 'Pehle KYUN dhoondha (bug), phir khaata jeeta: column 29,800']],
];
function frame(ph) {
  B(30, 30, 330, 150, 'Khaata (sach)', '', { st: 'good', icon: 'db' });
  cells(80, 75, [['+30,000'], ['-500'], ['+300']], { w: 85, h: 32 }); T(195, 140, 'jod = 29,800', { c: C.good, b: 1, size: 15 });
  const bad = ph !== 1;
  B(560, 30, 300, 90, 'Balance column', bad ? '30,300' : '29,800', { st: bad ? 'bad' : 'good', icon: 'db' });
  if (ph === 0) {
    num(195, 205, 1, C.good); num(530, 75, 2, C.bad);
    B(560, 250, 300, 65, 'App', 'dikha: 30,300', { st: 'bad', icon: 'phone' });
    A(710, 248, 710, 122, C.bad, { n: 3 }); L(400, 280, 'dono milane wala koi nahi', C.bad);
  } else if (ph === 1) {
    B(30, 250, 220, 65, 'Bank svc', 'transfer', { icon: 'server' });
    A(140, 248, 140, 182, C.msg, { n: 1 }); AP([[250, 300], [885, 300], [885, 75], [862, 75]], C.msg);
    B(390, 200, 170, 65, 'Job', 'raat 2 baje', { st: 'acc', icon: 'clock' });
    A(390, 220, 300, 182, C.acc, { n: 2 }); A(560, 215, 650, 122, C.good, { n: 3 });
    L(710, 175, '29,800 == 29,800', C.good);
  } else {
    B(400, 220, 140, 60, 'Job', 'farak!', { st: 'bad', icon: 'clock' }); num(470, 205, 1, C.bad);
    A(400, 235, 300, 182, C.muted); A(540, 235, 650, 122, C.muted);
    B(620, 250, 240, 65, 'Alert', 'account roka', { st: 'acc', icon: 'mail' });
    A(540, 260, 618, 280, C.acc, { n: 2 });
    num(195, 220, 3, C.good); L(195, 250, 'KYUN dhoondha,', C.good); L(195, 272, 'phir khaata jeeta', C.good);
  }
}
