// Card 3: Suresh ke paas 10, mujhe aur Mohan dono ko 10 mil gaye (do thread ek book pe). Ilaaj: ek symbol ek thread,
// book RAM me. Agar: mujhe 10 chahiye, uske paas 6 -> 6 ka sauda, baaki 4 book me.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Suresh 10 TCS bech raha, main aur Mohan dono 10 maang rahe. Do thread, ek book'],
  [3, GRN, 'ILAAJ', 'ILAAJ: TCS ke saare order ek line me, us line ka ek hi thread. Book RAM me'],
  [6, YEL, 'AGAR', 'AGAR mujhe 10 chahiye, uske paas 6? -> 6 ka sauda, baaki 4 book me, unka paisa ruka'],
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
  [[C.acc, 'TCS book → Thread-1', 'Thread-1 ne Arpan ke liye book dekhi, 10 share hain'], [C.acc, 'TCS book → Thread-2', 'Usi pal Thread-2 ne Mohan ke liye dekha, abhi bhi 10'], [C.bad, 'TCS book → Thread-1', 'Thread-1 ne 10 share Arpan ko de diye, 0 bache'], [C.bad, 'TCS book → Thread-2', 'Thread-2 ne bhi 10 Mohan ko de diye, book -10 pe']],
  [[C.msg, 'Arpan → TCS line', 'Arpan ne 9:30:01 pe 10 share ka BUY daala'], [C.msg, 'Mohan → TCS line', 'Mohan ne ek second baad 10 ka BUY daala'], [C.acc, 'TCS line → Thread-1', 'TCS line ko sirf ek thread ek-ek karke chalata'], [C.good, 'Thread-1 → TCS book', 'Arpan ko 10 mile (FILLED), Mohan OPEN reh ke line me ruka']],
  [[C.msg, 'Arpan → TCS book', 'Arpan ne 3000 pe 10 share ka BUY daala'], [C.msg, 'Suresh → TCS book', 'Suresh sirf 6 share bech raha tha, 3000 pe'], [C.acc, 'TCS book → Arpan', '6 ka sauda hua (PARTIALLY FILLED), baaki 4 ka paisa ruka raha'], [C.good, 'Naya seller → TCS book', 'Naye seller ne 4 beche, baaki bhar gaye, ab FILLED']],
];
function frame(ph) {
  if (ph === 0) {
    panel(0, 'Thread-1 padhta', ['Thread-1', 'Arpan: 10', 'acc'], ['TCS book', 'Suresh: 10', ''], 'padha', false, C.acc, '10 hain');
    panel(1, 'Thread-2 padhta', ['Thread-2', 'Mohan: 10', 'acc'], ['TCS book', 'Suresh: 10', ''], 'padha', false, C.acc, 'abhi bhi 10 dikhe');
    panel(2, 'Thread-1 deta', ['Thread-1', 'Arpan ko 10', 'bad'], ['TCS book', 'Suresh: 0', 'bad'], '10 share', false, C.bad, 'mere 10');
    panel(3, 'Thread-2 deta', ['Thread-2', 'Mohan ko 10', 'bad'], ['TCS book', 'Suresh: -10', 'bad'], '10 share', false, C.bad, 'Mohan ke bhi 10');
    L(450, 320, '10 share the, 20 bik gaye. Suresh ke paas the hi nahi', C.bad);
  } else if (ph === 1) {
    B(20, 50, 130, 55, 'Arpan', 'BUY 10, 9:30:01');
    B(20, 230, 130, 55, 'Mohan', 'BUY 10, 9:30:02');
    B(220, 120, 220, 90, 'TCS ki line', '', { st: 'acc' });
    cells(235, 160, [['1: Arpan', C.good], ['2: Mohan', C.muted]], { w: 95, h: 30 });
    B(520, 130, 120, 70, 'Thread-1', 'sirf TCS', { st: 'good' });
    B(700, 120, 170, 90, 'TCS book (RAM)', 'Suresh: 10 -> 0', { st: 'good' });
    A(150, 78, 218, 150, C.msg, { n: 1 }); A(150, 258, 218, 185, C.msg, { n: 2 });
    A(440, 165, 518, 165, C.acc, { n: 3 }); A(640, 165, 698, 165, C.good, { n: 4 });
    L(450, 260, 'Arpan: FILLED 10. Mohan: OPEN, line me ruka', C.good);
    L(450, 330, 'ek stock ki book ko ek waqt ek hi haath. Race hoti hi nahi', C.good);
  } else {
    B(20, 60, 140, 60, 'Arpan', 'BUY 10 @3000');
    B(20, 240, 140, 60, 'Suresh', 'SELL 6 @3000');
    B(330, 130, 230, 90, 'TCS book', 'Arpan: 4 bache', { st: 'acc' });
    B(700, 60, 170, 60, 'Arpan', 'PARTIALLY FILLED', { st: 'acc' });
    B(700, 240, 170, 60, 'Naya seller', 'SELL 4 @3000', { st: 'good' });
    A(160, 90, 328, 155, C.msg, { n: 1 }); A(160, 270, 328, 195, C.msg, { n: 2 });
    A(560, 150, 698, 90, C.acc, { n: 3 }); L(625, 105, '6 ka sauda', C.acc);
    A(698, 270, 562, 200, C.good, { n: 4 }); L(655, 212, 'baaki 4 bhare', C.good);
    L(450, 345, 'bache 4 ka paisa blocked raha. Naya seller aaya to FILLED', C.good);
  }
}
