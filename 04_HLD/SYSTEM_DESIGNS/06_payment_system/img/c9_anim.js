// Card 9: mahine bhar ki report primary pe, payment ruka. Ilaaj: report read replica pe, primary sirf payment.
// Agar: turant balance, copy aur peeche, copy bhi baithi (warehouse), primary gira (standby). Wapas: payment tez.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: merchant ki mahine bhar ki report primary pe. Mera payment ruka'],
  [3, GRN, 'ILAAJ', 'ILAAJ: bhaari report copy pe (read replica), primary sirf payment ke liye'],
  [6, YEL, 'AGAR', 'AGAR pay kiya, turant balance dekha? -> primary se padho, copy peeche'],
  [9, YEL, 'AGAR', 'AGAR copy aur peeche chali gayi? -> lag naapo, had paar hui to alert'],
  [12, YEL, 'AGAR', 'AGAR report itni bhaari ki copy bhi baithe? -> alag warehouse (CDC)'],
  [15, YEL, 'AGAR', 'AGAR primary gir gaya? -> sync standby ban jaata, kuch nahi khota'],
  [18, GRN, 'WAPAS', 'WAPAS: paisa aur haal primary se, report copy se. Payment tez rehta'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 40, 140, 70, 'Arpan', 'payment', { st: o.ar });
  B(20, 250, 160, 70, 'Merchant', 'mahine bhar report', { st: o.me });
  B(400, 40, 180, 80, 'Primary DB', s.pr || 'payment', { st: o.pr });
  if (!o.noRp) B(400, 250, 180, 80, 'Read replica', s.rp || '1-2 sec peeche', { st: o.rp });
}
function copyLine(c, n, dash) { A(490, 122, 490, 248, c || C.line, { dash: dash === undefined ? 1 : dash, n }); }
function frame(ph) {
  if (ph === 0) {
    base({ ar: 'bad', pr: 'bad', noRp: 1 }, { pr: 'busy' });
    A(180, 270, 420, 122, C.acc, { n: 1, at: 0.4 }); L(250, 190, 'bhaari report', C.acc, 'end');
    bar(640, 70, 220, 18, 1, ''); num(620, 79, 2, C.bad); L(750, 120, 'primary busy', C.bad);
    A(160, 75, 398, 75, C.bad, { dash: 1, n: 3 }); L(280, 60, 'pay: ruka', C.bad);
  } else if (ph === 1) {
    base({ ar: 'good', pr: 'good', rp: 'acc' });
    copyLine(); L(500, 190, 'copy, 1-2 sec peeche', C.muted, 'start');
    A(180, 285, 398, 285, C.acc, { n: 1 }); L(290, 270, 'report', C.acc);
    A(160, 65, 398, 65, C.msg, { n: 2 }); L(280, 50, 'pay', C.msg);
    A(398, 95, 162, 95, C.good, { n: 3 }); L(280, 120, 'hua, tez', C.good);
    bar(640, 70, 220, 18, 0.15, 'primary', 1); bar(640, 280, 220, 18, 0.7, 'replica');
  } else if (ph === 2) {
    base({ ar: 'good', pr: 'good', rp: 'acc' }, { rp: 'purana balance' });
    copyLine(); L(500, 190, '1-2 sec peeche', C.muted, 'start');
    A(160, 60, 398, 60, C.msg, { n: 1 }); L(280, 45, 'pay', C.msg);
    A(160, 105, 398, 270, C.bad, { dash: 1, n: 2, at: 0.3 }); cross(320, 205); L(200, 215, 'copy se: purana', C.bad, 'end');
    A(160, 85, 398, 85, C.good, { n: 3 }); L(280, 102, 'balance: primary se', C.good);
    L(700, 300, 'hal: paisa aur haal', C.good, 'start'); L(700, 322, 'primary se padho', C.good, 'start');
  } else if (ph === 3) {
    base({ pr: 'good', rp: 'bad' }, { rp: 'bahut peeche' });
    copyLine(C.acc, 1); L(500, 175, 'lag badhta gaya', C.acc, 'start');
    bar(640, 280, 220, 18, 1, 'replication lag'); num(620, 289, 2, C.bad); L(750, 330, 'had paar', C.bad);
    B(680, 120, 180, 60, 'alert', '', { st: 'good' });
    A(582, 270, 678, 165, C.good, { n: 3, at: 0.5 });
    L(250, 200, 'hal: kitni peeche hai naapo', C.good); L(250, 222, 'had paar -> alert', C.good);
  } else if (ph === 4) {
    base({ pr: 'good', rp: 'bad' }, { rp: 'baith gayi' });
    num(600, 255, 1, C.bad); cross(600, 300);
    B(680, 150, 180, 70, 'Warehouse', 'alag', { st: 'good' });
    A(582, 100, 678, 170, C.good, { n: 2, at: 0.5 }); L(650, 110, 'CDC', C.good, 'start');
    AP([[100, 322], [100, 342], [770, 342], [770, 222]], C.acc, { n: 3, at: 0.3 }); L(260, 335, 'report', C.acc);
    L(250, 180, 'hal: alag warehouse (CDC)', C.good);
  } else if (ph === 5) {
    base({ ar: 'good', pr: 'bad', rp: '' }, { pr: 'gira' });
    cross(600, 80); num(600, 30, 1, C.bad); L(618, 35, 'primary gira', C.bad, 'start');
    B(680, 40, 180, 80, 'Sync standby', 'ab primary', { st: 'good' });
    num(700, 160, 2, C.good); L(718, 165, 'standby primary bana', C.good, 'start');
    AP([[160, 95], [370, 95], [370, 135], [770, 135], [770, 122]], C.msg, { n: 3, at: 0.5 }); L(270, 120, 'pay', C.msg);
    L(250, 200, 'hal: kuch nahi khota (sync)', C.good);
  } else {
    base({ ar: 'good', me: 'good', pr: 'good', rp: 'good' });
    copyLine(); L(500, 190, 'copy, 1-2 sec peeche', C.muted, 'start');
    A(180, 285, 398, 285, C.good, { n: 1 }); L(290, 270, 'report', C.good);
    A(160, 65, 398, 65, C.msg, { n: 2 }); L(280, 50, 'pay', C.msg);
    A(398, 95, 162, 95, C.good, { n: 3 }); L(280, 120, 'payment tez', C.good);
  }
}
