// Card 1: do App, apni-apni ginti, 9 login andar. Ilaaj: sab App ek Redis ginti (60 sec umar).
// Agar: window ke kinare pe 5 + 5 -> 2 sec me 10 andar.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: had 5 thi. Do App, har App ki apni ginti, 9 login andar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: dono App ek hi Redis ginti dekhte hain. 6thi request pe 429'],
  [6, YEL, 'AGAR', 'AGAR window ke kinare pe 5 + 5 aayi? -> 2 sec me 10 andar'],
];
function setup() {}
// Arpan -> LB -> App-1 / App-2 (dono scene me same jagah)
function front(s1, s2, st) {
  B(20, 140, 110, 60, 'Arpan', '9 login'); B(190, 150, 80, 40, 'LB');
  A(130, 170, 188, 170, C.msg, { n: 1 });
  B(330, 40, 150, 60, 'App-1', s1, { st }); B(330, 250, 150, 60, 'App-2', s2, { st });
  A(270, 160, 328, 75, C.msg, { n: 2 }); L(284, 112, '5 bheje', C.msg, 'end');
  A(270, 180, 328, 275, C.msg, { n: 3 }); L(284, 232, '4 bheje', C.msg, 'end');
}
function frame(ph) {
  if (ph === 0) {
    front('apni ginti: 5', 'apni ginti: 4', 'bad');
    B(680, 140, 160, 60, 'Login', 'andar: 9', { st: 'bad' });
    A(480, 70, 678, 160, C.bad, { n: 4 }); L(600, 95, '5 <= 5, andar', C.bad);
    A(480, 280, 678, 185, C.bad, { n: 5 }); L(600, 260, '4 <= 5, andar', C.bad);
    L(450, 340, 'had 5 thi, par har App ne sirf apni ginti dekhi: 9 andar', C.bad);
  } else if (ph === 1) {
    front('', '', '');
    B(620, 135, 220, 70, 'Redis', 'rate:Arpan:login = 9', { st: 'acc' });
    L(730, 120, 'ek hi ginti, umar 60 sec', C.acc);
    A(480, 70, 618, 150, C.acc, { n: 4 }); L(560, 90, 'ginti +1', C.acc);
    A(480, 262, 618, 190, C.acc, { n: 5 }); L(520, 205, 'ginti +1', C.acc);
    AP([[760, 207], [760, 295], [482, 295]], C.bad, { n: 6, at: 0.4 }); L(775, 250, '6 > 5: mana', C.bad, 'start');
    AP([[405, 312], [405, 345], [75, 345], [75, 202]], C.bad, { n: 7, at: 0.5 }); L(240, 330, '429', C.bad);
    L(470, 349, 'pehli 5 andar, baaki 4 ko 429', C.good, 'start');
  } else {
    // time ki line, minute 10:00 | 10:01
    el('line', { x1: 60, y1: 200, x2: 840, y2: 200, stroke: C.line, 'stroke-width': 2 }, DYN);
    el('line', { x1: 450, y1: 120, x2: 450, y2: 250, stroke: C.acc, 'stroke-width': 3 }, DYN);
    T(255, 240, 'minute 10:00 (had 5)', { c: C.muted, size: 13 });
    T(645, 240, 'minute 10:01 (had 5)', { c: C.muted, size: 13 });
    T(450, 275, '10:01:00', { c: C.acc, b: 1, size: 14 });
    num(330, 120, 1, C.good); dots(5, 355, 180, C.good, { gap: 18 }); L(330, 95, '10:00:59 pe 5', C.good);
    num(450, 95, 2, C.acc); L(450, 70, 'ginti 0 se', C.acc);
    num(570, 120, 3, C.good); dots(5, 470, 180, C.good, { gap: 18 }); L(570, 95, '10:01:01 pe 5', C.good);
    el('path', { d: 'M350 160 L350 145 L555 145 L555 160', fill: 'none', stroke: C.bad, 'stroke-width': 3 }, DYN);
    T(450, 320, '2 sec me 10 andar, had 5 thi', { c: C.bad, b: 1, size: 16 });
    L(450, 350, 'isliye asli me token bucket (upar "Kaunsa algo")', C.good);
  }
}
