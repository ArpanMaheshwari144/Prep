// Card 8: 6 mahine me 2 crore row, purana koi padhta nahi, kharcha. Ilaaj: mahine ka partition, purana S3. Agar: purani khabar.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 6 mahine me ~2 crore row, sab ek table me. Purana koi nahi padhta, kharcha badha'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har mahine ka alag tukda (partition). Purana tukda S3 pe (archive)'],
  [6, YEL, 'AGAR', 'AGAR 1 saal purani khabar khuli? -> S3 archive se aati. Dheemi, par khulti'],
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
  [[C.bad, 'Ek table', 'May se Oct tak saari khabar ek hi table me'], [C.bad, 'Disk + backup', 'Purane mahine bhi mehngi disk pe aur har backup me, faltu kharcha']],
  [[C.good, 'Worker → Oct tukda', 'Nayi khabar sirf is mahine (Oct) ke tukde me gayi'], [C.acc, 'May tukda → S3', 'Purana May wala tukda DETACH karke sasti S3 pe bheja'], [C.good, 'Postgres', 'Postgres me sirf haal ke mahine, chhota, index RAM me']],
  [[C.msg, 'Arpan → Article Svc', 'Arpan ne pichhle saal ki khabar a12 kholi'], [C.muted, 'Article Svc → Postgres', 'Postgres me nahi mili, wahan sirf haal ke mahine'], [C.acc, 'Article Svc → S3 archive', 'Article Svc ne S3 archive me dhoondhi, wahan mili'], [C.good, 'Article Svc → Arpan', 'Khabar khul gayi, bas thodi der se']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 30, 560, 220, 'articles: ek table', '', { st: 'bad' });
    cells(40, 85, [['May', C.muted], ['Jun', C.muted], ['Jul', C.muted], ['Aug', C.muted], ['Sep', C.acc], ['Oct', C.good]], { w: 82, h: 34 });
    T(300, 160, 'May-Aug koi nahi padhta', { c: C.muted, size: 14 });
    num(60, 210, 1, C.bad); L(80, 215, '2 crore row, ek jagah', C.bad, 'start');
    B(660, 80, 210, 120, 'Disk + backup', 'har mahine badhta', { st: 'bad' });
    bar(680, 160, 170, 12, 0.9, '');
    A(580, 140, 658, 140, C.bad, { n: 2 });
    L(450, 330, 'thande data ka garam kharcha', C.bad);
  } else if (ph === 1) {
    B(20, 130, 150, 80, 'Worker', 'a91 (Oct)');
    B(260, 30, 360, 200, 'Postgres', '', { st: 'good' });
    cells(280, 90, [['Sep', C.acc], ['Oct', C.good]], { w: 150, h: 40 });
    T(440, 212, 'garam: chhota, tez', { c: C.good, size: 14 });
    B(700, 60, 170, 160, 'S3', 'May - Aug', { st: 'acc', icon: 'bucket' });
    AP([[170, 165], [509, 165], [509, 133]], C.good, { n: 1, at: 0.3 });
    A(620, 140, 698, 140, C.acc, { n: 2 }); L(660, 125, 'DETACH', C.acc);
    num(440, 260, 3, C.good); L(460, 265, 'index RAM me', C.good, 'start');
    L(450, 345, 'naya garam Postgres me, purana thanda S3 pe', C.good);
  } else {
    B(20, 130, 150, 80, 'Arpan', 'a12 (2025)', { icon: 'user' });
    B(270, 130, 200, 80, 'Article Svc', '', { st: 'good' });
    B(620, 20, 250, 80, 'Postgres', 'sirf haal ke mahine', { icon: 'db' });
    B(620, 220, 250, 80, 'S3 archive', 'purani yahan', { st: 'acc', icon: 'bucket' });
    A(170, 170, 268, 170, C.msg, { n: 1 });
    A(470, 150, 618, 60, C.muted, { n: 2, dash: 1 }); L(520, 85, 'nahi', C.muted, 'end');
    A(470, 190, 618, 260, C.acc, { n: 3 });
    A(268, 195, 172, 195, C.good, { n: 4 }); L(220, 225, 'khuli', C.good);
    L(450, 345, 'kabhi-kabhi hi khulti hai, dheema chalta hai', C.good);
  }
}
