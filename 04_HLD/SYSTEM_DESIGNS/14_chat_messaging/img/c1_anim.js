// Card 1: ek connection = ek thread (~1 MB) -> memory khatam. Ilaaj: event loop, 4 thread, epoll batata data kahan aaya.
const DUR = 18, FIX = 8;
const U = [140, 190], LB = [235, 190], CS = [348, 190];
function setup() {
  wire('M140 190 H195'); wire('M275 190 H360');
  box(20, 140, 120, 100, 'Log', 'subah jude');
  box(195, 165, 80, 50, 'LB');
  box(360, 30, 330, 300, 'Chat Server', 'ek machine', { stroke: C.acc });
  box(400, 260, 250, 56, 'epoll (OS)', 'batata data kahan aaya', { id: 'ep' });
}
function frame(t, fix, tl) {
  vis('ep', fix);
  const N = 30;
  let n = 0;
  for (let k = 0; k < N; k++) {
    const s = 0.3 + k * 0.17;
    if (tl >= s + 0.6) { n++; continue; }
    dot(path([[s, U], [s + 0.3, LB], [s + 0.6, CS]], tl), C.msg);
  }
  const conn = Math.round(100000 * n / N);
  if (!fix) {
    CAP.textContent = 'DIKKAT: ek connection = ek thread (~1 MB). Connection badhe, thread badhe, memory khatam';
    const cells = Math.min(60, n * 2);
    for (let i = 0; i < cells; i++) {
      const x = 378 + (i % 10) * 30, y = 80 + Math.floor(i / 10) * 26;
      el('rect', { x, y, width: 26, height: 20, rx: 3, fill: C.bg, stroke: C.bad, 'stroke-width': 1.2 }, DYN);
      T(x + 13, y + 14, 'T', { size: 11, c: C.bad, b: 1 });
    }
    if (n > 0) T(525, 255, 'har connection ka apna thread', { c: C.muted, size: 12 });
    T(790, 85, `connection: ${conn.toLocaleString('en-IN')}`, { size: 13 });
    T(790, 110, `thread: ${conn.toLocaleString('en-IN')}`, { size: 13, c: C.bad, b: 1 });
    T(790, 135, 'har thread ~1 MB', { size: 12, c: C.muted });
    bar(715, 175, 150, 18, n / 22, 'memory');
    if (n >= 22) {
      T(790, 225, 'memory khatam', { c: C.bad, b: 1, size: 15 });
      T(790, 248, '2 crore thread? nahi', { c: C.bad, size: 12 });
      cross(790, 290);
    }
    return;
  }
  CAP.textContent = 'ILAAJ: kuch thread (4) saare connection. OS batata data kahan aaya (epoll) = event loop';
  for (let i = 0; i < 60; i++) {
    const x = 385 + (i % 12) * 25, y = 88 + Math.floor(i / 12) * 22;
    el('circle', { cx: x, cy: y, r: 5, fill: i < n * 2 ? C.line : 'none', stroke: C.line }, DYN);
  }
  T(525, 205, 'chup connection = thodi memory', { c: C.muted, size: 12 });
  const ev = [[6.0, 7, 0], [6.8, 31, 1], [7.6, 50, 2], [8.4, 18, 3], [9.2, 44, 0]];
  ev.forEach(([s, i, th]) => {
    const p = [385 + (i % 12) * 25, 88 + Math.floor(i / 12) * 22];
    const thP = [440 + th * 54, 232];
    if (tl >= s && tl < s + 0.7) el('circle', { cx: p[0], cy: p[1], r: 6, fill: C.msg }, DYN);
    if (tl >= s && tl < s + 0.7) dot(path([[s, p], [s + 0.7, thP]], tl), C.msg, '', tl < s + 0.35 ? 'data' : '');
  });
  cells(415, 220, [['T1', C.good], ['T2', C.good], ['T3', C.good], ['T4', C.good]], { w: 50, h: 24 });
  T(790, 85, `connection: ${conn.toLocaleString('en-IN')}`, { size: 13 });
  T(790, 110, 'thread: 4', { size: 13, c: C.good, b: 1 });
  T(790, 135, 'thread tabhi jab data aaya', { size: 12, c: C.muted });
  bar(715, 175, 150, 18, 0.12 + 0.18 * n / N, 'memory', true);
  if (n >= N) T(790, 235, '~200 server kaafi', { c: C.good, b: 1, size: 15 });
}
