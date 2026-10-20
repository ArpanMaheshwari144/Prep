// Card 9: naam report.pdf, andar EXE. Ilaaj: worker pehle 4 byte padhta (magic bytes), reject.
// Agar: shuru PDF par andar virus (scan), fail file ka kya (quarantine). Wapas: asli PDF -> DONE.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: naam report.pdf, type application/pdf, andar virus.exe'],
  [3, GRN, 'ILAAJ', 'ILAAJ: worker pehle 4 byte padhta. "MZ" = EXE, "%PDF" nahi -> reject'],
  [6, YEL, 'AGAR', 'AGAR shuru PDF, andar virus? -> magic bytes sirf type, scan bhi chahiye'],
  [9, YEL, 'AGAR', 'AGAR jaanch me fail hui file? -> quarantine bucket (download band)'],
  [12, GRN, 'WAPAS', 'WAPAS: asli report.pdf: "%PDF" + scan pass -> DONE'],
];
function setup() {}
function base(o = {}, s = {}) {
  B(20, 150, 110, 70, 'Phone', 'report.pdf', { st: o.ph });
  B(190, 150, 120, 70, 'S3', 'tmp/', { st: o.s3 });
  B(380, 150, 150, 70, 'Worker', s.wk || 'pehle 4 byte', { st: o.wk });
  B(700, 30, 170, 60, 'DB', s.db || 'haal', { st: o.db });
  B(700, 260, 170, 60, s.rj || 'reject', s.rjs || '', { st: o.rj });
  if (o.sc) B(380, 275, 150, 60, 'Scan', 'ClamAV / S3 malware', { st: o.sc });
  A(130, 185, 188, 185, C.line); A(310, 185, 378, 185, C.line);
}
function mz() { cells(380, 95, [['M', C.bad], ['Z', C.bad], ['.', C.muted], ['.', C.muted]], { w: 34 }); }
function pdf() { cells(380, 95, [['%', C.good], ['P', C.good], ['D', C.good], ['F', C.good]], { w: 34 }); }
function frame(ph) {
  if (ph === 0) {
    base({ ph: 'bad', wk: 'bad', db: 'bad', rj: 'dim' }, { wk: 'naam dekha: .pdf', db: 'DONE (virus.exe)' });
    num(75, 125, 1, C.bad); L(93, 130, 'client ne bola: PDF', C.bad, 'start');
    L(250, 245, 'andar virus.exe', C.bad);
    A(310, 185, 378, 185, C.msg, { n: 2 });
    A(530, 165, 698, 60, C.bad, { n: 3, at: 0.5 }); L(600, 140, 'DONE', C.bad, 'start');
    T(615, 230, 'JHOOTH pe bharosa', { c: C.bad, b: 1, size: 16 });
  } else if (ph === 1) {
    base({ wk: 'good', rj: 'bad' }, { rjs: 'EXE' });
    mz(); num(390, 75, 2, C.acc); L(408, 80, 'pehle 4 byte: "MZ" = EXE', C.acc, 'start');
    A(310, 185, 378, 185, C.msg, { n: 1 });
    A(530, 205, 698, 290, C.bad, { n: 3, at: 0.5 }); L(600, 270, 'reject', C.bad, 'end');
    A(530, 165, 698, 60, C.line, { dash: 1 }); cross(665, 81);
    L(640, 60, 'DONE nahi', C.muted, 'end');
  } else if (ph === 2) {
    base({ wk: 'good', sc: 'bad', rj: 'bad' }, { wk: 'type: PDF', rjs: 'virus' });
    pdf(); num(560, 108, 1, C.good); L(578, 113, '"%PDF": type theek', C.good, 'start');
    A(455, 222, 455, 273, C.msg, { n: 2 }); L(470, 255, 'scan', C.msg, 'start');
    num(365, 290, 3, C.bad); L(350, 295, 'andar virus mila', C.bad, 'end');
    A(530, 300, 698, 300, C.bad, { n: 4, at: 0.7 });
    L(200, 340, 'hal: magic bytes + scan, dono', C.good);
  } else if (ph === 3) {
    base({ wk: 'bad', rj: 'acc' }, { wk: 'jaanch fail', rj: 'quarantine', rjs: 'bucket' });
    A(530, 205, 698, 290, C.acc, { n: 1, at: 0.5 }); L(600, 270, 'yahan rakho', C.acc, 'end');
    B(700, 140, 170, 60, 'download', '', { st: 'dim' });
    A(785, 258, 785, 202, C.bad, { dash: 1 }); cross(830, 230); num(745, 230, 2, C.bad);
    L(785, 345, 'download band', C.bad);
    L(200, 300, 'hal: baad me delete, ya security wale dekhein', C.good);
  } else {
    base({ ph: 'good', wk: 'good', sc: 'good', db: 'good', rj: 'dim' }, { db: 'DONE' });
    pdf(); num(365, 108, 1, C.good); L(350, 113, '"%PDF"', C.good, 'end');
    A(455, 222, 455, 273, C.msg, { n: 2 }); L(470, 255, 'scan: saaf', C.good, 'start');
    A(530, 165, 698, 60, C.good, { n: 3, at: 0.75 }); L(640, 125, 'DONE', C.good, 'start');
  }
}
