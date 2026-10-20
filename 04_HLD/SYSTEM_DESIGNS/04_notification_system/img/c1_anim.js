// Card 1: Order Svc seedha Email API ko call kare, company band -> order fail. Ilaaj: outbox + relay -> Kafka -> Notification Svc.
const DUR = 18, FIX = 8;
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  wire('M120 185 H170'); wire('M300 185 H700', 'w0');
  wire('M300 185 H350', 'w1'); wire('M460 185 H500', 'w2'); wire('M660 185 H700', 'w3');
  wire('M235 220 V280', 'w4'); wire('M300 300 L405 220', 'w5');
  box(20, 150, 100, 70, 'User', 'order kiya');
  box(170, 150, 130, 70, 'Order Svc');
  box(700, 150, 150, 70, 'Email API', 'company');
  box(170, 280, 130, 60, 'DB', 'order + outbox', { id: 'db', stroke: C.acc });
  box(350, 150, 110, 70, 'Kafka', 'log', { id: 'kf', stroke: C.acc });
  box(500, 150, 160, 70, 'Notification Svc', 'peeche padhe', { id: 'ns', stroke: C.acc });
}
function frame(t, fix, tl) {
  ['db', 'kf', 'ns', 'w1', 'w2', 'w3', 'w4', 'w5'].forEach(i => vis(i, fix)); vis('w0', !fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: Order Svc seedha email company ko call kare. Company band -> order hi FAIL';
    cross(872, 185); T(775, 245, 'company band', { c: C.bad, b: 1, size: 13 });
    dot(mv([120, 185], [158, 185], 0.3, 0.8, tl), C.msg);
    dot(mv([300, 185], [688, 185], 1.0, 2.0, tl), C.msg, '', 'email bhejo', C.msg);
    if (tl > 2.0 && tl < 5.2) T(235, 135, `jawab ka intezaar... ${Math.round((tl - 2) * 3)} sec`, { c: C.acc, b: 1, size: 12 });
    dot(mv([170, 185], [120, 185], 5.2, 5.8, tl), C.bad, '', 'FAIL', C.bad);
    if (tl > 5.8) T(70, 245, 'order FAIL', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: order aur khabar ek DB txn me (outbox), relay use Kafka me daalta. Order ko turant OK';
  const up = tl > 6.5;
  if (!up) { cross(872, 185); T(775, 245, 'company band', { c: C.bad, b: 1, size: 13 }); }
  else T(775, 245, 'company wapas', { c: C.good, b: 1, size: 13 });
  dot(mv([120, 185], [158, 185], 0.3, 0.8, tl), C.msg);
  dot(mv([235, 220], [235, 268], 0.9, 1.4, tl), C.acc, '', 'ek txn', C.acc);
  if (tl > 1.4) T(310, 335, 'order + khabar ek saath', { c: C.acc, size: 12, a: 'start' });
  dot(mv([170, 185], [120, 185], 1.5, 2.1, tl), C.good, '', 'OK turant', C.good);
  if (tl > 2.1) T(70, 245, 'order OK', { c: C.good, b: 1, size: 15 });
  dot(mv([300, 300], [405, 222], 2.4, 3.2, tl), C.acc, '', 'relay', C.acc);
  if (tl > 3.2) T(405, 245, tl > 7.8 ? 'offset aage' : 'khabar padi', { c: C.acc, b: 1, size: 12, halo: 1 });
  dot(mv([460, 185], [688, 185], 3.6, 4.6, tl), C.msg);
  if (tl > 4.6 && tl < 6.5) T(580, 135, 'fail -> khabar Kafka me padi', { c: C.bad, b: 1, size: 12 });
  dot(mv([460, 185], [688, 185], 6.8, 7.8, tl), C.good);
  if (tl > 7.8) T(775, 270, 'email gaya', { c: C.good, b: 1, size: 15 });
}
