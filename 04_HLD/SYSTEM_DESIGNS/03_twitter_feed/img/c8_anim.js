// Card 8: region me ek hi App, giri to region band. Ilaaj: kai App + ALB, App kuch yaad nahi rakhta.
// Agar: giri pata kaise, naya code (rolling), nayi App (dheere traffic). Wapas: App 2 theek, ALB ne joda.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: raat ko India region me ek hi App. Wo giri, poora India band'],
  [3, GRN, 'ILAAJ', 'ILAAJ: kai App, aage ALB baante. App 2 giri to bahar, baaki chalu'],
  [6, YEL, 'AGAR', 'AGAR App 2 giri, pata kaise? -> ALB har ~10 sec poochta "zinda ho?"'],
  [9, YEL, 'AGAR', 'AGAR naya code daalna, App band? -> nahi, ek-ek karke nikaalo, daalo, jodo'],
  [12, YEL, 'AGAR', 'AGAR nayi App judi, turant poori bheed? -> nahi, ALB dheere-dheere deta'],
  [15, GRN, 'WAPAS', 'WAPAS: App 2 theek hui, ALB ne wapas jod di'],
];
function setup() {}
const AY = [25, 150, 275];
// st: har App ka rang, sub: har App ka chhota naam
function apps(st, sub) {
  B(20, 145, 110, 70, 'India users');
  B(200, 150, 100, 60, 'ALB', '', { st: 'acc' });
  AY.forEach((y, i) => B(420, y, 130, 55, `App ${i + 1}`, sub[i] || '', { st: st[i] }));
  B(700, 150, 170, 60, 'Redis / DB', 'sab yahan');
}
function frame(ph) {
  if (ph === 0) {
    B(20, 145, 110, 70, 'India users');
    B(420, 150, 130, 60, 'Ek App', '', { st: 'bad' });
    cross(575, 180); num(615, 180, 1, C.bad); L(635, 185, 'giri', C.bad, 'start');
    A(130, 180, 418, 180, C.bad, { n: 2 }); L(275, 210, 'fail', C.bad);
    B(700, 150, 170, 60, 'Redis / DB');
    T(450, 300, 'poora India band', { c: C.bad, b: 1, size: 18 });
  } else if (ph === 1) {
    apps(['good', 'bad', 'good'], []);
    cross(575, 177); num(612, 177, 1, C.bad); L(630, 182, 'giri', C.bad, 'start');
    A(130, 180, 198, 180, C.msg, { n: 2 });
    A(300, 175, 418, 52, C.good, { n: 3 }); A(300, 185, 418, 302, C.good, { n: 3 });
    A(550, 52, 698, 170, C.good, { n: 4 }); A(550, 302, 698, 195, C.good, { n: 4 });
    L(785, 250, 'user ko farak nahi', C.good);
  } else if (ph === 2) {
    apps(['good', 'bad', 'good'], ['', 'chup', '']);
    A(130, 180, 198, 180, C.line);
    A(300, 180, 418, 177, C.acc, { dash: 1, n: 1 }); L(370, 158, 'zinda ho?', C.acc);
    cross(575, 177); num(612, 177, 2, C.bad); L(630, 182, 'chup', C.bad, 'start');
    A(300, 172, 418, 52, C.good, { n: 3 }); A(300, 188, 418, 302, C.good, { n: 3 });
    L(450, 345, 'hal: ALB har ~10 sec poochta, chup to bahar', C.good);
  } else if (ph === 3) {
    apps(['good', 'acc', ''], ['naya code', 'code daal rahe', 'purana']);
    num(410, 228, 1, C.acc); L(430, 233, 'bahar nikaali', C.acc, 'start');
    A(130, 180, 198, 180, C.msg, { n: 2 });
    A(300, 172, 418, 52, C.good, { n: 3 }); A(300, 188, 418, 302, C.good, { n: 3 });
    L(450, 345, 'hal: ek-ek karke nikaalo, daalo, jodo (rolling)', C.good);
  } else if (ph === 4) {
    apps(['good', 'good', 'acc'], ['', '', 'nayi']);
    num(580, 302, 1, C.acc); L(600, 307, 'data nahi: pehli dheemi', C.acc, 'start');
    A(130, 180, 198, 180, C.line);
    A(300, 172, 418, 52, C.good, { n: 2, w: 4 }); A(300, 180, 418, 177, C.good, { n: 2, w: 4 });
    A(300, 188, 418, 302, C.acc, { dash: 1, n: 3 }); L(330, 290, 'thoda-thoda', C.acc);
    L(720, 110, 'hal: ALB dheere-dheere traffic deta', C.good);
  } else {
    apps(['good', 'good', 'good'], ['', 'wapas', '']);
    num(580, 177, 1, C.good); L(600, 182, 'theek hui', C.good, 'start');
    A(130, 180, 198, 180, C.line);
    A(300, 172, 418, 52, C.good); A(300, 180, 418, 177, C.good, { n: 2 }); A(300, 188, 418, 302, C.good);
    L(370, 158, 'ALB ne joda', C.good);
  }
}
