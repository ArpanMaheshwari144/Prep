// Card 8: order DB me bana, Kafka bhejne se pehle server gira -> khabar gayab. Ilaaj: outbox (ek transaction) + relay.
const DUR = 18, FIX = 8;
function setup() {
  box(40, 155, 150, 74, 'Order Svc');
  box(310, 50, 210, 120, 'DB');
  box(690, 155, 140, 74, 'Kafka');
  wire('M190 180 L310 120'); wire('M190 205 H690', 'wdir');
  box(340, 260, 150, 60, 'Relay', 'har 1 sec', { id: 'rel', stroke: C.acc });
  wire('M415 170 V260', 'wr1'); wire('M490 290 L690 215', 'wr2');
}
function frame(t, fix, tl) {
  ['rel', 'wr1', 'wr2'].forEach(id => vis(id, fix)); vis('wdir', !fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: DB aur Kafka do alag kaam. #101 DB me likh gaya, Kafka bhejne se pehle crash';
    if (tl > 1.0) cells(330, 95, [['orders: 101', C.good]], { w: 170, h: 26 });
    dot(path([[0.4, [190, 180]], [1.0, [310, 120]]], tl < 1.0 ? tl : -1), C.msg);
    if (tl > 1.8) { cross(115, 192); T(115, 260, 'server gira', { c: C.bad, b: 1 }); }
    if (tl > 2.6) T(760, 260, '#101 ki khabar kabhi nahi aayi', { c: C.bad, b: 1, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: order + khabar EK transaction me (outbox). Relay commit ke BAAD Kafka ko bhejta';
  dot(path([[0.4, [190, 180]], [1.0, [310, 120]]], tl < 1.0 ? tl : -1), C.msg);
  if (tl > 1.0) { cells(330, 85, [['orders: 101', C.good]], { w: 170, h: 24 });
    cells(330, 118, [[tl > 4.2 ? 'outbox: E-101 sent' : 'outbox: E-101 baaki', tl > 4.2 ? C.good : C.acc]], { w: 170, h: 24 }); }
  if (tl > 1.0 && tl < 2.2) T(600, 70, 'BEGIN .. COMMIT: dono ya koi nahi', { c: C.acc, size: 12, a: 'start' });
  if (tl > 1.8) { cross(115, 192); T(115, 260, 'server gira', { c: C.bad, b: 1 }); }
  if (tl > 2.6) dot(path([[2.6, [415, 170]], [3.1, [415, 260]], [3.6, [490, 290]], [4.2, [690, 215]]], tl < 4.2 ? tl : -1), C.acc, '', 'E-101', C.acc);
  if (tl > 4.2) T(760, 260, 'khabar pahunchi', { c: C.good, b: 1 });
}
