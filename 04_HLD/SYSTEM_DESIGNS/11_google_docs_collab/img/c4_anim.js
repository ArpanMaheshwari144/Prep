// Card 4: 50 lakh connection, kai server chahiye. Ilaaj: docId se ring, ek doc = ek owner server. Agar: owner server gira.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: kai server, LB ne editor alag server pe bheje. OT kaun kare? do version, doc toota'],
  [3, GRN, 'ILAAJ', 'ILAAJ: docId se ring: doc:123 ka owner server-1. Dono editor wahin, OT ek jagah'],
  [6, YEL, 'AGAR', 'AGAR owner server gira? -> doc DB me hai. Ring agla server deta, wo DB se doc banata'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'LB → server-1', 'Ek server ~1 lakh connection jhelta, to kai hain; mujhe server-1 mila'],
   [C.msg, 'LB → server-3', 'Ravi ko server-3 mila, jabki dono ek hi doc:123 pe hain'],
   [C.bad, 'server-1, server-3', 'Dono apna OT karte: server-1 pe v8 = X, server-3 pe v8 = Y, doc toota']],
  [[C.msg, 'Main, Ravi → LB', 'Dono ne doc:123 khola, LB docId dekhta hai'],
   [C.good, 'LB → server-1', 'Ring pe doc:123 ka owner server-1, dono editor wahin bheje'],
   [C.good, 'server-1', 'OT ek hi jagah, ek kram, ek version, doc same']],
  [[C.bad, 'server-1', 'doc:123 ka owner server-1 gir gaya'],
   [C.acc, 'LB → server-2', 'Ring pe agla server-2 naya owner, editor ab yahin jude'],
   [C.good, 'DB → server-2', 'Doc DB me tha: server-2 ne wahin se banaya, editor version bata ke baaki op le lete']],
];
function frame(ph) {
  const srv = (st, subs) => ['server-1', 'server-2', 'server-3'].forEach((n, i) => B(520, 25 + i * 112, 180, 60, n, subs[i], { st: st[i], icon: 'server' }));
  B(15, 40, 150, 60, 'Main', 'doc:123', { icon: 'user' });
  B(15, 250, 150, 60, 'Ravi', 'doc:123', { icon: 'user' });
  if (ph === 0) {
    B(250, 145, 120, 60, 'LB', 'koi bhi');
    A(165, 70, 248, 160, C.msg); A(165, 280, 248, 190, C.msg);
    srv(['bad', 'dim', 'bad'], ['v8 = X', '', 'v8 = Y']);
    A(370, 165, 518, 60, C.msg, { n: 1 }); A(370, 185, 518, 280, C.msg, { n: 2 });
    num(730, 170, 3, C.bad); L(800, 205, 'do version', C.bad); L(800, 225, 'doc toota', C.bad);
  } else if (ph === 1) {
    B(230, 145, 160, 60, 'LB', 'docId -> ring');
    A(165, 70, 228, 160, C.msg, { n: 1 }); A(165, 280, 228, 190, C.msg);
    srv(['good', 'dim', 'dim'], ['doc:123 owner', 'doosre doc', 'doosre doc']);
    A(390, 165, 518, 60, C.good, { n: 2 });
    num(730, 55, 3, C.good); L(800, 90, 'OT ek jagah', C.good);
    L(450, 340, 'viewer kahin bhi jud sakta, editor owner pe', C.muted);
  } else {
    B(230, 145, 160, 60, 'LB', 'docId -> ring');
    A(165, 70, 228, 160, C.msg); A(165, 280, 228, 190, C.msg);
    srv(['bad', 'good', 'dim'], ['gir gaya', 'naya owner', '']); cross(720, 55); num(760, 55, 1, C.bad);
    A(390, 175, 518, 170, C.acc, { n: 2 });
    B(720, 250, 175, 60, 'DB', 'doc:123 ke op', { st: 'good', icon: 'db' });
    A(780, 248, 702, 185, C.good, { n: 3 }); L(810, 340, 'doc nahi khota', C.good);
  }
}
