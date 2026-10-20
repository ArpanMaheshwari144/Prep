// Card 2: GET phir SET = do kadam, 6 andar. Ilaaj: INCR ek kadam. Agar: EXPIRE se pehle crash, Lua atki.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: GET phir SET, do kadam. Dono App ne 4 padha, 6 andar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: INCR se padhna aur badhana ek hi kadam. Redis ek ke baad ek chalata'],
  [6, YEL, 'AGAR', 'AGAR INCR hua, EXPIRE se pehle crash? -> key kabhi nahi mitegi, user block'],
  [9, YEL, 'AGAR', 'AGAR Lua atak gayi? -> poora Redis ruk jaata, kyunki ek hi line hai'],
  [12, GRN, 'WAPAS', 'WAPAS: script chhoti rakhi, loop nahi. Line phir chal padi'],
];
function setup() {}
function base(s1, s2, st1, st2, rs, rst, bn = 1) {
  B(20, 140, 90, 80, 'Bot', '20 login');
  A(110, 170, 248, 100, C.msg, { n: bn }); A(110, 190, 248, 260, C.msg, { n: bn });
  B(250, 60, 130, 70, 'App-1', s1, { st: st1 }); B(250, 225, 130, 70, 'App-2', s2, { st: st2 });
  B(530, 135, 170, 80, 'Redis', rs, { st: rst });
}
// Redis ke andar ghoomti script (chakkar + teer)
function loop(cx, cy, c) {
  el('path', { d: `M${cx + 16} ${cy} A16 16 0 1 1 ${cx} ${cy - 16}`, fill: 'none', stroke: c, 'stroke-width': 3 }, DYN);
  el('path', { d: `M${cx} ${cy - 23} L${cx + 9} ${cy - 16} L${cx} ${cy - 9} Z`, fill: c }, DYN);
}
function frame(ph) {
  if (ph === 0) {
    base('padha: 4', 'padha: 4', 'bad', 'bad', 'ginti: 5', '');
    A(528, 150, 382, 82, C.acc, { dash: 1, n: 2, at: 0.2 }); A(528, 200, 382, 268, C.acc, { dash: 1, n: 2, at: 0.2 });
    A(382, 105, 528, 168, C.acc, { n: 3, at: 0.2 }); A(382, 248, 528, 184, C.acc, { n: 3, at: 0.2 });
    L(440, 70, 'GET: 4'); L(440, 300, 'GET: 4');
    L(430, 150, 'SET 5'); L(430, 215, 'SET 5');
    T(800, 170, 'andar: 6', { c: C.bad, b: 1, size: 24 }); T(800, 200, 'had: 5', { c: C.muted, size: 14 });
  } else if (ph === 1) {
    base('5: andar', '6: 429', 'good', 'bad', 'ginti: 6', 'acc');
    A(380, 95, 528, 160, C.good, { n: 2 }); A(380, 260, 528, 190, C.bad, { n: 3 });
    L(455, 108, 'INCR', C.good); L(455, 250, 'INCR (pehle ruk)', C.bad);
    T(800, 170, 'andar: 5', { c: C.good, b: 1, size: 24 }); T(800, 200, 'had: 5', { c: C.muted, size: 14 });
  } else if (ph === 2) {
    B(20, 140, 110, 70, 'Arpan');
    B(420, 30, 180, 70, 'Redis', '', { st: 'bad' }); cross(625, 65);
    num(405, 145, 1, C.good); num(605, 145, 2, C.bad);
    cells(420, 130, [['INCR', C.good], ['EXPIRE', C.bad]], { w: 86, h: 30 });
    L(660, 150, 'crash yahan', C.bad, 'start');
    L(640, 220, 'hal: INCR + EXPIRE ek Lua me', C.good, 'start');
    B(420, 230, 180, 70, 'rate:Arpan', 'umar: nahi', { st: 'bad' });
    A(130, 175, 410, 260, C.bad, { n: 3, at: 0.15 });
    dots(3, 220, 205, C.bad, { gap: 60 });
    T(280, 300, 'aaj  kal  parso: 429', { c: C.bad, b: 1, size: 14 });
  } else if (ph === 3) {
    base('ruka', 'ruka', 'bad', 'bad', '', 'bad', 2);
    loop(615, 190, C.bad); num(575, 178, 1, C.bad);
    L(615, 245, 'Lua loop: ye chal rahi', C.bad);
    A(380, 95, 528, 160, C.bad, { dash: 1, n: 3, at: 0.85 }); A(380, 260, 528, 190, C.bad, { dash: 1, n: 3, at: 0.85 });
    dots(3, 400, 115, C.msg, { gap: 22 }); dots(3, 400, 235, C.msg, { gap: 22 });
    L(455, 95, 'baaki ruke', C.bad);
    T(800, 175, '0 / sec', { c: C.bad, b: 1, size: 22 });
  } else {
    base('', '', 'good', 'good', 'INCR + EXPIRE', 'good');
    A(380, 95, 528, 160, C.good, { n: 2 }); A(380, 260, 528, 190, C.good, { n: 2 });
    L(615, 240, 'script chhoti, jhat se', C.good);
    A(700, 175, 780, 175, C.good, { n: 3, at: 0.35 }); dots(3, 800, 150, C.good, { gap: 25, cols: 1 });
  }
}
