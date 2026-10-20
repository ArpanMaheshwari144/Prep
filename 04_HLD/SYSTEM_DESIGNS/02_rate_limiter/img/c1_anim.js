// Card 1: do App, apni-apni ginti, 9 login andar. Ilaaj: sab App ek Redis ginti (60 sec umar).
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: do App, dono ki apni ginti. 9 login, saari andar'],
  [3, GRN, 'ILAAJ', 'ILAAJ: sab App ek hi Redis ginti dekhte hain. 6thi request pe 429'],
  [6, YEL, 'AGAR', 'AGAR window ke kinare pe 5 + 5 aayi? -> 2 sec me 10 andar'],
  [9, GRN, 'WAPAS', 'WAPAS: 60 sec baad key mit gayi, ginti phir 0 se'],
];
function setup() {}
function apps(st1, st2, s1, s2) {
  B(20, 140, 100, 70, 'Arpan', '9 login'); B(170, 150, 70, 50, 'LB');
  A(120, 175, 168, 175, C.msg, { n: 1, at: 0.45 });
  A(240, 165, 318, 100, C.msg, { n: 2 }); A(240, 185, 318, 260, C.msg, { n: 2 });
  B(320, 60, 130, 70, 'App-1', s1, { st: st1 }); B(320, 225, 130, 70, 'App-2', s2, { st: st2 });
}
function frame(ph) {
  if (ph === 0) {
    apps('bad', 'bad', 'ginti: 5', 'ginti: 4');
    A(450, 95, 478, 95, C.good, { n: 3, at: 0 }); dots(5, 500, 95, C.good);
    L(470, 72, 'apni ginti: 5 <= 5, andar', C.good, 'start');
    A(450, 260, 478, 260, C.good, { n: 3, at: 0 }); dots(4, 500, 260, C.good);
    L(470, 237, 'apni ginti: 4 <= 5, andar', C.good, 'start');
    T(770, 170, 'andar: 9', { c: C.bad, b: 1, size: 26 });
    T(770, 205, 'had: 5', { c: C.muted, size: 15 });
  } else if (ph === 1) {
    apps('', '', '', '');
    B(530, 135, 170, 80, 'Redis', 'ginti: 9', { st: 'acc' });
    A(450, 95, 528, 160, C.acc, { n: 3 }); A(450, 260, 528, 190, C.acc, { n: 3 });
    A(700, 160, 735, 140, C.good, { n: 4, at: 0 }); dots(5, 750, 140, C.good, { gap: 26 });
    A(700, 190, 735, 215, C.bad, { n: 5, at: 0 }); dots(4, 750, 215, C.bad, { gap: 26 });
    T(800, 115, 'andar: 5', { c: C.good, b: 1, size: 15 });
    T(800, 250, '429: 4', { c: C.bad, b: 1, size: 15 });
  } else if (ph === 2) {
    el('rect', { x: 60, y: 120, width: 390, height: 120, fill: '#1a2430', stroke: C.line }, DYN);
    el('rect', { x: 450, y: 120, width: 390, height: 120, fill: '#1a2430', stroke: C.line }, DYN);
    T(255, 300, 'minute 1: 5 / 5', { c: C.muted, size: 14 });
    T(645, 300, 'minute 2: 5 / 5', { c: C.muted, size: 14 });
    el('line', { x1: 450, y1: 100, x2: 450, y2: 260, stroke: C.acc, 'stroke-width': 3 }, DYN);
    T(450, 280, '10:01:00', { c: C.acc, b: 1, size: 14 });
    L(462, 140, 'ginti 0 se', C.acc, 'start');
    num(330, 180, 1, C.good); dots(5, 350, 205, C.good, { gap: 20 });
    num(570, 180, 2, C.good); dots(5, 470, 205, C.good, { gap: 20 });
    el('path', { d: 'M340 90 L340 75 L560 75 L560 90', fill: 'none', stroke: C.bad, 'stroke-width': 3 }, DYN);
    T(450, 60, '2 sec: 10 andar', { c: C.bad, b: 1, size: 16 });
    L(450, 335, 'hal: isliye asli me token bucket', C.good);
  } else {
    B(80, 110, 220, 80, 'rate:Arpan:login', 'ginti: 5', { st: 'dim' });
    A(310, 150, 570, 150, C.muted, { dash: 1, n: 1, at: 0.2 });
    T(440, 135, '60 sec', { c: C.muted, size: 14 });
    L(440, 175, 'key mit gayi', C.muted);
    B(580, 110, 220, 80, 'rate:Arpan:login', 'ginti: 0', { st: 'good' });
    A(690, 190, 690, 235, C.good, { n: 2 }); dot([690, 255], C.good); T(690, 290, 'andar', { c: C.good, b: 1, size: 14 });
  }
}
