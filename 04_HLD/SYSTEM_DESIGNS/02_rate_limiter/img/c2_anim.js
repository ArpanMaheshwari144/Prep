// Card 2: GET phir SET = do kadam, dono App ne 4 padha, 6 andar. Ilaaj: INCR ek kadam.
// Agar: INCR hua, EXPIRE se pehle crash -> key bina umar, user hamesha block. Hal: dono ek Lua me.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ginti padhna aur badhana do alag kadam. Beech me doosra bhi wahi padh leta'],
  [3, GRN, 'ILAAJ', 'ILAAJ: INCR = padhna + badhana ek hi kadam. Redis ek ke baad ek chalata'],
  [6, YEL, 'AGAR', 'AGAR INCR hua, EXPIRE se pehle crash? -> key bina umar, user hamesha block'],
];
function setup() {}
function frame(ph) {
  if (ph === 0) {
    // App-1 baayein, Redis beech, App-2 daayein: koi teer nahi katta
    B(20, 130, 180, 80, 'App-1', '4 < 5: andar', { st: 'bad' });
    B(360, 130, 180, 80, 'Redis', 'ginti: 4', {});
    B(700, 130, 180, 80, 'App-2', '4 < 5: andar', { st: 'bad' });
    L(450, 110, 'pehle se 4 andar, had 5', C.muted);
    A(358, 150, 202, 150, C.acc, { n: 1 }); L(280, 135, 'padha: 4', C.acc);
    A(542, 150, 698, 150, C.acc, { n: 2 }); L(620, 135, 'padha: 4', C.acc);
    A(202, 190, 358, 190, C.bad, { n: 3 }); L(280, 220, 'likha: 5', C.bad);
    A(698, 190, 542, 190, C.bad, { n: 4 }); L(620, 220, 'likha: 5', C.bad);
    L(450, 280, 'App-2 ne padha tab tak App-1 ne 5 likha hi nahi tha', C.acc);
    L(450, 335, 'dono andar: ginti 5 dikhi, par andar 6 (had 5 thi)', C.bad);
  } else if (ph === 1) {
    B(20, 130, 180, 80, 'App-1', '5: andar', { st: 'good' });
    B(360, 130, 180, 80, 'Redis', 'ginti: 4 -> 5 -> 6', { st: 'acc' });
    B(700, 130, 180, 80, 'App-2', '6: 429', { st: 'bad' });
    L(450, 110, 'ek ke baad ek', C.acc);
    A(202, 150, 358, 150, C.acc, { n: 1 }); L(280, 135, 'INCR', C.acc);
    A(358, 190, 202, 190, C.good, { n: 2 }); L(280, 220, 'ginti 5: andar', C.good);
    A(698, 150, 542, 150, C.acc, { n: 3 }); L(620, 135, 'INCR', C.acc);
    A(542, 190, 698, 190, C.bad, { n: 4 }); L(620, 220, 'ginti 6 > 5: 429', C.bad);
    L(450, 300, 'INCR = padhna + badhana ek hi kadam', C.good);
    L(450, 330, 'beech me koi doosra ghus hi nahi sakta', C.good);
  } else {
    B(300, 20, 300, 120, 'Redis', '', {});
    cells(325, 70, [['INCR', C.good], ['EXPIRE', C.bad]], { w: 120, h: 34 });
    num(385, 125, 1, C.good); L(385, 160, 'ho gaya', C.good);
    cross(585, 50); num(505, 125, 2, C.bad); L(505, 160, 'crash, hua hi nahi', C.bad);
    B(330, 210, 240, 60, 'rate:Arpan', 'umar: kabhi nahi', { st: 'bad' }); num(450, 192, 3, C.bad);
    B(20, 210, 110, 60, 'Arpan', 'aaj, kal...');
    A(328, 240, 132, 240, C.bad, { n: 4, at: 0.5 }); L(215, 290, 'har baar 429', C.bad);
    B(650, 40, 220, 60, 'Lua script', 'INCR + EXPIRE ek saath', { st: 'good' });
    L(760, 130, 'hal: ya dono, ya koi nahi', C.good);
    L(450, 340, 'key ban gayi par umar kabhi lagi hi nahi, to kabhi mitegi nahi', C.bad);
  }
}
