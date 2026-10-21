// Card 9: C2 mar gaya, lag badhta ja raha. Ilaaj: rebalance + kharab khabar DLQ. Agar: lag kaise naapoge.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: shaam ko C2 mar gaya. Uske tukde P4-6 koi nahi padh raha, lag badhta ja raha'],
  [3, GRN, 'ILAAJ', 'ILAAJ: C2 ke tukde baaki ko (rebalance): P4, P5 C1 ko, P6 C3 ko. Baar-baar fail = DLQ'],
  [6, YEL, 'AGAR', 'AGAR lag naapna hai? -> lag = aakhri offset - group kahan tak padha. Badhe to alert'],
];
function setup() {}
const STEPS = [
  [[C.good, 'P0-3, P7-9 → C1, C3', 'Baaki tukde apne consumer se padhe ja rahe'],
   [C.bad, 'C2', 'Shaam ko C2 mar gaya, P4-6 ka padhne wala koi nahi'],
   [C.bad, 'P4-6', 'Khabar aati rahi, padhi nahi gayi: lag badhta gaya']],
  [[C.acc, 'P4, P5 → C1', 'Rebalance: C2 ke tukde baaki ko, P4 aur P5 C1 ko'],
   [C.acc, 'P6 → C3', 'P6 C3 ko mila, koi tukda bina padhne wale ke nahi'],
   [C.bad, 'C3 → DLQ', 'Ek khabar 3 baar fail hui, alag line (DLQ) me daali, tukda nahi ruka']],
  [[C.msg, 'P4', 'P4 me aakhri khabar ka offset 1000'],
   [C.acc, 'group', 'Group ne P4 me 400 tak padha'],
   [C.bad, 'lag', 'Lag = 1000 - 400 = 600; badhta jaaye to alert, consumer badhao']],
];
function frame(ph) {
  if (ph === 0) {
    const P = [['P0-3', 'dim'], ['P4-6', 'bad'], ['P7-9', 'dim']], ys = [30, 140, 250];
    P.forEach(([p, st], i) => B(20, ys[i], 170, 60, p, i === 1 ? 'lag badhta' : '', { st }));
    bar(20, 207, 170, 12, 1, '');
    ['C1', 'C2', 'C3'].forEach((c, i) => B(560, ys[i], 200, 60, c, i === 1 ? 'mar gaya' : 'padh raha', { st: i === 1 ? 'bad' : 'good', icon: 'server' }));
    cross(785, 170); num(820, 170, 2, C.bad);
    A(190, 60, 558, 60, C.good, { n: 1 }); A(190, 280, 558, 280, C.good);
    A(190, 170, 558, 170, C.bad, { dash: 1, at: 0.3 }); num(105, 245, 3, C.bad);
    L(380, 330, 'P4-6 ka koi padhne wala nahi', C.bad);
  } else if (ph === 1) {
    const P = [['P0-3', 15], ['P4, P5', 95], ['P6', 175], ['P7-9', 255]];
    P.forEach(([p, y], i) => B(20, y, 160, 55, p, '', { st: i === 1 || i === 2 ? 'acc' : 'dim' }));
    B(560, 30, 200, 60, 'C1', 'P0-3 + P4, P5', { st: 'good', icon: 'server' });
    B(560, 140, 200, 60, 'C2', 'mar gaya', { st: 'bad', icon: 'server' }); cross(785, 170);
    B(560, 250, 200, 60, 'C3', 'P7-9 + P6', { st: 'good', icon: 'server' });
    A(180, 42, 558, 52, C.muted); A(180, 122, 558, 72, C.acc, { n: 1 });
    A(180, 202, 558, 265, C.acc, { n: 2 }); A(180, 282, 558, 285, C.muted);
    B(330, 300, 160, 50, 'DLQ', 'kharab', { st: 'bad', icon: 'queue' });
    A(558, 300, 492, 320, C.bad, { n: 3 });
  } else {
    B(20, 130, 170, 60, 'P4', 'tukda', { st: 'acc' });
    cells(230, 140, [['...'], ['400', C.good], ['...'], ['1000', C.msg]], { w: 110, h: 40 });
    num(615, 125, 1, C.msg); L(615, 205, 'aakhri: 1000', C.msg);
    num(340, 125, 2, C.acc); L(340, 205, 'group yahan tak: 400', C.acc);
    B(250, 250, 400, 70, 'lag = 1000 - 400 = 600', 'badhta jaaye to alert', { st: 'bad', icon: 'clock' }); num(680, 285, 3, C.bad);
  }
}
