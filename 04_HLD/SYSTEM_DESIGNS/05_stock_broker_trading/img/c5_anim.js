// Card 5: matching server gira, RAM ki order book gayab. Ilaaj: har order pehle log (disk) me number ke saath, phir book.
// Gira = log se replay. Agar: replay me der -> standby pehle se log padhta, book taiyaar.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 10:15 pe matching server gira. Book RAM me thi, 4,000 order gayab'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har order pehle log (disk) me, number ke saath, phir book me. Gira = log se replay'],
  [6, YEL, 'AGAR', 'AGAR replay me der lagi? -> standby pehle se log padhta, book taiyaar'],
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
    B(20, 140, 140, 60, 'Orders', '4,000 khule');
    B(300, 120, 260, 100, 'Matching server', 'book sirf RAM me', { st: 'bad' }); cross(585, 170);
    A(160, 170, 298, 170, C.msg, { n: 1 });
    num(430, 250, 2, C.bad); L(430, 275, 'server gira, RAM saaf', C.bad);
    B(660, 130, 200, 80, 'Order book', '0 order: gayab', { st: 'bad' }); num(760, 240, 3, C.bad); L(760, 265, 'kisne kya maanga, pata nahi', C.bad);
    L(450, 335, 'users ke order the, ab kahin likhe hi nahi', C.bad);
  } else if (ph === 1) {
    panel(0, 'pehle log', ['Order', 'Arpan BUY 10', ''], ['Log (disk)', '#501 likha', 'acc'], 'likha', true, C.acc, 'number ke saath');
    panel(1, 'phir book', ['Log (disk)', '#501', 'acc'], ['Book (RAM)', '#501 juda', 'good'], 'match', true, C.good, 'RAM me tez');
    panel(2, 'server gira', ['Matching', 'gira', 'bad'], ['Book (RAM)', 'gayab', 'bad'], '', true, C.bad, 'par log disk pe hai');
    panel(3, 'replay', ['Log (disk)', '#1 se #4000', 'acc'], ['Book (RAM)', 'wahi 4,000 wapas', 'good'], 'dobara chalao', true, C.good, 'kuch nahi khoya');
    L(450, 320, 'log hi sach hai, book uski photo', C.good);
  } else {
    B(20, 140, 150, 60, 'Log (disk)', '#1 ... #4000', { st: 'acc' });
    B(330, 40, 230, 70, 'Matching main', 'gira', { st: 'bad' }); cross(585, 75);
    B(330, 230, 230, 70, 'Standby', 'log padh ke book taiyaar', { st: 'good' });
    B(690, 140, 170, 60, 'Orders', 'chalte rahe', { st: 'good' });
    A(170, 160, 328, 75, C.acc, { n: 1 }); L(240, 95, 'padhta tha', C.acc);
    A(170, 185, 328, 265, C.acc, { n: 2 }); L(240, 245, 'saath-saath padhta', C.acc);
    num(445, 150, 3, C.bad); L(465, 155, 'main gira', C.bad, 'start');
    A(560, 265, 688, 180, C.good, { n: 4 }); L(640, 250, 'turant chalu', C.good);
    L(450, 345, 'do board ek saath nahi: standby sirf padhta, chalu tab jab main gire', C.good);
  }
}
