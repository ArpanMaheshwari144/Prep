// Card 2: do App ne ek saath 4 padha, dono ne 5 likha -> 6 andar. Ilaaj: INCR ek kadam.
const DUR = 16, FIX = 7.5;
const BOT = [110, 180], LB = [210, 192], A1 = [420, 108], A2 = [420, 278], R = [610, 180], RE = [600, 205];
function setup() {
  wire('M110 180 H170'); wire('M250 180 L360 95'); wire('M250 180 L360 265');
  wire('M480 95 L620 170'); wire('M480 265 L620 190');
  box(20, 135, 90, 90, 'Bot', '20 login');
  box(170, 155, 80, 50, 'LB');
  box(360, 65, 120, 60, 'App-1'); box(360, 235, 120, 60, 'App-2');
  box(620, 145, 170, 70, 'Redis', '', { r: 10, stroke: C.acc });
}
function frame(t, fix, tl) {
  CAP.textContent = fix ? 'ILAAJ: INCR = padhna + badhana EK kadam. Redis ek ke baad ek chalata'
                        : 'DIKKAT: GET phir SET = do kadam. Dono App ne 4 padha, dono ne 5 likha';
  T(890, 330, 'had: 5 · pehle se andar: 4', { a: 'end', c: C.muted, size: 12 });
  const d5 = path([[0.3, BOT], [0.7, LB], [1.1, A1]], tl), d6 = path([[0.3, BOT], [0.7, LB], [1.1, A2]], tl);
  let rc = 4;
  if (!fix) {
    dot(d5, C.msg, 5); dot(d6, C.msg, 6);
    if (tl > 1.1 && tl < 2.3) {               // GET jaata, 4 laata
      const f = tl < 1.7 ? [[1.1, A1], [1.7, R]] : [[1.7, R], [2.3, A1]];
      const g = tl < 1.7 ? [[1.1, A2], [1.7, R]] : [[1.7, R], [2.3, A2]];
      const lb = tl < 1.7 ? 'GET' : '4';
      dot(path(f, tl), C.acc, '', lb, C.acc); dot(path(g, tl), C.acc, '', lb, C.acc);
    }
    if (tl >= 2.3) { T(420, 150, '4 < 5 -> andar', { c: C.good, size: 12 }); T(420, 320, '4 < 5 -> andar', { c: C.good, size: 12 }); }
    if (tl > 2.6 && tl < 3.2) {
      dot(path([[2.6, A1], [3.2, R]], tl), C.acc, '', 'SET 5', C.acc);
      dot(path([[2.6, A2], [3.2, R]], tl), C.acc, '', 'SET 5', C.acc);
    }
    if (tl >= 3.2) rc = 5;
    if (tl >= 3.5) { T(890, 280, 'andar: 6', { a: 'end', c: C.bad, b: 1, size: 15 });
                     T(890, 305, 'had tooti! (dono ne 4 padha)', { a: 'end', c: C.bad, size: 12 }); }
  } else {
    dot(d5, C.msg, 5); dot(d6, C.msg, 6);
    T(705, 238, 'ek line: ek ke baad ek', { c: C.muted, size: 12 });
    if (tl > 1.1 && tl < 2.6) {
      const a = tl < 1.7 ? path([[1.1, A1], [1.7, RE]], tl) : path([[1.7, RE], [2.3, A1]], tl);
      if (tl < 2.3) dot(a, tl < 1.7 ? C.acc : C.good, '', tl < 1.7 ? 'INCR' : '5: andar', tl < 1.7 ? C.acc : C.good);
      const wait = [[1.1, A2], [1.7, [570, 228]], [2.0, [570, 228]], [2.0, RE]];
      const b = tl < 2.0 ? path(wait, tl) : path([[2.0, RE], [2.6, A2]], tl);
      dot(b, tl < 2.0 ? C.acc : C.bad, '', tl < 2.0 ? 'INCR (ruk)' : '6: 429', tl < 2.0 ? C.acc : C.bad);
    }
    if (tl >= 1.7) rc = 5;
    if (tl >= 2.0) rc = 6;
    if (tl >= 2.6) { T(420, 150, '5 -> andar', { c: C.good, size: 12 }); T(420, 320, '6 -> 429', { c: C.bad, size: 12 });
                     T(890, 280, 'andar: 5', { a: 'end', c: C.good, b: 1, size: 15 });
                     T(890, 305, '429: 1', { a: 'end', c: C.bad, b: 1, size: 13 }); }
  }
  T(705, 195, `ginti = ${rc}`, { c: C.acc, size: 14, b: 1 });
}
