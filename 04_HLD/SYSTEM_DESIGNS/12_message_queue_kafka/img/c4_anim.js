// Card 4: email ki 3 machine alag-alag, har ek ne P7 padha -> Ravi ko 3 email. Ilaaj: ek consumer group.
const DUR = 16, FIX = 7.5;
const P7 = [200, 192], M = [[560, 90], [560, 192], [560, 294]];
function setup() {
  box(80, 160, 120, 64, 'P7', 'Ravi ka order', { stroke: C.acc });
  M.forEach(([x, y], i) => { wire(`M200 192 L${x - 60} ${y}`); wire(`M${x + 60} ${y} L740 192`); box(x - 60, y - 30, 120, 60, `Email C${i + 1}`); });
  box(740, 160, 100, 64, 'Ravi');
}
function frame(t, fix, tl) {
  let mails = 0;
  if (!fix) {
    CAP.textContent = 'DIKKAT: teeno machine alag-alag. Har ek ne har message padha, Ravi ko 3 email';
    M.forEach((m, i) => { const s = 0.4 + i * 0.3;
      dot(path([[s, P7], [s + 0.7, [m[0] - 60, m[1] + 12]], [s + 1.4, [m[0] + 60, m[1] + 12]], [s + 2.0, [740, 192]]], tl < s + 2.0 ? tl : -1), C.msg);
      if (tl >= s + 2.0) mails++; });
    T(790, 260, `email: ${mails}`, { c: mails > 1 ? C.bad : C.fg, b: 1, size: 15 });
    if (mails > 1) T(790, 285, 'Ravi pareshaan', { c: C.bad, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: teeno ek team (consumer group). Har tukda team ke ek hi member ko: P7 sirf C3';
  el('rect', { x: 470, y: 45, width: 180, height: 290, rx: 10, fill: 'none', stroke: C.good, 'stroke-dasharray': '6 4' }, DYN);
  T(560, 352, 'group.id = email', { c: C.good, size: 12 });
  T(700, 95, 'P0-3', { c: C.muted, size: 11 }); T(700, 197, 'P4-6', { c: C.muted, size: 11 }); T(700, 299, 'P7-9', { c: C.acc, size: 11, b: 1 });
  const m = M[2], s = 0.5;
  dot(path([[s, P7], [s + 0.8, [m[0] - 60, m[1] + 12]], [s + 1.5, [m[0] + 60, m[1] + 12]], [s + 2.1, [740, 192]]], tl < s + 2.1 ? tl : -1), C.good);
  if (tl >= s + 2.1) mails = 1;
  T(790, 260, `email: ${mails}`, { c: C.good, b: 1, size: 15 });
}
