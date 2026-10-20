// Card 4: koshish se pehle hi SENT, call fail, Worker-B ne SENT dekh ke chhoda -> OTP gaya hi nahi. Ilaaj: SENDING (60 sec umar), OK pe SENT.
const DUR = 18, FIX = 8;
const K = [130, 190], A = [270, 85], B = [270, 275], AR = [410, 85], BR = [410, 275];
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  box(20, 150, 110, 80, 'Kafka', 'OTP khabar');
  wire('M130 190 L270 85'); wire('M130 190 L270 275');
  box(270, 50, 140, 70, 'Worker-A'); box(270, 240, 140, 70, 'Worker-B');
  wire('M410 85 L700 170'); wire('M410 275 L700 200'); wire('M410 85 L500 150'); wire('M410 275 L500 220');
  box(500, 150, 140, 70, 'Redis', 'OTP key', { stroke: C.acc });
  box(700, 150, 160, 80, 'SMS company');
}
function key(s, c) { T(570, 245, `key: ${s}`, { c, b: 1, size: 13, halo: 1 }); }
function frame(t, fix, tl) {
  dot(mv(K, A, 0.3, 0.9, tl), C.msg);
  if (!fix) {
    CAP.textContent = 'DIKKAT: call se pehle hi SENT laga di. Call fail, Worker-B ne SENT dekh ke chhod diya';
    dot(mv(AR, [488, 165], 1.0, 1.5, tl), C.acc, '', 'SENT', C.acc);
    if (tl > 1.5) key('SENT', C.bad);
    dot(mv(AR, [688, 175], 1.8, 2.6, tl), C.msg);
    if (tl > 2.6) T(780, 260, 'call fail', { c: C.bad, b: 1, size: 13 });
    dot(mv(K, B, 3.2, 3.8, tl), C.msg);
    dot(mv(BR, [488, 205], 4.0, 4.5, tl), C.acc, '', 'key?', C.acc);
    if (tl > 4.6) T(340, 335, 'SENT mili -> chhoda', { c: C.bad, b: 1, size: 13 });
    T(780, 300, tl > 4.6 ? 'OTP: 0 (kho gaya)' : 'OTP: 0', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: bhejte waqt SENDING (umar 60 sec), company OK bole tab SENT. Chhodna sirf SENT pe';
  let otp = 0;
  dot(mv(AR, [488, 165], 1.0, 1.5, tl), C.acc, '', 'SENDING', C.acc);
  dot(mv(AR, [688, 175], 1.8, 2.6, tl), C.msg);
  if (tl > 2.6 && tl < 5) T(780, 260, 'call fail', { c: C.bad, b: 1, size: 13 });
  if (tl > 2.6 && tl < 4.2) T(340, 38, 'Worker-A mara', { c: C.bad, b: 1, size: 12 });
  if (tl > 1.5 && tl < 4.2) key(`SENDING (umar ${Math.max(0, Math.round(60 - (tl - 1.5) * 23))})`, C.acc);
  if (tl >= 4.2 && tl < 5.6) key('khud mit gayi', C.good);
  dot(mv(K, B, 4.6, 5.2, tl), C.msg);
  dot(mv(BR, [488, 205], 5.4, 5.8, tl), C.acc, '', 'SENDING', C.acc);
  if (tl >= 5.8 && tl < 7.4) key('SENDING', C.acc);
  dot(mv(BR, [688, 195], 6.0, 6.8, tl), C.good);
  if (tl > 6.8) { otp = 1; T(780, 260, 'OK', { c: C.good, b: 1, size: 13 }); }
  if (tl >= 7.4) key('SENT', C.good);
  T(780, 300, `OTP: ${otp}`, { c: otp ? C.good : C.fg, b: 1, size: 15 });
}
