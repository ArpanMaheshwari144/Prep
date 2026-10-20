// Card 2: GET phir SET = do kadam, dono App ne 4 padha, 6 andar. Ilaaj: INCR ek kadam.
// Agar: INCR hua, EXPIRE se pehle crash -> key bina umar, user hamesha block. Hal: dono ek Lua me.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: GET phir SET, do kadam. Dono App ne 4 padha, dono andar, 6 ghus gaye'],
  [3, GRN, 'ILAAJ', 'ILAAJ: INCR = padhna + badhana ek hi kadam. Redis ek ke baad ek chalata'],
  [6, YEL, 'AGAR', 'AGAR INCR hua, EXPIRE se pehle crash? -> key bina umar, user hamesha block'],
];
function setup() {}
function bots() {
  B(20, 150, 110, 60, 'Bot', 'ek saath');
  A(130, 165, 248, 75, C.msg, { n: 1 }); A(130, 195, 248, 285, C.msg, { n: 2 });
}
function frame(ph) {
  if (ph === 0) {
    bots();
    B(250, 40, 150, 60, 'App-1', 'padha 4: andar', { st: 'bad' });
    B(250, 260, 150, 60, 'App-2', 'padha 4: andar', { st: 'bad' });
    B(620, 150, 190, 60, 'Redis', 'ginti: 4 -> 5', {});
    A(618, 160, 402, 58, C.acc, { n: 3, at: 0.15 }); L(500, 70, 'GET: 4', C.acc);
    A(618, 200, 402, 302, C.acc, { n: 4, at: 0.15 }); L(500, 290, 'GET: 4', C.acc);
    A(402, 88, 618, 175, C.bad, { n: 5, at: 0.25 }); L(470, 140, 'SET 5', C.bad);
    A(402, 272, 618, 188, C.bad, { n: 6, at: 0.25 }); L(470, 225, 'SET 5', C.bad);
    L(450, 350, 'dono ne 4 padha, dono andar. Ginti 5 likhi gayi, andar 6 gaye', C.bad);
  } else if (ph === 1) {
    bots();
    B(250, 40, 150, 60, 'App-1', '5: andar', { st: 'good' });
    B(250, 260, 150, 60, 'App-2', '6: 429', { st: 'bad' });
    B(620, 150, 190, 60, 'Redis', 'ginti: 6', { st: 'acc' });
    L(715, 135, 'ek ke baad ek', C.acc);
    A(402, 60, 618, 160, C.acc, { n: 3, at: 0.25 }); L(520, 85, 'INCR', C.acc);
    A(618, 175, 402, 88, C.good, { n: 4, at: 0.25 }); L(430, 155, 'ginti 5: andar', C.good);
    A(402, 272, 618, 188, C.acc, { n: 5, at: 0.25 }); L(520, 205, 'INCR', C.acc);
    A(618, 200, 402, 302, C.bad, { n: 6, at: 0.25 }); L(580, 300, 'ginti 6 > 5: mana', C.bad);
    L(450, 350, 'padhna + badhana ek kadam, beech me koi ghus nahi sakta', C.good);
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
