// Card 1: sabko wahi 20 khabar, har baar DB se sort. Ilaaj: Redis sorted set. Agar: Redis khaali -> lock, ek banaye.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: subah sab app khol rahe. Sabko wahi 20 khabar, phir bhi har baar DB pe sort'],
  [3, GRN, 'ILAAJ', 'ILAAJ: feed ek baar banti, Redis me time se (sorted set). Kholne pe sirf 20 id, DB nahi'],
  [6, YEL, 'AGAR', 'AGAR Redis restart, list khaali? -> DB se ek baar banao, lock ke saath'],
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
  [[C.msg, 'Users → Feed Svc', 'Subah lakhon log ek saath app khol ke feed maang rahe'], [C.bad, 'Feed Svc → DB', 'Har user ke liye DB pe ORDER BY time LIMIT 20 chala'], [C.bad, 'DB', 'Har second 500 baar wahi sort, DB bhar gaya aur feed dheemi']],
  [[C.acc, 'Worker → Redis', 'Nayi khabar aate hi worker ne Redis me ZADD kiya, time ke kram me'], [C.msg, 'Arpan → Feed Svc', 'Arpan ne app khola aur feed maangi'], [C.good, 'Redis → Feed Svc', 'Redis ne ZREVRANGE 0 19 se taazi 20 id de di'], [C.good, 'Feed Svc → Arpan', 'Arpan ko 20 khabar mili, DB tak koi nahi gaya']],
  [[C.acc, 'Feed Svc-A → Redis', 'Redis restart hua, list khaali; Feed Svc-A ne lock liya (SET NX)'], [C.good, 'Feed Svc-A → DB', 'Sirf A ne DB se ek baar list banayi'], [C.good, 'Feed Svc-A → Redis', 'A ne bani hui list wapas Redis me bhar di'], [C.acc, 'Feed Svc-B → Redis', 'B ko lock nahi mila, thoda ruka aur phir Redis se padha']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 120, 190, 80, 'Users', 'subah: lakhon', { icon: 'users' });
    B(330, 120, 200, 80, 'Feed Svc', 'har baar DB');
    B(650, 110, 220, 100, 'DB', 'ORDER BY time', { st: 'bad', icon: 'db' });
    bar(670, 175, 180, 12, 0.95, '');
    A(210, 160, 328, 160, C.msg, { n: 1 });
    A(530, 160, 648, 160, C.bad, { n: 2 });
    num(760, 250, 3, C.bad); L(760, 280, 'har baar wahi sort', C.bad);
    L(450, 345, 'sabko wahi 20 khabar, phir bhi har user pe sort', C.bad);
  } else if (ph === 1) {
    panel(0, 'nayi khabar', ['Worker', 'a91 aayi', ''], ['Redis', 'feed:latest', 'acc'], 'ZADD', true, C.acc, 'time ke kram me');
    panel(1, 'app khola', ['Arpan', 'feed?', ''], ['Feed Svc', '', ''], 'maangi', true, C.msg, 'subah 8 baje');
    panel(2, 'Redis se', ['Feed Svc', '20 id', 'good'], ['Redis', 'ZREVRANGE 0 19', 'acc'], 'top 20', false, C.good, '1 ms');
    panel(3, 'jawab', ['Arpan', '20 khabar', 'good'], ['Feed Svc', '', ''], 'feed', false, C.good, 'DB tak koi nahi');
    L(450, 330, 'feed ek baar banti, sab padhte. ~99% DB tak nahi jaata', C.good);
  } else {
    B(20, 30, 200, 80, 'Feed Svc-A', 'pehla miss', { st: 'good' });
    B(20, 240, 200, 80, 'Feed Svc-B', 'doosra miss', { st: 'acc' });
    B(360, 130, 220, 90, 'Redis', 'restart: khaali', { st: 'acc', icon: 'db' });
    B(690, 20, 180, 80, 'DB', 'articles', { icon: 'db' });
    A(220, 60, 358, 142, C.acc, { n: 1, at: 0.45 });
    A(220, 45, 688, 55, C.good, { n: 2, at: 0.75 });
    A(220, 98, 358, 190, C.good, { n: 3, at: 0.5 });
    A(220, 270, 358, 205, C.acc, { n: 4 });
    L(470, 250, 'DB pe sirf ek query, 1000 nahi', C.good, 'start');
    L(450, 345, 'lock: ek banaye, baaki ruk ke Redis se padhein', C.good);
  }
}
