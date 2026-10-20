// Card 9: naam report.pdf, andar EXE. Ilaaj: magic bytes (pehle 4 byte). Agar: PDF par andar virus -> scan bhi.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Ravi ne file daali: naam report.pdf, type application/pdf. Andar virus.exe nikla'],
  [3, GRN, 'ILAAJ', 'ILAAJ: worker pehle 4 byte padhta: "%PDF" to PDF, "MZ" to EXE, reject (magic bytes)'],
  [6, YEL, 'AGAR', 'AGAR shuru PDF, andar virus? -> magic bytes sirf type, scan bhi chahiye'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
const STEPS = [
  [[C.msg, 'Ravi → Worker', 'Ravi ki file aayi: naam report.pdf, type pdf'], [C.bad, 'Worker → Status DB', 'Worker ne naam pe bharosa karke DONE likh diya'], [C.bad, 'Status DB → Arpan', 'Arpan ne DONE dekh ke download kiya, andar EXE nikla']],
  [[C.acc, 'S3 → Worker', 'Worker ne S3 se file ke pehle 4 byte padhe'], [C.good, 'Worker → Status DB', 'Shuru me "MZ" tha matlab EXE, worker ne REJECT likha']],
  [[C.acc, 'Worker → Scanner', 'Shuru "%PDF" sahi tha, ab file virus scan me gayi'], [C.bad, 'Scanner', 'Scanner ko file ke andar virus mila'], [C.bad, 'Scanner → Quarantine', 'File quarantine me daal di, uska download band'], [C.good, 'Scanner → Status DB', 'Status DB me DONE nahi, REJECT likha gaya']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 170, 70, 'Ravi', 'report.pdf', { st: 'bad' });
    B(330, 130, 200, 70, 'Worker', 'naam pe bharosa', { st: 'bad' });
    B(680, 40, 190, 70, 'Status DB', 'T-55: DONE', { st: 'bad' });
    B(680, 230, 190, 70, 'Arpan', 'khola: virus', { st: 'bad' });
    A(190, 165, 328, 165, C.msg, { n: 1 }); L(260, 150, '"pdf" bola', C.msg);
    A(530, 150, 678, 80, C.bad, { n: 2 }); L(585, 95, 'pass', C.bad, 'end');
    A(775, 112, 775, 228, C.bad, { n: 3 }); L(790, 170, 'download', C.bad, 'start');
    T(430, 280, 'andar ke pehle byte: MZ ... (EXE)', { c: C.fg, size: 13 });
    L(450, 330, 'naam aur type client ka bola hua hai, sach nahi', C.bad);
  } else if (ph === 1) {
    B(20, 120, 160, 90, 'S3', 'T-55 ke bytes');
    B(330, 120, 200, 90, 'Worker', 'pehle 4 byte', { st: 'acc' });
    B(680, 120, 190, 90, 'Status DB', 'T-55: REJECT', { st: 'good' });
    A(180, 145, 328, 145, C.acc, { n: 1 }); L(255, 130, 'padha', C.acc);
    A(530, 165, 678, 165, C.good, { n: 2 }); L(605, 150, 'MZ = EXE', C.good);
    cells(240, 250, [['%PDF = PDF: aage', C.good], ['MZ = EXE: reject', C.bad]], { w: 200, h: 34 });
    L(450, 330, 'sach bytes me hai. Naam kuch bhi ho, pehle byte jhooth nahi bolte', C.good);
  } else {
    B(20, 130, 150, 70, 'Worker', '%PDF: type theek', { st: 'good' });
    B(300, 130, 200, 70, 'Scanner', 'ClamAV / S3 malware', { st: 'acc' });
    B(640, 40, 230, 70, 'Quarantine bucket', 'download band', { st: 'bad' });
    B(640, 230, 230, 70, 'Status DB', 'T-56: REJECT', { st: 'good' });
    A(170, 165, 298, 165, C.acc, { n: 1 }); L(235, 150, 'scan', C.acc);
    num(320, 105, 2, C.bad); L(340, 110, 'virus mila', C.bad, 'start');
    A(500, 150, 638, 80, C.bad, { n: 3 }); L(550, 95, 'file yahan', C.bad, 'end');
    A(500, 185, 638, 260, C.good, { n: 4 }); L(550, 250, 'DONE nahi', C.good, 'end');
    L(450, 340, 'magic bytes sirf type batate, virus scan batata', C.good);
  }
}
