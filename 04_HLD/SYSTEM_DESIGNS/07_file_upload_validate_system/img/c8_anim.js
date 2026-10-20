// Card 8: S3 ka pakka link WhatsApp pe gaya, hamesha chalta. Ilaaj: DB me sirf S3 key, 5 min ka signed GET.
// Agar: 5 min me download adhoora, 5 min ke andar link aage gaya. Wapas: 1 ghante baad khola -> expire.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: pakka S3 link WhatsApp group pe, koi bhi kabhi bhi khole'],
  [3, GRN, 'ILAAJ', 'ILAAJ: DB me sirf S3 key. Pehle maalik check, phir naya 5 min link'],
  [6, YEL, 'AGAR', 'AGAR 5 min me download adhoora? -> chalu download beech me nahi katta'],
  [9, YEL, 'AGAR', 'AGAR 5 min ke andar link aage bhej diya? -> ye risk maana hai'],
  [12, GRN, 'WAPAS', 'WAPAS: group me 1 ghante baad khola -> expire, nahi khula'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 150, 110, 70, 'Arpan', '', { st: o.ar });
  B(250, 150, 140, 70, 'Upload Svc', '', { st: o.up });
  B(250, 20, 140, 60, 'DB', s.db || 'sirf S3 key', { st: o.db });
  B(700, 150, 160, 70, 'S3', s.s3 || 'T-101 file', { st: o.s3 });
  if (o.g) B(420, 270, 200, 60, 'WhatsApp group', 'koi bhi', { st: o.g });
}
function frame(ph) {
  if (ph === 0) {
    base({ g: 'bad', db: 'bad', s3: 'bad' }, { db: 'pakka S3 link', s3: 'hamesha khula' });
    AP([[100, 222], [100, 300], [418, 300]], C.bad, { n: 1, at: 0.6 }); L(115, 270, 'pakka link bheja', C.bad, 'start');
    A(620, 290, 698, 222, C.bad, { n: 2, at: 0.5 }); L(680, 280, 'kabhi bhi khole', C.bad, 'start');
    dots(5, 445, 245, C.bad, { gap: 22 });
    T(560, 120, 'file sabke haath, hamesha', { c: C.bad, b: 1, size: 16 });
  } else if (ph === 1) {
    base({ ar: 'good', up: 'good', db: 'good', s3: 'good' });
    A(130, 175, 248, 175, C.msg, { n: 1, at: 0.5 }); L(190, 140, 'download', C.msg);
    A(320, 148, 320, 82, C.good, { n: 2 }); L(335, 120, 'maalik check + S3 key', C.good, 'start');
    A(248, 200, 132, 200, C.good, { n: 3, at: 0.5 }); L(190, 235, 'naya 5 min link', C.good);
    AP([[75, 222], [75, 260], [780, 260], [780, 222]], C.good, { n: 4 }); L(430, 285, 'signed GET se download', C.good);
  } else if (ph === 2) {
    base({ ar: 'good', s3: 'good' });
    AP([[75, 222], [75, 260], [780, 260], [780, 222]], C.good, { n: 1 });
    bar(470, 290, 200, 18, 0.6, '', 1); L(570, 330, 'download chalu', C.good);
    num(560, 120, 2, C.acc); L(578, 125, '5 min khatam', C.acc, 'start');
    L(560, 70, 'hal: umar sirf shuru karne ki hai', C.good);
  } else if (ph === 3) {
    base({ ar: 'good', g: 'acc', s3: 'acc' }, { s3: 'khul gaya' });
    AP([[100, 222], [100, 300], [418, 300]], C.acc, { n: 1, at: 0.6 }); L(115, 270, '5 min wala link', C.acc, 'start');
    A(620, 290, 698, 222, C.acc, { n: 2, at: 0.5 }); L(680, 280, '5 min ke andar', C.acc, 'start');
    L(560, 70, 'hal: risk maana. Zyada sensitive ho to', C.good);
    L(560, 92, 'CloudFront signed URL + IP ki had', C.good);
  } else {
    base({ ar: 'good', g: 'dim', s3: 'good' }, { s3: 'link expire' });
    AP([[100, 222], [100, 300], [418, 300]], C.msg, { n: 1, at: 0.6 }); L(115, 270, '5 min wala link', C.msg, 'start');
    A(620, 290, 698, 222, C.bad, { dash: 1, n: 2, at: 0.3 }); cross(672, 245);
    L(680, 300, '1 ghante baad: nahi khula', C.bad, 'start');
    L(560, 70, 'link aage gaya to bas 5 min tak chalta', C.good);
  }
}
