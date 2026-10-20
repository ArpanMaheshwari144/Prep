// Card 1: Order Svc seedha email company ko call kare, company band -> order fail. Ilaaj: outbox + relay -> Kafka.
// Agar: service Kafka se pehle giri, traffic 10 guna, company band, Notification Svc dheemi. Wapas: company aayi.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Order Svc seedha email company ko call kare. Band -> order fail'],
  [3, GRN, 'ILAAJ', 'ILAAJ: order + khabar ek DB txn (outbox), relay Kafka me. Order turant OK'],
  [6, YEL, 'AGAR', 'AGAR Kafka tak pahunchne se pehle service giri? -> khabar outbox me padi'],
  [9, YEL, 'AGAR', 'AGAR traffic achanak 10 guna? -> jhatka Kafka ki line sambhaalti'],
  [12, YEL, 'AGAR', 'AGAR email company band? -> khabar Kafka ke log me ~7 din padi'],
  [15, YEL, 'AGAR', 'AGAR Notification Svc dheemi? -> machine badhao (consumer group)'],
  [18, GRN, 'WAPAS', 'WAPAS: company 2 din baad wapas aayi -> wahin se aage chala'],
];
function setup() {}
// o: { user, ord, db, kf, ns, em } = box st ; sub = { db, kf, ns, em }
function base(o = {}, sub = {}) {
  B(20, 150, 100, 70, 'User', 'order kiya', { st: o.user });
  B(170, 150, 130, 70, 'Order Svc', '', { st: o.ord });
  B(700, 150, 150, 70, 'Email co.', sub.em || 'company', { st: o.em });
  if (o.noOut) return;
  B(170, 280, 130, 60, 'DB', sub.db || 'order + outbox', { st: o.db });
  B(350, 150, 110, 70, 'Kafka', sub.kf || 'log', { st: o.kf });
  if (!o.noNs) B(500, 150, 160, 70, 'Notification Svc', sub.ns || 'peeche padhe', { st: o.ns });
}
function frame(ph) {
  if (ph === 0) {
    base({ noOut: 1, em: 'bad', user: 'bad' }, { em: 'band' });
    cross(872, 185); num(745, 128, 1, C.bad); L(762, 133, 'company band', C.bad, 'start');
    A(120, 172, 168, 172, C.msg, { n: 2, at: 0.45 });
    A(300, 172, 698, 172, C.msg, { n: 3 }); L(500, 150, 'email bhejo, jawab ka intezaar', C.msg);
    A(698, 200, 302, 200, C.bad, { n: 4 }); L(500, 230, 'error', C.bad);
    A(168, 200, 122, 200, C.bad, { n: 5, at: 0.5 }); L(70, 250, 'order FAIL', C.bad);
  } else if (ph === 1) {
    base({ db: 'acc', kf: 'acc', ns: 'acc', em: 'good', user: 'good' });
    A(120, 172, 168, 172, C.msg, { n: 1, at: 0.45 });
    A(235, 222, 235, 278, C.acc, { n: 2 }); L(220, 260, 'ek txn: order + khabar', C.acc, 'end');
    A(168, 200, 122, 200, C.good, { n: 3, at: 0.5 }); L(70, 135, 'order OK turant', C.good);
    A(300, 310, 400, 222, C.acc, { n: 4, at: 0.55 }); L(318, 336, 'relay Kafka me daale', C.acc, 'start');
    A(460, 185, 498, 185, C.msg, { n: 5 }); L(480, 135, 'padhe', C.msg);
    A(660, 185, 698, 185, C.good, { n: 6 }); L(775, 250, 'email gaya', C.good);
  } else if (ph === 2) {
    base({ ord: 'bad', db: 'acc', kf: 'acc', ns: 'acc', em: 'good' }, { db: 'khabar padi' });
    A(120, 172, 168, 172, C.line);
    A(235, 222, 235, 278, C.acc, { n: 1 }); L(252, 255, 'ek txn likha', C.acc, 'start');
    num(185, 132, 2, C.bad); L(203, 137, 'gira', C.bad, 'start'); cross(270, 128);
    A(300, 310, 400, 222, C.acc, { n: 3, at: 0.55 }); L(318, 336, 'relay baad me bhejta', C.acc, 'start');
    A(460, 185, 498, 185, C.msg, { n: 4 });
    A(660, 185, 698, 185, C.good, { n: 5 }); L(775, 250, 'email gaya', C.good);
    L(500, 40, 'hal: khabar outbox ki row me, relay baad me bhejta, khoti nahi', C.good);
  } else if (ph === 3) {
    base({ user: 'bad', kf: 'acc', ns: 'good', em: 'good' }, { kf: 'line bhari', ns: 'apni speed' });
    dots(6, 30, 118, C.bad, { gap: 18 }); num(16, 92, 1, C.bad); L(34, 97, '10 guna', C.bad, 'start');
    A(120, 185, 168, 185, C.bad, { n: 2, at: 0.45 });
    A(300, 185, 348, 185, C.msg, { n: 3, at: 0.45 });
    dots(6, 357, 125, C.msg, { gap: 18 }); L(405, 103, 'line me ruki', C.acc);
    A(460, 185, 498, 185, C.good, { n: 4 }); L(580, 135, 'apni speed se', C.good);
    A(660, 185, 698, 185, C.good, { n: 5 });
    L(500, 40, 'hal: jhatka line sambhaalti, worker apni speed se', C.good);
    L(500, 62, 'sale 12 baje -> 11:50 tak worker badha do', C.good);
  } else if (ph === 4) {
    base({ user: 'good', kf: 'acc', ns: 'acc', em: 'bad' }, { kf: 'khabar padi', em: 'band' });
    cross(872, 185); num(745, 128, 1, C.bad); L(762, 133, 'band', C.bad, 'start');
    A(120, 172, 168, 172, C.line); A(300, 310, 400, 222, C.line);
    A(168, 200, 122, 200, C.good, { n: 4, at: 0.5 });
    A(660, 185, 698, 185, C.bad, { n: 2, dash: 1 }); L(680, 245, 'fail, ruko', C.bad);
    dots(4, 370, 255, C.acc, { gap: 22 }); num(352, 290, 3, C.acc); L(370, 295, '~7 din padi', C.acc, 'start');
    L(70, 250, 'order OK turant', C.good);
    L(500, 40, 'hal: Kafka me padi, offset yaad rakhta kahan tak padha', C.good);
  } else if (ph === 5) {
    B(20, 150, 100, 70, 'User', 'order kiya'); B(170, 150, 130, 70, 'Order Svc');
    B(170, 280, 130, 60, 'DB', 'order + outbox'); B(700, 150, 150, 70, 'Email co.', 'company', { st: 'good' });
    B(350, 150, 110, 70, 'Kafka', 'line 1, 2, 3', { st: 'acc' });
    B(510, 40, 140, 60, 'Notif Svc 2', 'nayi', { st: 'good' });
    B(510, 155, 140, 60, 'Notif Svc 1', 'dheemi', { st: 'acc' });
    B(510, 270, 140, 60, 'Notif Svc 3', 'nayi', { st: 'good' });
    A(120, 185, 168, 185, C.line); A(300, 185, 348, 185, C.line); A(300, 310, 400, 222, C.line);
    num(636, 173, 1, C.acc); num(636, 58, 2, C.good); num(636, 288, 2, C.good);
    A(460, 165, 508, 75, C.msg, { n: 3, at: 0.45 }); A(460, 185, 508, 185, C.msg);
    A(460, 205, 508, 295, C.msg, { n: 3, at: 0.45 });
    A(650, 70, 698, 165, C.good, { n: 4, at: 0.5 }); A(650, 185, 698, 185, C.good);
    A(650, 300, 698, 205, C.good, { n: 4, at: 0.5 });
    L(700, 40, 'hal: har line ek machine,', C.good, 'start'); L(700, 60, 'lines bat jaati', C.good, 'start');
  } else {
    base({ user: 'good', kf: 'acc', ns: 'good', em: 'good' }, { kf: 'offset yaad', em: 'wapas' });
    num(745, 128, 1, C.good); L(762, 133, 'wapas aaya', C.good, 'start');
    A(120, 172, 168, 172, C.line); A(300, 310, 400, 222, C.line);
    A(460, 185, 498, 185, C.good, { n: 2 }); L(480, 135, 'wahin se', C.good);
    A(660, 185, 698, 185, C.good, { n: 3 }); L(775, 250, 'email gaya', C.good);
    L(70, 250, 'order OK', C.good);
  }
}
