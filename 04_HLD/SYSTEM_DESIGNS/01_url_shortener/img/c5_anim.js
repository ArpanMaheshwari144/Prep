// Card 5: 90 TB ek machine me nahi. Ilaaj: code se shard (ring), 3 copy. Agar: ek machine mar gayi.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: saare link ek DB machine me, 5 saal me 90 TB, samaata nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: code ka hash, ring pe aage chalo, jo pehli machine mile wo uski. 3 copy'],
  [6, YEL, 'AGAR', 'AGAR ek machine mar gayi? -> baaki 2 copy se milta, link nahi gaye'],
];
function setup() {}
const CX = 600, CY = 175, R = 105;
const P = [[CX, CY - R], [CX + 91, CY + 52], [CX - 91, CY + 52]]; // DB-1 top, DB-2 right-neeche, DB-3 left-neeche
function ring(dead) {
  el('circle', { cx: CX, cy: CY, r: R, fill: 'none', stroke: C.line, 'stroke-width': 2, 'stroke-dasharray': '6 5' }, DYN);
  ['DB-1', 'DB-2', 'DB-3'].forEach((n, i) => {
    const d = dead === i; B(P[i][0] - 40, P[i][1] - 20, 80, 40, n, '', { st: d ? 'bad' : 'good' }); if (d) cross(P[i][0] + 58, P[i][1]);
  });
}
const HP = [CX - 95, CY - 45]; // hash ki jagah ring pe (DB-1 se thoda pehle)
const STEPS = [
  [[C.bad, 'App → DB', 'har link ek hi machine pe'], [C.bad, 'DB', '90 TB: samaata nahi']],
  [[C.acc, 'App → ring', 'code ka hash = ring pe ek jagah'], [C.good, 'ring → DB-1', 'aage chalo, pehli machine: DB-1'], [C.acc, 'DB-1 → DB-2, DB-3', 'copy agle 2 pe']],
  [[C.acc, 'App → ring', 'code ka hash'], [C.bad, 'ring → DB-1', 'DB-1 mar gayi'], [C.good, 'ring → DB-2', 'copy se mila, link gaya nahi']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 120, 60, 'App'); A(140, 160, 358, 160, C.bad, { n: 1 }); L(250, 140, 'har link yahin', C.bad);
    B(360, 110, 180, 100, 'DB', 'ek hi machine', { st: 'bad' }); cross(565, 160);
    bar(360, 230, 180, 16, 1, ''); T(450, 270, '90 TB', { c: C.bad, b: 1, size: 16 }); num(450, 300, 2, C.bad); L(450, 325, 'samaata nahi', C.bad);
  } else if (ph === 1) {
    B(20, 130, 120, 60, 'App', 'code: ab3x9Kq'); ring(-1);
    A(140, 150, HP[0] - 8, HP[1] + 4, C.acc, { n: 1, at: 0.45 }); L(300, 110, 'code ka hash = ring pe ek jagah', C.acc);
    dot(HP, C.acc, '');
    A(HP[0] + 8, HP[1] - 10, P[0][0] - 42, P[0][1] + 2, C.good, { n: 2 }); L(CX + 75, CY - R - 10, 'aage chalo, pehli: DB-1', C.good, 'start');
    A(P[0][0] + 20, P[0][1] + 20, P[1][0] - 20, P[1][1] - 20, C.muted, { n: 3, dash: 1 });
    A(P[1][0] - 42, P[1][1], P[2][0] + 42, P[2][1], C.muted, { dash: 1 });
    L(CX + 60, CY + 100, 'copy agle 2 pe', C.muted, 'start');
    L(20, 240, 'click code ke saath aata, seedha', C.muted, 'start'); L(20, 260, 'ek machine pe. Machine judi to', C.muted, 'start'); L(20, 280, 'sirf pados ki keys hilti', C.muted, 'start');
  } else {
    B(20, 130, 120, 60, 'App', 'code: ab3x9Kq'); ring(0);
    A(140, 150, HP[0] - 8, HP[1] + 4, C.acc, { n: 1, at: 0.45 }); dot(HP, C.acc, '');
    A(HP[0] + 8, HP[1] - 10, P[0][0] - 42, P[0][1] + 2, C.bad, { n: 2, dash: 1 }); L(CX + 75, CY - R - 10, 'DB-1 mar gayi', C.bad, 'start');
    A(HP[0] + 10, HP[1] + 10, P[1][0] - 42, P[1][1] - 8, C.good, { n: 3 }); L(CX + 60, CY + 100, 'copy se mila, link gaya nahi', C.good, 'start');
    L(20, 240, 'naya DB-1 aaya to baaki', C.muted, 'start'); L(20, 260, 'copy se bhar jaata', C.muted, 'start');
  }
}
