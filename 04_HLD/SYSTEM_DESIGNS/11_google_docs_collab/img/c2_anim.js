// Card 2: HTTP me server khud nahi bhejta -> Ravi ko refresh pe dikha. Ilaaj: WebSocket khula, dono taraf.
const DUR = 16, FIX = 7.5;
const ME = [160, 185], SVR = [530, 185];
function setup() {
  wire('M160 185 H380'); wire('M530 185 H720');
  box(30, 150, 130, 70, 'Main', 'X likha');
  box(380, 150, 150, 70, 'Conn-Server');
  box(720, 150, 140, 70, 'Ravi');
}
function frame(t, fix, tl) {
  if (!fix) {
    CAP.textContent = 'DIKKAT: HTTP me server khud kuch nahi bhejta. X server pe aaya, Ravi ko refresh tak nahi';
    dot(path([[0.3, ME], [1.1, [368, 185]]], tl < 1.1 ? tl : -1), C.msg, 'X', 'POST');
    if (tl >= 1.1) T(455, 245, 'X aa gaya', { c: C.fg, size: 12 });
    if (tl >= 1.5 && tl < 2.2) dot(path([[1.5, SVR], [2.2, [625, 185]]], tl), C.msg, 'X');
    if (tl >= 2.2) { cross(625, 185); T(625, 140, 'khud nahi bhej sakta', { c: C.bad, b: 1, size: 12 }); }
    const got = tl >= 5.2;
    T(790, 250, got ? 'Ravi: HELLO X' : 'Ravi: HELLO', { c: got ? C.acc : C.bad, b: 1, size: 13 });
    if (!got && tl >= 2.2) T(790, 272, '(purana)', { c: C.bad, size: 12 });
    if (tl >= 3.4 && tl < 4.2) T(790, 135, 'refresh', { c: C.acc, b: 1, size: 13 });
    if (tl >= 3.6 && tl < 4.4) dot(path([[3.6, [708, 200]], [4.4, [542, 200]]], tl), C.acc, '', 'GET', C.acc);
    if (tl >= 4.4 && tl < 5.2) dot(path([[4.4, [542, 200]], [5.2, [708, 200]]], tl), C.acc, 'X');
    if (tl >= 5.2) T(790, 272, 'refresh ke baad hi', { c: C.acc, size: 12 });
    return;
  }
  CAP.textContent = 'ILAAJ: connection khula rehta, dono taraf (WebSocket). Server khud bhejta, ~200 ms me dikhta';
  el('path', { d: 'M160 185 H380 M530 185 H720', stroke: C.good, 'stroke-width': 4, fill: 'none' }, DYN);
  T(270, 172, 'WebSocket', { c: C.good, b: 1, size: 12 }); T(625, 172, 'WebSocket', { c: C.good, b: 1, size: 12 });
  T(455, 300, 'khula rehta, dono taraf', { c: C.good, size: 12 });
  dot(path([[0.3, ME], [1.0, [368, 185]]], tl < 1.0 ? tl : -1), C.msg, 'X');
  dot(path([[1.0, SVR], [1.5, [708, 185]]], tl > 1.0 && tl < 1.5 ? tl : -1), C.good, 'X', 'push', C.good);
  const got = tl >= 1.5;
  T(790, 250, got ? 'Ravi: HELLO X' : 'Ravi: HELLO', { c: got ? C.good : C.fg, b: 1, size: 13 });
  if (got) T(790, 272, '~200 ms, refresh nahi', { c: C.good, size: 12 });
}
