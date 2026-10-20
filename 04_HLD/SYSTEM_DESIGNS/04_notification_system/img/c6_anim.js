// Card 6: Twilio dheemi, har call 30 sec, saare SMS worker phanse, OTP ruka. Ilaaj: 2-3 sec timeout + circuit breaker -> OTP SNS se, baad me test call.
const DUR = 18, FIX = 8;
const WY = [80, 190, 300];
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  WY.forEach((y, i) => {
    wire(`M166 190 L210 ${y}`);
    wire(`M300 ${y} L680 75`, 'd' + i); wire(`M300 ${y} L400 190`, 'f' + i);
    box(210, y - 22, 90, 44, `W${i + 1}`);
  });
  wire('M520 190 L680 75', 'f3'); wire('M520 190 L680 305', 'f4');
  box(400, 155, 120, 70, 'Circuit', 'breaker', { id: 'cb', stroke: C.acc });
  box(680, 40, 180, 70, 'Twilio', 'dheemi', { stroke: C.bad });
  box(680, 270, 180, 70, 'SNS', 'doosri company', { id: 'sns', stroke: C.acc });
}
function frame(t, fix, tl) {
  [0, 1, 2].forEach(i => { vis('d' + i, !fix); vis('f' + i, fix); });
  ['f3', 'f4', 'cb', 'sns'].forEach(i => vis(i, fix));
  if (!fix) {
    CAP.textContent = 'DIKKAT: Twilio dheemi. Har call 30 sec intezaar, saare SMS worker phanse, OTP bhi ruka';
    cells(20, 177, [['SMS', C.msg], ['SMS', C.msg], ['OTP', C.bad]], { w: 46 });
    WY.forEach((y, i) => {
      const s = 0.4 + i * 0.3;
      dot(mv([300, y], [668, 62 + i * 13], s, s + 0.8, tl), C.msg);
      if (tl > s + 0.8) { dot([668, 62 + i * 13], C.msg);
        T(255, y + 38, `phansa ${Math.min(30, Math.round((tl - s - 0.8) * 5))} sec`, { c: C.bad, b: 1, size: 12 }); }
    });
    T(770, 135, 'har call 30 sec', { c: C.bad, b: 1, size: 13 });
    if (tl > 3) T(93, 225, 'OTP line me ruka', { c: C.bad, b: 1, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: 2-3 sec me chhodo (timeout). Baar-baar fail -> circuit breaker khula, OTP SNS se';
  let fails = 0;
  WY.forEach((y, i) => {
    const s = 0.3 + i * 0.4;
    dot(mv([300, y], [400, 190], s, s + 0.5, tl), C.msg);
    dot(mv([520, 190], [668, 75], s + 0.5, s + 1.0, tl), C.msg);
    if (tl > s + 1.0 && tl < s + 1.6) dot([668, 75], C.acc, '', '2 sec', C.acc);
    if (tl >= s + 1.6) fails++;
  });
  const open = tl >= 3.4 && tl < 7.6;
  T(460, 140, `fail: ${fails}`, { c: fails ? C.bad : C.fg, b: 1, size: 13 });
  if (open) { cross(600, 132); T(460, 248, 'KHULA: Twilio band', { c: C.bad, b: 1, size: 13 }); }
  cells(20, 177, tl < 3.9 ? [['SMS', C.msg], ['SMS', C.msg], ['OTP', C.good]] : [['SMS', C.msg], ['SMS', C.msg]], { w: 46 });
  dot(mv([166, 190], [300, 190], 3.9, 4.3, tl), C.good, '', 'OTP', C.good);
  dot(mv([300, 190], [400, 190], 4.3, 4.6, tl), C.good, '', 'OTP', C.good);
  dot(mv([520, 190], [668, 300], 4.6, 5.3, tl), C.good, '', 'OTP', C.good);
  if (tl > 5.3) T(660, 360 - 20, 'OTP gaya (SNS)', { c: C.good, b: 1, size: 13, a: 'end' });
  dot(mv([520, 190], [668, 75], 6.4, 7.2, tl), C.acc, '', 'test call', C.acc);
  if (tl >= 7.6) T(460, 248, 'Twilio theek -> wapas', { c: C.good, b: 1, size: 13 });
}
