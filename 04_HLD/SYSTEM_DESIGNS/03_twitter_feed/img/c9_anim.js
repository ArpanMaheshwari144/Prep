// Card 9: padhna + likhna ek service me, highlights ki bheed me mera tweet dheema. Ilaaj: alag service, ALB path se.
// Agar: feed ki bheed badhi, Suresh ke 5,000 follower. Wapas: bheed Timeline pe, mera tweet turant.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: padhna, likhna ek service me. Crore log scroll, mera tweet dheema'],
  [3, GRN, 'ILAAJ', 'ILAAJ: likhne aur padhne ki alag service. ALB path se baante'],
  [6, YEL, 'AGAR', 'AGAR feed ki bheed badh gayi? -> sirf Timeline badhao, Tweet Svc pe nahi'],
  [9, YEL, 'AGAR', 'AGAR Suresh ke 5,000 follower? -> Follow store se 1000-1000 padh ke likho'],
  [12, GRN, 'WAPAS', 'WAPAS: bheed Timeline pe, mera tweet Tweet Svc pe turant save'],
];
function setup() {}
function left() {
  B(20, 30, 120, 70, 'Crore log', 'GET /feed');
  B(20, 260, 120, 70, 'Main', 'POST /tweet');
  B(220, 155, 100, 60, 'ALB');
}
function svcs(tst) {
  B(470, 30, 200, 65, 'Timeline Svc x 20', 'GET /feed');
  B(470, 265, 200, 65, 'Tweet Svc x 2', 'POST /tweet', { st: tst });
}
function frame(ph) {
  if (ph === 0) {
    left();
    A(140, 65, 218, 170, C.msg, { n: 1 });
    A(140, 295, 218, 200, C.acc, { n: 2 });
    B(470, 140, 180, 85, 'Ek service', 'padhna + likhna', { st: 'bad' });
    A(320, 185, 468, 185, C.msg, { n: 3 });
    dots(4, 345, 220, C.msg, { gap: 22 }); dot([435, 220], C.acc);
    L(400, 255, 'mera tweet bheed ki line me', C.acc);
    num(690, 183, 4, C.bad); L(710, 188, 'tweet dheema', C.bad, 'start');
  } else if (ph === 1) {
    left(); svcs('good');
    A(140, 65, 218, 170, C.msg, { n: 1 }); A(140, 295, 218, 200, C.acc, { n: 1 });
    A(320, 175, 468, 62, C.msg, { n: 2 }); A(320, 195, 468, 297, C.good, { n: 2 });
    T(690, 62, 'bheed sirf yahan', { c: C.muted, size: 13, a: 'start' });
    L(690, 302, 'turant save', C.good, 'start');
  } else if (ph === 2) {
    left();
    num(80, 125, 1, C.bad); L(100, 130, 'bheed badhi', C.bad, 'start');
    A(140, 65, 218, 170, C.msg);
    ['Timeline Svc', 'Timeline Svc', 'Timeline (naya)'].forEach((s, i) =>
      B(470, 15 + i * 55, 200, 45, s, '', { st: i === 2 ? 'acc' : 'good' }));
    A(320, 175, 468, 92, C.good, { n: 2, at: 0.55 }); A(320, 175, 468, 37, C.good); A(320, 175, 468, 147, C.good);
    B(470, 265, 200, 65, 'Tweet Svc x 2', 'POST /tweet');
    A(320, 195, 468, 297, C.line);
    L(780, 300, 'koi asar nahi', C.muted);
    L(785, 200, 'hal: sirf Timeline badhao', C.good);
  } else if (ph === 3) {
    B(20, 150, 120, 60, 'Suresh', '5,000 follower');
    num(640, 50, 1, C.bad); L(660, 55, 'ek baar me sab: bhaari', C.bad, 'start');
    B(200, 150, 110, 60, 'Fanout');
    A(140, 180, 198, 180, C.msg, { n: 2 });
    B(400, 20, 200, 60, 'Follow store');
    A(255, 148, 398, 50, C.msg);
    A(500, 82, 500, 143, C.acc, { n: 3 }); L(515, 118, '1000-1000 padho', C.acc, 'start');
    cells(360, 148, [['1-1000', C.acc], ['..2000', C.acc], ['..3000', C.acc], ['..4000', C.acc], ['..5000', C.acc]], { w: 90, h: 28 });
    B(400, 260, 300, 60, '5,000 inbox', '', { st: 'good' });
    A(550, 180, 550, 258, C.good, { n: 4 }); L(565, 225, 'inbox likho', C.good, 'start');
    L(220, 300, 'hal: 1000-1000 padh ke likho', C.good);
  } else {
    left(); svcs('good');
    A(140, 65, 218, 170, C.msg); A(320, 175, 468, 62, C.msg);
    A(140, 295, 218, 200, C.acc, { n: 1 }); A(320, 195, 468, 297, C.good, { n: 2 });
    T(690, 62, 'bheed yahan', { c: C.muted, size: 13, a: 'start' });
    L(690, 302, 'turant save', C.good, 'start');
  }
}
