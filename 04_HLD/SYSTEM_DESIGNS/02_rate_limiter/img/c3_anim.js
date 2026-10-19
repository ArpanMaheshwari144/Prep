// Card 3: 429 wali bhi App tak ghoomi, App bhar gaya. Ilaaj: limiter gateway pe, sabse aage.
const DUR = 16, FIX = 7.5, N = 14, GAP = 0.3;
const BOT = [110, 180], GW = [215, 192], LB = [390, 192], APP = [560, 192], RD = [740, 192];
function setup() {
  wire('M110 180 H170', 'w0'); wire('M170 180 H350', 'wbug'); wire('M260 180 H350'); wire('M430 180 H500'); wire('M620 180 H680', 'wapp');
  wire('M215 205 L215 290 L690 290 L740 225', 'wgw');
  box(20, 135, 90, 90, 'Bot', '10,000 / sec');
  box(170, 160, 90, 50, 'Gateway', '', { id: 'gw', stroke: C.acc });
  box(350, 160, 80, 50, 'LB');
  box(500, 140, 120, 80, 'App');
  box(680, 150, 130, 70, 'Redis', 'ginti');
}
function frame(t, fix, tl) {
  CAP.textContent = fix ? 'ILAAJ: limiter API Gateway pe, sabse aage. Had paar = wahin 429, LB / App tak nahi'
                        : 'DIKKAT: limiter App ke andar. 429 wali request bhi LB -> App tak ghoom ke aati';
  vis('gw', fix); vis('wbug', !fix); vis('wgw', fix); vis('wapp', !fix);
  let inApp = 0, blocked = 0;
  for (let i = 0; i < N; i++) {
    const s = 0.4 + i * GAP, okReq = i < 3;
    if (!fix) {
      const p = path([[s, BOT], [s + 0.5, LB], [s + 0.9, APP]], tl);
      if (tl >= s + 0.9) { inApp++; if (!okReq) blocked++; }
      if (p && tl < s + 1.5) dot(p, tl >= s + 0.9 ? (okReq ? C.good : C.bad) : C.msg, '', tl >= s + 0.9 && !okReq ? '429' : '');
    } else {
      const pts = okReq ? [[s, BOT], [s + 0.3, GW], [s + 0.8, LB], [s + 1.2, APP]] : [[s, BOT], [s + 0.3, GW]];
      const end = pts[pts.length - 1][0], p = path(pts, tl);
      if (tl >= end) { okReq ? inApp++ : blocked++; }
      if (p && tl < end + 0.6) dot(p, tl >= end ? (okReq ? C.good : C.bad) : C.msg, '', tl >= end && !okReq ? '429' : '');
    }
  }
  const load = fix ? inApp / N : inApp / N;
  bar(510, 240, 100, 16, load, 'App ka bojh');
  T(560, 280, fix ? `App tak: ${inApp}` : `App tak: ${inApp}`, { c: fix ? C.good : C.bad, b: 1, size: 13 });
  if (!fix && tl > 5) T(560, 300, 'App bhar gaya', { c: C.bad, b: 1, size: 13 });
  if (fix) T(215, 140, `wahin 429: ${blocked}`, { c: C.bad, b: 1, size: 13 });
}
