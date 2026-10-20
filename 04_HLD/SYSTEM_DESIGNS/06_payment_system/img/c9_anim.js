// Card 9: mahine bhar ki report primary pe, primary busy, payment ruka. Ilaaj: report read replica pe, primary sirf payment.
const DUR = 17, FIX = 7.5;
const AR = [160, 75], PR = [376, 75], ME = [180, 285], PRL = [388, 112], RPL = [388, 290];
function setup() {
  wire('M160 75 H400'); wire('M180 285 L400 110', 'mr'); wire('M180 290 H400', 'mg'); wire('M490 120 V250', 'wr');
  box(20, 40, 140, 70, 'Arpan', 'payment');
  box(20, 250, 160, 70, 'Merchant', 'mahine bhar report');
  box(400, 40, 180, 80, 'Primary DB', 'payment');
  box(400, 250, 180, 80, 'Read replica', '1-2 sec peeche', { id: 'rp' });
}
function frame(t, fix, tl) {
  vis('mr', !fix); ['mg', 'wr', 'rp'].forEach(id => vis(id, fix));
  if (!fix) {
    CAP.textContent = 'DIKKAT: merchant ki mahine bhar ki report primary pe. Primary busy, mera payment ruka';
    if (tl < 1.3) dot(path([[0.3, ME], [1.3, PRL]], tl), C.acc, '', 'report', C.acc);
    bar(640, 70, 220, 18, Math.min(1, 0.1 + Math.max(0, tl - 1.3) * 0.75), 'primary busy');
    if (tl > 2.8) dot(path([[2.8, AR], [3.5, PR]], tl), tl > 3.5 ? C.bad : C.msg, '', tl > 3.5 ? 'ruka' : 'pay', tl > 3.5 ? C.bad : C.msg);
    if (tl > 3.8) T(640, 130, `payment ruka: ${Math.floor(tl - 3.5)} sec`, { a: 'start', c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: report copy pe (read replica, 1-2 sec peeche). Primary sirf payment, payment tez';
  if (tl < 1.3) dot(path([[0.3, ME], [1.3, RPL]], tl), C.acc, '', 'report', C.acc);
  bar(640, 70, 220, 18, 0.15, 'primary busy');
  bar(640, 280, 220, 18, Math.min(0.8, Math.max(0, tl - 1.3) * 0.6), 'replica busy');
  for (let k = 0; k < 8; k++) { const s = 0.2 + k; if (tl > s && tl < s + 0.6) dot(path([[s, [490, 120]], [s + 0.6, [490, 250]]], tl), C.line); }
  T(500, 190, 'copy, 1-2 sec peeche', { a: 'start', c: C.muted, size: 11 });
  if (tl > 2.8 && tl < 3.4) dot(path([[2.8, AR], [3.4, PR]], tl), C.msg, '', 'pay');
  if (tl > 3.6 && tl < 4.2) dot(path([[3.6, PR], [4.2, [172, 75]]], tl), C.good, '', 'hua', C.good);
  if (tl >= 4.2) T(640, 130, 'payment tez: hua', { a: 'start', c: C.good, b: 1, size: 13 });
}
