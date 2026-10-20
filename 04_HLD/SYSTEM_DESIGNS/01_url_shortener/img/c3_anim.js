// Card 3: dono ko ek code. Ilaaj: Counter har App ko alag range. Agar: same code maanga, 400 pe crash, Counter gira. Wapas.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine aur Ravi ne link banaya, dono ko ek hi code mila'],
  [3, GRN, 'ILAAJ', 'ILAAJ: Counter har App ko alag range: App-1 ko 1-1000, App-2 ko 1001-2000'],
  [6, YEL, 'AGAR', 'AGAR do log ek saath same code maangein? -> DB ka UNIQUE ek ko rokta, 409'],
  [9, YEL, 'AGAR', 'AGAR App-1 range me 400 pe crash? -> 401 se 1000 bekaar, jaan-boojh ke'],
  [12, YEL, 'AGAR', 'AGAR Counter hi gir gaya? -> App ki range chalti rehti hai'],
  [15, GRN, 'WAPAS', 'WAPAS: Counter ki copy uth gayi, range khatam to agli range wahan se'],
];
function setup() {}
function people(n) {
  B(20, 40, 110, 60, 'Arpan'); B(20, 250, 110, 60, 'Ravi');
  A(130, 70, 228, 75, C.msg, { n }); A(130, 280, 228, 285, C.msg, { n });
}
function apps(st1, st2, s1, s2) {
  B(230, 40, 140, 70, 'App-1', s1, { st: st1 }); B(230, 250, 140, 70, 'App-2', s2, { st: st2 });
}
function db(st, sub) { B(720, 145, 140, 70, 'DB', sub, { st }); }
function counter(st, sub) { B(440, 145, 130, 70, 'Counter', sub, { st }); }
function frame(ph) {
  if (ph === 0) {
    people(1); apps('bad', 'bad', 'code: X', 'code: X'); db('bad', '');
    A(370, 75, 718, 165, C.bad, { n: 2 }); A(370, 285, 718, 195, C.bad, { n: 2 });
    num(790, 250, 3, C.bad); L(790, 280, 'ek code, do link', C.bad);
    L(545, 100, 'dono ne khud banaya', C.bad);
  } else if (ph === 1) {
    people(2); counter('acc', ''); db('good', 'code alag');
    apps('good', 'good', 'range 1-1000', 'range 1001-2000');
    A(470, 145, 372, 100, C.acc, { n: 1 }); A(470, 215, 372, 262, C.acc, { n: 1 });
    L(430, 185, 'range di', C.acc, 'end');
    A(370, 75, 718, 165, C.good, { n: 3, at: 0.6 }); L(600, 110, 'number: 1', C.good);
    A(370, 285, 718, 195, C.good, { n: 3, at: 0.6 }); L(600, 260, 'number: 1001', C.good);
  } else if (ph === 2) {
    people(1); apps('good', 'bad', '', ''); db('acc', 'UNIQUE');
    L(75, 125, 'same code maanga', C.msg);
    A(370, 75, 718, 165, C.good, { n: 2 }); A(370, 280, 718, 190, C.acc, { n: 2, at: 0.3 });
    A(718, 210, 372, 305, C.bad, { n: 3, at: 0.6 }); L(560, 290, '409', C.bad);
    L(600, 110, 'save', C.good);
    L(790, 250, 'hal: UNIQUE ek ko rokta', C.good);
  } else if (ph === 3) {
    people(0); db('', '');
    apps('bad', 'good', 'range 1-1000', 'range 1001-2000'); cross(395, 75);
    num(440, 50, 1, C.bad); L(460, 55, 'crash: 400 pe', C.bad, 'start');
    cells(230, 120, [['1-400', C.good], ['401-1000', C.bad]], { w: 80, h: 22 });
    L(400, 165, 'bekaar', C.bad, 'start');
    A(370, 285, 718, 195, C.good, { n: 2, at: 0.6 }); L(600, 260, 'App-2 chalti', C.good);
    L(720, 330, 'hal: chalta hai, 3.5 lakh crore me', C.good); L(720, 345, 'kuch hazaar se farak nahi', C.good);
  } else if (ph === 4) {
    people(0); counter('bad', ''); cross(597, 180);
    num(440, 135, 1, C.bad); L(460, 130, 'gira', C.bad, 'start');
    apps('good', 'good', 'range 1-1000', 'range 1001-2000'); db('good', '');
    A(370, 75, 718, 165, C.good, { n: 2, at: 0.6 }); A(370, 285, 718, 195, C.good, { n: 2, at: 0.6 });
    L(600, 110, 'apni range se', C.good);
    B(600, 270, 150, 60, 'Counter copy', 'uth rahi', { st: 'acc' });
    L(780, 345, 'hal: tab tak copy uth jaati', C.good);
  } else {
    people(0); counter('dim', ''); db('good', '');
    apps('good', 'acc', 'range 1-1000', 'range khatam');
    B(600, 270, 150, 60, 'Counter copy', 'uth gayi', { st: 'good' });
    num(675, 250, 1, C.good); L(695, 255, 'uthi', C.good, 'start');
    A(598, 300, 372, 300, C.acc, { n: 2 }); L(485, 330, 'agli range', C.acc);
    A(370, 75, 718, 165, C.good);
  }
}
