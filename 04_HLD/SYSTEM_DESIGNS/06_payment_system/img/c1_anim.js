// Card 1: Arpan -500 hua, Kirana +500 se pehle server gira. Ilaaj: ek transaction, WAL se ulta.
// Agar: ledger INSERT fail, network toota, A->B + B->A deadlock. Wapas: COMMIT ke baad gira, kuch nahi khota.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Arpan -500 hua, phir server gira, Kirana +500 hua hi nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: -500, +500 ek transaction. COMMIT se pehle gira -> WAL se ulta'],
  [6, YEL, 'AGAR', 'AGAR ledger ka INSERT fail? -> ROLLBACK, Arpan ka -500 bhi ulta'],
  [9, YEL, 'AGAR', 'AGAR network toota, consistency ya availability? -> sahi hona pehle'],
  [12, YEL, 'AGAR', 'AGAR A->B aur B->A ek saath, dono atke? -> taala pehle chhoti account id pe'],
  [15, GRN, 'WAPAS', 'WAPAS: COMMIT ke baad gira -> DB log dekh ke kaam poora, kuch nahi khota'],
];
function setup() {}
// o: { sv, a, k, lg } = st ; s: { a, k, lg } = sub
function base(o = {}, s = {}) {
  B(20, 150, 150, 70, 'Payment Svc', 'Arpan -> Kirana', { st: o.sv });
  B(300, 30, 300, 300, 'SQL DB', '', { st: 'dim' });
  B(330, 70, 240, 55, 'Arpan', s.a || '', { st: o.a });
  B(330, 155, 240, 55, 'Kirana', s.k || '', { st: o.k });
  B(330, 240, 240, 55, 'ledger', s.lg || '2 line', { st: o.lg });
}
function wal(lines, st) {
  B(670, 60, 200, 200, 'WAL (DB ka log)', '', { st: st || 'acc' });
  lines.forEach(([s, c], i) => T(690, 110 + i * 26, s, { a: 'start', c, b: 1, size: 13 }));
}
function frame(ph) {
  if (ph === 0) {
    base({ sv: 'bad', a: 'acc', k: 'bad' }, { a: '-500 kata', k: '+500 nahi aaya' });
    A(170, 165, 328, 100, C.acc, { n: 1, at: 0.45 }); L(245, 110, '-500', C.acc);
    num(95, 120, 2, C.bad); L(113, 125, 'server gira', C.bad, 'start'); cross(195, 205);
    A(170, 195, 328, 185, C.bad, { dash: 1, n: 3, at: 0.6 }); L(245, 225, 'gaya hi nahi', C.bad);
    L(770, 160, 'kata: 500', C.acc); L(770, 185, 'juda: 0', C.bad);
    L(770, 225, '500 gayab', C.bad);
  } else if (ph === 1) {
    base({ sv: 'bad', a: 'good', k: '' }, { a: '500 wapas', k: '+500 likha nahi' });
    wal([['BEGIN', C.fg], ['Arpan -500', C.acc], ['COMMIT nahi', C.bad]]);
    A(170, 165, 328, 100, C.acc, { n: 1, at: 0.45 }); L(245, 110, 'BEGIN, -500', C.acc);
    num(30, 250, 2, C.bad); L(48, 255, 'COMMIT se pehle gira', C.bad, 'start'); cross(195, 205);
    A(668, 120, 572, 98, C.good, { n: 3, at: 0.5 });
    L(885, 290, 'DB utha, log: COMMIT nahi -> ulta', C.good, 'end');
    L(885, 312, 'kata 0 == juda 0, kuch nahi khoya', C.good, 'end');
  } else if (ph === 2) {
    base({ a: 'good', k: 'good', lg: 'bad' }, { a: '-500 ulta', k: '+500 ulta', lg: 'INSERT fail' });
    A(170, 165, 328, 100, C.acc, { n: 1, at: 0.45 }); L(245, 110, '-500, +500', C.acc);
    num(625, 320, 2, C.bad); L(645, 325, 'INSERT fail', C.bad, 'start');
    AP([[572, 268], [640, 268], [640, 98], [572, 98]], C.good, { n: 3, at: 0.6 });
    L(660, 150, 'ROLLBACK', C.good, 'start');
    L(450, 346, 'hal: ROLLBACK, paanchon me se ek bhi nahi bachta', C.good);
    L(95, 255, 'user: "nahi hua"', C.good);
  } else if (ph === 3) {
    base({ sv: 'acc', a: 'dim' }, { a: 'balance ?' });
    B(20, 40, 150, 55, 'Arpan app', 'pay 500');
    A(95, 95, 95, 148, C.msg, { n: 1 });
    A(170, 165, 328, 100, C.bad, { dash: 1, n: 2, at: 0.3 }); cross(255, 135);
    L(230, 90, 'network toota', C.bad);
    A(60, 148, 60, 97, C.bad, { n: 3 }); L(48, 125, 'mana', C.bad, 'end');
    L(95, 255, 'balance pakka nahi', C.acc); L(95, 275, '-> payment mana', C.bad);
    L(450, 346, 'hal: sahi hona pehle. Galat balance pe kaatna usse bhi bura', C.good);
    B(670, 120, 200, 70, 'Dashboard', 'thoda purana', { st: 'good' });
    L(770, 215, 'chalta rahe, chalega', C.good);
  } else if (ph === 4) {
    B(20, 50, 150, 60, 'txn A->B', '');
    B(20, 230, 150, 60, 'txn B->A', '');
    B(300, 30, 300, 300, 'SQL DB', '', { st: 'dim' });
    B(330, 70, 240, 55, 'A', 'taala: txn A->B', { st: 'acc' });
    B(330, 230, 240, 55, 'B', 'taala: txn B->A', { st: 'acc' });
    A(170, 80, 328, 92, C.acc, { n: 1, at: 0.5 }); A(170, 260, 328, 255, C.acc, { n: 1, at: 0.5 });
    A(170, 100, 328, 240, C.bad, { dash: 1, n: 2, at: 0.2 }); A(170, 245, 328, 110, C.bad, { dash: 1, n: 2, at: 0.2 });
    L(218, 122, 'B ka wait', C.bad, 'start'); L(218, 234, 'A ka wait', C.bad, 'start');
    num(625, 175, 3, C.bad); L(645, 180, 'dono atke', C.bad, 'start');
    L(450, 346, 'hal: taala hamesha chhoti account id pe pehle lo', C.good);
  } else {
    base({ sv: 'bad', a: 'good', k: 'good', lg: 'good' }, { a: '-500', k: '+500', lg: '2 line' });
    wal([['BEGIN', C.fg], ['Arpan -500', C.good], ['Kirana +500', C.good], ['ledger 2 line', C.good], ['COMMIT', C.good]], 'good');
    A(170, 165, 328, 100, C.good, { n: 1, at: 0.45 }); L(245, 110, 'COMMIT ho gaya', C.good);
    num(95, 120, 2, C.bad); L(113, 125, 'phir gira', C.bad, 'start'); cross(195, 205);
    A(668, 180, 572, 182, C.good, { n: 3, at: 0.5 }); L(770, 290, 'DB utha, log me COMMIT', C.good);
    L(770, 312, '-> kaam poora', C.good);
    L(450, 346, 'kata 500 == juda 500, kuch nahi khoya', C.good);
  }
}
