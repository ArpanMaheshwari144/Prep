// Card 7: ek hi App machine giri -> booking band. Ilaaj: App stateless, hold DB me, LB GET /health.
// Agar: ek fail = jhatka (2-3 fail = bahar), payment beech me (same bookingId). Wapas: App-2 wahi hold dekhti.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek hi App machine. Main seat chun raha tha, wo giri, booking band'],
  [3, GRN, 'ILAAJ', 'ILAAJ: App stateless, hold DB me. LB GET /health, 2-3 fail = machine bahar'],
  [6, YEL, 'AGAR', 'AGAR giri machine bahar kaise? -> 2-3 baar fail = bahar, ek fail jhatka'],
  [9, YEL, 'AGAR', 'AGAR payment beech me tha, machine giri? -> same bookingId, doosri machine'],
  [12, GRN, 'WAPAS', 'WAPAS: App-2 wahi hold dekhti (DB me tha). Seat wahin rehti hai'],
];
function setup() {}
function core(mSub, mSt, a1Sub, a1St, a2, dbSub, dbSt) {
  B(20, 150, 110, 70, 'Main', mSub, { st: mSt });
  B(360, 60, 120, 60, 'App-1', a1Sub, { st: a1St });
  if (a2) { B(190, 160, 80, 50, 'LB'); B(360, 250, 120, 60, 'App-2', a2 === 1 ? '' : a2, { st: a2 === 1 ? '' : 'good' }); }
  B(650, 150, 200, 80, 'SQL DB', dbSub || 'A1: HELD (Main)', { st: dbSt || 'acc' });
}
function frame(ph) {
  if (ph === 0) {
    core('booking band', 'bad', 'giri', 'bad', 0);
    A(130, 170, 358, 85, C.msg, { n: 1, at: 0.4 }); L(225, 110, 'hold A1', C.msg);
    A(480, 95, 648, 170, C.msg, { n: 2 });
    cross(500, 52); num(530, 40, 3, C.bad); L(550, 45, 'App-1 giri', C.bad, 'start');
    A(130, 200, 358, 112, C.bad, { dash: 1, n: 4, at: 0.4 }); L(270, 190, 'Pay: koi nahi', C.bad);
  } else if (ph === 1) {
    core('', '', 'bahar', 'bad', 1);
    cross(500, 52); num(530, 40, 1, C.bad); L(550, 45, 'App-1 giri', C.bad, 'start');
    A(250, 158, 358, 105, C.acc, { dash: 1, n: 2, at: 0.4 }); L(200, 125, 'GET /health', C.acc);
    T(300, 95, 'fail: 3', { c: C.bad, b: 1, size: 13 });
    A(130, 185, 188, 185, C.msg, { n: 3 });
    A(250, 212, 358, 270, C.good, { n: 4 }); L(250, 270, 'App-2 pe', C.good);
  } else if (ph === 2) {
    B(360, 60, 120, 60, 'App-1', 'ek fail', { st: 'acc' }); B(190, 160, 80, 50, 'LB');
    A(250, 158, 358, 105, C.acc, { dash: 1, n: 1, at: 0.4 }); L(200, 125, 'GET /health', C.acc);
    T(300, 95, 'fail: 1', { c: C.acc, b: 1, size: 13 });
    num(530, 90, 2, C.good); L(550, 95, 'sirf jhatka, andar hi', C.good, 'start');
    L(330, 345, 'hal: 2-3 baar fail ho, tabhi bahar', C.good);
  } else if (ph === 3) {
    core('Pay (dobara)', 'acc', 'giri', 'bad', 1, 'idempotent', 'good');
    cross(500, 52); num(530, 40, 1, C.bad); L(550, 45, 'beech me giri', C.bad, 'start');
    A(130, 185, 188, 185, C.msg, { n: 2 }); L(160, 255, 'same bookingId', C.msg);
    A(250, 212, 358, 270, C.msg, { n: 3 });
    A(480, 280, 648, 205, C.good, { n: 4 }); L(600, 280, 'do baar nahi katega', C.good);
  } else {
    core('seat wahin', 'good', 'giri', 'dim', 'wahi hold', '', 'good');
    A(130, 185, 188, 185, C.msg, { n: 1 });
    A(250, 212, 358, 270, C.msg, { n: 2 });
    A(480, 280, 648, 205, C.good, { n: 3 }); L(600, 280, 'HELD mila', C.good);
  }
}
