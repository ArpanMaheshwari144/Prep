// Card 2: push, email, SMS ek hi line me. SMS company dheemi, worker atka, push + email bhi atke.
// Ilaaj: har raaste ki alag line + worker. Agar: SMS line lambi -> sirf SMS worker badhao.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: push, email, SMS ek hi line me. SMS company dheemi, worker wahin atka, sab atke'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har raaste ki apni line aur apna worker. SMS ruka, push + email chalte rahe'],
  [6, YEL, 'AGAR', 'AGAR SMS ki line lambi ho gayi? -> sirf SMS ke worker badhao, baaki ko mat chhuo'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'Ek line → Worker', 'SMS sabse aage'], [C.bad, 'Worker → SMS company', '30 sec / msg, worker atka'], [C.bad, 'push, email', 'SMS ke peeche khade']],
  [[C.good, 'Ghatna → 3 line', 'har raaste ki apni line aur worker.|SMS dheema, push + email aage']],
  [[C.good, 'Ghatna → 3 line', 'sirf SMS worker 1 -> 4.|Push aur email ko nahi chhua']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 110, 380, 100, 'Ek hi line', '', { st: 'bad' });
    cells(35, 155, [['SMS', C.bad], ['push', C.muted], ['email', C.muted], ['push', C.muted], ['email', C.muted]], { w: 66, h: 34 });
    B(470, 125, 140, 70, 'Worker', 'SMS pe atka', { st: 'bad' });
    B(700, 125, 170, 70, 'SMS company', '30 sec / msg', { st: 'bad' });
    A(400, 160, 468, 160, C.bad, { n: 1 }); A(610, 160, 698, 160, C.bad, { n: 2, dash: 1 });
    num(240, 245, 3, C.bad); L(240, 270, 'push aur email SMS ke peeche khade', C.bad);
    L(450, 330, 'ek dheema raasta sabko rok deta', C.bad);
  } else {
    const more = ph === 2;
    const R = [['SMS line', 'SMS worker' + (more ? ' x 4' : ''), 'SMS company', more ? 'acc' : 'bad', 'dheemi hi hai'],
               ['push line', 'push worker', 'phone', 'good', 'pahuncha'],
               ['email line', 'email worker', 'Email company', 'good', 'pahuncha']];
    B(20, 140, 120, 60, 'Ghatna', 'order#9');
    R.forEach(([l, w, d, st, s], i) => {
      const y = 25 + i * 110;
      B(220, y, 150, 60, l, '', { st: i ? 'good' : (more ? 'acc' : 'bad') });
      B(440, y, 170, 60, w, '', { st });
      B(690, y, 170, 60, d, s, { st });
      A(140, 170, 218, y + 30, C.msg, { n: i ? 0 : 1, at: 0.5 });
      A(370, y + 30, 438, y + 30, st === 'good' ? C.good : C.acc);
      A(610, y + 30, 688, y + 30, st === 'good' ? C.good : C.acc);
    });
    L(450, 355, more ? 'sirf SMS worker 1 -> 4. Push aur email pe koi asar nahi' : 'SMS dheema, par push aur email apni line me aage nikal gaye', C.good);
  }
}
