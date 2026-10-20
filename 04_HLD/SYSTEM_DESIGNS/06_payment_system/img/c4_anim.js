// Card 4: bank se 2000 top-up, PSP dheema (30 sec), har thread wahin atka. Ilaaj: 2-3 sec timeout = PENDING,
// 5 fail = circuit OPEN, saaf "abhi nahi". Agar: OPEN kab khulega -> 30 sec baad ek test call.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: bank se 2000 top-up. PSP dheema (30 sec), har thread uske jawab pe atka'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 2-3 sec me jawab nahi = PENDING. 5 fail = circuit OPEN, saaf "abhi nahi"'],
  [6, YEL, 'AGAR', 'AGAR circuit OPEN, phir kab khulega? -> 30 sec baad ek test call'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 30, title, { c: C.fg, b: 1, size: 13 });
  B(x + 40, 50, 125, 48, top[0], top[1], { st: top[2] });
  B(x + 40, 165, 125, 58, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 70, 100, x + 70, 163, c); else A(x + 70, 163, x + 70, 100, c); } // khaali lab = teer nahi
  T(x + 140, 135, lab, { c, b: 1, size: 12 });
  T(x + 102, 252, note, { c: C.muted, size: 11 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 150, 70, 'Arpan', 'top-up 2000');
    B(260, 40, 230, 250, 'Payment Svc', '', { st: 'bad' });
    cells(280, 90, [['thread 1: wait...', C.bad]], { w: 190, h: 30 });
    cells(280, 130, [['thread 2: wait...', C.bad]], { w: 190, h: 30 });
    cells(280, 170, [['thread 3: wait...', C.bad]], { w: 190, h: 30 });
    cells(280, 210, [['... sab: wait', C.bad]], { w: 190, h: 30 });
    B(680, 130, 190, 70, 'PSP (bank)', 'dheema: 30 sec', { st: 'bad' });
    A(170, 165, 258, 165, C.msg, { n: 1 });
    A(490, 165, 678, 165, C.bad, { n: 2, dash: 1 }); L(585, 150, 'call, jawab ka wait', C.bad);
    num(775, 235, 3, C.bad); L(775, 260, 'koi thread khaali nahi', C.bad);
    L(450, 330, 'ek dheema PSP, aur poora system band', C.bad);
  } else if (ph === 1) {
    panel(0, 'call', ['Payment Svc', 'top-up 2000', ''], ['PSP', '3 sec, jawab nahi', 'bad'], 'call', true, C.acc, '3 sec baad chhoda');
    panel(1, 'PENDING', ['Payment Svc', 'pata nahi kata?', 'acc'], ['Payment DB', 'top-up: PENDING', 'acc'], 'likha', true, C.acc, '"fail" nahi, PENDING');
    panel(2, '5 fail', ['Breaker', 'OPEN', 'bad'], ['PSP', '5 me 5 fail', 'bad'], '', true, C.bad, 'call jaati hi nahi');
    A(525, 100, 525, 163, C.bad, { dash: 1 }); cross(525, 131); T(595, 135, 'call roki', { c: C.bad, b: 1, size: 12 });
    panel(3, 'saaf jawab', ['Arpan', '"abhi nahi"', 'good'], ['Payment Svc', '', ''], 'turant', false, C.good, 'paisa hila hi nahi');
    L(450, 320, 'thread jaldi chhoot jaate, baaki payment chalte rehte', C.good);
  } else {
    B(20, 130, 170, 80, 'Breaker', 'OPEN, 30 sec', { st: 'acc' });
    B(330, 130, 200, 80, 'Test call', 'sirf ek, PSP ko', { st: 'acc' });
    B(680, 40, 190, 80, 'PSP ne jawab diya', 'breaker band', { st: 'good' });
    B(680, 230, 190, 80, 'phir fail', 'phir OPEN', { st: 'bad' });
    A(190, 170, 328, 170, C.acc, { n: 1 }); L(260, 155, '30 sec baad', C.acc);
    A(530, 150, 678, 85, C.good, { n: 2 }); L(590, 95, 'chali', C.good);
    A(530, 190, 678, 265, C.bad); L(590, 255, 'ya nahi chali', C.bad);
    L(450, 340, 'chali to sab call wapas PSP ko. Nahi chali to 30 sec aur ruko', C.good);
  }
}
