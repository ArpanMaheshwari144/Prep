// Card 8: sab ek table me (May se Oct), purana koi padhta nahi, disk + backup ka kharcha.
// Ilaaj: mahine ka partition, purana S3. Agar: DETACH pe lock, 1 saal purani khuli. Wapas: DB utna hi.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: sab ek table me (May se Oct), purana koi padhta nahi, faltu kharcha'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har mahine ka alag partition, purana S3 pe. Postgres me sirf haal ka'],
  [6, YEL, 'AGAR', 'AGAR DETACH pe lock lagega? -> normal DETACH: pal bhar ka lock, padhai ruki'],
  [9, YEL, 'AGAR', 'AGAR 1 saal purani khabar khuli? -> S3 archive se aati. Dheemi, par khulti'],
  [12, GRN, 'WAPAS', 'WAPAS: DB utna hi rehta. Naya mahina garam, purana thanda (S3)'],
];
const OLD = ['May', 'Jun', 'Jul', 'Aug', 'Sep'];
function setup() {}
function split(o = {}) {
  B(20, 60, 110, 60, 'User', o.u);
  B(250, 30, 250, 130, 'Postgres', 'garam, index', { st: o.pst || 'good' });
  cells(330, 90, [[o.cur || 'Oct', C.good]], { w: 90, h: 40 });
  B(560, 200, 320, 120, 'S3 archive', 'thanda, sasta', { st: o.sst || 'good' });
  cells(576, 260, (o.old || OLD).map(m => [m, C.muted]), { w: 52, h: 34 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 60, 110, 60, 'User', 'Oct padhta');
    B(250, 30, 420, 170, 'Postgres: articles', '~2 crore row', { st: 'bad' });
    cells(270, 100, OLD.map(m => [m, C.bad]).concat([['Oct', C.good]]), { w: 60, h: 40 });
    A(130, 90, 248, 90, C.msg, { n: 1 }); L(190, 75, 'sirf Oct', C.msg);
    num(270, 170, 2, C.bad); L(288, 175, 'May se Sep: koi padhta nahi', C.bad, 'start');
    bar(270, 260, 380, 22, 1, 'disk + backup'); T(460, 320, 'faltu kharcha', { c: C.bad, b: 1, size: 16 });
  } else if (ph === 1) {
    split({ u: 'Oct padhta' });
    A(375, 162, 600, 255, C.acc, { n: 1 }); L(420, 230, 'mahina beeta -> S3', C.acc);
    A(130, 90, 248, 90, C.good, { n: 2 }); L(190, 75, 'tez', C.good);
  } else if (ph === 2) {
    split({ u: 'Oct padhta', pst: 'bad' });
    num(520, 40, 1, C.acc); L(538, 45, 'DETACH Sep', C.acc, 'start');
    T(375, 152, 'LOCK', { c: C.bad, b: 1, size: 16 });
    num(520, 80, 2, C.bad); L(538, 85, 'poore table pe pal bhar', C.bad, 'start');
    A(130, 90, 248, 90, C.bad, { dash: 1, n: 3 }); cross(190, 125); L(190, 160, 'padhai ruki', C.bad);
    L(20, 205, 'hal: PG 14+ me DETACH ... CONCURRENTLY,', C.good, 'start');
    L(20, 225, 'kuch nahi rukta', C.good, 'start');
  } else if (ph === 3) {
    split({ u: '1 saal purani', sst: 'acc', old: ['1 saal', '..', 'Jul', 'Aug', 'Sep'] });
    AP([[75, 120], [75, 290], [558, 290]], C.acc, { n: 1, at: 0.3 }); L(300, 280, 'S3 se aati', C.acc);
    T(75, 320, 'dheemi', { c: C.acc, b: 1, size: 14 });
    L(560, 185, 'hal: kabhi-kabhi hi khulti, dheemi chalegi', C.good, 'start');
  } else {
    split({ u: 'naya padhta', cur: 'naya', old: ['Jun', 'Jul', 'Aug', 'Sep', 'Oct'] });
    A(375, 162, 600, 255, C.good, { n: 1 }); L(420, 230, 'Oct beeta -> S3', C.good);
    A(130, 90, 248, 90, C.good, { n: 2 }); L(190, 75, 'tez', C.good);
    L(520, 60, 'DB utna hi', C.good, 'start'); L(520, 85, 'naya garam, purana thanda', C.good, 'start');
  }
}
