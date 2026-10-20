// Card 3: dono App ko ek hi code. Ilaaj: Counter range. Agar: Counter gira.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: dono App ne DB se "agla number?" poocha, dono ko 501 mila'],
  [3, GRN, 'ILAAJ', 'ILAAJ: Counter har App ko alag range deta, App apni range se number leta'],
  [6, YEL, 'AGAR', 'AGAR Counter hi gir gaya? -> App ki range chalti rehti, tab tak copy uth jaati'],
];
function setup() {}
// Main/Ravi (x 20-130), App-1/App-2 (x 250-380), DB/Counter (x 560-700)
function ppl() { B(20, 40, 110, 50, 'Main'); B(20, 260, 110, 50, 'Ravi'); }
function apps(s1, s2, st) { B(250, 40, 130, 50, 'App-1', s1, { st }); B(250, 260, 130, 50, 'App-2', s2, { st }); }
function req(n) { A(130, 52, 248, 52, C.msg, { n }); A(130, 272, 248, 272, C.msg); }
function code(n, a, b, c) {
  A(248, 80, 132, 80, c, { n }); A(248, 300, 132, 300, c);
  T(190, 112, a, { c, b: 1, size: 14 }); T(190, 332, b, { c, b: 1, size: 14 });
}
function frame(ph) {
  if (ph === 0) {
    ppl(); req(1); apps('', '');
    B(560, 150, 140, 60, 'DB', 'agla = 501', { st: 'bad' });
    A(380, 60, 558, 160, C.acc, { n: 2, at: 0.3 }); A(380, 290, 558, 200, C.acc);
    L(480, 85, 'agla no.?', C.acc); L(480, 285, 'agla no.?', C.acc);
    A(558, 175, 382, 85, C.bad, { n: 3, at: 0.6 }); A(558, 185, 382, 275, C.bad);
    code(4, 'code 501', 'code 501', C.bad);
    cross(740, 180); L(790, 165, 'ek hi code', C.bad, 'start'); L(790, 190, 'do link ko', C.bad, 'start');
  } else if (ph === 1) {
    ppl(); req(2); apps('range 1-1000', 'range 1001-2000', 'good');
    B(560, 150, 140, 60, 'Counter', 'ZooKeeper / DB', { st: 'acc' });
    A(558, 165, 382, 70, C.acc, { n: 1, at: 0.5, dash: 1 }); A(558, 195, 382, 290, C.acc, { dash: 1 });
    L(480, 95, 'pehle hi: 1-1000', C.acc); L(480, 275, 'pehle hi: 1001-2000', C.acc);
    code(3, 'code 1', 'code 1001', C.good);
    L(630, 335, 'ek range kabhi do App ko nahi. 1000 link pe Counter ki ek call', C.good);
  } else {
    ppl(); req(2); apps('abhi 400 / 1000', 'abhi 1300 / 2000', 'good');
    B(560, 150, 140, 60, 'Counter', '', { st: 'bad' }); cross(720, 180); num(630, 230, 1, C.bad); L(630, 255, 'gira', C.bad);
    A(380, 70, 558, 160, C.muted, { dash: 1 }); A(380, 280, 558, 200, C.muted, { dash: 1 }); cross(470, 180);
    
    code(3, 'code 401', 'code 1301', C.good);
    L(20, 190, 'range pehle se App ke paas, usi se agla number.', C.good, 'start'); L(20, 210, 'Counter se kuch nahi maanga', C.good, 'start');
    B(740, 40, 140, 60, 'Counter copy', 'uth rahi', { st: 'acc' }); A(640, 148, 740, 100, C.acc, { n: 4, dash: 1 });
    L(630, 300, 'range khatam hone se pehle copy taiyaar', C.muted);
  }
}
