// Card 6: bot har 60 sec phir aata. Ilaaj: 429 -> Kafka -> Pattern Svc ginta -> WAF block list (edge pe).
const DUR = 19, FIX = 8;
const BOT = [70, 192], WAF = [210, 192], GW = [390, 192], K = [390, 310], P = [600, 310];
function setup() {
  wire('M120 192 H170'); wire('M250 192 H345'); wire('M435 192 H560');
  wire('M390 222 L390 285', 'wk'); wire('M435 310 H540', 'wp'); wire('M600 285 L600 250 L210 250 L210 222', 'ww');
  box(20, 150, 100, 84, 'Bot', 'roz, din bhar');
  box(170, 165, 80, 54, 'WAF', 'edge', { id: 'waf' });
  box(345, 162, 90, 60, 'Gateway', '429', { stroke: C.acc });
  box(560, 165, 110, 54, 'App');
  box(345, 285, 90, 50, 'Kafka', '', { id: 'kf' });
  box(540, 285, 130, 50, 'Pattern Svc', '', { id: 'ps' });
}
function frame(t, fix, tl) {
  ['kf', 'ps', 'wk', 'wp', 'ww'].forEach(id => vis(id, fix));
  if (!fix) {
    CAP.textContent = 'DIKKAT: 429 sirf bolta "60 sec baad aao". Bot 60 sec baad phir aata, din bhar';
    for (let k = 0; k < 4; k++) {
      const s = 0.3 + k * 1.8, p = path([[s, BOT], [s + 0.5, WAF], [s + 1.0, GW]], tl);
      if (tl < s + 1.6) dot(p, tl > s + 1.0 ? C.bad : C.msg, '', tl > s + 1.0 ? '429' : '');
    }
    const n = Math.min(4, Math.floor((tl - 0.3) / 1.8) + 1);
    T(70, 130, `${String(9 + n).padStart(2, '0')}:0${n} baje`, { c: C.muted, size: 12 });
    T(390, 130, 'kabhi nahi rukta', { c: C.bad, b: 1, size: 13 });
    return;
  }
  CAP.textContent = 'ILAAJ: har 429 Kafka me. Pattern Svc: 10 min me 500+ 429 -> WAF block list, edge pe hi roko';
  const banned = tl > 4.4;
  for (let k = 0; k < 6; k++) {
    const s = 0.3 + k * 0.8;
    if (s + 0.5 > 4.4 && banned) {
      const p = path([[s, BOT], [s + 0.5, WAF]], tl);
      if (tl < s + 1.1) dot(p, tl > s + 0.5 ? C.bad : C.msg, '', tl > s + 0.5 ? 'BAN' : '');
    } else {
      const p = path([[s, BOT], [s + 0.5, WAF], [s + 1.0, GW]], tl);
      if (tl < s + 1.4) dot(p, tl > s + 1.0 ? C.bad : C.msg, '', tl > s + 1.0 ? '429' : '');
      dot(path([[s + 1.0, GW], [s + 1.4, K], [s + 1.8, P]], tl < s + 1.8 ? tl : -1), C.acc);
    }
  }
  const cnt = Math.min(500, Math.round(Math.max(0, tl - 1.2) * 160));
  T(605, 355, `bot ke 429: ${cnt}`, { c: cnt >= 500 ? C.bad : C.acc, b: 1, size: 13 });
  if (banned) T(210, 145, 'block list: bot', { c: C.bad, b: 1, size: 13 });
}
