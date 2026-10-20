// Card 2: GET phir SET = do kadam, dono App ne 4 padha, 6 andar. Ilaaj: INCR ek kadam.
// Agar: INCR hua, EXPIRE se pehle crash -> key bina umar, user hamesha block. Hal: dono ek Lua me.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ginti padhna aur badhana do alag kadam. Beech me doosra bhi wahi padh leta'],
  [3, GRN, 'ILAAJ', 'ILAAJ: INCR = padhna + badhana ek hi kadam. Redis ek ke baad ek chalata'],
  [6, YEL, 'AGAR', 'AGAR ginti badhi, par "60 sec baad mita dena" bolne se pehle crash? -> hamesha block'],
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
    // App ne ginti badhai, "60 sec baad mita dena" bhejne se pehle App crash
    B(20, 40, 150, 70, 'App', 'yahin crash', { st: 'bad' });
    B(380, 40, 240, 70, 'Redis', 'ginti 6, kabhi nahi mitegi', { st: 'bad' });
    A(170, 60, 378, 60, C.good, { n: 1 }); L(275, 45, 'ginti +1', C.good);
    A(170, 95, 378, 95, C.bad, { n: 2, dash: 1, at: 0.3 }); cross(320, 95);
    L(275, 135, '"60 sec baad mita dena" pahuncha hi nahi', C.bad);
    B(380, 250, 240, 60, 'Arpan', 'agle din');
    A(470, 248, 470, 112, C.msg, { n: 3 }); L(455, 190, 'login', C.msg, 'end');
    A(530, 112, 530, 248, C.bad, { n: 4 }); L(545, 190, '6 > 5: 429', C.bad, 'start');
    L(500, 340, 'aaj, kal, har din block', C.bad);
    B(660, 150, 210, 80, 'hal: Lua script', 'dono kaam ek saath bhejo', { st: 'good' });
    L(765, 255, 'ya dono hon, ya koi nahi', C.good);
  }
}
