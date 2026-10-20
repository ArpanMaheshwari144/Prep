// Card 9: source 100 guna, saari likhai ek DB machine pe -> disk aur likhai full.
// Ilaaj: article_id ke hash se shard. Agar: taazi 20 (scatter-gather), shard badhaya, do shard pe wahi khabar. Wapas.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: source 100 guna, saari likhai ek machine pe, disk aur likhai full'],
  [3, GRN, 'ILAAJ', 'ILAAJ: article_id se tukde, hash(a91) -> shard 2. Likhai sab pe barabar'],
  [6, YEL, 'AGAR', 'AGAR shard hain, taazi 20 kahan se? -> Redis se. Miss to har tukde se thoda'],
  [9, YEL, 'AGAR', 'AGAR shard badhaya? -> thoda hi data hilta (consistent hashing)'],
  [12, YEL, 'AGAR', 'AGAR do shard pe wahi khabar? -> dedupe pehle saanjhe Bloom / Redis se'],
  [15, GRN, 'WAPAS', 'WAPAS: sab tukde chalu, likhai sab tukdon pe barabar'],
];
const SH = [30, 140, 250];
function setup() {}
function shards(o = {}) {
  SH.forEach((y, i) => {
    B(500, y, 160, 60, 'shard ' + (i + 1), (o.sub || [])[i], { st: (o.st || [])[i] || 'good' });
    if (o.bars) bar(690, y + 22, 150, 18, o.bars[i], '', 1);
  });
}
function worker(sub) { B(20, 150, 130, 60, 'Worker', sub); }
function frame(ph) {
  if (ph === 0) {
    worker('100 guna');
    dots(10, 30, 240, C.msg, { gap: 12, r: 5 }); L(85, 275, '100 guna source', C.msg);
    B(400, 110, 200, 100, 'DB', 'ek machine', { st: 'bad' });
    A(150, 180, 398, 160, C.msg, { n: 1 }); L(275, 150, 'saari likhai', C.msg);
    num(620, 110, 2, C.bad); L(638, 115, 'disk full', C.bad, 'start');
    bar(620, 150, 220, 20, 1, ''); L(730, 200, 'likhai full', C.bad);
  } else if (ph === 1) {
    worker('hash(a91)'); shards({ bars: [0.4, 0.4, 0.4] });
    A(150, 180, 498, 170, C.good, { n: 1 }); L(320, 160, 'hash(a91) -> shard 2', C.good);
    A(150, 165, 498, 60, C.msg, { n: 1 }); A(150, 195, 498, 280, C.msg, { n: 1 }); L(330, 270, 'baaki id: shard 1, 3', C.msg);
    L(765, 20, 'bojh barabar', C.good);
  } else if (ph === 2) {
    B(20, 150, 130, 60, 'Feed Svc', 'taazi 20');
    B(220, 30, 140, 60, 'Redis', 'miss', { st: 'acc' });
    shards();
    A(110, 148, 218, 75, C.acc, { n: 1 }); L(120, 100, 'miss', C.acc, 'end');
    SH.forEach(y => A(150, 180, 498, y + 30, C.acc, { n: 2, at: 0.7 }));
    L(280, 260, 'har tukde se thoda', C.acc);
    T(780, 160, 'scatter-gather', { c: C.muted, size: 13 });
    AP([[580, 312], [580, 335], [85, 335], [85, 212]], C.good, { n: 3, at: 0.5 });
    L(330, 325, 'mila ke taazi 20', C.good);
  } else if (ph === 3) {
    worker('');
    [20, 100, 180].forEach((y, i) => B(500, y, 160, 60, 'shard ' + (i + 1), '', { st: 'good' }));
    B(500, 260, 160, 60, 'shard 4', 'naya', { st: 'acc' });
    num(690, 290, 1, C.acc); L(708, 295, 'naya shard', C.acc, 'start');
    AP([[660, 50], [760, 50], [760, 270], [662, 278]], C.acc, { n: 2, at: 0.5 });
    L(775, 140, 'thoda hi', C.acc, 'start'); L(775, 160, 'data hilta', C.acc, 'start');
    A(150, 180, 498, 290, C.good, { n: 3 }); L(300, 280, 'naye shard ka hissa', C.good);
    L(20, 30, 'hal: consistent hashing, thoda hi data hilta', C.good, 'start');
  } else if (ph === 4) {
    worker('match jeeta x2');
    B(220, 30, 160, 60, 'Bloom / Redis', 'saanjha', { st: 'good' });
    shards({ sub: ['match jeeta', '', 'dobara?'], st: ['good', 'good', 'bad'] });
    A(110, 148, 218, 75, C.acc, { n: 1 }); L(120, 100, 'pehle dekha?', C.acc, 'end');
    A(150, 170, 498, 60, C.good, { n: 2 }); L(400, 112, 'pehli: likho', C.good, 'start');
    A(150, 195, 498, 280, C.bad, { dash: 1, n: 3 }); cross(420, 252); L(300, 290, 'doosri: skip', C.good);
    L(680, 338, 'UNIQUE sirf shard ke andar', C.muted, 'start');
  } else {
    worker('hash(id)'); shards({ bars: [0.45, 0.45, 0.45] });
    A(150, 165, 498, 60, C.good, { n: 1 }); A(150, 180, 498, 170, C.good, { n: 1 });
    A(150, 195, 498, 280, C.good, { n: 1 });
    L(765, 20, 'bojh barabar', C.good); L(300, 320, 'sab tukde chalu', C.good); L(20, 30, 'har id apne tukde pe', C.good, 'start');
  }
}
