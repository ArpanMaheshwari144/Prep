// Card 10: broker ki disk bharti ja rahi. Ilaaj: retention 7 din, purana segment poora udao. Agar: har key ki sirf aakhri value -> compaction.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Kafka padh ke delete nahi karta. Roz ~100 GB judta raha, disk bharti ja rahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 7 din rakho, phir purana segment (~1 GB) poora udao (retention)'],
  [6, YEL, 'AGAR', 'AGAR har key ki sirf aakhri value chahiye? -> compaction: key ki aakhri khabar hi rakho'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Producer → Broker', 'Roz ~100 GB khabar disk pe file ke aakhir me judti'],
   [C.bad, 'Broker', 'Kuch bhi mitaya nahi gaya, 30 din ke segment pade hain'],
   [C.bad, 'Disk', 'Disk 95% bhar gayi, ab likhai rukne wali hai']],
  [[C.acc, 'Broker', 'Retention 7 din: sirf aakhri 7 din ke segment rakhne hain'],
   [C.bad, 'purane segment', 'Din 1 se 3 ke segment poore ek saath udaye, din 4-10 rakhe'],
   [C.good, 'Disk', 'Disk ~700 GB pe tiki: khaali retention se hoti, padhne se nahi']],
  [[C.msg, 'Topic: profile', 'Ravi (user:7) ka naam 3 baar badla: A, phir B, phir C'],
   [C.acc, 'compaction', 'Har key ki sirf aakhri khabar rakhi: user:7 = C'],
   [C.good, 'naya consumer', 'Naya consumer bas aakhri value padhta, purani history nahi chahiye']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 140, 170, 60, 'Producer', '~100 GB / din', { icon: 'server' });
    B(260, 100, 450, 140, 'Broker: disk', '', { st: 'bad', icon: 'db' });
    cells(310, 160, [['din 1', C.bad], ['din 2', C.bad], ['...', C.bad], ['din 29', C.bad], ['din 30', C.bad]], { w: 76, h: 36 });
    A(185, 170, 258, 170, C.msg, { n: 1 }); num(735, 175, 2, C.bad);
    bar(260, 260, 450, 16, 0.95, ''); num(735, 268, 3, C.bad); L(485, 305, 'disk 95%: likhai rukne wali', C.bad);
  } else if (ph === 1) {
    B(15, 140, 170, 60, 'Producer', '~100 GB / din', { icon: 'server' });
    B(260, 100, 450, 140, 'Broker: retention 7 din', '', { st: 'good', icon: 'db' }); num(735, 115, 1, C.acc);
    cells(310, 160, [['din 1-3 udaye', C.bad], ['din 4..9', C.good], ['din 10', C.good]], { w: 125, h: 36 }); num(400, 75, 2, C.bad); L(500, 75, 'poora segment udao', C.bad);
    A(185, 170, 258, 170, C.msg);
    bar(260, 260, 450, 16, 0.35, '', true); num(735, 268, 3, C.good); L(485, 305, '~700 GB pe tiki (7 din x 100 GB)', C.good);
  } else {
    B(10, 40, 235, 60, 'Topic: profile', 'key = user:7', { icon: 'queue' });
    T(260, 50, 'pehle:', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(260, 65, [['user:7 = A'], ['user:7 = B'], ['user:7 = C', C.acc]], { w: 140, h: 36 });
    A(245, 70, 258, 82, C.msg, { n: 1 });
    A(540, 105, 540, 185, C.acc, { n: 2 }); L(650, 145, 'compaction', C.acc);
    T(420, 215, 'ab:', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(470, 195, [['user:7 = C', C.good]], { w: 140, h: 36 });
    B(15, 260, 190, 60, 'Naya consumer', 'naam = C', { st: 'good', icon: 'server' });
    AP([[540, 233], [540, 290], [207, 290]], C.good, { n: 3, at: 0.4 });
  }
}
