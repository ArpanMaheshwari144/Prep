// Card 6: wallet -500 hua, Bank B payout fail, ek transaction nahi -> 500 latka. Ilaaj: SAGA, fail pe ulta.
// Agar: SAGA ke beech service giri, ulta kaam bhi fail. Wapas: PROCESSING phir SUCCESS ya paisa wapas.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: wallet hamare DB, payout Bank B me. Ek transaction nahi, 500 latka'],
  [3, GRN, 'ILAAJ', 'ILAAJ: SAGA, har kadam table me. Payout fail -> ulta: wallet me 500 wapas'],
  [6, YEL, 'AGAR', 'AGAR SAGA ke beech service giri? -> uth ke saga table dekhe, wahin se aage'],
  [9, YEL, 'AGAR', 'AGAR ulta kaam bhi fail? -> retry + alert, phir manual queue'],
  [12, GRN, 'WAPAS', 'WAPAS: user ko PROCESSING, phir SUCCESS ya paisa wapas. Hisaab saaf'],
];
function setup() {}
// o: { sv, wl, bk } st ; s: { wl, bk } ; rows = saga table
function base(o = {}, s = {}, rows) {
  B(330, 30, 200, 70, 'Payment Svc', '+ SAGA', { st: o.sv });
  B(40, 190, 170, 70, 'Arpan wallet', s.wl || 'hamara DB', { st: o.wl });
  B(680, 190, 170, 70, 'Bank B', s.bk || 'merchant payout', { st: o.bk });
  if (rows) {
    B(330, 190, 200, 125, 'saga table', '', { st: 'acc' });
    rows.forEach(([t, c], i) => T(345, 240 + i * 24, t, { a: 'start', c, b: 1, size: 13 }));
  }
}
function frame(ph) {
  if (ph === 0) {
    base({ wl: 'acc', bk: 'bad' }, { wl: '-500 kata', bk: 'payout fail' });
    A(330, 60, 125, 188, C.acc, { n: 1, at: 0.4 }); L(200, 95, '-500', C.acc);
    A(530, 60, 765, 188, C.msg, { n: 2, at: 0.4 }); L(660, 95, '+500 payout', C.msg);
    num(765, 280, 3, C.bad); L(785, 285, 'fail', C.bad, 'start'); cross(872, 225);
    L(430, 230, '500 beech me latka', C.bad); L(430, 252, 'na wallet me, na merchant ko', C.bad);
    L(430, 300, 'do alag system, ek transaction nahi', C.muted);
  } else if (ph === 1) {
    base({ wl: 'good', bk: 'bad' }, { wl: '500 wapas', bk: 'payout fail' },
      [['1. DEBITED (-500)', C.acc], ['2. PAYOUT: fail', C.bad], ['3. ulta: +500 wapas', C.good]]);
    A(330, 60, 125, 188, C.acc, { n: 1, at: 0.4 }); L(200, 95, '-500', C.acc);
    A(530, 60, 765, 188, C.msg, { n: 2, at: 0.4 }); L(660, 95, '+500 payout', C.msg);
    num(765, 280, 3, C.bad); L(785, 285, 'fail', C.bad, 'start'); cross(872, 225);
    A(345, 100, 160, 188, C.good, { n: 4, at: 0.6 }); L(290, 170, '+500 wapas', C.good, 'start');
    A(430, 100, 430, 188, C.line); L(440, 150, 'har kadam likha', C.muted, 'start');
  } else if (ph === 2) {
    base({ sv: 'bad', wl: 'acc' }, { wl: '-500 kata' }, [['1. DEBITED (-500)', C.acc], ['2. PAYOUT: ?', C.muted]]);
    A(330, 60, 125, 188, C.acc, { n: 1, at: 0.4 }); L(200, 95, '-500', C.acc);
    num(560, 40, 2, C.bad); L(578, 45, 'service giri', C.bad, 'start'); cross(555, 85);
    A(450, 188, 450, 102, C.good, { n: 3 }); L(470, 150, 'uth ke saga table dekha', C.good, 'start');
    L(440, 345, 'hal: wahin se aage badhe ya ulta kare', C.good);
  } else if (ph === 3) {
    base({ wl: 'bad', bk: 'bad' }, { wl: '+500 nahi aaya', bk: 'payout fail' },
      [['1. DEBITED (-500)', C.acc], ['2. PAYOUT: fail', C.bad], ['3. ulta: fail', C.bad]]);
    B(680, 30, 180, 60, 'manual queue', '', { st: 'good' });
    num(765, 280, 1, C.bad); L(785, 285, 'fail', C.bad, 'start');
    A(345, 100, 160, 188, C.bad, { dash: 1, n: 2, at: 0.6 }); L(290, 170, 'ulta fail', C.bad, 'start');
    num(430, 15, 3, C.acc); L(448, 20, 'retry + alert', C.acc, 'start');
    A(530, 60, 678, 60, C.good, { n: 4 }); L(605, 110, 'phir manual', C.good);
    L(440, 345, 'hal: retry + alert, phir manual queue. Paisa beech me na atke', C.good);
  } else {
    base({ wl: 'good', bk: '' }, { wl: '500 wapas', bk: 'payout fail' },
      [['1. DEBITED (-500)', C.acc], ['2. PAYOUT: fail', C.bad], ['3. ulta: +500 wapas', C.good]]);
    num(30, 165, 1, C.acc); L(48, 170, 'PROCESSING', C.acc, 'start');
    A(345, 100, 160, 188, C.good, { n: 2, at: 0.6 }); L(290, 170, '+500 wapas', C.good, 'start');
    L(125, 290, 'user: paisa wapas', C.good);
    L(605, 330, 'hisaab saaf', C.good);
  }
}
