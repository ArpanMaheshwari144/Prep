// Card 5: Ravi ka order analytics aur dashboard ko bhi chahiye. Ilaaj: har service ka apna group. Agar: nayi service aaj judi.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: teeno service ek hi group me. Khabar group me ek ko jaati: sirf Email ko mili'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har service ka apna group. Alag group = sabko poori khabar, har ek ka apna offset'],
  [6, YEL, 'AGAR', 'AGAR nayi service aaj judi? -> bas naya group, producer kuch nahi badalta'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Order → Topic', 'Ravi ka order topic me aaya'],
   [C.good, 'Topic → Email', 'Teeno ek hi group me: khabar sirf ek member ko, Email ko mili'],
   [C.bad, 'Analytics, Dashboard', 'Analytics aur dashboard ko Ravi ka order dikha hi nahi']],
  [[C.msg, 'Order → Topic', 'Ravi ka order topic me aaya'],
   [C.good, 'Topic → teeno', 'Har service ka apna group: teeno ko poori khabar mili'],
   [C.acc, 'offset', 'Har group apna offset khud yaad rakhta: 120, 120, 95; koi kisi ko nahi rokta']],
  [[C.acc, 'Fraud', 'Nayi service Fraud aaj judi, apna naya group banaya'],
   [C.acc, 'Fraud', 'Naye group ka offset hai hi nahi: batao shuru se ya ab se (auto.offset.reset)'],
   [C.good, 'Topic → Fraud', 'Shuru se = 7 din ka purana sab padha; producer ne kuch nahi badla']],
];
function frame(ph) {
  B(10, 140, 190, 60, 'Order svc', ph === 2 ? 'kuch nahi badla' : 'producer', { icon: 'server' });
  B(230, 120, 260, 100, 'Topic: orders', '', { icon: 'queue' });
  cells(272, 165, [['118'], ['119'], ['120', C.acc]], { w: 68, h: 34 });
  const ys = [25, 140, 255];
  if (ph === 0) {
    A(200, 170, 228, 170, C.msg, { n: 1 });
    el('rect', { x: 585, y: 10, width: 300, height: 315, rx: 12, fill: 'none', stroke: C.bad, 'stroke-dasharray': '6 5', 'stroke-width': 2 }, DYN);
    T(735, 345, 'ek hi group', { c: C.bad, b: 1, size: 14 });
    B(600, ys[0], 270, 60, 'Email', 'mila', { st: 'good', icon: 'mail' });
    B(600, ys[1], 270, 60, 'Analytics', 'kuch nahi', { st: 'bad', icon: 'server' });
    B(600, ys[2], 270, 60, 'Dashboard', 'kuch nahi', { st: 'bad', icon: 'server' });
    A(492, 150, 598, 55, C.good, { n: 2 }); num(560, 230, 3, C.bad);
  } else if (ph === 1) {
    A(200, 170, 228, 170, C.msg, { n: 1 });
    [['Email', 'group email: offset 120', 'mail'], ['Analytics', 'group analytics: 120', 'server'], ['Dashboard', 'group dash: 95', 'server']]
      .forEach(([n, s, ic], i) => { B(600, ys[i], 285, 60, n, s, { st: 'good', icon: ic }); A(492, 170, 598, ys[i] + 30, C.good, { n: i === 1 ? 2 : 0 }); });
    num(222, 300, 3, C.acc); L(400, 300, 'dashboard peeche (95), email nahi rukta', C.acc);
  } else {
    B(600, ys[0], 285, 60, 'Email', 'group email', { st: 'dim', icon: 'mail' });
    B(600, ys[1], 285, 70, 'Fraud (naya)', 'offset: shuru se?', { st: 'acc', icon: 'server' }); num(570, 130, 1, C.acc); num(570, 210, 2, C.acc);
    B(600, ys[2] + 10, 285, 60, 'Dashboard', 'group dash', { st: 'dim', icon: 'server' });
    AP([[370, 222], [370, 300], [540, 300], [540, 190], [598, 190]], C.good, { n: 3, at: 0.3 }); L(360, 330, 'shuru se: 7 din ka sab', C.good);
  }
}
