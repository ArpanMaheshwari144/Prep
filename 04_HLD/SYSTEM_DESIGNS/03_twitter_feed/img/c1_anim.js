// Card 1: feed khulte hi 200 logon ka jod (dheema). Ilaaj: tweet hote hi follower ke inbox me id (fanout).
// Agar: poora tweet (batch), unfollow (chhaanto), naya follow (aakhri 20). Wapas: bas inbox padho.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: feed khuli to 200 logon ke tweet laao, jodo, sort. Har baar dheema'],
  [3, GRN, 'ILAAJ', 'ILAAJ: tweet hote hi follower ke inbox me id (fanout). Kholo, ek hi read'],
  [6, YEL, 'AGAR', 'AGAR inbox me sirf id, poora tweet? -> 50 id ek saath laao (batch)'],
  [9, YEL, 'AGAR', 'AGAR unfollow, tweet inbox me pade? -> padhte waqt follow list se chhaanto'],
  [12, YEL, 'AGAR', 'AGAR abhi follow kiya? -> uske aakhri 20 tweet id mere inbox me daal do'],
  [15, GRN, 'WAPAS', 'WAPAS: inbox pehle se bana. App kholo, bas apna inbox padho, ek read'],
];
function setup() {}
// fanout wala hissa: Ravi -> Fanout -> mera inbox
function fan(c, n1, n2) {
  B(20, 25, 110, 60, 'Ravi', 'tweet t9');
  A(130, 55, 218, 55, c, { n: n1 });
  B(220, 30, 110, 50, 'Fanout', '', { st: c === C.line ? '' : 'acc' });
  A(330, 60, 478, 128, c, { n: n2, at: 0.45 });
}
function inbox(items, st) {
  B(480, 110, 380, 100, 'Redis: mera inbox', 'tweet_id ki list', { st: st || 'acc' });
  cells(500, 165, items, { w: 60, h: 28 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 110, 70, 'Main', 'feed khola');
    A(130, 185, 198, 185, C.msg, { n: 1 });
    B(200, 155, 110, 60, 'Feed Svc', '', { st: 'bad' });
    A(310, 185, 388, 185, C.msg, { n: 2 });
    B(390, 150, 110, 70, 'Tweet DB', '', { st: 'bad' });
    const fn = ['Ravi', 'Asha', 'Mohan', 'Neha', '+196'];
    fn.forEach((f, i) => {
      const y = 20 + i * 66;
      B(620, y, 110, 40, f);
      A(500, 185, 618, y + 20, C.acc, { n: i === 2 ? 3 : 0, at: 0.5 });
    });
    L(810, 175, '200 log', C.acc); L(810, 195, 'ke tweet', C.acc);
    num(410, 250, 4, C.bad); L(430, 255, 'jodo + sort', C.bad, 'start');
    L(255, 270, 'feed ghoom rahi', C.bad); L(255, 290, 'har baar, har user', C.bad);
  } else if (ph === 1) {
    fan(C.acc, 1, 2); L(420, 75, 't9 ki id gayi', C.acc, 'start');
    inbox([['t9', C.good], ['t7', C.fg], ['t5', C.fg], ['t2', C.fg]]);
    B(20, 250, 110, 60, 'Main', 'feed khola');
    AP([[130, 280], [640, 280], [640, 212]], C.good, { n: 3, at: 0.4 });
    L(330, 305, 'ek hi read', C.good);
    T(760, 260, 'feed aayi', { c: C.good, b: 1, size: 16 });
  } else if (ph === 2) {
    B(20, 160, 100, 60, 'Main');
    A(120, 180, 198, 180, C.msg, { n: 1 });
    B(200, 155, 130, 70, 'Feed Svc');
    B(450, 25, 200, 60, 'Redis inbox', '50 id');
    A(300, 153, 448, 60, C.msg, { n: 2 }); L(370, 85, '50 id mili', C.msg, 'end');
    B(450, 255, 200, 70, 'Tweet store', 'cache / DB', { st: 'good' });
    A(330, 210, 448, 280, C.good, { n: 3 }); L(395, 290, 'ek saath (batch)', C.good, 'end');
    A(198, 205, 120, 205, C.good, { n: 4 }); L(110, 250, 'poore 50 tweet', C.good);
    T(690, 270, 'batch: 1 chakkar', { c: C.good, b: 1, size: 15, a: 'start' });
    T(690, 305, 'ek-ek: 50 chakkar', { c: C.bad, b: 1, size: 15, a: 'start' });
    L(770, 150, 'hal: 50 id ek saath (batch)', C.good);
  } else if (ph === 3) {
    B(20, 160, 130, 60, 'Main', 'Mohan unfollow');
    B(230, 20, 420, 80, 'mera inbox');
    cells(250, 58, [['t9 Ravi', C.fg], ['t8 Mohan', C.bad], ['t5 Asha', C.fg]], { w: 120, h: 28 });
    L(440, 120, 'Mohan ka t8 abhi bhi pada', C.bad);
    B(700, 20, 170, 80, 'follow list', 'Ravi, Asha');
    B(380, 175, 150, 60, 'Feed Svc');
    A(150, 190, 378, 205, C.msg, { n: 1 });
    A(455, 132, 455, 173, C.msg, { n: 2 });
    A(785, 102, 532, 195, C.acc, { n: 3 }); L(690, 165, 'Mohan nahi hai', C.acc, 'start');
    A(455, 237, 455, 283, C.good, { n: 4 });
    cells(330, 288, [['t9 Ravi', C.good], ['t5 Asha', C.good]], { w: 110, h: 28 });
    cells(640, 288, [['t8 Mohan', C.bad]], { w: 110, h: 28 }); L(695, 338, 'chhaanta', C.bad);
    L(165, 275, 'hal: follow list se chhaanto', C.good);
  } else if (ph === 4) {
    B(20, 35, 130, 60, 'Main', 'Neha ko follow');
    A(150, 65, 298, 65, C.msg, { n: 1 });
    B(300, 35, 140, 60, 'Follow Svc');
    B(20, 240, 140, 60, 'Neha', 'aakhri 20 tweet');
    A(160, 270, 518, 220, C.good, { n: 2, at: 0.45 }); L(330, 275, 'aakhri 20 id daale', C.good);
    B(520, 130, 340, 100, 'mera inbox', '', { st: 'good' });
    cells(540, 175, [['n20', C.good], ['n19', C.good], ['..', C.good], ['t9', C.fg]], { w: 60, h: 28 });
    L(690, 270, 'agle tweet ka intezaar nahi', C.good);
    L(680, 45, 'hal: aakhri 20 id inbox me', C.good);
  } else {
    fan(C.line, 0, 0);
    inbox([['n20', C.fg], ['t9', C.fg], ['t7', C.fg], ['t5', C.fg]], 'good');
    B(20, 250, 110, 60, 'Main', 'feed khola', { st: 'good' });
    AP([[130, 280], [640, 280], [640, 212]], C.good, { n: 1, at: 0.4 });
    L(330, 305, 'bas apna inbox, ek read', C.good);
    T(760, 260, 'feed turant', { c: C.good, b: 1, size: 16 });
  }
}
