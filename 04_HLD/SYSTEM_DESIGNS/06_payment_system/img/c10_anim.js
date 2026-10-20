// Card 10: saal me ~1000 crore row, ek DB, index RAM me nahi, likhai dheemi. Ilaaj: shard by account_id.
// Agar: do account alag shard pe -> saga: A se kaato, B me daalo.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: saal me ~1000 crore row, ek hi DB. Index RAM me nahi samaata, har likhai dheemi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: account_id se tukde (shard). Mera balance aur meri history ek hi tukde pe'],
  [6, YEL, 'AGAR', 'AGAR do account alag shard pe? -> saga: A se kaato, B me daalo'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Payment Svc → Ek DB', 'Payment Svc har payment ek hi DB me likh raha'], [C.bad, 'Index 400 GB, RAM 64 GB', 'Index 400 GB ka, RAM sirf 64 GB, to index disk se padhna padta'], [C.bad, 'Ek DB', 'Isliye DB pe har nayi likhai dheemi ho gayi']],
  [[C.good, 'Payment Svc → Shard-1', 'Arpan ka balance aur history ek hi tukde (Shard-1) pe'], [C.good, 'Payment Svc → Shard-2', 'Kirana ka sab uske apne tukde (Shard-2) pe']],
  [[C.acc, 'Saga Svc → Shard-1', 'Do tukde, ek transaction nahi; saga pehle Shard-1 me Arpan se 500 kaatta'], [C.good, 'Saga Svc → Shard-2', 'Phir Shard-2 me Kirana ko 500 daalta'], [C.bad, 'fail?', 'Doosra kadam fail hua to saga Shard-1 me Arpan ko 500 wapas deta']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 170, 70, 'Payment Svc', 'likho');
    B(350, 90, 260, 150, 'Ek DB', '1000 crore row', { st: 'bad' });
    bar(370, 200, 220, 12, 1, '');
    A(190, 165, 348, 165, C.msg, { n: 1 });
    B(690, 40, 180, 60, 'RAM', '64 GB', { st: 'dim' });
    B(690, 210, 180, 60, 'Index', '400 GB', { st: 'bad' });
    num(700, 160, 2, C.bad); L(720, 165, 'disk se padha', C.bad, 'start');
    num(480, 270, 3, C.bad); L(500, 275, 'har likhai dheemi', C.bad, 'start');
    L(450, 340, 'ek machine ki had, data ki nahi', C.bad);
  } else if (ph === 1) {
    B(20, 130, 170, 70, 'Payment Svc', 'account_id dekho', { st: 'acc' });
    B(560, 20, 300, 70, 'Shard-1', 'Arpan: balance + history', { st: 'good' });
    B(560, 140, 300, 70, 'Shard-2', 'Kirana: balance + history', { st: 'good' });
    B(560, 260, 300, 70, 'Shard-3', 'baaki account', { st: 'good' });
    A(190, 150, 558, 55, C.good, { n: 1, at: 0.75 }); L(370, 85, 'Arpan -> Shard-1', C.good);
    A(190, 175, 558, 175, C.good, { n: 2, at: 0.75 }); L(370, 165, 'Kirana -> Shard-2', C.good);
    A(190, 195, 558, 290, C.muted);
    L(370, 345, 'mera balance aur history ek tukde pe: ek transaction kaafi', C.good);
  } else {
    B(20, 130, 170, 70, 'Saga Svc', 'Arpan -> Kirana 500', { st: 'acc' });
    B(560, 30, 300, 80, 'Shard-1', 'Arpan -500', { st: 'acc' });
    B(560, 230, 300, 80, 'Shard-2', 'Kirana +500', { st: 'good' });
    A(190, 150, 558, 70, C.acc, { n: 1 }); L(370, 85, 'pehle kaato', C.acc);
    A(190, 185, 558, 270, C.good, { n: 2 }); L(370, 260, 'phir daalo', C.good);
    num(570, 170, 3, C.bad); L(590, 175, 'fail? Shard-1 me +500 wapas', C.bad, 'start');
    L(450, 345, 'do shard pe ek transaction nahi chalta, isliye saga (card 6 wala)', C.good);
  }
}
