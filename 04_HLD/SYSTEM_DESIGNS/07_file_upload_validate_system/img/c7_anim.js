// Card 7: Ravi ne T-101 daal ke meri file le li. Ilaaj: har request pe owner check, 403.
// Agar: id random thi (leak hoti), admin maange. Wapas: Arpan ko file mili.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Ravi ne GET /download/T-101 kiya, service ne file de di'],
  [3, GRN, 'ILAAJ', 'ILAAJ: har request pe maalik check. JWT ravi, T-101 arpan ka -> 403'],
  [6, YEL, 'AGAR', 'AGAR id to random tha, phir check kyun? -> id leak hoti (screenshot)'],
  [9, YEL, 'AGAR', 'AGAR admin maange to? -> owner ho ya admin role ho, warna 403'],
  [12, GRN, 'WAPAS', 'WAPAS: Arpan ne maanga, owner_id == jwt.user_id -> file mili'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 40, 110, 60, s.top || 'Ravi', s.rv || 'JWT: ravi', { st: o.rv });
  B(20, 270, 110, 60, 'Arpan', 'JWT: arpan', { st: o.ar });
  B(220, 150, 130, 70, 'Gateway', 'JWT: kaun', { st: o.gw });
  B(430, 150, 150, 70, 'Upload Svc', s.up || 'owner check', { st: o.up });
  B(690, 30, 180, 70, 'DB', 'T-101 maalik: arpan', { st: o.db });
  B(690, 260, 180, 70, 'S3', 'T-101 file', { st: o.s3 });
}
function frame(ph) {
  if (ph === 0) {
    base({ rv: 'bad', ar: 'dim', up: 'bad', s3: 'bad' }, { up: 'sirf id dekhi' });
    A(130, 75, 218, 165, C.msg, { n: 1, at: 0.5 }); L(285, 245, 'GET /download/T-101', C.msg);
    A(350, 185, 428, 185, C.msg, { n: 2 });
    A(580, 205, 688, 290, C.bad, { n: 3, at: 0.5 }); L(600, 270, 'file lo', C.bad, 'end');
    A(428, 160, 132, 58, C.bad, { n: 4, at: 0.5 }); L(330, 85, 'file de di', C.bad, 'start');
    T(505, 120, 'CHORI', { c: C.bad, b: 1, size: 20 });
  } else if (ph === 1) {
    base({ rv: 'bad', ar: 'dim', gw: 'good', up: 'good', db: 'good' }, { rv: '403' });
    A(130, 75, 218, 165, C.msg, { n: 1, at: 0.5 });
    A(350, 185, 428, 185, C.msg, { n: 2 }); L(390, 240, 'JWT: ravi', C.msg);
    A(580, 165, 688, 75, C.good, { n: 3, at: 0.5 }); L(655, 140, 'maalik? arpan', C.good, 'start');
    A(428, 160, 132, 58, C.bad, { n: 4, at: 0.5 }); L(330, 85, '403', C.bad, 'start');
    A(580, 205, 688, 290, C.line, { dash: 1 }); cross(634, 248);
  } else if (ph === 2) {
    base({ rv: 'bad', ar: 'acc', gw: 'good', up: 'good', db: 'good' }, { rv: '403' });
    A(60, 268, 60, 102, C.acc, { n: 1 }); L(75, 190, 'screenshot me T-101', C.acc, 'start');
    A(130, 75, 218, 165, C.msg, { n: 2, at: 0.5 });
    A(350, 185, 428, 185, C.msg, { n: 3 });
    A(428, 160, 132, 58, C.bad, { n: 4, at: 0.5 }); L(330, 85, '403', C.bad, 'start');
    L(505, 330, 'hal: owner check, id leak ho tab bhi file nahi', C.good);
  } else if (ph === 3) {
    base({ rv: 'good', ar: 'dim', gw: 'good', up: 'good', db: 'good', s3: 'good' },
      { top: 'Admin', rv: 'role: admin', up: 'owner ya admin?' });
    A(130, 75, 218, 165, C.msg, { n: 1, at: 0.5 });
    A(350, 185, 428, 185, C.msg, { n: 2 });
    A(580, 165, 688, 75, C.good, { n: 3, at: 0.5 }); L(655, 140, 'admin role hai', C.good, 'start');
    A(580, 205, 688, 290, C.good, { n: 4, at: 0.5 }); L(600, 270, 'file', C.good, 'end');
    L(505, 345, 'hal: na owner na admin -> 403', C.good);
  } else {
    base({ rv: 'dim', ar: 'good', gw: 'good', up: 'good', db: 'good', s3: 'good' });
    A(130, 295, 218, 205, C.msg, { n: 1, at: 0.5 });
    A(350, 185, 428, 185, C.msg, { n: 2 });
    A(580, 165, 688, 75, C.good, { n: 3, at: 0.5 }); L(655, 140, 'owner_id == jwt.user_id', C.good, 'start');
    A(428, 210, 132, 310, C.good, { n: 4, at: 0.5 }); L(330, 300, 'file mili', C.good, 'start');
  }
}
