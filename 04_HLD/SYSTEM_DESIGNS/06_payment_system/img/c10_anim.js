// Card 10: ek DB, 1000 crore row, index RAM me nahi, disk se, likhai dheemi. Ilaaj: shard by account_id.
const DUR = 17, FIX = 7.5;
const SV = [160, 185], DBL = [318, 185], DBR = [570, 200], DK = [648, 245];
const SH = [[408, 60], [408, 180], [408, 300]];
function setup() {
  wire('M160 185 H330', 'w1'); wire('M570 150 L660 95', 'w2'); wire('M570 200 L660 245', 'w3');
  wire('M160 185 L420 60', 'g1'); wire('M160 185 H420', 'g2'); wire('M160 185 L420 300', 'g3');
  box(20, 150, 140, 70, 'Payment Svc');
  box(330, 90, 240, 170, 'SQL DB', '1000 crore row', { id: 'one' });
  box(660, 60, 190, 70, 'RAM', 'index nahi samaata', { id: 'ram' });
  box(660, 210, 190, 70, 'Disk', 'dheema', { id: 'dk' });
  box(420, 20, 220, 80, 'shard 1', '', { id: 'h1' });
  box(420, 135, 220, 90, 'shard 2', 'Arpan: balance + history', { id: 'h2', stroke: C.acc });
  box(420, 260, 220, 80, 'shard 3', '', { id: 'h3' });
}
function frame(t, fix, tl) {
  ['w1', 'w2', 'w3', 'one', 'ram', 'dk'].forEach(id => vis(id, !fix));
  ['g1', 'g2', 'g3', 'h1', 'h2', 'h3'].forEach(id => vis(id, fix));
  if (!fix) {
    CAP.textContent = 'DIKKAT: ek DB me 1000 crore row. Index RAM me nahi samaata, disk se padhta, likhai dheemi';
    cross(872, 95);
    for (let k = 0; k < 4; k++) {
      const s = 0.3 + 1.6 * k;
      if (tl > s && tl < s + 0.6) dot(path([[s, SV], [s + 0.6, DBL]], tl), C.msg, '', 'likhai');
      if (tl > s + 0.6 && tl < s + 2.4) dot(path([[s + 0.6, DBR], [s + 2.4, DK]], tl), C.bad, '', 'disk se...', C.bad);
    }
    if (tl >= 2.5) T(450, 300, 'har likhai dheemi', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: account_id se tukde (shard). Arpan ka balance + history ek shard pe, ek transaction';
  T(290, 205, 'account_id', { c: C.acc, size: 12 });
  for (let k = 0; k < 12; k++) {
    const s = 0.3 + 0.5 * k, i = k % 3;
    if (tl > s && tl < s + 0.5) dot(path([[s, SV], [s + 0.5, SH[i]]], tl), i === 1 ? C.acc : C.msg, '', i === 1 ? 'Arpan' : '');
  }
  if (tl >= 0.8) T(530, 210, 'ek transaction, ek shard', { c: C.good, b: 1, size: 12 });
  if (tl >= 1.5) T(760, 185, 'har likhai tez', { c: C.good, b: 1, size: 15 });
}
