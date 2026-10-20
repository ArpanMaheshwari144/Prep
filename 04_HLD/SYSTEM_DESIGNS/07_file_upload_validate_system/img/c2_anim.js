// Card 2: 5 GB server ke raaste S3 (bytes do baar). Ilaaj: signed URL, phone seedha S3.
// Agar: 50 GB bheje, /complete aaya hi nahi. Wapas: /complete T-102 -> jaanch ki line.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 5 GB phone -> server -> S3. Bytes do baar, thread minuton atka'],
  [3, GRN, 'ILAAJ', 'ILAAJ: server sirf signed URL (10 min) + T-102. Phone 5 GB seedha S3'],
  [6, YEL, 'AGAR', 'AGAR had 5 GB, koi 50 GB bhej de? -> had paas me, S3 khud reject'],
  [9, YEL, 'AGAR', 'AGAR "/complete" aaya hi nahi? -> S3 event se queue me daalo'],
  [12, GRN, 'WAPAS', 'WAPAS: phone ne "/complete T-102" bola -> T-102 jaanch ki line me'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 150, 110, 70, 'Phone', s.ph || '5 GB video', { st: o.ph });
  B(340, 20, 160, 70, 'Upload Svc', s.up || '', { st: o.up });
  B(700, 150, 160, 70, 'S3', s.s3 || '', { st: o.s3 });
  if (o.q) B(700, 280, 160, 60, 'Kafka', s.q || 'jaanch ki line', { st: o.q });
}
function frame(ph) {
  if (ph === 0) {
    base({ up: 'bad', s3: 'acc' }, { up: 'thread atka' });
    A(130, 160, 338, 60, C.bad, { n: 1, at: 0.45 }); L(200, 90, '5 GB server pe', C.bad);
    A(500, 60, 698, 160, C.bad, { n: 2, at: 0.5 }); L(650, 90, 'wahi 5 GB phir S3', C.bad, 'start');
    T(420, 230, 'bytes do baar', { c: C.bad, b: 1, size: 22 });
    L(420, 262, 'mehnga bhi, dheema bhi', C.bad);
  } else if (ph === 1) {
    base({ ph: 'good', up: 'good', s3: 'good' }, { up: 'bytes ko haath nahi', s3: 'T-102: 5 GB' });
    A(130, 155, 338, 45, C.msg, { n: 1, at: 0.5 }); L(205, 82, 'init', C.msg, 'end');
    A(338, 75, 132, 180, C.good, { n: 2, at: 0.5 }); L(260, 150, 'signed URL (10 min) + T-102', C.good, 'start');
    A(130, 200, 698, 200, C.good, { n: 3, w: 4 }); L(415, 230, '5 GB seedha S3', C.good);
  } else if (ph === 2) {
    base({ ph: 'bad', up: 'good', s3: 'good' }, { ph: '50 GB', up: 'paas me had 5 GB', s3: 'had 5 GB' });
    A(130, 200, 698, 200, C.bad, { dash: 1, n: 1, at: 0.3 }); L(300, 230, '50 GB bheje', C.bad);
    cross(670, 200); num(640, 250, 2, C.bad); L(658, 255, 'S3 khud reject', C.bad, 'start');
    L(450, 300, 'hal: POST me content-length-range, PUT me size sign hota', C.good);
    L(450, 322, 'S3 event pe size phir se dekh lo', C.good);
  } else if (ph === 3) {
    base({ ph: 'acc', up: '', s3: 'good', q: 'good' }, { s3: 'T-102: aa gayi' });
    A(130, 200, 698, 200, C.good, { n: 1 }); L(415, 230, '5 GB pahunchi', C.good);
    A(130, 160, 338, 60, C.bad, { dash: 1 }); cross(240, 110); num(205, 90, 2, C.bad);
    L(190, 60, '/complete nahi aaya', C.bad, 'end');
    A(780, 222, 780, 278, C.good, { n: 3 }); L(760, 255, 'S3 event: file aa gayi', C.good, 'end');
    L(560, 330, 'hal: client pe bharosa nahi', C.good, 'end');
  } else {
    base({ ph: 'good', up: 'good', s3: 'good', q: 'good' }, { s3: 'T-102: 5 GB', q: 'T-102' });
    A(130, 160, 338, 60, C.msg, { n: 1, at: 0.45 }); L(200, 90, '/complete T-102', C.msg);
    AP([[500, 70], [600, 70], [600, 310], [698, 310]], C.good, { n: 2, at: 0.5 });
    L(615, 170, 'jaancho', C.good, 'start');
    L(415, 230, 'bytes pehle hi S3 me', C.muted);
  }
}
