// Card 1: Ravi ko 500 bheje, beech me server gira. Ilaaj: ek transaction (BEGIN .. COMMIT). Agar: beech me gira -> WAL se ulta.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: mera -500 hua, server gira, Ravi ka +500 hua hi nahi. 500 gayab'],
  [3, GRN, 'ILAAJ', 'ILAAJ: BEGIN, Arpan -500, Ravi +500, COMMIT. Dono hon ya koi nahi (transaction)'],
  [6, YEL, 'AGAR', 'AGAR COMMIT se pehle server gira? -> DB log (WAL) dekhta, COMMIT nahi to ulta, 1000 wapas'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Bank svc → Arpan', 'Arpan ke khaate se 500 kaate: 1000 se 500'],
   [C.bad, 'Bank svc', 'Ravi ko +500 karne se pehle server gir gaya'],
   [C.bad, 'Ravi', 'Ravi ke 2000 hi rahe: 500 na mere paas, na Ravi ke paas']],
  [[C.msg, 'Bank svc → DB', 'BEGIN: dono kaam ek transaction me shuru'],
   [C.acc, 'Arpan, Ravi', 'Arpan -500 aur Ravi +500, dono usi transaction ke andar'],
   [C.good, 'COMMIT', 'COMMIT: dono ek saath pakke; Arpan 500, Ravi 2500']],
  [[C.bad, 'Bank svc', '-500 ke baad, COMMIT se pehle server gira'],
   [C.acc, 'DB → WAL', 'Uthte hi DB ne apna log (WAL) dekha: COMMIT likha hi nahi'],
   [C.good, 'DB', 'Adhoora kaam ulta: Arpan ke 1000 wapas, kuch gayab nahi']],
];
function frame(ph) {
  const dead = ph !== 1;
  B(15, 140, 180, 60, 'Bank svc', dead ? 'gir gaya' : '', { st: dead ? 'bad' : '', icon: 'server' });
  if (dead) cross(215, 170);
  if (ph === 0) {
    num(250, 170, 2, C.bad);
    B(400, 40, 300, 60, 'Arpan', '1000 -> 500', { st: 'bad', icon: 'user' });
    B(400, 250, 300, 60, 'Ravi', '2000 (+500 hua hi nahi)', { st: 'bad', icon: 'user' });
    A(195, 150, 398, 70, C.msg, { n: 1 }); A(195, 190, 398, 275, C.bad, { dash: 1 }); num(730, 280, 3, C.bad);
    L(550, 180, '500 gayab', C.bad);
  } else if (ph === 1) {
    el('rect', { x: 370, y: 20, width: 370, height: 305, rx: 12, fill: 'none', stroke: C.acc, 'stroke-dasharray': '6 5', 'stroke-width': 2 }, DYN);
    T(555, 345, 'BEGIN ... COMMIT (ek transaction)', { c: C.acc, b: 1, size: 14 });
    B(400, 40, 310, 60, 'Arpan', '1000 -> 500', { st: 'good', icon: 'user' });
    B(400, 250, 310, 60, 'Ravi', '2000 -> 2500', { st: 'good', icon: 'user' });
    A(195, 170, 368, 170, C.msg, { n: 1 }); num(555, 160, 2, C.acc); L(555, 195, '-500 aur +500 saath', C.acc); A(742, 170, 768, 170, C.good);
    B(770, 140, 120, 60, 'COMMIT', 'dono pakke', { st: 'good' }); num(830, 225, 3, C.good);
  } else {
    num(250, 170, 1, C.bad);
    B(330, 30, 260, 110, 'DB log (WAL)', '', { st: 'acc', icon: 'db' });
    cells(372, 85, [['BEGIN'], ['-500'], ['COMMIT?', C.bad]], { w: 66, h: 34 }); num(620, 85, 2, C.acc);
    B(400, 250, 300, 60, 'Arpan', '500 -> 1000 wapas', { st: 'good', icon: 'user' });
    A(460, 142, 520, 248, C.good, { n: 3 }); L(700, 200, 'COMMIT nahi = ulta', C.good);
  }
}
