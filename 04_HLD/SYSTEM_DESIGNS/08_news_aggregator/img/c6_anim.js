// Card 6: sirf tech + sports chahiye. Ilaaj: har category ki list, padhte waqt milao. Agar: 2 category -> id se dedupe.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: mujhe sirf tech aur sports chahiye. Ek hi feed me politics, bollywood sab'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har category ki list pehle se (feed:tech, feed:sports). Kholne pe dono milao'],
  [6, YEL, 'AGAR', 'AGAR khabar 2 category list me? -> dono me ZADD, milate waqt id se dedupe'],
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
  [[C.bad, 'Feed Svc → Arpan', 'Arpan ko sabki ek hi feed mili: politics, bollywood sab'], [C.bad, 'har user ki alag feed?', 'Har user ki alag feed = 10 lakh feed, har khabar 10 lakh jagah daalni padti']],
  [[C.acc, 'Worker → feed:tech', 'Khabar aayi to worker ne apni category ki list me daali'], [C.msg, 'Arpan → Feed Svc', 'Arpan ki pasand tech aur sports hai, usne feed maangi'], [C.good, 'Redis → Feed Svc', 'Redis se feed:tech aur feed:sports ke top 20 aaye'], [C.good, 'Feed Svc → Arpan', 'Time se milake 20 diye; list sirf ~20, 10 lakh nahi']],
  [[C.acc, 'Worker → 2 list', 'Khabar a91 tech bhi, sports bhi, to dono list me ZADD'], [C.acc, 'Redis → Feed Svc', 'Feed Svc ne dono list padhi, a91 do baar aayi'], [C.good, 'Feed Svc', 'id se dedupe kiya, a91 feed me ek hi baar']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 120, 200, 80, 'Arpan', 'tech + sports', { icon: 'user' });
    B(340, 20, 230, 250, 'Ek hi feed', '', { st: 'bad' });
    [['politics', C.muted], ['tech', C.good], ['bollywood', C.muted], ['sports', C.good], ['politics', C.muted]].forEach(([t, c], i) =>
      cells(360, 70 + i * 38, [[t, c]], { w: 190, h: 30 }));
    A(338, 160, 222, 160, C.bad, { n: 1 });
    B(660, 110, 210, 100, '10 lakh feed?', 'har user ki alag', { st: 'bad' });
    num(765, 250, 2, C.bad); L(765, 280, 'har khabar 10 lakh jagah', C.bad);
    L(450, 345, 'sabko sab, ya phir 10 lakh feed. Dono galat', C.bad);
  } else if (ph === 1) {
    B(20, 20, 170, 70, 'Worker', 'a91 (tech)');
    B(20, 230, 170, 70, 'Arpan', 'tech + sports', { icon: 'user' });
    B(330, 130, 190, 80, 'Feed Svc', 'milao', { st: 'good' });
    B(640, 10, 230, 60, 'feed:tech', 'a91, a77 ...', { st: 'acc' });
    B(640, 95, 230, 60, 'feed:sports', 'a88, a60 ...', { st: 'acc' });
    B(640, 180, 230, 60, 'feed:politics', 'nahi chahiye', { st: 'dim' });
    A(190, 45, 638, 40, C.acc, { n: 1, at: 0.3 });
    A(190, 245, 328, 172, C.msg, { n: 2 });
    A(638, 120, 522, 165, C.good, { n: 3 }); A(638, 50, 522, 150, C.good);
    A(330, 205, 192, 282, C.good, { n: 4, at: 0.5 });
    L(450, 345, 'list pehle se bani, milana padhte waqt. Sirf ~20 list', C.good);
  } else {
    B(20, 130, 170, 80, 'Worker', 'a91: tech + sports');
    B(330, 30, 220, 70, 'feed:tech', 'a91, a77', { st: 'acc' });
    B(330, 230, 220, 70, 'feed:sports', 'a91, a60', { st: 'acc' });
    B(660, 110, 210, 120, 'Feed Svc', '', { st: 'good' });
    cells(675, 160, [['a91', C.bad], ['a77', C.good], ['a91', C.bad]], { w: 56, h: 28 });
    T(765, 215, 'a91 ek hi baar', { c: C.good, b: 1, size: 14 });
    A(190, 155, 328, 70, C.acc, { n: 1 }); A(190, 185, 328, 265, C.acc);
    A(550, 70, 658, 140, C.acc, { n: 2 }); A(550, 265, 658, 200, C.acc);
    num(765, 260, 3, C.good); L(785, 265, 'dedupe', C.good, 'start');
    L(450, 345, 'do list me rakhna sasta, milate waqt id dekho', C.good);
  }
}
