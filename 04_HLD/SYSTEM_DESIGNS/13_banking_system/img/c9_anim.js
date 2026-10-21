// Card 9: dukaan ko 500 aur usi second refund 200 -> deadlock. Ilaaj: taala hamesha chhoti id pehle. Agar: phir bhi phans gaye.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Txn1 ne Arpan pakda, dukaan ka intezaar; Txn2 ne dukaan, Arpan ka. Gol ghera'],
  [3, GRN, 'ILAAJ', 'ILAAJ: taala hamesha ek kram me, chhoti id pehle: dono pehle 7 (Arpan), phir 9 (dukaan)'],
  [6, YEL, 'AGAR', 'AGAR phir bhi phans gaye? -> DB ek ko wapas karta, app WAHI id se dobara'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Txn1 → Arpan', 'Txn1 (mera 500) ne Arpan (7) pakda, ab dukaan ka intezaar'],
   [C.msg, 'Txn2 → Dukaan', 'Txn2 (refund 200) ne dukaan (9) pakdi, ab Arpan ka intezaar'],
   [C.bad, 'dono', 'Dono ek doosre ka intezaar karte rahe: gol ghera, koi aage nahi']],
  [[C.good, 'Txn1 → 7', 'Niyam: chhoti id pehle. Txn1 ne pehle 7 (Arpan) pakda'],
   [C.acc, 'Txn2 → 7', 'Txn2 bhi pehle 7 maangta, isliye line me ruka'],
   [C.good, 'Txn1 → 9', 'Txn1 ne 9 pakda, poora hua, phir Txn2 chala: ghera bana hi nahi']],
  [[C.bad, 'Txn1, Txn2', 'Kram ke baad bhi kisi raaste se ghera ban gaya'],
   [C.acc, 'DB', 'DB ne ghera khud pakda, Txn2 ko wapas kiya (rollback)'],
   [C.good, 'App → DB', 'App ne thoda ruk ke WAHI id se dobara bheja, ek hi baar kata']],
];
function frame(ph) {
  B(15, 40, 200, 60, 'Txn1', '500 dukaan ko', { icon: 'server' });
  B(15, 250, 200, 60, 'Txn2', '200 refund', { icon: 'server' });
  B(560, 40, 220, 60, 'Arpan', 'id 7', { st: ph === 0 ? 'acc' : 'good', icon: 'user' });
  B(560, 250, 220, 60, 'Dukaan', 'id 9', { st: ph === 0 ? 'acc' : 'good', icon: 'user' });
  if (ph === 0) {
    A(215, 65, 558, 65, C.msg, { n: 1 }); L(390, 40, 'pakda', C.msg);
    A(215, 285, 558, 285, C.msg, { n: 2 }); L(390, 315, 'pakdi', C.msg);
    A(215, 90, 558, 260, C.bad, { dash: 1, at: 0.7 }); A(215, 260, 558, 90, C.bad, { dash: 1, at: 0.7 });
    num(387, 175, 3, C.bad); L(830, 175, 'gol ghera', C.bad);
  } else if (ph === 1) {
    A(215, 65, 558, 65, C.good, { n: 1 }); L(390, 40, '7 pakda', C.good);
    A(215, 260, 558, 95, C.acc, { n: 2, dash: 1, at: 0.3 }); L(115, 228, '7 ka intezaar', C.acc);
    A(215, 85, 558, 270, C.good, { n: 3, at: 0.7 }); L(500, 210, 'phir 9', C.good);
    L(840, 175, 'ek hi kram', C.good);
  } else {
    A(215, 90, 558, 260, C.bad, { dash: 1 }); A(215, 260, 558, 90, C.bad, { dash: 1 }); num(387, 175, 1, C.bad);
    B(785, 140, 105, 60, 'DB', 'pakda', { st: 'acc', icon: 'db' }); num(838, 115, 2, C.acc);
    AP([[110, 312], [110, 345], [670, 345], [670, 312]], C.good, { n: 3, at: 0.4 }); L(390, 330, 'WAHI id se dobara', C.good);
  }
}
