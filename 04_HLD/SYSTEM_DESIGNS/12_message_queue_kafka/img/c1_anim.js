// Card 1: queue RAM me, restart = 500 signup gayab. Ilaaj: disk pe log + offset, consumer wahin se aage.
const DUR = 17, FIX = 8;
const P = [70, 192];
function setup() {
  box(20, 150, 100, 84, 'Signup Svc', 'producer');
  wire('M120 192 H250'); wire('M510 192 H700');
  box(250, 140, 260, 110, 'Queue');
  box(700, 150, 140, 84, 'Email Svc', 'consumer');
}
function frame(t, fix, tl) {
  if (!fix) {
    CAP.textContent = 'DIKKAT: 500 signup machine ki RAM me the. Restart hua, RAM saaf, sab gayab';
    T(380, 180, 'RAM me', { c: C.muted, size: 12 });
    const restart = tl > 3.6;
    for (let k = 0; k < 6; k++) { const s = 0.3 + k * 0.4;
      if (!restart) dot(path([[s, P], [s + 0.6, [290 + k * 36, 220]]], tl), C.msg); }
    if (restart && tl < 4.4) { el('rect', { x: 250, y: 140, width: 260, height: 110, rx: 8, fill: C.bad, opacity: 0.35 }, DYN);
      T(380, 285, 'RESTART', { c: C.bad, b: 1, size: 15 }); }
    if (restart) T(380, 222, 'khaali', { c: C.bad, b: 1, size: 14 });
    if (tl > 4.6) T(770, 130, '"koi signup aaya hi nahi"', { c: C.bad, b: 1, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: disk pe file (log), padhne se mitta nahi. Email apna number yaad rakhe (offset)';
  T(380, 180, 'disk pe log', { c: C.acc, size: 12 });
  const n = Math.min(8, Math.floor(tl / 0.4) + 1);
  const items = []; for (let i = 0; i < n; i++) items.push([`${118 + i}`, i + 118 <= 120 ? C.muted : C.fg]);
  cells(262, 200, items, { w: 26, h: 26 });
  const off = tl < 3.5 ? 120 : Math.min(125, 120 + Math.floor((tl - 3.5) / 0.6) + 1);
  if (tl > 2.0 && tl < 3.4) { el('rect', { x: 700, y: 150, width: 140, height: 84, rx: 8, fill: C.bad, opacity: 0.35 }, DYN);
    T(770, 260, 'Email restart', { c: C.bad, b: 1, size: 13 }); }
  T(770, 300, `offset = ${off}`, { c: C.acc, b: 1, size: 14 });
  if (tl > 3.5) { T(770, 130, `${off} se aage padh raha`, { c: C.good, b: 1, size: 12 });
    dot(path([[0, [510, 192]], [0.6, [700, 192]]], (tl - 3.5) % 0.6), C.good); }
  T(380, 285, 'restart ho, message disk pe pade', { c: C.good, size: 12 });
}
