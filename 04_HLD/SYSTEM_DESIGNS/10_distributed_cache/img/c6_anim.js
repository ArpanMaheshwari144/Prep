// Card 6: IPL final, live:score key ki machine akeli mar rahi. Ilaaj: L1 local cache. Agar: garam key pe likhna -> like#0..#9.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: crore log ek hi key live:score padh rahe. Ek key = ek machine, A akeli mar rahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har App apni RAM me score ki 1-2 sec ki copy (L1). A tak 1-2 sec me ek baar'],
  [6, YEL, 'AGAR', 'AGAR garam key pe likhna bhi (like)? -> tukde like#0..#9, kisi ek me likho, padhte jodo'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'log → App', 'IPL final: crore log har second score maang rahe'],
   [C.bad, 'App → A', 'Score ek hi key live:score, wo sirf machine A pe, sab A pe gaye'],
   [C.bad, 'A', 'A ka CPU full, jabki B aur C khaali baithe hain']],
  [[C.good, 'log → App', 'Har App score apni RAM (L1) se deta, A tak jaata hi nahi'],
   [C.acc, 'App → A', 'App 1-2 sec me sirf ek baar A se taaza score laata'],
   [C.good, 'A', 'A pe bojh crore se ghat ke kuch hazaar, aaram se']],
  [[C.acc, 'App → like#k', 'Har like kisi ek random tukde me likho: like#0 se like#9'],
   [C.good, 'A, B, C', 'Tukde alag machines pe, likhai teeno me bat gayi'],
   [C.good, 'tukde → jod', 'Padhte waqt saare tukde jodo, wahi total likes']],
];
function frame(ph) {
  if (ph === 0) {
    dots(20, 22, 130, C.msg, { gap: 14, cols: 5 }); L(55, 215, 'crore log', C.msg);
    [30, 140, 250].forEach((y, i) => B(170, y, 150, 55, 'App ' + (i + 1), '', { icon: 'server' }));
    A(98, 150, 168, 168, C.msg, { n: 1 });
    B(600, 30, 200, 55, 'B', 'khaali', { st: 'dim' }); B(600, 250, 200, 55, 'C', 'khaali', { st: 'dim' });
    B(600, 135, 200, 65, 'A', 'live:score', { st: 'bad' }); bar(600, 210, 200, 12, 1, '');
    A(320, 58, 598, 150, C.bad); A(320, 168, 598, 168, C.bad, { n: 2 }); A(320, 278, 598, 186, C.bad);
    num(830, 168, 3, C.bad); L(700, 120, 'CPU 100%', C.bad);
  } else if (ph === 1) {
    dots(20, 22, 130, C.msg, { gap: 14, cols: 5 }); L(55, 215, 'crore log', C.msg);
    [30, 140, 250].forEach((y, i) => B(150, y, 200, 55, 'App ' + (i + 1), 'L1: score (1 sec)', { st: 'good', icon: 'server' }));
    A(98, 150, 148, 168, C.good, { n: 1 });
    B(600, 30, 200, 55, 'B', '', { st: 'dim' }); B(600, 250, 200, 55, 'C', '', { st: 'dim' });
    B(600, 135, 200, 65, 'A', 'live:score: aaram', { st: 'good' }); num(830, 168, 3, C.good);
    A(350, 58, 598, 150, C.acc, { dash: 1 }); A(350, 168, 598, 168, C.acc, { n: 2, dash: 1 }); A(350, 278, 598, 186, C.acc, { dash: 1 });
    L(700, 112, '1-2 sec me ek baar', C.acc);
    L(450, 340, 'score 1-2 sec purana chalta, paisa / stock me nahi', C.muted);
  } else {
    B(15, 135, 150, 65, 'App', '+1 like', { icon: 'server' });
    B(330, 15, 250, 95, 'A', '', { st: 'good' }); cells(342, 60, [['like#0'], ['like#1'], ['like#2'], ['like#3']], { w: 56, h: 32 });
    B(330, 128, 250, 95, 'B', '', { st: 'good' }); cells(342, 173, [['like#4'], ['like#5'], ['like#6']], { w: 56, h: 32 });
    B(330, 241, 250, 95, 'C', '', { st: 'good' }); cells(342, 286, [['like#7'], ['like#8', C.acc], ['like#9']], { w: 56, h: 32 });
    AP([[90, 202], [90, 347], [426, 347], [426, 322]], C.acc, { n: 1, at: 0.35 }); L(230, 330, 'random tukda: #8', C.acc);
    num(305, 175, 2, C.good);
    B(690, 135, 190, 65, 'jod', 'total likes', { st: 'good' });
    A(582, 62, 688, 150, C.good); A(582, 175, 688, 168, C.good, { n: 3 }); A(582, 288, 688, 186, C.good);
  }
}
