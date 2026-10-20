// Card 10: ek DB, 1000 crore row, index RAM me nahi, disk se, likhai dheemi. Ilaaj: shard by account_id.
// Agar: do account alag shard (saga), ek account bahut bada (sub-account), Swiggy 3000 credit/sec (ledger line). Wapas.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek DB, 1000 crore row. Index RAM me nahi, disk se, likhai dheemi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: account_id se tukde (shard). Mera balance + history ek shard pe'],
  [6, YEL, 'AGAR', 'AGAR do account alag shard pe? -> saga: A se kaato, B me daalo'],
  [9, YEL, 'AGAR', 'AGAR ek account bahut bada (merchant)? -> sub-account alag shard pe'],
  [12, YEL, 'AGAR', 'AGAR Swiggy pe 3000 credit/sec, lock ki line? -> credit sirf ledger line'],
  [15, GRN, 'WAPAS', 'WAPAS: jo saath chahiye wo ek shard pe, transaction ek shard tak'],
];
function setup() {}
const HY = [20, 135, 260];
function shards(st = [], sub = []) {
  B(20, 150, 140, 70, 'Payment Svc');
  HY.forEach((y, i) => B(420, y, 220, i === 1 ? 90 : 80, `shard ${i + 1}`, sub[i] || '', { st: st[i] }));
}
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 140, 70, 'Payment Svc');
    B(300, 110, 240, 150, 'SQL DB', '1000 crore row', { st: 'bad' });
    B(660, 40, 200, 70, 'RAM', 'index nahi samaata', { st: 'bad' });
    B(660, 240, 200, 70, 'Disk', 'dheema', { st: 'acc' });
    A(160, 185, 298, 185, C.msg, { n: 1 }); L(230, 170, 'likhai', C.msg);
    A(540, 140, 658, 80, C.bad, { dash: 1, n: 2, at: 0.4 }); cross(620, 100);
    A(540, 230, 658, 270, C.bad, { n: 3, at: 0.4 }); L(600, 215, 'disk se padhta', C.bad, 'start');
    L(420, 300, 'har likhai dheemi', C.bad);
  } else if (ph === 1) {
    shards(['', 'good', ''], ['', 'Arpan: balance + history', '']);
    A(160, 170, 418, 60, C.msg, { n: 1, at: 0.5 }); A(160, 185, 418, 180, C.acc, { n: 1, at: 0.6 });
    A(160, 200, 418, 300, C.msg, { n: 1, at: 0.5 }); L(300, 215, 'account_id', C.acc);
    num(660, 180, 2, C.good); L(678, 185, 'ek transaction,', C.good, 'start'); L(678, 205, 'ek shard', C.good, 'start');
    L(760, 300, 'har likhai tez', C.good);
  } else if (ph === 2) {
    shards(['acc', '', 'acc'], ['A', '', 'B']);
    A(160, 170, 418, 60, C.acc, { n: 1, at: 0.5 }); L(250, 95, 'A se kaato', C.acc);
    A(160, 200, 418, 300, C.good, { n: 2, at: 0.5 }); L(250, 280, 'B me daalo', C.good);
    L(760, 160, 'ek transaction', C.bad); L(760, 180, 'nahi chalti', C.bad);
    L(760, 300, 'hal: saga, fail pe ulta', C.good);
  } else if (ph === 3) {
    shards(['good', 'bad', 'good'], ['merchant sub-1', 'merchant: garam', 'merchant sub-2']);
    bar(660, 165, 140, 16, 1, ''); num(830, 173, 1, C.bad); L(730, 205, 'shard garam', C.bad);
    A(160, 170, 418, 60, C.good, { n: 2, at: 0.5 }); A(160, 200, 418, 300, C.good, { n: 2, at: 0.5 });
    L(250, 95, 'sub-account', C.good); L(250, 280, 'sub-account', C.good);
    L(760, 300, 'hal: report me jodo', C.good);
  } else if (ph === 4) {
    shards(['', 'bad', ''], ['', 'Swiggy row: lock', '']);
    A(160, 185, 418, 180, C.bad, { n: 1, at: 0.4 }); L(260, 160, '3000 credit/sec', C.bad);
    dots(6, 255, 215, C.acc, { gap: 20 }); num(230, 215, 2, C.bad); L(305, 250, 'lock ki line', C.bad);
    L(660, 70, 'hal: credit sirf ledger line me', C.good, 'start'); L(660, 92, '(koi lock nahi)', C.good, 'start');
    L(660, 290, 'balance peeche batch me', C.good, 'start'); L(660, 312, 'ya 10 sub-account', C.good, 'start');
  } else {
    shards(['good', 'good', 'good'], ['', 'Arpan: balance + history', '']);
    A(160, 170, 418, 60, C.good, { n: 1, at: 0.5 }); A(160, 185, 418, 180, C.good, { n: 1, at: 0.6 });
    A(160, 200, 418, 300, C.good, { n: 1, at: 0.5 }); L(300, 215, 'account_id', C.acc);
    num(660, 180, 2, C.good); L(678, 185, 'transaction ek', C.good, 'start'); L(678, 205, 'shard tak', C.good, 'start');
  }
}
