// Card 4: Redis gira, limiter andha. Ilaaj: copy taiyaar, main gira to copy main.
// Agar: copy bhi gayi -> pehle se tay: login band (fail-closed), feed khula (fail-open).
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Redis gira. Gateway ginti pooche to jawab hi nahi, limiter andha'],
  [3, GRN, 'ILAAJ', 'ILAAJ: Redis ki copy taiyaar. Main gira to copy main ban jaati (Redis Cluster)'],
  [6, YEL, 'AGAR', 'AGAR copy bhi gayi? -> pehle se tay: login band (fail-closed), feed khula (fail-open)'],
];
function setup() {}
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 110, 60, 'User', 'login');
    B(260, 140, 150, 80, 'Gateway', 'ginti?', { st: 'bad' });
    B(620, 140, 160, 80, 'Redis', 'ginti yahin thi', { st: 'bad' }); cross(805, 180);
    A(130, 180, 258, 180, C.msg, { n: 1 });
    A(410, 165, 618, 165, C.bad, { n: 2, dash: 1 }); L(515, 150, 'ginti +1?', C.bad);
    num(700, 245, 3, C.bad); L(700, 270, 'gira', C.bad);
    num(335, 245, 4, C.bad); L(335, 270, 'jawab nahi: andar bhejun ya rokun?', C.bad);
    L(450, 335, 'ginti ek hi jagah thi, wahi gayi, ab kisi ki had nahi dikh rahi', C.bad);
  } else if (ph === 1) {
    B(20, 150, 110, 60, 'User', 'login');
    B(260, 140, 150, 80, 'Gateway', '5 ms timeout', { st: 'good' });
    B(620, 40, 160, 60, 'Redis main', '', { st: 'bad' }); cross(805, 70); num(745, 120, 1, C.bad); L(765, 125, 'gira', C.bad, 'start');
    B(620, 220, 160, 70, 'Redis copy', 'ab yahi main', { st: 'good' });
    A(700, 102, 700, 218, C.acc, { n: 2, dash: 1, at: 0.6 }); L(715, 175, 'copy main bani', C.acc, 'start');
    A(130, 180, 258, 180, C.msg, { n: 3 });
    A(410, 200, 618, 245, C.acc, { n: 4, at: 0.4 }); L(500, 205, 'ginti +1', C.acc);
    A(618, 270, 410, 215, C.good, { n: 5, at: 0.4 }); L(510, 285, 'ginti 3: andar', C.good);
    L(450, 345, 'beech ke kuch second ki ginti kho sakti, rate limit me chalta hai', C.muted);
  } else {
    B(20, 50, 120, 60, 'User', 'login');
    B(20, 250, 120, 60, 'User', 'feed');
    B(280, 145, 140, 70, 'Gateway', 'Redis nahi', { st: 'acc' });
    B(560, 20, 120, 50, 'Redis main', '', { st: 'bad' }); B(720, 20, 120, 50, 'Redis copy', '', { st: 'bad' });
    cross(700, 45); cross(860, 45); num(620, 95, 1, C.bad); L(640, 100, 'dono gaye', C.bad, 'start');
    A(140, 70, 300, 143, C.msg, { n: 2, at: 0.45 });
    A(278, 180, 80, 112, C.bad, { n: 3, at: 0.5 }); L(115, 185, '503, thodi der baad', C.bad);
    A(140, 280, 278, 200, C.msg, { n: 4, at: 0.4 });
    B(620, 250, 160, 60, 'Feed App', 'khula', { st: 'good' });
    A(420, 190, 618, 270, C.good, { n: 5, at: 0.5 }); L(470, 262, 'feed khula', C.good);
    L(450, 345, 'login band: koi bina had password guess na kare. Feed khula: site chalti rahe', C.good);
  }
}
