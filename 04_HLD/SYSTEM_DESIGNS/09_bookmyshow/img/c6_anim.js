// Card 6: 2 baje Redis restart, cache khaali, har miss replica pe -> replica baith gayi. Ilaaj: SET NX EX 5, pehla hi banaye.
const DUR = 18, FIX = 8;
const U = [130, 190], AL = [208, 190], AR = [330, 200], RL = [440, 90], PL = [668, 210];
function setup() {
  wire('M130 190 H220'); wire('M330 175 L450 80'); wire('M330 200 L680 210');
  box(20, 150, 110, 80, 'Hazaaron user', 'seat map');
  box(220, 150, 110, 80, 'App');
  box(450, 40, 180, 70, 'Redis', '', { stroke: C.acc });
  box(680, 170, 170, 80, 'Read replica');
}
function frame(t, fix, tl) {
  T(890, 30, 'samay 2:00', { a: 'end', c: C.muted, size: 12 });
  if (!fix) {
    CAP.textContent = 'DIKKAT: 2 baje Redis restart, cache khaali. Har miss seedha replica pe, replica baith gayi';
    T(540, 88, 'restart: khaali', { c: C.bad, b: 1, size: 12 });
    for (let k = 0; k < 40; k++) {
      const s = k * 0.15, p = path([[s, U], [s + 0.4, AL], [s + 0.5, AR], [s + 1.1, PL]], tl);
      if (tl < s + 1.1) dot(p, C.msg);
    }
    bar(680, 280, 170, 16, Math.min(1, tl / 3), '');
    T(765, 315, 'replica load', { c: C.muted, size: 12 });
    if (tl > 3.5) { cross(868, 210); T(765, 158, 'replica baith gayi', { c: C.bad, b: 1, size: 13 }); }
    if (tl > 4.0) T(75, 255, 'map nahi khulta', { c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: pehla SET NX EX 5 lock leta, replica se ek query, map Redis me. Baaki ruk ke Redis se';
  T(540, 88, tl < 3.8 ? 'khaali' : 'seatmap:show42', { c: tl < 3.8 ? C.bad : C.good, b: 1, size: 12 });
  for (let k = 0; k < 5; k++) { const s = 0.2 + k * 0.1; if (tl > s && tl < s + 0.5) dot(path([[s, U], [s + 0.5, AL]], tl), C.msg); }
  if (tl > 1.0 && tl < 2.0) dot(tl < 1.5 ? path([[1.0, AR], [1.5, RL]], tl) : path([[1.5, RL], [2.0, AR]], tl),
    tl < 1.5 ? C.acc : C.good, '', tl < 1.5 ? 'SET NX EX 5' : 'lock mila', tl < 1.5 ? C.acc : C.good);
  if (tl > 2.0 && tl < 3.2) dot(tl < 2.6 ? path([[2.0, AR], [2.6, PL]], tl) : path([[2.6, PL], [3.2, AR]], tl), C.acc, '', 'ek query', C.acc);
  if (tl > 3.2 && tl < 3.8) dot(path([[3.2, AR], [3.8, RL]], tl), C.good, '', 'map rakha', C.good);
  if (tl > 0.8 && tl < 4.0) {
    for (let i = 0; i < 4; i++) dot([245 + i * 22, 275], C.msg);
    T(278, 305, 'baaki ruke (50-100 ms)', { c: C.acc, size: 12 });
  }
  if (tl > 4.0 && tl < 5.2) for (let i = 0; i < 4; i++) {
    const p = tl < 4.6 ? path([[4.0, [245 + i * 22, 275]], [4.6, [440, 95 + i * 3]]], tl) : path([[4.6, [440, 95]], [5.2, AR]], tl);
    dot(p, tl < 4.6 ? C.msg : C.good);
  }
  if (tl > 5.2) T(75, 255, 'map khula', { c: C.good, b: 1, size: 13 });
  bar(680, 280, 170, 16, 0.1, '', true);
  T(765, 315, 'replica: sirf 1 query', { c: C.good, size: 12 });
}
