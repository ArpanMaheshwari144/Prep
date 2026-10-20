// Card 6: SMS company (Twilio) band nahi, dheemi. Har call 30 sec, saare SMS worker phanse, OTP bhi ruka.
// Ilaaj: timeout 2-3 sec + circuit breaker, OTP doosri company (SNS) se. Agar: Twilio band, push / email pe asar? nahi.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Twilio band nahi, dheema. Har call 30 sec, saare SMS worker wahin phanse'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 2-3 sec me chhodo (timeout). Baar-baar fail: circuit breaker khula, SNS se bhejo'],
  [6, YEL, 'AGAR', 'AGAR Twilio ka breaker khula, push / email pe asar? -> nahi, har company ka apna breaker'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
const STEPS = [
  [[C.acc, 'OTP line → SMS worker', 'Arpan ka OTP'], [C.bad, 'SMS worker x3 → Twilio', '30 sec wait. Koi worker khaali nahi']],
  [[C.acc, 'SMS worker → Twilio', 'call, 3 sec baad chhoda'], [C.bad, 'Breaker → Twilio', '5 fail: KHULA, call roki'], [C.good, 'SMS worker → SNS', 'doosri company se OTP'], [C.good, 'Breaker → Twilio', 'thodi der baad ek test call']],
  [[C.good, 'SMS / push / email line', 'har company ka apna breaker.|Sirf SMS ruka, push + email chalu']],
];
function frame(ph) {
  if (ph === 0) {
    ['SMS worker 1', 'SMS worker 2', 'SMS worker 3'].forEach((w, i) => {
      B(240, 30 + i * 100, 170, 60, w, '30 sec wait...', { st: 'bad' });
      A(410, 60 + i * 100, 638, 165, C.bad, { dash: 1 });
    });
    B(640, 130, 200, 70, 'Twilio', 'dheemi: 30 sec', { st: 'bad' });
    B(20, 130, 150, 70, 'OTP line', 'Arpan ka OTP', { st: 'acc' });
    A(170, 165, 238, 165, C.acc, { n: 1 }); num(325, 345, 2, C.bad); L(345, 350, 'koi worker khaali nahi', C.bad, 'start');
    L(740, 240, 'band hoti to pata chalta,', C.bad); L(740, 260, 'dheemi me sab phanse', C.bad);
  } else if (ph === 1) {
    panel(0, 'timeout', ['SMS worker', '', ''], ['Twilio', '3 sec, jawab nahi', 'bad'], 'call', true, C.acc, '3 sec baad chhoda');
    panel(1, 'breaker khula', ['Breaker', 'KHULA', 'bad'], ['Twilio', '5 me 5 fail', 'bad'], '', true, C.bad, 'ab Twilio ko call nahi');
    A(305, 100, 305, 163, C.bad, { dash: 1 }); cross(305, 131); T(375, 135, 'call roki', { c: C.bad, b: 1, size: 12 });
    panel(2, 'doosri company', ['SMS worker', 'Arpan ka OTP', ''], ['SNS', 'OTP gaya', 'good'], 'bheja', true, C.good, 'OTP ruka nahi');
    panel(3, 'test call', ['Breaker', 'thodi der baad', 'acc'], ['Twilio', 'theek? haan', 'good'], 'ek call', true, C.good, 'theek to wapas Twilio');
    L(450, 320, 'dheema matlab band maano. Khabar line me rukti, phenki nahi', C.good);
  } else {
    [['SMS', 'Twilio: breaker KHULA', 'bad'], ['push', 'FCM: chalu', 'good'], ['email', 'SES: chalu', 'good']].forEach(([r, s, st], i) => {
      const y = 30 + i * 105;
      B(20, y, 140, 65, r + ' line', '');
      B(260, y, 170, 65, r + ' worker', '', { st: st === 'bad' ? 'acc' : 'good' });
      B(560, y, 250, 65, s.split(':')[0], s.split(':')[1], { st });
      A(160, y + 32, 258, y + 32, C.msg, { n: i ? 0 : 1 });
      A(430, y + 32, 558, y + 32, st === 'bad' ? C.bad : C.good, { dash: st === 'bad' ? 1 : 0 });
    });
    L(450, 350, 'Twilio ka breaker sirf SMS rokta. Push aur email apni line, apni company', C.good);
  }
}
