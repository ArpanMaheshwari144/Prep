// Card 7: LIKE '%cricket%' -> index nahi laga, poori table padhi.
// Ilaaj: inverted index (ES) id deta, text DB / cache se. Agar: do shabd, galat spelling, ES aur DB alag. Wapas.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: LIKE %cricket%: index nahi laga, poori table (2 crore row) padhi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har shabd ki list (inverted index, ES). ES id de, text DB / cache se'],
  [6, YEL, 'AGAR', 'AGAR "cricket kohli" dhoondha? -> dono list milao, AND = a45'],
  [9, YEL, 'AGAR', 'AGAR "crikcet" likh diya? -> fuzzy (ES): 1-2 akshar ka farak chal jaata'],
  [12, YEL, 'AGAR', 'AGAR ES aur DB alag ho gaye? -> DB sach hai, ES dobara bana lo (reindex)'],
  [15, GRN, 'WAPAS', 'WAPAS: reindex hua, ES phir DB jaisa. Khabar nahi khoi, search chalu'],
];
function setup() {}
function base(o = {}) {
  B(20, 150, 100, 60, 'User', o.q);
  B(170, 150, 120, 60, 'Feed Svc', '', { st: o.sv });
  B(370, 30, 300, 110, 'Elasticsearch', '', { st: o.es || 'good' });
  B(370, 240, 300, 80, 'DB / cache', o.db || 'sach, text', { st: o.dbst || 'good' });
  (o.idx || [['cricket -> [a12, a45, a90]', C.good]]).forEach(([s, c], i) =>
    T(520, 82 + i * 24, s, { c, b: 1, size: 13 }));
}
function ask(n, c) { A(120, 180, 168, 180, c || C.msg, { n }); A(290, 160, 368, 70, c || C.msg, { n: n + 1 }); }
function frame(ph) {
  if (ph === 0) {
    B(20, 150, 100, 60, 'User', '"cricket"');
    B(300, 30, 360, 290, 'DB: articles', '', { st: 'bad' });
    A(120, 180, 298, 180, C.msg, { n: 1 }); L(210, 140, "LIKE '%cricket%'", C.msg);
    dots(60, 340, 95, C.bad, { gap: 28, cols: 11, r: 7 });
    num(690, 70, 2, C.bad); L(708, 75, 'index nahi laga', C.bad, 'start');
    num(690, 130, 3, C.bad); L(708, 135, 'ek-ek row padhi', C.bad, 'start');
    T(480, 300, 'poori table: 2 crore row', { c: C.bad, b: 1, size: 15 });
    T(780, 250, 'dheema', { c: C.bad, b: 1, size: 20 });
  } else if (ph === 1) {
    base({ q: '"cricket"', sv: 'good' });
    ask(1); L(250, 115, 'shabd do', C.msg);
    A(368, 125, 292, 195, C.good, { n: 3 }); L(700, 115, 'sirf id', C.good, 'start');
    A(260, 210, 368, 270, C.acc, { n: 4 }); L(250, 255, 'text laao', C.acc);
    L(700, 285, 'a12, a45, a90 ka text', C.good, 'start');
  } else if (ph === 2) {
    base({ q: 'cricket kohli', sv: 'good', idx: [['cricket -> [a12, a45, a90]', C.good], ['kohli -> [a45, a77]', C.good]] });
    ask(1); L(250, 115, 'do shabd', C.msg);
    num(690, 70, 3, C.acc); L(708, 75, 'dono list milao', C.acc, 'start');
    cells(708, 95, [['a45', C.good]], { w: 50 }); L(770, 112, 'AND', C.good, 'start');
    A(368, 125, 292, 195, C.good, { n: 4 }); L(20, 300, 'OR = sab, phir score se sort', C.good, 'start');
  } else if (ph === 3) {
    base({ q: '"crikcet"', sv: 'good', idx: [['crikcet ~ cricket', C.acc], ['cricket -> [a12, a45, a90]', C.good]] });
    ask(1); L(250, 115, 'galat spelling', C.msg);
    num(690, 70, 3, C.acc); L(708, 75, 'fuzzy: 1-2 akshar', C.acc, 'start');
    L(708, 95, 'ka farak chalta', C.acc, 'start');
    A(368, 125, 292, 195, C.good, { n: 4 }); L(20, 300, 'a12, a45, a90 mile', C.good, 'start');
  } else if (ph === 4) {
    base({ q: '"cricket"', es: 'bad', idx: [['cricket -> [a12, a45]', C.bad]], db: 'a12, a45, a90' });
    num(690, 70, 1, C.bad); L(708, 75, 'a90 ES me nahi', C.bad, 'start');
    L(708, 95, 'ES aur DB alag', C.bad, 'start');
    A(520, 238, 520, 142, C.good, { n: 2 }); L(540, 195, 'reindex', C.good, 'start');
    L(680, 285, 'DB sach hai', C.good, 'start');
    L(20, 330, 'hal: likhna fail ho to retry ki line', C.good, 'start');
  } else {
    base({ q: '"cricket"', sv: 'good', db: 'a12, a45, a90' });
    ask(1);
    A(368, 125, 292, 195, C.good, { n: 3 }); L(700, 115, 'phir DB jaisa', C.good, 'start');
    A(260, 210, 368, 270, C.good, { n: 4 }); L(250, 255, 'text laao', C.good);
    L(700, 285, 'khabar nahi khoi', C.good, 'start');
  }
}
