// Card 3: machine B mari, uski key ka bojh DB pe. Ilaaj: replica (async), copy main bane. Agar: network toota -> quorum.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: machine B mar gayi, uski saari key MISS, achanak saara bojh DB pe'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har machine ki ek copy (async). B mari to copy main ban jaati, DB bach gaya'],
  [6, YEL, 'AGAR', 'AGAR network toota, do main? -> zyadatar machine (quorum) tay karti, akela likhna band'],
];
function setup() {}
const STEPS = [
  [[C.bad, 'App → B', 'App ne user:42 B se maanga, par B mar chuki hai'],
   [C.bad, 'App → DB', 'B ki saari key MISS, har request seedha DB pe gayi'],
   [C.bad, 'DB', 'DB pe achanak bojh aaya, DB dheema, sab dheema']],
  [[C.acc, 'B → copy', 'B ki har likhai thodi der baad uski copy pe bhi jaati (async)'],
   [C.bad, 'B', 'B mar gayi, par uska data copy ke paas pehle se hai'],
   [C.good, 'copy', 'Copy ~10-30 sec me naya main ban gayi'],
   [C.good, 'App → copy', 'App ab copy se padhta, HIT, DB tak jaana nahi pada']],
  [[C.bad, 'network', 'Network beech se toota, B ek taraf akela reh gaya'],
   [C.good, 'A, C → copy', 'Doosri taraf zyadatar machine hain, unhone copy ko main chuna'],
   [C.bad, 'B', 'B ke saath zyadatar nahi, wo likhna band karta, do main nahi bante']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 140, 150, 60, 'App', 'get user:42', { icon: 'server' });
    B(330, 30, 140, 50, 'A', 'apni key', { st: 'dim' });
    B(320, 140, 180, 60, 'B', 'user:42 yahan tha', { st: 'bad' }); cross(525, 170);
    B(330, 260, 140, 50, 'C', 'apni key', { st: 'dim' });
    A(165, 170, 318, 170, C.bad, { n: 1 });
    B(650, 140, 160, 60, 'DB', 'dheema', { st: 'bad', icon: 'db' });
    AP([[90, 200], [90, 335], [730, 335], [730, 202]], C.bad, { n: 2, at: 0.5 }); L(560, 322, 'B ki saari key MISS', C.bad);
    bar(650, 220, 160, 14, 1, ''); num(840, 170, 3, C.bad); L(730, 120, 'achanak bojh', C.bad);
  } else if (ph === 1) {
    B(15, 140, 150, 60, 'App', 'get user:42', { icon: 'server' });
    B(330, 40, 160, 60, 'B (main)', 'mar gayi', { st: 'bad' }); cross(515, 70); num(560, 70, 2, C.bad);
    A(410, 102, 410, 218, C.acc, { n: 1, dash: 1 }); L(510, 160, 'har likhai baad me copy pe', C.acc, 'start');
    B(330, 220, 160, 60, 'B ki copy', 'ab main', { st: 'good' }); num(520, 250, 3, C.good); L(545, 255, '~10-30 sec me main', C.good, 'start');
    A(165, 185, 328, 245, C.good, { n: 4 }); L(230, 260, 'HIT', C.good);
    B(700, 40, 150, 60, 'DB', 'shaant', { st: 'good', icon: 'db' });
    L(450, 330, 'asar sirf B ki key pe, aur wo bhi kuch sec', C.muted);
  } else {
    el('line', { x1: 300, y1: 15, x2: 300, y2: 340, stroke: C.bad, 'stroke-width': 3, 'stroke-dasharray': '8 6' }, DYN);
    num(300, 30, 1, C.bad); L(300, 352, 'network toota', C.bad);
    B(50, 140, 180, 60, 'B (purana main)', 'akela', { st: 'bad' }); num(140, 230, 3, C.bad);
    L(140, 260, 'zyadatar saath nahi:', C.bad); L(140, 280, 'likhna band', C.bad);
    B(470, 40, 180, 60, 'B ki copy', 'naya main', { st: 'good' });
    B(380, 250, 140, 55, 'A', 'vote: copy', { st: 'dim' }); B(640, 250, 140, 55, 'C', 'vote: copy', { st: 'dim' });
    A(450, 248, 530, 102, C.good, { n: 2 }); A(710, 248, 600, 102, C.good);
    L(790, 150, 'zyadatar ek taraf', C.good); L(790, 172, '= quorum,', C.good); L(790, 194, 'wahi main chunte', C.good);
  }
}
