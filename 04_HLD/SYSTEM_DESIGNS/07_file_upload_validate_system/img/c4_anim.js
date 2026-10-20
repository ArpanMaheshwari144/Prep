// Card 4: app crash, /complete nahi aaya, T-103 UPLOADING pe atka. Ilaaj: sweeper job.
// Agar: /complete aur S3 event dono chhoote, sweeper do machine pe. Wapas: T-103 line me, DONE.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: app crash, "complete" nahi aaya. T-103 UPLOADING pe atka'],
  [3, GRN, 'ILAAJ', 'ILAAJ: sweeper: 10 min se haal same -> bytes S3 me hain to queue me'],
  [6, YEL, 'AGAR', 'AGAR /complete aur S3 event dono chhoot gaye? -> isi liye sweeper'],
  [9, YEL, 'AGAR', 'AGAR sweeper do machine pe chal gaya? -> ek hi chale (lock / leader)'],
  [12, GRN, 'WAPAS', 'WAPAS: T-103 dobara line me, worker ne haal dekh ke jaancha -> DONE'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 30, 110, 60, 'Phone', s.ph || '', { st: o.ph });
  B(220, 30, 140, 60, 'Upload Svc', '', { st: o.up });
  B(220, 255, 140, 60, 'S3', s.s3 || 'T-103 bytes', { st: o.s3 });
  B(450, 140, 170, 70, 'DB', s.db || 'T-103: UPLOADING', { st: o.db });
  if (!o.noSw) B(450, 265, 170, 60, 'Sweeper', s.sw || 'har 5 min', { st: o.sw });
  B(720, 140, 140, 70, 'Kafka', s.kf || 'line', { st: o.kf });
  B(720, 20, 140, 60, 'Worker', s.wk || '', { st: o.wk });
}
function frame(ph) {
  if (ph === 0) {
    base({ noSw: 1, ph: 'bad', db: 'bad', s3: 'acc' }, { ph: 'crash' });
    num(75, 115, 1, C.bad); L(93, 120, 'crash', C.bad, 'start'); cross(160, 60);
    A(130, 60, 218, 60, C.bad, { dash: 1 });
    num(290, 115, 2, C.bad); L(308, 120, '"complete" nahi aaya', C.bad, 'start');
    num(535, 235, 3, C.bad); L(553, 240, 'hamesha atka, koi dekhta nahi', C.bad, 'start');
    L(290, 340, 'bytes pade, paisa lag raha', C.acc);
  } else if (ph === 1) {
    base({ db: 'acc', sw: 'good', kf: 'good', s3: 'good' }, { db: 'T-103: 10 min same' });
    A(535, 263, 535, 212, C.good, { n: 1 }); L(520, 245, 'atki file?', C.good, 'end');
    A(448, 295, 362, 285, C.good, { n: 2 }); L(405, 330, 'bytes hain?', C.good);
    A(620, 290, 718, 200, C.good, { n: 3, at: 0.5 }); L(690, 290, 'dobara line me', C.good, 'start');
    L(535, 120, 'jaanch 3 sec ki, 10 min se same = atki', C.acc);
    L(20, 160, 'bytes nahi -> FAILED, user dobara daale', C.muted, 'start');
  } else if (ph === 2) {
    base({ ph: 'bad', db: 'acc', sw: 'good', kf: 'good' });
    A(130, 60, 218, 60, C.bad, { dash: 1 }); cross(175, 60); num(175, 110, 1, C.bad);
    L(160, 140, '/complete nahi', C.bad);
    AP([[362, 300], [400, 300], [400, 345], [790, 345], [790, 212]], C.bad, { dash: 1 });
    cross(790, 295); num(760, 320, 1, C.bad); L(745, 325, 'S3 event nahi', C.bad, 'end');
    A(535, 263, 535, 212, C.good, { n: 2 });
    A(620, 290, 718, 200, C.good, { n: 3, at: 0.5 });
    L(535, 120, 'hal: sweeper har kuch min atki file dhoondhta', C.good);
  } else if (ph === 3) {
    base({ noSw: 1, db: 'acc', kf: 'acc', wk: 'good' }, { kf: 'T-103, T-103', wk: 'haal dekh ke skip' });
    B(450, 265, 120, 60, 'Sweeper 1', '', { st: 'acc' });
    B(590, 265, 120, 60, 'Sweeper 2', '', { st: 'acc' });
    A(510, 263, 740, 212, C.acc, { n: 1, at: 0.3 }); A(650, 263, 760, 212, C.acc, { n: 1, at: 0.5 });
    A(790, 138, 790, 82, C.msg, { n: 2 }); L(805, 115, 'doosri: skip', C.good, 'start');
    L(20, 140, 'hal: ek hi chale (lock / leader),', C.good, 'start');
    L(20, 162, 'ya chalne do: dobara daalna safe', C.good, 'start');
  } else {
    base({ db: 'good', kf: 'good', wk: 'good', sw: '' }, { db: 'T-103: DONE', kf: 'T-103' });
    A(620, 290, 718, 200, C.good, { n: 1, at: 0.5 }); L(690, 290, 'dobara line me', C.good, 'start');
    A(790, 138, 790, 82, C.msg, { n: 2 }); L(805, 115, 'jaancha', C.msg, 'start');
    A(718, 60, 622, 160, C.good, { n: 3, at: 0.5 }); L(640, 90, 'DONE likha', C.good, 'end');
  }
}
