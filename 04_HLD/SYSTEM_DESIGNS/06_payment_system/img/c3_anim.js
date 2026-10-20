// Card 3: jawab raste me kho gaya, app ne dobara bheja, server ne naya payment maana -> 1000. Ilaaj: key-555, dobara = saved jawab.
const DUR = 18, FIX = 7.5;
const AP = [150, 175], SV = [338, 175], SVR = [490, 175], DB = [688, 175], LOST = [250, 175];
let TL = 0;
function setup() {
  wire('M150 175 H350'); wire('M490 175 H700'); wire('M420 210 V280', 'wst');
  box(20, 140, 130, 70, 'Arpan app', 'metro');
  box(350, 140, 140, 70, 'Payment Svc');
  box(700, 140, 160, 70, 'SQL DB');
  box(350, 280, 140, 60, 'Idempotency', 'store', { id: 'ids', stroke: C.acc });
}
function lost(a, end) {
  if (TL > a && TL < a + 0.6) dot(path([[a, SV], [a + 0.6, LOST]], TL), C.good, '', 'hua');
  if (TL >= a + 0.6 && TL < (end || 99)) { cross(LOST[0], LOST[1]); T(250, 145, 'network gaya', { c: C.bad, b: 1, size: 12 }); }
}
function frame(t, fix, tl) {
  TL = tl;
  vis('ids', fix); vis('wst', fix);
  let cut = 0;
  if (!fix) {
    CAP.textContent = 'DIKKAT: jawab raste me kho gaya, app ne dobara bheja, server ne naya payment maana';
    if (tl < 1.6) dot(path([[0.3, AP], [1.0, SV], [1.0, SVR], [1.6, DB]], tl), C.msg, '', '500');
    if (tl >= 1.6) cut = 500;
    lost(1.8, 3.2);
    if (tl > 2.9) T(85, 125, 'timeout -> dobara', { c: C.acc, b: 1, size: 12 });
    if (tl > 3.2 && tl < 4.5) dot(path([[3.2, AP], [3.9, SV], [3.9, SVR], [4.5, DB]], tl), C.msg, '', '500 (dobara)');
    if (tl >= 4.5) { cut = 1000; T(420, 250, 'server: ye naya payment hai', { c: C.bad, size: 12 }); }
  } else {
    CAP.textContent = 'ILAAJ: screen khulte key-555. Retry pe wahi key -> store me pehle se = saved jawab';
    const K = 'key-555';
    if (tl < 1.0) dot(path([[0.3, AP], [1.0, SV]], tl), C.msg, '', '500 + ' + K);
    if (tl > 1.0 && tl < 1.5) dot(path([[1.0, [420, 215]], [1.5, [420, 268]]], tl), C.acc, '', K + '?', C.acc);
    if (tl >= 1.5 && tl < 2.4) T(500, 305, 'nahi -> IN_PROGRESS', { a: 'start', c: C.acc, b: 1 });
    if (tl > 1.5 && tl < 2.1) dot(path([[1.5, SVR], [2.1, DB]], tl), C.msg, '', '-500');
    if (tl >= 2.1) cut = 500;
    if (tl >= 2.4) T(500, 305, 'DONE + jawab', { a: 'start', c: C.good, b: 1 });
    lost(2.5, 3.9);
    if (tl > 3.6) T(85, 125, 'timeout -> dobara', { c: C.acc, b: 1, size: 12 });
    if (tl > 3.9 && tl < 4.6) dot(path([[3.9, AP], [4.6, SV]], tl), C.msg, '', K + ' (dobara)');
    if (tl > 4.6 && tl < 5.1) dot(path([[4.6, [420, 215]], [5.1, [420, 268]]], tl), C.acc, '', K + '?', C.acc);
    if (tl >= 5.1) T(500, 330, 'pehle se -> saved jawab', { a: 'start', c: C.good, b: 1 });
    if (tl > 5.3 && tl < 6.0) dot(path([[5.3, SV], [6.0, AP]], tl), C.good, '', '500 hua');
    if (tl >= 6.0) T(85, 235, '"500 hua" (ek hi)', { c: C.good, b: 1, size: 13 });
  }
  T(780, 240, `kata: ${cut}`, { c: cut > 500 ? C.bad : cut ? C.good : C.fg, b: 1, size: 16 });
}
