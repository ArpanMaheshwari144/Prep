// Card 3: Bieber ka push = 10 crore kaam, mera tweet line me peeche. Ilaaj: aam push, celeb pull, feed pe milao.
// Agar: had pe palta, 50 celeb pull, laakhon like. Wapas: line khaali, mera tweet turant.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Bieber ka bhi push. Kafka me 10 crore kaam, mera tweet ghanton'],
  [3, GRN, 'ILAAJ', 'ILAAJ: aam ka push, celeb ka pull. Bieber celeb cache me, feed pe milao'],
  [6, YEL, 'AGAR', 'AGAR user had (~10,000) pe? -> 12,000 upar pull, 8,000 neeche tabhi push'],
  [9, YEL, 'AGAR', 'AGAR 50 celeb follow kiye, 50 pull mehnge? -> chhote cache me, saste'],
  [12, YEL, 'AGAR', 'AGAR celeb tweet pe laakhon like? -> ginti ke 10 tukde, padhte waqt jodo'],
  [15, GRN, 'WAPAS', 'WAPAS: line khaali rehti, mera m1 turant followers ke inbox me'],
];
function setup() {}
function base(kst, ksub, ist) {
  B(20, 30, 120, 60, 'Bieber', '10 crore fan');
  B(20, 260, 120, 60, 'Main', 'tweet m1');
  B(240, 150, 280, 70, 'Kafka line', ksub, { st: kst });
  B(650, 155, 150, 60, 'inbox', '', { st: ist });
}
function frame(ph) {
  if (ph === 0) {
    base('bad', 'fanout kaam', 'bad');
    bar(255, 195, 250, 16, 1, '');
    A(140, 60, 238, 160, C.bad, { n: 1 }); L(250, 120, '10 crore kaam', C.bad, 'start');
    A(140, 290, 238, 210, C.msg, { n: 2 }); L(260, 255, 'm1 line ke aakhir me', C.msg, 'start');
    A(520, 185, 648, 185, C.bad, { dash: 1, n: 3 }); L(585, 240, 'ghanton der', C.bad);
    T(725, 290, 'mera tweet kahan?', { c: C.bad, b: 1, size: 13 });
  } else if (ph === 1) {
    base('good', 'khaali', 'good');
    B(240, 20, 170, 60, 'Celeb cache', 'B1', { st: 'acc' });
    A(140, 55, 238, 50, C.acc, { n: 1 });
    A(140, 290, 238, 210, C.good, { n: 2 });
    A(520, 185, 648, 185, C.good, { n: 3 }); cells(670, 185, [['m1', C.good], ['r4', C.fg]], { w: 40, h: 22 });
    B(530, 270, 270, 70, 'Feed = inbox + celeb', '', { st: 'good' });
    cells(560, 302, [['B1', C.acc], ['m1', C.good], ['r4', C.fg]], { w: 60, h: 26 });
    A(725, 217, 725, 268, C.good, { n: 4 });
    AP([[410, 50], [830, 50], [830, 290], [802, 290]], C.acc, { n: 4, at: 0.3 });
    L(400, 305, 'feed khule tab milao', C.good);
  } else if (ph === 2) {
    A(60, 200, 860, 200, C.line);
    [[250, '8,000'], [450, '10,000'], [650, '12,000']].forEach(([x, s]) => {
      el('line', { x1: x, y1: 190, x2: x, y2: 210, stroke: C.muted, 'stroke-width': 2 }, DYN);
      T(x, 180, s, { c: C.muted, size: 13 });
    });
    T(140, 150, 'push', { c: C.good, b: 1, size: 16 }); T(770, 150, 'pull', { c: C.acc, b: 1, size: 16 });
    dot([450, 200], C.acc);
    num(450, 90, 1, C.bad); L(450, 70, 'had pe aage-peeche: palta?', C.bad);
    num(450, 250, 2, C.good); L(470, 255, 'beech me nahi palta', C.good, 'start');
    L(450, 320, 'hal: 12,000 upar pull, 8,000 neeche tabhi push', C.good);
  } else if (ph === 3) {
    B(20, 30, 130, 60, 'Main', '50 celeb follow');
    A(85, 90, 85, 148, C.msg, { n: 1 });
    B(20, 150, 130, 70, 'Feed Svc');
    B(380, 130, 200, 100, 'Celeb cache', 'chhota, RAM', { st: 'good' });
    A(150, 170, 378, 170, C.acc, { n: 2 }); L(265, 150, '50 pull', C.acc);
    A(378, 205, 150, 205, C.good, { n: 3 }); L(265, 235, 'jaldi, sasta', C.good);
    B(690, 140, 170, 80, 'Tweet DB', '', { st: 'dim' });
    A(580, 180, 688, 180, C.line, { dash: 1 }); L(775, 250, 'yahan nahi jaana', C.muted);
    L(450, 320, 'hal: celeb ke tweet alag chhote cache me', C.good);
  } else if (ph === 4) {
    T(90, 60, 'laakhon like', { c: C.msg, b: 1, size: 14 });
    dots(12, 45, 85, C.msg, { cols: 4, gap: 30 });
    num(570, 70, 1, C.bad); L(590, 75, 'ek key = ek machine: garam', C.bad, 'start');
    T(530, 155, 'tweet123#0 .. #9', { c: C.muted, size: 13 });
    A(150, 115, 330, 165, C.acc, { n: 2 }); L(200, 165, 'baant do', C.acc);
    const cs = []; for (let i = 0; i < 10; i++) cs.push([`#${i}`, C.acc]);
    cells(230, 170, cs, { w: 56, h: 28 });
    A(530, 202, 530, 262, C.good, { n: 3 });
    T(530, 285, 'padhte waqt jodo = kul like', { c: C.good, b: 1, size: 14 });
    L(450, 335, 'hal: ginti ke 10 tukde, padhte waqt jodo', C.good);
  } else {
    base('good', 'khaali', 'good');
    B(240, 20, 170, 60, 'Celeb cache', 'B1');
    A(140, 55, 238, 50, C.line);
    A(140, 290, 238, 210, C.good, { n: 1 });
    A(520, 185, 648, 185, C.good, { n: 2 }); cells(670, 185, [['m1', C.good], ['r4', C.fg]], { w: 40, h: 22 });
    L(725, 245, 'turant', C.good);
  }
}
