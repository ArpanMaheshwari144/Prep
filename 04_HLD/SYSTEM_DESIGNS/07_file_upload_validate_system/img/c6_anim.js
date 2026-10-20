// Card 6: folder (1000 file), har file ka alag id, folder ka haal nahi. Ilaaj: parent P-1 + child id, done_count.
// Agar: ek file FAILED (PARTIAL), do worker ek saath +1. Wapas: fail wali dobara, P-1 DONE.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 1000 file, har file ka alag id. Folder ka koi haal nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: folder P-1, har file child. Child DONE pe done_count +1'],
  [6, YEL, 'AGAR', 'AGAR folder ki koi file FAILED? -> P-1 PARTIAL, sirf wahi dobara'],
  [9, YEL, 'AGAR', 'AGAR do worker ek saath done_count +1? -> ek UPDATE, ginti nahi khoti'],
  [12, GRN, 'WAPAS', 'WAPAS: fail wali file dobara DONE -> sab DONE, P-1 DONE'],
];
function setup() {}
function kids(c, mid) {
  cells(360, 172, [['file 1', c], ['file 2', mid || c], ['...', C.muted], ['file 1000', c]], { w: 58 });
}
function base(o = {}, s = {}) {
  B(20, 150, 110, 70, 'Phone', 'folder: 1000', { st: o.ph });
  B(180, 150, 130, 70, 'Upload Svc', s.up || '', { st: o.up });
  if (!o.noP) B(690, 150, 180, 70, 'P-1 (folder)', s.p || '', { st: o.p });
  if (!o.noW) B(410, 275, 140, 60, 'Worker', '', { st: o.wk });
}
function frame(ph) {
  if (ph === 0) {
    base({ noP: 1, noW: 1, ph: 'bad' });
    A(130, 185, 178, 185, C.msg, { n: 1, at: 0.45 });
    A(310, 185, 358, 185, C.msg); kids(C.msg);
    num(75, 125, 2, C.bad); L(93, 130, '"folder ho gaya?"', C.bad, 'start');
    L(480, 230, '1000 id khud dekhne padte', C.bad);
    B(690, 150, 180, 70, 'folder ka haal', 'koi nahi', { st: 'bad' });
    T(780, 260, 'ANDHA', { c: C.bad, b: 1, size: 20 });
  } else if (ph === 1) {
    base({ ph: 'good', up: 'good', p: 'good', wk: 'good' }, { up: 'batch init', p: 'done_count: 1000' });
    A(130, 175, 178, 175, C.msg, { n: 1, at: 0.45 });
    A(178, 200, 132, 200, C.good, { n: 2, at: 0.5 }); L(75, 250, '1000 URL + child id', C.good);
    A(310, 185, 358, 185, C.line); kids(C.good);
    A(480, 273, 480, 200, C.good, { n: 3 }); L(495, 245, 'child DONE', C.good, 'start');
    A(550, 305, 760, 222, C.good, { n: 4, at: 0.5 }); L(670, 300, '+1', C.good, 'start');
    L(780, 130, 'sab DONE to P-1 DONE', C.good);
  } else if (ph === 2) {
    base({ p: 'acc', wk: 'acc' }, { p: 'PARTIAL' });
    A(310, 185, 358, 185, C.line); kids(C.good, C.bad);
    num(451, 150, 1, C.bad); L(469, 155, 'FAILED', C.bad, 'start');
    A(550, 305, 760, 222, C.acc, { n: 2, at: 0.5 }); L(670, 300, 'ek FAILED', C.acc, 'start');
    AP([[451, 273], [451, 202]], C.good, { n: 3 }); L(436, 255, 'sirf wahi dobara', C.good, 'end');
    L(780, 130, 'hal: poora folder nahi', C.good);
  } else if (ph === 3) {
    base({ noW: 1, p: 'good' }, { p: 'done_count + 1' });
    B(420, 30, 140, 60, 'Worker 1', '', { st: 'acc' });
    B(420, 275, 140, 60, 'Worker 2', '', { st: 'acc' });
    A(560, 70, 688, 165, C.acc, { n: 1, at: 0.4 }); A(560, 295, 688, 205, C.acc, { n: 1, at: 0.4 });
    L(612, 60, '+1', C.acc, 'start'); L(612, 320, '+1', C.acc, 'start');
    L(780, 250, 'DB ke andar, ginti nahi khoti', C.good);
    L(780, 130, 'hal: ek hi UPDATE', C.good);
    L(20, 345, 'UPDATE done_count = done_count + 1', C.good, 'start');
  } else {
    base({ ph: 'good', p: 'good', wk: 'good' }, { p: 'DONE (1000)' });
    A(310, 185, 358, 185, C.line); kids(C.good);
    AP([[451, 273], [451, 202]], C.good, { n: 1 }); L(436, 255, 'dobara DONE', C.good, 'end');
    A(550, 305, 760, 222, C.good, { n: 2, at: 0.5 }); L(670, 300, '+1', C.good, 'start');
    num(780, 125, 3, C.good); L(798, 130, 'P-1 DONE', C.good, 'start');
  }
}
