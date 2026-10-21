// Card 11: 5 saal ka khaata ek table me. Ilaaj: mahine ke tukde (partition) + S3 archive, snapshot. Agar: RBI ko 2021 ka statement.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 5 saal ki ~36 arab line ek hi table me. Query + backup sab chhoote, dheema'],
  [3, GRN, 'ILAAJ', 'ILAAJ: mahine ke tukde (partition). Mahina band hua to alag karke S3 pe (archive), sasta'],
  [6, YEL, 'AGAR', 'AGAR RBI ko 2021 ka statement chahiye? -> S3 pe hi query (Athena, Parquet)'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'Khaata table', '5 saal ki ~36 arab line ek hi table me pad gayi'],
   [C.bad, 'App → table', 'Roz ki query ko bhi badi table chhooni padti, dheema'],
   [C.bad, 'Backup', 'Backup bhi poori table ka: mehnga aur ghanton lamba']],
  [[C.good, 'Khaata', 'Mahine ke tukde (partition): is mahine ka tukda chhota, query tez'],
   [C.acc, 'DB → S3', 'Mahina band hua to tukda alag karke S3 pe (archive), sasta'],
   [C.good, 'App', 'Balance = mahine ka snapshot + baad ki line, 5 saal nahi jodte']],
  [[C.msg, 'RBI → Athena', 'RBI ne 2021 ka statement maanga, query Athena pe gayi'],
   [C.good, 'Athena → S3', 'S3 pe hi query (Athena, Parquet), tukda wapas laane ki zaroorat nahi'],
   [C.acc, 'S3', 'Object Lock: purani file na delete, na badal sakte (5 saal, PMLA)']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 30, 170, 60, 'App', 'roz ki query', { icon: 'server' });
    B(15, 250, 170, 60, 'Backup', 'raat ko', { icon: 'clock' });
    B(330, 100, 400, 130, 'Khaata: ek table', '2021 .. 2026, ~36 arab line', { st: 'bad', icon: 'db' }); bar(330, 245, 400, 14, 1, '');
    num(760, 165, 1, C.bad);
    A(185, 60, 328, 130, C.bad, { n: 2 }); A(185, 280, 328, 210, C.bad, { n: 3 });
    L(530, 300, 'dheema + mehnga', C.bad);
  } else if (ph === 1) {
    B(15, 140, 170, 60, 'App', 'balance', { icon: 'server' });
    B(250, 40, 400, 140, 'DB: khaata (tukde)', '', { st: 'good', icon: 'db' });
    cells(300, 95, [['2026-10', C.good], ['2026-09', C.good], ['2026-08', C.good]], { w: 110, h: 36 }); num(470, 205, 1, C.good);
    A(185, 160, 248, 120, C.good, { n: 3 }); L(220, 230, 'snapshot + baad ki line', C.good);
    B(690, 230, 200, 90, 'S3 archive', '2021-25: sasta', { st: 'acc', icon: 'cloud' });
    A(650, 150, 760, 228, C.acc, { n: 2 }); L(780, 150, 'mahina band', C.acc);
  } else {
    B(15, 140, 170, 60, 'RBI', '2021 statement', { icon: 'user' });
    B(300, 140, 190, 60, 'Athena', 'S3 pe hi query', { st: 'good', icon: 'server' });
    B(620, 110, 270, 120, 'S3 archive', '2021 (Parquet)', { st: 'good', icon: 'cloud' });
    A(185, 170, 298, 170, C.msg, { n: 1 }); A(490, 170, 618, 170, C.good, { n: 2 });
    B(620, 270, 270, 60, 'Object Lock', 'na delete, na badal', { st: 'acc', icon: 'lock' }); num(600, 300, 3, C.acc);
    L(300, 300, 'tukda wapas DB me nahi laana pada', C.good);
  }
}
