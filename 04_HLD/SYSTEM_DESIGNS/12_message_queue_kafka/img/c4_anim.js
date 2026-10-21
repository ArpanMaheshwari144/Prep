// Card 4: email service ki 3 machine, Ravi ko 3 email. Ilaaj: ek consumer group, har tukda ek hi member ko. Agar: ek machine giri -> rebalance.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: teeno machine ka group.id alag, teeno ne P7 padha. Ravi ko ek order ke 3 email'],
  [3, GRN, 'ILAAJ', 'ILAAJ: teeno ek team (consumer group). Har tukda team ke ek hi member ko: P7 sirf C3 ko'],
  [6, YEL, 'AGAR', 'AGAR team ki ek machine giri? -> uske tukde baaki machines ko (rebalance)'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'P7 → C1, C2, C3', 'Teeno machine ka group.id alag (apna naam), teeno ne P7 padha'],
   [C.bad, 'C1, C2, C3 → Ravi', 'Teeno ne apni taraf se email bheja'],
   [C.bad, 'Ravi', 'Ravi ko ek order ke 3 email aaye']],
  [[C.msg, 'P → C', 'Ek team (group: email): P0-3 C1 ko, P4-6 C2 ko, P7-9 C3 ko'],
   [C.good, 'C3 → Ravi', 'Ravi P7 pe hai, sirf C3 ne email bheja'],
   [C.good, 'Ravi', 'Ek order, ek email']],
  [[C.bad, 'C3', 'Team ki machine C3 gir gayi'],
   [C.acc, 'P7-9 → C1', 'Uske tukde baaki machine ko mile (rebalance): P7-9 ab C1 ke paas'],
   [C.good, 'C1 → Ravi', 'Ravi ko email phir bhi ek hi gaya']],
];
function frame(ph) {
  const ys = [30, 140, 250];
  if (ph === 0) {
    B(20, 140, 170, 60, 'P7', 'Ravi ka order', { st: 'acc' });
    ['C1', 'C2', 'C3'].forEach((n, i) => {
      B(330, ys[i], 200, 60, n, 'group.id: ' + n.toLowerCase(), { st: 'bad', icon: 'server' });
      A(190, 170, 328, ys[i] + 30, C.msg, { n: i === 1 ? 1 : 0 });
      A(530, ys[i] + 30, 688, 170, C.bad, { n: i === 1 ? 2 : 0 });
    });
    B(690, 140, 190, 60, 'Ravi', '3 email', { st: 'bad', icon: 'mail' }); num(785, 225, 3, C.bad);
    L(785, 255, 'ek order, teen baar', C.bad);
  } else {
    el('rect', { x: 300, y: 15, width: 260, height: 315, rx: 12, fill: 'none', stroke: C.good, 'stroke-dasharray': '6 5', 'stroke-width': 2 }, DYN);
    T(430, 348, 'ek team: group.id = email', { c: C.good, b: 1, size: 14 });
    const P = ['P0-3', 'P4-6', 'P7-9'];
    P.forEach((p, i) => B(20, ys[i], 150, 60, p, i === 2 ? 'Ravi P7 pe' : '', { st: i === 2 ? 'acc' : 'dim' }));
    if (ph === 1) {
      ['C1', 'C2', 'C3'].forEach((n, i) => { B(330, ys[i], 200, 60, n, 'padhta ' + P[i], { st: 'good', icon: 'server' }); A(170, ys[i] + 30, 328, ys[i] + 30, C.msg, { n: i === 0 ? 1 : 0 }); });
      B(690, 250, 190, 60, 'Ravi', '1 email', { st: 'good', icon: 'mail' });
      A(530, 280, 688, 280, C.good, { n: 2 }); num(785, 225, 3, C.good);
    } else {
      B(330, 30, 200, 60, 'C1', 'P0-3 + P7-9', { st: 'good', icon: 'server' });
      B(330, 140, 200, 60, 'C2', 'padhta P4-6', { st: 'good', icon: 'server' });
      B(330, 250, 200, 60, 'C3', 'gir gayi', { st: 'bad', icon: 'server' }); cross(545, 280); num(545, 245, 1, C.bad);
      A(170, 60, 328, 60, C.msg); A(170, 170, 328, 170, C.msg);
      A(170, 270, 328, 75, C.acc, { n: 2, dash: 1, at: 0.4 });
      B(690, 30, 190, 60, 'Ravi', '1 email', { st: 'good', icon: 'mail' });
      A(530, 60, 688, 60, C.good, { n: 3 });
    }
  }
}
