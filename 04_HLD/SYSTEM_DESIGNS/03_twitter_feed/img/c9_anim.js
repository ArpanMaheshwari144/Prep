// Card 9: ek hi service me padhna + likhna, highlights ki bheed me mera tweet dheema. Ilaaj: alag service,
// ALB path se baant-ta, jahan bheed wahi badhao. Agar: feed ki bheed badhi -> sirf Timeline badhao.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: padhna + likhna ek hi service me. Crore log scroll, mera tweet usi line me dheema'],
  [3, GRN, 'ILAAJ', 'ILAAJ: likhne aur padhne ki alag service. ALB path se baant-ta'],
  [6, YEL, 'AGAR', 'AGAR feed ki bheed aur badh gayi? -> sirf Timeline Svc badhao, Tweet Svc ko chhuo mat'],
];
function setup() {}
function frame(ph) {
  if (ph === 0) {
    B(20, 40, 150, 70, 'Crore log', 'GET /feed', { st: 'bad' });
    B(20, 230, 150, 60, 'Arpan', 'POST /tweet');
    B(400, 130, 200, 80, 'Ek service', 'sab yahin', { st: 'bad' });
    A(170, 75, 398, 155, C.bad, { n: 1 }); A(170, 260, 398, 190, C.acc, { n: 2 });
    num(500, 245, 3, C.bad); L(520, 250, 'Arpan ka tweet line me atka', C.bad, 'start');
    L(450, 330, 'padhne ki bheed ne likhna bhi rok diya', C.bad);
  } else {
    const more = ph === 2;
    B(20, 40, 150, 70, 'Crore log', 'GET /feed', { st: 'acc' });
    B(20, 230, 150, 60, 'Arpan', 'POST /tweet', { st: 'good' });
    B(280, 130, 110, 70, 'ALB', 'path dekho');
    B(560, 20, 290, 110, more ? 'Timeline Svc x 40' : 'Timeline Svc x 20', 'feed padhti', { st: more ? 'acc' : 'good' });
    B(560, 220, 290, 80, 'Tweet Svc x 2', 'tweet likhti', { st: 'good' });
    A(170, 75, 278, 150, C.acc, { n: 1 }); A(170, 260, 278, 185, C.good, { n: 2 });
    A(390, 150, 558, 75, C.acc, { n: 3 }); L(470, 95, '/feed', C.acc);
    A(390, 185, 558, 260, C.good, { n: 4 }); L(470, 245, '/tweet', C.good);
    if (more) L(705, 155, 'sirf yahi badhaya (20 -> 40)', C.acc);
    L(450, 340, more ? 'Tweet Svc pe koi asar nahi, Arpan ka tweet turant' : 'ek ki bheed doosre ko nahi rokti', C.good);
  }
}
