// Card 4: kai server, editor alag server pe -> do OT, do version. Ilaaj: docId se ring pe ek owner.
const DUR = 17, FIX = 8;
const ME = [140, 90], RV = [140, 280], LB = [285, 185];
const S = [[548, 62], [548, 182], [548, 302]];
function setup() {
  wire('M140 90 L230 175'); wire('M140 280 L230 195');
  S.forEach(([x, y]) => wire(`M340 185 L560 ${y}`));
  box(20, 60, 120, 60, 'Main', 'doc:123');
  box(20, 250, 120, 60, 'Ravi', 'doc:123');
  box(230, 150, 110, 70, 'LB');
  box(560, 30, 170, 64, 'Server-1', '~1 lakh conn');
  box(560, 150, 170, 64, 'Server-2', '~1 lakh conn');
  box(560, 270, 170, 64, 'Server-3', '~1 lakh conn');
}
function frame(t, fix, tl) {
  T(285, 205, fix ? 'ring: docId' : 'koi bhi', { c: fix ? C.good : C.muted, size: 12, b: fix ? 1 : 0 });
  T(285, 340, '50 lakh conn -> kai server', { c: C.muted, size: 12 });
  dot(path([[0.3, ME], [0.9, LB], [1.6, S[0]]], tl < 1.6 ? tl : -1), C.msg, 'X');
  if (!fix) {
    CAP.textContent = 'DIKKAT: Main server-1 pe, Ravi server-3 pe. Dono server OT karein -> do version';
    dot(path([[0.5, RV], [1.1, LB], [1.8, S[2]]], tl < 1.8 ? tl : -1), C.acc, 'Y');
    if (tl >= 1.6) T(740, 67, 'v8 = X', { c: C.bad, b: 1, size: 14, a: 'start' });
    if (tl >= 1.8) T(740, 307, 'v8 = Y', { c: C.bad, b: 1, size: 14, a: 'start' });
    if (tl >= 2.4) { T(810, 175, 'OT kaun kare?', { c: C.bad, b: 1, size: 13 });
                     T(810, 197, 'doc toota', { c: C.bad, b: 1, size: 13 }); }
    return;
  }
  CAP.textContent = 'ILAAJ: docId se ring pe server chuno. doc:123 ka owner server-1, dono editor wahin, OT ek jagah';
  dot(path([[0.5, RV], [1.1, LB], [1.8, [548, 75]]], tl < 1.8 ? tl : -1), C.acc, 'Y');
  if (tl >= 0.9) T(450, 105, 'doc:123 -> server-1', { c: C.good, b: 1, size: 12, halo: 1 });
  if (tl >= 1.6) T(740, 55, 'owner', { c: C.good, b: 1, size: 13, a: 'start' });
  if (tl >= 1.8) T(740, 77, 'v8 X, v9 Y', { c: C.good, b: 1, size: 13, a: 'start' });
  if (tl >= 2.4) T(810, 190, 'OT ek jagah', { c: C.good, b: 1, size: 14 });
}
