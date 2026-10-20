// Card 1: sabko wahi 20 khabar, har baar DB se sort. Ilaaj: feed ek baar Redis sorted set me.
// Agar: breaking news turant (ZADD), Redis restart (lock ke saath ek banaye). Wapas: sab Redis se.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: subah sab ne khola, har baar DB se ORDER BY ... LIMIT 20'],
  [3, GRN, 'ILAAJ', 'ILAAJ: feed ek baar bani, Redis sorted set. ZREVRANGE 0 19 = 20 id'],
  [6, YEL, 'AGAR', 'AGAR breaking news kitni der me feed me? -> aate hi ZADD, turant'],
  [9, YEL, 'AGAR', 'AGAR Redis restart, list khaali? -> DB se ek baar banao, lock ke saath'],
  [12, GRN, 'WAPAS', 'WAPAS: list phir bani, sab Redis se padhte. DB tak koi nahi jaata'],
];
function setup() {}
function base(o = {}) {
  B(20, 140, 110, 70, 'Users', 'subah');
  dots(3, 40, 232, C.msg, { gap: 22 });
  B(250, 140, 140, 70, 'Feed Svc', '', { st: o.f });
  if (o.r !== undefined) B(520, 40, 180, 70, 'Redis', o.r, { st: o.rst });
  if (o.fe !== undefined) B(760, 40, 120, 70, 'Fetcher', o.fe, { st: o.fest });
  B(520, 250, 180, 70, 'DB', o.db, { st: o.dbst });
}
function frame(ph) {
  if (ph === 0) {
    base({ f: 'bad', db: 'ORDER BY ... LIMIT 20', dbst: 'bad' });
    A(130, 165, 248, 165, C.msg, { n: 1 }); L(190, 140, 'feed kholo', C.msg);
    A(390, 195, 518, 275, C.bad, { n: 2 }); L(440, 290, 'har baar sort', C.bad, 'end');
    bar(720, 270, 150, 18, 1, 'DB CPU'); T(795, 315, '500 / sec sort', { c: C.bad, b: 1, size: 14 });
    A(248, 195, 130, 195, C.bad, { dash: 1, n: 3 }); L(190, 225, 'dheemi', C.bad);
  } else if (ph === 1) {
    base({ f: 'good', r: 'feed:latest', rst: 'good', fe: '', dbst: 'dim' });
    A(758, 75, 702, 75, C.acc, { n: 1 }); L(731, 103, 'ZADD', C.acc);
    A(130, 165, 248, 165, C.msg, { n: 2 }); L(190, 140, 'feed kholo', C.msg);
    A(390, 150, 518, 85, C.good, { n: 3 }); L(525, 135, 'ZREVRANGE 0 19 = 20 id', C.good, 'start');
    A(248, 195, 130, 195, C.good, { n: 4 }); L(190, 225, '20 id, tez', C.good);
    L(610, 238, 'DB tak koi nahi', C.good);
  } else if (ph === 2) {
    base({ f: 'good', r: 'feed:latest', rst: 'good', fe: 'breaking!', fest: 'acc', dbst: 'dim' });
    A(758, 75, 702, 75, C.acc, { n: 1 }); L(731, 103, 'ZADD', C.acc);
    cells(520, 125, [['nayi', C.acc], ['k1', C.good], ['k2', C.good], ['..', C.muted]], { w: 40, h: 24 });
    L(705, 142, 'sabse upar', C.acc, 'start');
    A(130, 165, 248, 165, C.msg, { n: 2 }); L(190, 140, 'feed kholo', C.msg);
    A(390, 150, 518, 85, C.good, { n: 3 });
    A(248, 195, 130, 195, C.good, { n: 4 }); L(190, 225, 'turant dikhi', C.good);
    L(20, 25, 'hal: aate hi ZADD, turant. "5 min" sirf sabse bura haal', C.good, 'start');
  } else if (ph === 3) {
    base({ f: 'acc', r: 'list khaali', rst: 'bad', db: 'ek baar', dbst: 'acc' });
    cross(728, 75); num(728, 30, 1, C.bad); L(746, 35, 'restart', C.bad, 'start');
    A(130, 165, 248, 165, C.msg, { n: 2 }); L(190, 140, 'feed kholo', C.msg);
    A(390, 195, 518, 275, C.acc, { n: 3 }); L(430, 282, 'ek banata (lock)', C.acc, 'end');
    dots(3, 265, 232, C.acc, { gap: 22, hollow: 1 }); L(300, 260, 'baaki ruke', C.acc);
    A(610, 248, 610, 112, C.good, { n: 4 }); L(630, 190, 'list bani', C.good, 'start');
    L(20, 25, 'hal: ek banaye, baaki ruk ke Redis se padhein', C.good, 'start');
  } else {
    base({ f: 'good', r: 'list wapas', rst: 'good', fe: '', dbst: 'dim' });
    A(130, 165, 248, 165, C.msg, { n: 1 }); L(190, 140, 'feed kholo', C.msg);
    A(390, 150, 518, 85, C.good, { n: 2 }); L(525, 135, 'ZREVRANGE 0 19', C.good, 'start');
    A(248, 195, 130, 195, C.good, { n: 3 }); L(190, 225, '20 id, tez', C.good);
    L(610, 238, 'DB tak koi nahi', C.good);
  }
}
