// Card 7: sale, har user had me, phir bhi server gira. Ilaaj: gateway pe system ki had + load shedding (503).
const DUR = 17, FIX = 7.5;
const GW = [380, 192], APP = [580, 205];
const U = [[70, 70], [70, 130], [70, 192], [70, 254], [70, 316]];
function setup() {
  U.forEach(([x, y], i) => { box(x - 50, y - 22, 100, 44, `user ${i + 1}`); wire(`M${x + 50} ${y} L335 192`); });
  wire('M425 192 H560');
  box(335, 162, 90, 60, 'Gateway', 'limiter', { stroke: C.acc });
  box(560, 150, 120, 84, 'Server', 'CPU');
}
function frame(t, fix, tl) {
  let cpu = 0;
  T(70, 22, 'har user apni had me', { c: C.good, size: 12 });
  for (let k = 0; k < 20; k++) {
    const u = k % 5, s = 0.3 + k * 0.22, imp = k % 3 === 0;   // har teesri = login / payment
    if (!fix) {
      const p = path([[s, U[u]], [s + 0.5, GW], [s + 0.9, APP]], tl);
      if (tl >= s + 0.9) cpu++;
      if (tl < s + 1.2) dot(p, C.msg);
    } else {
      const drop = !imp && k > 8;
      const pts = drop ? [[s, U[u]], [s + 0.5, GW]] : [[s, U[u]], [s + 0.5, GW], [s + 0.9, APP]];
      const end = pts[pts.length - 1][0];
      if (tl >= end && !drop) cpu++;
      if (tl < end + 0.5) dot(drop && tl >= end ? [380, 245] : path(pts, tl), drop && tl >= end ? C.bad : imp ? C.good : C.msg, '',
        drop && tl >= end ? '503' : '', C.bad);
    }
  }
  if (!fix) {
    CAP.textContent = 'DIKKAT: laakhon user, har ek apni had me. Par kul jod se server CPU 100%';
    const f = cpu / 20; bar(570, 245, 100, 16, f, '');
    T(620, 285, `CPU ${Math.round(100 * f)}%`, { c: f > 0.9 ? C.bad : C.fg, b: 1 });
    if (f >= 1) T(620, 305, 'server gira, sab band', { c: C.bad, b: 1 });
  } else {
    CAP.textContent = 'ILAAJ: gateway pe system ki had (5,000/sec). CPU 90% paar: kam zaroori 503, login/payment bachao';
    const f = Math.min(0.85, cpu / 20 * 1.6); bar(570, 245, 100, 16, f, '');
    T(620, 285, `CPU ${Math.round(100 * f)}%`, { c: C.good, b: 1 });
    T(380, 140, 'had: 5,000 / sec', { c: C.acc, size: 12 });
    T(880, 310, 'hara = login / payment', { a: 'end', c: C.good, size: 12 });
    T(880, 330, 'laal 503 = recommendation', { a: 'end', c: C.bad, size: 12 });
  }
}
