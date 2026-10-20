// Card 4: match khatam, Virat ka tweet, crore feed ek saath DB se, DB gira. Ilaaj: hot-tweet cache (RAM), miss pe
// sirf ek request DB (lock), 4 frame. Agar: Virat ne edit kiya -> cache se key hatao, agla read DB se naya.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: match khatam, Virat ka tweet. Crore feed ek saath DB se wahi maangti, DB gira'],
  [3, GRN, 'ILAAJ', 'ILAAJ: garam tweet RAM me (hot-tweet cache). Miss pe sirf ek request DB jaati'],
  [6, YEL, 'AGAR', 'AGAR Virat ne tweet edit kiya, cache me purana? -> cache se key hata do'],
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
  [[C.bad, 'Crore feeds → Feed Svc', 'Crore logon ki feed me Virat ka ek hi tweet chahiye tha'], [C.bad, 'Feed Svc → Tweet DB', 'Har feed ke liye Feed Svc ne DB se wahi tweet maanga, crore baar'], [C.bad, 'Tweet DB', 'Disk pe crore read aaye aur DB gir gaya']],
  [[C.acc, 'Feed Svc → Hot cache', 'Pehli request ko cache me nahi mila, usne lock le liya'], [C.acc, 'Tweet DB → Feed Svc', 'Sirf usi ek request ne DB se tweet v1 laaya, baaki ruke'], [C.good, 'Feed Svc → Hot cache', 'v1 RAM wale hot cache me rakha aur lock khol diya'], [C.good, 'Hot cache → Crore feeds', 'Baaki sab ko RAM se ~0.1 ms me tweet mil gaya']],
  [[C.acc, 'Virat → Tweet Svc', 'Virat ne tweet edit kiya, naya version v2 bana'], [C.good, 'Tweet Svc → Tweet DB', 'Tweet Svc ne pehle DB me v2 likha, sach DB me rehta'], [C.bad, 'Tweet Svc → Hot cache', 'Phir cache se purani key hata di, taaki v1 na dikhe'], [C.good, 'agla read', 'Agle read ko cache me nahi mila, DB se naya v2 aaya']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 110, 150, 110, 'Crore feeds', '', { st: 'bad' }); dots(18, 35, 160, C.bad, { cols: 6, gap: 20, r: 6 });
    B(300, 135, 150, 60, 'Feed Svc', '');
    B(620, 120, 170, 90, 'Tweet DB', 'Virat: v1', { st: 'bad' }); cross(815, 165);
    A(170, 165, 298, 165, C.bad, { n: 1 }); L(235, 145, 'Virat ka tweet?', C.bad);
    A(450, 165, 618, 165, C.bad, { n: 2 }); L(535, 145, 'crore baar', C.bad);
    num(705, 240, 3, C.bad); L(705, 265, 'disk pe crore read: gira', C.bad);
    L(450, 330, 'sab ek hi tweet maang rahe the, aur har baar disk tak gaye', C.bad);
  } else if (ph === 1) {
    panel(0, 'pehli request', ['Feed Svc', 'Virat ka tweet?', ''], ['Hot cache', 'nahi mila (miss)', 'bad'], 'hai?', true, C.acc, 'lock: sirf yahi DB jaaye');
    panel(1, 'ek hi DB tak', ['Feed Svc', '', ''], ['Tweet DB', 'Virat: v1', ''], 'v1 laaya', false, C.acc, 'baaki request ruki');
    panel(2, 'cache me rakha', ['Feed Svc', '', ''], ['Hot cache', 'v1 (RAM)', 'good'], 'rakha', true, C.good, 'lock khula');
    panel(3, 'baaki crore', ['Crore feeds', '', 'good'], ['Hot cache', 'v1 (RAM)', 'good'], 'RAM se', false, C.good, '~0.1 ms, DB tak nahi');
    L(450, 320, 'DB tak sirf ek request gayi. Baaki sab RAM se', C.good);
  } else {
    B(20, 140, 120, 60, 'Virat', 'edit: v2', { st: 'acc' });
    B(240, 140, 150, 60, 'Tweet Svc', '');
    B(560, 40, 170, 60, 'Tweet DB', 'v2', { st: 'good' });
    B(560, 240, 170, 60, 'Hot cache', 'v1 hataya', { st: 'bad' }); cross(755, 270);
    A(140, 170, 238, 170, C.acc, { n: 1 });
    A(390, 155, 558, 75, C.good, { n: 2 }); L(465, 95, 'pehle DB: v2', C.good);
    A(390, 185, 558, 265, C.bad, { n: 3 }); L(420, 285, 'phir cache se key hatao', C.bad);
    L(645, 135, 'agla read: miss -> DB se v2', C.good); num(645, 160, 4, C.good);
    L(450, 340, 'purana v1 cache me nahi bacha, sabko naya v2 dikhta', C.good);
  }
}
