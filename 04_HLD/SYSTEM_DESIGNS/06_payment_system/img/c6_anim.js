// Card 6: wallet -500 hua, Bank B payout fail, ek transaction nahi -> 500 latka. Ilaaj: SAGA, fail pe ulta (500 wapas).
const DUR = 17, FIX = 7.5;
const SV = [430, 65], WL = [140, 182], BK = [750, 182], SG = [430, 100], SGT = [430, 178];
function setup() {
  wire('M330 65 L125 190'); wire('M530 65 L765 190'); wire('M430 100 V190', 'wsg');
  box(330, 30, 200, 70, 'Payment Svc', '+ SAGA');
  box(40, 190, 170, 70, 'Arpan wallet', 'hamara DB');
  box(680, 190, 170, 70, 'Bank B', 'merchant payout');
  box(330, 190, 200, 125, 'saga table', '', { id: 'sg' });
}
function frame(t, fix, tl) {
  vis('sg', fix); vis('wsg', fix);
  const L = [330, 65], R = [530, 65];
  if (tl < 1.0) dot(path([[0.3, L], [1.0, WL]], tl), C.acc, '', '-500', C.acc);
  if (tl > 1.4 && tl < 2.1) dot(path([[1.4, R], [2.1, BK]], tl), C.msg, '', '+500 payout');
  if (tl >= 2.1) { cross(872, 225); T(765, 285, 'payout fail', { c: C.bad, b: 1, size: 13 }); }
  if (!fix) {
    CAP.textContent = 'DIKKAT: wallet hamare DB me, payout Bank B me. Ek transaction nahi, payout fail = 500 latka';
    if (tl >= 1.0) T(125, 285, '-500 kata', { c: C.acc, b: 1, size: 13 });
    if (tl >= 3.0) { T(430, 240, '500 beech me latka', { c: C.bad, b: 1, size: 15 });
                     T(430, 265, 'na wallet me, na merchant ko', { c: C.bad, size: 12 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: SAGA. Har kadam saga table me. Payout fail -> ulta kaam: wallet me 500 wapas';
  const rows = [];
  if (tl >= 1.0) rows.push(['1. DEBITED (-500)', C.acc]);
  if (tl >= 2.1) rows.push(['2. PAYOUT: fail', C.bad]);
  if (tl > 2.7 && tl < 3.4) dot(path([[2.7, L], [3.4, WL]], tl), C.good, '', '+500 wapas', C.good);
  if (tl >= 3.4) rows.push(['3. ulta: +500 wapas', C.good]);
  rows.forEach(([s, c], i) => T(345, 240 + i * 26, s, { a: 'start', c, b: 1, size: 13 }));
  T(125, 285, tl >= 3.4 ? '500 wapas' : tl >= 1.0 ? '-500 kata' : '', { c: tl >= 3.4 ? C.good : C.acc, b: 1, size: 13 });
  if (tl >= 4.0) T(450, 340, 'User: PROCESSING -> paisa wapas. Hisaab saaf', { c: C.good, b: 1, size: 13 });
}
