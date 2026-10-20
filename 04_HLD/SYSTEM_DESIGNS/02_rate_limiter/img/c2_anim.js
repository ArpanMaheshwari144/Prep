// Card 2: GET phir SET = do kadam, dono App ne 4 padha, 6 andar. Ilaaj: INCR ek kadam.
// Agar: INCR hua, EXPIRE se pehle crash -> key bina umar, user hamesha block. Hal: dono ek Lua me.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: had 5, andar pehle se 4. Do App ek saath aaye. Pal-pal dekho kya hua'],
  [3, GRN, 'ILAAJ', 'ILAAJ: INCR = padhna + badhana ek hi kadam. Redis ek ke baad ek chalata'],
  [6, YEL, 'AGAR', 'AGAR ginti badhi, par "60 sec baad mita dena" bolne se pehle crash? -> hamesha block'],
];
function setup() {}
function frame(ph) {
  if (ph === 0) {
    // movie jaisa: 4 frame, baayein se daayein, har frame ek pal. Har frame me Redis ki ginti abhi kitni
    const F = [
      ['pal 1', 'App-1 padhta', 1, 'acc', 4, 'App-1: 4 dikha', '4 < 5, andar aane do'],
      ['pal 2', 'App-2 padhta', 2, 'acc', 4, 'App-2: 4 dikha', 'App-1 ne abhi likha nahi'],
      ['pal 3', 'App-1 likhta', 1, 'bad', 5, 'App-1: 5 likha', 'pehla aadmi andar'],
      ['pal 4', 'App-2 likhta', 2, 'bad', 5, 'App-2: 5 likha', 'doosra bhi andar'],
    ];
    F.forEach(([pal, kya, who, st, val, l1, l2], k) => {
      const x = 15 + k * 220;
      el('rect', { x, y: 5, width: 205, height: 255, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
      num(x + 20, 25, k + 1, st === 'bad' ? C.bad : C.acc); T(x + 112, 30, pal + ': ' + kya, { c: C.fg, b: 1, size: 13 });
      B(x + 10, 50, 80, 40, 'App-1', '', { st: who === 1 ? st : 'dim' });
      B(x + 115, 50, 80, 40, 'App-2', '', { st: who === 2 ? st : 'dim' });
      B(x + 45, 165, 115, 55, 'Redis', 'ginti: ' + val, { st: 'acc' });
      const ax = who === 1 ? x + 50 : x + 155;
      if (st === 'acc') A(ax + (who === 1 ? 30 : -30), 163, ax, 92, C.acc); else A(ax, 92, ax + (who === 1 ? 30 : -30), 163, C.bad);
      T(who === 1 ? x + 150 : x + 55, 135, l1, { c: st === 'bad' ? C.bad : C.acc, b: 1, size: 12 }); // teer ki doosri taraf
      T(x + 102, 245, l2, { c: C.muted, size: 11 });
    });
    L(450, 300, 'Redis me 5 likha hai, par andar 6 gaye (had 5 thi)', C.bad);
    L(450, 335, 'galti: padhna aur likhna do alag kadam. Beech me doosra purana 4 padh gaya', C.acc);
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
