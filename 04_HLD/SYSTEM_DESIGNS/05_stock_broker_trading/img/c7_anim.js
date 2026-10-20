// Card 7: paisa Wallet DB me kata, Portfolio DB fail, share nahi. Ilaaj: status log + retry aage. Agar: retry dobara, match se pehle, ghanton fail, DB band. Wapas.
const DUR = 21;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Wallet DB me INFY ka 20k kata, Portfolio DB fail, share nahi aaye'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har kadam ka haal likho. Fail to likhe haal se retry, aage hi'],
  [6, YEL, 'AGAR', 'AGAR retry dobara chal gaya? -> har kadam trade_id se, ho chuka = no-op'],
  [9, YEL, 'AGAR', 'AGAR match se pehle hi atak gaya? -> sirf tab ulta (SAGA): block hatao'],
  [12, YEL, 'AGAR', 'AGAR ghanton tak fail ho raha? -> trade PENDING + alert, ops share daalta'],
  [15, YEL, 'AGAR', 'AGAR Portfolio DB ghanta bhar band, user dobara order kare? -> nahi'],
  [18, GRN, 'WAPAS', 'WAPAS: Portfolio DB wapas, system wahi trade_id se retry, SETTLED'],
];
function setup() {}
function sett(sub, st) { B(20, 110, 140, 120, 'Settlement', sub, { st }); }
function wal(sub, st) { B(520, 40, 200, 80, 'Wallet DB', sub, { st }); }
function por(sub, st) { B(520, 230, 200, 80, 'Portfolio DB', sub, { st }); }
function log(a, b) {
  T(85, 254, 'status log', { c: C.muted, size: 12 });
  cells(20, 262, [['wallet done', a], ['portfolio done', b]], { w: 130 });
}
function toW(c, n, s) { A(160, 140, 518, 85, c, { n, at: 0.2 }); L(330, 98, s, c); }
function fail(n, s) { A(160, 180, 518, 250, C.bad, { n, at: 0.2, dash: 1 }); cross(470, 240); L(400, 205, s, C.bad); }
function retry(c, n, s) { A(160, 215, 518, 290, c, { n, at: 0.25 }); L(420, 300, s, c); }
function frame(ph) {
  if (ph === 0) {
    sett('trade T1', 'bad'); wal('-20k', 'good'); por('share: nahi', 'bad');
    toW(C.good, 1, 'INFY 20k kata'); fail(2, 'fail');
    T(800, 182, 'paisa gaya,', { c: C.bad, b: 1, size: 14 }); T(800, 202, 'share nahi', { c: C.bad, b: 1, size: 14 });
  } else if (ph === 1) {
    sett('SETTLED', 'good'); wal('-20k', 'good'); por('+ INFY', 'good');
    toW(C.good, 1, 'wallet done'); fail(2, 'fail'); retry(C.good, 3, 'likhe haal se retry');
    log(C.good, C.good);
  } else if (ph === 2) {
    sett('SETTLED', 'good'); wal('', 'dim'); por('T1 ho chuka', 'good');
    retry(C.acc, 1, 'T1 phir chala');
    num(745, 270, 2, C.good); L(765, 275, 'no-op', C.good, 'start');
    L(620, 340, 'share do baar nahi', C.good);
    log(C.good, C.good);
  } else if (ph === 3) {
    B(20, 110, 140, 120, 'Order', 'match nahi', { st: 'bad' });
    num(30, 95, 1, C.bad); L(50, 100, 'match se pehle atka', C.bad, 'start');
    wal('blocked: 0', 'good'); por('', 'dim');
    toW(C.acc, 2, 'ulta: block hatao');
    L(330, 300, 'hal: sirf yahan ulta (SAGA)', C.good);
  } else if (ph === 4) {
    sett('PENDING', 'acc'); wal('wallet done', 'good'); por('share daala', 'good');
    fail(1, 'retry: fail, fail, fail');
    B(300, 275, 130, 55, 'Ops', '', { st: 'acc' });
    A(90, 230, 298, 300, C.acc, { n: 2, at: 0.3 }); L(175, 300, 'alert', C.acc);
    A(430, 300, 518, 285, C.good, { n: 3, at: 0.5 }); L(480, 340, 'share daalta', C.good);
  } else if (ph === 5) {
    B(20, 15, 140, 55, 'Arpan', '');
    sett('purana + naya', 'bad'); wal('paisa do baar', 'bad'); por('10 INFY (5 nahi)', 'bad');
    num(745, 230, 1, C.bad); L(765, 235, 'ghanta band tha', C.bad, 'start');
    A(90, 70, 90, 108, C.bad, { n: 2 }); L(110, 95, 'naya order', C.bad, 'start');
    A(160, 140, 518, 85, C.bad, { n: 3, at: 0.2 }); A(160, 190, 518, 265, C.bad, { n: 3, at: 0.2 });
    L(330, 98, 'DB wapas: dono chale', C.bad);
    L(330, 335, 'hal: trade PENDING + alert, system retry, user kuch nahi karta', C.good);
  } else {
    B(20, 15, 140, 55, 'Arpan', 'kuch nahi kiya', { st: 'good' });
    sett('SETTLED', 'good'); wal('wallet done', 'good'); por('5 INFY', 'good');
    num(745, 230, 1, C.good); L(765, 235, 'wapas aaya', C.good, 'start');
    retry(C.good, 2, 'wahi T1 se retry');
    log(C.good, C.good);
  }
}
