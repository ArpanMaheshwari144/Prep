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
function base(s1, s2, st1, st2, rs, rst) {
  B(20, 140, 90, 80, 'Bot', '20 login'); A(110, 170, 248, 100); A(110, 190, 248, 260);
  B(250, 60, 130, 70, 'App-1', s1, { st: st1 }); B(250, 225, 130, 70, 'App-2', s2, { st: st2 });
  B(530, 135, 170, 80, 'Redis', rs, { st: rst });
}
function frame(ph) {
  if (ph === 0) {
    base('padha: 4', 'padha: 4', 'bad', 'bad', 'ginti: 5', '');
    A(380, 95, 528, 160, C.acc); A(380, 260, 528, 190, C.acc);
    T(455, 112, 'SET 5', { c: C.acc, b: 1, size: 13, halo: 1 }); T(455, 252, 'SET 5', { c: C.acc, b: 1, size: 13, halo: 1 });
    T(800, 170, 'andar: 6', { c: C.bad, b: 1, size: 24 }); T(800, 200, 'had: 5', { c: C.muted, size: 14 });
  } else if (ph === 1) {
    base('5: andar', '6: 429', 'good', 'bad', 'ginti: 6', 'acc');
    A(380, 95, 528, 160, C.good); A(380, 260, 528, 190, C.bad);
    T(455, 112, 'INCR', { c: C.acc, b: 1, size: 13, halo: 1 }); T(455, 252, 'INCR', { c: C.acc, b: 1, size: 13, halo: 1 });
    T(800, 170, 'andar: 5', { c: C.good, b: 1, size: 24 }); T(800, 200, 'had: 5', { c: C.muted, size: 14 });
  } else if (ph === 2) {
    B(20, 140, 110, 70, 'Arpan');
    B(420, 30, 180, 70, 'Redis', '', { st: 'bad' }); cross(625, 65);
    cells(420, 130, [['INCR', C.good], ['EXPIRE', C.bad]], { w: 86, h: 30 });
    B(420, 230, 180, 70, 'rate:Arpan', 'umar: nahi', { st: 'bad' });
    A(130, 175, 410, 260, C.bad);
    dots(3, 200, 205, C.bad, { gap: 60 });
    T(260, 300, 'aaj  kal  parso: 429', { c: C.bad, b: 1, size: 14 });
  } else if (ph === 3) {
    dots(9, 120, 175, C.msg, { gap: 34 });
    A(420, 175, 498, 175, C.bad);
    B(500, 120, 220, 110, 'Redis', 'Lua: loop', { st: 'bad' });
    T(255, 135, 'sab ruki', { c: C.bad, b: 1, size: 15 });
    T(810, 175, '0 / sec', { c: C.bad, b: 1, size: 20 });
  } else {
    dots(3, 320, 175, C.msg, { gap: 34 });
    A(420, 175, 498, 175, C.good);
    B(500, 120, 220, 110, 'Redis', 'Lua: chhoti', { st: 'good' });
    A(720, 175, 790, 175, C.good);
    dots(3, 810, 150, C.good, { gap: 26, cols: 1 });
  }
}
