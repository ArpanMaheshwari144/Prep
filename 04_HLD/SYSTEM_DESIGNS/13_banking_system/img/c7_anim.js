// Card 7: 11 baje salary, ek Banking service bhar gayi. Ilaaj: kai machine + LB (stateless), DB ki sync copy. Agar: main DB gira.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek hi machine. 11 baje laakhon transfer, CPU bhara, machine giri'],
  [3, GRN, 'ILAAJ', 'ILAAJ: kai machine + LB (stateless). DB ki sync copy doosri building me'],
  [6, YEL, 'AGAR', 'AGAR main DB gira? -> sync copy promote (~1-2 min). Beech me error to WAHI id se dobara'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'log → Bank svc', '11 baje sabki salary aayi, laakhon transfer ek saath'],
   [C.bad, 'Bank svc', 'Ek hi machine: CPU 100%, machine gir gayi'],
   [C.bad, 'log', 'Machine giri to koi bhi paisa nahi bhej pa raha']],
  [[C.msg, 'log → LB', 'Saari request pehle LB pe aati'],
   [C.good, 'LB → svc 1..3', 'LB kisi bhi machine ko deta: sach DB me, machine kuch yaad nahi rakhti'],
   [C.acc, 'DB → sync copy', 'DB ki har likhai doosri building ki copy pe bhi (sync), tabhi OK']],
  [[C.bad, 'Main DB', 'Main DB gir gaya'],
   [C.acc, 'sync copy', 'Doosri building wali sync copy ~1-2 min me main bani (Multi-AZ)'],
   [C.good, 'App → svc', 'Beech me error aaya to app ne WAHI id se dobara bheja, do baar nahi kata']],
];
function frame(ph) {
  dots(20, 18, 130, C.msg, { gap: 13, cols: 5 }); L(50, 210, 'laakhon log', C.msg);
  if (ph === 0) {
    B(300, 130, 220, 80, 'Bank svc', 'ek hi machine', { st: 'bad', icon: 'server' }); bar(300, 222, 220, 12, 1, '');
    cross(545, 170); num(580, 170, 2, C.bad);
    A(90, 165, 298, 165, C.msg, { n: 1 });
    B(660, 130, 210, 80, 'Log', 'bhej nahi pa rahe', { st: 'bad', icon: 'phone' }); num(765, 240, 3, C.bad);
  } else {
    B(150, 140, 110, 60, 'LB', '', { icon: 'server' }); A(90, 170, 148, 170, C.msg, { n: ph === 1 ? 1 : 0 });
    const ys = [30, 140, 250];
    ys.forEach((y, i) => { B(330, y, 170, 60, 'Bank svc ' + (i + 1), 'stateless', { st: 'good', icon: 'server' }); A(260, 170, 328, y + 30, C.good, { n: i === 0 && ph === 1 ? 2 : 0 }); });
    const dead = ph === 2;
    B(620, 40, 250, 70, dead ? 'Main DB' : 'DB (main)', dead ? 'gir gaya' : 'building A', { st: dead ? 'bad' : 'good', icon: 'db' });
    B(620, 240, 250, 70, dead ? 'Copy -> naya main' : 'DB sync copy', 'building B', { st: dead ? 'good' : 'acc', icon: 'db' });
    ys.forEach(y => A(500, y + 30, 618, dead ? 270 : 80, C.muted));
    if (!dead) { A(745, 112, 745, 238, C.acc, { n: 3 }); L(810, 175, 'har likhai', C.acc); }
    else { num(595, 75, 1, C.bad); num(640, 215, 2, C.acc); num(30, 300, 3, C.good); L(170, 320, 'app: WAHI id se dobara', C.good); }
  }
}
