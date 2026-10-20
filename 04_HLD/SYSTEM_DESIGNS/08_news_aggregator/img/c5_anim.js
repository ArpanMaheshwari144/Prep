// Card 5: chhota source 30 sec atka, baaki 999 ruke. Ilaaj: timeout + circuit breaker. Agar: HALF-OPEN test call.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek chhota source 30 sec atka. Fetcher uske jawab pe ruka, baaki 999 bhi ruke'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 2-3 sec me chhodo (timeout). Baar-baar fail to call hi band (breaker OPEN)'],
  [6, YEL, 'AGAR', 'AGAR OPEN hai, wapas kab chalu? -> thodi der baad HALF-OPEN: ek test call'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'Fetcher → chhota blog', 'call gayi, 30 sec jawab nahi'], [C.bad, 'baaki 999 source', 'line me ruke'], [C.bad, 'Feed', 'nayi khabar aani band']],
  [[C.acc, 'Fetcher → chhota blog', 'call, 3 sec baad chhoda'], [C.good, 'Fetcher → NDTV', 'agle source pe aage'], [C.bad, 'Breaker → chhota blog', '5 baar fail: OPEN, call roki'], [C.good, 'Worker → Redis', 'baaki 999 ki khabar feed me']],
  [[C.acc, 'Breaker → Test call', 'thodi der baad sirf ek call|(HALF-OPEN)'], [C.good, 'Test call → CLOSED', 'chali: breaker band, sab call chalu'], [C.bad, 'ya fail → OPEN', 'phir OPEN, thodi der aur ruko']],
];
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
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 170, 80, 'Fetcher', 'jawab ka wait', { st: 'bad' });
    B(330, 20, 230, 70, 'Chhota blog', '30 sec atka', { st: 'bad' });
    B(330, 150, 300, 90, 'Baaki 999', '', { st: 'acc' });
    cells(345, 195, [['NDTV', C.acc], ['ABP', C.acc], ['...', C.line], ['BBC', C.acc]], { w: 62, h: 30 });
    B(720, 260, 150, 70, 'Feed', 'purani', { st: 'bad' });
    A(190, 150, 328, 60, C.bad, { n: 1, dash: 1 });
    num(660, 195, 2, C.bad); L(680, 200, 'ruke', C.bad, 'start');
    A(630, 230, 718, 285, C.bad, { n: 3, dash: 1 });
    L(450, 345, 'ek beemar source ne poori feed rok di', C.bad);
  } else if (ph === 1) {
    panel(0, 'timeout', ['Fetcher', '', ''], ['Chhota blog', '3 sec chup', 'bad'], 'call', true, C.acc, '3 sec baad chhoda');
    panel(1, 'aage', ['Fetcher', '', ''], ['NDTV', 'a91 mili', 'good'], 'agla', true, C.good, 'line nahi ruki');
    panel(2, 'breaker', ['Breaker', 'OPEN', 'bad'], ['Chhota blog', '5 me 5 fail', 'bad'], '', true, C.bad, 'call jaati hi nahi');
    A(525, 110, 525, 158, C.bad, { dash: 1 }); cross(525, 134); T(595, 139, 'call roki', { c: C.bad, b: 1, size: 14 });
    panel(3, 'feed chalu', ['Worker', '999 ki khabar', 'good'], ['Redis', 'feed taazi', 'good'], 'ZADD', true, C.good, 'sirf blog ki der');
    L(450, 320, 'ek source ki khabar der se, baaki feed chalti rahi', C.good);
  } else {
    B(20, 130, 170, 80, 'Breaker', 'OPEN', { st: 'acc' });
    B(320, 130, 220, 80, 'Test call', 'sirf ek (HALF-OPEN)', { st: 'acc' });
    B(680, 30, 190, 80, 'CLOSED', 'sab call chalu', { st: 'good' });
    B(680, 230, 190, 80, 'OPEN', 'thodi der aur', { st: 'bad' });
    A(190, 170, 318, 170, C.acc, { n: 1 }); L(255, 155, 'kuch der baad', C.acc);
    A(540, 150, 678, 75, C.good, { n: 2 }); L(590, 95, 'chali', C.good, 'end');
    A(540, 190, 678, 265, C.bad, { n: 3 }); L(590, 255, 'fail', C.bad, 'end');
    L(450, 345, 'ek call se jaanch: theek hua to sab wapas', C.good);
  }
}
