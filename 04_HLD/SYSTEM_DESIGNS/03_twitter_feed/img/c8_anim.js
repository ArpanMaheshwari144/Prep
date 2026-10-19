// Card 8: region me ek hi App, giri to region band. Ilaaj: kai App + ALB, App kuch yaad nahi rakhta.
const DUR = 16, FIX = 7.5;
const U = [70, 192], LB = [260, 192], A = [[520, 80], [520, 192], [520, 304]];
function setup() {
  const bug = grp('bug'), fix = grp('fix');
  box(20, 150, 100, 84, 'India users');
  box(460, 160, 120, 64, 'Ek App', '', { parent: bug }); wire('M120 192 H460', '', bug);
  box(210, 160, 100, 64, 'ALB', '', { parent: fix, stroke: C.acc }); wire('M120 192 H210', '', fix);
  A.forEach(([x, y], i) => { box(x - 60, y - 30, 120, 60, `App ${i + 1}`, '', { parent: fix }); wire(`M310 192 L${x - 60} ${y}`, '', fix); });
  box(700, 160, 150, 64, 'Redis / DB', 'sab yahan', { parent: fix }); A.forEach(([x, y]) => wire(`M580 ${y} L700 192`, '', fix));
}
function frame(t, fix, tl) {
  vis('bug', !fix); vis('fix', fix);
  if (!fix) {
    CAP.textContent = 'DIKKAT: raat ko India region me ek hi App machine. Wo giri, poora India band';
    const down = tl > 2.2;
    for (let k = 0; k < 8; k++) { const s = 0.3 + k * 0.5;
      const p = path([[s, U], [s + 0.8, [455, 192]]], tl);
      if (tl < s + 1.1) dot(p, down && tl > s + 0.8 ? C.bad : C.msg, '', down && tl > s + 0.8 ? 'fail' : '', C.bad); }
    if (down) { cross(520, 192); T(520, 260, 'India band', { c: C.bad, b: 1, size: 15 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: kai App, aage ALB baante. App kuch yaad nahi rakhta. App 2 giri to bahar, baaki chalu';
  const down = tl > 2.0;
  if (down) cross(520, 192);
  for (let k = 0; k < 10; k++) { const s = 0.3 + k * 0.45;
    let a = k % 3; if (down && s > 1.6 && a === 1) a = k % 2 ? 0 : 2;
    const p = path([[s, U], [s + 0.4, LB], [s + 0.9, [A[a][0] - 60, A[a][1]]]], tl);
    if (tl < s + 1.1) dot(p, C.good); }
  if (down) T(775, 250, 'user ko farak nahi', { c: C.good, b: 1, size: 13 });
}
