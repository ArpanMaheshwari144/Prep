// Card 1: har click DB pe. Ilaaj: Redis cache-aside. Agar: link badla, Redis me purana.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: sale ka link viral, har click seedha DB pe, DB baith gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: beech me Redis. Pehle Redis dekho, mila to wahin se (cache-aside)'],
  [6, YEL, 'AGAR', 'AGAR link badla, Redis me purana? -> pehle DB badlo, phir Redis se hatao'],
];
function setup() {}
function users(n) { B(20, 130, 120, 60, 'Users', n, {}); }
const STEPS = [
  [[C.msg, 'Users → App', 'Sale ka link viral hua, lakhon log ek saath us pe click kar rahe hain'], [C.bad, 'App → DB', 'Har click pe App ne asli URL DB se maanga, har baar disk tak gaya (~10 ms)'], [C.bad, 'DB', 'Itni padhai DB jhel nahi paaya, 100% pe baith gaya aur sab link khulne band']],
  [[C.msg, 'Users → App', 'User ne short link pe click kiya, request App tak aayi'], [C.good, 'App → Redis', 'App ne pehle Redis me dhoondha; 95% baar wahin mil jaata, 0.1 ms me jawab'], [C.acc, 'App → DB', 'Redis me nahi mila to hi App DB tak gaya aur asli URL laaya'], [C.acc, 'App → Redis', 'DB se laaya URL App ne Redis me rakh diya, taaki agla click wahin se khule']],
  [[C.acc, 'Admin → App', 'Admin ne link ka target URL badal diya'], [C.good, 'App → DB', 'App ne pehle DB me naya URL likha, kyunki sach DB me rehta hai'], [C.bad, 'App → Redis', 'Phir Redis se purani key hata di; agla click DB se naya URL laayega']],
];
function frame(ph) {
  if (ph === 0) {
    users('lakhon click'); A(140, 160, 258, 160, C.msg, { n: 1 }); dots(4, 165, 140, C.msg, { gap: 20 });
    B(260, 130, 120, 60, 'App'); A(380, 160, 558, 160, C.bad, { n: 2 }); dots(4, 405, 140, C.bad, { gap: 20 });
    B(560, 130, 120, 60, 'DB', 'disk', { st: 'bad' }); cross(705, 160);
    bar(560, 215, 120, 16, 1, ''); T(620, 255, '100%', { c: C.bad, b: 1, size: 14 });
    num(620, 285, 3, C.bad); L(620, 310, 'DB baith gaya', C.bad);
    L(470, 200, 'har click disk tak (~10 ms)', C.bad);
  } else if (ph === 1) {
    users(''); A(140, 160, 258, 160, C.msg, { n: 1 });
    B(260, 130, 120, 60, 'App');
    B(560, 50, 120, 60, 'Redis', 'RAM, ~0.1 ms', { st: 'good' });
    A(380, 145, 558, 85, C.good, { n: 2, at: 0.35 }); L(470, 75, 'hai?', C.good);
    A(558, 95, 380, 155, C.good); L(470, 150, 'mila, 95% yahin', C.good);
    B(560, 230, 120, 60, 'DB', 'disk', {});
    A(380, 175, 558, 255, C.muted, { n: 3, dash: 1, at: 0.4 }); L(470, 265, 'nahi mila to DB se laao', C.muted);
    // copy App hi Redis me rakhta (DB khud Redis ko nahi bhejta)
    AP([[300, 128], [300, 20], [620, 20], [620, 48]], C.muted, { n: 4, dash: 1, at: 0.8 });
    L(310, 12, 'App copy Redis me rakhta', C.muted, 'start');
    bar(560, 305, 120, 16, 0.05, '', 1); T(620, 345, '5%', { c: C.good, b: 1, size: 14 });
  } else {
    B(20, 130, 120, 60, 'Admin', 'link badla', { st: 'acc' });
    A(140, 160, 258, 160, C.acc, { n: 1 }); B(260, 130, 120, 60, 'App');
    B(560, 230, 120, 60, 'DB', 'naya link', { st: 'good' });
    A(380, 175, 558, 255, C.good, { n: 2, at: 0.4 }); L(470, 265, 'pehle DB badlo', C.good);
    B(560, 50, 120, 60, 'Redis', 'purana link', { st: 'bad' });
    A(380, 145, 558, 85, C.bad, { n: 3, at: 0.35 }); cross(520, 90); L(470, 75, 'phir Redis se hatao', C.bad);
    L(620, 135, 'agla click: DB se naya, Redis me rakho', C.muted);
    L(300, 320, 'ulta kiya to beech ka click purana link wapas Redis me bhar deta', C.acc, 'start');
  }
}
