// Card 4: Virat ka tweet, crore feed DB pe -> DB gira. Ilaaj: hot-tweet cache (RAM), miss pe sirf ek DB jaaye.
// Agar: ek key garam (kai copy), edit / delete (key hatao). Wapas: naya laaya, phir sab RAM se.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: har feed Virat ka tweet DB se laa rahi. Crore read, DB gira'],
  [3, GRN, 'ILAAJ', 'ILAAJ: garam tweet RAM me. Miss pe sirf ek request DB, baaki uska jawab'],
  [6, YEL, 'AGAR', 'AGAR ek key pe bheed, machine garam? -> key ki kai copy, kisi se bhi padho'],
  [9, YEL, 'AGAR', 'AGAR Virat ne edit kiya, cache me purana? -> cache se key hata do'],
  [12, GRN, 'WAPAS', 'WAPAS: agla read DB se naya laaya, phir sab RAM se, DB aaram se'],
];
function setup() {}
function users() {
  for (let i = 0; i < 4; i++) {
    const y = 30 + i * 80;
    B(20, y, 110, 44, `log ${i + 1}`);
    A(130, y + 22, 228, 180, C.msg, { n: i === 1 ? 1 : 0, at: 0.45 });
  }
  B(230, 150, 110, 60, 'Feed Svc');
}
function frame(ph) {
  if (ph === 0) {
    users();
    B(680, 230, 160, 70, 'Tweet DB', '', { st: 'bad' });
    A(340, 190, 678, 260, C.bad, { n: 2, at: 0.3 }); L(530, 270, 'crore read', C.bad);
    dots(5, 470, 205, C.bad, { gap: 30 });
    cross(865, 265); num(735, 325, 3, C.bad); L(755, 330, 'DB gira', C.bad, 'start');
    T(760, 150, 'feed band', { c: C.bad, b: 1, size: 18 });
  } else if (ph === 1) {
    users();
    B(450, 30, 180, 65, 'Hot-tweet cache', 'RAM', { st: 'good' });
    A(340, 165, 448, 75, C.good, { n: 2 }); L(470, 125, 'baaki sab yahin se', C.good, 'start');
    B(680, 230, 160, 70, 'Tweet DB', '', { st: 'good' });
    A(600, 97, 720, 228, C.acc, { dash: 1, n: 3 }); L(700, 165, 'miss: sirf ek (lock)', C.acc, 'start');
    bar(690, 320, 140, 14, 0.1, '', 1);
  } else if (ph === 2) {
    users();
    for (let i = 0; i < 5; i++) {
      const y = 20 + i * 55;
      B(470, y, 150, 42, `tweet:123#${i + 1}`, '', { st: 'good' });
      A(340, 180, 468, y + 21, C.good, { n: i === 2 ? 2 : 0, at: 0.6 });
    }
    B(700, 20, 170, 60, 'ek machine', 'tweet:123', { st: 'bad' });
    bar(710, 95, 150, 14, 1, '');
    num(720, 135, 1, C.bad); L(740, 140, 'garam', C.bad, 'start');
    B(220, 265, 130, 55, 'App memory', '1-2 sec', { st: 'good' });
    A(285, 212, 285, 263, C.good, { n: 3 });
    L(720, 320, 'hal: key ki kai copy, kisi se bhi padho', C.good);
  } else if (ph === 3) {
    B(20, 30, 120, 55, 'Virat', 'tweet edit');
    AP([[140, 57], [760, 57], [760, 228]], C.msg, { n: 1, at: 0.6 }); L(560, 45, 'edit DB me', C.msg);
    B(680, 230, 160, 70, 'Tweet DB', 'naya', { st: 'good' });
    B(430, 140, 180, 65, 'Hot-tweet cache', 'purana', { st: 'bad' });
    num(450, 230, 2, C.acc); L(470, 235, 'key hata do', C.acc, 'start');
    B(230, 150, 110, 60, 'Feed Svc');
    A(340, 175, 428, 172, C.good, { n: 3 }); L(385, 130, 'agla read', C.good);
    A(610, 190, 678, 250, C.good, { n: 4 }); L(760, 330, 'DB se naya', C.good);
    L(240, 320, 'hal: cache se key hata do', C.good);
  } else {
    users();
    B(450, 30, 180, 65, 'Hot-tweet cache', 'naya', { st: 'good' });
    A(720, 228, 600, 97, C.good, { dash: 1, n: 2 }); L(690, 150, 'naya laaya', C.good, 'start');
    A(340, 165, 448, 75, C.good, { n: 3 }); L(470, 125, 'sab RAM se', C.good, 'start');
    B(680, 230, 160, 70, 'Tweet DB', '', { st: 'good' });
    bar(690, 320, 140, 14, 0.1, '', 1); L(665, 295, 'DB aaram se', C.good, 'end');
  }
}
