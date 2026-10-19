// Card 10: koi message nahi hatata, disk full, likhna band. Ilaaj: 7 din retention, purana segment poora udao.
const DUR = 18, FIX = 8;
function setup() {
  box(20, 150, 100, 84, 'Orders', 'naye');
  box(200, 120, 560, 150, 'Broker disk');
  wire('M120 192 H200');
}
function frame(t, fix, tl) {
  const segs = [];
  if (!fix) {
    CAP.textContent = 'DIKKAT: sab message hamesha pade, koi nahi hatata. Disk full, naya order likhna band';
    const n = Math.min(9, 3 + Math.floor(tl / 0.6));
    for (let i = 0; i < n; i++) segs.push([`din ${i + 1}`, n >= 9 ? C.bad : C.fg]);
    cells(215, 175, segs, { w: 54, h: 40 });
    bar(330, 300, 300, 16, n / 9, 'disk');
    if (n >= 9) { T(480, 345, 'disk full: order flow ruka', { c: C.bad, b: 1 });
      T(70, 130, 'likh nahi sakte', { c: C.bad, b: 1, size: 12 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: 7 din rakho, phir purana segment (~1 GB) poora udao (retention). Padhne se nahi';
  const day = 7 + Math.min(4, Math.floor(tl / 1.2));
  for (let d = day - 6; d <= day; d++) segs.push([`din ${d}`, d === day && tl > 1.2 ? C.good : C.fg]);
  cells(230, 175, segs, { w: 66, h: 40 });
  const ph = (tl % 1.2) / 1.2;
  if (tl > 1.2 && day < 11) { const y = 225 + ph * 80;
    el('rect', { x: 140, y, width: 66, height: 34, rx: 4, fill: C.bg, stroke: C.bad }, DYN);
    T(173, y + 22, `din ${day - 7}`, { c: C.bad, size: 11, b: 1 }); }
  T(173, 350, 'purana: poora uda', { c: C.bad, size: 12 });
  bar(330, 300, 300, 16, 0.6, 'disk', true);
  T(480, 345, 'disk barabar, replay 7 din tak', { c: C.good, b: 1 });
}
