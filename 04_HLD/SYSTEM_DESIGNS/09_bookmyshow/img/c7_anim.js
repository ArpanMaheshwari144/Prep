// Card 7: ek hi App machine, giri -> booking band. Ilaaj: App stateless, hold DB me, LB health check -> App-2.
const DUR = 18, FIX = 8;
const M = [130, 185], LBL = [178, 185], LBR = [270, 185], A1L = [348, 90], A1R = [480, 90], A2L = [348, 280],
  A2R = [480, 280], DL = [638, 190];
function setup() {
  wire('M130 185 L360 90', 'wd');
  wire('M130 185 H190', 'w0'); wire('M270 185 L360 90', 'w1'); wire('M270 185 L360 280', 'w2');
  wire('M480 90 L650 180'); wire('M480 280 L650 200', 'w3');
  box(20, 150, 110, 70, 'Main', 'seat chun raha');
  box(190, 160, 80, 50, 'LB', '', { id: 'lb' });
  box(360, 60, 120, 60, 'App-1');
  box(360, 250, 120, 60, 'App-2', '', { id: 'a2' });
  box(650, 150, 200, 80, 'SQL DB', '', { stroke: C.acc });
}
function down(at, tl) {
  if (tl > at) {
    el('rect', { x: 360, y: 60, width: 120, height: 60, rx: 8, fill: C.bad, opacity: 0.3 }, DYN);
    cross(500, 70); T(420, 45, 'App-1 giri', { c: C.bad, b: 1, size: 13 });
  }
}
function frame(t, fix, tl) {
  ['lb', 'a2', 'w0', 'w2', 'w3'].forEach(id => vis(id, fix)); vis('wd', !fix); vis('w1', fix && tl < 4.2);
  if (tl > 1.6) T(750, 205, 'A1: HELD (Main)', { c: C.acc, b: 1, size: 13 });
  if (!fix) {
    CAP.textContent = 'DIKKAT: ek hi App machine. Shaam ko main seat chun raha tha, wo giri -> booking band';
    if (tl > 0.3 && tl < 1.6) dot(tl < 1.0 ? path([[0.3, M], [1.0, A1L]], tl) : path([[1.0, A1R], [1.6, DL]], tl), C.msg, '', 'hold A1', C.msg);
    down(2.5, tl);
    if (tl > 3.5) dot(path([[3.5, M], [4.3, [290, 115]]], tl), tl < 4.3 ? C.msg : C.bad, '', tl < 4.3 ? 'Pay' : 'koi nahi', tl < 4.3 ? C.msg : C.bad);
    if (tl > 4.6) T(75, 255, 'booking band', { c: C.bad, b: 1, size: 15 });
    return;
  }
  CAP.textContent = 'ILAAJ: App stateless, hold DB me. LB GET /health, 2-3 fail = bahar. App-2 wahi hold dekhti';
  if (tl > 0.3 && tl < 1.6) dot(tl < 0.6 ? path([[0.3, M], [0.6, LBL]], tl) : tl < 1.0 ? path([[0.6, LBR], [1.0, A1L]], tl)
    : path([[1.0, A1R], [1.6, DL]], tl), C.msg, '', 'hold A1', C.msg);
  down(2.0, tl);
  let fails = 0;
  for (let k = 0; k < 3; k++) {
    const s = 2.4 + k * 0.6;
    if (tl > s && tl < s + 0.4) dot(path([[s, LBR], [s + 0.4, [335, 108]]], tl), C.acc, '', 'GET /health', C.acc);
    if (tl >= s + 0.4) fails++;
  }
  if (tl > 2.4 && tl < 4.2) T(230, 235, `fail: ${fails}`, { c: C.bad, b: 1, size: 13 });
  if (tl >= 4.2) T(230, 235, 'App-1 bahar', { c: C.bad, b: 1, size: 13 });
  if (tl > 4.6 && tl < 6.8) {
    const p = tl < 4.9 ? path([[4.6, M], [4.9, LBL]], tl) : tl < 5.3 ? path([[4.9, LBR], [5.3, A2L]], tl)
      : tl < 5.9 ? path([[5.3, A2R], [5.9, [638, 205]]], tl) : tl < 6.4 ? path([[5.9, [638, 205]], [6.4, A2R]], tl)
      : path([[6.4, A2L], [6.8, LBR]], tl);
    dot(p, tl < 5.9 ? C.msg : C.good, '', tl < 5.9 ? 'Pay' : 'HELD mila', tl < 5.9 ? C.msg : C.good);
  }
  if (tl >= 6.8) dot(path([[6.8, LBL], [7.4, [75, 245]]], tl), C.good);
  if (tl >= 7.4) T(75, 275, 'seat wahin', { c: C.good, b: 1, size: 15 });
}
