// Card 2: send dabaya, network gaya, app ne dobara bheja -> 1000 kate. Ilaaj: idempotency key (UNIQUE). Agar: wahi id, amount 5000.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: koi id nahi. Jawab khoya, app ne dobara bheja, server ne do transfer samjhe'],
  [3, GRN, 'ILAAJ', 'ILAAJ: app id banata (T-77), DB me UNIQUE. Wahi id dobara = pehla jawab, paisa nahi katta'],
  [6, YEL, 'AGAR', 'AGAR wahi id, par amount 5000? -> request ka hash nahi mila, 422, purana jawab nahi'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'App → Server', 'Send dabaya: 500 bhejne ki request, bina kisi id ke'],
   [C.bad, 'Server → App', '500 kata, par jawab raaste me kho gaya (network)'],
   [C.bad, 'App → Server', 'Timeout pe app ne dobara bheja, server ne naya samjha: 1000 kate']],
  [[C.acc, 'App', 'Screen khulte hi app ne id banayi: T-77'],
   [C.good, 'App → DB', 'Send: T-77 DB me UNIQUE ke saath likhi, 500 kata'],
   [C.good, 'App → DB', 'Retry me wahi T-77: pehle se hai, pehla jawab wapas, paisa nahi kata']],
  [[C.msg, 'App → Server', 'T-77 ke saath 500 aaya, ho gaya'],
   [C.bad, 'App → Server', 'Phir wahi T-77 aaya, par amount 5000'],
   [C.bad, 'Server', 'Request ka hash pehle wale se nahi mila: 422, paisa nahi kata']],
];
function frame(ph) {
  B(15, 140, 180, 60, 'App', ph === 0 ? 'id nahi' : 'id: T-77', { st: ph === 1 ? 'acc' : '', icon: 'phone' });
  B(360, 140, 180, 60, 'Server', '', { icon: 'server' });
  if (ph === 0) {
    B(690, 140, 190, 60, 'Arpan', '1000 kate', { st: 'bad', icon: 'user' });
    A(195, 150, 358, 150, C.msg, { n: 1 }); A(358, 175, 197, 175, C.bad, { n: 2, dash: 1, at: 0.25 }); cross(240, 175);
    AP([[105, 202], [105, 290], [450, 290], [450, 202]], C.bad, { n: 3, at: 0.4 }); L(280, 320, 'dobara bheja', C.bad);
    A(540, 170, 688, 170, C.bad); L(615, 140, '500 + 500', C.bad);
  } else if (ph === 1) {
    num(105, 115, 1, C.acc);
    B(660, 120, 225, 100, 'DB', '', { st: 'good', icon: 'db' }); cells(705, 165, [['T-77 (UNIQUE)', C.acc]], { w: 160, h: 34 });
    A(195, 160, 358, 160, C.msg); A(540, 160, 658, 150, C.good, { n: 2 });
    AP([[105, 202], [105, 290], [770, 290], [770, 222]], C.good, { n: 3, at: 0.35 }); L(450, 320, 'wahi T-77: pehle se hai, sirf ek baar kata', C.good);
  } else {
    B(660, 120, 225, 100, 'DB', '', { st: 'good', icon: 'db' }); cells(705, 165, [['T-77: 500 (hash)', C.acc]], { w: 165, h: 34 });
    A(195, 155, 358, 155, C.msg, { n: 1 }); A(540, 160, 658, 160, C.msg);
    AP([[105, 202], [105, 290], [450, 290], [450, 202]], C.bad, { n: 2, at: 0.4 }); L(280, 320, 'T-77, amount 5000', C.bad);
    num(450, 110, 3, C.bad); L(450, 80, 'hash alag: 422', C.bad);
  }
}
