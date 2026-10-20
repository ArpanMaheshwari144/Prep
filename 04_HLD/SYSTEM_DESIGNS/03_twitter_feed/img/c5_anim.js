// Card 5: Redis 50 crore inbox, RAM bhar rahi (soye users ke bhi). Ilaaj: aakhri 800 rakho (trim) + soye ka inbox
// 30 din me mite (TTL). Agar: soya user wapas aaya, inbox hai hi nahi -> pehli baar pull se banao.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Redis me 50 crore inbox, har inbox me saare id. 40 crore to soye hue users ke'],
  [3, GRN, 'ILAAJ', 'ILAAJ: inbox me sirf aakhri 800 (trim). Jo 30 din nahi aaya, uska inbox mita do (TTL)'],
  [6, YEL, 'AGAR', 'AGAR soya user 2 mahine baad wapas aaya, uska inbox hai hi nahi? -> pull se banao'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'Redis', 'har inbox me saare id.|40 cr soye hue users ke, RAM 95%']],
  [[C.good, 'naya tweet', 'inbox me upar juda'], [C.bad, 'trim', '801va kata: sirf aakhri 800'], [C.good, 'Suresh ka inbox', '30 din app nahi khola: mita (TTL)']],
  [[C.msg, 'Suresh → Feed Svc', '2 mahine baad app khola'], [C.bad, 'Feed Svc → Redis', 'inbox? nahi'], [C.acc, 'Feed Svc → Tweet DB', 'pull: follow walon ke tweet jodo'], [C.good, 'Feed Svc → Redis', 'inbox bana ke rakha, aage push']],
];
function frame(ph) {
  if (ph === 0) {
    B(200, 30, 500, 210, 'Redis (RAM)', '', { st: 'bad' });
    B(230, 80, 140, 120, 'zinda', '10 cr inbox', { st: 'good' });
    B(390, 80, 290, 120, 'soye hue', '40 cr: mahine se nahi aaye', { st: 'dim' });
    bar(200, 260, 500, 18, 0.95, ''); T(450, 305, 'RAM 95% bhar gayi', { c: C.bad, b: 1, size: 15 });
    num(780, 135, 1, C.bad); L(780, 165, 'har inbox me', C.bad); L(780, 185, 'saare id', C.bad);
    L(450, 345, 'RAM ka bada hissa unke liye jo app kholte hi nahi', C.bad);
  } else if (ph === 1) {
    // trim: naya upar judta, 800 ke neeche kat-ta
    T(240, 35, 'Arpan ka inbox (naya baayein)', { c: C.fg, b: 1, size: 15 });
    cells(30, 50, [['t901', C.good], ['t900', C.good], ['...', C.line], ['t102', C.good], ['t101', C.bad]], { w: 70, h: 34 });
    num(45, 110, 1, C.good); L(65, 115, 'naya tweet upar juda', C.good, 'start');
    cross(338, 67); num(338, 110, 2, C.bad); L(330, 135, '801va kata (trim)', C.bad);
    // TTL: soya user
    B(520, 40, 330, 70, 'Suresh ka inbox', '30 din se app nahi khola', { st: 'dim' });
    A(685, 112, 685, 175, C.muted, { n: 3, dash: 1 }); L(700, 150, '30 din', C.muted, 'start');
    B(560, 180, 250, 50, 'inbox mit gaya (TTL)', '', { st: 'good' });
    bar(200, 270, 500, 18, 0.35, '', 1); T(450, 310, 'RAM 35%: sirf zinda logon ki, aakhri 800', { c: C.good, b: 1, size: 14 });
  } else {
    B(20, 140, 130, 60, 'Suresh', '2 mahine baad');
    B(250, 140, 150, 60, 'Feed Svc', '');
    B(560, 75, 200, 60, 'Redis', 'Suresh ka inbox: nahi', { st: 'bad' });
    B(560, 250, 200, 60, 'Tweet DB', 'follow walon ke tweet', { st: 'acc' });
    A(150, 170, 248, 170, C.msg, { n: 1 }); L(200, 150, 'app khola', C.msg);
    A(400, 155, 558, 110, C.bad, { n: 2 }); L(470, 120, 'inbox?', C.bad);
    A(400, 180, 558, 275, C.acc, { n: 3 }); L(470, 255, 'pull: jod ke banao', C.acc);
    AP([[325, 138], [325, 45], [640, 45], [640, 73]], C.good, { n: 4, at: 0.7 }); L(470, 32, 'Feed Svc ne inbox bana ke rakha, aage push', C.good);
    L(450, 345, 'pehli baar thoda dheema, par galat nahi. RAM sirf zinda logon ki', C.good);
  }
}
