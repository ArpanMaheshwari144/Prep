// Card 6: folder (1000 file), har file alag id, folder ka haal nahi. Ilaaj: parent P-1 + child, done_count. Agar: ek FAILED.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: poora folder daala (1000 file). Har file ka alag id, folder ka haal pata nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: folder ka id P-1, har file uska child. Child DONE pe ginti +1, sab DONE to P-1 DONE'],
  [6, YEL, 'AGAR', 'AGAR folder ki koi file FAILED? -> P-1 PARTIAL, sirf wahi dobara'],
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
  [[C.msg, 'App → Upload Svc', '1000 file, har file alag'], [C.acc, 'Upload Svc → 1000 id', 'har file ka apna id,|folder ka koi id nahi'], [C.bad, 'Upload Svc → App', '"folder ho gaya?" ka jawab nahi:|1000 id khud dekho']],
  [[C.acc, 'Upload Svc → App', 'P-1 + 1000 URL, ek baar me'], [C.good, 'Worker → Status DB', 'C-7: DONE'], [C.acc, 'Worker → P-1', 'done_count +1|(ek UPDATE, DB ke andar)'], [C.good, 'P-1 → App', '1000 / 1000: P-1 DONE.|Ek poochh, poora haal']],
  [[C.acc, 'P-1 → App', 'PARTIAL: 999 DONE, C-42 FAILED'], [C.msg, 'App → Upload Svc', 'sirf C-42 dobara'], [C.good, 'Upload Svc → Worker', 'ek file jaanch, 999 nahi'], [C.good, 'Worker → P-1', 'C-42 DONE: P-1 DONE']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 150, 70, 'Arpan ka app', 'folder: 1000 file');
    B(330, 130, 170, 70, 'Upload Svc', '');
    B(620, 60, 260, 170, '1000 alag id', '', { st: 'bad' });
    cells(640, 105, [['T-1', C.acc], ['T-2', C.acc], ['...', C.line], ['T-1000', C.acc]], { w: 55, h: 30 });
    A(170, 150, 328, 150, C.msg, { n: 1 }); L(250, 135, '1000 file', C.msg);
    A(500, 165, 618, 165, C.acc, { n: 2 }); L(560, 150, 'id', C.acc);
    A(328, 185, 172, 185, C.bad, { n: 3 }); L(250, 210, 'jawab nahi', C.bad);
    L(750, 255, '1000 id khud dekho', C.bad);
    L(450, 330, 'folder ka koi haal hi nahi, sirf 1000 tukde', C.bad);
  } else if (ph === 1) {
    panel(0, 'batch init', ['Arpan ka app', 'P-1 + 1000 URL', 'good'], ['Upload Svc', 'P-1, child C-1..', 'acc'], 'diya', false, C.acc, 'ek baar me');
    panel(1, 'ek file DONE', ['Worker', 'C-7 jaancha', 'good'], ['Status DB', 'C-7: DONE', 'good'], 'likha', true, C.good, 'child ka haal');
    panel(2, 'ginti', ['Worker', 'C-7 DONE', ''], ['P-1', 'done 999 / 1000', 'acc'], '+1', true, C.acc, 'ek UPDATE, DB ke andar');
    panel(3, 'sab DONE', ['Arpan ka app', 'P-1: DONE', 'good'], ['P-1', '1000 / 1000', 'good'], 'ek poochh', false, C.good, 'ek id, poora haal');
    L(450, 320, 'folder ka haal = bachchon ka jod', C.good);
  } else {
    B(20, 40, 170, 70, 'Arpan ka app', 'P-1: PARTIAL', { st: 'acc' });
    B(330, 30, 280, 110, 'P-1 (folder)', '', { st: 'acc' });
    cells(345, 75, [['999 DONE', C.good], ['C-42 FAILED', C.bad]], { w: 120, h: 32 });
    B(20, 240, 170, 70, 'Upload Svc', '');
    B(700, 240, 170, 70, 'Worker', 'C-42 jaancha', { st: 'good' });
    A(328, 75, 192, 75, C.acc, { n: 1 }); L(260, 60, 'PARTIAL', C.acc);
    A(105, 112, 105, 238, C.msg, { n: 2 }); L(120, 180, 'sirf C-42 dobara', C.msg, 'start');
    A(190, 275, 698, 275, C.good, { n: 3, at: 0.25 }); L(445, 262, 'ek file, 999 nahi', C.good);
    A(785, 238, 612, 120, C.good, { n: 4 }); L(735, 165, 'P-1: DONE', C.good, 'start');
    L(450, 345, 'ek file fail hui to sirf wahi dobara, poora folder nahi', C.good);
  }
}
