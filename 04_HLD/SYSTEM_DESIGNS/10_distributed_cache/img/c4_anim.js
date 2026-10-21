// Card 4: Ravi ne naam badla, sabko purana dikh raha. Ilaaj: pehle DB, phir cache key DELETE. Agar: delete fail -> TTL.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Ravi ne naam badla, DB me naya, par cache me purana. Sabko purana dikh raha'],
  [3, GRN, 'ILAAJ', 'ILAAJ: pehle DB me likho, phir cache ki key DELETE. Agla padhne wala DB se naya laata'],
  [6, YEL, 'AGAR', 'AGAR cache ka delete fail? -> umar (TTL) bachati, utni der hi purana dikhega'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Ravi → App', 'Ravi ne apna naam "Ravi" se "Ravi K" kiya'],
   [C.msg, 'App → DB', 'App ne DB me "Ravi K" likha, cache ko kisi ne chhua hi nahi'],
   [C.msg, 'Mohan → Cache', 'Mohan ne Ravi ki profile kholi, App ne cache se padha'],
   [C.bad, 'Cache → Mohan', 'Cache me purana "Ravi" tha, Mohan ko purana naam dikha']],
  [[C.msg, 'App → DB', 'Ravi ka naya naam pehle DB me likha: "Ravi K"'],
   [C.acc, 'App → Cache', 'Phir cache se Ravi ki key DELETE ki, purana naam gaya'],
   [C.good, 'DB → App', 'Mohan aaya, cache khaali (MISS), App DB se "Ravi K" laaya'],
   [C.good, 'App → Cache', 'Naya naam cache me rakha, ab sabko taaza dikhega']],
  [[C.msg, 'App → DB', 'DB me "Ravi K" likh diya, ye ho gaya'],
   [C.bad, 'App → Cache', 'Cache ka DELETE network ki gadbad se fail, purana "Ravi" baitha'],
   [C.acc, 'TTL', 'Key ki umar 5 min, khatam hote hi khud mitegi, phir DB se naya']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 30, 150, 60, 'Ravi', 'naam badla', { icon: 'user' });
    B(15, 230, 150, 60, 'Mohan', 'profile dekhi', { icon: 'user' });
    B(330, 130, 150, 60, 'App', '', { icon: 'server' });
    B(650, 30, 210, 60, 'DB', 'naam = Ravi K', { st: 'good', icon: 'db' });
    B(650, 230, 210, 60, 'Cache', 'naam = Ravi (purana)', { st: 'bad' });
    A(165, 60, 328, 150, C.msg, { n: 1 }); A(482, 150, 648, 62, C.msg, { n: 2 });
    A(165, 255, 328, 180, C.msg); A(482, 175, 648, 252, C.msg, { n: 3 });
    AP([[755, 292], [755, 335], [90, 335], [90, 292]], C.bad, { n: 4, at: 0.5 }); L(590, 322, 'Mohan ko dikha: "Ravi"', C.bad);
  } else if (ph === 1) {
    B(15, 30, 150, 60, 'Ravi', 'naam badla', { icon: 'user' });
    B(15, 230, 150, 60, 'Mohan', 'profile dekhi', { icon: 'user' });
    B(330, 130, 150, 60, 'App', '', { icon: 'server' });
    B(650, 30, 210, 60, 'DB', 'naam = Ravi K', { st: 'good', icon: 'db' });
    B(650, 230, 210, 60, 'Cache', 'naam = Ravi K (naya)', { st: 'good' });
    A(165, 60, 328, 145, C.msg); A(165, 255, 328, 180, C.msg);
    A(482, 140, 648, 50, C.msg, { n: 1, at: 0.3 }); A(648, 80, 482, 160, C.good, { n: 3, at: 0.25 });
    A(482, 170, 648, 238, C.acc, { n: 2, at: 0.3 }); L(610, 198, 'DEL', C.acc);
    A(482, 188, 648, 270, C.good, { n: 4, at: 0.75 });
    L(450, 330, 'key khaali: jo bhi pehle padhe, DB se naya laaye', C.muted);
  } else {
    B(15, 130, 150, 60, 'App', '', { icon: 'server' });
    B(560, 30, 210, 60, 'DB', 'naam = Ravi K', { st: 'good', icon: 'db' });
    B(560, 200, 210, 60, 'Cache', 'naam = Ravi (purana)', { st: 'bad' });
    A(165, 145, 558, 62, C.msg, { n: 1 });
    A(165, 175, 558, 228, C.bad, { n: 2, dash: 1, at: 0.3 }); cross(420, 210); L(420, 250, 'DEL fail', C.bad);
    B(560, 285, 210, 50, 'TTL 5 min', 'phir khud mitegi', { st: 'acc', icon: 'clock' }); num(800, 310, 3, C.acc);
    L(240, 300, 'pakka chahiye: DB ke log se', C.muted); L(240, 320, 'delete (CDC)', C.muted);
  }
}
