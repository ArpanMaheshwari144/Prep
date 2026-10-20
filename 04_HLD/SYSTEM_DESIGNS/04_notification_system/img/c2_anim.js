// Card 2: push / email / SMS ek hi line, worker SMS pe atka to sab atke. Ilaaj: har raaste ki alag line + alag worker.
const DUR = 17, FIX = 7.5;
const ROW = [70, 190, 310], NM = ['Push', 'Email', 'SMS'];
const mv = (a, b, s, e, tl) => (tl >= s && tl <= e ? path([[s, a], [e, b]], tl) : null);
function setup() {
  box(20, 150, 120, 80, 'Notification', 'Svc');
  wire('M140 190 H480', 'one');
  ROW.forEach((y, i) => {
    wire(`M140 190 L170 ${y}`, 'r' + i); wire(`M170 ${y} H480`, 'q' + i);
    wire(`M590 ${y} H690`, 'c' + i);
    box(480, y - 25, 110, 50, `${NM[i]} worker`, '', { id: 'wk' + i });
    box(690, y - 25, 120, 50, `${NM[i]} co.`, '', { stroke: i === 2 ? C.bad : C.line });
  });
  wire('M590 190 L690 70', 'x0'); wire('M590 190 L690 310', 'x2');
  box(480, 165, 110, 50, 'Worker', '', { id: 'w1' });
}
function frame(t, fix, tl) {
  [0, 1, 2].forEach(i => { vis('r' + i, fix); vis('q' + i, fix); vis('wk' + i, fix); });
  vis('c0', fix); vis('c2', fix); vis('one', !fix); vis('w1', !fix); vis('x0', !fix); vis('x2', !fix);
  T(850, 345, 'SMS co. dheemi', { c: C.bad, size: 11, a: 'end' });
  if (!fix) {
    CAP.textContent = 'DIKKAT: push, email, SMS ek hi line. Worker SMS pe atka, push + email bhi peeche';
    cells(170, 177, [['Email', C.msg], ['Push', C.msg], ['Email', C.msg], ['Push', C.msg], ['SMS', C.bad]], { w: 48 });
    T(305, 165, 'ek hi line', { c: C.muted, size: 12 });
    dot(mv([590, 190], [678, 300], 0.5, 1.2, tl), C.bad, '', 'SMS', C.bad);
    if (tl > 1.2) dot([678, 300], C.bad);
    if (tl > 1.2) T(535, 245, `SMS pe atka ${Math.round((tl - 1.2) * 4)} sec`, { c: C.bad, b: 1, size: 12 });
    T(835, 75, '0', { c: C.bad, b: 1, size: 15 }); T(835, 195, '0', { c: C.bad, b: 1, size: 15 });
    if (tl > 3) T(305, 225, 'push + email intezaar me', { c: C.bad, b: 1, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: har raaste ki alag line + alag worker. SMS dheema to sirf SMS ruka';
  const n = [0, 0];
  [0, 1].forEach(i => {
    for (let k = 0; k < 6; k++) {
      const s = 0.4 + k * 1.0 + i * 0.3;
      if (tl >= s + 0.6) n[i]++;
      dot(mv([590, ROW[i]], [678, ROW[i]], s, s + 0.6, tl), C.good);
    }
    cells(170, ROW[i] - 13, Array(Math.max(1, 5 - n[i])).fill([NM[i], C.msg]), { w: 48 });
    T(835, ROW[i] + 5, `${n[i]}`, { c: C.good, b: 1, size: 15 });
  });
  cells(170, ROW[2] - 13, [['SMS', C.bad], ['SMS', C.bad], ['SMS', C.bad]], { w: 48 });
  dot(mv([590, 310], [678, 310], 0.5, 1.5, tl), C.bad);
  if (tl > 1.5) T(635, 290, 'dheema', { c: C.bad, b: 1, size: 12 });
  T(305, 115, 'push chala', { c: C.good, size: 12 }); T(305, 235, 'email chala', { c: C.good, size: 12 });
}
