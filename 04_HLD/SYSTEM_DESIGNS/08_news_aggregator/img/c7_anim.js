// Card 7: 'cricket' dhoondha, LIKE '%cricket%' poori table. Ilaaj: inverted index (ES). Agar: cricket kohli -> AND.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine "cricket" dhoondha. LIKE %cricket% ne 2 crore row ek-ek padhi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har shabd ki list ki wo kis khabar me hai (inverted index, Elasticsearch)'],
  [6, YEL, 'AGAR', 'AGAR "cricket kohli" dhoondha? -> dono list milao, AND = a45'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Arpan → Search Svc', '"cricket" dhoondha'], [C.bad, 'Search Svc → DB', 'LIKE %cricket%: % aage, index nahi lagta'], [C.bad, 'DB', '2 crore row ek-ek padhi, jawab 8 sec']],
  [[C.msg, 'Search Svc → ES', '"cricket" ki list do'], [C.good, 'ES → Search Svc', 'a12, a45, a90: sirf id'], [C.acc, 'Search Svc → Redis / DB', 'in id ka text laao'], [C.good, 'Search Svc → Arpan', 'results, ms me']],
  [[C.acc, 'cricket', 'a12, a45, a90'], [C.acc, 'kohli', 'a45, a77'], [C.good, 'AND', 'dono me sirf a45. Phir score se sort']],
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
    B(20, 130, 160, 80, 'Arpan', '"cricket"', { icon: 'user' });
    B(300, 130, 190, 80, 'Search Svc', '');
    B(610, 70, 260, 200, 'DB: articles', '', { st: 'bad' });
    for (let i = 0; i < 5; i++) cells(630, 115 + i * 28, [['row ' + (i + 1), C.bad], ['...', C.line]], { w: 105, h: 22 });
    A(180, 170, 298, 170, C.msg, { n: 1 });
    A(490, 170, 608, 170, C.bad, { n: 2 });
    num(740, 300, 3, C.bad); L(760, 305, '2 crore row', C.bad, 'start');
    T(400, 290, "WHERE title LIKE '%cricket%'", { c: C.fg, size: 14 });
    L(450, 345, '% aage hai to index kaam nahi aata, poori table', C.bad);
  } else if (ph === 1) {
    B(20, 130, 150, 80, 'Arpan', '', { icon: 'user' });
    B(260, 130, 180, 80, 'Search Svc', '', { st: 'good' });
    B(560, 10, 310, 140, 'ES (inverted index)', '', { st: 'acc' });
    T(715, 80, 'cricket -> a12, a45, a90', { c: C.fg, size: 14 });
    T(715, 110, 'kohli   -> a45, a77', { c: C.fg, size: 14 });
    B(560, 230, 310, 70, 'Redis / DB', 'text yahan, sach yahan', { icon: 'db' });
    A(440, 150, 558, 95, C.msg, { n: 1, at: 0.4 });
    A(558, 125, 442, 170, C.good, { n: 2, at: 0.6 });
    A(440, 190, 558, 260, C.acc, { n: 3 });
    A(258, 170, 172, 170, C.good, { n: 4 });
    L(450, 345, 'ES sirf id deta. Gira to DB se dobara bana lo', C.good);
  } else {
    B(20, 30, 400, 70, '', '', { st: 'acc' }); T(100, 71, 'cricket', { b: 1, size: 17 });
    cells(190, 50, [['a12', C.muted], ['a45', C.good], ['a90', C.muted]], { w: 60, h: 30 });
    B(20, 160, 400, 70, '', '', { st: 'acc' }); T(100, 201, 'kohli', { b: 1, size: 17 });
    cells(190, 180, [['a45', C.good], ['a77', C.muted]], { w: 60, h: 30 });
    B(580, 90, 290, 90, 'AND', 'a45', { st: 'good' });
    num(440, 65, 1, C.acc); num(440, 195, 2, C.acc);
    A(420, 70, 578, 120, C.good); A(420, 195, 578, 150, C.good);
    num(725, 210, 3, C.good); L(745, 215, 'score se sort', C.good, 'start');
    L(450, 300, 'OR hota to sab: a12, a45, a77, a90', C.muted);
    L(450, 345, 'do list ka milan, poori table nahi', C.good);
  }
}
