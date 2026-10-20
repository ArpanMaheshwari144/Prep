// Card 5: har inbox me saare id, soye logon ke bhi -> RAM bhar rahi. Ilaaj: aakhri 800 (trim), soye ka inbox TTL se mite.
// Agar: 800 ke aage scroll, agla page (cursor), Redis gira, copy bhi gayi. Wapas: soya user wapas, pull se banao.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har inbox me saare id, soye logon ke bhi. 50 crore inbox, RAM bhari'],
  [3, GRN, 'ILAAJ', 'ILAAJ: naya tweet upar, 800 se neeche kaato. Soye ka inbox 30 din me mite'],
  [6, YEL, 'AGAR', 'AGAR aakhri 800 ke aage scroll kiya? -> wahan se store (DB) se laao'],
  [9, YEL, 'AGAR', 'AGAR neeche scroll, agla page? -> aakhri dikhe tweet ka time (cursor)'],
  [12, YEL, 'AGAR', 'AGAR Redis gira, feed ka kya? -> copy (replica) sambhalti hai'],
  [15, YEL, 'AGAR', 'AGAR Redis ki copy bhi gayi? -> kuch request mana, inbox dheere bharo'],
  [18, GRN, 'WAPAS', 'WAPAS: soya user wapas aaya -> pehli baar pull se banao, phir bharo'],
];
function setup() {}
function frame(ph) {
  if (ph <= 1) {
    const fix = ph === 1;
    B(20, 130, 100, 60, 'Fanout');
    B(180, 20, 690, 90, 'mera inbox', fix ? 'aakhri 800' : 'saare id', { st: fix ? 'good' : 'bad' });
    B(180, 165, 400, 85, 'Ravi ka inbox', '1 mahine se nahi aaya', { st: fix ? 'dim' : 'bad' });
    B(620, 165, 250, 85, 'Redis RAM', '50 crore inbox');
    bar(640, 215, 210, 18, fix ? 0.3 : 1, '', fix);
    if (!fix) {
      const cs = []; for (let i = 0; i < 12; i++) cs.push([`t${90 - i}`, C.fg]);
      cells(200, 70, cs, { w: 50, h: 26 });
      cells(200, 212, [['t1', C.muted], ['t2', C.muted], ['..', C.muted], ['t90', C.muted]], { w: 60, h: 26 });
      A(120, 145, 178, 75, C.acc, { n: 1 }); A(120, 175, 178, 205, C.acc, { n: 2 });
      L(380, 280, 'koi padhta nahi', C.bad);
      num(670, 290, 3, C.bad); L(690, 295, 'RAM bhar rahi', C.bad, 'start');
    } else {
      const cs = [['t91', C.good]]; for (let i = 0; i < 7; i++) cs.push([`t${90 - i}`, C.fg]);
      cells(200, 70, cs, { w: 48, h: 26 });
      A(120, 145, 178, 75, C.good, { n: 1 }); L(225, 125, 'naya upar', C.good);
      A(625, 83, 703, 83, C.bad, { n: 2 }); cells(708, 70, [['t83', C.bad]], { w: 54, h: 26 });
      L(780, 87, 'kata', C.bad, 'start');
      num(200, 285, 3, C.good); L(220, 290, 'TTL 30 din: Ravi ka inbox mit gaya', C.good, 'start');
    }
  } else if (ph === 2) {
    B(20, 150, 110, 60, 'Main', 'neeche scroll');
    A(130, 180, 198, 180, C.msg, { n: 1 });
    B(200, 150, 120, 60, 'Feed Svc');
    B(420, 30, 260, 70, 'mera inbox', 'aakhri 800', { st: 'acc' });
    A(320, 160, 418, 75, C.msg, { n: 2 }); L(330, 110, 'pehle 800', C.msg, 'end');
    num(705, 65, 3, C.acc); L(725, 70, '800 khatam', C.acc, 'start');
    B(420, 250, 260, 70, 'Tweet store (DB)', '', { st: 'good' });
    A(320, 200, 418, 280, C.good, { n: 4 }); L(330, 260, 'aage ka', C.good, 'end');
    L(785, 190, 'hal: wahan se DB se laao', C.good);
    T(550, 350 - 5, 'bahut kam log itna neeche jaate', { c: C.muted, size: 12 });
  } else if (ph === 3) {
    T(40, 45, 'dikh chuka:', { c: C.muted, size: 13, a: 'start' });
    cells(40, 58, [['t10', C.acc], ['t9', C.fg], ['t8', C.fg], ['t7', C.fg]], { w: 60, h: 28 });
    num(310, 72, 1, C.acc); L(330, 77, 'beech me t10 aaya', C.acc, 'start');
    T(40, 145, 'page number (page 2):', { c: C.bad, size: 13, b: 1, a: 'start' });
    cells(300, 132, [['t7', C.bad], ['t6', C.fg], ['t5', C.fg]], { w: 60, h: 28 });
    L(510, 150, 't7 dobara, page khiska', C.bad, 'start');
    T(40, 235, 'cursor: t7 ke time se purane 50', { c: C.good, size: 13, b: 1, a: 'start' });
    num(320, 260, 2, C.good);
    cells(345, 246, [['t6', C.good], ['t5', C.good], ['t4', C.good]], { w: 60, h: 28 });
    L(550, 265, 'page nahi khiska', C.good, 'start');
    L(450, 330, 'hal: aakhri dikhe tweet ka time (cursor)', C.good);
  } else if (ph === 4) {
    B(20, 150, 120, 60, 'Feed Svc');
    B(330, 30, 180, 65, 'Redis main', '', { st: 'bad' });
    cross(535, 62); num(575, 62, 1, C.bad); L(595, 67, 'gira', C.bad, 'start');
    A(140, 165, 328, 75, C.bad, { dash: 1, n: 2, at: 0.35 }); cross(300, 95);
    B(330, 240, 180, 65, 'Redis copy', 'ab main', { st: 'good' });
    A(420, 97, 420, 238, C.acc, { dash: 1 }); L(430, 175, 'copy main bani', C.acc, 'start');
    A(140, 195, 328, 270, C.good, { n: 3 }); L(220, 270, 'feed copy se', C.good);
    B(680, 150, 180, 70, 'Tweet DB', '', { st: 'dim' }); L(770, 245, 'DB pe bojh nahi', C.muted);
    L(720, 320, 'hal: copy (replica) sambhalti', C.good);
  } else if (ph === 5) {
    B(20, 50, 110, 55, 'kuch log', '', { st: 'good' });
    B(20, 250, 110, 55, 'baaki log', '', { st: 'bad' });
    B(190, 150, 120, 60, 'Feed Svc');
    B(380, 15, 160, 55, 'Redis main', '', { st: 'bad' });
    B(380, 85, 160, 55, 'Redis copy', '', { st: 'bad' });
    cross(565, 42); cross(565, 112); num(605, 78, 1, C.bad); L(625, 83, 'dono gaye', C.bad, 'start');
    A(130, 275, 212, 228, C.bad, { n: 2, at: 0.4 }); cross(232, 240); T(275, 260, '503', { c: C.bad, b: 1, size: 18 });
    A(130, 80, 225, 148, C.good, { n: 3, at: 0.45 });
    B(640, 240, 200, 70, 'Tweet DB');
    A(310, 195, 638, 270, C.good, { n: 3, at: 0.55 }); L(450, 260, 'feed DB se', C.good);
    B(640, 140, 200, 55, 'inbox', 'dheere bharo', { st: 'acc' });
    A(740, 238, 740, 197, C.acc, { n: 4 });
    L(450, 335, 'hal: kuch request mana, inbox dheere-dheere bharo', C.good);
  } else {
    B(20, 150, 110, 60, 'Ravi', 'wapas aaya', { st: 'good' });
    A(130, 180, 198, 180, C.msg, { n: 1 });
    B(200, 150, 120, 60, 'Feed Svc');
    B(420, 30, 220, 65, 'Ravi ka inbox', 'ab bana', { st: 'good' });
    num(670, 62, 2, C.acc); L(690, 67, 'pehle: tha hi nahi', C.acc, 'start');
    B(420, 250, 220, 65, 'Tweet store');
    A(320, 200, 418, 275, C.good, { n: 3 }); L(330, 270, 'pull se banao', C.good, 'end');
    A(530, 248, 530, 97, C.good, { n: 4 }); L(545, 180, 'phir bharo', C.good, 'start');
  }
}
