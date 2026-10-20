// Card 1: Arpan -500 hua, Kirana +500 se pehle server gira. Ilaaj: ek transaction, WAL se ulta.
// Agar: ledger INSERT fail, network toota, A->B + B->A deadlock. Wapas: COMMIT ke baad gira, kuch nahi khota.
// Naksha: paisa baayein se daayein -> Arpan (account) -> Payment Svc -> Kirana (account), ledger neeche.
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
// o: { a, sv, k, lg } = st ; s: { a, k, lg } = sub
function base(o = {}, s = {}) {
  B(20, 130, 150, 70, 'Arpan', s.a || 'account', { st: o.a });
  B(300, 130, 170, 70, 'Payment Svc', 'Arpan -> Kirana', { st: o.sv });
  B(600, 130, 150, 70, 'Kirana', s.k || 'account', { st: o.k });
  if (o.lg) B(300, 250, 170, 55, 'ledger', s.lg || '', { st: o.lg });   // ledger sirf jis scene me card laaye
}
function wal(lines, st) {
  B(560, 225, 320, 115, 'WAL (DB ka log)', '', { st: st || 'acc' });
  lines.forEach(([s, c], i) => T(580 + (i % 2) * 150, 275 + Math.floor(i / 2) * 22, s, { a: 'start', c, b: 1, size: 12 }));
}
function frame(ph) {
  if (ph === 0) {
    base({ a: 'acc', sv: 'bad', k: 'bad' }, { a: '-500 kata', k: '+500 nahi aaya' });
    A(170, 165, 298, 165, C.acc, { n: 1, at: 0.4 }); L(235, 150, '-500 nikla', C.acc);
    num(330, 110, 2, C.bad); L(348, 115, 'server gira', C.bad, 'start'); cross(490, 135);
    A(470, 180, 598, 180, C.bad, { dash: 1, n: 3, at: 0.3 }); cross(560, 180);
    L(535, 220, 'pahuncha hi nahi', C.bad);
    L(780, 270, 'kata: 500', C.acc, 'start'); L(780, 295, 'juda: 0', C.bad, 'start');
    L(780, 330, '500 gayab', C.bad, 'start');
  } else if (ph === 1) {
    base({ a: 'good', sv: 'bad', k: '' }, { a: '500 wapas', k: '+500 likha nahi' });
    wal([['BEGIN', C.fg], ['Arpan -500', C.acc], ['COMMIT nahi', C.bad]]);
    A(170, 165, 298, 165, C.acc, { n: 1, at: 0.4 }); L(235, 150, 'BEGIN, -500', C.acc);
    num(330, 110, 2, C.bad); L(348, 115, 'COMMIT se pehle gira', C.bad, 'start'); cross(490, 135);
    AP([[560, 330], [95, 330], [95, 202]], C.good, { n: 3, at: 0.5 });
    L(330, 40, 'DB utha, log: COMMIT nahi -> ulta', C.good);
    L(330, 62, 'kata 0 == juda 0, kuch nahi khoya', C.good);
  } else if (ph === 2) {
    base({ a: 'good', k: 'good', lg: 'bad' }, { a: '-500 ulta', k: '+500 ulta', lg: 'INSERT fail' });
    A(170, 165, 298, 165, C.acc, { n: 1, at: 0.4 }); A(470, 165, 598, 165, C.acc, { n: 1, at: 0.6 });
    L(235, 150, '-500', C.acc); L(535, 150, '+500', C.acc);
    A(385, 200, 385, 248, C.bad, { n: 2 }); L(400, 228, 'INSERT fail', C.bad, 'start');
    AP([[300, 290], [95, 290], [95, 202]], C.good, { n: 3, at: 0.5 });
    AP([[470, 290], [675, 290], [675, 202]], C.good, { n: 3, at: 0.5 });
    L(385, 110, 'ROLLBACK: dono ulte', C.good);
    L(95, 115, 'user: "nahi hua"', C.good);
    L(450, 346, 'hal: ROLLBACK, paanchon me se ek bhi nahi bachta', C.good);
  } else if (ph === 3) {
    base({ a: 'dim', sv: 'acc', k: 'dim' }, { a: 'balance ?' });
    B(300, 20, 170, 50, 'Arpan app', 'pay 500');
    A(365, 72, 365, 128, C.msg, { n: 1, at: 0.5 });
    A(298, 175, 172, 175, C.bad, { dash: 1, n: 2, at: 0.25 }); cross(220, 175);
    L(235, 220, 'network toota', C.bad);
    A(410, 128, 410, 72, C.bad, { n: 3, at: 0.5 }); L(425, 105, 'mana', C.bad, 'start');
    L(385, 330, 'balance pakka nahi -> payment mana', C.acc);
    B(600, 250, 200, 55, 'Dashboard', 'thoda purana', { st: 'good' });
    L(700, 325, 'chalta rahe, chalega', C.good);
    L(450, 350, 'hal: sahi hona pehle. Galat balance pe kaatna usse bhi bura', C.good);
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
    base({ a: 'good', sv: 'bad', k: 'good' }, { a: '-500', k: '+500' });
    wal([['BEGIN', C.fg], ['Arpan -500', C.good], ['Kirana +500', C.good], ['COMMIT', C.good]], 'good');
    A(170, 165, 298, 165, C.good, { n: 1, at: 0.4 }); A(470, 165, 598, 165, C.good, { n: 1, at: 0.6 });
    L(385, 110, 'COMMIT ho gaya', C.good);
    num(330, 85, 2, C.bad); L(348, 90, 'phir gira', C.bad, 'start'); cross(490, 135);
    A(720, 223, 700, 202, C.good, { n: 3, at: 0.2 });
    L(330, 40, 'DB utha, log me COMMIT -> kaam poora', C.good);
    L(190, 330, 'kata 500 == juda 500', C.good);
  }
}
