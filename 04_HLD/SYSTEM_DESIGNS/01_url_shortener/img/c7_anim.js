// Card 7: LB gira. Ilaaj: 2 LB + Route 53 health check. Agar: phone ne purana pata yaad rakha.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek hi LB, wo gira to App kitne bhi hon, koi request andar nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: do LB, aage Route 53 har 30 sec poochta. Chup raha to list se bahar'],
  [6, YEL, 'AGAR', 'AGAR phone ne purana pata yaad rakha? -> ~60 sec kuch request fail (DNS TTL)'],
];
function setup() {}
function apps(st) { B(740, 60, 110, 50, 'App-1', '', { st }); B(740, 200, 110, 50, 'App-2', '', { st }); }
function frame(ph) {
  if (ph === 0) {
    B(20, 110, 110, 60, 'Phone'); B(220, 110, 110, 60, 'DNS');
    A(130, 130, 218, 130, C.msg, { n: 1 }); A(218, 150, 132, 150, C.msg, { n: 2 }); L(175, 185, 'LB-1 ka pata', C.msg);
    B(480, 110, 120, 60, 'LB-1', 'ek hi', { st: 'bad' }); cross(625, 140); L(540, 195, 'gira', C.bad);
    A(130, 165, 478, 165, C.bad, { n: 3, dash: 1, at: 0.7 });
    apps('dim'); A(600, 130, 738, 85, C.muted, { dash: 1 }); A(600, 150, 738, 225, C.muted, { dash: 1 });
    L(540, 300, 'App zinda, par koi request andar nahi', C.bad);
  } else if (ph === 1) {
    B(20, 110, 110, 60, 'Phone'); B(220, 100, 130, 70, 'Route 53', 'health check', { st: 'acc' });
    B(480, 30, 120, 60, 'LB-1', '', { st: 'bad' }); cross(625, 60);
    B(480, 220, 120, 60, 'LB-2', '', { st: 'good' });
    A(350, 115, 478, 60, C.acc, { n: 1, dash: 1, at: 0.4 }); L(330, 45, '/health? 3 baar chup', C.acc, 'start');
    A(350, 140, 478, 240, C.acc, { n: 1, dash: 1, at: 0.4 }); L(400, 215, '/health? haan', C.good);
    L(540, 110, 'list se bahar', C.bad);
    A(130, 130, 218, 130, C.msg, { n: 2 }); A(218, 150, 132, 150, C.good, { n: 3 }); L(175, 95, 'LB-2 ka pata', C.good);
    A(75, 170, 478, 268, C.good, { n: 4, at: 0.6 });
    apps(''); A(600, 240, 738, 95, C.good); A(600, 255, 738, 225, C.good);
  } else {
    B(20, 110, 110, 60, 'Phone', 'yaad: LB-1'); B(220, 100, 130, 70, 'Route 53', 'ab: LB-2', { st: 'acc' });
    B(480, 30, 120, 60, 'LB-1', '', { st: 'bad' }); cross(625, 60);
    B(480, 220, 120, 60, 'LB-2', '', { st: 'good' });
    A(130, 125, 478, 65, C.bad, { n: 1, dash: 1, at: 0.5 }); L(300, 65, 'purana pata, fail', C.bad);
    L(75, 200, 'TTL ~60 sec', C.acc); num(75, 225, 2, C.acc); L(75, 250, 'phir DNS se naya', C.muted);
    A(130, 150, 218, 150, C.msg, { n: 3 }); A(75, 170, 478, 262, C.good, { n: 4, at: 0.6 });
    apps(''); A(600, 240, 738, 95, C.good); A(600, 255, 738, 225, C.good);
    L(540, 330, 'isliye LB khud kai zone me (ALB), har jagah 2+', C.muted);
  }
}
