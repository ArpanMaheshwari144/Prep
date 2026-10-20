// Card 3: 4.5 GB chadh gayi, wifi gaya, shuru se. Ilaaj: multipart, rasid S3 pe. Agar: app band, kal -> ListParts.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 4.5 GB chadh chuki thi, wifi chala gaya. Ek lamba upload, ab shuru se 5 GB'],
  [3, GRN, 'ILAAJ', 'ILAAJ: 100 tukde, har tukde ki rasid (ETag) S3 pe. Wifi gaya to sirf bache tukde'],
  [6, YEL, 'AGAR', 'AGAR app band, kal wapas aaya? -> S3 se poochho (ListParts U-9)'],
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
  [[C.msg, 'Phone → S3', 'ek lamba upload, 4.5 GB chadha'], [C.bad, 'wifi gaya', 'upload beech me toota'], [C.bad, 'Phone → S3', 'phir 0 se poore 5 GB']],
  [[C.good, 'Phone → S3', 'tukde 1-90, har tukde ki|rasid (ETag) S3 pe'], [C.bad, 'wifi gaya', 'beech me toota'], [C.acc, 'Phone → S3', 'sirf bache 91-100 bheje'], [C.good, 'Phone → S3', 'COMPLETE: S3 ne tukde jode']],
  [[C.acc, 'Phone → S3', 'kal app khola: ListParts U-9'], [C.good, 'S3 → Phone', '1-90 pahunch chuke'], [C.good, 'Phone → S3', 'sirf 91-100. Rasid S3 pe thi,|phone pe nahi']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 160, 70, 'Arpan ka phone', 'video 5 GB');
    B(560, 110, 310, 110, 'S3', '', { st: 'bad' });
    bar(580, 160, 270, 14, 0.9, ''); T(715, 200, '4.5 GB, adhoora', { c: C.bad, size: 12 });
    A(180, 150, 558, 150, C.msg, { n: 1 }); L(370, 135, 'ek lamba upload', C.msg);
    num(370, 175, 2, C.bad); L(390, 180, 'wifi gaya', C.bad, 'start'); cross(330, 175);
    A(180, 200, 558, 200, C.bad, { n: 3, dash: 1 }); L(370, 225, 'phir 0 se 5 GB', C.bad);
    L(450, 320, '90% pe toota, aur sab bekaar', C.bad);
  } else if (ph === 1) {
    B(20, 130, 160, 70, 'Arpan ka phone', 'video, 100 tukde');
    B(400, 30, 470, 200, 'S3: upload U-9', '', { st: 'good' });
    cells(420, 75, [['1', C.good], ['2', C.good], ['...', C.good], ['90', C.good]], { w: 70, h: 32 });
    cells(420, 120, [['91', C.line], ['...', C.line], ['100', C.line]], { w: 70, h: 32 });
    T(800, 92, '90 rasid', { c: C.good, size: 12 }); T(800, 137, 'baaki', { c: C.muted, size: 12 });
    A(180, 140, 398, 90, C.good, { n: 1 }); L(280, 95, 'tukde 1-90', C.good);
    num(80, 230, 2, C.bad); L(100, 235, 'wifi gaya', C.bad, 'start');
    A(180, 170, 398, 137, C.acc, { n: 3 }); L(300, 180, 'sirf 91-100', C.acc);
    A(180, 195, 398, 210, C.good, { n: 4 }); L(290, 228, 'COMPLETE', C.good);
    L(450, 320, 'jis tukde ki rasid S3 pe, wo pakka pahuncha. Shuru se kuch nahi', C.good);
  } else {
    B(20, 90, 180, 150, 'Arpan ka phone', 'kal khola', { st: 'acc' });
    B(620, 90, 250, 150, 'S3: upload U-9', '1-90 ki rasid', { st: 'good' });
    A(200, 120, 618, 120, C.acc, { n: 1 }); L(330, 106, 'ListParts U-9', C.acc);
    A(618, 170, 202, 170, C.good, { n: 2 }); L(330, 156, '1-90 aa chuke', C.good);
    A(200, 220, 618, 220, C.good, { n: 3 }); L(330, 206, 'sirf 91-100', C.good);
    L(450, 310, 'rasid S3 pe hai, phone pe nahi. Phone band ho to bhi kuch nahi khota', C.good);
  }
}
