// Card 4: koshish se pehle hi SENT laga di, call fail hui, Worker-B ne SENT dekh ke chhoda, OTP gaya hi nahi.
// Ilaaj: pehle SENDING (60 sec umar), company OK ke baad SENT. Agar: SENDING wala worker mara -> key khud mit-ti.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: bhejne se pehle hi key SENT laga di. Call fail hui, OTP kabhi nahi gaya. Pal-pal'],
  [3, GRN, 'ILAAJ', 'ILAAJ: pehle SENDING (umar 60 sec), company OK bole tab SENT. Chhodna sirf SENT pe'],
  [6, YEL, 'AGAR', 'AGAR SENDING laga ke worker hi mar gaya? -> 60 sec me key khud mit-ti, agla bhejta'],
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
  [[C.bad, 'Worker-A → Redis', 'bhejne se pehle hi k5 = SENT'], [C.bad, 'Worker-A → SMS company', 'call fail, OTP nahi gaya'], [C.acc, 'Redis → Worker-B', '"bheja ja chuka"'], [C.bad, 'Worker-B → Arpan', 'kuch nahi bheja, login atka']],
  [[C.acc, 'Worker-A → Redis', 'k5 = SENDING (umar 60 sec)'], [C.good, 'Worker-A → SMS company', 'bheja, company ne OK bola'], [C.good, 'Worker-A → Redis', 'ab k5 = SENT'], [C.good, 'Redis → Worker-B', 'SENT dekha: sahi skip']],
  [[C.acc, 'Worker-A → Redis', 'k5 = SENDING'], [C.bad, 'Worker-A mara', 'SENT likhne wala koi nahi'], [C.good, 'Redis', '60 sec: key khud mit gayi'], [C.good, 'Worker-B → Arpan', 'OTP bheja, khoya nahi']],
];
function frame(ph) {
  if (ph === 0) {
    panel(0, 'pehle hi SENT', ['Worker-A', 'OTP k5', ''], ['Redis', 'k5 = SENT', 'bad'], 'likha', true, C.bad, 'abhi bheja hi nahi');
    panel(1, 'call fail', ['Worker-A', '', ''], ['SMS company', 'error', 'bad'], 'bhejo', true, C.bad, 'OTP nahi gaya');
    panel(2, 'Worker-B', ['Worker-B', 'k5 phir', ''], ['Redis', 'k5 = SENT', 'bad'], 'dekha', false, C.acc, '"bheja ja chuka"');
    panel(3, 'chhod diya', ['Worker-B', 'kuch nahi bheja', 'bad'], ['Arpan', 'OTP aaya hi nahi', 'bad'], '', true, C.bad, 'login atka');
    A(735, 110, 735, 158, C.bad, { dash: 1 }); cross(735, 134); T(750, 139, 'OTP', { c: C.bad, b: 1, size: 14, a: 'start' });
    A(735, 110, 735, 158, C.bad, { dash: 1 }); cross(735, 134); T(750, 139, 'OTP', { c: C.bad, b: 1, size: 14, a: 'start' });
    L(450, 320, 'SENT jhooth tha. Koshish adhoori thi, par sabne poori maan li', C.bad);
  } else if (ph === 1) {
    panel(0, 'SENDING', ['Worker-A', 'OTP k5', ''], ['Redis', 'k5 = SENDING', 'acc'], 'likha', true, C.acc, 'umar 60 sec');
    panel(1, 'bheja', ['Worker-A', '', ''], ['SMS company', 'OK', 'good'], 'bhejo', true, C.good, '~2-3 sec');
    panel(2, 'ab SENT', ['Worker-A', '', ''], ['Redis', 'k5 = SENT', 'good'], 'likha', true, C.good, 'company ne OK bola');
    panel(3, 'Worker-B', ['Worker-B', 'k5 phir', ''], ['Redis', 'k5 = SENT', 'good'], 'dekha', false, C.good, 'sahi skip: OTP gaya tha');
    L(450, 320, 'call fail hoti to Worker-A khud key hata deta, agla phir bhejta', C.good);
  } else {
    panel(0, 'SENDING', ['Worker-A', 'OTP k5', ''], ['Redis', 'k5 = SENDING', 'acc'], 'likha', true, C.acc, 'umar 60 sec');
    panel(1, 'worker mara', ['Worker-A', 'mara', 'bad'], ['Redis', 'k5 = SENDING', 'acc'], '', true, C.bad, 'SENT likhne wala koi nahi');
    A(295, 110, 295, 158, C.bad, { dash: 1 }); cross(295, 134); T(310, 139, 'SENT', { c: C.bad, b: 1, size: 14, a: 'start' });
    A(295, 110, 295, 158, C.bad, { dash: 1 }); cross(295, 134); T(310, 139, 'SENT', { c: C.bad, b: 1, size: 14, a: 'start' });
    panel(2, '60 sec baad', ['Redis', 'k5 = SENDING', 'acc'], ['Redis', 'k5 mit gayi', 'good'], 'umar khatam', true, C.muted, 'apne aap');
    panel(3, 'Worker-B', ['Worker-B', 'k5 phir', ''], ['Arpan', 'OTP aaya', 'good'], 'bheja', true, C.good, 'khoya nahi');
    L(450, 320, 'kabhi do baar bhi chala jaaye to chalega: OTP do baar aana, na aane se behtar', C.good);
  }
}
