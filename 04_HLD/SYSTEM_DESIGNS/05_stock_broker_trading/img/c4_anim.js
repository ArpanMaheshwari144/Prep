// Card 4: 9:15 bheed, har order lock + DB, jam. Ilaaj: TCS ki ek line, ek thread RAM me. Agar: badhaoge kaise. Wapas.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 9:15 baje sab TCS pe toot pade, har order pe lock + DB, jam'],
  [3, GRN, 'ILAAJ', 'ILAAJ: TCS ki ek line, ek thread RAM me tez nipta deta. ACCEPTED turant'],
  [6, YEL, 'AGAR', 'AGAR baantoge nahi, to badhaoge kaise? -> stocks ke beech: TCS, INFY alag'],
  [9, GRN, 'WAPAS', 'WAPAS: line lambi thi, par order khoya nahi. Sauda hua to FILLED'],
];
function setup() {}
function crowd(c) { dots(20, 30, 115, c, { cols: 4, gap: 22 }); T(65, 240, 'hazaaron / sec', { c: C.muted, size: 12 }); }
function frame(ph) {
  if (ph === 0) {
    crowd(C.msg);
    A(115, 160, 258, 160, C.msg, { n: 1, at: 0.4 }); L(185, 148, '9:15', C.msg);
    B(260, 120, 150, 80, 'Order Svc', 'har order: lock', { st: 'bad' });
    A(410, 160, 558, 160, C.bad, { n: 2, at: 0.2 }); L(495, 148, 'lock + DB', C.bad);
    dots(5, 440, 190, C.bad, { gap: 22, hollow: 1 });
    B(560, 120, 150, 80, 'DB', 'ek-ek, dheema', { st: 'bad' });
    num(760, 160, 3, C.bad); T(805, 168, 'jam', { c: C.bad, b: 1, size: 24 });
  } else if (ph === 1) {
    crowd(C.good);
    A(115, 150, 218, 150, C.msg, { n: 1, at: 0.4 });
    cells(220, 138, [['', C.msg], ['', C.msg], ['', C.msg], ['', C.msg], ['', C.msg]], { w: 30, h: 26 });
    T(290, 128, 'TCS line', { c: C.muted, size: 12 });
    A(220, 190, 117, 190, C.good, { n: 2, at: 0.4 }); L(170, 215, 'ACCEPTED turant', C.good);
    A(392, 151, 488, 151, C.good, { n: 3, at: 0.3 }); L(445, 139, 'RAM me', C.good);
    B(490, 115, 150, 75, 'TCS thread', 'ek hi', { st: 'good' });
    A(640, 152, 718, 152, C.good, { n: 4, at: 0.3 });
    B(720, 115, 150, 75, 'TCS book', '');
    L(795, 215, 'sauda hua to FILLED', C.good);
  } else if (ph === 2) {
    B(20, 50, 120, 60, 'TCS bheed', ''); B(20, 220, 120, 60, 'INFY bheed', '');
    A(140, 80, 218, 80, C.msg, { n: 2 }); A(140, 250, 218, 250, C.msg, { n: 2 });
    cells(220, 67, [['', C.msg], ['', C.msg], ['', C.msg], ['', C.msg]], { w: 30, h: 26 });
    cells(220, 237, [['', C.msg], ['', C.msg], ['', C.msg], ['', C.msg]], { w: 30, h: 26 });
    T(288, 58, 'TCS line', { c: C.muted, size: 12 }); T(288, 228, 'INFY line', { c: C.muted, size: 12 });
    A(358, 80, 478, 80, C.good, { n: 3, at: 0.3 }); A(358, 250, 478, 250, C.good, { n: 3, at: 0.3 });
    B(480, 45, 150, 70, 'TCS thread', 'thread 1', { st: 'good' });
    B(480, 215, 150, 70, 'INFY thread', 'thread 2', { st: 'good' });
    B(720, 45, 150, 70, 'TCS thread 2?', '', { st: 'dim' }); cross(885, 80);
    AP([[340, 66], [340, 25], [795, 25], [795, 43]], C.bad, { dash: 1, n: 1, at: 0.75 });
    L(765, 140, 'TCS ke andar baanta: toota', C.bad);
    L(450, 335, 'hal: baanto stocks ke beech, TCS ek thread, INFY doosra', C.good);
  } else {
    crowd(C.good);
    A(115, 150, 218, 150, C.msg);
    cells(220, 138, [['', C.good], ['', C.good]], { w: 30, h: 26 });
    T(255, 128, 'TCS line', { c: C.muted, size: 12 });
    A(290, 151, 488, 151, C.good, { n: 1, at: 0.5 }); L(390, 139, 'sab nipte', C.good);
    B(490, 115, 150, 75, 'TCS thread', 'ek hi', { st: 'good' });
    A(640, 152, 718, 152, C.good, { n: 2, at: 0.3 }); L(680, 139, 'sauda', C.good);
    B(720, 115, 150, 75, 'TCS book', '');
    AP([[795, 192], [795, 300], [65, 300], [65, 252]], C.good, { n: 3, at: 0.3 }); L(470, 288, 'FILLED, koi order khoya nahi', C.good);
  }
}
