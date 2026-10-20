// Card 7: 12 baje sale, ek hi line me OTP 50,000 offer ke peeche -> 10 min late, login fail. Ilaaj: high / med / low alag line + apne worker.
const DUR = 18, FIX = 8;
const RY = [70, 190, 310];
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  box(770, 30, 110, 310, 'Users');
  RY.forEach((y, i) => {
    wire(`M140 ${y} H620`, 'q' + i); wire(`M730 ${y} H770`, 'u' + i);
    box(620, y - 25, 110, 50, ['OTP worker', 'Order worker', 'Offer worker'][i], '', { id: 'w' + i, stroke: i ? C.line : C.good });
  });
  box(620, 45, 110, 50, 'Worker', '', { id: 'w' });
}
function frame(t, fix, tl) {
  [0, 1, 2].forEach(i => { vis('w' + i, fix); vis('q' + i, fix || i === 0); vis('u' + i, fix || i === 0); });
  vis('w', !fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: 12 baje sale. Ek hi line me OTP 50,000 offer ke peeche';
    cells(140, 57, [['OTP', C.bad]].concat(Array(7).fill(['offer', C.acc])), { w: 52 });
    T(365, 40, '50,000 offer OTP se aage', { c: C.acc, b: 1, size: 12 });
    for (let k = 0; k < 14; k++) dot(mv([730, 70], [758, 70], 0.3 + k * 0.5, 0.7 + k * 0.5, tl), C.acc);
    T(365, 130, `offer baaki: ${(50000 - Math.round(tl * 100) * 10).toLocaleString('en-IN')}`, { c: C.acc, size: 13 });
    T(365, 160, `OTP ka intezaar: ${Math.min(10, Math.round(tl * 1.6))} min`, { c: C.bad, b: 1, size: 14 });
    if (tl > 6.2) T(365, 190, 'OTP time khatam -> login FAIL', { c: C.bad, b: 1, size: 14 });
    return;
  }
  CAP.textContent = 'ILAAJ: OTP ki apni tez line + apne worker. High = OTP, beech = order, low = offer';
  T(365, 35, 'high line', { c: C.good, size: 12, b: 1 });
  T(365, 155, 'beech wali line', { c: C.muted, size: 12 });
  T(365, 275, 'low line: offer ruk sakta', { c: C.muted, size: 12 });
  if (tl < 0.6) cells(140, 57, [['OTP', C.good]], { w: 52 });
  dot(mv([192, 70], [620, 70], 0.6, 1.1, tl), C.good, '', 'OTP', C.good);
  dot(mv([730, 70], [758, 70], 1.2, 1.5, tl), C.good);
  if (tl > 1.5) T(365, 105, 'OTP turant pahuncha, login OK', { c: C.good, b: 1, size: 14 });
  cells(140, 177, Array(3).fill(['order', C.msg]), { w: 52 });
  cells(140, 297, Array(8).fill(['offer', C.acc]), { w: 52 });
  for (let k = 0; k < 9; k++) {
    dot(mv([730, 190], [758, 190], 0.5 + k, 0.9 + k, tl), C.msg);
    dot(mv([730, 310], [758, 310], 0.8 + k, 1.2 + k, tl), C.acc);
  }
}
