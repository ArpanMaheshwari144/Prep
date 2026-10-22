// Card 3: dono ne sabse upar ek saath type kiya. Ilaaj: OT, kram server ka, op khiskao. Agar: beech me mera pending op.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: dono ne pos 0 pe ek saath likha. Jo aaya wahi laga diya, do alag doc ban gaye'],
  [3, GRN, 'ILAAJ', 'ILAAJ: kram server ka (OT): X v8. Y ne X nahi dekha, to 0 → 1 (v9). XYHELLO'],
  [6, YEL, 'AGAR', 'AGAR Ravi ka op baad me aaya, maine beech me likh diya? -> app apna pending op khiskata'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Main → Ravi', 'Maine pos 0 pe "X" likha, apne paas lagaya aur Ravi ko bheja'],
   [C.msg, 'Ravi → Main', 'Usi pal Ravi ne pos 0 pe "Y" likha aur mujhe bheja'],
   [C.bad, 'dono screen', 'Jo aaya wahi 0 pe laga: mere paas YXHELLO, Ravi ke paas XYHELLO']],
  [[C.msg, 'Main → Server', 'Maine pos 0 pe "X" bheja. Mere paas doc v7 tha'],
   [C.msg, 'Ravi → Server', 'Usi pal Ravi ne bhi pos 0 pe "Y" bheja, wo bhi v7 pe'],
   [C.acc, 'Server', 'X pehle aaya: v8, doc XHELLO. Y v7 pe bana tha, X dekha hi nahi, isliye 0 → 1 khiska (v9)'],
   [C.good, 'Server → dono', 'Server ne yahi kram dono ko bheja. Dono ki screen XYHELLO, same']],
  [[C.acc, 'Main', 'Maine end pe "Z" likha (pos 5), server ka jawab abhi nahi aaya'],
   [C.msg, 'Server → Main', 'Beech me Ravi ka "X" pos 0 pe aaya (v8)'],
   [C.good, 'Main ka app', 'App ne X lagaya, apna pending Z pos 5 se 6 khiskaya: XHELLOZ']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 30, 200, 60, 'Main', 'X @0', { icon: 'user' });
    B(15, 255, 200, 60, 'Ravi', 'Y @0', { icon: 'user' });
    A(85, 92, 85, 253, C.msg, { n: 1 }); A(150, 253, 150, 92, C.msg, { n: 2 });
    T(430, 45, 'meri screen:', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(430, 60, [['Y', C.bad], ['X', C.bad], ['HELLO']], { w: 70, h: 34 });
    T(430, 255, 'Ravi ki screen:', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(430, 270, [['X', C.bad], ['Y', C.bad], ['HELLO']], { w: 70, h: 34 });
    num(470, 175, 3, C.bad); L(650, 180, 'do alag doc ban gaye', C.bad);
  } else if (ph === 1) {
    B(15, 30, 200, 60, 'Main', '"X" @0, v7', { icon: 'user' });
    B(15, 255, 200, 60, 'Ravi', '"Y" @0, v7', { icon: 'user' });
    B(300, 30, 330, 270, 'Server', '', { st: 'good' });
    A(215, 60, 298, 110, C.msg, { n: 1, at: 0.45 }); A(215, 285, 298, 230, C.msg, { n: 2, at: 0.45 });
    num(605, 60, 3, C.acc);
    T(320, 112, 'v8: X pehle aaya, 0 pe laga', { c: C.good, b: 1, size: 14, a: 'start' });
    cells(320, 124, [['X', C.good], ['HELLO']], { w: 64, h: 34 });
    T(320, 196, 'v9: Y v7 pe bana, X dekha nahi', { c: C.acc, b: 1, size: 14, a: 'start' });
    cells(320, 208, [['X'], ['Y', C.acc], ['HELLO']], { w: 64, h: 34 });
    L(578, 230, 'Y: 0 → 1', C.acc);
    L(465, 280, 'X ne 0 le liya, Y ek aage', C.acc);
    A(632, 170, 700, 170, C.good, { n: 4, at: 0.5 });
    T(705, 125, 'dono ke paas:', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(705, 152, [['X', C.good], ['Y', C.good], ['HELLO']], { w: 56, h: 34 });
    L(450, 330, 'kram sirf server ka, phone ki ghadi kabhi nahi', C.muted);
  } else {
    B(15, 130, 210, 65, 'Main ka app', 'pending: Z @5', { st: 'acc', icon: 'user' }); num(245, 110, 1, C.acc);
    B(15, 270, 210, 55, 'Server', 'Ravi ka X @0 = v8', { icon: 'server' });
    A(120, 268, 120, 197, C.msg, { n: 2 });
    T(430, 45, 'pehle (mera app):', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(430, 60, [['HELLO'], ['Z @5', C.acc]], { w: 100, h: 34 });
    A(530, 100, 530, 205, C.good, { n: 3 }); L(690, 155, 'X laga, Z khiska', C.good);
    T(430, 225, 'ab:', { c: C.muted, b: 1, size: 14, a: 'start' });
    cells(430, 240, [['X', C.good], ['HELLO'], ['Z @6', C.acc]], { w: 100, h: 34 });
    L(580, 320, 'dono ka likha bacha, Z sahi jagah', C.good);
  }
}
