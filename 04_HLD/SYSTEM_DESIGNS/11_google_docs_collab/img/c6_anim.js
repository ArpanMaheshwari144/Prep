// Card 6: har akshar pe DB me likha, DB baith gaya. Ilaaj: Redis buffer + copy (WAIT 1) tab ACK, har 1 sec batch. Agar: ACK se pehle Redis gira.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har akshar ek op, har op seedha DB me likha. Lakhon likhai / sec, DB baith gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: op Redis buffer + copy dono me (WAIT 1), tab ACK. Har 1 sec ek batch Cassandra me'],
  [6, YEL, 'AGAR', 'AGAR ACK se pehle Redis gira? -> app op rakhta, opId ke saath phir bhejta'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Main → Server', 'Main type kar raha, har akshar ek op ban ke server pe'],
   [C.bad, 'Server → DB', 'Har op ke liye seedha DB me ek likhai'],
   [C.bad, 'DB', 'Lakhon log type kar rahe, lakhon likhai / sec, DB baith gaya']],
  [[C.acc, 'Server → Redis', 'Op pehle Redis buffer me likha, RAM me, ~1 ms'],
   [C.acc, 'Redis → copy', 'Copy me bhi pahuncha (WAIT 1), tabhi pakka'],
   [C.good, 'Server → Main', 'Tab ACK: Main ko turant "saved" dikha'],
   [C.good, 'Redis → Cassandra', 'Har 1 sec ek batch Cassandra me, DB pe likhai kam']],
  [[C.msg, 'Main → Server', 'Maine op bheja (opId 77), app ne apne paas bhi rakha'],
   [C.bad, 'Redis', 'ACK se pehle Redis gir gaya, ACK nahi aaya'],
   [C.good, 'Main → Server', 'Judte hi opId 77 phir bheja; pehle se hai to skip, nahi to likho']],
];
function frame(ph) {
  B(15, 140, 150, 60, 'Main', ph === 2 ? 'op 77 rakha' : 'type kar raha', { icon: 'user' });
  B(230, 140, 150, 60, 'Server', '', { icon: 'server' });
  if (ph === 0) {
    A(165, 170, 228, 170, C.msg, { n: 1 });
    B(650, 140, 200, 60, 'DB', 'lakhon likhai / sec', { st: 'bad', icon: 'db' });
    [-20, -10, 0, 10, 20].forEach(d => A(380, 170 + d, 648, 170 + d, C.bad, { n: d === 0 ? 2 : 0 }));
    bar(650, 215, 200, 14, 1, ''); num(870, 170, 3, C.bad); L(750, 260, 'DB baith gaya', C.bad);
    L(450, 320, 'har akshar = ek op = ek DB likhai', C.muted);
  } else if (ph === 1) {
    B(450, 30, 190, 60, 'Redis buffer', 'RAM, ~1 ms', { st: 'acc', icon: 'queue' });
    B(450, 250, 190, 60, 'Redis copy', 'WAIT 1', { st: 'acc', icon: 'queue' });
    B(700, 140, 180, 60, 'Cassandra', 'batch me', { st: 'good', icon: 'db' });
    A(380, 150, 448, 70, C.acc, { n: 1 }); A(545, 92, 545, 248, C.acc, { n: 2 });
    A(228, 190, 167, 190, C.good, { n: 3 }); L(200, 235, 'ACK: saved', C.good);
    A(642, 70, 740, 138, C.good, { n: 4 }); L(790, 95, 'har 1 sec', C.good);
    L(450, 340, 'batch ACK ke raaste me nahi, typing nahi rukti', C.muted);
  } else {
    B(450, 120, 190, 60, 'Redis buffer', 'gir gaya', { st: 'bad', icon: 'queue' }); cross(665, 150); num(700, 150, 2, C.bad);
    A(165, 160, 228, 160, C.msg, { n: 1 }); A(380, 160, 448, 150, C.msg);
    A(228, 185, 167, 185, C.bad, { dash: 1 }); L(200, 230, 'ACK nahi aaya', C.bad);
    AP([[90, 202], [90, 300], [305, 300], [305, 202]], C.good, { n: 3, at: 0.5 }); L(200, 330, 'phir bheja: opId 77', C.good);
    L(600, 260, 'opId pehle se? skip', C.good); L(600, 282, 'nahi? likho', C.good);
  }
}
