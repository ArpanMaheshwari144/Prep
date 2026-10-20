// Card 2: ek App giri, site band. Ilaaj: kai App + LB, stateless. Agar: App dheemi. Wapas: /health haan, list me wapas.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek hi App machine, wo giri, poori site band'],
  [3, GRN, 'ILAAJ', 'ILAAJ: kai App, aage LB baari-baari bhejta. App stateless, koi bhi click le'],
  [6, YEL, 'AGAR', 'AGAR App giri nahi, bas dheemi? -> kam connection wali ko bhejo'],
  [9, GRN, 'WAPAS', 'WAPAS: giri App theek hui, /health haan, LB use wapas list me leta'],
];
function setup() {}
const YS = [30, 150, 270];
function apps(sts, subs) {
  B(20, 150, 100, 70, 'Users', 'click'); A(120, 185, 198, 185, C.msg);
  B(200, 155, 90, 60, 'LB', '');
  YS.forEach((y, i) => B(450, y, 130, 60, `App-${i + 1}`, subs[i] || '', { st: sts[i] }));
  B(730, 150, 140, 60, 'Redis / DB', 'sab yahan');
}
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 100, 70, 'Users', 'click');
    B(450, 150, 130, 70, 'App', 'ek hi', { st: 'bad' }); cross(610, 185);
    num(480, 125, 1, C.bad); L(500, 130, 'giri', C.bad, 'start');
    A(120, 185, 448, 185, C.bad, { dash: 1, n: 2 }); L(284, 170, 'koi jawab nahi', C.bad);
    T(760, 192, 'site band', { c: C.bad, b: 1, size: 24 });
  } else if (ph === 1) {
    apps(['good', 'bad', 'good'], ['', 'giri', '']);
    cross(605, 180); num(515, 135, 1, C.bad); L(535, 140, '/health: chup', C.bad, 'start');
    A(290, 175, 448, 60, C.good, { n: 2 }); A(290, 205, 448, 300, C.good, { n: 2 });
    A(290, 190, 448, 180, C.line, { dash: 1 }); L(370, 170, 'list se hata', C.muted);
    L(330, 95, 'baari-baari', C.good, 'end');
    A(580, 60, 728, 170, C.good, { n: 3, at: 0.4 }); A(580, 300, 728, 190, C.good, { n: 3, at: 0.4 });
    L(800, 240, 'koi bhi App', C.good); L(800, 258, 'koi bhi click', C.good);
  } else if (ph === 2) {
    apps(['', 'acc', 'good'], ['', 'dheemi', '']);
    dots(2, 600, 60, C.msg, { gap: 20 });
    num(600, 135, 1, C.acc); dots(6, 625, 170, C.acc, { gap: 18, cols: 3 }); L(625, 140, 'connection bhare', C.acc, 'start');
    dots(1, 600, 300, C.good);
    A(290, 175, 448, 60, C.line); A(290, 190, 448, 180, C.line, { dash: 1 });
    A(290, 205, 448, 300, C.good, { n: 2 }); L(400, 250, 'kam connection', C.good, 'start');
    L(735, 290, 'hal: least-conn, jiske kam', C.good); L(735, 308, 'connection khule, use bhejo', C.good);
  } else {
    apps(['good', 'good', 'good'], ['', 'theek', '']);
    A(290, 178, 448, 173, C.acc, { dash: 1, n: 1 }); L(370, 158, '/health?', C.acc);
    A(448, 198, 292, 198, C.good, { n: 2 }); L(370, 225, 'haan', C.good);
    A(290, 170, 448, 60, C.good); A(290, 210, 448, 300, C.good);
    num(195, 285, 3, C.good); L(215, 290, 'wapas list me', C.good, 'start');
    A(580, 60, 728, 170, C.line); A(580, 180, 728, 180, C.line); A(580, 300, 728, 190, C.line);
  }
}
