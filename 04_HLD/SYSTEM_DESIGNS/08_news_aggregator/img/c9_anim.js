// Card 9: source 100 guna, ek DB machine nahi jhelti. Ilaaj: shard hash(article_id). Agar: taazi 20 -> Redis, miss to har shard se.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: source 100 guna. Saari likhai ek hi DB machine pe, disk aur likhai full'],
  [3, GRN, 'ILAAJ', 'ILAAJ: article_id se tukde (shard). hash(a91) = 2 to shard 2. Likhai sab pe barabar'],
  [6, YEL, 'AGAR', 'AGAR shard hain, taazi 20 kahan se? -> Redis se. Miss to har tukde se thoda'],
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
  [[C.bad, 'Workers → DB', 'Source 100 guna hue, saari likhai ek hi DB machine pe'], [C.bad, 'DB', 'Disk aur likhai dono full, sab dheema ho gaya']],
  [[C.acc, 'Worker', 'Worker ne article id ka hash nikala: hash(a91) = 2'], [C.good, 'Worker → Shard 2', 'Isliye a91 shard 2 pe likhi gayi'], [C.good, 'Shard 1, 2, 3', 'Bojh teeno pe barabar bata; ek mara to sirf uska hissa jaata']],
  [[C.good, 'Feed Svc → Redis', 'Feed Svc ne Redis ke feed:latest se 20 id li, zyadatar yahin se'], [C.acc, 'Feed Svc → har shard', 'Redis me nahi mila to har shard se uske top 20 maange'], [C.good, 'Feed Svc', 'Sab milake time se top 20 chune (scatter-gather)']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 20, 170, 60, 'Worker', '');
    B(20, 120, 170, 60, 'Worker', '');
    B(20, 220, 170, 60, 'Worker', '');
    B(470, 90, 280, 140, 'Ek DB', '100 guna likhai', { st: 'bad' });
    bar(490, 180, 240, 14, 1, '');
    A(190, 50, 468, 130, C.bad, { n: 1, at: 0.4 }); A(190, 150, 468, 160, C.bad); A(190, 250, 468, 190, C.bad);
    num(610, 270, 2, C.bad); L(630, 275, 'disk full', C.bad, 'start');
    L(450, 345, 'ek machine ki had aa gayi', C.bad);
  } else if (ph === 1) {
    B(20, 130, 200, 80, 'Worker', 'hash(a91) = 2', { st: 'acc' });
    B(620, 20, 250, 70, 'Shard 1', 'a12, a77 ...', { st: 'good', icon: 'db' });
    B(620, 135, 250, 70, 'Shard 2', 'a91 yahan', { st: 'good', icon: 'db' });
    B(620, 250, 250, 70, 'Shard 3', 'a45, a60 ...', { st: 'good', icon: 'db' });
    num(120, 250, 1, C.acc); L(140, 255, 'hash', C.acc, 'start');
    A(220, 170, 618, 170, C.good, { n: 2 }); L(420, 155, 'a91', C.good);
    A(220, 150, 618, 55, C.muted, { dash: 1 }); A(220, 190, 618, 285, C.muted, { dash: 1 });
    num(560, 330, 3, C.good); L(580, 335, 'bojh barabar', C.good, 'start');
  } else {
    B(20, 130, 190, 80, 'Feed Svc', 'taazi 20?', { st: 'good' });
    B(330, 10, 220, 70, 'Redis', 'feed:latest', { st: 'good' });
    B(650, 110, 220, 60, 'Shard 1', 'top 20', { icon: 'db' });
    B(650, 185, 220, 60, 'Shard 2', 'top 20', { icon: 'db' });
    B(650, 260, 220, 60, 'Shard 3', 'top 20', { icon: 'db' });
    A(210, 150, 328, 50, C.good, { n: 1 });
    A(210, 175, 648, 140, C.acc, { n: 2, dash: 1, at: 0.4 }); A(210, 185, 648, 215, C.acc, { dash: 1 }); A(210, 195, 648, 290, C.acc, { dash: 1 });
    num(115, 260, 3, C.good); L(135, 265, 'milao: top 20', C.good, 'start');
    L(450, 345, 'zyadatar Redis se. Miss pe har tukde se thoda', C.good);
  }
}
