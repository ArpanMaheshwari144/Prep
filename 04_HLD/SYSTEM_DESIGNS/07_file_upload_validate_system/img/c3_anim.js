// Card 3: 4.5 GB pe wifi gaya, ek lamba upload bekaar. Ilaaj: multipart, har tukde ki rasid (ETag).
// Agar: app band, kal wapas (ListParts); COMPLETE aaya hi nahi (lifecycle). Wapas: bache tukde + COMPLETE.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: ek lamba upload, 4.5 GB pe wifi gaya. Phir 0 se 5 GB'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 100 tukde, har tukde ki rasid (ETag). Wifi gaya to sirf 91 - 100'],
  [6, YEL, 'AGAR', 'AGAR app band, kal wapas aaya? -> S3 se poochho (ListParts U-9)'],
  [9, YEL, 'AGAR', 'AGAR COMPLETE aaya hi nahi? -> S3 lifecycle adhoore tukde hataye'],
  [12, GRN, 'WAPAS', 'WAPAS: bache 91 - 100 gaye, phir COMPLETE -> poori file S3 pe'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 150, 110, 70, 'Phone', s.ph || '5 GB', { st: o.ph });
  B(690, 150, 180, 70, 'S3', s.s3 || '', { st: o.s3 });
}
// 10 dabbe = 100 tukde (har dabba 10 tukde); upto tak hara, baaki rang c
function parts(upto, c, y) {
  const it = [];
  for (let i = 1; i <= 10; i++) it.push([String(i * 10), i * 10 <= upto ? C.good : c]);
  cells(150, y, it, { w: 46, h: 26 });
}
function frame(ph) {
  if (ph === 0) {
    base({ ph: 'bad', s3: 'bad' }, { s3: '' });
    bar(170, 180, 480, 26, 0.9, '');
    T(410, 170, 'ek lamba upload: 4.5 GB', { c: C.acc, size: 13 });
    A(130, 193, 168, 193, C.acc);
    num(612, 245, 1, C.bad); L(630, 250, 'wifi gaya', C.bad, 'start'); cross(665, 193);
    num(410, 290, 2, C.bad); L(410, 320, 'sab bekaar, phir 0 se 5 GB', C.bad);
  } else if (ph === 1) {
    base({ ph: 'good', s3: 'good' }, { s3: 'rasid: 1 - 90' });
    parts(90, C.msg, 80);
    AP([[75, 148], [75, 93], [148, 93]], C.good, { n: 1 }); L(150, 68, 'kai tukde saath', C.good, 'start');
    AP([[648, 93], [780, 93], [780, 148]], C.good, { n: 2, at: 0.4 }); L(870, 68, 'har tukde ki rasid', C.good, 'end');
    num(400, 190, 3, C.bad); L(418, 195, 'wifi gaya', C.bad, 'start');
    A(130, 205, 688, 205, C.msg, { n: 4, at: 0.3 }); L(410, 240, 'sirf bache 91 - 100', C.msg);
  } else if (ph === 2) {
    base({ ph: 'acc', s3: 'good' }, { ph: 'kal wapas', s3: 'rasid: 1 - 90' });
    num(75, 125, 1, C.acc); L(93, 130, 'app band tha', C.acc, 'start');
    A(130, 170, 688, 170, C.msg, { n: 2, at: 0.3 }); L(410, 160, 'ListParts U-9', C.msg);
    A(688, 205, 132, 205, C.good, { n: 3, at: 0.3 }); L(410, 235, 'rasid: 1 - 90', C.good);
    parts(90, C.msg, 260);
    L(410, 320, 'hal: rasid S3 pe hai, app pe nahi', C.good);
  } else if (ph === 3) {
    base({ ph: 'bad', s3: 'acc' }, { ph: 'gaya', s3: 'adhoore tukde' });
    B(690, 280, 180, 60, 'S3 lifecycle', 'kuch din (jaise 7)', { st: 'good' });
    parts(90, C.bad, 80);
    A(130, 205, 688, 205, C.bad, { dash: 1 }); cross(410, 205); num(380, 245, 1, C.bad);
    L(398, 250, 'COMPLETE nahi aaya', C.bad, 'start');
    A(780, 278, 780, 222, C.good, { n: 2 }); L(765, 255, 'abort, hata diya', C.good, 'end');
    L(410, 320, 'hal: lifecycle adhoore uploads ke bytes khud hataata', C.good);
  } else {
    base({ ph: 'good', s3: 'good' }, { s3: 'poori file' });
    parts(100, C.good, 80);
    A(130, 170, 688, 170, C.good, { n: 1, at: 0.3 }); L(410, 160, '91 - 100', C.good);
    A(130, 205, 688, 205, C.good, { n: 2, at: 0.3 }); L(410, 235, 'COMPLETE', C.good);
    L(410, 300, 'user ko shuru se kuch nahi', C.good);
  }
}
