// Card 4: 9:15 sab TCS pe toot pade. Har order pe lock + DB = jam. Ilaaj: ek line, ek thread RAM me, ACCEPTED turant.
// Agar: baantoge nahi, to badhaoge kaise? -> stocks ke beech baanto, TCS ke andar nahi.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 9:15, lakhon order TCS pe. Har order pe DB lock, ek-ek karke, sab jam'],
  [3, GRN, 'ILAAJ', 'ILAAJ: TCS ki ek line, ek thread RAM me tez nipta deta. Line me lagte hi ACCEPTED'],
  [6, YEL, 'AGAR', 'AGAR baantoge nahi, to badhaoge kaise? -> stocks ke beech: TCS, INFY alag'],
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
  [[C.bad, 'Lakhon order → Order Svc', '9:15 pe sab TCS pe'], [C.bad, 'Order Svc → DB', 'har order pe DB lock, likho, chhodo'], [C.bad, 'DB', 'ek-ek karke, baaki intezaar me']],
  [[C.msg, 'Arpan → TCS line', 'line me laga: no. 4,812'], [C.good, 'Order Svc → Arpan', 'turant ACCEPTED, app nahi ghooma'], [C.good, 'Thread-1 → TCS book (RAM)', 'RAM me match, microsecond'], [C.good, 'Thread-1 → Arpan', 'thodi der baad FILLED']],
  [[C.good, 'har stock → apna thread', 'TCS, INFY, HDFC alag.|5000 stock: stock % 8 core'], [C.bad, 'TCS do thread me?', 'nahi: kram tootta.|Ek stock = ek line']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 110, 150, 110, 'Lakhon order', 'TCS, 9:15', { st: 'bad' }); dots(15, 35, 175, C.bad, { cols: 8, gap: 15, r: 5 });
    B(300, 130, 160, 70, 'Order Svc', '');
    B(620, 120, 220, 90, 'DB', 'lock lo, likho, chhodo', { st: 'bad' });
    A(170, 165, 298, 165, C.bad, { n: 1 }); A(460, 165, 618, 165, C.bad, { n: 2 }); L(540, 145, 'har order pe', C.bad);
    num(730, 245, 3, C.bad); L(730, 270, 'baaki sab lock ke intezaar me', C.bad);
    L(450, 330, 'disk + lock har order pe: line lambi, app ghoomta', C.bad);
  } else if (ph === 1) {
    panel(0, 'line me laga', ['Arpan', 'BUY TCS', ''], ['TCS line', 'no. 4,812', 'acc'], 'bheja', true, C.msg, 'order kho nahi sakta');
    panel(1, 'turant jawab', ['Arpan', 'ACCEPTED', 'good'], ['Order Svc', '', ''], 'turant', false, C.good, 'app nahi ghoomta');
    panel(2, 'RAM me match', ['Thread-1', 'sirf TCS', 'good'], ['TCS book (RAM)', 'microsecond', 'good'], 'match', true, C.good, 'lakhon / sec');
    panel(3, 'sauda hua', ['Arpan', 'FILLED', 'good'], ['Thread-1', '', ''], 'khabar', false, C.good, 'thodi der baad');
    L(450, 320, 'line lambi = thodi der, par order khota nahi aur jam nahi', C.good);
  } else {
    [['TCS', 'Thread-1'], ['INFY', 'Thread-2'], ['HDFC', 'Thread-3'], ['baaki 5000', 'stock % 8']].forEach(([s, t], i) => {
      const y = 25 + i * 80;
      B(40, y, 180, 55, s + ' ki line', '', { st: i < 3 ? 'good' : '' });
      B(400, y, 180, 55, t, i < 3 ? '1 core' : '8 core = 8 thread', { st: i < 3 ? 'good' : '' });
      A(220, y + 27, 398, y + 27, C.good, { n: i === 0 ? 1 : 0 });
    });
    B(630, 40, 210, 70, 'TCS do thread me?', 'kram toot-ta', { st: 'bad' }); cross(862, 75);
    num(735, 140, 2, C.bad); L(735, 165, 'ek stock = ek line', C.bad);
    L(450, 350, 'baantna stocks ke BEECH, ek stock ke andar kabhi nahi', C.good);
  }
}
