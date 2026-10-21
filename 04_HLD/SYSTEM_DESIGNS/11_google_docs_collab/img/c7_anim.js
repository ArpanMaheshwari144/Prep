// Card 7: metro me net 1 sec gaya, typing rukegi? Ilaaj: likhna AP (pending op), permission CP (SQL). Agar: offline me access hata.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: metro me net gaya. Har likhai server ke "haan" pe ruki (strong), typing ruk gayi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: likhna AP: op phone me pending, typing chalti. Permission CP: net aate hi SQL check'],
  [6, YEL, 'AGAR', 'AGAR offline me access hata diya? -> net aate hi SQL check, access nahi to op reject'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'Main ✕ Server', 'Metro me net 1 second ke liye gaya'],
   [C.bad, 'Main ka app', 'Har likhai server ke "haan" ka intezaar karti (strong), typing ruk gayi']],
  [[C.acc, 'Main ka app', 'Net gaya, par typing chalti rahi; op phone me pending line me jama'],
   [C.msg, 'Main → Server', 'Net aate hi teeno pending op server ko bheje'],
   [C.good, 'Server → SQL', 'Pehle permission SQL se check (CP): access hai'],
   [C.good, 'Server', 'Op khiska ke lage, aakhir me sabka doc same (AP)']],
  [[C.bad, 'Owner → SQL', 'Main offline tha, doc ke owner ne mera access hata diya'],
   [C.msg, 'Main → Server', 'Net aate hi pending op bheje'],
   [C.bad, 'Server → SQL', 'SQL ne bola access nahi: saare pending op reject']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 130, 210, 70, 'Main ka app', 'typing ruki', { st: 'bad', icon: 'phone' });
    B(620, 130, 180, 70, 'Server', '', { icon: 'server' });
    A(225, 165, 618, 165, C.bad, { n: 1, dash: 1, at: 0.3 }); cross(470, 165); L(470, 130, 'net gaya (metro)', C.bad);
    num(120, 230, 2, C.bad); L(120, 262, '"haan" ka intezaar', C.bad); L(120, 284, 'har akshar pe', C.bad);
    L(560, 320, 'typing rukna = sabse bura, ye nahi chahiye', C.muted);
  } else if (ph === 1) {
    B(15, 30, 210, 70, 'Main ka app', 'typing chalti', { st: 'good', icon: 'phone' });
    cells(20, 125, [['op1', C.acc], ['op2', C.acc], ['op3', C.acc]], { w: 66, h: 32 }); num(240, 141, 1, C.acc);
    L(120, 185, 'pending (phone me)', C.acc);
    B(380, 120, 180, 70, 'Server', 'OT', { st: 'good', icon: 'server' });
    A(225, 80, 378, 135, C.msg, { n: 2 });
    B(700, 30, 180, 65, 'SQL', 'permission (CP)', { st: 'good', icon: 'db' });
    A(562, 140, 698, 70, C.good, { n: 3 }); L(790, 120, 'access hai', C.good);
    B(700, 240, 180, 65, 'Doc', 'sab same', { st: 'good' });
    A(562, 175, 698, 265, C.good, { n: 4 }); L(470, 300, 'khiska ke laga (AP)', C.good);
  } else {
    B(15, 140, 210, 70, 'Main ka app', 'pending: 3 op', { st: 'acc', icon: 'phone' });
    B(380, 140, 180, 70, 'Server', '', { icon: 'server' });
    B(700, 30, 180, 65, 'SQL', 'access nahi', { st: 'bad', icon: 'db' });
    B(700, 250, 180, 65, 'Doc owner', 'access hataya', { icon: 'user' });
    A(790, 248, 790, 97, C.bad, { n: 1 });
    A(225, 175, 378, 175, C.msg, { n: 2 });
    A(562, 160, 698, 70, C.bad, { n: 3 }); L(470, 260, 'pending op reject', C.bad);
    L(470, 282, 'access kabhi galat nahi', C.muted);
  }
}
