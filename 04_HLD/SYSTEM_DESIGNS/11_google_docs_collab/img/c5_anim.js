// Card 5: Ravi viewer server-2 pe, server-1 ka op wahan nahi pahunchta. Ilaaj: doc ka channel, Redis pub/sub.
const DUR = 16, FIX = 7.5;
function setup() {
  wire('M140 92 H300'); wire('M460 282 H700');
  box(20, 60, 120, 64, 'Main', 'editor');
  box(300, 60, 160, 64, 'Server-1', 'owner, OT');
  box(300, 250, 160, 64, 'Server-2');
  box(700, 250, 150, 64, 'Ravi', 'sirf padh raha');
  wire('M460 92 L590 160', 'w1'); wire('M590 190 L460 262', 'w2');
  box(590, 130, 220, 64, 'Redis pub/sub', 'channel doc:123', { id: 'rd', stroke: C.acc });
}
function frame(t, fix, tl) {
  ['rd', 'w1', 'w2'].forEach(i => vis(i, fix));
  dot(path([[0.3, [140, 92]], [1.0, [288, 92]]], tl < 1.0 ? tl : -1), C.msg, 'X');
  if (tl >= 1.0) T(380, 145, 'X laga (v8)', { c: C.fg, size: 12 });
  if (!fix) {
    CAP.textContent = 'DIKKAT: maine server-1 pe likha, server-2 ko kuch pata nahi. Ravi ke paas purana doc';
    if (tl >= 1.6) { cross(380, 195); T(420, 230, 'server-2 ko pata nahi', { c: C.bad, size: 12, a: 'start' }); }
    T(775, 340, 'Ravi: HELLO (purana)', { c: tl >= 1.6 ? C.bad : C.fg, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: har doc ka ek channel (Redis pub/sub). Server-1 op daalta, server-2 sunta, Ravi ko bhejta';
  if (tl > 1.1 && tl < 1.8) dot(path([[1.1, [460, 92]], [1.8, [590, 160]]], tl), C.acc, 'X', 'publish', C.acc);
  if (tl > 1.8 && tl < 2.5) dot(path([[1.8, [590, 190]], [2.5, [460, 262]]], tl), C.acc, 'X', 'subscriber', C.acc);
  if (tl > 2.5 && tl < 3.1) dot(path([[2.5, [460, 282]], [3.1, [688, 282]]], tl), C.good, 'X');
  if (tl >= 1.8) T(700, 215, 'bheja aur bhool gaya (store nahi)', { c: C.muted, size: 12 });
  const got = tl >= 3.1;
  T(775, 340, got ? 'Ravi: HELLO X' : 'Ravi: HELLO', { c: got ? C.good : C.fg, b: 1, size: 13 });
}
