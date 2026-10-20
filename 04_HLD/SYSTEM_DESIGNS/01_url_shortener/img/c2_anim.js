// Card 2: ek hi App giri, site band. Ilaaj: kai App + LB. Agar: App dheemi.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek hi App machine, wo giri to poori site band'],
  [3, GRN, 'ILAAJ', 'ILAAJ: kai App, aage LB. App-1 giri to LB agla click App-2 ko deta'],
  [6, YEL, 'AGAR', 'AGAR App giri nahi, bas dheemi? -> jiske kam connection khule, use bhejo'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'Users → App', 'Saare users ek hi App machine pe aa rahe the, koi doosri machine nahi thi'], [C.bad, 'App giri', 'Wo ek machine giri to kisi ko jawab nahi mila, poori site band ho gayi']],
  [[C.msg, 'Users → LB', 'Click pehle LB pe aata hai, LB tay karta hai kaunsi App ko dena'], [C.bad, 'LB → App-1', 'App-1 giri hai; /health ka jawab nahi diya, to LB use bhejna band karta'], [C.good, 'LB → App-2', 'Agla click LB ne App-2 ko de diya, user ko farak hi nahi pada'], [C.good, 'App-2 → Redis / DB', 'App-2 apne paas kuch yaad nahi rakhti, data Redis / DB se laati hai']],
  [[C.msg, 'Users → LB', 'Naya click LB pe aaya; App-1 pe pehle se 50 connection khule hain'], [C.good, 'LB → App-2', 'LB ne kam bheed wali App-2 (5 connection) ko diya; round-robin aadhe click dheemi pe bhejta']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 120, 60, 'Users'); A(140, 160, 358, 160, C.bad, { n: 1 });
    B(360, 130, 130, 60, 'App', 'ek hi', { st: 'bad' }); cross(515, 160); L(425, 220, 'giri', C.bad);
    A(490, 160, 598, 160, C.muted, { dash: 1 }); B(600, 130, 120, 60, 'DB', '', { st: 'dim' });
    num(80, 230, 2, C.bad); L(80, 255, 'koi jawab nahi', C.bad); L(80, 275, 'poori site band', C.bad);
  } else if (ph === 1) {
    B(20, 130, 120, 60, 'Users'); A(140, 160, 238, 160, C.msg, { n: 1 });
    B(240, 130, 110, 60, 'LB', 'round-robin', { st: 'acc' });
    B(480, 40, 130, 60, 'App-1', '', { st: 'bad' }); cross(635, 70); L(545, 20, 'giri, /health chup', C.bad);
    A(350, 145, 478, 75, C.bad, { n: 2, dash: 1, at: 0.4 });
    B(480, 220, 130, 60, 'App-2', '', { st: 'good' });
    A(350, 175, 478, 245, C.good, { n: 3, at: 0.4 }); L(400, 260, 'agla click yahan', C.good);
    B(700, 220, 120, 60, 'Redis / DB'); A(610, 250, 698, 250, C.good, { n: 4 });
    L(545, 320, 'App kuch yaad nahi rakhta (stateless), koi bhi App koi bhi click le', C.muted);
  } else {
    B(20, 130, 120, 60, 'Users'); A(140, 160, 238, 160, C.msg, { n: 1 });
    B(240, 130, 110, 60, 'LB', 'least-conn', { st: 'acc' });
    B(480, 40, 130, 60, 'App-1', 'khule: 50', { st: 'acc' }); L(545, 20, 'dheemi, giri nahi', C.acc);
    bar(480, 110, 130, 12, 0.95, '');
    A(350, 145, 478, 75, C.muted, { dash: 1 });
    B(480, 220, 130, 60, 'App-2', 'khule: 5', { st: 'good' }); bar(480, 290, 130, 12, 0.1, '', 1);
    A(350, 175, 478, 245, C.good, { n: 2, at: 0.4 }); L(390, 300, 'kam connection wali ko', C.good);
    L(545, 335, 'round-robin hota to aadhe click dheemi App-1 pe jaate', C.muted);
  }
}
