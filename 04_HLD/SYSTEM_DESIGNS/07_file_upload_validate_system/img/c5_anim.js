// Card 5: deadline ke din sab har 2 sec haal poochh rahe, primary DB ghira. Ilaaj: Redis + read replica.
// Agar: DB me DONE par Redis purana, replica peeche, Redis gira. Wapas: Redis wapas.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 10,000 log har 2 sec seedha DB pe, primary DB ghir gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: GET /status pehle Redis (1 ms). Nahi mila to replica, Redis me rakho'],
  [6, YEL, 'AGAR', 'AGAR DB me DONE, app me VALIDATING? -> DONE pe Redis key hatao'],
  [9, YEL, 'AGAR', 'AGAR replica peeche, purana haal Redis me baitha? -> SET NX + chhoti umar'],
  [12, YEL, 'AGAR', 'AGAR Redis gira? -> seedha replica pe jao, dheema par chalta hai'],
  [15, GRN, 'WAPAS', 'WAPAS: Redis wapas aaya -> haal phir Redis se, 1 ms'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 150, 120, 70, '10,000 log', 'har 2 sec', { st: o.u });
  if (!o.noC) {
    B(330, 30, 150, 70, 'Redis', s.rd || '1 ms', { st: o.rd });
    B(330, 265, 150, 70, 'Read replica', s.rp || 'copy', { st: o.rp });
    A(638, 205, 482, 290, C.line);
  }
  B(640, 150, 160, 70, 'Primary DB', s.db || 'sach', { st: o.db });
  B(650, 20, 140, 60, 'Worker', '', { st: o.wk });
}
function frame(ph) {
  if (ph === 0) {
    base({ noC: 1, u: 'bad', db: 'bad' }, { db: 'ghir gaya' });
    dots(6, 30, 250, C.msg, { gap: 18 });
    A(140, 185, 638, 185, C.bad, { n: 1, w: 4 }); L(390, 170, 'GET /status/T-101, har 2 sec', C.msg);
    num(720, 245, 2, C.bad); L(738, 250, 'ghir gaya', C.bad, 'start');
    T(390, 240, 'sab dheema', { c: C.bad, b: 1, size: 20 });
  } else if (ph === 1) {
    base({ u: 'good', rd: 'good', rp: 'acc', db: 'good' }, { rd: 'T-101: VALIDATING' });
    A(140, 165, 328, 70, C.good, { n: 1, at: 0.5 }); L(215, 85, 'pehle Redis', C.good, 'end');
    A(140, 205, 328, 295, C.msg, { n: 2, at: 0.5 }); L(215, 290, 'nahi mila: replica', C.msg, 'end');
    A(405, 263, 405, 102, C.acc, { n: 3 }); L(420, 185, 'Redis me rakh do', C.acc, 'start');
  } else if (ph === 2) {
    base({ u: 'bad', rd: 'bad', db: 'good' }, { rd: 'T-101: VALIDATING', db: 'T-101: DONE' });
    A(720, 82, 720, 148, C.good, { n: 1 }); L(735, 120, 'DONE likha', C.good, 'start');
    num(500, 22, 2, C.bad); L(518, 27, 'purana pada', C.bad, 'start');
    A(328, 80, 142, 170, C.bad, { n: 3, at: 0.5 }); L(215, 100, 'VALIDATING', C.bad, 'end');
    L(500, 320, 'hal: DONE likho + Redis key hatao', C.good, 'start');
    L(500, 342, 'chhoot gaya to umar (TTL) baad naya', C.good, 'start');
  } else if (ph === 3) {
    base({ rd: 'bad', rp: 'acc', db: 'good' }, { rd: 'T-101: VALIDATING', rp: 'VALIDATING (peeche)', db: 'T-101: DONE' });
    A(720, 82, 720, 148, C.good, { n: 1 }); L(735, 120, 'DONE likha', C.good, 'start');
    num(500, 300, 2, C.acc); L(518, 305, 'replica peeche', C.acc, 'start');
    A(405, 263, 405, 102, C.bad, { n: 3 }); L(420, 185, 'purana haal baith gaya', C.bad, 'start');
    L(20, 300, 'hal: miss pe sirf SET NX + chhoti umar,', C.good, 'start');
    L(20, 322, 'ya primary se padho', C.good, 'start');
  } else if (ph === 4) {
    base({ u: 'good', rd: 'bad', rp: 'good', db: 'good' }, { rd: 'gira' });
    cross(505, 65); num(540, 65, 1, C.bad); L(558, 70, 'gira', C.bad, 'start');
    A(140, 205, 328, 295, C.good, { n: 2, at: 0.5 }); L(215, 290, 'seedha replica', C.good, 'end');
    L(215, 312, 'dheema par chalta', C.acc, 'end');
    L(215, 120, 'hal: poochne pe rate limit', C.good, 'end');
  } else {
    base({ u: 'good', rd: 'good', rp: 'good', db: 'good' }, { rd: 'wapas, 1 ms' });
    num(500, 22, 1, C.good); L(518, 27, 'wapas aaya', C.good, 'start');
    A(140, 165, 328, 70, C.good, { n: 2, at: 0.5 }); L(215, 85, 'pehle Redis', C.good, 'end');
  }
}
