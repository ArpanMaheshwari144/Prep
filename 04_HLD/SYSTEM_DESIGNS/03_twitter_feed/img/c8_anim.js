// Card 8: har region me ek hi App, India wali giri, India band. Ilaaj: kai App + ALB, App kuch yaad nahi rakhta.
// Agar: naya code daalna -> ek-ek App badlo (rolling), baaki bheed sambhaalti.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: India region me ek hi App machine. Wo giri, poora India band'],
  [3, GRN, 'ILAAJ', 'ILAAJ: kai App, aage ALB. App kuch yaad nahi rakhta, sab Redis / DB me'],
  [6, YEL, 'AGAR', 'AGAR naya code daalna hai, App band karein? -> nahi, ek-ek karke badlo (rolling)'],
];
function setup() {}
function apps(st, sub) {
  ['App-1', 'App-2', 'App-3'].forEach((n, i) => B(420, 30 + i * 105, 160, 65, n, sub[i], { st: st[i] }));
}
const STEPS = [
  [[C.bad, 'India users → App', 'ek hi machine'], [C.bad, 'App giri', 'poora India band']],
  [[C.msg, 'India users → ALB', 'har request pehle ALB pe'], [C.good, 'ALB → App-1, App-3', 'App-2 giri: /health fail, bahar.|Baaki pe request']],
  [[C.good, 'App-1', 'pehle App-1 pe naya code'], [C.acc, 'App-2', 'ab App-2 bahar, badal rahe'], [C.good, 'ALB → App-1, App-3', 'bheed baaki App sambhaal leti']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 140, 60, 'India users', '');
    B(420, 130, 180, 80, 'App', 'ek hi', { st: 'bad' }); cross(625, 170);
    A(160, 170, 418, 170, C.bad, { n: 1, dash: 1 }); num(510, 245, 2, C.bad); L(510, 270, 'giri', C.bad);
    L(450, 330, 'ek machine = ek kamzor jagah. Wahi giri to sab band', C.bad);
  } else if (ph === 1) {
    B(20, 140, 130, 60, 'India users', '');
    B(220, 140, 110, 60, 'ALB', '/health');
    apps(['good', 'bad', 'good'], ['', 'giri', '']); cross(605, 167);
    B(700, 135, 170, 70, 'Redis / DB', 'data yahan', { st: 'acc' });
    A(150, 170, 218, 170, C.msg, { n: 1 });
    A(330, 160, 418, 62, C.good, { n: 2 }); A(330, 185, 418, 272, C.good);
    A(330, 172, 418, 167, C.bad, { dash: 1 }); L(372, 150, 'App-2 bahar', C.bad);
    A(580, 62, 698, 150, C.muted); A(580, 272, 698, 190, C.muted);
    L(450, 345, 'koi bhi App gire ya jude, user ko farak nahi', C.good);
  } else {
    B(20, 140, 130, 60, 'India users', '');
    B(220, 140, 110, 60, 'ALB', '');
    apps(['good', 'acc', 'good'], ['naya code', 'badal rahe', 'purana code']);
    A(150, 170, 218, 170, C.msg);
    A(330, 160, 418, 62, C.good, { n: 3 }); A(330, 185, 418, 272, C.good);
    num(610, 62, 1, C.good); L(630, 67, 'pehle App-1 badla', C.good, 'start');
    num(610, 167, 2, C.acc); L(630, 172, 'ab App-2 (bahar)', C.acc, 'start');
    L(450, 345, 'bheed baaki App sambhaal leti. Ek-ek karke sab naye, koi band nahi', C.good);
  }
}
