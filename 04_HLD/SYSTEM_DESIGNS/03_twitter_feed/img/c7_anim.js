// Card 7: main Bangalore, data US me, har feed 300 ms. Ilaaj: har region me copy, Route 53 paas wala region deta.
// Agar: poora India region gira -> Route 53 US pe palat deta, feed thodi purani par chalti.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: main Bangalore me, saara data US me. Har feed aane-jaane me ~300 ms'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har region me data ki copy. Route 53 paas wale region (India) ka pata deta'],
  [6, YEL, 'AGAR', 'AGAR poora India region gira? -> Route 53 US pe palat deta. Feed thodi purani, par chalti'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'Arpan → US region', 'India me baithe Arpan ki request samundar paar US tak gayi'], [C.bad, 'US region → Arpan', 'Jawab wapas aane me har feed pe ~300 ms lage']],
  [[C.msg, 'Arpan → Route 53', 'Arpan ke phone ne Route 53 se poocha site kahan hai'], [C.acc, 'Route 53 → Arpan', 'Route 53 ne paas wale India region ka pata diya'], [C.good, 'Arpan → India region', 'Request India region gayi, sirf ~30 ms me jawab'], [C.muted, 'US region → India region', 'US ka data India me copy hota, thoda peeche chalta']],
  [[C.bad, 'Route 53 → India region', 'Route 53 ne /health poocha, India region ne jawab nahi diya'], [C.acc, 'Route 53 → Arpan', 'Route 53 ne ab Arpan ko US region ka pata diya'], [C.good, 'Arpan → US region', 'Thoda dheema aur feed thodi purani, par app chalti rahi']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 150, 60, 'Arpan', 'Bangalore');
    B(680, 130, 180, 80, 'US region', 'saara data', { st: 'bad' });
    A(170, 160, 678, 160, C.bad, { n: 1 }); A(678, 185, 172, 185, C.bad, { n: 2 });
    L(425, 145, 'samundar paar jaana', C.bad); L(425, 215, 'wapas aana', C.bad);
    T(425, 270, '~300 ms har feed', { c: C.bad, b: 1, size: 16 });
    L(450, 330, 'itni door har baar jaana-aana, feed dheemi', C.bad);
  } else if (ph === 1) {
    B(20, 140, 130, 60, 'Arpan', 'Bangalore');
    B(260, 30, 150, 60, 'Route 53', 'paas kaun?', { st: 'acc' });
    B(520, 160, 170, 70, 'India region', 'data ki copy', { st: 'good' });
    B(730, 30, 150, 70, 'US region', 'data');
    A(110, 138, 258, 55, C.msg, { n: 1 }); L(110, 75, 'site kahan?', C.msg);
    A(310, 92, 152, 168, C.acc, { n: 2, at: 0.5 }); L(270, 150, 'India', C.acc, 'start');
    A(150, 185, 518, 195, C.good, { n: 3 }); L(330, 215, '~30 ms', C.good);
    A(805, 102, 690, 175, C.muted, { n: 4, dash: 1 }); L(800, 160, 'copy peeche', C.muted, 'start');
    L(450, 330, 'paas se do, tez. Copy thodi purani ho sakti (2 sec), feed me chalega', C.good);
  } else {
    B(20, 140, 130, 60, 'Arpan', 'Bangalore');
    B(260, 30, 150, 60, 'Route 53', 'health check', { st: 'acc' });
    B(520, 160, 170, 70, 'India region', '', { st: 'bad' }); cross(715, 195);
    B(730, 30, 150, 70, 'US region', 'zinda', { st: 'good' });
    A(410, 70, 518, 175, C.bad, { n: 1, dash: 1 }); L(470, 105, 'jawab nahi', C.bad, 'start');
    A(258, 85, 152, 165, C.acc, { n: 2, at: 0.6 }); L(270, 135, 'ab US', C.acc, 'start');
    AP([[85, 202], [85, 300], [805, 300], [805, 102]], C.good, { n: 3, at: 0.5 }); L(450, 325, 'US se, thoda dheema, feed thodi purani', C.good);
  }
}
