// Card 3: Worker-A ne email bheja, "padh liya" se pehle mara, Worker-B ne phir bheja -> 2 email.
// Ilaaj: khabar ki key (khabar_id + raasta) Redis me. Agar: Redis gira, key 24 ghante me mit gayi. Wapas: Redis + DB.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Worker-A ne email bheja, "padh liya" se pehle mara. B ne phir bheja'],
  [3, GRN, 'ILAAJ', 'ILAAJ: bhejne se pehle key (khabar_id + raasta) Redis me. Mili to skip'],
  [6, YEL, 'AGAR', 'AGAR key wala Redis gir gaya? -> DB me (khabar_id, raasta) UNIQUE rokta'],
  [9, YEL, 'AGAR', 'AGAR Redis ki key 24 ghante me mit gayi? -> DB ki row nahi bhoolti'],
  [12, GRN, 'WAPAS', 'WAPAS: Redis tez raasta, DB aakhri deewar. Dobara aayi khabar skip'],
];
function setup() {}
function base(o = {}) {
  B(20, 150, 120, 80, 'Kafka', 'khabar K-1');
  B(290, 50, 140, 70, 'Worker-A', '', { st: o.a }); B(290, 240, 140, 70, 'Worker-B', '', { st: o.b });
  B(730, 150, 130, 80, 'User', o.mails, { st: o.u });
  if (o.redis !== undefined) B(510, 140, 140, 70, 'Redis', o.redis, { st: o.rst });
  if (o.db !== undefined) B(510, 270, 140, 60, 'DB', o.db, { st: o.dbst });
}
function aDead(n) { num(275, 30, n, C.bad); L(293, 35, 'mara, "padh liya" nahi bola', C.bad, 'start'); cross(268, 62); }
function frame(ph) {
  if (ph === 0) {
    base({ a: 'bad', b: 'bad', u: 'bad', mails: 'email: 2' });
    A(140, 175, 288, 90, C.msg, { n: 1, at: 0.5 });
    A(430, 85, 728, 175, C.msg, { n: 2, at: 0.5 });
    aDead(3);
    A(140, 205, 288, 270, C.acc, { n: 4, at: 0.5 }); L(150, 270, 'khabar dobara', C.acc, 'start');
    A(430, 275, 728, 205, C.bad, { n: 5, at: 0.5 });
  } else if (ph === 1) {
    base({ a: 'bad', b: 'good', u: 'good', mails: 'email: 1', redis: 'K-1:email', rst: 'acc' });
    A(140, 175, 288, 90, C.msg, { n: 1, at: 0.5 });
    A(430, 95, 508, 160, C.acc, { n: 2, at: 0.5 }); L(515, 232, 'nahi mili -> likha', C.acc, 'start');
    A(430, 62, 728, 160, C.good, { n: 3, at: 0.45 });
    aDead(4);
    A(140, 205, 288, 270, C.acc, { n: 5, at: 0.5 }); L(150, 270, 'khabar dobara', C.acc, 'start');
    A(430, 260, 508, 195, C.acc, { n: 6, at: 0.5 }); L(440, 330, 'key mili -> skip', C.good, 'start');
  } else if (ph === 2) {
    base({ b: 'good', u: 'good', mails: 'email: 1', redis: 'gira', rst: 'bad', db: 'K-1, email', dbst: 'acc' });
    cross(675, 175); num(675, 130, 1, C.bad); L(693, 135, 'gira', C.bad, 'start');
    A(430, 75, 728, 165, C.line);
    A(140, 205, 288, 270, C.acc, { n: 2, at: 0.5 }); L(150, 270, 'khabar dobara', C.acc, 'start');
    A(430, 285, 508, 290, C.acc, { n: 3, at: 0.5 }); L(470, 270, 'likho', C.acc);
    A(508, 315, 432, 305, C.bad, { n: 4, at: 0.5 }); L(440, 340, 'pehle se hai -> skip', C.good, 'start');
    L(450, 25, 'hal: DB me (khabar_id, raasta) UNIQUE, ek hi baar lagta', C.good);
  } else if (ph === 3) {
    base({ b: 'good', u: 'good', mails: 'email: 1', redis: 'K-1: mit gayi', rst: 'acc', db: 'K-1, email', dbst: 'good' });
    num(675, 130, 1, C.acc); L(693, 135, '24 ghante baad', C.acc, 'start');
    A(430, 75, 728, 165, C.line);
    A(140, 205, 288, 270, C.acc, { n: 2, at: 0.5 }); L(150, 270, 'khabar dobara', C.acc, 'start');
    A(430, 255, 508, 195, C.acc, { n: 3, at: 0.5 }); L(520, 230, 'key nahi', C.acc, 'start');
    A(430, 285, 508, 290, C.acc, { n: 4, at: 0.5 });
    A(508, 315, 432, 305, C.good, { n: 5, at: 0.5 }); L(440, 340, 'row hai -> skip', C.good, 'start');
    L(450, 25, 'hal: DB ki row nahi bhoolti, wo phir bhi rokegi', C.good);
  } else {
    base({ b: 'good', u: 'good', mails: 'email: 1', redis: 'tez raasta', rst: 'good', db: 'aakhri deewar', dbst: 'good' });
    A(430, 75, 728, 165, C.line);
    A(140, 205, 288, 270, C.acc, { n: 1, at: 0.5 }); L(150, 270, 'khabar dobara', C.acc, 'start');
    A(430, 260, 508, 195, C.good, { n: 2, at: 0.5 }); L(520, 230, 'key mili -> skip', C.good, 'start');
    A(430, 300, 508, 300, C.line, { dash: 1 });
  }
}
